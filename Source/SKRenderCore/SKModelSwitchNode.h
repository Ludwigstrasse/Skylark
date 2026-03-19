#ifndef SKMODELSWITCHNODE_H
#define SKMODELSWITCHNODE_H
#include "SKSwitchNode.h"
#include "SKGeometryNode.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKModelSwitchNode : public SKSwitchNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKModelSwitchNode();
		virtual ~SKModelSwitchNode();
	public:
		virtual SKGeometryNode * GetGeometryNode(unsigned int uiLodLevel);
		virtual void UpDateView(SKCuller & Culler, double dAppTime);
	};
	DECLARE_Ptr(SKModelSwitchNode);
	SKTYPE_MARCO(SKModelSwitchNode);

}
#endif