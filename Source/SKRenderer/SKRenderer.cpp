#include "SKRenderer/SKRenderer.h"
#include "SKRenderer/SKRenderPipeline.h"
#include "SKRenderer/SKDefaultPasses.h"

namespace Skylark
{
	TUniquePtr<ISKRenderPipeline> SKCreateDefaultPipeline()
	{
		auto Pipeline = std::make_unique<FSKRenderPipeline>();
		Pipeline->AddPass(std::make_unique<FSKBasePass>());
		Pipeline->AddPass(std::make_unique<FSKEdgePass>());
		Pipeline->AddPass(std::make_unique<FSKHiddenLinePass>());
		Pipeline->AddPass(std::make_unique<FSKSectionPass>());
		Pipeline->AddPass(std::make_unique<FSKSelectionPass>());
		Pipeline->AddPass(std::make_unique<FSKFieldVizPass>());
		Pipeline->AddPass(std::make_unique<FSKPostProcessPass>());
		Pipeline->AddPass(std::make_unique<FSKSelectionOverlayPass>());
		return Pipeline;
	}
}
