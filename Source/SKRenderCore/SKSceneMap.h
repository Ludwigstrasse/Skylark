#ifndef SKSCENEMAP_H
#define SKSCENEMAP_H
#include "SKScene.h"
#include "SKActor.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKSceneMap : public SKObject, public SKResource
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKSceneMap(const TCHAR * pName);
		SKSceneMap();
		virtual ~SKSceneMap();
		void AddActor(SKActor * pActor);
		SKActor *GetActor(SKUsedName Name);
		void DeleteActor(SKActor * pActor);
		SKUsedName m_Name;
		friend class SKViewFamliyInfo;
		friend class SKWorld;
		friend class SKReflectCubLightActor;
		virtual bool PostLoad(void * pDate = NULL);
		SKScene *GetScene()
		{
			return m_pScene;
		}
		virtual void OnDestroy();
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		static SKPointer<SKSceneMap> Default;
	public:
		static const SKSceneMap *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual unsigned int GetResourceType()const
		{
			return RT_MAP;
		}
	protected:

		SKScenePtr m_pScene;
		SKArray<SKActorPtr> m_pActorArray;

	};
	DECLARE_Ptr(SKSceneMap);
	SKTYPE_MARCO(SKSceneMap);
	DECLARE_Proxy(SKSceneMap);
}
#endif