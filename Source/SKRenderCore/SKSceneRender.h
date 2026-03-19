#ifndef SKSCENERENDER_H
#define SKSCENERENDER_H
#include "SK2DTexture.h"
#include "SKDepthStencil.h"
#include "SKSpatial.h"
#include "SKScene.h"
#include "SKPostEffectSet.h"
#include "SKDebugDraw.h"
#include "SKRenderTarget.h"
namespace SKEngine2
{
	DECLARE_Ptr(SKCustomMaterial);
	class SKGRAPHIC_API SKSceneRenderInterface : public SKReference,public SKMemObject
	{
	public:
		SKSceneRenderInterface();
		virtual ~SKSceneRenderInterface() = 0;
		

		

		

		virtual SKRenderTarget * GetRenderTarget(unsigned int uiIndex);

		FORCEINLINE void SetParam(unsigned int uiClearFlag,SKColorRGBA ClearColorRGBA,SKREAL fClearDepth,
						unsigned int uiClearStencil,bool bUseViewClear = false)
		{
			m_uiClearFlag = uiClearFlag;
			m_ClearColorRGBA = ClearColorRGBA;
			m_fClearDepth = fClearDepth;
			m_uiClearStencil = uiClearStencil;
			m_bUseViewPortClear = bUseViewClear;

		}
		virtual bool AddRenderTarget(SKRenderTarget * pTarget);
		
		void SetRenderTargets();
		void EndRenderTargets();
		virtual void ClearRTAndDepth() = 0;
	protected:
		SKArray<SKRenderTargetPtr> m_pTargetArray;
		
		unsigned int m_uiClearFlag;
		SKColorRGBA m_ClearColorRGBA;
		SKREAL m_fClearDepth;
		unsigned int m_uiClearStencil;
		bool  m_bUseViewPortClear;
		
		unsigned int m_uiRTWidth;
		unsigned int m_uiRTHeight;
		unsigned int m_uiMulSample;
	};
	DECLARE_Ptr(SKSceneRenderInterface);
	
	class SKGRAPHIC_API SKSceneRender : public SKSceneRenderInterface
	{
	public:
		SKSceneRender();
		virtual ~SKSceneRender();
		
		FORCEINLINE SKDepthStencil * GetDepthStencil(unsigned int uiRenderGroup)const
		{
			if (uiRenderGroup >= SKCuller::RG_MAX)
			{
				return false;
			}
			return m_pDepthStencil[uiRenderGroup];
		}	
		virtual bool SetDepthStencil(SKDepthStencil * pDepthStencil,unsigned int uiRenderGroup);
		virtual bool Draw(SKCuller & Culler,double dAppTime);
		virtual void ClearRTAndDepth();
		virtual void DrawUseCurView(SKCuller & Culler,double dAppTime);
		virtual void DisableUseCurView(SKCuller & Culler,double dAppTime);
		void SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag);
		void ClearUseState();
	protected:
		virtual bool OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime) = 0;
		SKDepthStencilPtr m_pDepthStencil[SKCuller::RG_MAX];
		SKRenderState m_SaveRenderState;
		SKRenderState m_UseState;
		unsigned int m_uiRenderStateInheritFlag;
	};
	DECLARE_Ptr(SKSceneRender);


	class SKGRAPHIC_API SKMaterialSceneRender : public SKSceneRender
	{
	public:
		SKMaterialSceneRender();
		virtual ~SKMaterialSceneRender();
		virtual void DrawUseCurView(SKCuller & Culler,double dAppTime);
		virtual void DisableUseCurView(SKCuller & Culler,double dAppTime);
		void SetNormalDepthTexture(VS2DTexture * pNormalDepthTexture)
		{
			m_pNormalDepthTexture = pNormalDepthTexture;
		}
	protected:
		virtual bool OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		VS2DTexturePtr m_pNormalDepthTexture;

		SKArray<SKRenderContext *> m_NormalAndDepth;
		SKArray<SKRenderContext *> m_NoNormalOrDepth;
		SKArray<SKRenderContext *> m_Combine;
		SKArray<SKRenderContext *> m_AlphaBlend;
		void GetGroup(SKCuller & Culler, unsigned int uiRenderGroup);
		void DrawGroup(SKCuller & Culler, unsigned int uiRenderGroup, SKArray<SKRenderContext *> & Group, double dAppTime);
		void DrawProjectShadow(SKCuller & Culler, unsigned int uiRenderGroup, double dAppTime);
		void DrawVolumeShadow(SKCuller & Culler, unsigned int uiRenderGroup, double dAppTime);
	};
	DECLARE_Ptr(SKMaterialSceneRender);

	class SKGRAPHIC_API SKDebugDrawSceneRender : public SKSceneRender
	{
	public:
		SKDebugDrawSceneRender();
		virtual ~SKDebugDrawSceneRender();
		FORCEINLINE SKDebugDraw * GetDebugDraw(unsigned int uiRenderGroup)
		{
			if (uiRenderGroup >= SKCuller::RG_MAX)
			{
				return false;
			}
			return m_pDebugDraw[uiRenderGroup];
		}
	protected:
		virtual bool OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		SKDebugDrawPtr m_pDebugDraw[SKCuller::RG_MAX];
	};
	DECLARE_Ptr(SKDebugDrawSceneRender);


	class SKGRAPHIC_API SKNormalDepthSceneRender : public SKSceneRender
	{
	public:
		SKNormalDepthSceneRender();
		virtual ~SKNormalDepthSceneRender();
	protected:
		virtual bool OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
	};
	DECLARE_Ptr(SKNormalDepthSceneRender);


	class SKGRAPHIC_API SKPrezSceneRender : public SKSceneRender
	{
	public:
		SKPrezSceneRender();
		virtual ~SKPrezSceneRender();
	protected:
		virtual bool OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
	};
	DECLARE_Ptr(SKPrezSceneRender);

	
	class SKGRAPHIC_API SKPostEffectSceneRender : public SKSceneRenderInterface
	{
	protected:
		SKTexAllStatePtr m_pTexAllState;
		virtual bool OnDraw(SKCuller & Culler);
		SKDepthStencilPtr m_pDepthStencil;
		SKCustomMaterialPtr m_pCustomMaterial;
	public:
		virtual bool SetDepthStencil(SKDepthStencil * pDepthStencil);
		SKDepthStencil * GetDepthStencil()const
		{
			return m_pDepthStencil;
		}
		SKPostEffectSceneRender();
		virtual ~SKPostEffectSceneRender();
		virtual void GetRT(unsigned int uiWidth,unsigned int uiHeight);
		virtual void DisableRT();
		virtual void SetSourceTarget(SKTexture * pTexture);
		virtual bool Draw(SKCuller & Culler,double dAppTime);
		virtual void ClearRTAndDepth();
		virtual void SetNoUseRTRenderSize(unsigned int uiWidth,unsigned int uiHeight);
	};
	DECLARE_Ptr(SKPostEffectSceneRender);

	class SKGRAPHIC_API SKPEBlurSceneRender : public SKPostEffectSceneRender
	{
	protected:
		
	public:
		SKPEBlurSceneRender();
		virtual ~SKPEBlurSceneRender();

	};
	DECLARE_Ptr(SKPEBlurSceneRender);

	class SKGRAPHIC_API SKPEGraySceneRender : public SKPostEffectSceneRender
	{

	public:
		SKPEGraySceneRender();
		virtual ~SKPEGraySceneRender();	
		virtual void SetSourceTarget(SKTexture * pTexture);
	};
	DECLARE_Ptr(SKPEGraySceneRender);

	class SKGRAPHIC_API SKPESSRSceneRender : public SKPostEffectSceneRender
	{

	public:
		SKPESSRSceneRender();
		virtual ~SKPESSRSceneRender();	
		virtual void SetUseTexture(SKTexture * pNormalDepthTexture, SKTexture *pSceneColorTexture);
		virtual bool OnDraw(SKCuller & Culler);
	protected:
		SKTexAllStatePtr m_pNormalDepthTexture;
		SKTexAllStatePtr m_pSceneColorTexture;
	};
	DECLARE_Ptr(SKPESSRSceneRender);

	class SKGRAPHIC_API SKGammaCorrectSceneRender : public SKPostEffectSceneRender
	{

	public:
		SKGammaCorrectSceneRender();
		virtual ~SKGammaCorrectSceneRender();	
		virtual void SetSourceTarget(SKTexture * pTexture);
	};
	DECLARE_Ptr(SKGammaCorrectSceneRender);

	class SKGRAPHIC_API SKPEScreenQuadSceneRender : public SKPostEffectSceneRender
	{

	public:
		SKPEScreenQuadSceneRender();
		virtual ~SKPEScreenQuadSceneRender();
		virtual void SetSourceTarget(SKTexture * pTexture);
	};
	DECLARE_Ptr(SKPEScreenQuadSceneRender);

	class SKGRAPHIC_API SKPEMaterialSceneRender : public SKPostEffectSceneRender
	{
	public:
		SKPEMaterialSceneRender();
		virtual ~SKPEMaterialSceneRender();
		SKMaterialInstance * m_pMaterialInstacne;
	protected:
		
		virtual bool OnDraw(SKCuller & Culler);
	};
	DECLARE_Ptr(SKPEMaterialSceneRender);
	class SKGRAPHIC_API SKPEVolumeShadowMapSceneRender : public SKPostEffectSceneRender
	{

	public:
		SKPEVolumeShadowMapSceneRender();
		virtual ~SKPEVolumeShadowMapSceneRender();
	};
	DECLARE_Ptr(SKPEVolumeShadowMapSceneRender);

	class SKPointLight;
	class SKGRAPHIC_API SKShadowMapSceneRender : public SKSceneRender
	{
	public:
		enum //
		{
			SMT_CUB,
			SMT_SHADOWMAP,
			SMT_DUAL_PARABOLOID,
			SMT_MAX
		};
		SKShadowMapSceneRender(unsigned int uiShadowMapType);
		virtual ~SKShadowMapSceneRender();	
		SKLocalLight * m_pLocalLight;
		virtual bool Draw(SKCuller & Culler,double dAppTime);
	protected:
		virtual bool OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		unsigned int m_uiShadowMapType;
	};
	DECLARE_Ptr(SKShadowMapSceneRender);

	class SKMaterialInstance;
	class SKGRAPHIC_API SKLightMaterialSceneRender : public SKSceneRenderInterface
	{
	public:

		SKLightMaterialSceneRender();
		virtual ~SKLightMaterialSceneRender();	
		SKMaterialInstance * m_pMaterialInstacne;
		virtual bool Draw(SKCuller & Culler,double dAppTime);
		virtual bool SetDepthStencil(SKDepthStencil * pDepthStencil);
		SKDepthStencil * GetDepthStencil()const
		{
			return m_pDepthStencil;
		}
		virtual void ClearRTAndDepth();

		SKLight * m_pLight;
	protected:
		SKDepthStencilPtr m_pDepthStencil;
		virtual bool OnDraw(SKCuller & Culler);
	};
	DECLARE_Ptr(SKLightMaterialSceneRender);

	class SKGRAPHIC_API SKProjectShadowSceneRender : public SKSceneRender
	{
	public:

		SKProjectShadowSceneRender();
		virtual ~SKProjectShadowSceneRender();	
		SKLocalLight * m_pLocalLight;
		virtual bool Draw(SKCuller & Culler,double dAppTime);
		VS2DTexture * m_pNormalDepthTexture;
		SKREAL m_fLightRange;
		SKVector3 m_LightWorldDirection;
		SKREAL m_Falloff;
		SKREAL m_Theta;
		SKREAL m_Phi;
	protected:
		virtual bool OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime);
		SKTexAllStatePtr m_pTexAllState;
		SKShaderKey m_DirectionLightShaderKey;
		SKShaderKey m_PointLightShaderKey;
		SKShaderKey m_SpotLightShaderKey;
		SKShaderKey m_NULLShaderKey;

	};
	DECLARE_Ptr(SKProjectShadowSceneRender);
	class SKGRAPHIC_API SKVolumeShadowSceneRender : public SKSceneRender
	{
	public:

		SKVolumeShadowSceneRender();
		virtual ~SKVolumeShadowSceneRender();
		SKLocalLight * m_pLocalLight;
		virtual bool Draw(SKCuller & Culler, double dAppTime);
	protected:
		virtual bool OnDraw(SKCuller & Culler, unsigned int uiRenderGroup, double dAppTime);

	};
	DECLARE_Ptr(SKVolumeShadowSceneRender);
}
#endif