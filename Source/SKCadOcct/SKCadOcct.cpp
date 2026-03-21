#include "SKCadOcct/SKCadOcct.h"
#include "SKCore/SKCoreLog.h"
#include "SKGeometry/SKMeshBuilder.h"

#ifndef SK_WITH_OCCT
	#define SK_WITH_OCCT 0
#endif

namespace Skylark
{
	class FSKCadModelBasic final : public ISKCadModel
	{
	public:
		explicit FSKCadModelBasic(FSKString InPath, FSKMeshData InMesh)
			: Path(std::move(InPath)), Mesh(std::move(InMesh))
		{
		}

		const FSKString& GetSourcePath() const override { return Path; }
		const FSKMeshData* GetDisplayMesh() const override { return &Mesh; }

	private:
		FSKString Path;
		FSKMeshData Mesh;
	};

	bool FSKOcctCadImporter::CanImport(const char* FilePathUtf8) const
	{
		if (!FilePathUtf8)
		{
			return false;
		}
		FSKString P(FilePathUtf8);
		for (auto& c : P) c = (char)std::tolower((unsigned char)c);

		return P.ends_with(".step") || P.ends_with(".stp") || P.ends_with(".iges") || P.ends_with(".igs") || P.ends_with(".brep");
	}

	TSharedPtr<ISKCadModel> FSKOcctCadImporter::Import(const char* FilePathUtf8, const FSKCadTessellationParams& Params)
	{
#if SK_WITH_OCCT
		// ---- Real OCCT implementation should live here (V6+ migration target) ----
		// Planned pipeline:
		// 1) Load file -> TopoDS_Shape
		// 2) Iterate faces -> triangulation (Poly_Triangulation)
		// 3) Build FSKMeshSection per face (OwnerId = Face TopoId, HitProxyId assigned by HitProxyTable)
		// 4) Optional: extract edges from TopoDS_Edge to get true feature edges
		// 5) Return ISKCadModel with display mesh
		(void)Params;
		SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKCadOcct: SK_WITH_OCCT enabled but implementation is not linked in this source-only drop.");
		return nullptr;
#else
		(void)Params;
		SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKCadOcct: OCCT is disabled (SK_WITH_OCCT=OFF). Returning stub CAD model (empty mesh).");

		FSKMeshData Mesh{};
		// Keep it valid but empty. Host can still test pipeline & selection buffer clearing.
		return std::make_shared<FSKCadModelBasic>(FilePathUtf8 ? FSKString(FilePathUtf8) : FSKString(), std::move(Mesh));
#endif
	}
}
