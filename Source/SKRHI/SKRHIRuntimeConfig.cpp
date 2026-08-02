#include "SKRHI/SKRHIRuntimeConfig.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
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
            const FSKString Name = NormalizeText(Text);
            if (Name == "1" || Name == "true" || Name == "on" || Name == "yes")
            {
                OutValue = true;
                return true;
            }
            if (Name == "0" || Name == "false" || Name == "off" || Name == "no")
            {
                OutValue = false;
                return true;
            }
            return false;
        }

        static bool TryParseIntText(const char* Text, int32& OutValue)
        {
            if (!Text || !*Text)
            {
                return false;
            }

            char* EndPtr = nullptr;
            const long Parsed = std::strtol(Text, &EndPtr, 10);
            if (!EndPtr || *EndPtr != '\0')
            {
                return false;
            }

            OutValue = static_cast<int32>(Parsed);
            return true;
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

        static void ParseSizeValue(const char* Text, int32& OutWidth, int32& OutHeight)
        {
            if (!Text)
            {
                return;
            }

            const std::string Value(Text);
            const size_t Split = Value.find_first_of("xX");
            if (Split == std::string::npos)
            {
                return;
            }

            int32 ParsedWidth = OutWidth;
            int32 ParsedHeight = OutHeight;
            if (TryParseIntText(Value.substr(0, Split).c_str(), ParsedWidth) &&
                TryParseIntText(Value.substr(Split + 1).c_str(), ParsedHeight))
            {
                if (ParsedWidth > 0)
                {
                    OutWidth = ParsedWidth;
                }
                if (ParsedHeight > 0)
                {
                    OutHeight = ParsedHeight;
                }
            }
        }
    }

    void SKInitDefaultRHIRuntimeOptions(FSKRHIRuntimeOptions& OutOptions)
    {
        OutOptions = {};
        OutOptions.RequestedApi = SKGetDefaultRHIApiForPlatform();

    #if defined(SK_PLATFORM_WINDOWS) && SK_PLATFORM_WINDOWS
        if (OutOptions.RequestedApi == ESKRHIApi::Null)
        {
            OutOptions.RequestedApi = ESKRHIApi::D3D11;
        }
    #endif

        if (const char* EnvRhi = std::getenv("SKYLARK_RHI"))
        {
            ESKRHIApi Parsed = OutOptions.RequestedApi;
            if (SKTryParseRHIApi(EnvRhi, Parsed))
            {
                OutOptions.RequestedApi = Parsed;
            }
        }
        if (const char* EnvScene = std::getenv("SKYLARK_SCENE"))
        {
            OutOptions.ScenePath = EnvScene;
        }
        if (const char* EnvWidth = std::getenv("SKYLARK_WIDTH"))
        {
            int32 Parsed = OutOptions.Width;
            if (TryParseIntText(EnvWidth, Parsed) && Parsed > 0)
            {
                OutOptions.Width = Parsed;
            }
        }
        if (const char* EnvHeight = std::getenv("SKYLARK_HEIGHT"))
        {
            int32 Parsed = OutOptions.Height;
            if (TryParseIntText(EnvHeight, Parsed) && Parsed > 0)
            {
                OutOptions.Height = Parsed;
            }
        }
        if (const char* EnvSize = std::getenv("SKYLARK_SIZE"))
        {
            ParseSizeValue(EnvSize, OutOptions.Width, OutOptions.Height);
        }
        if (const char* EnvVSync = std::getenv("SKYLARK_VSYNC"))
        {
            bool Parsed = OutOptions.bEnableVSync;
            if (TryParseBoolText(EnvVSync, Parsed))
            {
                OutOptions.bEnableVSync = Parsed;
            }
        }
        if (const char* EnvDebug = std::getenv("SKYLARK_RHI_DEBUG"))
        {
            bool Parsed = OutOptions.bEnableDebugLayer;
            if (TryParseBoolText(EnvDebug, Parsed))
            {
                OutOptions.bEnableDebugLayer = Parsed;
            }
        }
        if (const char* EnvThread = std::getenv("SKYLARK_RENDER_THREAD"))
        {
            bool Parsed = OutOptions.bUseRenderThread;
            if (TryParseBoolText(EnvThread, Parsed))
            {
                OutOptions.bUseRenderThread = Parsed;
            }
        }
    }

    bool SKParseRHIRuntimeOptions(int Argc, char** Argv, FSKRHIRuntimeOptions& OutOptions, FSKString* OutWarnings)
    {
        SKInitDefaultRHIRuntimeOptions(OutOptions);

        for (int Index = 1; Index < Argc; ++Index)
        {
            const std::string_view Arg = Argv[Index] ? std::string_view(Argv[Index]) : std::string_view();
            if (Arg.empty())
            {
                continue;
            }

            if (Arg == "--help" || Arg == "-h")
            {
                OutOptions.bShowHelp = true;
                continue;
            }

            if ((Arg == "--rhi" || Arg == "--backend") && (Index + 1) < Argc)
            {
                ESKRHIApi Parsed = OutOptions.RequestedApi;
                if (SKTryParseRHIApi(Argv[Index + 1], Parsed))
                {
                    OutOptions.RequestedApi = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Unknown RHI backend: ") + Argv[Index + 1]);
                }
                ++Index;
                continue;
            }

            if ((Arg == "--scene") && (Index + 1) < Argc)
            {
                OutOptions.ScenePath = Argv[Index + 1];
                ++Index;
                continue;
            }

            if ((Arg == "--width") && (Index + 1) < Argc)
            {
                int32 Parsed = OutOptions.Width;
                if (TryParseIntText(Argv[Index + 1], Parsed) && Parsed > 0)
                {
                    OutOptions.Width = Parsed;
                }
                ++Index;
                continue;
            }

            if ((Arg == "--height") && (Index + 1) < Argc)
            {
                int32 Parsed = OutOptions.Height;
                if (TryParseIntText(Argv[Index + 1], Parsed) && Parsed > 0)
                {
                    OutOptions.Height = Parsed;
                }
                ++Index;
                continue;
            }

            if ((Arg == "--size") && (Index + 1) < Argc)
            {
                ParseSizeValue(Argv[Index + 1], OutOptions.Width, OutOptions.Height);
                ++Index;
                continue;
            }

            if ((Arg == "--vsync") && (Index + 1) < Argc)
            {
                bool Parsed = OutOptions.bEnableVSync;
                if (TryParseBoolText(Argv[Index + 1], Parsed))
                {
                    OutOptions.bEnableVSync = Parsed;
                }
                ++Index;
                continue;
            }

            if (Arg == "--debug-rhi")
            {
                OutOptions.bEnableDebugLayer = true;
                continue;
            }

            if (Arg == "--render-thread")
            {
                OutOptions.bUseRenderThread = true;
                continue;
            }

            if (Arg == "--no-render-thread")
            {
                OutOptions.bUseRenderThread = false;
                continue;
            }

            if (Arg == "--no-vsync")
            {
                OutOptions.bEnableVSync = false;
                continue;
            }

            constexpr std::string_view RhiPrefix = "--rhi=";
            constexpr std::string_view BackendPrefix = "--backend=";
            constexpr std::string_view ScenePrefix = "--scene=";
            constexpr std::string_view WidthPrefix = "--width=";
            constexpr std::string_view HeightPrefix = "--height=";
            constexpr std::string_view SizePrefix = "--size=";
            constexpr std::string_view VSyncPrefix = "--vsync=";

            if (Arg.rfind(RhiPrefix, 0) == 0)
            {
                const std::string Value(Arg.substr(RhiPrefix.size()));
                ESKRHIApi Parsed = OutOptions.RequestedApi;
                if (SKTryParseRHIApi(Value.c_str(), Parsed))
                {
                    OutOptions.RequestedApi = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Unknown RHI backend: ") + Value.c_str());
                }
                continue;
            }

            if (Arg.rfind(BackendPrefix, 0) == 0)
            {
                const std::string Value(Arg.substr(BackendPrefix.size()));
                ESKRHIApi Parsed = OutOptions.RequestedApi;
                if (SKTryParseRHIApi(Value.c_str(), Parsed))
                {
                    OutOptions.RequestedApi = Parsed;
                }
                else
                {
                    AppendWarning(OutWarnings, FSKString("Unknown RHI backend: ") + Value.c_str());
                }
                continue;
            }

            if (Arg.rfind(ScenePrefix, 0) == 0)
            {
                OutOptions.ScenePath = std::string(Arg.substr(ScenePrefix.size())).c_str();
                continue;
            }

            if (Arg.rfind(WidthPrefix, 0) == 0)
            {
                int32 Parsed = OutOptions.Width;
                const std::string Value(Arg.substr(WidthPrefix.size()));
                if (TryParseIntText(Value.c_str(), Parsed) && Parsed > 0)
                {
                    OutOptions.Width = Parsed;
                }
                continue;
            }

            if (Arg.rfind(HeightPrefix, 0) == 0)
            {
                int32 Parsed = OutOptions.Height;
                const std::string Value(Arg.substr(HeightPrefix.size()));
                if (TryParseIntText(Value.c_str(), Parsed) && Parsed > 0)
                {
                    OutOptions.Height = Parsed;
                }
                continue;
            }

            if (Arg.rfind(SizePrefix, 0) == 0)
            {
                const std::string Value(Arg.substr(SizePrefix.size()));
                ParseSizeValue(Value.c_str(), OutOptions.Width, OutOptions.Height);
                continue;
            }

            if (Arg.rfind(VSyncPrefix, 0) == 0)
            {
                bool Parsed = OutOptions.bEnableVSync;
                const std::string Value(Arg.substr(VSyncPrefix.size()));
                if (TryParseBoolText(Value.c_str(), Parsed))
                {
                    OutOptions.bEnableVSync = Parsed;
                }
                continue;
            }

            if (!Arg.empty() && Arg.front() != '-')
            {
                OutOptions.ScenePath = std::string(Arg).c_str();
                continue;
            }
        }

        OutOptions.Width = (std::max)(1, OutOptions.Width);
        OutOptions.Height = (std::max)(1, OutOptions.Height);
        return true;
    }

    FSKString SKBuildRHIBackendListString(bool bSupportedOnly)
    {
        const ESKRHIApi Backends[] = {
            ESKRHIApi::D3D11,
            ESKRHIApi::D3D12,
            ESKRHIApi::OpenGL,
            ESKRHIApi::Vulkan,
            ESKRHIApi::Null,
        };

        std::ostringstream Builder;
        bool bFirst = true;
        for (ESKRHIApi Api : Backends)
        {
            const FSKRHIBackendDesc* Desc = SKFindRHIBackend(Api);
            if (!Desc)
            {
                continue;
            }
            if (bSupportedOnly && !SKIsRHIBackendSupported(Api))
            {
                continue;
            }
            if (!bFirst)
            {
                Builder << ", ";
            }
            Builder << Desc->Name;
            bFirst = false;
        }
        return Builder.str().c_str();
    }
}
