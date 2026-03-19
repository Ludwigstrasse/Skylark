#ifndef SKMESHNODE_H
#define SKMESHNODE_H
#include "SKNode.h"
#include "SKResource.h"
namespace SKEngine2
{
	class SKGeometryNode;
	class SKSwitchNode;
	class SKStream;
	class SKGRAPHIC_API SKMeshNode : public SKNode,public SKResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		SKMeshNode();
		virtual ~SKMeshNode();
		virtual unsigned int GetResourceType()const
		{
			return RT_MAX;
		}
		FORCEINLINE void SetIsDrawBoundVolume(bool bIsDrawBoundVolume)
		{
			m_bIsDrawBoundVolume = bIsDrawBoundVolume;
		}
		FORCEINLINE unsigned int GetRenderGroup()const
		{
			return m_uiRenderGroup;
		}
		FORCEINLINE void SetRenderGroup(unsigned int uiRenderGroup)
		{
			m_uiRenderGroup = uiRenderGroup;
		}

		virtual void UpDateView(SKCuller & Culler,double dAppTime);

		virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);
		bool m_bReceiveShadow;
		bool m_bCastShadow;
		bool m_bLighted;
	protected:
		bool m_bIsDrawBoundVolume;
		unsigned int m_uiRenderGroup;
	};
	DECLARE_Ptr(SKMeshNode);
	SKTYPE_MARCO(SKMeshNode);
}
#endif