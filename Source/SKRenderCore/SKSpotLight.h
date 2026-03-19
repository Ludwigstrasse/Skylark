#ifndef SKSPOTLIGHT_H
#define SKSPOTLIGHT_H
#include "SKLight.h"
#include "SKSceneRender.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKSpotLight : public SKLocalLight
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //ShadowType
		{
			ST_NORMAL,
			ST_VOLUME,
			ST_PROJECT,
			ST_MAX
		};
		SKSpotLight();
		virtual ~SKSpotLight();

	public:
		FORCEINLINE void Set(SKREAL		Range,
			SKREAL		Falloff,
			SKREAL		Theta,
			SKREAL		Phi)
		{
			m_Range = Range;
			m_Falloff = Falloff;
			m_Theta = Theta;
			m_Phi = Phi;
			GetLightRange();
		}
		FORCEINLINE SKREAL GetRange()const
		{
			return m_Range;
		}
		FORCEINLINE SKREAL GetFalloff()const
		{
			return m_Falloff;
		}
		FORCEINLINE SKREAL GetTheta()const
		{
			return m_Theta;
		}
		FORCEINLINE SKREAL GetPhi()const
		{
			return m_Phi;
		}
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual bool PostLoad(void * pDate = NULL);
		virtual unsigned int GetLightType()const{return LT_SPOT;}
		virtual bool Cullby(SKCuller & Culler);
		virtual bool IsRelative(SKGeometry * pGeometry);
		void ResetShadow();
		void SetShadowType(unsigned int uiShadowType);
		virtual void DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DisableDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DrawNoDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DisableNoDepandenceShadowMap(double dAppTime);
		FORCEINLINE unsigned int GetShadowType()const
		{
			return m_uiShadowType;
		}
		virtual void DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
	protected:
		SKShadowMapSceneRenderPtr m_pShadowMapSceneRender;
		unsigned int m_uiShadowType;
		void DrawNormalShadowMap(SKCuller & CurCuller,double dAppTime);		

		SKRenderTargetPtr m_pShadowRenderTarget;
		SKProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;

		virtual void GetLightRange();
		SKAABB3 m_WorldRenderBV;
		SKREAL		m_Range;
		SKREAL		m_Falloff;
		SKREAL		m_Theta;
		SKREAL		m_Phi;
	};
	DECLARE_Ptr(SKSpotLight);
	SKTYPE_MARCO(SKSpotLight);

}
#endif