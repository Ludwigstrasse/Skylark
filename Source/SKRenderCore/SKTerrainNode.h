#ifndef SKTERRAINNODE_H
#define SKTERRAINNODE_H
#include "SKMeshNode.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKTerrainNode : public SKMeshNode
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum
		{
			MAX_TESSELATION_LEVEL = 8,
			MIN_TESSELATION_LEVEL = 1,
			MAX_NUM = 10,
			MIN_NUM = 1,
			WIDTH_SCALE = 50
		};

		virtual ~SKTerrainNode() = 0;
		virtual unsigned int GetResourceType()const
		{
			return RT_TERRAIN;
		}
	protected:
		SKTerrainNode();
		unsigned int m_uiTesselationLevel;		//ÿӲ㼶ӼܶȹϵGetChildNumX
		unsigned int m_uiNumX;					//ܶ
		unsigned int m_uiNumZ;
		unsigned int m_uiTotleNum;

		unsigned char *m_pHeight;
		SKREAL m_fHeightScale;
		virtual bool CreateChild() = 0;
		void SetNum(unsigned int uiNumX,unsigned int uiNumZ);
		void SetTesselationLevel(unsigned int uiTesselationLevel);
	public:
		FORCEINLINE unsigned int GetTesselationLevel()const { return m_uiTesselationLevel ;}
		FORCEINLINE unsigned int GetNumX()const{return m_uiNumX;}
		FORCEINLINE unsigned int GetNumZ()const{return m_uiNumZ;}
		FORCEINLINE unsigned int GetChildNumX()const {return 1<<(m_uiNumX - m_uiTesselationLevel);}
		FORCEINLINE unsigned int GetChildNumZ()const {return 1<<(m_uiNumZ - m_uiTesselationLevel);}

		
		
		bool CreateTerrain(unsigned int uiNumX,unsigned int uiNumZ,unsigned int uiTesselationLevel);
		bool CreateTarrainFromHeightMap(const TCHAR *pFileName,unsigned int uiTesselationLevel,SKREAL fHeightScale);
		
		FORCEINLINE SKREAL GetHeightScale()const
		{
			return m_fHeightScale;
		}
		unsigned char GetRawHeight(unsigned int uiX,unsigned int uiZ);
		SKREAL GetHeight(unsigned int uiX,unsigned int uiZ);
	};
	DECLARE_Ptr(SKTerrainNode);
	SKTYPE_MARCO(SKTerrainNode);
}
#endif