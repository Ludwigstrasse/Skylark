#pragma once
#include "SKCAD/SKCadImporter.h"

namespace Skylark
{
	/**
	 * OCCT CAD importer module (optional)
	 *
	 * This module is OFF by default (SK_WITH_OCCT=OFF) to keep Skylark buildable without OCCT.
	 * When enabled, it provides:
	 * - STEP/IGES/BREP load via OCCT
	 * - Tessellation with industrial deflection controls
	 * - Stable mapping: TopoDS_* -> FSKObjectId (TopoId)
	 *
	 * V6: provides interface + stub implementation (no OCCT dependency unless enabled).
	 */

	class FSKOcctCadImporter final : public ISKCadImporter
	{
	public:
		bool CanImport(const char* FilePathUtf8) const override;
		TSharedPtr<ISKCadModel> Import(const char* FilePathUtf8, const FSKCadTessellationParams& Params) override;
	};
}
