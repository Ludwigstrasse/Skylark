#include "Import/External/ExternalFormatStubs.h"

namespace Skylark::FormatWorkbench
{
    FwExternalFormatStubImporter::FwExternalFormatStubImporter(EModelFileFormat InFormat, std::string InBackendName)
        : Format(InFormat)
        , BackendName(std::move(InBackendName))
    {
    }

    const char* FwExternalFormatStubImporter::Name() const
    {
        return BackendName.c_str();
    }

    bool FwExternalFormatStubImporter::CanImport(const std::filesystem::path& Path) const
    {
        return DetectFormatByExtension(Path) == Format;
    }

    FwImportResult FwExternalFormatStubImporter::Import(const std::filesystem::path&, const FwImportOptions&, const FwProgressCallback&) const
    {
        FwImportResult Result;
        Result.Report.Format = Format;
        Result.Report.AddError(std::string(ToString(Format)) + " commercial importer is architected but intentionally not enabled in this M0.2 source gate. Enable the dedicated backend module instead of faking data.");
        return Result;
    }
}
