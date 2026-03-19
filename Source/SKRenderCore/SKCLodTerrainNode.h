#ifndef SKCLODTERRAINNODE_H
#define SKCLODTERRAINNODE_H
#include "SKTerrainNode.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKCLodTerrainNode : public SKTerrainNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Terrain Node Type
		{
			TNT_ROAM,
			TNT_QUAD,
			TNT_MAX
		};
		virtual ~SKCLodTerrainNode();
		SKCLodTerrainNode();
	protected:
		

		
		SKREAL	m_fCLODScale;

		unsigned int m_uiTerrainNodeType;
		void LinkNeighbor();
		void ComputeVariance();

		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		virtual unsigned int AddChild(SKSpatial * pChild);

		void SetCLODScale(SKREAL fCLODScale);

		FORCEINLINE SKREAL GetCLODScale()const { return m_fCLODScale; }

		void SetTerrainNodeType(unsigned int uiTerrainNodeType);
	};
	DECLARE_Ptr(SKCLodTerrainNode);
	SKTYPE_MARCO(SKCLodTerrainNode);
}
#endif