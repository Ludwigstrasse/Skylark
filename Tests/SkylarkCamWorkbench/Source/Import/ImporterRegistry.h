#pragma once
#include "Import/IModelImporter.h"

#include <memory>
#include <vector>

namespace Skylark::FormatWorkbench
{
    class FwImporterRegistry final
    {
    public:
        void Register(std::unique_ptr<IModelImporter> Importer);
        [[nodiscard]] const IModelImporter* FindImporter(const std::filesystem::path& Path) const;
        [[nodiscard]] FwImportResult Import(const std::filesystem::path& Path, const FwImportOptions& Options, const FwProgressCallback& Progress = {}) const;
        [[nodiscard]] std::vector<std::string> ImporterNames() const;

    private:
        std::vector<std::unique_ptr<IModelImporter>> Importers;
    };

    FwImporterRegistry CreateDefaultImporterRegistry();
}
