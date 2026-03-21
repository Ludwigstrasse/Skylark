#pragma once
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	using FSKRHICreateDeviceFn = TUniquePtr<ISKRHIDevice>(*)();
	using FSKRHISupportQueryFn = bool(*)();

	struct FSKRHIBackendDesc
	{
		ESKRHIApi Api = ESKRHIApi::Null;
		const char* Name = "null";
		FSKRHICreateDeviceFn CreateDevice = nullptr;
		FSKRHISupportQueryFn IsSupported = nullptr;
	};

	// Backend registration / lookup (UE DynamicRHI-style registry).
	void SKRegisterRHIBackend(const FSKRHIBackendDesc& Desc);
	const FSKRHIBackendDesc* SKFindRHIBackend(ESKRHIApi Api);
	const FSKRHIBackendDesc* SKFindRHIBackendByName(const char* Name);

	bool SKIsRHIBackendSupported(ESKRHIApi Api);
	bool SKIsRHIBackendNameSupported(const char* Name);

	// Human-readable helpers for command line / config / logging.
	const char* SKRHIApiToString(ESKRHIApi Api);
	bool SKTryParseRHIApi(const char* Text, ESKRHIApi& OutApi);
	ESKRHIApi SKGetDefaultRHIApiForPlatform();

	// Factory helper (UE: DynamicRHI). Falls back to NullRHI when backend is unavailable.
	TUniquePtr<ISKRHIDevice> SKCreateRHIDevice(const FSKRHIDeviceDesc& Desc);
}
