#include "Import/ImportTypes.h"

namespace Skylark::FormatWorkbench
{
    void FwImportReport::AddInfo(std::string Text)
    {
        Messages.push_back({ EImportSeverity::Info, std::move(Text) });
    }

    void FwImportReport::AddWarning(std::string Text)
    {
        Messages.push_back({ EImportSeverity::Warning, std::move(Text) });
    }

    void FwImportReport::AddError(std::string Text)
    {
        Messages.push_back({ EImportSeverity::Error, std::move(Text) });
    }
}
