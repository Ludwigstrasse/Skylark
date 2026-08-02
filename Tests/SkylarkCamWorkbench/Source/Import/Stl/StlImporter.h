#pragma once
#include "Import/IModelImporter.h"

namespace Skylark::FormatWorkbench
{
    class FwStlImporter final : public IModelImporter
    {
    public:
        [[nodiscard]] const char* Name() const override { return "Native compact STL importer"; }
        [[nodiscard]] bool CanImport(const std::filesystem::path& Path) const override;
        FwImportResult Import(const std::filesystem::path& Path, const FwImportOptions& Options, const FwProgressCallback& Progress) const override;
    };

    bool GenerateBinaryStlFile(const std::filesystem::path& Path, std::uint64_t TriangleCount, std::string& ErrorMessage);
}
