#pragma once
#include "SKCAD/SKCadHLR.h"

namespace Skylark
{
	class FSKCadOcctHLRProvider final : public ISKHLRProvider
	{
	public:
		bool ComputeHLR(const FSKHLRRequest& Req, FSKHLRResult& Out) override;
	};
}
