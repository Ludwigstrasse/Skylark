#include "SKCadOcct/SKCadOcctHLRProvider.h"
#include "SKCore/SKCoreLog.h"

#if defined(SK_WITH_OCCT) && SK_WITH_OCCT
	// OCCT includes go here when enabled in your toolchain.
	// #include <TopoDS_Shape.hxx>
	// #include <HLRAlgo_Projector.hxx>
	// #include <HLRBRep_Algo.hxx>
#endif

namespace Skylark
{
	bool FSKCadOcctHLRProvider::ComputeHLR(const FSKHLRRequest& Req, FSKHLRResult& Out)
	{
		(void)Req;
		Out.Visible.clear();
		Out.Hidden.clear();

#if defined(SK_WITH_OCCT) && SK_WITH_OCCT
		// V10: placeholder.
		// Implement: GeometryKey -> TopoDS_Shape; build projector from Req.ViewProj;
		// run HLRBRep; extract visible/hidden edges into Out.Visible/Out.Hidden.
		return false;
#else
		SK_LOG(GLogSkylark, ESKLogVerbosity::Verbose, "SKCadOcctHLRProvider: OCCT disabled.");
		return false;
#endif
	}
}
