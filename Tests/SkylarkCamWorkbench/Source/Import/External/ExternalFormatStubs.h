#pragma once
#include "Import/IModelImporter.h"
#include "Import/FormatDetector.h"

namespace Skylark::FormatWorkbench
{
    class FwExternalFormatStubImporter final : public IModelImporter
    {
    public:
        FwExternalFormatStubImporter(EModelFileFormat InFormat, std::string InBackendName);
        [[nodiscard]] const char* Name() const override;
        [[nodiscard]] bool CanImport(const std::filesystem::path& Path) const override;
        FwImportResult Import(const std::filesystem::path& Path, const FwImportOptions& Options, const FwProgressCallback& Progress) const override;

    private:
        EModelFileFormat Format = EModelFileFormat::Unknown;
        std::string BackendName;
    };
}
