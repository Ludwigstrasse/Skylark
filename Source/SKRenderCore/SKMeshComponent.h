#ifndef SKMESHCOMPONENT_H
#define SKMESHCOMPONENT_H
#include "SKNodeComponent.h"
namespace SKEngine2
{

	DECLARE_Ptr(SKModelMeshNode);
	class SKGRAPHIC_API SKMeshComponent : public SKNodeComponent
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKMeshComponent();
		virtual ~SKMeshComponent() = 0;
		virtual bool IsNeedDraw()
		{
			return true;
		}
	protected:
		virtual void UpdateWorldBound(double dAppTime);//߽ 

		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(SKCuller & Culler, bool bNoCull, double dAppTime);
		virtual void SetPostLoadNodeParam();
	public:
		virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);
		virtual void CreateLocalAABB();
		void SetIsDrawBoundVolume(bool bIsDrawBoundVolume);

		void SetMorphTreeNodePara(const SKUsedName & ShowName, void * pPara);

		void CastShadow(bool bCastShadow);


		SKMaterialInstance * GetUseMaterialInstance(unsigned int SubMeshID)const;

		SKMaterialInstance * GetMaterialInstance(unsigned int SubMeshID,unsigned int UseID)const;

		bool SetUseMaterialInstance(unsigned int SubMeshID, unsigned int UseID);

		unsigned int GetSubMeshNum();
		virtual void DelayUpdate();
	protected:
		SKModelMeshNodePtr m_pNode;
		//Cache Last ModelMeshNode m_pNodeother render thread will crash when it is delete.
		SKModelMeshNodePtr m_pSaveNode;
		SKArray<unsigned int> m_UseID;
		bool m_bCastShadow;
		void ResetUseID();
	};
	DECLARE_Ptr(SKMeshComponent);
	SKTYPE_MARCO(SKMeshComponent);
}
#endif