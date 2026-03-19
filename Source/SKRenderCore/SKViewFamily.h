#ifndef SKVIEWFAMILY_H
#define SKVIEWFAMILY_H
#include "SKSystem.h"
#include "SKReference.h"
#include "SKCamera.h"
#include "SKSceneRender.h"
#include "SKCubeTexture.h"
namespace SKEngine2
{
	DECLARE_Proxy(SKPostEffectSet);
	class SKGRAPHIC_API SKSceneRenderMethod : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		SKSceneRenderMethod();
		virtual ~SKSceneRenderMethod() = 0;
		void SetPostEffect(SKPostEffectSetR* pPostEffectSet);
		virtual void Draw(SKCuller & Culler,double dAppTime);
		virtual void GetRT(unsigned int uiWidth,unsigned int uiHeight);
		virtual void DisableRT();
		FORCEINLINE void SetDebugDrawInfo(bool bDrawDebugInfo, bool bBeforePostDebugDraw = true)
		{
			m_bBeforePostDebugDraw = bBeforePostDebugDraw;
			m_bDrawDebugInfo = bDrawDebugInfo;
		}
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);
		SKRenderTarget * GetFinalColorRT();

		SKPostEffectSet * GetPostEffectSet()const
		{
			return m_pPostEffectInstance;
		}
		SKDebugDraw * GetDebugDraw(unsigned int uiRenderGroup);
		virtual void SetUseState(SKRenderState & RenderState,unsigned int uiRenderStateInheritFlag) = 0;
		virtual void ClearUseState() = 0;

		// must use in viewfamily OnDraw , every frame you call this , and pFinalColorRT  must youself manage.
		virtual void SetColorRT(SKRenderTarget * pFinalColorRT,unsigned int uiWidth,unsigned int uiHeight);
	protected:
		SKPostEffectSetRPtr m_pPostEffectSet;
		SKPostEffectSetPtr	m_pPostEffectInstance;
		SKDebugDrawSceneRenderPtr m_pDebugDrawSceneRender;
		bool m_bBeforePostDebugDraw;
		bool m_bDrawDebugInfo;
		SKDepthStencilPtr m_pDepthStencil[SKCuller::RG_MAX];
		SKRenderTargetPtr m_pColorRT;
		bool ColorRTIsOutSet;
	};
	DECLARE_Ptr(SKSceneRenderMethod);
	SKTYPE_MARCO(SKSceneRenderMethod);
	class SKGRAPHIC_API SKForwordHighEffectSceneRenderMethod : public SKSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKForwordHighEffectSceneRenderMethod();
		~SKForwordHighEffectSceneRenderMethod();
		virtual void Draw(SKCuller & Culler,double dAppTime);
		virtual void GetRT(unsigned int uiWidth,unsigned int uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(SKRenderState & RenderState,unsigned int uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:	
		SKMaterialSceneRenderPtr m_pMaterialSceneRenderder;
		SKNormalDepthSceneRenderPtr m_pNormalDepthSceneRender;
		SKGammaCorrectSceneRenderPtr m_pGammaCorrectSceneRender;
		SKPESSRSceneRenderPtr m_pSSRSceneRender;
		SKRenderTargetPtr m_pMaterialRT;
		SKRenderTargetPtr m_pNormalDepthRT;
		SKRenderTargetPtr m_pSSRRT;
	};
	DECLARE_Ptr(SKForwordHighEffectSceneRenderMethod);
	SKTYPE_MARCO(SKForwordHighEffectSceneRenderMethod);
	
	class SKGRAPHIC_API SKForwordEffectSceneRenderMethod : public SKSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKForwordEffectSceneRenderMethod();
		~SKForwordEffectSceneRenderMethod();
		virtual void Draw(SKCuller & Culler, double dAppTime);
		virtual void GetRT(unsigned int uiWidth, unsigned int uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:
		SKMaterialSceneRenderPtr m_pMaterialSceneRenderder;
		SKGammaCorrectSceneRenderPtr m_pGammaCorrectSceneRender;
		SKRenderTargetPtr m_pMaterialRT;
	};
	DECLARE_Ptr(SKForwordEffectSceneRenderMethod);
	SKTYPE_MARCO(SKForwordEffectSceneRenderMethod);

	class SKGRAPHIC_API SKSimpleForwordEffectSceneRenderMethod : public SKSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKSimpleForwordEffectSceneRenderMethod();
		~SKSimpleForwordEffectSceneRenderMethod();
		virtual void Draw(SKCuller & Culler, double dAppTime);
		virtual void GetRT(unsigned int uiWidth, unsigned int uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:
		SKMaterialSceneRenderPtr m_pMaterialSceneRenderder;
	};
	DECLARE_Ptr(SKSimpleForwordEffectSceneRenderMethod);
	SKTYPE_MARCO(SKSimpleForwordEffectSceneRenderMethod);
	class SKGRAPHIC_API SKViewFamily : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		
		SKViewFamily(const SKString &ViewFamilyName, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName);
		SKViewFamily(const SKString &ViewFamilyName,SKCamera * pCamera);
		virtual ~SKViewFamily();
		FORCEINLINE void AddScene(SKScene* pScene);
		FORCEINLINE SKScene * GetScene(unsigned int i)const;
		FORCEINLINE void ClearAllScene();
		FORCEINLINE void DeleteScene(unsigned int i);
		FORCEINLINE void DeleteScene(SKScene * pScene);
		FORCEINLINE unsigned int GetSceneNum()const;
		virtual void Draw(double dAppTime);
		virtual void OnDraw(double dAppTime);
		virtual void Update(double dAppTime);

		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		bool IsReCreate(unsigned int uiWidth,unsigned int uiHeight);

		SKString m_ViewFamilyName;
		bool m_bEnable;

		SKSceneRenderMethodPtr m_pSceneRenderMethod;

		virtual SKRenderTarget * GetFinalColorRT();

		void SetCullUseID(int USeID = -1)
		{
			m_Culler.SetUseMaterialIndex(USeID);
		}
	protected:
		SKCamera* m_pCamera;
		SKArray<SKScene *> m_pScene;
		SKCuller	m_Culler;

	protected:
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;	


		
	};
	FORCEINLINE void SKViewFamily::AddScene(SKScene * pSecen)
	{
		if(pSecen)
		{
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				if (m_pScene[i] == pSecen)
				{
					return ;
				}
			}
			m_pScene.AddElement(pSecen);
		}
	}
	FORCEINLINE SKScene * SKViewFamily::GetScene(unsigned int i)const
	{
		if(i >= m_pScene.GetNum())
			return NULL;
		return m_pScene[i];
	}
	FORCEINLINE void SKViewFamily::ClearAllScene()
	{
		m_pScene.Clear();
	}
	FORCEINLINE void SKViewFamily::DeleteScene(unsigned int i)
	{	
		if(i >= m_pScene.GetNum())
			return ;
		m_pScene.Erase(i);

	}
	FORCEINLINE void SKViewFamily::DeleteScene(SKScene * pScene)
	{
		if (!pScene)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pScene.GetNum() ; i++)
		{
			if(pScene == m_pScene[i])
			{
				m_pScene.Erase(i);
			}
		}
	}

	FORCEINLINE unsigned int SKViewFamily::GetSceneNum()const
	{
		return m_pScene.GetNum();
	}
	class SKGRAPHIC_API SKWindowViewFamily : public SKViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		enum 
		{
			VT_WINDOW_NORMAL,
			VT_WINDOW_MAX
		};
		SKWindowViewFamily(const SKString &ViewFamilyName, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName, int iWindowID = -1);
		SKWindowViewFamily(const SKString &ViewFamilyName,SKCamera * pCamera,int iWindowID = -1);
		virtual ~SKWindowViewFamily();
		virtual void Draw(double dAppTime);
		virtual void OnDraw(double dAppTime);
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);

	protected:
		int m_iWindowID;
		SKPEScreenQuadSceneRenderPtr  m_pScreenQuadRenderer;
	};

	class SKGRAPHIC_API SKCaptureViewFamily : public SKViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		enum //ViewFamily Type
		{
			VT_CAPTURE_2D_CAPTURE_NORMAL,
			VT_CAPTURE_CUB_CAPTURE_NORMAL,
			VT_MAX
		};
		SKCaptureViewFamily(const SKString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName);
		SKCaptureViewFamily(const SKString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,SKCamera * pCamera);
		virtual ~SKCaptureViewFamily();
		virtual void OnDraw(double dAppTime);
		void SetSize(unsigned int uiWidth,unsigned int uiHeight);
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		virtual SKTexture * GetTexture()const = 0;
		
		virtual void Draw(double dAppTime);
		friend class SKCaptureTexAllState;
	protected:
		SKPEScreenQuadSceneRenderPtr  m_pScreenQuadRenderer;
		SKCaptureTexAllState*  m_pTexOwner;
		bool m_OnlyUpdateOneTime;
		
	};
	class SKGRAPHIC_API SKCubCaptureViewFamily : public SKCaptureViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		SKCubCaptureViewFamily(const SKString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName);
		SKCubCaptureViewFamily(const SKString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,SKCamera * pCamera);
		virtual ~SKCubCaptureViewFamily();
		virtual void OnDraw(double dAppTime);
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		virtual void Update(double dAppTime);
		virtual SKTexture * GetTexture()const;
	protected:
		SKCameraPtr CubCameraPtr[SKCubeTexture::F_MAX];
		SKCuller m_CubCuller[SKCubeTexture::F_MAX];
		SKRenderTargetPtr m_pCubRenderTarget[SKCubeTexture::F_MAX];
		
	};

	class SKGRAPHIC_API VS2DCaptureViewFamily : public SKCaptureViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VS2DCaptureViewFamily(const SKString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,SKCamera * pCamera,SKPostEffectSetR* pPostEffectSet,const TCHAR * RenderMethodRTTIName);
		VS2DCaptureViewFamily(const SKString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,SKCamera * pCamera);
		virtual ~VS2DCaptureViewFamily();
		virtual void CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight);
		virtual SKTexture * GetTexture()const;
		virtual void OnDraw(double dAppTime);
	protected:
		SKRenderTargetPtr m_pRenderTarget;

	};

}
#endif