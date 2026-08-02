#include "Import/FormatDetector.h"

#include <algorithm>
#include <cctype>
#include <string>

namespace Skylark::FormatWorkbench
{
    static std::string Lowercase(std::string Text)
    {
        std::transform(Text.begin(), Text.end(), Text.begin(), [](unsigned char C) { return static_cast<char>(std::tolower(C)); });
        return Text;
    }

    EModelFileFormat DetectFormatByExtension(const std::filesystem::path& Path)
    {
        const std::string Ext = Lowercase(Path.extension().string());
        if (Ext == ".stl") return EModelFileFormat::BinaryStl;
        if (Ext == ".step" || Ext == ".stp") return EModelFileFormat::Step;
        if (Ext == ".obj") return EModelFileFormat::Obj;
        if (Ext == ".ply") return EModelFileFormat::Ply;
        if (Ext == ".gltf" || Ext == ".glb") return EModelFileFormat::Gltf;
        if (Ext == ".jt") return EModelFileFormat::Jt;
        if (Ext == ".3dxml") return EModelFileFormat::ThreeDXml;
        return EModelFileFormat::Unknown;
    }

    bool IsStlFormat(EModelFileFormat Format)
    {
        return Format == EModelFileFormat::BinaryStl || Format == EModelFileFormat::AsciiStl;
    }
}
