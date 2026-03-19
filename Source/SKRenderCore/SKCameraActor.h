#ifndef SKCAMERAACTOR_H
#define SKCAMERAACTOR_H
#include "SKActor.h"
#include "SKCamera.h"
#include "SKPostEffectSet.h"
namespace SKEngine2
{
	class SKViewFamily;
	class SKGRAPHIC_API SKViewFamliyInfo : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		DECLARE_INITIAL
		SKViewFamliyInfo();
		~SKViewFamliyInfo();
		SKArray<SKString> m_SceneMapName;
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiCaptureViewFamilyType;
		SKString	m_ViewFamilyName;
		SKPostEffectSetRPtr m_pPostEffect;
		SKViewFamily * m_pViewFamily;
		SKString	m_RenderMethodRTTIName;
		void GetScene();
	};
	DECLARE_Ptr(SKViewFamliyInfo);
	SKTYPE_MARCO(SKViewFamliyInfo);
	class  SKGRAPHIC_API SKCameraActor : public SKActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKCameraActor();
		virtual ~SKCameraActor();
		DECLARE_INITIAL

		SKArray<SKViewFamliyInfoPtr> m_ViewFamliyInfoArray;
		GET_TYPE_NODE(SKCamera)
		virtual void Update(double dAppTime);
		virtual void CreateDefaultComponentNode();
		void AddCustomCullPlane(const SKPlane3& P);
		void ClearCustomCullPlane();
	};
	DECLARE_Ptr(SKCameraActor);
	SKTYPE_MARCO(SKCameraActor);
}
#endif