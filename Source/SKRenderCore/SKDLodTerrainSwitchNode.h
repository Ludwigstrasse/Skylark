#ifndef SKDLODTERRAINSWITCHNODE_H
#define SKDLODTERRAINSWITCHNODE_H
#include "SKSwitchNode.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKDLodTerrainSwitchNode : public SKSwitchNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKDLodTerrainSwitchNode(unsigned int uiIndexXInTerrain,unsigned int uiIndexZInTerrain);
		virtual ~SKDLodTerrainSwitchNode();

	public:

		virtual void UpDateView(SKCuller & Culler,double dAppTime);
		
	
	protected:
		SKDLodTerrainSwitchNode();
		unsigned int m_uiIndexXInTerrain;
		unsigned int m_uiIndexZInTerrain;
	};
	DECLARE_Ptr(SKDLodTerrainSwitchNode);
	SKTYPE_MARCO(SKDLodTerrainSwitchNode);
}
#endif