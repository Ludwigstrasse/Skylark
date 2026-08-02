#include "Core/MemoryInfo.h"
#include "Import/ImporterRegistry.h"
#include "Import/Stl/StlImporter.h"
#include "Report/JsonReportWriter.h"
#include "Skylark/SkylarkMeshAdapter.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#if SKFW_WITH_SKYLARK
    #include "SKGeometry/SKGeometryRegistry.h"
#endif

using namespace Skylark::FormatWorkbench;

namespace
{
    struct CliOptions final
    {
        std::filesystem::path InputPath;
        std::filesystem::path ReportPath;
        std::filesystem::path GeneratePath;
        std::uint64_t GenerateTriangles = 0;
        std::uint64_t MaxPreviewTriangles = 100000;
        std::uint32_t TrianglesPerBlock = 1000000;
        bool DisableSkylarkPreview = false;
        bool ShowHelp = false;
    };

    static void PrintHelp()
    {
        std::cout
            << "SkylarkFormatWorkbenchCli\n"
            << "  --input <file.stl>\n"
            << "  --report <report.json>\n"
            << "  --generate-binary-stl <file.stl> --triangles <count>\n"
            << "  --triangles-per-block <count>\n"
            << "  --max-preview-triangles <count>\n"
            << "  --no-skylark-preview\n";
    }

    static bool ParseUInt64(const char* Text, std::uint64_t& Value)
    {
        char* End = nullptr;
        unsigned long long Parsed = std::strtoull(Text, &End, 10);
        if (End == Text || *End != '\0')
            return false;
        Value = static_cast<std::uint64_t>(Parsed);
        return true;
    }

    static bool ParseArgs(int Argc, char** Argv, CliOptions& Options)
    {
        for (int I = 1; I < Argc; ++I)
        {
            const std::string Arg = Argv[I];
            auto NeedValue = [&](const char* Name) -> const char*
            {
                if (I + 1 >= Argc)
                {
                    std::cerr << "Missing value after " << Name << "\n";
                    return nullptr;
                }
                return Argv[++I];
            };

            if (Arg == "--help" || Arg == "-h")
                Options.ShowHelp = true;
            else if (Arg == "--input")
            {
                const char* Value = NeedValue("--input");
                if (!Value) return false;
                Options.InputPath = Value;
            }
            else if (Arg == "--report")
            {
                const char* Value = NeedValue("--report");
                if (!Value) return false;
                Options.ReportPath = Value;
            }
            else if (Arg == "--generate-binary-stl")
            {
                const char* Value = NeedValue("--generate-binary-stl");
                if (!Value) return false;
                Options.GeneratePath = Value;
            }
            else if (Arg == "--triangles")
            {
                const char* Value = NeedValue("--triangles");
                if (!Value || !ParseUInt64(Value, Options.GenerateTriangles)) return false;
            }
            else if (Arg == "--triangles-per-block")
            {
                const char* Value = NeedValue("--triangles-per-block");
                std::uint64_t Parsed = 0;
                if (!Value || !ParseUInt64(Value, Parsed) || Parsed == 0 || Parsed > 0xFFFFFFFFull) return false;
                Options.TrianglesPerBlock = static_cast<std::uint32_t>(Parsed);
            }
            else if (Arg == "--max-preview-triangles")
            {
                const char* Value = NeedValue("--max-preview-triangles");
                if (!Value || !ParseUInt64(Value, Options.MaxPreviewTriangles)) return false;
            }
            else if (Arg == "--no-skylark-preview")
                Options.DisableSkylarkPreview = true;
            else
            {
                std::cerr << "Unknown argument: " << Arg << "\n";
                return false;
            }
        }
        return true;
    }

    static void PrintReport(const FwImportReport& Report)
    {
        char Buffer[64]{};
        std::cout << "============================================================\n";
        std::cout << "Skylark Format Workbench Import Report\n";
        std::cout << "============================================================\n";
        std::cout << "Status           : " << (Report.Succeeded ? "PASS" : "FAIL") << "\n";
        std::cout << "Format           : " << ToString(Report.Format) << "\n";
        std::cout << "File bytes       : " << Report.FileBytes << "\n";
        std::cout << "Triangles        : " << Report.Triangles << "\n";
        std::cout << "Vertices         : " << Report.Vertices << "\n";
        std::cout << "Mesh blocks      : " << Report.MeshBlocks << "\n";
        std::cout << "Compact CPU bytes: " << Report.CompactCpuBytes << " (" << FormatBytesHumanReadable(Report.CompactCpuBytes, Buffer, sizeof(Buffer)) << ")\n";
        std::cout << "Import time      : " << Report.ImportMilliseconds << " ms\n";
        std::cout << "RSS after import : " << Report.MemoryAfter.ResidentBytes << " (" << FormatBytesHumanReadable(Report.MemoryAfter.ResidentBytes, Buffer, sizeof(Buffer)) << ")\n";
        for (const FwImportMessage& Message : Report.Messages)
        {
            std::cout << "Message          : " << Message.Text << "\n";
        }
    }
}

int main(int Argc, char** Argv)
{
    CliOptions Cli;
    if (!ParseArgs(Argc, Argv, Cli))
    {
        PrintHelp();
        return 2;
    }
    if (Cli.ShowHelp)
    {
        PrintHelp();
        return 0;
    }

    if (!Cli.GeneratePath.empty())
    {
        if (Cli.GenerateTriangles == 0)
        {
            std::cerr << "--triangles must be greater than zero when generating STL.\n";
            return 2;
        }
        std::string Error;
        if (!GenerateBinaryStlFile(Cli.GeneratePath, Cli.GenerateTriangles, Error))
        {
            std::cerr << Error << "\n";
            return 1;
        }
        std::cout << "Generated binary STL: " << Cli.GeneratePath.string() << " triangles=" << Cli.GenerateTriangles << "\n";
    }

    if (Cli.InputPath.empty())
        return Cli.GeneratePath.empty() ? 2 : 0;

    FwImportOptions ImportOptions;
    ImportOptions.TrianglesPerBlock = Cli.TrianglesPerBlock;
    ImportOptions.MaxSkylarkPreviewTriangles = Cli.MaxPreviewTriangles;
    ImportOptions.BuildSkylarkPreview = !Cli.DisableSkylarkPreview;

    FwImporterRegistry Registry = CreateDefaultImporterRegistry();
    FwImportResult Result = Registry.Import(Cli.InputPath, ImportOptions, [](double Percent, const char* Stage)
    {
        if (Percent > 0.0)
            std::cout << "[PROGRESS] " << Stage << " " << Percent << "%\n";
        return true;
    });

#if SKFW_WITH_SKYLARK
    if (Result.Report.Succeeded && Result.Document && ImportOptions.BuildSkylarkPreview)
    {
        Skylark::FSKGeometryRegistry GeometryRegistry;
        FwSkylarkPreviewOptions PreviewOptions;
        PreviewOptions.MaxTriangles = Cli.MaxPreviewTriangles;
        const FwSkylarkPreviewResult Preview = RegisterPreviewMeshInSkylark(*Result.Document, GeometryRegistry, PreviewOptions);
        Result.Report.AddInfo(Preview.Message + " GeometryKey=" + std::to_string(Preview.GeometryKey));
    }
#endif

    PrintReport(Result.Report);

    if (!Cli.ReportPath.empty())
    {
        std::string Error;
        if (!WriteImportReportJsonFile(Cli.ReportPath, Result.Report, Result.Document.get(), Error))
        {
            std::cerr << Error << "\n";
            return 1;
        }
        std::cout << "Report written    : " << Cli.ReportPath.string() << "\n";
    }

    return Result.Report.Succeeded ? 0 : 1;
}
