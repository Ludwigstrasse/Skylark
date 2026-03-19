#ifndef SKSTATICACTOR_H
#define SKSTATICACTOR_H
#include "SKActor.h"
#include "SKStaticMeshComponent.h"
namespace SKEngine2
{
	DECLARE_Proxy(SKStaticMeshNode);
	class SKGRAPHIC_API SKStaticActor : public SKActor
	{
		//PRIORITY
	
		//RTTI
		DECLARE_RTTI;
	public:
		friend class SKWorld;
		SKStaticActor();
		virtual ~SKStaticActor();
		DECLARE_INITIAL


		GET_TYPE_NODE(SKStaticMeshComponent)
		virtual void CreateDefaultComponentNode();

	};
	DECLARE_Ptr(SKStaticActor);
	SKTYPE_MARCO(SKStaticActor);
}
#endif