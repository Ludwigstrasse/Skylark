#include "SKEngine/SKEngine.h"

int main()
{
    Skylark::ISKEngine* pEngine = Skylark::SKCreateEngine();
    Skylark::SKDestroyEngine(pEngine);
    return 0;
}
