#ifndef SKSTATICMESHCOMPONENT_H
#define SKSTATICMESHCOMPONENT_H
#include "SKMeshComponent.h"
namespace SKEngine2
{

	DECLARE_Ptr(SKStaticMeshNode);
	DECLARE_Proxy(SKStaticMeshNode);
	class SKGRAPHIC_API SKStaticMeshComponent : public SKMeshComponent
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKStaticMeshComponent();
		virtual ~SKStaticMeshComponent();
		void SetStaticMeshResouce(SKStaticMeshNodeR * pStaticMeshResource);
		SKStaticMeshNode * GetStaticMeshNode();
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);
		virtual void PostCreate();
		virtual bool BeforeSave(void * pDate = NULL);
		virtual bool PostLoad(void * pDate = NULL);
		
	protected:
		SKStaticMeshNodeRPtr m_pStaticMeshResource;
		

	};
	DECLARE_Ptr(SKStaticMeshComponent);
	SKTYPE_MARCO(SKStaticMeshComponent);
}
#endif