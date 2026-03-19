#include "SKDirectionLight.h"
#include "SKGraphicInclude.h"
#include "SKSceneRender.h"
#include "SKStream.h"
#include "SKRay3.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDirectionLight,SKLocalLight)
BEGIN_ADD_PROPERTY(SKDirectionLight,SKLocalLight)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, DirectionLightShadowType,SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunWidth, LightFunWidth, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunHeight, LightFunHeight, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunNear, LightFunNear, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunFar, LightFunFar, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, SKProperty::F_SAVE_LOAD_CLONE)
BEGIN_ADD_ENUM
ADD_ENUM(DirectionLightShadowType, ST_VOLUME)
ADD_ENUM(DirectionLightShadowType, ST_OSM)
ADD_ENUM(DirectionLightShadowType, ST_CSM)
ADD_ENUM(DirectionLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDirectionLight)
IMPLEMENT_INITIAL_END
#define CSM_LEVLE 3
SKDirectionLight::SKDirectionLight()
{
	m_uiRTWidth = 1024;
	SetShadowType(ST_OSM);
	m_ZBias = 0.001f;
	m_fLightFunWidth = 1000.0f;
	m_fLightFunHeight = 1000.0f; 
	m_fLightFunNear = 1.0f; 
	m_fLightFunFar = 8000.0f;
}
SKDirectionLight::~SKDirectionLight()
{

}
void SKDirectionLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	m_pPEVolumeSMSceneRender = NULL;
	m_pShadowMapSceneRender = NULL;
	m_pProjectShadowSceneRender = NULL;
	m_pVolumeShadowSceneRender = NULL;
	m_pCSMRTArray.Clear();
}
void SKDirectionLight::SetShadowType(unsigned int uiShadowType)
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
	else if (uiShadowType == ST_OSM || uiShadowType == ST_PROJECT)
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
	else if (uiShadowType == ST_CSM)
	{
		for (unsigned int i = 0 ; i < CSM_LEVLE ; i++)
		{
			m_pShadowTexture.AddElement(SK_NEW SKTexAllState());
			m_pShadowTexture[i]->SetSamplerState((SKSamplerState*)SKSamplerState::GetShadowMapSampler());
		}
		m_pCSMRTArray.SetBufferNum(CSM_LEVLE);
		m_pShadowMapSceneRender = SK_NEW SKShadowMapSceneRender(SKShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(SKRenderer::CF_USE_MAX, SKColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	}
	m_uiShadowType = uiShadowType;

}
bool SKDirectionLight::PostClone(SKObject * pObjectSrc)
{
	SKLight::PostClone(pObjectSrc);

	SetShadowType(m_uiShadowType);
	return true;
}
bool SKDirectionLight::PostLoad(void * pDate)
{
	SKLight::PostLoad(pDate);
	SetShadowType(m_uiShadowType);
	return true;
}


void SKDirectionLight::DisableDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{
	if (m_uiShadowType == ST_VOLUME || m_uiShadowType == ST_OSM || m_uiShadowType == ST_PROJECT)
	{
		if (m_pVolumeShadowRenderTarget)
		{
			SKResourceManager::Disable2DRenderTarget(m_pVolumeShadowRenderTarget);
		}
	}
	else if (m_uiShadowType == ST_CSM)
	{
		for (unsigned int i = 0 ; i < CSM_LEVLE ; i++)
		{
			if (m_pCSMRTArray[i])
			{
				SKResourceManager::Disable2DRenderTarget(m_pCSMRTArray[i]);
			}
		}
	}
	for (unsigned int i = 0 ; i < m_pShadowTexture.GetNum() ; i++)
	{
		m_pShadowTexture[i]->m_pTex = NULL;
	}
}
void SKDirectionLight::DrawVolumeShadow(SKCuller & CurCuller,double dAppTime)
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
	m_pPEVolumeSMSceneRender->Draw(CurCuller,dAppTime);

	m_pShadowTexture[0]->m_pTex = m_pVolumeShadowRenderTarget->GetCreateBy();

}
SKAABB3 SKDirectionLight::GetMaxAABB(SKArray<SKAABB3> &AABBArray)
{
	SKAABB3 Temp;
	for (unsigned int i = 0 ;i < AABBArray.GetNum() ; i++)
	{
		
		if (i == 0)
		{
			Temp = AABBArray[i];
		}
		else
		{
			Temp = Temp.MergAABB(AABBArray[i]);
		}
	}
	return Temp;
}
void SKDirectionLight::DrawOSM(SKCuller & CurCuller,double dAppTime)
{
#define DRAW_DEPTH \
	{\
		m_pOSMShadowRenderTarget = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R16F, 0);\
		SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_D24S8, 0);\
		m_pShadowMapSceneRender->ClearRTAndDepth();\
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, SKCuller::RG_NORMAL);\
		m_pShadowMapSceneRender->AddRenderTarget(m_pOSMShadowRenderTarget);\
		m_pShadowMapSceneRender->Draw(DSMCuller, dAppTime);\
		m_pShadowTexture[0]->m_pTex = m_pOSMShadowRenderTarget->GetCreateBy();\
		SKResourceManager::DisableDepthStencil(pDepthStencil);\
	}
	SKDirShadowMapCuller	DSMCuller;
	DSMCuller.m_pLocalLight = this;
	if (m_pScene.GetNum() > 0)
	{
		DSMCuller.PushCameraPlane(*CurCuller.GetCamera());
		for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
		{
			SKScene * pScene = m_pScene[i];
			if (!pScene)
			{
				continue;
			}

			pScene->ComputeVisibleSet(DSMCuller, false, dAppTime);

		}
		DSMCuller.Sort();
	}

	SKArray<SKAABB3> CasterAABBArray;
	GetCullerAABBArray(DSMCuller, CasterAABBArray);
	if (CasterAABBArray.GetNum() == 0)
	{
		DRAW_DEPTH;
		return;
	}

	SKAABB3 CasterAABB = GetMaxAABB(CasterAABBArray);

	SKVector3 Center = CasterAABB.GetCenter();

	SKVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	SKRay3 Ray(Center,Dir * (-1.0f));

	unsigned int Q;
	SKREAL tN,tF;
	if(CasterAABB.RelationWith(Ray,Q,tN,tF) != SKINTERSECT)
	{
		DRAW_DEPTH;
		return ;
	}

	SKVector3 LigthPT = Center - Dir * tN * 2.0f;

	SKCamera LightCamera;
	LightCamera.CreateFromLookAt(LigthPT,Center);

	SKMatrix3X3W LightView = LightCamera.GetViewMatrix();

	SKAABB3 NewCasterAABB;
	NewCasterAABB.Transform(CasterAABB,LightView);

	SKREAL NewNear = NewCasterAABB.GetMinPoint().z;
	SKREAL NewFar = NewCasterAABB.GetMaxPoint().z;
	LightCamera.SetOrthogonal(NewCasterAABB.GetMaxPoint().x - NewCasterAABB.GetMinPoint().x,
		NewCasterAABB.GetMaxPoint().y - NewCasterAABB.GetMinPoint().y,
		NewNear,NewFar);

	m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();


	DRAW_DEPTH;
}
void SKDirectionLight::DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{	
		if (m_uiShadowType == ST_OSM)
		{
			DrawOSM(CurCuller,dAppTime);	
		}
		else if (m_uiShadowType == ST_CSM)
		{
			DrawCSM(CurCuller,dAppTime);
		}

	}
}
void SKDirectionLight::DrawCSM(SKCuller & CurCuller, double dAppTime)
{
	if (HaveLightFun())
	{
		return;
	}
#define DRAW_CSM_DEPTH \
	{\
		m_pCSMRTArray[i] = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R16F, 0);\
		SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_D24S8, 0);\
		m_pShadowMapSceneRender->ClearRTAndDepth();\
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, SKCuller::RG_NORMAL);\
		m_pShadowMapSceneRender->AddRenderTarget(m_pCSMRTArray[i]);\
		m_pShadowMapSceneRender->Draw(Culler, dAppTime);\
		m_pShadowTexture[i]->m_pTex = m_pCSMRTArray[i]->GetCreateBy();\
		SKResourceManager::DisableDepthStencil(pDepthStencil);\
	}

	SKCamera * pCamera = CurCuller.GetCamera();
	SKREAL fNear = pCamera->GetZNear();
	SKREAL fFar = pCamera->GetZFar();

	SKVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	SKREAL Range[CSM_LEVLE + 1] = {fNear , 1000.0f , 3500.0f , fFar};
	for (unsigned int i = 0; i < CSM_LEVLE; i++)
	{

		SKDirShadowMapCuller Culler;
		Culler.m_pLocalLight = this;

		if (m_pScene.GetNum() > 0)
		{
			pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), Range[i], Range[i + 1]);
			Culler.PushCameraPlane(*pCamera);
			for (unsigned int i = 0; i < m_pScene.GetNum(); i++)
			{
				SKScene * pScene = m_pScene[i];
				if (!pScene)
				{
					continue;
				}

				pScene->ComputeVisibleSet(Culler, false, dAppTime);

			}
			Culler.Sort();
		}

		SKArray<SKAABB3> CasterAABBArray;
		GetCullerAABBArray(Culler, CasterAABBArray);
		if (CasterAABBArray.GetNum() == 0)
		{
			DRAW_CSM_DEPTH;
			continue;
		}
		SKAABB3 CasterAABB = GetMaxAABB(CasterAABBArray);
		SKAABB3 ReceiverAABB = Culler.GetCamera()->GetFrustumAABB();

		SKMatrix3X3W LightRot;
		LightRot.CreateFromLookDir(SKVector3::ms_Zero,Dir);

		SKAABB3 NewCasterAABB, NewReceiverAABB;
		NewCasterAABB.Transform(CasterAABB, LightRot);
		NewReceiverAABB.Transform(ReceiverAABB, LightRot);

		SKAABB3 MinAABB = NewReceiverAABB.GetMin(NewCasterAABB);
		SKVector3 MinP(MinAABB.GetMinPoint().x, MinAABB.GetMinPoint().y, NewCasterAABB.GetMinPoint().z);
		SKVector3 MaxP(MinAABB.GetMaxPoint().x, MinAABB.GetMaxPoint().y, NewCasterAABB.GetMaxPoint().z);
		MinAABB.Set(MaxP,MinP);
		SKVector3 Center = MinAABB.GetCenter();

		SKRay3 Ray(Center, SKVector3::ms_Front * (-1.0f));

		unsigned int Q;
		SKREAL tN, tF;
		if (MinAABB.RelationWith(Ray, Q, tN, tF) != SKINTERSECT)
		{
			DRAW_CSM_DEPTH;
			continue;
		}

		SKVector3 LigthPT = Center - SKVector3::ms_Front * tN * 10.0f;

		SKVector3 NewLightPT = LigthPT * LightRot.GetInverse();

		SKCamera LightCamera;
		LightCamera.CreateFromLookDir(NewLightPT, Dir);

		MinAABB.Set(MinAABB.GetMaxPoint() - LigthPT,MinAABB.GetMinPoint() - LigthPT);

		
		LightCamera.SetOrthogonal(MinAABB.GetMaxPoint().x - MinAABB.GetMinPoint().x,
			MinAABB.GetMaxPoint().y - MinAABB.GetMinPoint().y,
			MinAABB.GetMinPoint().z, MinAABB.GetMaxPoint().z);

		m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();

		m_CSMLightShadowMatrix[i] = m_LightShadowMatrix;

		DRAW_CSM_DEPTH;
		
	}
	pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), fNear, fFar);
}
void SKDirectionLight::GetLightRange()
{
	
	SKVector3 Point3 = GetWorldTranslate();
	SKVector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	SKVector3 Middle = Point3 + Dir * (m_fLightFunFar + m_fLightFunNear) * 0.5;
	SKOBB3 OBB(Dir, Up, Right, (m_fLightFunFar - m_fLightFunNear) * 0.5f, m_fLightFunHeight * 0.5f, m_fLightFunWidth * 0.5f, Middle);
	m_WorldRenderBV = OBB.GetAABB();
	if (HaveLightFun())
	{
		SKCamera LightCamera;
		LightCamera.CreateFromLookDir(Point3, Dir);
		LightCamera.SetOrthogonal(m_fLightFunWidth, m_fLightFunHeight, m_fLightFunNear, m_fLightFunFar);
		LightCamera.UpdateAll(0);
		m_MVP = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();
	}
}
bool SKDirectionLight::IsRelative(SKGeometry * pGeometry)
{
	if (!SKLight::IsRelative(pGeometry))
	{
		return false;
	}
	if (HaveLightFun())
	{
		SKAABB3 GeometryAABB = pGeometry->GetWorldAABB();
		if (GeometryAABB.RelationWith(m_WorldRenderBV) == SKNOINTERSECT)
		{
			return false;
		}
	}	
	return true;
}
void SKDirectionLight::DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	if (m_bEnable && m_bIsCastShadow && m_uiShadowType == ST_PROJECT)
	{
		SKVector3 Dir,Up,Right;
		GetWorldDir(Dir,Up,Right);
		SKDirShadowMapCuller	SMCuller;
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

		SKArray<SKDirShadowMapCuller> Temp;
		SKMeshNode * pCurMeshNode = NULL;
		for (unsigned int t = 0; t <= SKCuller::SKT_MAX; t++)
		{
			for (unsigned int j = 0; j < SMCuller.GetVisibleNum(t); j++)
			{
				SKRenderContext& RenderContext = SMCuller.GetVisibleSpatial(j, t);
				if (pCurMeshNode != RenderContext.m_pMeshNode)
				{
					Temp.AddElement(SKDirShadowMapCuller());
					Temp[Temp.GetNum() - 1].PushCameraPlane(*SMCuller.GetCamera());
					pCurMeshNode = RenderContext.m_pMeshNode;
				}
				Temp[Temp.GetNum() - 1].InsertObject(RenderContext, t);
			}
		}
		m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
		for (unsigned int i = 0; i < Temp.GetNum(); i++)
		{
			SKArray<SKAABB3> CasterAABBArray;
			GetCullerAABBArray(Temp[i], CasterAABBArray);
	
			SKAABB3 CasterAABB = GetMaxAABB(CasterAABBArray);
			SKVector3 Center = CasterAABB.GetCenter();

			SKRay3 Ray(Center, Dir * (-1.0f));

			unsigned int Q;
			SKREAL tN, tF;
			if (CasterAABB.RelationWith(Ray, Q, tN, tF) != SKINTERSECT)
			{
				continue;
			}

			SKVector3 LigthPT = Center - Dir * tN * 2.0f;

			SKCamera LightCamera;
			LightCamera.CreateFromLookAt(LigthPT, Center);

			LightCamera.UpdateAll(0);
			SKMatrix3X3W LightView = LightCamera.GetViewMatrix();

			SKAABB3 NewCasterAABB;
			NewCasterAABB.Transform(CasterAABB, LightView);


			LightCamera.SetOrthogonal(NewCasterAABB.GetMaxPoint().x - NewCasterAABB.GetMinPoint().x,
				NewCasterAABB.GetMaxPoint().y - NewCasterAABB.GetMinPoint().y,
				NewCasterAABB.GetMinPoint().z, NewCasterAABB.GetMaxPoint().z + 10000.0f);

			m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();


			m_pProjectShadowRenderTarget = SKResourceManager::Get2DRenderTarget(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_R16F, 0);
			SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, SKRenderer::SFT_D24S8, 0);

			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, SKCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pProjectShadowRenderTarget);
			m_pShadowMapSceneRender->Draw(Temp[i], dAppTime);
			m_pShadowTexture[0]->m_pTex = m_pProjectShadowRenderTarget->GetCreateBy();
			SKResourceManager::DisableDepthStencil(pDepthStencil);

			m_pProjectShadowSceneRender->Draw(Temp[i], dAppTime);
			DisableDepandenceShadowMap(Temp[i], dAppTime);
		}	
	}
}
bool SKDirectionLight::SetLightMaterial(SKMaterialR * pMaterial)
{
	if (SKLocalLight::SetLightMaterial(pMaterial))
	{
		return true;
	}
	return false;
}
bool SKDirectionLight::Cullby(SKCuller & Culler)
{
	if (HaveLightFun())
	{
		unsigned int uiVSF = Culler.IsVisible(m_WorldRenderBV, true);
		if (uiVSF == SKCuller::SKF_ALL || uiVSF == SKCuller::SKF_PARTIAL)
		{
			m_bEnable = true;
			Culler.InsertLight(this);

		}
	}
	else
	{
		m_bEnable = true;
		Culler.InsertLight(this);
	}
	return true;
}