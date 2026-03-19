#ifndef SKPOINTLIGHT_H
#define SKPOINTLIGHT_H
#include "SKLight.h"
#include "SKCubeTexture.h"
#include "SKRenderTarget.h"
#include "SKDepthStencil.h"
#include "SKSceneRender.h"
namespace SKEngine2
{
	//cube shadow ڹԴ̫Զrtֱʲž徫Ȳ
	//volume ڲͶӰǱպϵģǹ⻬ģͬ⻬ǵĵطҪǶΡ
	class SKStream;
	class SKGRAPHIC_API SKPointLight : public SKLocalLight
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKPointLight();
		virtual ~SKPointLight();
	public:
		
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual bool PostLoad(void * pDate = NULL);
		
		FORCEINLINE void SetRange(SKREAL Range)
		{
			m_Range = Range;
			GetLightRange();
		}
		FORCEINLINE SKREAL GetRange()const
		{
			return m_Range;
		}
		virtual void BuildSceneInfo(SKScene * pScene);
		virtual unsigned int GetLightType()const{return LT_POINT;}
		virtual void DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DisableDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DrawNoDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DisableNoDepandenceShadowMap(double dAppTime);
		enum //ShadowType
		{
			ST_CUB,
			ST_VOLUME,
			ST_DUAL_PARABOLOID,
			ST_PROJECT,
			ST_MAX
		};
		virtual bool Cullby(SKCuller & Culler);
		void SetShadowType(unsigned int uiShadowType);
		FORCEINLINE unsigned int GetShadowType()const
		{
			return m_uiShadowType;
		}
		virtual bool IsRelative(SKGeometry * pGeometry);
		virtual void DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
		virtual void DrawVolumeShadow(SKCuller & CurCuller, double dAppTime);
	protected:
		void ResetShadow();

		void DrawNormalCubShadow(SKCuller & CurCuller,double dAppTime);
		void DrawFovCubShadow(SKCuller & CurCuller,double dAppTime);

		void DrawNormalProjectShadow(SKCuller & CurCuller,double dAppTime);
		void DrawFovProjectShadow(SKCuller & CurCuller,double dAppTime);

		void DrawDualParaboloidShadow(SKCuller & CurCuller, double dAppTime);


		SKProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;
		
		unsigned int m_uiShadowType;
		SKRenderTargetPtr m_pCubRenderTarget[SKCubeTexture::F_MAX];
		
		SKShadowMapSceneRenderPtr m_pShadowMapSceneRender;

		SKPEVolumeShadowMapSceneRenderPtr m_pPEVolumeSMSceneRender;
		SKVolumeShadowSceneRenderPtr m_pVolumeShadowSceneRender;

		SKRenderTargetPtr m_pVolumeShadowRenderTarget;
		SKRenderTargetPtr m_pVolumeShadowFirstPassRenderTarget;

#define m_pDualParaboloidRenderTarget0 m_pVolumeShadowFirstPassRenderTarget
#define m_pDualParaboloidRenderTarget1 m_pVolumeShadowRenderTarget

#define m_pProjectShadowRenderTarget m_pVolumeShadowRenderTarget

		
		virtual void GetLightRange();
		SKSphere3 m_WorldRenderBV;
		SKREAL		m_Range;
	public:
		friend class SKVolumeShadowMapCuller;
		friend class SKDualParaboloidCuller;
	};
	DECLARE_Ptr(SKPointLight);
	SKTYPE_MARCO(SKPointLight);


}
#endif