#ifndef SKSKELECTONACTOR_H
#define SKSKELECTONACTOR_H
#include "SKActor.h"
#include "SKSkelectonMeshComponent.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKSkelectonActor : public SKActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		friend class SKWorld;
		SKSkelectonActor();
		virtual ~SKSkelectonActor();

		DECLARE_INITIAL


		GET_TYPE_NODE(SKSkelectonMeshComponent)
		//virtual void Update(double dAppTime);
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(SKSkelectonActor);
	SKTYPE_MARCO(SKSkelectonActor);
}
#endif