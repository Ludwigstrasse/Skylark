#include "Model/CamProjectJson.h"

#include <cctype>
#include <charconv>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <sstream>
#include <utility>

namespace skylark::cam
{
namespace
{
struct JsonValue
{
    enum class Kind { Null, Bool, Number, String, Object, Array } kind{Kind::Null};
    bool boolValue{false};
    double numberValue{0.0};
    std::string stringValue;
    std::map<std::string, JsonValue> objectValue;
    std::vector<JsonValue> arrayValue;

    const JsonValue* Find(const std::string& key) const
    {
        if (kind != Kind::Object) { return nullptr; }
        const auto it = objectValue.find(key);
        return it == objectValue.end() ? nullptr : &it->second;
    }
};

class JsonParser
{
public:
    explicit JsonParser(std::string_view text) : m_text(text) {}

    bool Parse(JsonValue& value, std::string& error)
    {
        SkipWhitespace();
        if (!ParseValue(value, error)) { return false; }
        SkipWhitespace();
        if (m_pos != m_text.size())
        {
            error = "Unexpected trailing JSON content.";
            return false;
        }
        return true;
    }

private:
    bool ParseValue(JsonValue& value, std::string& error)
    {
        SkipWhitespace();
        if (m_pos >= m_text.size())
        {
            error = "Unexpected end of JSON input.";
            return false;
        }

        const char ch = m_text[m_pos];
        if (ch == '{') { return ParseObject(value, error); }
        if (ch == '[') { return ParseArray(value, error); }
        if (ch == '"')
        {
            value.kind = JsonValue::Kind::String;
            return ParseString(value.stringValue, error);
        }
        if (ch == '-' || std::isdigit(static_cast<unsigned char>(ch))) { return ParseNumber(value, error); }
        if (MatchLiteral("true")) { value.kind = JsonValue::Kind::Bool; value.boolValue = true; return true; }
        if (MatchLiteral("false")) { value.kind = JsonValue::Kind::Bool; value.boolValue = false; return true; }
        if (MatchLiteral("null")) { value.kind = JsonValue::Kind::Null; return true; }

        error = "Unexpected JSON token.";
        return false;
    }

    bool ParseObject(JsonValue& value, std::string& error)
    {
        value = JsonValue{};
        value.kind = JsonValue::Kind::Object;
        ++m_pos;
        SkipWhitespace();
        if (Consume('}')) { return true; }

        while (m_pos < m_text.size())
        {
            std::string key;
            if (!ParseString(key, error)) { return false; }
            SkipWhitespace();
            if (!Consume(':'))
            {
                error = "Expected ':' after JSON object key.";
                return false;
            }
            JsonValue child;
            if (!ParseValue(child, error)) { return false; }
            value.objectValue.emplace(std::move(key), std::move(child));
            SkipWhitespace();
            if (Consume('}')) { return true; }
            if (!Consume(','))
            {
                error = "Expected ',' or '}' in JSON object.";
                return false;
            }
            SkipWhitespace();
        }

        error = "Unterminated JSON object.";
        return false;
    }

    bool ParseArray(JsonValue& value, std::string& error)
    {
        value = JsonValue{};
        value.kind = JsonValue::Kind::Array;
        ++m_pos;
        SkipWhitespace();
        if (Consume(']')) { return true; }

        while (m_pos < m_text.size())
        {
            JsonValue child;
            if (!ParseValue(child, error)) { return false; }
            value.arrayValue.push_back(std::move(child));
            SkipWhitespace();
            if (Consume(']')) { return true; }
            if (!Consume(','))
            {
                error = "Expected ',' or ']' in JSON array.";
                return false;
            }
            SkipWhitespace();
        }

        error = "Unterminated JSON array.";
        return false;
    }

    bool ParseString(std::string& value, std::string& error)
    {
        if (!Consume('"'))
        {
            error = "Expected JSON string.";
            return false;
        }

        value.clear();
        while (m_pos < m_text.size())
        {
            const char ch = m_text[m_pos++];
            if (ch == '"') { return true; }
            if (ch == '\\')
            {
                if (m_pos >= m_text.size())
                {
                    error = "Unterminated JSON escape sequence.";
                    return false;
                }
                const char escaped = m_text[m_pos++];
                switch (escaped)
                {
                case '"': value.push_back('"'); break;
                case '\\': value.push_back('\\'); break;
                case '/': value.push_back('/'); break;
                case 'b': value.push_back('\b'); break;
                case 'f': value.push_back('\f'); break;
                case 'n': value.push_back('\n'); break;
                case 'r': value.push_back('\r'); break;
                case 't': value.push_back('\t'); break;
                default:
                    error = "Unsupported JSON escape sequence.";
                    return false;
                }
            }
            else
            {
                value.push_back(ch);
            }
        }

        error = "Unterminated JSON string.";
        return false;
    }

    bool ParseNumber(JsonValue& value, std::string& error)
    {
        const std::size_t begin = m_pos;
        if (m_text[m_pos] == '-') { ++m_pos; }
        while (m_pos < m_text.size() && std::isdigit(static_cast<unsigned char>(m_text[m_pos]))) { ++m_pos; }
        if (m_pos < m_text.size() && m_text[m_pos] == '.')
        {
            ++m_pos;
            while (m_pos < m_text.size() && std::isdigit(static_cast<unsigned char>(m_text[m_pos]))) { ++m_pos; }
        }
        if (m_pos < m_text.size() && (m_text[m_pos] == 'e' || m_text[m_pos] == 'E'))
        {
            ++m_pos;
            if (m_pos < m_text.size() && (m_text[m_pos] == '+' || m_text[m_pos] == '-')) { ++m_pos; }
            while (m_pos < m_text.size() && std::isdigit(static_cast<unsigned char>(m_text[m_pos]))) { ++m_pos; }
        }

        const std::string number(m_text.substr(begin, m_pos - begin));
        char* endPtr = nullptr;
        const double parsed = std::strtod(number.c_str(), &endPtr);
        if (endPtr == number.c_str() || *endPtr != '\0')
        {
            error = "Invalid JSON number.";
            return false;
        }
        value.kind = JsonValue::Kind::Number;
        value.numberValue = parsed;
        return true;
    }

    bool Consume(char expected)
    {
        if (m_pos < m_text.size() && m_text[m_pos] == expected)
        {
            ++m_pos;
            return true;
        }
        return false;
    }

    bool MatchLiteral(std::string_view literal)
    {
        if (m_text.substr(m_pos, literal.size()) == literal)
        {
            m_pos += literal.size();
            return true;
        }
        return false;
    }

    void SkipWhitespace()
    {
        while (m_pos < m_text.size() && std::isspace(static_cast<unsigned char>(m_text[m_pos]))) { ++m_pos; }
    }

    std::string_view m_text;
    std::size_t m_pos{0};
};

std::string EscapeJson(const std::string& value)
{
    std::ostringstream out;
    for (char ch : value)
    {
        switch (ch)
        {
        case '"': out << "\\\""; break;
        case '\\': out << "\\\\"; break;
        case '\b': out << "\\b"; break;
        case '\f': out << "\\f"; break;
        case '\n': out << "\\n"; break;
        case '\r': out << "\\r"; break;
        case '\t': out << "\\t"; break;
        default: out << ch; break;
        }
    }
    return out.str();
}

void WriteIndent(std::ostringstream& out, int indent)
{
    for (int i = 0; i < indent; ++i) { out << ' '; }
}

void WriteVec3(std::ostringstream& out, const Vec3d& value)
{
    out << "{\"x\":" << value.x << ",\"y\":" << value.y << ",\"z\":" << value.z << '}';
}

void WriteBounds(std::ostringstream& out, const BoundingBox3d& box)
{
    out << "{\"valid\":" << (box.IsValid() ? "true" : "false") << ",\"min\":";
    WriteVec3(out, box.min);
    out << ",\"max\":";
    WriteVec3(out, box.max);
    out << '}';
}

const JsonValue* FindRequiredArray(const JsonValue& parent, const std::string& key, std::string& error)
{
    const JsonValue* value = parent.Find(key);
    if (value == nullptr || value->kind != JsonValue::Kind::Array)
    {
        error = "Missing array: " + key;
        return nullptr;
    }
    return value;
}

std::string ReadString(const JsonValue& parent, const std::string& key, const std::string& fallback)
{
    const JsonValue* value = parent.Find(key);
    return value != nullptr && value->kind == JsonValue::Kind::String ? value->stringValue : fallback;
}

bool ReadBool(const JsonValue& parent, const std::string& key, bool fallback)
{
    const JsonValue* value = parent.Find(key);
    return value != nullptr && value->kind == JsonValue::Kind::Bool ? value->boolValue : fallback;
}

double ReadNumber(const JsonValue& parent, const std::string& key, double fallback)
{
    const JsonValue* value = parent.Find(key);
    return value != nullptr && value->kind == JsonValue::Kind::Number ? value->numberValue : fallback;
}

std::uint64_t ReadUInt64(const JsonValue& parent, const std::string& key, std::uint64_t fallback)
{
    const double number = ReadNumber(parent, key, static_cast<double>(fallback));
    return number < 0.0 ? fallback : static_cast<std::uint64_t>(number);
}

std::uint32_t ReadUInt32(const JsonValue& parent, const std::string& key, std::uint32_t fallback)
{
    const double number = ReadNumber(parent, key, static_cast<double>(fallback));
    return number < 0.0 ? fallback : static_cast<std::uint32_t>(number);
}

int ReadInt(const JsonValue& parent, const std::string& key, int fallback)
{
    return static_cast<int>(ReadNumber(parent, key, static_cast<double>(fallback)));
}

Vec3d ReadVec3(const JsonValue& parent, const std::string& key, const Vec3d& fallback)
{
    const JsonValue* value = parent.Find(key);
    if (value == nullptr || value->kind != JsonValue::Kind::Object) { return fallback; }
    return {ReadNumber(*value, "x", fallback.x), ReadNumber(*value, "y", fallback.y), ReadNumber(*value, "z", fallback.z)};
}

BoundingBox3d ReadBounds(const JsonValue& parent, const std::string& key)
{
    BoundingBox3d box;
    const JsonValue* value = parent.Find(key);
    if (value == nullptr || value->kind != JsonValue::Kind::Object) { return box; }
    if (!ReadBool(*value, "valid", false)) { return box; }
    box.min = ReadVec3(*value, "min", box.min);
    box.max = ReadVec3(*value, "max", box.max);
    return box;
}

CamGeometryKind ReadGeometryKind(const std::string& text)
{
    if (text == "BRep") { return CamGeometryKind::BRep; }
    if (text == "Mesh") { return CamGeometryKind::Mesh; }
    if (text == "Assembly") { return CamGeometryKind::Assembly; }
    if (text == "Lightweight") { return CamGeometryKind::Lightweight; }
    return CamGeometryKind::Unknown;
}

CutterKind ReadCutterKind(const std::string& text)
{
    if (text == "Flat End Mill") { return CutterKind::FlatEndMill; }
    if (text == "Ball End Mill") { return CutterKind::BallEndMill; }
    if (text == "Bull Nose End Mill") { return CutterKind::BullNoseEndMill; }
    if (text == "Drill") { return CutterKind::Drill; }
    if (text == "Chamfer Mill") { return CutterKind::ChamferMill; }
    return CutterKind::FlatEndMill;
}

CamOperationType ReadOperationType(const std::string& text)
{
    if (text == "3D Roughing") { return CamOperationType::Roughing3D; }
    if (text == "Z-Level Finishing") { return CamOperationType::ZLevelFinishing; }
    if (text == "Parallel Finishing") { return CamOperationType::ParallelFinishing; }
    if (text == "Projection") { return CamOperationType::Projection; }
    if (text == "Pencil") { return CamOperationType::Pencil; }
    if (text == "Drilling") { return CamOperationType::Drilling; }
    if (text == "2.5D Contour") { return CamOperationType::Contour2D; }
    if (text == "2.5D Pocket") { return CamOperationType::Pocket2D; }
    return CamOperationType::Roughing3D;
}

OperationState ReadOperationState(const std::string& text)
{
    if (text == "Valid") { return OperationState::Valid; }
    if (text == "Dirty") { return OperationState::Dirty; }
    if (text == "Calculating") { return OperationState::Calculating; }
    if (text == "Failed") { return OperationState::Failed; }
    if (text == "Calculated") { return OperationState::Calculated; }
    if (text == "Suppressed") { return OperationState::Suppressed; }
    return OperationState::Draft;
}

ToolpathMoveKind ReadToolpathMoveKind(const std::string& text)
{
    if (text == "Rapid") { return ToolpathMoveKind::Rapid; }
    if (text == "Lead In") { return ToolpathMoveKind::LeadIn; }
    if (text == "Lead Out") { return ToolpathMoveKind::LeadOut; }
    if (text == "Retract") { return ToolpathMoveKind::Retract; }
    return ToolpathMoveKind::Feed;
}

ImportedModel ReadImportedModel(const JsonValue& value)
{
    ImportedModel model;
    model.id = ReadUInt64(value, "id", 0ULL);
    model.name = ReadString(value, "name", "Imported Model");
    model.sourcePath = ReadString(value, "sourcePath", "");
    model.geometryKind = ReadGeometryKind(ReadString(value, "geometryKind", "Unknown"));
    model.placeholder = ReadBool(value, "placeholder", false);
    model.assemblyRoot.name = ReadString(value, "assemblyRoot", model.name);

    if (const JsonValue* mesh = value.Find("meshPart"); mesh != nullptr && mesh->kind == JsonValue::Kind::Object)
    {
        model.meshPart.name = ReadString(*mesh, "name", model.name);
        model.meshPart.triangleCount = ReadUInt64(*mesh, "triangleCount", 0ULL);
        model.meshPart.chunkCount = ReadUInt32(*mesh, "chunkCount", 0U);
        model.meshPart.bvhReady = ReadBool(*mesh, "bvhReady", false);
        model.meshPart.lodReady = ReadBool(*mesh, "lodReady", false);
        model.meshPart.bvhNodeCount = ReadUInt32(*mesh, "bvhNodeCount", 0U);
        model.meshPart.lodLevelCount = ReadUInt32(*mesh, "lodLevelCount", 0U);
        model.meshPart.bounds = ReadBounds(*mesh, "bounds");
    }

    if (const JsonValue* brep = value.Find("brepPart"); brep != nullptr && brep->kind == JsonValue::Kind::Object)
    {
        model.brepPart.name = ReadString(*brep, "name", model.name);
        model.brepPart.bodyCount = ReadUInt32(*brep, "bodyCount", 0U);
        model.brepPart.faceCount = ReadUInt32(*brep, "faceCount", 0U);
        model.brepPart.edgeCount = ReadUInt32(*brep, "edgeCount", 0U);
        model.brepPart.bounds = ReadBounds(*brep, "bounds");
    }
    return model;
}

CamSetup ReadSetup(const JsonValue& value)
{
    CamSetup setup;
    setup.name = ReadString(value, "name", setup.name);
    if (const JsonValue* wcs = value.Find("wcs"); wcs != nullptr && wcs->kind == JsonValue::Kind::Object)
    {
        setup.wcs.name = ReadString(*wcs, "name", setup.wcs.name);
        setup.wcs.origin = ReadVec3(*wcs, "origin", setup.wcs.origin);
        setup.wcs.xAxis = ReadVec3(*wcs, "xAxis", setup.wcs.xAxis);
        setup.wcs.yAxis = ReadVec3(*wcs, "yAxis", setup.wcs.yAxis);
        setup.wcs.zAxis = ReadVec3(*wcs, "zAxis", setup.wcs.zAxis);
    }
    if (const JsonValue* stock = value.Find("stock"); stock != nullptr && stock->kind == JsonValue::Kind::Object)
    {
        setup.stock.name = ReadString(*stock, "name", setup.stock.name);
        setup.stock.sizeX = ReadNumber(*stock, "sizeX", setup.stock.sizeX);
        setup.stock.sizeY = ReadNumber(*stock, "sizeY", setup.stock.sizeY);
        setup.stock.sizeZ = ReadNumber(*stock, "sizeZ", setup.stock.sizeZ);
        setup.stock.offset = ReadNumber(*stock, "offset", setup.stock.offset);
    }
    if (const JsonValue* fixture = value.Find("fixture"); fixture != nullptr && fixture->kind == JsonValue::Kind::Object)
    {
        setup.fixture.name = ReadString(*fixture, "name", setup.fixture.name);
        setup.fixture.collisionEnabled = ReadBool(*fixture, "collisionEnabled", setup.fixture.collisionEnabled);
    }
    if (const JsonValue* machine = value.Find("machine"); machine != nullptr && machine->kind == JsonValue::Kind::Object)
    {
        setup.machine.name = ReadString(*machine, "name", setup.machine.name);
        setup.machine.axisCount = ReadInt(*machine, "axisCount", setup.machine.axisCount);
        setup.machine.maxFeed = ReadNumber(*machine, "maxFeed", setup.machine.maxFeed);
        setup.machine.maxSpindleRpm = ReadNumber(*machine, "maxSpindleRpm", setup.machine.maxSpindleRpm);
    }
    return setup;
}

ToolDefinition ReadTool(const JsonValue& value)
{
    ToolDefinition tool;
    tool.id = ReadUInt64(value, "id", 0ULL);
    tool.name = ReadString(value, "name", "Tool");
    tool.kind = ReadCutterKind(ReadString(value, "kind", "Flat End Mill"));
    tool.diameter = ReadNumber(value, "diameter", tool.diameter);
    tool.cornerRadius = ReadNumber(value, "cornerRadius", tool.cornerRadius);
    tool.fluteLength = ReadNumber(value, "fluteLength", tool.fluteLength);
    tool.gaugeLength = ReadNumber(value, "gaugeLength", tool.gaugeLength);
    return tool;
}

OperationNode ReadOperation(const JsonValue& value)
{
    OperationNode op;
    op.id = ReadUInt64(value, "id", 0ULL);
    op.name = ReadString(value, "name", "Operation");
    op.type = ReadOperationType(ReadString(value, "type", "3D Roughing"));
    op.state = ReadOperationState(ReadString(value, "state", "Draft"));
    op.suppressed = ReadBool(value, "suppressed", false);
    return op;
}

ToolpathDocument ReadToolpath(const JsonValue& value)
{
    ToolpathDocument doc;
    doc.id = ReadUInt64(value, "id", 0ULL);
    doc.name = ReadString(value, "name", "Toolpath");
    doc.visible = ReadBool(value, "visible", true);
    if (const JsonValue* moves = value.Find("moves"); moves != nullptr && moves->kind == JsonValue::Kind::Array)
    {
        for (const JsonValue& moveValue : moves->arrayValue)
        {
            if (moveValue.kind != JsonValue::Kind::Object) { continue; }
            ToolpathMove move;
            move.kind = ReadToolpathMoveKind(ReadString(moveValue, "kind", "Feed"));
            move.point = ReadVec3(moveValue, "point", {});
            move.feedRate = ReadNumber(moveValue, "feedRate", 1000.0);
            doc.moves.push_back(move);
        }
    }
    return doc;
}
}

std::string SerializeCamProjectToJson(const CamProject& project)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(6);
    out << "{\n";
    WriteIndent(out, 2); out << "\"schema\": \"SkylarkCamWorkbench.Project\",\n";
    WriteIndent(out, 2); out << "\"milestone\": \"M3\",\n";
    WriteIndent(out, 2); out << "\"name\": \"" << EscapeJson(project.GetName()) << "\",\n";

    WriteIndent(out, 2); out << "\"importedModels\": [\n";
    const auto& models = project.GetImportedModels();
    for (std::size_t i = 0; i < models.size(); ++i)
    {
        const ImportedModel& model = models[i];
        WriteIndent(out, 4); out << "{\n";
        WriteIndent(out, 6); out << "\"id\": " << model.id << ",\n";
        WriteIndent(out, 6); out << "\"name\": \"" << EscapeJson(model.name) << "\",\n";
        WriteIndent(out, 6); out << "\"sourcePath\": \"" << EscapeJson(model.sourcePath) << "\",\n";
        WriteIndent(out, 6); out << "\"geometryKind\": \"" << ToString(model.geometryKind) << "\",\n";
        WriteIndent(out, 6); out << "\"placeholder\": " << (model.placeholder ? "true" : "false") << ",\n";
        WriteIndent(out, 6); out << "\"assemblyRoot\": \"" << EscapeJson(model.assemblyRoot.name) << "\",\n";
        WriteIndent(out, 6); out << "\"meshPart\": {\"name\": \"" << EscapeJson(model.meshPart.name) << "\", \"triangleCount\": " << model.meshPart.triangleCount << ", \"chunkCount\": " << model.meshPart.chunkCount << ", \"bvhReady\": " << (model.meshPart.bvhReady ? "true" : "false") << ", \"lodReady\": " << (model.meshPart.lodReady ? "true" : "false") << ", \"bvhNodeCount\": " << model.meshPart.bvhNodeCount << ", \"lodLevelCount\": " << model.meshPart.lodLevelCount << ", \"bounds\": ";
        WriteBounds(out, model.meshPart.bounds);
        out << "},\n";
        WriteIndent(out, 6); out << "\"brepPart\": {\"name\": \"" << EscapeJson(model.brepPart.name) << "\", \"bodyCount\": " << model.brepPart.bodyCount << ", \"faceCount\": " << model.brepPart.faceCount << ", \"edgeCount\": " << model.brepPart.edgeCount << ", \"bounds\": ";
        WriteBounds(out, model.brepPart.bounds);
        out << "}\n";
        WriteIndent(out, 4); out << "}" << (i + 1U < models.size() ? "," : "") << "\n";
    }
    WriteIndent(out, 2); out << "],\n";

    WriteIndent(out, 2); out << "\"setups\": [\n";
    const auto& setups = project.GetSetups();
    for (std::size_t i = 0; i < setups.size(); ++i)
    {
        const CamSetup& setup = setups[i];
        WriteIndent(out, 4); out << "{\n";
        WriteIndent(out, 6); out << "\"name\": \"" << EscapeJson(setup.name) << "\",\n";
        WriteIndent(out, 6); out << "\"wcs\": {\"name\": \"" << EscapeJson(setup.wcs.name) << "\", \"origin\": "; WriteVec3(out, setup.wcs.origin); out << ", \"xAxis\": "; WriteVec3(out, setup.wcs.xAxis); out << ", \"yAxis\": "; WriteVec3(out, setup.wcs.yAxis); out << ", \"zAxis\": "; WriteVec3(out, setup.wcs.zAxis); out << "},\n";
        WriteIndent(out, 6); out << "\"stock\": {\"name\": \"" << EscapeJson(setup.stock.name) << "\", \"sizeX\": " << setup.stock.sizeX << ", \"sizeY\": " << setup.stock.sizeY << ", \"sizeZ\": " << setup.stock.sizeZ << ", \"offset\": " << setup.stock.offset << "},\n";
        WriteIndent(out, 6); out << "\"fixture\": {\"name\": \"" << EscapeJson(setup.fixture.name) << "\", \"collisionEnabled\": " << (setup.fixture.collisionEnabled ? "true" : "false") << "},\n";
        WriteIndent(out, 6); out << "\"machine\": {\"name\": \"" << EscapeJson(setup.machine.name) << "\", \"axisCount\": " << setup.machine.axisCount << ", \"maxFeed\": " << setup.machine.maxFeed << ", \"maxSpindleRpm\": " << setup.machine.maxSpindleRpm << "}\n";
        WriteIndent(out, 4); out << "}" << (i + 1U < setups.size() ? "," : "") << "\n";
    }
    WriteIndent(out, 2); out << "],\n";

    WriteIndent(out, 2); out << "\"toolLibrary\": [\n";
    const auto& tools = project.GetToolLibrary().GetTools();
    for (std::size_t i = 0; i < tools.size(); ++i)
    {
        const ToolDefinition& tool = tools[i];
        WriteIndent(out, 4); out << "{\"id\": " << tool.id << ", \"name\": \"" << EscapeJson(tool.name) << "\", \"kind\": \"" << ToString(tool.kind) << "\", \"diameter\": " << tool.diameter << ", \"cornerRadius\": " << tool.cornerRadius << ", \"fluteLength\": " << tool.fluteLength << ", \"gaugeLength\": " << tool.gaugeLength << "}" << (i + 1U < tools.size() ? "," : "") << "\n";
    }
    WriteIndent(out, 2); out << "],\n";

    WriteIndent(out, 2); out << "\"operationTree\": [\n";
    const auto& operations = project.GetOperationTree().GetOperations();
    for (std::size_t i = 0; i < operations.size(); ++i)
    {
        const OperationNode& op = operations[i];
        WriteIndent(out, 4); out << "{\"id\": " << op.id << ", \"name\": \"" << EscapeJson(op.name) << "\", \"type\": \"" << ToString(op.type) << "\", \"state\": \"" << ToString(op.state) << "\", \"suppressed\": " << (op.suppressed ? "true" : "false") << "}" << (i + 1U < operations.size() ? "," : "") << "\n";
    }
    WriteIndent(out, 2); out << "],\n";

    WriteIndent(out, 2); out << "\"toolpathDocuments\": [\n";
    const auto& documents = project.GetToolpathDocuments();
    for (std::size_t i = 0; i < documents.size(); ++i)
    {
        const ToolpathDocument& document = documents[i];
        WriteIndent(out, 4); out << "{\"id\": " << document.id << ", \"name\": \"" << EscapeJson(document.name) << "\", \"visible\": " << (document.visible ? "true" : "false") << ", \"moves\": [";
        for (std::size_t m = 0; m < document.moves.size(); ++m)
        {
            const ToolpathMove& move = document.moves[m];
            out << "{\"kind\": \"" << ToString(move.kind) << "\", \"point\": ";
            WriteVec3(out, move.point);
            out << ", \"feedRate\": " << move.feedRate << "}" << (m + 1U < document.moves.size() ? ", " : "");
        }
        out << "]}" << (i + 1U < documents.size() ? "," : "") << "\n";
    }
    WriteIndent(out, 2); out << "],\n";

    WriteIndent(out, 2); out << "\"postConfig\": {\"activePost\": \"" << EscapeJson(project.GetPostConfig().activePost) << "\", \"ncExtension\": \"" << EscapeJson(project.GetPostConfig().ncExtension) << "\"},\n";
    WriteIndent(out, 2); out << "\"simulationSettings\": {\"enableToolAnimation\": " << (project.GetSimulationSettings().enableToolAnimation ? "true" : "false") << ", \"enableCollisionCheck\": " << (project.GetSimulationSettings().enableCollisionCheck ? "true" : "false") << ", \"enableStockRemoval\": " << (project.GetSimulationSettings().enableStockRemoval ? "true" : "false") << "}\n";
    out << "}\n";
    return out.str();
}

CamProjectJsonResult DeserializeCamProjectFromJson(std::string_view jsonText)
{
    CamProjectJsonResult result;
    JsonParser parser(jsonText);
    JsonValue root;
    std::string error;
    if (!parser.Parse(root, error))
    {
        result.message = error;
        return result;
    }
    if (root.kind != JsonValue::Kind::Object)
    {
        result.message = "Project JSON root must be an object.";
        return result;
    }
    if (ReadString(root, "schema", "") != "SkylarkCamWorkbench.Project")
    {
        result.message = "Unsupported project JSON schema.";
        return result;
    }

    CamProject project;
    project.SetName(ReadString(root, "name", "Untitled CAM Project"));

    const JsonValue* models = FindRequiredArray(root, "importedModels", error);
    if (models == nullptr) { result.message = error; return result; }
    for (const JsonValue& modelValue : models->arrayValue)
    {
        if (modelValue.kind == JsonValue::Kind::Object) { project.AddImportedModel(ReadImportedModel(modelValue)); }
    }

    const JsonValue* setups = FindRequiredArray(root, "setups", error);
    if (setups == nullptr) { result.message = error; return result; }
    for (const JsonValue& setupValue : setups->arrayValue)
    {
        if (setupValue.kind == JsonValue::Kind::Object) { project.AddSetup(ReadSetup(setupValue)); }
    }

    const JsonValue* tools = FindRequiredArray(root, "toolLibrary", error);
    if (tools == nullptr) { result.message = error; return result; }
    for (const JsonValue& toolValue : tools->arrayValue)
    {
        if (toolValue.kind == JsonValue::Kind::Object) { project.GetToolLibrary().AddTool(ReadTool(toolValue)); }
    }

    const JsonValue* operations = FindRequiredArray(root, "operationTree", error);
    if (operations == nullptr) { result.message = error; return result; }
    for (const JsonValue& opValue : operations->arrayValue)
    {
        if (opValue.kind == JsonValue::Kind::Object) { project.GetOperationTree().AddOperation(ReadOperation(opValue)); }
    }

    if (const JsonValue* documents = root.Find("toolpathDocuments"); documents != nullptr && documents->kind == JsonValue::Kind::Array)
    {
        for (const JsonValue& docValue : documents->arrayValue)
        {
            if (docValue.kind == JsonValue::Kind::Object) { project.AddToolpathDocument(ReadToolpath(docValue)); }
        }
    }

    if (const JsonValue* post = root.Find("postConfig"); post != nullptr && post->kind == JsonValue::Kind::Object)
    {
        PostProcessorConfig config;
        config.activePost = ReadString(*post, "activePost", config.activePost);
        config.ncExtension = ReadString(*post, "ncExtension", config.ncExtension);
        project.SetPostConfig(config);
    }

    if (const JsonValue* sim = root.Find("simulationSettings"); sim != nullptr && sim->kind == JsonValue::Kind::Object)
    {
        SimulationSettings settings;
        settings.enableToolAnimation = ReadBool(*sim, "enableToolAnimation", settings.enableToolAnimation);
        settings.enableCollisionCheck = ReadBool(*sim, "enableCollisionCheck", settings.enableCollisionCheck);
        settings.enableStockRemoval = ReadBool(*sim, "enableStockRemoval", settings.enableStockRemoval);
        project.SetSimulationSettings(settings);
    }

    result.success = true;
    result.message = "Project JSON loaded.";
    result.project = std::move(project);
    return result;
}

bool SaveCamProjectJson(const std::filesystem::path& path, const CamProject& project, std::string& message)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file)
    {
        message = "Failed to open project JSON for writing: " + path.string();
        return false;
    }
    const std::string json = SerializeCamProjectToJson(project);
    file.write(json.data(), static_cast<std::streamsize>(json.size()));
    if (!file)
    {
        message = "Failed to write project JSON: " + path.string();
        return false;
    }
    message = "Project JSON saved: " + path.string();
    return true;
}

CamProjectJsonResult LoadCamProjectJson(const std::filesystem::path& path)
{
    CamProjectJsonResult result;
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        result.message = "Failed to open project JSON for reading: " + path.string();
        return result;
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    result = DeserializeCamProjectFromJson(buffer.str());
    if (result.success) { result.message = "Project JSON loaded: " + path.string(); }
    return result;
}
}
