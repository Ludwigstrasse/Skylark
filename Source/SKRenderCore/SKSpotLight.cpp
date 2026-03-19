#include "SKSpotLight.h"
#include "SKGraphicInclude.h"
#include "SKSceneRender.h"
#include "SKStream.h"
#include "SKOBB3.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSpotLight,SKLocalLight)
BEGIN_ADD_PROPERTY(SKSpotLight,SKLocalLight)
REGISTER_PROPERTY(m_Range, m_Range, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Falloff, Falloff, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Theta, Theta, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Phi, Phi, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, SpotLightShadowType, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(SpotLightShadowType, ST_NORMAL)
ADD_ENUM(SpotLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSpotLight)
IMPLEMENT_INITIAL_END
SKSpotLight::SKSpotLight()
{
	m_Range = 1500.0f;

	m_Falloff		= 1.0;
	m_Theta			= SKPI2 / 2.0;
	m_Phi			= SKPI2;
	m_uiRTWidth = 1024;
	m_ZBias = 0.005f;
	SetShadowType(ST_NORMAL);
	m_ProjectShadowColor = SKColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
}
SKSpotLight::~SKSpotLight()
{
	m_pShadowTexture.Clear();

	m_pShadowMapSceneRender = NULL;

}
void SKSpotLight::GetLightRange()
{	
	SKVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	SKVector3 Point3 = GetWorldTranslate();
	SKREAL R = TAN(m_Phi * 0.5f) * m_Range;
	SKOBB3 Obb(Dir, Up, Right, m_Range * 0.5f, R, R, Point3 + Dir * m_Range * 0.5f);
	m_WorldRenderBV = Obb.GetAABB();
	if (HaveLightFun())
	{
		SKCamera LightCamera;
		LightCamera.CreateFromLookDir(Point3, Dir);
		LightCamera.UpdateAll(0);

		LightCamera.SetPerspectiveFov(m_Phi, 1.0f, 1.0f, m_Range);
		m_MVP = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();
	}	
}
bool SKSpotLight::PostClone(SKObject * pObjectSrc)
{
	SKLight::PostClone(pObjectSrc);
	SetShadowType(m_uiShadowType);
	return true;
}
bool SKSpotLight::PostLoad(void * pDate)
{
	SKLight::PostLoad(pDate);
	SetShadowType(m_uiShadowType);
	return true;
}
bool SKSpotLight::Cullby(SKCuller & Culler)
{
	unsigned int uiVSF = Culler.IsVisible(m_WorldRenderBV,true);
	if (uiVSF == SKCuller::SKF_ALL || uiVSF == SKCuller::SKF_PARTIAL)
	{
		m_bEnable = true;
		Culler.InsertLight(this);
	}
	return true;
}
bool SKSpotLight::IsRelative(SKGeometry * pGeometry)
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
void SKSpotLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	m_pShadowMapSceneRender = NULL;
	m_pProjectShadowSceneRender = NULL;
}
void SKSpotLight::SetShadowType(unsigned int uiShadowType)
{
	ResetShadow();
	if (uiShadowType == ST_NORMAL || uiShadowType == ST_PROJECT)
	{
		m_pShadowTexture.AddElement(SK_NEW SKTexAllState());
		m_pShadowTexture[0]->SetSamplerState((SKSamplerState*)SKSamplerState::GetShadowMapSampler());
		m_pShadowMapSceneRender = SK_NEW SKShadowMapSceneRender(SKShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(SKRenderer::CF_USE_MAX, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
		if (uiShadowType == ST_PROJECT)
		{
			m_pProjectShadowSceneRender = SK_NEW SKProjectShadowSceneRender();
			m_pProjectShadowSceneRender->m_pLocalLight = this;
			m_pProjectShadowSceneRender->SetParam(SKRenderer::CF_STENCIL, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);
		}
	}
	m_bShadowMapDrawEnd = false;
	m_uiShadowType = uiShadowType;
}
void SKSpotLight::DisableDepandenceShadowMap(SKCuller & CurCuller,double dAppTime)
{
	if (m_uiShadowType == ST_PROJECT)
	{
		if (m_pShadowRenderTarget)
		{
			SKResourceManager::Disable2DRenderTarget(m_pShadowRenderTarget);
		}
		for (unsigned int i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->m_pTex = NULL;
		}
	}
	
}
void SKSpotLight::DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_uiShadowType == ST_PROJECT)
		{
			SKVector3 WorldPos = GetWorldTranslate();

			SKVector3 Dir,Up,Right;
			GetWorldDir(Dir,Up,Right);

			SKShadowCuller TempCuller;


			SKMatrix3X3 Rot = GetWorldRotate();
			SKCamera TempCamera;
			TempCamera.CreateFromLookDir(GetWorldTranslate(), Dir);
			TempCamera.SetPerspectiveFov(m_Phi, 1.0f, 1.0f, m_Range);

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

			m_pShadowRenderTarget = SKResourceManager::Get2DRenderTarget(m_uiRTWidth,m_uiRTWidth,SKRenderer::SFT_R32F,0);
			SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,SKRenderer::SFT_D24S8,0);
			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,SKCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pShadowRenderTarget);

			m_pShadowMapSceneRender->Draw(TempCuller,dAppTime);

			m_pShadowTexture[0]->m_pTex = m_pShadowRenderTarget->GetCreateBy();

			SKResourceManager::DisableDepthStencil(pDepthStencil);

			m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
			m_pProjectShadowSceneRender->m_fLightRange = m_Range;
			m_pProjectShadowSceneRender->m_LightWorldDirection = Dir;
			m_pProjectShadowSceneRender->m_Falloff = m_Falloff;
			m_pProjectShadowSceneRender->m_Theta = m_Theta;
			m_pProjectShadowSceneRender->m_Phi = m_Phi;
			m_pProjectShadowSceneRender->Draw(CurCuller, dAppTime);
			DisableDepandenceShadowMap(TempCuller, dAppTime);
		}
	}
}
void SKSpotLight::DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{
	
}
void SKSpotLight::DrawNormalShadowMap(SKCuller & CurCuller,double dAppTime)
{
	if (CurCuller.HasLight(this) == false)
	{
		return;
	}
	SKVector3 Dir,Up,Right;
	GetWorldDir(Dir,Up,Right);

	SKShadowCuller	TempCuller;

	SKCamera LightCamera;
	LightCamera.CreateFromLookDir(GetWorldTranslate(),Dir);
	LightCamera.SetPerspectiveFov(m_Phi,1.0f,1.0f,m_Range);
	if (m_pScene.GetNum() > 0)
	{
		TempCuller.m_pLocalLight = this;
		TempCuller.PushCameraPlane(LightCamera);
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
	}

	m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();

	m_pShadowRenderTarget = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R32F, 0);
	SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_D24S8, 0);
	m_pShadowMapSceneRender->ClearRTAndDepth();
	m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, SKCuller::RG_NORMAL);
	m_pShadowMapSceneRender->AddRenderTarget(m_pShadowRenderTarget);

	m_pShadowMapSceneRender->Draw(TempCuller, dAppTime);

	m_pShadowTexture[0]->m_pTex = m_pShadowRenderTarget->GetCreateBy();

	SKResourceManager::DisableDepthStencil(pDepthStencil);
}
void SKSpotLight::DrawNoDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
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
		if (m_uiShadowType == ST_NORMAL)
		{
			
			DrawNormalShadowMap(CurCuller, dAppTime);
		}
	}
}
void SKSpotLight::DisableNoDepandenceShadowMap(double dAppTime)
{
	SKLocalLight::DisableNoDepandenceShadowMap(dAppTime);
	if (m_uiShadowType == ST_NORMAL)
	{
		if (m_pShadowRenderTarget)
		{
			SKResourceManager::Disable2DRenderTarget(m_pShadowRenderTarget);
		}
		for (unsigned int i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->m_pTex = NULL;
		}
	}
	
}