#include "SKRHI/SKRHIFactory.h"
#include "SKRHI/SKNullRHI/SKNullRHI.h"

#if defined(SK_WITH_D3D11_RHI) && SK_WITH_D3D11_RHI
	#include "SKD3D11RHI/SKD3D11RHI.h"
#endif

namespace Skylark
{
	TUniquePtr<ISKRHIDevice> SKCreateRHIDevice(const FSKRHIDeviceDesc& Desc)
	{
		switch (Desc.Api)
		{
		case ESKRHIApi::D3D11:
		{
#if defined(SK_WITH_D3D11_RHI) && SK_WITH_D3D11_RHI
			return SKCreateD3D11RHIDevice();
#else
			return std::make_unique<FSKNullRHIDevice>();
#endif
		}
		case ESKRHIApi::Null:
		default:
			return std::make_unique<FSKNullRHIDevice>();
		}
	}
}
