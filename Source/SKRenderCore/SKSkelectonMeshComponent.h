#ifndef SKSKELECTONCOMPONENT_H
#define SKSKELECTONCOMPONENT_H
#include "SKMeshComponent.h"
namespace SKEngine2
{
	DECLARE_Proxy(SKSkelectonMeshNode);
	DECLARE_Ptr(SKSkelectonMeshNode);
	class SKGRAPHIC_API SKSkelectonMeshComponent : public SKMeshComponent
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKSkelectonMeshComponent();
		virtual ~SKSkelectonMeshComponent();
		void SetSkelectonMeshResource(SKSkelectonMeshNodeR * pSkelectonMeshResource);
		SKSkelectonMeshNode * GetSkelectonMeshNode();
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);
		virtual void PostCreate();
		void SetIsDrawSkelecton(bool bIsDrawSkelecton);
		
		virtual bool BeforeSave(void * pDate = NULL);
		bool PlayAnim(const SKString & AnimName, SKREAL fRatio = 1.0f, unsigned int uiRepeatType = SKController::RT_NONE);
		SKSocketNode * GetSocketNode(const SKUsedName & SocketName);
		void SetAnimTreeNodePara(const SKUsedName & ShowName, void * pPara);
		virtual bool PostLoad(void * pDate);
	protected:
		SKSkelectonMeshNodeRPtr m_pSkelectonMeshResource;
		

	};
	DECLARE_Ptr(SKSkelectonMeshComponent);
	SKTYPE_MARCO(SKSkelectonMeshComponent);
}
#endif