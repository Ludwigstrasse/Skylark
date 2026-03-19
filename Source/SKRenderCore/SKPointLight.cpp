#include "SKPointLight.h"
#include "SKGraphicInclude.h"
#include "SKSceneRender.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPointLight,SKLocalLight)
BEGIN_ADD_PROPERTY(SKPointLight,SKLocalLight)
REGISTER_PROPERTY(m_Range, Range, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, PointLightShadowType, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(PointLightShadowType, ST_CUB)
ADD_ENUM(PointLightShadowType, ST_VOLUME)
ADD_ENUM(PointLightShadowType, ST_DUAL_PARABOLOID)
ADD_ENUM(PointLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPointLight)
IMPLEMENT_INITIAL_END
SKPointLight::SKPointLight()

{

	m_Range = 1000.0f;
	m_uiRTWidth = 1024;
	SetShadowType(ST_CUB);
	m_ZBias = 0.001f;
	m_LightFunScale = SKVector2(0.001f,0.001f);
	m_LightFunOffset = SKVector2(0.0f,0.0f);
	m_ProjectShadowColor = SKColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
}
SKPointLight::~SKPointLight()
{

}
void SKPointLight::GetLightRange()
{	
	
	SKVector3 Point3 = GetWorldTranslate();
	m_WorldRenderBV = SKSphere3(Point3, m_Range);
	
}
bool SKPointLight::IsRelative(SKGeometry * pGeometry)
{
	if (!SKLocalLight::IsRelative(pGeometry))
	{
		return false;
	}
	SKAABB3 GeometryAABB = pGeometry->GetWorldAABB();
	if (GeometryAABB.RelationWith(m_WorldRenderBV) == SKNOINTERSECT)
	{
		return false;
	}
	return true;
}
void SKPointLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
	{
		m_pCubRenderTarget[i] = NULL;
	}

	m_pVolumeShadowFirstPassRenderTarget = NULL;
	m_pPEVolumeSMSceneRender = NULL;
	m_pVolumeShadowRenderTarget = NULL;

	m_pShadowMapSceneRender = NULL;

	m_pProjectShadowSceneRender = NULL;
}
void SKPointLight::SetShadowType(unsigned int uiShadowType)
{

	ResetShadow();
	if (uiShadowType == ST_VOLUME)
	{
		
		m_pShadowTexture.AddElement(SK_NEW SKTexAllState());

		m_pShadowTexture[0]->SetSamplerState((SKSamplerState*)SKSamplerState::GetDoubleLine());

		m_pPEVolumeSMSceneRender = SK_NEW SKPEVolumeShadowMapSceneRender();
		m_pPEVolumeSMSceneRender->SetParam(SKRenderer::CF_COLOR,SKColorRGBA(1.0f,1.0f,1.0f,1.0f),1.0f,0);
		
		m_pVolumeShadowSceneRender = SK_NEW SKVolumeShadowSceneRender();
		m_pVolumeShadowSceneRender->m_pLocalLight = this;
		m_pVolumeShadowSceneRender->SetParam(SKRenderer::CF_STENCIL, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);
		
	}
	else if (uiShadowType == ST_CUB)
	{
		

		m_pShadowTexture.AddElement(SK_NEW SKTexAllState());

		m_pShadowTexture[0]->m_pTex = SK_NEW SKCubeTexture(m_uiRTWidth,SKRenderer::SFT_R16F);
		m_pShadowTexture[0]->SetSamplerState((SKSamplerState*)SKSamplerState::GetShadowMapSampler());
		for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
		{
			m_pCubRenderTarget[i] = SKResourceManager::CreateRenderTarget(m_pShadowTexture[0]->m_pTex,0,0,i);
		}
		
		m_pShadowMapSceneRender = SK_NEW SKShadowMapSceneRender(SKShadowMapSceneRender::SMT_CUB);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(SKRenderer::CF_USE_MAX, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	}
	else if (uiShadowType == ST_DUAL_PARABOLOID)
	{
	
		m_pShadowTexture.AddElement(SK_NEW SKTexAllState());
		m_pShadowTexture.AddElement(SK_NEW SKTexAllState());
		m_pShadowTexture[0]->SetSamplerState((SKSamplerState*)SKSamplerState::GetShadowMapSampler());
		m_pShadowTexture[1]->SetSamplerState((SKSamplerState*)SKSamplerState::GetShadowMapSampler());
		m_pShadowMapSceneRender = SK_NEW SKShadowMapSceneRender(SKShadowMapSceneRender::SMT_DUAL_PARABOLOID);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(SKRenderer::CF_USE_MAX, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	}
	else if (uiShadowType == ST_PROJECT)
	{
		m_pShadowTexture.AddElement(SK_NEW SKTexAllState());
		m_pShadowTexture[0]->SetSamplerState((SKSamplerState*)SKSamplerState::GetShadowMapSampler());
		m_pShadowMapSceneRender = SK_NEW SKShadowMapSceneRender(SKShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(SKRenderer::CF_USE_MAX, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
		m_pProjectShadowSceneRender = SK_NEW SKProjectShadowSceneRender();
		m_pProjectShadowSceneRender->m_pLocalLight = this;
		m_pProjectShadowSceneRender->SetParam(SKRenderer::CF_STENCIL, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);
	}
	m_uiShadowType = uiShadowType;

}
bool SKPointLight::PostClone(SKObject * pObjectSrc)
{
	SKLight::PostClone(pObjectSrc);

	SetShadowType(m_uiShadowType);
	return true;
}
bool SKPointLight::PostLoad(void * pDate)
{
	SKLight::PostLoad(pDate);

	SetShadowType(m_uiShadowType);
	return true;
}
void SKPointLight::DrawNoDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_bShadowMapDrawEnd == false)
		{
			m_bShadowMapDrawEnd = true;
		}
		else
		{
			return;
		}
		if (m_uiShadowType == ST_CUB)
		{
			if (CurCuller.GetCamera()->GetFov() > AngleToRadian(90.0f))
			{
				DrawNormalCubShadow(CurCuller, dAppTime);
			}

		}
		else if (m_uiShadowType == ST_DUAL_PARABOLOID)
		{
			DrawDualParaboloidShadow(CurCuller, dAppTime);
		}
	}
}
void SKPointLight::DisableNoDepandenceShadowMap(double dAppTime)
{
	SKLocalLight::DisableNoDepandenceShadowMap(dAppTime);
	if (m_uiShadowType == ST_DUAL_PARABOLOID)
	{
		if (m_pDualParaboloidRenderTarget0)
		{
			SKResourceManager::Disable2DRenderTarget(m_pDualParaboloidRenderTarget0);
		}

		if (m_pDualParaboloidRenderTarget1)
		{
			SKResourceManager::Disable2DRenderTarget(m_pDualParaboloidRenderTarget1);
		}
		for (unsigned int i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->m_pTex = NULL;
		}
	}
}
void SKPointLight::DisableDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{
	if (m_uiShadowType == ST_VOLUME || m_uiShadowType == ST_PROJECT)
	{
		if (m_pVolumeShadowRenderTarget)
		{
			SKResourceManager::Disable2DRenderTarget(m_pVolumeShadowRenderTarget);
		}
		for (unsigned int i = 0 ; i < m_pShadowTexture.GetNum() ; i++)
		{
			m_pShadowTexture[i]->m_pTex = NULL;
		}
	}
}
void SKPointLight::DrawNormalCubShadow(SKCuller & CurCuller,double dAppTime)
{
	SKMatrix3X3 MatTemp[SKCubeTexture::F_MAX] = {SKMatrix3X3::ms_CameraViewRight,SKMatrix3X3::ms_CameraViewLeft,SKMatrix3X3::ms_CameraViewUp,SKMatrix3X3::ms_CameraViewDown,SKMatrix3X3::ms_CameraViewFront,SKMatrix3X3::ms_CameraViewBack};

	
	SKCamera CubCamera[SKCubeTexture::F_MAX];

	SKVector3 WorldPos = GetWorldTranslate();

	SKShadowCuller ShawdowCuller[SKCubeTexture::F_MAX];
	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
		{
			CubCamera[Index].CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			CubCamera[Index].SetLocalRotate(MatTemp[Index]);
			CubCamera[Index].SetPerspectiveFov(AngleToRadian(90.0f), 1.0f, 1.0f, m_Range);
			ShawdowCuller[Index].PushCameraPlane(CubCamera[Index]);
			ShawdowCuller[Index].m_pLocalLight = this;
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				SKScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}
				pScene->ComputeVisibleSet(ShawdowCuller[Index], false, dAppTime);
			}
			ShawdowCuller[Index].Sort();
		}
	}

	SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,SKRenderer::SFT_D24S8,0);
	
	for (unsigned int k = 0 ; k < SKCubeTexture::F_MAX ;k++)
	{
		m_pShadowMapSceneRender->ClearRTAndDepth();
		m_pShadowMapSceneRender->AddRenderTarget(m_pCubRenderTarget[k]);
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,SKCuller::RG_NORMAL);
		m_pShadowMapSceneRender->Draw(ShawdowCuller[k], dAppTime);
	}
	SKResourceManager::DisableDepthStencil(pDepthStencil);
}
void SKPointLight::DrawNormalProjectShadow(SKCuller & CurCuller,double dAppTime)
{
	SKMatrix3X3 MatTemp[SKCubeTexture::F_MAX] = {SKMatrix3X3::ms_CameraViewRight,SKMatrix3X3::ms_CameraViewLeft,SKMatrix3X3::ms_CameraViewUp,SKMatrix3X3::ms_CameraViewDown,SKMatrix3X3::ms_CameraViewFront,SKMatrix3X3::ms_CameraViewBack};

	SKVector3 WorldPos = GetWorldTranslate();


	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
		{
			SKCamera TempCamera;
			TempCamera.CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			TempCamera.SetLocalRotate(MatTemp[Index]);
			TempCamera.SetPerspectiveFov(SKPI2, 1.0f, 1.0f, m_Range);
			if (!CurCuller.IsVisible(TempCamera.GetFrustumAABB()))
			{
				continue;
			}

			SKShadowCuller TempCuller;
			TempCuller.PushCameraPlane(TempCamera);
			TempCuller.m_pLocalLight = this;

			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				SKScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(TempCuller,false,dAppTime);	

			}
			TempCuller.Sort();



			m_LightShadowMatrix = TempCamera.GetViewMatrix() * TempCamera.GetProjMatrix();

			m_pProjectShadowRenderTarget = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R32F, 0);
			SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,SKRenderer::SFT_D24S8,0);

			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,SKCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pProjectShadowRenderTarget);
			m_pShadowMapSceneRender->Draw(TempCuller,dAppTime);
			m_pShadowTexture[0]->m_pTex = m_pProjectShadowRenderTarget->GetCreateBy();
			SKResourceManager::DisableDepthStencil(pDepthStencil);

			m_pProjectShadowSceneRender->Draw(CurCuller,dAppTime);

			DisableDepandenceShadowMap(TempCuller,dAppTime);

		}
	}
}
void SKPointLight::DrawFovProjectShadow(SKCuller & CurCuller,double dAppTime)
{
	SKMatrix3X3 MatTemp[SKCubeTexture::F_MAX] = {SKMatrix3X3::ms_CameraViewRight,SKMatrix3X3::ms_CameraViewLeft,SKMatrix3X3::ms_CameraViewUp,SKMatrix3X3::ms_CameraViewDown,SKMatrix3X3::ms_CameraViewFront,SKMatrix3X3::ms_CameraViewBack};

	SKMatrix3X3 ViewTran;
	CurCuller.GetCamera()->GetViewMatrix().Get3X3(ViewTran);
	SKMatrix3X3 New;
	New.InverseOf(ViewTran);
	for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
	{
		MatTemp[Index] = MatTemp[Index] * New;
	}
	SKCameraPtr CubCameraPtr[SKCubeTexture::F_MAX];

	SKVector3 WorldPos = GetWorldTranslate();

	SKPlane3 CameraPlane[SKCamera::CP_MAX];
	CurCuller.GetCamera()->GetPlane(CameraPlane);
	unsigned int CullFlag = 0;
	for (unsigned int i = 0 ; i < SKCamera::CP_MAX ; i++)
	{
		int iSide = WorldPos.RelationWith(CameraPlane[i]);
		if (iSide == SKBACK)
		{
			CullFlag = CullFlag | (1 << i);
		}
	}
	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
		{
			
			if (((1 << Index) & CullFlag) == false)
			{
				continue;
			}

			SKCamera TempCamera;
			TempCamera.CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			TempCamera.SetLocalRotate(MatTemp[Index]);
			TempCamera.SetPerspectiveFov(SKPI2, 1.0f, 1.0f, m_Range);
			if (!CurCuller.IsVisible(TempCamera.GetFrustumAABB()))
			{
				continue;
			}
			SKShadowCuller TempCuller;
			TempCuller.PushCameraPlane(TempCamera);
			TempCuller.m_pLocalLight = this;
			
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				SKScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(TempCuller,false,dAppTime);	

			}
			TempCuller.Sort();

			m_LightShadowMatrix = TempCamera.GetViewMatrix() * TempCamera.GetProjMatrix();

			m_pProjectShadowRenderTarget = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R32F, 0);
			SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,SKRenderer::SFT_D24S8,0);
			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,SKCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pProjectShadowRenderTarget);
			m_pShadowMapSceneRender->Draw(TempCuller,dAppTime);
			m_pShadowTexture[0]->m_pTex = m_pProjectShadowRenderTarget->GetCreateBy();
			SKResourceManager::DisableDepthStencil(pDepthStencil);

			m_pProjectShadowSceneRender->Draw(CurCuller,dAppTime);
			DisableDepandenceShadowMap(TempCuller,dAppTime);
		}		
	}

}
void SKPointLight::DrawFovCubShadow(SKCuller & CurCuller,double dAppTime)
{

	SKMatrix3X3 MatTemp[SKCubeTexture::F_MAX] = {SKMatrix3X3::ms_CameraViewRight,SKMatrix3X3::ms_CameraViewLeft,SKMatrix3X3::ms_CameraViewUp,SKMatrix3X3::ms_CameraViewDown,SKMatrix3X3::ms_CameraViewFront,SKMatrix3X3::ms_CameraViewBack};

	SKMatrix3X3 ViewTran;
	CurCuller.GetCamera()->GetViewMatrix().Get3X3(ViewTran);
	SKMatrix3X3 New;
	New.InverseOf(ViewTran);
	for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
	{
		MatTemp[Index] = MatTemp[Index] * New;
	}
	SKCamera CubCamera[SKCubeTexture::F_MAX];

	SKVector3 WorldPos = GetWorldTranslate();

	SKPlane3 CameraPlane[SKCamera::CP_MAX];
	CurCuller.GetCamera()->GetPlane(CameraPlane);
	unsigned int CullFlag = 0;
	for (unsigned int i = 0 ; i < SKCamera::CP_MAX ; i++)
	{
		int iSide = WorldPos.RelationWith(CameraPlane[i]);
		if (iSide == SKBACK)
		{
			CullFlag = CullFlag | (1 << i);
		}
	}
	SKShadowCuller ShawdowCuller[SKCubeTexture::F_MAX];
	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0 ; Index < SKCubeTexture::F_MAX ;Index++)
		{

			if (((1 << Index) & CullFlag) == false)
			{
				continue;
			}
			CubCamera[Index].CreateFromEuler(WorldPos, 0.0f, 0.0f, 0.0f);
			CubCamera[Index].SetLocalRotate(MatTemp[Index]);
			CubCamera[Index].SetPerspectiveFov(AngleToRadian(90.0f), 1.0f, 1.0f, m_Range);
			ShawdowCuller[Index].PushCameraPlane(CubCamera[Index]);
			ShawdowCuller[Index].m_pLocalLight = this;
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				SKScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(ShawdowCuller[Index], false, dAppTime);

			}
			ShawdowCuller[Index].Sort();
		}
	}

	SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,SKRenderer::SFT_D24S8,0);

	for (unsigned int k = 0 ; k < SKCubeTexture::F_MAX ;k++)
	{
		m_pShadowMapSceneRender->ClearRTAndDepth();
		m_pShadowMapSceneRender->AddRenderTarget(m_pCubRenderTarget[k]);
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,SKCuller::RG_NORMAL);
		m_pShadowMapSceneRender->Draw(ShawdowCuller[k], dAppTime);

	}
	SKResourceManager::DisableDepthStencil(pDepthStencil);
}
void SKPointLight::BuildSceneInfo(SKScene * pScene)
{
	SKLocalLight::BuildSceneInfo(pScene);
	if (m_uiShadowType == ST_CUB && m_bIsCastShadow && !m_bIsStatic)
	{
		
	}
}
void SKPointLight::DrawVolumeShadow(SKCuller & CurCuller, double dAppTime)
{
	if (m_uiShadowType != ST_VOLUME)
	{
		return;
	}
	SKVolumeShadowMapCuller SMCuller;
	SMCuller.m_pLocalLight = this;
	if (m_pScene.GetNum() > 0)
	{

		SMCuller.PushCameraPlane(*CurCuller.GetCamera());

		for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
		{
			SKScene * pScene = m_pScene[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(SMCuller, false, dAppTime);

		}
		SMCuller.Sort();
	}


	m_pVolumeShadowSceneRender->Draw(SMCuller, dAppTime);

	m_pVolumeShadowRenderTarget = SKResourceManager::Get2DRenderTarget(SKRenderer::ms_pRenderer->GetCurRTWidth(), SKRenderer::ms_pRenderer->GetCurRTHeight(), SKRenderer::SFT_A8R8G8B8, 0);

	m_pPEVolumeSMSceneRender->ClearRTAndDepth();
	m_pPEVolumeSMSceneRender->AddRenderTarget(m_pVolumeShadowRenderTarget);
	m_pPEVolumeSMSceneRender->Draw(CurCuller, dAppTime);

	m_pShadowTexture[0]->m_pTex = m_pVolumeShadowRenderTarget->GetCreateBy();
}
void SKPointLight::DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_uiShadowType == ST_CUB)
		{
			if (CurCuller.GetCamera()->GetFov() <= AngleToRadian(90.0f))
			{
				DrawFovCubShadow(CurCuller, dAppTime);
			}
		}
	}
}
void SKPointLight::DrawDualParaboloidShadow(SKCuller & CurCuller, double dAppTime)
{
	SKVector3 WorldPos = GetWorldTranslate();
	SKCamera DPCamera[2];
	SKShadowCuller DPCuller[2];
	if (m_pScene.GetNum() > 0)
	{
		for (unsigned int Index = 0; Index < 2; Index++)
		{
			SKREAL ZDirection = (Index == 0) ? 1.0f : -1.0f;
			DPCamera[Index].CreateFromLookDir(WorldPos, SKVector3(0.0f, 0.0f, ZDirection));
			DPCamera[Index].SetOrthogonal(2.0f * m_Range,2.0f * m_Range,0,m_Range);

			DPCuller[Index].m_pLocalLight = this;

			DPCuller[Index].PushCameraPlane(DPCamera[Index]);
			DPCamera[Index].ClearProject();
			for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
			{
				SKScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(DPCuller[Index], false, dAppTime);

			}
			DPCuller[Index].Sort();

		}

	}

	SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_D24S8, 0);
	m_pDualParaboloidRenderTarget0 = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R16F, 0);
	m_pDualParaboloidRenderTarget1 = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R16F, 0);
	for (unsigned int Index = 0; Index < 2; Index++)
	{
		SKRenderTarget * pTarget = Index == 0 ? m_pDualParaboloidRenderTarget0 : m_pDualParaboloidRenderTarget1;

		m_pShadowMapSceneRender->ClearRTAndDepth();
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, SKCuller::RG_NORMAL);
		m_pShadowMapSceneRender->AddRenderTarget(pTarget);

		m_pShadowMapSceneRender->Draw(DPCuller[Index], dAppTime);

		m_pShadowTexture[Index]->m_pTex = pTarget->GetCreateBy();


	}
	SKResourceManager::DisableDepthStencil(pDepthStencil);
}
bool SKPointLight::Cullby(SKCuller & Culler)
{
	unsigned int uiVSF = Culler.IsVisible(m_WorldRenderBV,true);
	if (uiVSF == SKCuller::SKF_ALL || uiVSF == SKCuller::SKF_PARTIAL)
	{
		m_bEnable = true;
		Culler.InsertLight(this);

	}	
	
	return true;
}
void SKPointLight::DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_uiShadowType == ST_PROJECT)
		{
			m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
			m_pProjectShadowSceneRender->m_fLightRange = m_Range;
			if (CurCuller.GetCamera()->GetFov() <= AngleToRadian(90.0f))
			{
	 			DrawFovProjectShadow(CurCuller,dAppTime);
	 		}
	 		else
	 		{
				DrawNormalProjectShadow(CurCuller,dAppTime);
			}
		}
	}
}