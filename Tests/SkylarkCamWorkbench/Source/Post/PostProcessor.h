#pragma once

#include "Model/CamProject.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace skylark::cam
{
struct PostDescriptor
{
    std::string id;
    std::string name;
    bool availableInM0{false};
    bool availableInM6{false};
};

struct PostProcessorOptions
{
    std::string programName{"O1001"};
    std::string setupName;
    std::string commentPrefix{"("};
    bool useLineNumbers{true};
    std::uint32_t firstLineNumber{10};
    std::uint32_t lineNumberStep{10};
    double spindleRpm{8000.0};
    double defaultFeedRate{1200.0};
    double safeZ{60.0};
    bool emitPercentWrapper{true};
};

struct NcBlock
{
    std::uint32_t lineNumber{0};
    std::string text;
};

struct GCodeProgram
{
    bool generated{false};
    std::string postId;
    std::string postName;
    std::string programName;
    std::vector<NcBlock> blocks;
    std::vector<std::string> warnings;
    std::string text;
};

struct OperationSheet
{
    bool generated{false};
    std::string title;
    std::vector<std::string> lines;
    std::string text;
};

class IPostProcessor
{
public:
    virtual ~IPostProcessor() = default;
    virtual std::string GetId() const = 0;
    virtual std::string GetName() const = 0;
    virtual GCodeProgram Generate(const CamProject& project, const PostProcessorOptions& options) const = 0;
};

class GenericIsoPost final : public IPostProcessor
{
public:
    std::string GetId() const override;
    std::string GetName() const override;
    GCodeProgram Generate(const CamProject& project, const PostProcessorOptions& options) const override;
};

class Fanuc3AxisPost final : public IPostProcessor
{
public:
    std::string GetId() const override;
    std::string GetName() const override;
    GCodeProgram Generate(const CamProject& project, const PostProcessorOptions& options) const override;
};

std::vector<PostDescriptor> CreateM0PostProcessorList();
std::vector<PostDescriptor> CreateM6PostProcessorList();
GCodeProgram GenerateGenericIsoGCode(const CamProject& project, const PostProcessorOptions& options = {});
GCodeProgram GenerateFanuc3AxisGCode(const CamProject& project, const PostProcessorOptions& options = {});
std::string GenerateNcPreview(const GCodeProgram& program, std::size_t maxBlocks = 200U);
OperationSheet GenerateOperationSheet(const CamProject& project, const GCodeProgram* program = nullptr);
bool ExportGCodeProgram(const GCodeProgram& program, const std::filesystem::path& path, std::string& message);
}
