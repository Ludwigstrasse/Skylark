#pragma once
#include "Import/ImportTypes.h"

#include <filesystem>
#include <ostream>

namespace Skylark::FormatWorkbench
{
    bool WriteImportReportJson(std::ostream& Output, const FwImportReport& Report, const FwModelDocument* Document);
    bool WriteImportReportJsonFile(const std::filesystem::path& Path, const FwImportReport& Report, const FwModelDocument* Document, std::string& ErrorMessage);
}
