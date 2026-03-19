#include "SKViewFamily.h"
#include "SKGraphicInclude.h"
#include "SKCubeTexture.h"
#include "SKCaptureTexAllState.h"
#include "SKResourceManager.h"
#include "SKProfiler.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKSceneRenderMethod, SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKSceneRenderMethod)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKSceneRenderMethod, SKObject)
END_ADD_PROPERTY
SKSceneRenderMethod::SKSceneRenderMethod()
{
	m_pPostEffectSet = NULL;
	m_pPostEffectInstance = NULL;

	m_pDebugDrawSceneRender = SK_NEW SKDebugDrawSceneRender();
	m_pDebugDrawSceneRender->SetParam(SKRenderer::CF_NONE,SKColorRGBA(0.0f,0.0f,0.0f,1.0f),1.0f,0);

	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
	m_pColorRT = NULL;
	ColorRTIsOutSet = false;
}
void SKSceneRenderMethod::SetPostEffect(SKPostEffectSetR* pPostEffectSet)
{
	m_pPostEffectSet = pPostEffectSet;
	m_pPostEffectInstance = NULL;
	if (m_pPostEffectSet)
	{

		m_pPostEffectSet->AddLoadEventObject(this);
	}
}
SKSceneRenderMethod::~SKSceneRenderMethod()
{
	m_pPostEffectInstance = NULL;
	m_pPostEffectSet = NULL;
	m_pDebugDrawSceneRender = NULL;
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
}
SKRenderTarget * SKSceneRenderMethod::GetFinalColorRT()
{
	if (m_pPostEffectInstance)
	{
		return m_pPostEffectInstance->GetEndTarget(0);
	}
	else
	{
		return m_pColorRT;
	}
}
void SKSceneRenderMethod::DisableRT()
{
	if (m_pColorRT && !ColorRTIsOutSet)
	{
		SKResourceManager::Disable2DRenderTarget(m_pColorRT);
	}
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		if(m_pDepthStencil[i])
		{
			SKResourceManager::DisableDepthStencil(m_pDepthStencil[i]);
		}
	}
	if (m_pPostEffectInstance)
	{
		m_pPostEffectInstance->DisableRT();
	}
}
void SKSceneRenderMethod::SetColorRT(SKRenderTarget * pFinalColorRT,unsigned int uiWidth,unsigned int uiHeight)
{
	if (m_pPostEffectInstance)
	{
		if (!m_pPostEffectInstance->SetEndTarget(pFinalColorRT))
		{
			if (m_pColorRT)
			{
				SKResourceManager::Disable2DRenderTarget(m_pColorRT);
			}
			ColorRTIsOutSet = true;
			m_pColorRT = pFinalColorRT;
		}

	}
	else
	{
		if (m_pColorRT)
		{
			SKResourceManager::Disable2DRenderTarget(m_pColorRT);
		}
		ColorRTIsOutSet = true;
		m_pColorRT = pFinalColorRT;

	}
}
void SKSceneRenderMethod::GetRT(unsigned int uiWidth,unsigned int uiHeight)
{

	m_pColorRT = NULL;

	m_pColorRT = SKResourceManager::Get2DRenderTarget(uiWidth,uiHeight,SKRenderer::SFT_A8R8G8B8,0);

	ColorRTIsOutSet = false;
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
		m_pDepthStencil[i] = SKResourceManager::GetDepthStencil(uiWidth,uiHeight,SKRenderer::SFT_D24S8,0);
	}

	if (m_pDebugDrawSceneRender)
	{
		m_pDebugDrawSceneRender->ClearRTAndDepth();
		if (m_bBeforePostDebugDraw)
		{
			m_pDebugDrawSceneRender->AddRenderTarget(m_pColorRT);
		}
		else
		{
			m_pDebugDrawSceneRender->AddRenderTarget(GetFinalColorRT());
		}

		for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
		{
			m_pDebugDrawSceneRender->SetDepthStencil(m_pDepthStencil[i],i);
		}
	}	

	if (m_pPostEffectInstance)
	{
		static SKArray<SKRenderTarget *> Temp;
		Temp.Clear();
		Temp.AddElement(m_pColorRT);
		m_pPostEffectInstance->SetBeginTargetArray(&Temp);
		m_pPostEffectInstance->GetRT(uiWidth,uiHeight);
	}
}
void SKSceneRenderMethod::Draw(SKCuller & Culler,double dAppTime)
{

	if (m_bDrawDebugInfo)
	{
		m_pDebugDrawSceneRender->ClearRTAndDepth();
		if (m_bBeforePostDebugDraw)
		{
			m_pDebugDrawSceneRender->AddRenderTarget(m_pColorRT);
		}
		else
		{
			m_pDebugDrawSceneRender->AddRenderTarget(GetFinalColorRT());
		}

		for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
		{
			m_pDebugDrawSceneRender->SetDepthStencil(m_pDepthStencil[i],i);
		}
	}

	if (m_bBeforePostDebugDraw && m_bDrawDebugInfo)
	{
		m_pDebugDrawSceneRender->Draw(Culler,dAppTime);
	}

	if (m_pPostEffectInstance)
	{	
		m_pPostEffectInstance->Draw(Culler,dAppTime);

	}
	if (!m_bBeforePostDebugDraw && m_bDrawDebugInfo)
	{
		m_pDebugDrawSceneRender->Draw(Culler,dAppTime);
	}
}
void SKSceneRenderMethod::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	SKPostEffectSet *	pPostEffectInstance = m_pPostEffectSet->GetResource();
	m_pPostEffectInstance = (SKPostEffectSet *)SKObject::CloneCreateObject(pPostEffectInstance);
}

SKDebugDraw * SKSceneRenderMethod::GetDebugDraw(unsigned int uiRenderGroup)
{
	if (m_pDebugDrawSceneRender)
	{
		return m_pDebugDrawSceneRender->GetDebugDraw(uiRenderGroup);
	}
	return NULL;
}
IMPLEMENT_RTTI_NoCreateFun(SKViewFamily,SKObject)
SKViewFamily::SKViewFamily(const SKString &ViewFamilyName,SKCamera * pCamera)
{
	m_pCamera = pCamera;
	m_pCamera->AddViewFamily(this);

	m_uiWidth = 0;
	m_uiHeight = 0;




	m_ViewFamilyName = ViewFamilyName;
	m_bEnable = true;
}
SKViewFamily::SKViewFamily(const SKString &ViewFamilyName, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
{
	m_pCamera = pCamera;
	m_pCamera->AddViewFamily(this);

	m_uiWidth = 0;
	m_uiHeight = 0;

	


	m_ViewFamilyName = ViewFamilyName;
	m_bEnable = true;

	SKString RMName = RenderMethodRTTIName;
	m_pSceneRenderMethod = DynamicCast<SKSceneRenderMethod>(SKObject::GetNoGCInstance(RMName));
	if (m_pSceneRenderMethod)
	{
		m_pSceneRenderMethod->SetPostEffect(pPostEffectSet);
	}
	
}

SKViewFamily::~SKViewFamily()
{
	m_pCamera = NULL;

	m_pScene.Clear();

}
bool SKViewFamily::IsReCreate(unsigned int uiWidth,unsigned int uiHeight)
{
	if (m_uiWidth == uiWidth && m_uiHeight == uiHeight)
	{
		return false;
	}
	return true;
}



void SKViewFamily::Update(double dAppTime)
{
	
	SKArray<SKScene *> Temp ;
	if (!m_pCamera)
	{
		return ;
	}
	if (!m_pCamera->m_bEnable)
	{
		return ;
	}
	Temp.Clear();	
	Temp.AddElement(m_pScene,0,m_pScene.GetNum() - 1);
	if (Temp.GetNum() > 0)
	{
		m_Culler.ClearPlaneState();
		m_Culler.ClearAllPlane();
		m_Culler.ClearVisibleSet();
		m_Culler.ClearLight();
		m_Culler.PushCameraPlane(*m_pCamera);
		for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
		{
			SKScene * pScene = Temp[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(m_Culler,false,dAppTime);	
	
		}

		if(m_Culler.GetAllVisibleNum() == 0)
			return ;
		m_Culler.Sort();
		for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
		{
			SKScene * pScene = Temp[i];
			if (!pScene)
			{
				continue;
			}
			for (unsigned int i = 0 ; i < pScene->GetAllLightNum() ; i++)
			{
				SKLight * pLight = pScene->GetAllLight(i);
				if (pLight)
				{
					pLight->Cullby(m_Culler);
				}
			}
		}
		m_Culler.GetAndSortLight();
	}
}
void SKViewFamily::OnDraw(double dAppTime)
{
	if (m_pSceneRenderMethod)
	{
		m_pSceneRenderMethod->Draw(m_Culler,dAppTime);
	}
}
void SKViewFamily::Draw(double dAppTime)
{
	if (m_pCamera)
	{
		if (!m_pCamera->m_bEnable)
		{
			return ;
		}

		if (m_pSceneRenderMethod)
		{
			m_pSceneRenderMethod->GetRT(m_uiWidth,m_uiHeight);
		}
		
		OnDraw(dAppTime);

		if (m_pSceneRenderMethod)
		{
			m_pSceneRenderMethod->DisableRT();
		}
		
	}
}
void SKViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	m_uiHeight = uiHeight;
	m_uiWidth = uiWidth;
	
}
SKRenderTarget * SKViewFamily::GetFinalColorRT()
{	

	if (m_pSceneRenderMethod)
	{
		return m_pSceneRenderMethod->GetFinalColorRT();
	}
	return NULL;

}
IMPLEMENT_RTTI_NoCreateFun(SKWindowViewFamily,SKViewFamily);
SKWindowViewFamily::SKWindowViewFamily(const SKString &ViewFamilyName, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName, int iWindowID)
					:SKViewFamily(ViewFamilyName,pCamera,pPostEffectSet,RenderMethodRTTIName)
{

	
	m_iWindowID = iWindowID;
	m_pScreenQuadRenderer = SK_NEW SKPEScreenQuadSceneRender();

}
SKWindowViewFamily::SKWindowViewFamily(const SKString &ViewFamilyName,SKCamera * pCamera,int iWindowID)
:SKViewFamily(ViewFamilyName,pCamera)
{
	m_iWindowID = iWindowID;
	m_pScreenQuadRenderer = SK_NEW SKPEScreenQuadSceneRender();
}
SKWindowViewFamily::~SKWindowViewFamily()
{
	m_pScreenQuadRenderer = NULL;
	
}
void SKWindowViewFamily::OnDraw(double dAppTime)
{
	SKViewFamily::OnDraw(dAppTime);
	SKRenderTarget *pRenderTarget = GetFinalColorRT();
	SKMAC_ASSERT(pRenderTarget);

	m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
	m_pScreenQuadRenderer->Draw(m_Culler, dAppTime);


}
void SKWindowViewFamily::Draw(double dAppTime)
{
	unsigned int uiHeight = SKRenderer::ms_pRenderer->GetScreenHeight();
	unsigned int uiWidth = SKRenderer::ms_pRenderer->GetScreenWith();
	SKRenderer::ms_pRenderer->UseWindow(m_iWindowID);
	if (SKRenderer::ms_pRenderer->IsWindowed())
	{

		SKRenderer::ChildWindowInfo * pChildInfo = SKRenderer::ms_pRenderer->GetChildWindowInfo(m_iWindowID);
		if (pChildInfo)
		{
			uiHeight = pChildInfo->m_uiHeight;
			uiWidth = pChildInfo->m_uiWidth;
		}

	}

	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}

	SKViewFamily::Draw(dAppTime);
	
}
void SKWindowViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	SKViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);

	m_pScreenQuadRenderer->SetNoUseRTRenderSize(uiWidth,uiHeight);

}
IMPLEMENT_RTTI_NoCreateFun(SKCaptureViewFamily,SKViewFamily)
SKCaptureViewFamily::SKCaptureViewFamily(const SKString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,SKCamera * pCamera)
:SKViewFamily(ViewFamilyName,pCamera)
{
	SKMAC_ASSERT(uiWidth && uiHeight);
	m_pScreenQuadRenderer = SK_NEW SKPEScreenQuadSceneRender();
	
	m_OnlyUpdateOneTime = false;
	m_pTexOwner = NULL;
}
SKCaptureViewFamily::SKCaptureViewFamily(const SKString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
:SKViewFamily(ViewFamilyName,pCamera,pPostEffectSet,RenderMethodRTTIName)
{
	SKMAC_ASSERT(uiWidth && uiHeight);
	m_pScreenQuadRenderer = SK_NEW SKPEScreenQuadSceneRender();

	m_OnlyUpdateOneTime = false;
	m_pTexOwner = NULL;
}
void SKCaptureViewFamily::Draw(double dAppTime)
{
	
	SKViewFamily::Draw(dAppTime);
	if (m_pTexOwner)
	{
		m_pTexOwner->NotifyEndDraw();
		m_pTexOwner = NULL;
	}
	if (m_OnlyUpdateOneTime)
	{
		m_bEnable = false;
	}
}
void SKCaptureViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	SKViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);
	m_pScreenQuadRenderer->SetNoUseRTRenderSize(uiWidth,uiHeight);

}
SKCaptureViewFamily::~SKCaptureViewFamily()
{
	m_pScreenQuadRenderer = NULL;
}
void SKCaptureViewFamily::OnDraw(double dAppTime)
{
	SKViewFamily::OnDraw(dAppTime);
	SKRenderTarget *pRenderTarget = GetFinalColorRT();
	SKMAC_ASSERT(pRenderTarget);

	m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
	m_pScreenQuadRenderer->Draw(m_Culler,dAppTime);
}
void SKCaptureViewFamily::SetSize(unsigned int uiWidth,unsigned int uiHeight)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
IMPLEMENT_RTTI_NoCreateFun(VS2DCaptureViewFamily,SKCaptureViewFamily)
VS2DCaptureViewFamily::VS2DCaptureViewFamily(const SKString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,SKCamera * pCamera)
:SKCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
VS2DCaptureViewFamily::VS2DCaptureViewFamily(const SKString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
:SKCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
VS2DCaptureViewFamily::~VS2DCaptureViewFamily()
{
	m_pRenderTarget = NULL;
}
void VS2DCaptureViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	SKCaptureViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);
	VS2DTexture * pTexture = SK_NEW VS2DTexture(m_uiWidth,m_uiHeight,SKRenderer::SFT_A8R8G8B8);

	m_pRenderTarget = SKResourceManager::CreateRenderTarget(pTexture,SKRenderer::ms_pRenderer->GetCurMultisample());

}
SKTexture * VS2DCaptureViewFamily::GetTexture()const
{
	return m_pRenderTarget->GetCreateBy();
}
void VS2DCaptureViewFamily::OnDraw(double dAppTime)
{

	SKViewFamily::OnDraw(dAppTime);
	SKRenderTarget *pRenderTarget = GetFinalColorRT();
	SKMAC_ASSERT(pRenderTarget);

 	m_pScreenQuadRenderer->ClearRTAndDepth();
 	m_pScreenQuadRenderer->AddRenderTarget(m_pRenderTarget);
	m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
	m_pScreenQuadRenderer->Draw(m_Culler,dAppTime);

}
IMPLEMENT_RTTI_NoCreateFun(SKCubCaptureViewFamily,SKCaptureViewFamily)
SKCubCaptureViewFamily::SKCubCaptureViewFamily(const SKString &ViewFamilyName,unsigned int uiWidth,unsigned int uiHeight,SKCamera * pCamera)
:SKCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
SKCubCaptureViewFamily::SKCubCaptureViewFamily(const SKString &ViewFamilyName, unsigned int uiWidth, unsigned int uiHeight, SKCamera * pCamera, SKPostEffectSetR* pPostEffectSet, const TCHAR * RenderMethodRTTIName)
:SKCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName)
{
	if (IsReCreate(uiWidth,uiHeight))
	{
		CreateRenderTargetBuffer(uiWidth,uiHeight);
	}
}
SKCubCaptureViewFamily::~SKCubCaptureViewFamily()
{
	for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ;i++)
	{
		m_pCubRenderTarget[i] = NULL;
	}
}

SKTexture * SKCubCaptureViewFamily::GetTexture()const
{
	return m_pCubRenderTarget[0]->GetCreateBy();
}
void SKCubCaptureViewFamily::OnDraw(double dAppTime)
{

	for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
	{
		m_pSceneRenderMethod->Draw(m_CubCuller[Index],dAppTime);

		SKRenderTarget *pRenderTarget = GetFinalColorRT();
		SKMAC_ASSERT(pRenderTarget);
		m_pScreenQuadRenderer->ClearRTAndDepth();
		m_pScreenQuadRenderer->AddRenderTarget(m_pCubRenderTarget[Index]);
		m_pScreenQuadRenderer->SetSourceTarget(pRenderTarget->GetCreateBy());
		m_pScreenQuadRenderer->Draw(m_Culler,dAppTime);

	}
		
}
void SKCubCaptureViewFamily::CreateRenderTargetBuffer(unsigned int uiWidth,unsigned int uiHeight)
{
	
	SKCaptureViewFamily::CreateRenderTargetBuffer(uiWidth,uiHeight);
	SKCubeTexture * pTexture = SK_NEW SKCubeTexture(m_uiWidth,SKRenderer::SFT_A8R8G8B8);
	for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ;i++)
	{
		SKRenderTarget * pRenderTarget = SKResourceManager::CreateRenderTarget(pTexture,SKRenderer::ms_pRenderer->GetCurMultisample(),0,i);
		m_pCubRenderTarget[i] = pRenderTarget;
	}

}
void SKCubCaptureViewFamily::Update(double dAppTime)
{
	SKArray<SKScene *> Temp ;
	if (!m_pCamera)
	{
		return ;
	}
	if (!m_pCamera->m_bEnable)
	{
		return ;
	}
	Temp.Clear();	
	Temp.AddElement(m_pScene,0,m_pScene.GetNum() - 1);
	SKMatrix3X3 MatTemp[SKCubeTexture::F_MAX] = {SKMatrix3X3::ms_CameraViewRight,SKMatrix3X3::ms_CameraViewLeft,SKMatrix3X3::ms_CameraViewUp,SKMatrix3X3::ms_CameraViewDown,SKMatrix3X3::ms_CameraViewFront,SKMatrix3X3::ms_CameraViewBack};
	
	if (Temp.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
		{
			m_CubCuller[Index].ClearPlaneState();
			m_CubCuller[Index].ClearAllPlane();
			m_CubCuller[Index].ClearVisibleSet();
			m_CubCuller[Index].ClearLight();

			CubCameraPtr[Index] = SK_NEW SKCamera();
			CubCameraPtr[Index]->CreateFromEuler(m_pCamera->GetWorldTranslate(),0.0f,0.0f,0.0f);
			CubCameraPtr[Index]->SetLocalRotate(MatTemp[Index]);
			CubCameraPtr[Index]->SetPerspectiveFov(AngleToRadian(90.0f),1.0f,1.0f,m_pCamera->GetZFar());
			CubCameraPtr[Index]->UpdateAll(0);

			m_CubCuller[Index].PushCameraPlane(*CubCameraPtr[Index]);
			for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
			{
				SKScene * pScene = Temp[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(m_CubCuller[Index],false,dAppTime);	

			}

			if(m_CubCuller[Index].GetAllVisibleNum() == 0)
				continue ;
			m_CubCuller[Index].Sort();
			for (unsigned int i = 0 ; i < Temp.GetNum() ;i++)
			{
				SKScene * pScene = Temp[i];
				if (!pScene)
				{
					continue;
				}
				for (unsigned int i = 0 ; i < pScene->GetAllLightNum() ; i++)
				{
					SKLight * pLight = pScene->GetAllLight(i);
					if (pLight)
					{
						pLight->Cullby(m_CubCuller[Index]);
					}
				}
			}
		}
		
	}
}

IMPLEMENT_RTTI(SKForwordHighEffectSceneRenderMethod, SKSceneRenderMethod)
BEGIN_ADD_PROPERTY(SKForwordHighEffectSceneRenderMethod, SKSceneRenderMethod)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKForwordHighEffectSceneRenderMethod)
IMPLEMENT_INITIAL_END
SKForwordHighEffectSceneRenderMethod::~SKForwordHighEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = NULL;
	m_pNormalDepthSceneRender = NULL;
	m_pGammaCorrectSceneRender = NULL;
	m_pSSRSceneRender = NULL;

	m_pMaterialRT = NULL;
	m_pNormalDepthRT = NULL;
}
SKForwordHighEffectSceneRenderMethod::SKForwordHighEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = SK_NEW SKMaterialSceneRender();
	m_pMaterialSceneRenderder->SetParam(SKRenderer::CF_COLOR,SKColorRGBA(0.0f,0.0f,0.0f,1.0f),1.0f,0);
	m_pNormalDepthSceneRender = SK_NEW SKNormalDepthSceneRender();
	m_pNormalDepthSceneRender->SetParam(SKRenderer::CF_USE_MAX,SKColorRGBA(0.0f,0.0f,1.0f,0.0f),1.0f,0);
	m_pGammaCorrectSceneRender = SK_NEW SKGammaCorrectSceneRender();
	m_pSSRSceneRender = SK_NEW SKPESSRSceneRender();

	m_pMaterialRT = NULL;
	m_pNormalDepthRT = NULL;

}
void SKForwordHighEffectSceneRenderMethod::Draw(SKCuller & Culler, double dAppTime)
{
	m_pNormalDepthSceneRender->Draw(Culler,dAppTime);
	m_pMaterialSceneRenderder->Draw(Culler,dAppTime);

	//m_pSSRSceneRender->SetUseTexture(m_pNormalDepthRT->GetCreateBy(),m_pMaterialRT->GetCreateBy());
	//m_pSSRSceneRender->Draw(Culler,dAppTime);

	m_pGammaCorrectSceneRender->SetSourceTarget(m_pMaterialRT->GetCreateBy());
	m_pGammaCorrectSceneRender->Draw(Culler,dAppTime);
	SKSceneRenderMethod::Draw(Culler,dAppTime);
}
void SKForwordHighEffectSceneRenderMethod::DisableRT()
{
	SKSceneRenderMethod::DisableRT();
	if (m_pMaterialRT)
	{
		SKResourceManager::Disable2DRenderTarget(m_pMaterialRT);
	}
	
	if (m_pNormalDepthRT)
	{
		SKResourceManager::Disable2DRenderTarget(m_pNormalDepthRT);
	}
	
	if (m_pSSRRT)
	{
		SKResourceManager::Disable2DRenderTarget(m_pSSRRT);
	}
}
void SKForwordHighEffectSceneRenderMethod::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	SKSceneRenderMethod::GetRT(uiWidth,uiHeight);

	m_pMaterialRT = NULL;
	m_pMaterialRT = SKResourceManager::Get2DRenderTarget(uiWidth,uiHeight,SKRenderer::SFT_A16B16G16R16F,0);

	m_pNormalDepthRT = NULL;
	m_pNormalDepthRT = SKResourceManager::Get2DRenderTarget(uiWidth,uiHeight,SKRenderer::SFT_A16B16G16R16F,0);

	m_pSSRRT = NULL;
	//SFT_ARGB8 ?
	m_pSSRRT = SKResourceManager::Get2DRenderTarget(uiWidth,uiHeight,SKRenderer::SFT_A16B16G16R16F,0);


	m_pNormalDepthSceneRender->ClearRTAndDepth();
	m_pNormalDepthSceneRender->AddRenderTarget(m_pNormalDepthRT);
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pNormalDepthSceneRender->SetDepthStencil(m_pDepthStencil[i],i);
	}

	m_pMaterialSceneRenderder->ClearRTAndDepth();
	m_pMaterialSceneRenderder->AddRenderTarget(m_pMaterialRT);
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pMaterialSceneRenderder->SetDepthStencil(m_pDepthStencil[i],i);
	}
	m_pMaterialSceneRenderder->SetNormalDepthTexture((VS2DTexture *)m_pNormalDepthRT->GetCreateBy());

	m_pSSRSceneRender->ClearRTAndDepth();
	m_pSSRSceneRender->AddRenderTarget(m_pSSRRT);

	m_pGammaCorrectSceneRender->ClearRTAndDepth();
	m_pGammaCorrectSceneRender->AddRenderTarget(m_pColorRT);

}
void SKForwordHighEffectSceneRenderMethod::SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_pMaterialSceneRenderder->SetUseState(RenderState,uiRenderStateInheritFlag);
}
void SKForwordHighEffectSceneRenderMethod::ClearUseState()
{
	m_pMaterialSceneRenderder->ClearUseState();
}


IMPLEMENT_RTTI(SKForwordEffectSceneRenderMethod, SKSceneRenderMethod)
BEGIN_ADD_PROPERTY(SKForwordEffectSceneRenderMethod, SKSceneRenderMethod)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKForwordEffectSceneRenderMethod)
IMPLEMENT_INITIAL_END
SKForwordEffectSceneRenderMethod::~SKForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = NULL;

	m_pGammaCorrectSceneRender = NULL;

	m_pMaterialRT = NULL;
}
SKForwordEffectSceneRenderMethod::SKForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = SK_NEW SKMaterialSceneRender();
	m_pMaterialSceneRenderder->SetParam(SKRenderer::CF_USE_MAX, SKColorRGBA(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	m_pGammaCorrectSceneRender = SK_NEW SKGammaCorrectSceneRender();


	m_pMaterialRT = NULL;


}
void SKForwordEffectSceneRenderMethod::Draw(SKCuller & Culler, double dAppTime)
{

	m_pMaterialSceneRenderder->Draw(Culler, dAppTime);
	m_pGammaCorrectSceneRender->SetSourceTarget(m_pMaterialRT->GetCreateBy());
	m_pGammaCorrectSceneRender->Draw(Culler, dAppTime);	
	SKSceneRenderMethod::Draw(Culler, dAppTime);
}
void SKForwordEffectSceneRenderMethod::DisableRT()
{
	SKSceneRenderMethod::DisableRT();
	if (m_pMaterialRT)
	{
		SKResourceManager::Disable2DRenderTarget(m_pMaterialRT);
	}
}
void SKForwordEffectSceneRenderMethod::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	SKSceneRenderMethod::GetRT(uiWidth, uiHeight);

	m_pMaterialRT = NULL;
	m_pMaterialRT = SKResourceManager::Get2DRenderTarget(uiWidth, uiHeight, SKRenderer::SFT_A16B16G16R16F, 0);


	m_pMaterialSceneRenderder->ClearRTAndDepth();
	m_pMaterialSceneRenderder->AddRenderTarget(m_pMaterialRT);
	for (unsigned int i = 0; i < SKCuller::RG_MAX; i++)
	{
		m_pMaterialSceneRenderder->SetDepthStencil(m_pDepthStencil[i], i);
	}

	m_pGammaCorrectSceneRender->ClearRTAndDepth();
	m_pGammaCorrectSceneRender->AddRenderTarget(m_pColorRT);

}
void SKForwordEffectSceneRenderMethod::SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_pMaterialSceneRenderder->SetUseState(RenderState, uiRenderStateInheritFlag);
}
void SKForwordEffectSceneRenderMethod::ClearUseState()
{
	m_pMaterialSceneRenderder->ClearUseState();
}

IMPLEMENT_RTTI(SKSimpleForwordEffectSceneRenderMethod, SKSceneRenderMethod)
BEGIN_ADD_PROPERTY(SKSimpleForwordEffectSceneRenderMethod, SKSceneRenderMethod)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSimpleForwordEffectSceneRenderMethod)
IMPLEMENT_INITIAL_END
SKSimpleForwordEffectSceneRenderMethod::~SKSimpleForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = NULL;
}
SKSimpleForwordEffectSceneRenderMethod::SKSimpleForwordEffectSceneRenderMethod()
{
	m_pMaterialSceneRenderder = SK_NEW SKMaterialSceneRender();
	m_pMaterialSceneRenderder->SetParam(SKRenderer::CF_USE_MAX, SKColorRGBA(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);



}
void SKSimpleForwordEffectSceneRenderMethod::Draw(SKCuller & Culler, double dAppTime)
{


	m_pMaterialSceneRenderder->Draw(Culler, dAppTime);

	SKSceneRenderMethod::Draw(Culler, dAppTime);
}
void SKSimpleForwordEffectSceneRenderMethod::DisableRT()
{
	SKSceneRenderMethod::DisableRT();


}
void SKSimpleForwordEffectSceneRenderMethod::GetRT(unsigned int uiWidth, unsigned int uiHeight)
{
	SKSceneRenderMethod::GetRT(uiWidth, uiHeight);


	m_pMaterialSceneRenderder->ClearRTAndDepth();
	m_pMaterialSceneRenderder->AddRenderTarget(m_pColorRT);
	for (unsigned int i = 0; i < SKCuller::RG_MAX; i++)
	{
		m_pMaterialSceneRenderder->SetDepthStencil(m_pDepthStencil[i], i);
	}



}
void SKSimpleForwordEffectSceneRenderMethod::SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_pMaterialSceneRenderder->SetUseState(RenderState, uiRenderStateInheritFlag);
}
void SKSimpleForwordEffectSceneRenderMethod::ClearUseState()
{
	m_pMaterialSceneRenderder->ClearUseState();
}