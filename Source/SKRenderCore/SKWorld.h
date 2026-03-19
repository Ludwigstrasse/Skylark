#ifndef SKWORLD_H
#define SKWORLD_H
#include "SKObject.h"
#include "SKVector3.h"
#include "SKQuat.h"

#include "SKActor.h"
#include "SKSceneMap.h"
#include "SKCameraActor.h"
namespace SKEngine2
{
	class SKStream;
	class SKLightActor;
	class SKGRAPHIC_API SKWorld : public SKObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		SKWorld();
		virtual ~SKWorld();

		DECLARE_INITIAL
		static SKWorld * ms_pWorld;

	protected:
		SKArray<SKSceneMapPtr>	m_SceneArray;
		SKArray<SKActor *>	m_ActorArray;
		SKArray<SKMessage> m_MessageArray;

	public:
		
		
		SKSceneMap* CreateScene(const TCHAR * pName);

		SKSceneMap * GetScene(const SKUsedName & Name);
		
		SKActor * CreateActor(const TCHAR *  ActorPath, const SKVector3 & Pos = SKVector3::ms_Zero, const SKMatrix3X3 & Rotate = SKMatrix3X3::ms_Identity, const SKVector3 &Scale = SKVector3::ms_One, SKSceneMap * pSceneMap = NULL);
		void DestroyActor(SKActor * pActor);

		template<typename T>
		SKActor *CreateActor(const SKVector3 & Pos = SKVector3::ms_Zero, const SKMatrix3X3 & Rotate = SKMatrix3X3::ms_Identity, const SKVector3 &Scale = SKVector3::ms_One, SKSceneMap * pSceneMap = NULL);

		bool LoadMap(const TCHAR * MapPath,bool IsAsyn);
		bool UnLoadMap(const TCHAR * MapPath);
		void AttachCaptureViewFamilyToCamera(SKCameraActor * pCameraActor,
			unsigned int uiCaptureViewFamilyType,const SKString & ViewFamilyName,
			unsigned int uiWidth,unsigned int uiHeight,
			SKArray<SKString> & SceneMapName,
			const TCHAR * RenderMethodRTTIName,
			SKPostEffectSetR *  pPostEffectSet = NULL);
		void AttachWindowViewFamilyToCamera(SKCameraActor * pCameraActor,
			unsigned int uiWindowViewFamilyType,const SKString & ViewFamilyName,
			SKArray<SKString> & SceneMapName,
			const TCHAR * RenderMethodRTTIName,
			int iWindowID = -1,
			SKPostEffectSetR *  pPostEffectSet = NULL);
		SKActor *GetActor(SKUsedName Name);
		void Update(double dAppTime);
		void AddMessage(const SKMessage & Message);
		void UpdateLightActorScene(SKLightActor * pActor);
		void ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z);
		
	protected:
		friend class SKResourceManager;
		void AddActor(SKActor * pActor);
		void DeleteActor(SKActor * pActor);
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy,int Data = 0);
		void DestroyScene(const SKUsedName & Name);
		void AddSceneMap(SKSceneMap * pSceneMap);
	};
	DECLARE_Ptr(SKWorld);
	SKTYPE_MARCO(SKWorld);
	template<typename T>
	SKActor *SKWorld::CreateActor(const SKVector3 & Pos, const SKMatrix3X3 & Rotate, const SKVector3 &Scale, SKSceneMap * pSceneMap)
	{
		if (!T::ms_Type.IsDerived(SKActor::ms_Type))
			return NULL;
		SKActor * pActor = NULL;
		pActor = (SKActor*)SKObject::GetInstance<T>();
		pActor->CreateDefaultComponentNode();
		pActor->SetWorldPos(Pos);
		pActor->SetWorldRotate(Rotate);
		pActor->SetWorldScale(Scale);
		if (pSceneMap)
		{
			pSceneMap->AddActor(pActor);
		}
		else
		{
			m_SceneArray[0]->AddActor(pActor);
		}
		AddActor(pActor);
		return pActor;
	}
}
#endif