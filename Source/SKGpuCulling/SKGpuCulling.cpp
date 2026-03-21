#include "SKGpuCulling/SKGpuCulling.h"

namespace Skylark
{
	TUniquePtr<ISKGpuCulling> SKCreateGpuCulling()
	{
		return std::make_unique<FSKCpuGpuCulling>();
	}
}
