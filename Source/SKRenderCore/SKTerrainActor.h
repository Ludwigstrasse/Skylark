#ifndef SKTERRAINACTOR_H
#define SKTERRAINACTOR_H
#include "SKActor.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKTerrainActor : public SKActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKTerrainActor();
		virtual ~SKTerrainActor() = 0;

		DECLARE_INITIAL_NO_CLASS_FACTORY
	};
	DECLARE_Ptr(SKTerrainActor);
	SKTYPE_MARCO(SKTerrainActor);

	class SKGRAPHIC_API SKCLodTerrainActor : public SKTerrainActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKCLodTerrainActor();
		virtual ~SKCLodTerrainActor();
		DECLARE_INITIAL
		GET_TYPE_NODE(SKCLodTerrainNode)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(SKCLodTerrainActor);
	SKTYPE_MARCO(SKCLodTerrainActor);


	class SKGRAPHIC_API SKDLodTerrainActor : public SKTerrainActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKDLodTerrainActor();
		virtual ~SKDLodTerrainActor();
		DECLARE_INITIAL

		GET_TYPE_NODE(SKDLodTerrainNode)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(SKDLodTerrainActor);
	SKTYPE_MARCO(SKDLodTerrainActor);


}
#endif