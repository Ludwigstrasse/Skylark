#ifndef SKSWITCHNODE_H
#define SKSWITCHNODE_H
#include "SKNode.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKSwitchNode : public SKNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKSwitchNode();
		virtual ~SKSwitchNode();


		virtual SKSpatial * GetActiveNode()const;
	protected:
		unsigned int m_uiActiveNode;

	protected:
		virtual void UpdateWorldBound(double dAppTime);//߽ 

		virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);
	};
	DECLARE_Ptr(SKSwitchNode);
	SKTYPE_MARCO(SKSwitchNode);

}
#endif