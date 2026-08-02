#include "Import/ImporterRegistry.h"
#include "Import/Stl/StlImporter.h"
#include "Import/External/ExternalFormatStubs.h"

namespace Skylark::FormatWorkbench
{
    void FwImporterRegistry::Register(std::unique_ptr<IModelImporter> Importer)
    {
        if (Importer)
            Importers.push_back(std::move(Importer));
    }

    const IModelImporter* FwImporterRegistry::FindImporter(const std::filesystem::path& Path) const
    {
        for (const std::unique_ptr<IModelImporter>& Importer : Importers)
        {
            if (Importer->CanImport(Path))
                return Importer.get();
        }
        return nullptr;
    }

    FwImportResult FwImporterRegistry::Import(const std::filesystem::path& Path, const FwImportOptions& Options, const FwProgressCallback& Progress) const
    {
        const IModelImporter* Importer = FindImporter(Path);
        if (Importer == nullptr)
        {
            FwImportResult Result;
            Result.Report.AddError("No importer registered for file extension: " + Path.extension().string());
            return Result;
        }
        return Importer->Import(Path, Options, Progress);
    }

    std::vector<std::string> FwImporterRegistry::ImporterNames() const
    {
        std::vector<std::string> Names;
        Names.reserve(Importers.size());
        for (const std::unique_ptr<IModelImporter>& Importer : Importers)
            Names.emplace_back(Importer->Name());
        return Names;
    }

    FwImporterRegistry CreateDefaultImporterRegistry()
    {
        FwImporterRegistry Registry;
        Registry.Register(std::make_unique<FwStlImporter>());
        Registry.Register(std::make_unique<FwExternalFormatStubImporter>(EModelFileFormat::Step, "STEPControl / XDE STEP importer placeholder"));
        Registry.Register(std::make_unique<FwExternalFormatStubImporter>(EModelFileFormat::Obj, "Assimp OBJ importer placeholder"));
        Registry.Register(std::make_unique<FwExternalFormatStubImporter>(EModelFileFormat::Ply, "Assimp or native PLY importer placeholder"));
        Registry.Register(std::make_unique<FwExternalFormatStubImporter>(EModelFileFormat::Gltf, "fastgltf / Assimp glTF importer placeholder"));
        Registry.Register(std::make_unique<FwExternalFormatStubImporter>(EModelFileFormat::Jt, "JT importer placeholder"));
        Registry.Register(std::make_unique<FwExternalFormatStubImporter>(EModelFileFormat::ThreeDXml, "3DXML importer placeholder"));
        return Registry;
    }
}
