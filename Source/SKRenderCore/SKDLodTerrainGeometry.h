#ifndef SKDLODTERRAINGEOMETRY_H
#define SKDLODTERRAINGEOMETRY_H
#include "SKGeometry.h"
namespace SKEngine2
{

	class SKDLodTerrainNode;
	class SKGRAPHIC_API SKDLodTerrainGeometry : public SKGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKDLodTerrainGeometry();
		virtual ~SKDLodTerrainGeometry();
		bool CreateMesh(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
			unsigned int uiLevel,unsigned int uiTotleLevel);
	protected:
		SKDLodTerrainNode * GetParentTerrainNode()const;
	};
	DECLARE_Ptr(SKDLodTerrainGeometry);
	SKTYPE_MARCO(SKDLodTerrainGeometry);
};
#endif