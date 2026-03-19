#ifndef SKSOCKETNODE_H
#define SKSOCKETNODE_H
#include "SKSpatial.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKSocketNode : public SKSpatial
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class SKNodeComponent;
		friend class SKSkelectonMeshNode;
		SKSocketNode();
		virtual ~SKSocketNode();
		virtual void UpdateWorldBound(double dAppTime){};
		
		virtual void ComputeNodeVisibleSet(SKCuller & Culler, bool bNoCull, double dAppTime){};
		virtual void ComputeVisibleSet(SKCuller & Culler, bool bNoCull, double dAppTime){};
		SKUsedName m_cName;
	protected:
		virtual void UpdateNodeAll(double dAppTime);
	};
	DECLARE_Ptr(SKSocketNode);
	SKTYPE_MARCO(SKSocketNode);
}
#endif