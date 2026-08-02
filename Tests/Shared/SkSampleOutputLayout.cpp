#include "SkSampleOutputLayout.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace Skylark
{
    namespace
    {
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

        static FSKString ToLowerCopy(const char* Text)
        {
            FSKString Result;
            if (!Text)
            {
                return Result;
            }
            while (*Text)
            {
                Result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(*Text))));
                ++Text;
            }
            return Result;
        }

        static bool TryParseBoolText(const char* Text, bool& OutValue)
        {
            const FSKString Value = ToLowerCopy(Text);
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

        static FSKString SanitizeToken(FSKString Value)
        {
            for (char& Ch : Value)
            {
                const bool bOk =
                    std::isalnum(static_cast<unsigned char>(Ch)) ||
                    Ch == '-' || Ch == '_' || Ch == '.';
                if (!bOk)
                {
                    Ch = '_';
                }
            }
            while (!Value.empty() && (Value.front() == '_' || Value.front() == '.'))
            {
                Value.erase(Value.begin());
            }
            if (Value.empty())
            {
                Value = "default";
            }
            return Value;
        }

        static FSKString SceneStem(const std::filesystem::path& ScenePath)
        {
            const std::filesystem::path FileName = ScenePath.filename();
            FSKString Stem = FileName.stem().string().c_str();
            if (FileName.extension() == ".gz")
            {
                Stem = FileName.stem().stem().string().c_str();
            }
            if (Stem.empty())
            {
                Stem = "scene";
            }
            return SanitizeToken(Stem);
        }

        static std::filesystem::path ResolveOutputDirectory(const FSKSampleOutputLayoutOptions& Options)
        {
            if (!Options.OutputDirectory.empty())
            {
                return std::filesystem::path(Options.OutputDirectory.c_str());
            }
            return std::filesystem::path("out") / "sample_reports";
        }
    }

    void SKInitSampleOutputLayoutOptions(FSKSampleOutputLayoutOptions& OutOptions)
    {
        OutOptions = {};
        if (const char* EnvOutputDir = std::getenv("SKYLARK_OUTPUT_DIR"))
        {
            OutOptions.OutputDirectory = EnvOutputDir;
        }
        if (const char* EnvSessionTag = std::getenv("SKYLARK_SESSION_TAG"))
        {
            OutOptions.SessionTag = EnvSessionTag;
        }
        if (const char* EnvAutoReports = std::getenv("SKYLARK_AUTO_REPORT_FILES"))
        {
            bool Parsed = OutOptions.bAutoReportFiles;
            if (TryParseBoolText(EnvAutoReports, Parsed))
            {
                OutOptions.bAutoReportFiles = Parsed;
            }
        }
    }

    bool SKParseSampleOutputLayoutOptions(int Argc, char** Argv, FSKSampleOutputLayoutOptions& OutOptions, FSKString* OutWarnings)
    {
        SKInitSampleOutputLayoutOptions(OutOptions);

        for (int Index = 1; Index < Argc; ++Index)
        {
            const char* Arg = Argv[Index];
            if (!Arg)
            {
                continue;
            }

            const std::string_view View(Arg);
            if (View.rfind("--output-dir=", 0) == 0)
            {
                OutOptions.OutputDirectory = FSKString(View.substr(13));
                continue;
            }
            if (View == "--output-dir" && (Index + 1) < Argc)
            {
                OutOptions.OutputDirectory = Argv[++Index];
                continue;
            }
            if (View.rfind("--session-tag=", 0) == 0)
            {
                OutOptions.SessionTag = FSKString(View.substr(14));
                continue;
            }
            if (View == "--session-tag" && (Index + 1) < Argc)
            {
                OutOptions.SessionTag = Argv[++Index];
                continue;
            }
            if (View == "--auto-report-files")
            {
                OutOptions.bAutoReportFiles = true;
                continue;
            }
            if (View == "--no-auto-report-files")
            {
                OutOptions.bAutoReportFiles = false;
                continue;
            }
        }

        return true;
    }

    void SKPrintSampleOutputLayoutOptionSummary(const FSKSampleOutputLayoutOptions& Options)
    {
        std::fprintf(stdout,
            "Output layout: dir=%s session-tag=%s auto-report-files=%s\n",
            Options.OutputDirectory.empty() ? "<default>" : Options.OutputDirectory.c_str(),
            Options.SessionTag.empty() ? "<none>" : Options.SessionTag.c_str(),
            Options.bAutoReportFiles ? "on" : "off");
    }

    std::filesystem::path SKBuildSampleOutputLayoutPath(
        const FSKSampleOutputLayoutOptions& Options,
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        const char* ArtifactTag,
        const char* Extension)
    {
        const std::filesystem::path OutputDir = ResolveOutputDirectory(Options);

        FSKString FileName;
        FileName += SanitizeToken(AppName ? AppName : "app");
        FileName += "_";
        FileName += SanitizeToken(BackendName ? BackendName : "backend");
        FileName += "_";
        FileName += SceneStem(ScenePath);
        if (!Options.SessionTag.empty())
        {
            FileName += "_";
            FileName += SanitizeToken(Options.SessionTag);
        }
        if (ArtifactTag && *ArtifactTag)
        {
            FileName += "_";
            FileName += SanitizeToken(ArtifactTag);
        }
        if (Extension && *Extension)
        {
            FileName += ".";
            FileName += Extension;
        }

        return OutputDir / std::filesystem::path(FileName.c_str());
    }

    bool SKApplySampleOutputLayoutDefaults(
        const FSKSampleOutputLayoutOptions& Options,
        const char* AppName,
        const char* BackendName,
        const std::filesystem::path& ScenePath,
        FSKSampleBenchmarkOptions& InOutBenchmarkOptions,
        FSKSampleRunReportOptions& InOutRunReportOptions,
        FSKString* OutWarnings)
    {
        if (!Options.bAutoReportFiles && Options.OutputDirectory.empty())
        {
            return true;
        }

        const std::filesystem::path OutputDir = ResolveOutputDirectory(Options);
        std::error_code Ec;
        std::filesystem::create_directories(OutputDir, Ec);
        if (Ec)
        {
            AppendWarning(OutWarnings, FSKString("Failed to create output directory: ") + OutputDir.string().c_str());
            return false;
        }

        if (InOutBenchmarkOptions.CsvPath.empty())
        {
            InOutBenchmarkOptions.CsvPath = SKBuildSampleOutputLayoutPath(
                Options,
                AppName,
                BackendName,
                ScenePath,
                "benchmark",
                "csv").string().c_str();
        }

        if (InOutRunReportOptions.ReportPath.empty())
        {
            InOutRunReportOptions.ReportPath = SKBuildSampleOutputLayoutPath(
                Options,
                AppName,
                BackendName,
                ScenePath,
                "run_report",
                "txt").string().c_str();
        }

        return true;
    }
}
