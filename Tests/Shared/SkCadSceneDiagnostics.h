#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SkCadSceneImporter.h"

namespace Skylark
{
    struct FSKCadSceneDiagnosticsOptions
    {
        bool bPrintSceneSummary = true;
        bool bPrintSceneBounds = true;
    };

    void SKInitCadSceneDiagnosticsOptions(FSKCadSceneDiagnosticsOptions& OutOptions);
    bool SKParseCadSceneDiagnosticsOptions(int Argc, char** Argv, FSKCadSceneDiagnosticsOptions& OutOptions, FSKString* OutWarnings = nullptr);
    void SKPrintCadSceneDiagnosticsOptionSummary(const FSKCadSceneDiagnosticsOptions& Options);
    void SKPrintCadSceneDocumentDiagnostics(const CadSceneTest::FSkCadSceneDocument& Document, const FSKCadSceneDiagnosticsOptions& Options);
}
