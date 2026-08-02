#include "SkSampleRunReport.h"
    #include "SkSampleAppHost.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

    namespace Skylark
    {
        namespace
        {
            static FSKString NormalizeText(const char* Text)
            {
                FSKString Out;
                if (!Text)
                {
                    return Out;
                }
                while (*Text)
                {
                    Out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(*Text))));
                    ++Text;
                }
                return Out;
            }

            static bool TryParseBoolText(const char* Text, bool& OutValue)
            {
                const FSKString Value = NormalizeText(Text);
                if (Value == "1" || Value == "true" || Value == "on" || Value == "yes")
                {
                    OutValue = true;
                    return true;
                }
                if (Value == "0" || Value == "false" || Value == "off" || Value == "no")
                {
                    OutValue = false;
                    return true;
                }
                return false;
            }

        static void AppendWarning(FSKString* OutWarnings, const FSKString& Message)
        {
            if (!OutWarnings)
            {
                return;
            }
            if (!OutWarnings->empty())
            {
                OutWarnings->append("\n");
            }
            OutWarnings->append(Message);
        }
    }

        void SKInitSampleRunReportOptions(FSKSampleRunReportOptions& OutOptions)
        {
            OutOptions = {};
            if (const char* EnvPath = std::getenv("SKYLARK_RUN_REPORT"))
            {
                OutOptions.ReportPath = EnvPath;
            }
            if (const char* EnvAppend = std::getenv("SKYLARK_APPEND_RUN_REPORT"))
            {
                bool Parsed = OutOptions.bAppend;
                if (TryParseBoolText(EnvAppend, Parsed))
                {
                    OutOptions.bAppend = Parsed;
                }
            }
            if (const char* EnvPrint = std::getenv("SKYLARK_PRINT_RUN_REPORT"))
            {
                bool Parsed = OutOptions.bPrintPathOnWrite;
                if (TryParseBoolText(EnvPrint, Parsed))
                {
                    OutOptions.bPrintPathOnWrite = Parsed;
                }
            }
        }

    bool SKParseSampleRunReportOptions(int Argc, char** Argv, FSKSampleRunReportOptions& OutOptions, FSKString* OutWarnings)
    {
        SKInitSampleRunReportOptions(OutOptions);
        for (int Index = 1; Index < Argc; ++Index)
        {
            const std::string_view Arg = Argv[Index] ? std::string_view(Argv[Index]) : std::string_view();
            if (Arg.empty())
            {
                continue;
            }
            if (Arg.rfind("--run-report=", 0) == 0)
            {
                OutOptions.ReportPath = std::string(Arg.substr(13));
                continue;
            }
            if (Arg == "--run-report" && (Index + 1) < Argc)
            {
                OutOptions.ReportPath = Argv[++Index];
                continue;
            }
            if (Arg == "--append-run-report")
            {
                OutOptions.bAppend = true;
                continue;
            }
            if (Arg == "--no-append-run-report")
            {
                OutOptions.bAppend = false;
                continue;
            }
            if (Arg == "--print-run-report")
            {
                OutOptions.bPrintPathOnWrite = true;
                continue;
            }
            if (Arg == "--no-print-run-report")
            {
                OutOptions.bPrintPathOnWrite = false;
                continue;
            }
            if (Arg.rfind("--print-run-report=", 0) == 0)
            {
                bool Parsed = OutOptions.bPrintPathOnWrite;
                if (TryParseBoolText(std::string(Arg.substr(19)).c_str(), Parsed))
                {
                    OutOptions.bPrintPathOnWrite = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Invalid bool value for --print-run-report: ") + std::string(Arg.substr(19)).c_str());
                }
                continue;
            }
        }
        return true;
    }

    void SKPrintSampleRunReportOptionSummary(const FSKSampleRunReportOptions& Options)
    {
        std::fprintf(stdout,
            "Run report: path=%s append=%s print=%s\n",
            Options.ReportPath.empty() ? "<disabled>" : Options.ReportPath.c_str(),
            Options.bAppend ? "on" : "off",
            Options.bPrintPathOnWrite ? "on" : "off");
    }

    FSKString SKBuildSampleRunReportText(const FSKSampleRunReportInput& Input)
    {
        std::ostringstream Oss;
        Oss << "app=" << (Input.AppName.empty() ? "Skylark" : Input.AppName) << "\n";
        Oss << "backend=" << (Input.BackendName.empty() ? "<unspecified>" : Input.BackendName) << "\n";
        Oss << "scene=" << (Input.ScenePath.empty() ? std::string("<unspecified>") : Input.ScenePath.string()) << "\n";
        Oss << "scene_display=" << SKGetSampleSceneDisplayName(Input.ScenePath) << "\n";
        Oss << "window=" << Input.Width << "x" << Input.Height << "\n";
        Oss << "vsync=" << (Input.bEnableVSync ? "on" : "off") << "\n";

        if (Input.RuntimeStats)
        {
            const double Fps = (Input.RuntimeStats->TotalSeconds > 1.0e-9)
                ? (static_cast<double>(Input.RuntimeStats->TotalFrameCount) / Input.RuntimeStats->TotalSeconds)
                : 0.0;
            const double Ms = Input.RuntimeStats->SmoothedFrameSeconds * 1000.0;
            Oss << "frames_total=" << Input.RuntimeStats->TotalFrameCount << "\n";
            Oss << "seconds_total=" << Input.RuntimeStats->TotalSeconds << "\n";
            Oss << "fps_average=" << Fps << "\n";
            Oss << "frame_ms_smoothed=" << Ms << "\n";
        }

        if (Input.BenchmarkState)
        {
            Oss << "benchmark_total_frames=" << Input.BenchmarkState->TotalFrameCount << "\n";
            Oss << "benchmark_total_seconds=" << Input.BenchmarkState->TotalSeconds << "\n";
            Oss << "benchmark_measured_frames=" << Input.BenchmarkState->MeasuredFrameCount << "\n";
            Oss << "benchmark_measured_seconds=" << Input.BenchmarkState->MeasuredSeconds << "\n";
        }

        if (Input.SceneDocument)
        {
            Oss << "materials=" << Input.SceneDocument->Materials.size() << "\n";
            Oss << "geometry=" << Input.SceneDocument->GeometryKeys.size() << "\n";
            Oss << "draw_items=" << Input.SceneDocument->DrawItems.size() << "\n";
            Oss << "scene_radius=" << Input.SceneDocument->SceneRadius << "\n";
            Oss << "scene_center="
                << Input.SceneDocument->SceneCenter.X << ","
                << Input.SceneDocument->SceneCenter.Y << ","
                << Input.SceneDocument->SceneCenter.Z << "\n";
            Oss << "scene_min="
                << Input.SceneDocument->SceneMin.X << ","
                << Input.SceneDocument->SceneMin.Y << ","
                << Input.SceneDocument->SceneMin.Z << "\n";
            Oss << "scene_max="
                << Input.SceneDocument->SceneMax.X << ","
                << Input.SceneDocument->SceneMax.Y << ","
                << Input.SceneDocument->SceneMax.Z << "\n";
        }

            return Oss.str().c_str();
        }

        bool SKWriteSampleRunReport(
            const FSKSampleRunReportOptions& Options,
            const FSKSampleRunReportInput& Input,
            FSKString* OutError,
            std::filesystem::path* OutWrittenPath)
        {
            if (Options.ReportPath.empty())
            {
                if (OutWrittenPath)
                {
                    OutWrittenPath->clear();
                }
                return true;
            }

        const std::filesystem::path ReportPath = std::filesystem::path(Options.ReportPath.c_str());
        std::error_code Ec;
        if (ReportPath.has_parent_path())
        {
            std::filesystem::create_directories(ReportPath.parent_path(), Ec);
            if (Ec)
            {
                if (OutError)
                {
                    *OutError = FSKString("Failed to create run report directory: ") + ReportPath.parent_path().string().c_str();
                }
                return false;
            }
        }

            std::ofstream OutFile;
            OutFile.open(ReportPath, Options.bAppend ? (std::ios::out | std::ios::app) : std::ios::out);
            if (!OutFile.is_open())
            {
                if (OutError)
                {
                    *OutError = FSKString("Failed to open run report for writing: ") + ReportPath.string().c_str();
                }
                return false;
            }

        if (Options.bAppend)
        {
            OutFile << "-----\n";
        }
        OutFile << SKBuildSampleRunReportText(Input);
        OutFile.flush();
        if (!OutFile.good())
        {
            if (OutError)
            {
                *OutError = FSKString("Failed while writing run report: ") + ReportPath.string().c_str();
            }
            return false;
        }

            if (OutWrittenPath)
            {
                *OutWrittenPath = ReportPath;
            }
            return true;
        }
    }
