#include "SkSampleControls.h"

#include <cstdio>

namespace Skylark
{
    namespace
    {
        static FSKString BuildCadEditorLabControls()
        {
            return
                "Controls:\n"
                "  LMB pick, RMB orbit, MMB pan, wheel zoom, F fit\n"
                "  1 shaded+edges, 2 shaded, 3 wireframe, 4 hidden-line\n"
                "  [ ] explode, H hide, I isolate, U show all, Ctrl+C clone\n"
                "  W/S/A/D/Q/E move selected, R reset selected offset\n";
        }

        static FSKString BuildCadSceneTechniquesControls()
        {
            return
                "Controls:\n"
                "  RMB orbit, MMB pan, wheel zoom, LMB pick, F fit\n"
                "  1 shaded, 2 shaded+edges, 3 wireframe, 4 xray, 5 object-id, 6 normals\n"
                "  A all edges, B boundary edges, C sharp edges\n"
                "  E toggle animated explode, [ ] explode amount, V toggle vsync\n";
        }
    }

    FSKString SKBuildSampleControlsText(ESKSampleControlProfile Profile)
    {
        switch (Profile)
        {
        case ESKSampleControlProfile::CadEditorLab:
            return BuildCadEditorLabControls();
        case ESKSampleControlProfile::CadSceneTechniques:
            return BuildCadSceneTechniquesControls();
        default:
            return FSKString();
        }
    }

    void SKPrintSampleControls(ESKSampleControlProfile Profile)
    {
        const FSKString Text = SKBuildSampleControlsText(Profile);
        if (!Text.empty())
        {
            std::fputs(Text.c_str(), stdout);
        }
    }
}
