#ifndef SKCLODTERRAINGEOMETRY_H
#define SKCLODTERRAINGEOMETRY_H
#include "SKGeometry.h"
/*
	˳
j	4 9 14 19 24
	3 8 13 18 23
	2 7 12 17 22
	1 6 11 16 21
	0 5 10 15 20 i
*/
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKCLodTerrainGeometry : public SKGeometry
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum	// TerrainGeometry Type
		{
			TGT_ROAM,
			TGT_QUAD,
			TGT_MAX
		};
		enum	// Neightbor Type
		{
			NT_LEFT,
			NT_RIGHT,
			NT_TOP,
			NT_BOTTOM,
			NT_MAX
		};
		SKCLodTerrainGeometry();
		virtual ~SKCLodTerrainGeometry() = 0;
	public:

		virtual bool CreateMeshDate(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
									unsigned int uiTesselationLevel);
		virtual unsigned int GetTerrainGeometryType() = 0;
		virtual bool AddNeighbor(SKCLodTerrainGeometry * pTerrainGemetry,unsigned int uiNeighbor);
		virtual void LinkNeighbor() = 0;
		//ÿڵԼ任ڵھӺ͵ǰڵLOD
		virtual void SetLocalScale(const SKVector3& Scale){};
		virtual void SetLocalTranslate(const SKVector3& Translate){};
		virtual void SetLocalRotate(const SKMatrix3X3 & Rotate){};
		virtual void SetLocalTransform(const SKTransform & LocalTransform){};
		virtual void SetLocalMat(const SKMatrix3X3W SKMat){};

		virtual void ComputeVariance(unsigned int uiCurLevel = 0) = 0;
		virtual void ClearInfo() = 0;
		
		virtual void UpDateView(SKCuller & Culler,double dAppTime);
	protected:
		SKCLodTerrainGeometry * m_pNeighbor[NT_MAX];
		unsigned int m_uiIndexXInTerrain;
		unsigned int m_uiIndexZInTerrain;
		virtual void Tessellate(const SKVector3 & CameraPos) = 0;
	};
	DECLARE_Ptr(SKCLodTerrainGeometry);
	SKTYPE_MARCO(SKCLodTerrainGeometry);
}
#endif