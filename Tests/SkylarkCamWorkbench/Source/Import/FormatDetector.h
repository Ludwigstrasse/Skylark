#pragma once
#include "Model/ModelTypes.h"

#include <filesystem>

namespace Skylark::FormatWorkbench
{
    EModelFileFormat DetectFormatByExtension(const std::filesystem::path& Path);
    bool IsStlFormat(EModelFileFormat Format);
}
