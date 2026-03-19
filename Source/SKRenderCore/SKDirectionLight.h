#ifndef SKDIRECTIONLIGHT_H
#define SKDIRECTIONLIGHT_H
#include "SKLight.h"
#include "SKCubeTexture.h"
#include "SKRenderTarget.h"
#include "SKDepthStencil.h"
#include "SKSceneRender.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKDirectionLight : public SKLocalLight
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKDirectionLight();
		virtual ~SKDirectionLight();
	public:
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual bool PostLoad(void * pDate = NULL);
		virtual unsigned int GetLightType()const{return LT_DIRECTION;}

		enum //ShadowType
		{
			ST_VOLUME,
			ST_OSM,
			ST_CSM,
			ST_PROJECT,
			ST_MAX
		};
		void SetShadowType(unsigned int uiShadowType);
		FORCEINLINE unsigned int GetShadowType()const
		{
			return m_uiShadowType;
		}
		virtual void DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DisableDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
		virtual void DrawVolumeShadow(SKCuller & CurCuller, double dAppTime);

		SKMatrix3X3W m_CSMLightShadowMatrix[3];



	protected:
	
		unsigned int m_uiShadowType;

		SKProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;
		SKShadowMapSceneRenderPtr m_pShadowMapSceneRender;
		SKPEVolumeShadowMapSceneRenderPtr m_pPEVolumeSMSceneRender;
		SKVolumeShadowSceneRenderPtr m_pVolumeShadowSceneRender;

		SKRenderTargetPtr m_pVolumeShadowRenderTarget;

#define m_pOSMShadowRenderTarget m_pVolumeShadowRenderTarget
#define m_pProjectShadowRenderTarget m_pVolumeShadowRenderTarget
		SKArray<SKRenderTargetPtr> m_pCSMRTArray;

		void DrawOSM(SKCuller & CurCuller,double dAppTime);
		void DrawCSM(SKCuller & CurCuller,double dAppTime);


		SKAABB3 GetMaxAABB(SKArray<SKAABB3> &AABBArray);


		void ResetShadow();

		float m_fLightFunWidth;
		float m_fLightFunHeight;
		float m_fLightFunFar;
		float m_fLightFunNear;
		virtual void GetLightRange();
		SKAABB3 m_WorldRenderBV;
	public:
		FORCEINLINE void SetLightFuncParam(float fLightFunWidth,
			float fLightFunHeight,
			float fLightFunFar,
			float fLightFunNear = 1.0f)
		{
			m_fLightFunWidth = fLightFunWidth;
			m_fLightFunHeight = fLightFunHeight;
			m_fLightFunFar = fLightFunFar;
			m_fLightFunNear = fLightFunNear;
			GetLightRange();
		}
		virtual bool SetLightMaterial(SKMaterialR * pMaterial);
		virtual bool Cullby(SKCuller & Culler);
		virtual bool IsRelative(SKGeometry * pGeometry);
	};
	DECLARE_Ptr(SKDirectionLight);
	SKTYPE_MARCO(SKDirectionLight);

}
#endif