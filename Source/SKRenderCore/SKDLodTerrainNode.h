#ifndef SKDLODTERRAINNODE_H
#define SKDLODTERRAINNODE_H
#include "SKTerrainNode.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKDLodTerrainNode : public SKTerrainNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		SKDLodTerrainNode();
		virtual ~SKDLodTerrainNode();
	public:
		FORCEINLINE void SetDLodExtend(int uiDLodExtend)
		{
			m_uiDLodExtend = uiDLodExtend;
		}
		FORCEINLINE int GetDLodExtend()const
		{
			return m_uiDLodExtend;
		}

		FORCEINLINE void SetDLodScale(SKREAL fDLodScale)
		{
			m_fDLodScale = fDLodScale;
		}

		FORCEINLINE SKREAL GetDLodScale()const { return m_fDLodScale; }
		
	protected:
		int m_uiDLodExtend;
		SKREAL m_fDLodScale;
		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		
	};
	DECLARE_Ptr(SKDLodTerrainNode);
	SKTYPE_MARCO(SKDLodTerrainNode);


}
#endif