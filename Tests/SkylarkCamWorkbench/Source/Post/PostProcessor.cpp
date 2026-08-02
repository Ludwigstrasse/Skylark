#include "Post/PostProcessor.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <system_error>
#include <utility>

namespace skylark::cam
{
namespace
{
std::string FormatDouble(double value)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3) << value;
    std::string text = stream.str();
    while (!text.empty() && text.back() == '0') { text.pop_back(); }
    if (!text.empty() && text.back() == '.') { text.pop_back(); }
    if (text == "-0") { return "0"; }
    return text;
}

std::string SanitizeProgramName(std::string value)
{
    if (value.empty()) { return "O1001"; }
    std::string result;
    result.reserve(value.size());
    for (char ch : value)
    {
        if (std::isalnum(static_cast<unsigned char>(ch))) { result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch)))); }
    }
    if (result.empty()) { result = "O1001"; }
    if (result.front() != 'O') { result.insert(result.begin(), 'O'); }
    return result;
}

const CamSetup* SelectSetup(const CamProject& project, const PostProcessorOptions& options)
{
    if (!options.setupName.empty())
    {
        for (const CamSetup& setup : project.GetSetups())
        {
            if (setup.name == options.setupName) { return &setup; }
        }
    }
    if (!project.GetSetups().empty()) { return &project.GetSetups().back(); }
    return nullptr;
}

const ToolDefinition* SelectTool(const CamProject& project)
{
    const auto& tools = project.GetToolLibrary().GetTools();
    if (tools.empty()) { return nullptr; }
    for (const ToolDefinition& tool : tools)
    {
        if (tool.kind == CutterKind::BallEndMill) { return &tool; }
    }
    return &tools.front();
}

std::vector<const ToolpathDocument*> CollectVisibleToolpaths(const CamProject& project)
{
    std::vector<const ToolpathDocument*> result;
    for (const ToolpathDocument& document : project.GetToolpathDocuments())
    {
        if (document.visible && !document.moves.empty()) { result.push_back(&document); }
    }
    return result;
}

struct NcEmitter
{
    GCodeProgram program;
    PostProcessorOptions options;
    std::uint32_t nextLine{10};

    explicit NcEmitter(PostProcessorOptions value) : options(std::move(value)), nextLine(options.firstLineNumber) {}

    void AddRaw(std::string text)
    {
        program.blocks.push_back({0U, std::move(text)});
    }

    void Add(std::string text)
    {
        NcBlock block;
        block.lineNumber = options.useLineNumbers ? nextLine : 0U;
        block.text = std::move(text);
        if (options.useLineNumbers) { nextLine += options.lineNumberStep; }
        program.blocks.push_back(std::move(block));
    }

    void Comment(const std::string& text)
    {
        Add("(" + text + ")");
    }

    std::string BuildText() const
    {
        std::ostringstream stream;
        for (const NcBlock& block : program.blocks)
        {
            if (block.lineNumber > 0U) { stream << 'N' << block.lineNumber << ' '; }
            stream << block.text << '\n';
        }
        return stream.str();
    }
};

std::string MoveToGCode(const ToolpathMove& move)
{
    std::ostringstream stream;
    switch (move.kind)
    {
    case ToolpathMoveKind::Rapid:
    case ToolpathMoveKind::Retract:
        stream << "G0";
        break;
    case ToolpathMoveKind::Feed:
    case ToolpathMoveKind::LeadIn:
    case ToolpathMoveKind::LeadOut:
        stream << "G1";
        break;
    }
    stream << " X" << FormatDouble(move.point.x)
           << " Y" << FormatDouble(move.point.y)
           << " Z" << FormatDouble(move.point.z);
    if (move.kind == ToolpathMoveKind::Feed || move.kind == ToolpathMoveKind::LeadIn || move.kind == ToolpathMoveKind::LeadOut)
    {
        stream << " F" << FormatDouble(move.feedRate);
    }
    return stream.str();
}

void EmitToolpathDocuments(NcEmitter& emitter, const std::vector<const ToolpathDocument*>& toolpaths)
{
    for (const ToolpathDocument* document : toolpaths)
    {
        emitter.Comment("TOOLPATH " + document->name);
        ToolpathMoveKind previousKind = ToolpathMoveKind::Rapid;
        bool hasPrevious = false;
        for (const ToolpathMove& move : document->moves)
        {
            if (!hasPrevious || previousKind != move.kind)
            {
                emitter.Comment("MOVE KIND " + ToString(move.kind));
                previousKind = move.kind;
                hasPrevious = true;
            }
            emitter.Add(MoveToGCode(move));
        }
    }
}

GCodeProgram GenerateProgram(
    const CamProject& project,
    PostProcessorOptions options,
    const std::string& postId,
    const std::string& postName,
    bool fanucMode)
{
    NcEmitter emitter(std::move(options));
    emitter.program.generated = false;
    emitter.program.postId = postId;
    emitter.program.postName = postName;
    emitter.program.programName = SanitizeProgramName(emitter.options.programName);

    const CamSetup* setup = SelectSetup(project, emitter.options);
    const ToolDefinition* tool = SelectTool(project);
    const std::vector<const ToolpathDocument*> toolpaths = CollectVisibleToolpaths(project);

    if (setup == nullptr) { emitter.program.warnings.push_back("No setup exists. Create Setup before post processing."); }
    if (tool == nullptr) { emitter.program.warnings.push_back("No tool exists. Add a ToolLibrary entry before post processing."); }
    if (toolpaths.empty()) { emitter.program.warnings.push_back("No visible ToolpathDocument exists. Calculate M4 toolpath before post processing."); }

    if (emitter.options.emitPercentWrapper) { emitter.AddRaw("%"); }
    emitter.Add(emitter.program.programName);
    emitter.Comment("POST " + postName);
    emitter.Comment("PROJECT " + project.GetName());
    if (setup != nullptr)
    {
        emitter.Comment("SETUP " + setup->name);
        emitter.Comment("WCS " + setup->wcs.name + " ORIGIN X" + FormatDouble(setup->wcs.origin.x) + " Y" + FormatDouble(setup->wcs.origin.y) + " Z" + FormatDouble(setup->wcs.origin.z));
        emitter.Comment("STOCK " + setup->stock.name + " " + FormatDouble(setup->stock.sizeX) + " x " + FormatDouble(setup->stock.sizeY) + " x " + FormatDouble(setup->stock.sizeZ));
        emitter.Comment("FIXTURE " + setup->fixture.name);
        emitter.Comment("MACHINE " + setup->machine.name);
    }
    if (tool != nullptr)
    {
        emitter.Comment("TOOL " + tool->name + " D" + FormatDouble(tool->diameter) + " GAUGE " + FormatDouble(tool->gaugeLength));
    }

    emitter.Add("G21 G90 G17");
    emitter.Add("G40 G49 G80");
    emitter.Add("G54");
    if (tool != nullptr)
    {
        emitter.Add("T" + std::to_string(tool->id) + " M6");
    }
    emitter.Add("S" + FormatDouble(emitter.options.spindleRpm) + " M3");
    emitter.Add("G0 Z" + FormatDouble(emitter.options.safeZ));
    if (fanucMode)
    {
        emitter.Add("G43 H" + std::to_string(tool != nullptr ? tool->id : 1ULL) + " Z" + FormatDouble(emitter.options.safeZ));
    }

    EmitToolpathDocuments(emitter, toolpaths);

    emitter.Add("G0 Z" + FormatDouble(emitter.options.safeZ));
    emitter.Add("M5");
    emitter.Add("G91 G28 Z0");
    emitter.Add("G90");
    emitter.Add("M30");
    if (emitter.options.emitPercentWrapper) { emitter.AddRaw("%"); }

    emitter.program.generated = emitter.program.warnings.empty();
    emitter.program.text = emitter.BuildText();
    return emitter.program;
}

std::string OperationTypeStats(const CamProject& project)
{
    std::ostringstream stream;
    bool first = true;
    for (const OperationNode& operation : project.GetOperationTree().GetOperations())
    {
        if (!first) { stream << ", "; }
        first = false;
        stream << operation.name << " [" << ToString(operation.type) << '/' << ToString(operation.state) << ']';
    }
    return stream.str();
}
}

std::string GenericIsoPost::GetId() const { return "generic_iso"; }
std::string GenericIsoPost::GetName() const { return "Generic ISO 3-Axis"; }
GCodeProgram GenericIsoPost::Generate(const CamProject& project, const PostProcessorOptions& options) const
{
    return GenerateProgram(project, options, GetId(), GetName(), false);
}

std::string Fanuc3AxisPost::GetId() const { return "fanuc_3x"; }
std::string Fanuc3AxisPost::GetName() const { return "FANUC 3-Axis"; }
GCodeProgram Fanuc3AxisPost::Generate(const CamProject& project, const PostProcessorOptions& options) const
{
    return GenerateProgram(project, options, GetId(), GetName(), true);
}

std::vector<PostDescriptor> CreateM0PostProcessorList()
{
    return {
        {"generic_iso", "Generic ISO 3-Axis", false, true},
        {"fanuc_3x", "FANUC 3-Axis", false, true},
        {"siemens_3x", "Siemens 3-Axis", false, false}};
}

std::vector<PostDescriptor> CreateM6PostProcessorList()
{
    return {
        {"generic_iso", "Generic ISO 3-Axis", true, true},
        {"fanuc_3x", "FANUC 3-Axis", true, true},
        {"siemens_3x", "Siemens 3-Axis Stub", false, false}};
}

GCodeProgram GenerateGenericIsoGCode(const CamProject& project, const PostProcessorOptions& options)
{
    GenericIsoPost post;
    return post.Generate(project, options);
}

GCodeProgram GenerateFanuc3AxisGCode(const CamProject& project, const PostProcessorOptions& options)
{
    Fanuc3AxisPost post;
    return post.Generate(project, options);
}

std::string GenerateNcPreview(const GCodeProgram& program, std::size_t maxBlocks)
{
    std::ostringstream stream;
    stream << "NC Preview - " << program.postName << " / " << program.programName << '\n';
    stream << "Generated: " << (program.generated ? "yes" : "with warnings") << '\n';
    if (!program.warnings.empty())
    {
        stream << "Warnings:" << '\n';
        for (const std::string& warning : program.warnings) { stream << "- " << warning << '\n'; }
    }
    stream << "--- Blocks ---" << '\n';
    const std::size_t count = std::min(maxBlocks, program.blocks.size());
    for (std::size_t i = 0; i < count; ++i)
    {
        const NcBlock& block = program.blocks[i];
        if (block.lineNumber > 0U) { stream << 'N' << block.lineNumber << ' '; }
        stream << block.text << '\n';
    }
    if (program.blocks.size() > count)
    {
        stream << "... " << (program.blocks.size() - count) << " block(s) omitted from preview" << '\n';
    }
    return stream.str();
}

OperationSheet GenerateOperationSheet(const CamProject& project, const GCodeProgram* program)
{
    OperationSheet sheet;
    sheet.generated = true;
    sheet.title = "Skylark CAM Operation Sheet";

    auto add = [&](std::string line) { sheet.lines.push_back(std::move(line)); };
    add(sheet.title);
    add("Project: " + project.GetName());
    add("Setups: " + std::to_string(project.GetSetups().size()));
    for (const CamSetup& setup : project.GetSetups())
    {
        add("  Setup: " + setup.name);
        add("    WCS: " + setup.wcs.name + " origin=(" + FormatDouble(setup.wcs.origin.x) + ", " + FormatDouble(setup.wcs.origin.y) + ", " + FormatDouble(setup.wcs.origin.z) + ")");
        add("    Stock: " + setup.stock.name + " " + FormatDouble(setup.stock.sizeX) + " x " + FormatDouble(setup.stock.sizeY) + " x " + FormatDouble(setup.stock.sizeZ));
        add("    Fixture: " + setup.fixture.name + (setup.fixture.collisionEnabled ? " [collision enabled]" : " [collision disabled]"));
        add("    Machine: " + setup.machine.name + " axes=" + std::to_string(setup.machine.axisCount));
    }

    add("Tools: " + std::to_string(project.GetToolLibrary().GetTools().size()));
    for (const ToolDefinition& tool : project.GetToolLibrary().GetTools())
    {
        add("  T" + std::to_string(tool.id) + " " + tool.name + " kind=" + ToString(tool.kind) + " D" + FormatDouble(tool.diameter) + " gauge=" + FormatDouble(tool.gaugeLength));
    }

    add("Operations: " + std::to_string(project.GetOperationTree().GetOperations().size()));
    add("  " + OperationTypeStats(project));

    std::size_t totalMoves = 0U;
    std::size_t feedMoves = 0U;
    BoundingBox3d toolpathBounds;
    for (const ToolpathDocument& document : project.GetToolpathDocuments())
    {
        totalMoves += document.moves.size();
        for (const ToolpathMove& move : document.moves)
        {
            if (move.kind == ToolpathMoveKind::Feed) { ++feedMoves; }
            toolpathBounds.Expand(move.point);
        }
    }
    add("Toolpaths: " + std::to_string(project.GetToolpathDocuments().size()));
    add("  Total moves: " + std::to_string(totalMoves));
    add("  Feed moves: " + std::to_string(feedMoves));
    if (toolpathBounds.IsValid())
    {
        const Vec3d size = toolpathBounds.Size();
        add("  Toolpath bounds size: " + FormatDouble(size.x) + " x " + FormatDouble(size.y) + " x " + FormatDouble(size.z));
    }

    if (program != nullptr)
    {
        add("NC Program: " + program->programName);
        add("  Post: " + program->postName);
        add("  Blocks: " + std::to_string(program->blocks.size()));
        add("  Generated: " + std::string(program->generated ? "yes" : "with warnings"));
        for (const std::string& warning : program->warnings) { add("  Warning: " + warning); }
    }

    std::ostringstream stream;
    for (const std::string& line : sheet.lines) { stream << line << '\n'; }
    sheet.text = stream.str();
    return sheet;
}

bool ExportGCodeProgram(const GCodeProgram& program, const std::filesystem::path& path, std::string& message)
{
    if (program.text.empty())
    {
        message = "Cannot export empty G-code program.";
        return false;
    }

    std::error_code ec;
    const std::filesystem::path parent = path.parent_path();
    if (!parent.empty()) { std::filesystem::create_directories(parent, ec); }
    if (ec)
    {
        message = "Cannot create NC output directory: " + ec.message();
        return false;
    }

    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output)
    {
        message = "Cannot open NC output file: " + path.string();
        return false;
    }

    output << program.text;
    if (!output.good())
    {
        message = "Failed while writing NC output file: " + path.string();
        return false;
    }

    message = "Exported NC program: " + path.string();
    return true;
}
}
