#ifndef SKLIGHTACTOR_H
#define SKLIGHTACTOR_H
#include "SKActor.h"
#include "SKLight.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKLightActor : public SKActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKLightActor();
		virtual ~SKLightActor() = 0;

		virtual unsigned int GetLightType() = 0;

		GET_TYPE_NODE(SKLight)
		virtual void Update(double dAppTime);
		DECLARE_INITIAL_NO_CLASS_FACTORY
		SKArray<SKUsedName> m_ShadowSceneNameArray;
		virtual void AddToSceneMap(SKSceneMap * pSceneMap);
	};
	DECLARE_Ptr(SKLightActor);
	SKTYPE_MARCO(SKLightActor);

	class SKGRAPHIC_API SKDirectionLightActor : public SKLightActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKDirectionLightActor();
		virtual ~SKDirectionLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return SKLight::LT_DIRECTION;
		}
		GET_TYPE_NODE(SKDirectionLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(SKDirectionLightActor);
	SKTYPE_MARCO(SKDirectionLightActor);


	class SKGRAPHIC_API SKPointLightActor : public SKLightActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKPointLightActor();
		virtual ~SKPointLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return SKLight::LT_POINT;
		}
		GET_TYPE_NODE(SKPointLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(SKPointLightActor);
	SKTYPE_MARCO(SKPointLightActor);


	class SKGRAPHIC_API SKSpotLightActor : public SKLightActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKSpotLightActor();
		virtual ~SKSpotLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return SKLight::LT_SPOT;
		}
		GET_TYPE_NODE(SKSpotLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(SKSpotLightActor);
	SKTYPE_MARCO(SKSpotLightActor);

	class SKGRAPHIC_API SKSkyLightActor : public SKLightActor
	{
		//RTTI
		DECLARE_RTTI;
	public:
		SKSkyLightActor();
		virtual ~SKSkyLightActor();
		DECLARE_INITIAL
		virtual unsigned int GetLightType()
		{
			return SKLight::LT_SKY;
		}
		GET_TYPE_NODE(SKSkyLight)
		virtual void CreateDefaultComponentNode();
	};
	DECLARE_Ptr(SKSkyLightActor);
	SKTYPE_MARCO(SKSkyLightActor);
}
#endif