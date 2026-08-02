#pragma once
#include "Import/ImportTypes.h"

#include <filesystem>
#include <string>

namespace Skylark::FormatWorkbench
{
    class IModelImporter
    {
    public:
        virtual ~IModelImporter() = default;
        [[nodiscard]] virtual const char* Name() const = 0;
        [[nodiscard]] virtual bool CanImport(const std::filesystem::path& Path) const = 0;
        virtual FwImportResult Import(const std::filesystem::path& Path, const FwImportOptions& Options, const FwProgressCallback& Progress) const = 0;
    };
}
