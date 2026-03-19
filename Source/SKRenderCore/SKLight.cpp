#include "SKLight.h"
#include "SKGraphicInclude.h"
#include "SKSceneRender.h"
#include "SKStream.h"
#include "SKMaterial.h"
#include "SKRenderTarget.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKLight, SKNodeComponent)
BEGIN_ADD_PROPERTY(SKLight, SKNodeComponent)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKLight)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKLight::SKLight()
{

	m_bEnable = true;

	
}
SKLight::~SKLight()
{

	
}
bool SKLight::IsRelative(SKGeometry * pGeometry)
{
	if(pGeometry->GetMeshNode()->m_bLighted)
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}
bool SKLight::Cullby(SKCuller & Culler)
{
	m_bEnable = true;
	Culler.InsertLight(this);
	return true;
}
void SKLight::UpdateLightState(double dAppTime)
{
	SKNodeComponent::UpdateLightState(dAppTime);
	m_pAllLight.AddElement(this);
}
void SKLight::UpdateAll(double dAppTime)
{
	SKNodeComponent::UpdateAll(dAppTime);
	m_bEnable = false;
}
void SKLight::UpdateTransform(double dAppTime)
{
	if (m_bIsChanged)
	{
		GetLightRange();
	}
	SKNodeComponent::UpdateTransform(dAppTime);
}
void SKLight::BuildSceneInfo(SKScene * pScene)
{

}
IMPLEMENT_RTTI_NoCreateFun(SKLocalLight,SKLight)
BEGIN_ADD_PROPERTY(SKLocalLight,SKLight)
REGISTER_PROPERTY(m_Diffuse, Diffuse, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Specular, Specular, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bIsCastShadow, IsCastShadow, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_uiRTWidth, RTWidth, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ZBias, ZBias, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_MVP, MVP, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiLightMaterialRTWidth, LightMaterialRTWidth, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ProjectShadowColor, ProjectShadowColor, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_LightFunScale, LightFunScale, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_LightFunOffset, LightFunOffset, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pLightMaterial, LightMaterial, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_ProjectShadowFallOff, ProjectShadowFallOff, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKLocalLight)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKLocalLight::SKLocalLight()
{

	m_bEnable = true;
	m_Diffuse = SKColorRGBA(1.0f,1.0f,1.0f,1.0f);
	m_Specular = SKColorRGBA(1.0f,1.0f,1.0f,1.0f);
	m_bIsCastShadow = true;
	m_pShadowTexture.AddElement(SK_NEW SKTexAllState());
	m_ZBias = 0.0f;
	m_pLightFunDiffuseTexture = NULL;
	m_uiLightMaterialRTWidth = 1024;
	m_uiRTWidth = 1024;
	m_pLMSceneRender = NULL;
	m_LightFunScale = SKVector2(0.5f, -0.5f);
	m_LightFunOffset = SKVector2(0.5f, 0.5f);
	m_ProjectShadowColor = SKColorRGBA(0.2f,0.2f,0.2f,1.0f);
	m_ProjectShadowFallOff = 1.0f;
	m_bShadowMapDrawEnd = false;
	m_MVP.Identity();
}
SKLocalLight::~SKLocalLight()
{
	m_pShadowTexture.Clear();
	m_pLightFunDiffuseTexture = NULL;
	m_pLMSceneRender = NULL;
}
void SKLocalLight::SetCastShadow(bool bIsCastShadow)
{
	m_bIsCastShadow = bIsCastShadow;
}
void SKLocalLight::DrawNoDepandenceShadowMap(SKCuller & CurCuller,double dAppTime)
{

}
void SKLocalLight::DisableNoDepandenceShadowMap(double dAppTime)
{
	m_bShadowMapDrawEnd = false;
}
void SKLocalLight::DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{

}
void SKLocalLight::DisableDepandenceShadowMap(SKCuller & CurCuller, double dAppTime)
{

}
void SKLocalLight::DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{

}
void SKLocalLight::DrawVolumeShadow(SKCuller & CurCuller, double dAppTime)
{

}
void SKLocalLight::SetLightMaterialRtWidth(unsigned int uiRtWidth)
{
	if (uiRtWidth == m_uiLightMaterialRTWidth)
	{
		return ;
	}

	m_uiLightMaterialRTWidth = uiRtWidth;

	if (m_pLightMaterial)
	{
		m_pLightFunDiffuseTexture->m_pTex = SK_NEW VS2DTexture(m_uiLightMaterialRTWidth,m_uiLightMaterialRTWidth,SKRenderer::SFT_A8R8G8B8);
		m_pLightFunDiffuseRenderTarget = SKResourceManager::CreateRenderTarget(m_pLightFunDiffuseTexture->m_pTex);
	}
}
bool SKLocalLight::SetLightMaterial(SKMaterialR * pMaterial)
{
	if (pMaterial)
	{
		m_pLightMaterial = SK_NEW SKMaterialInstance(pMaterial);
		m_pLightFunDiffuseTexture = SK_NEW SKTexAllState();
		m_pLightFunDiffuseTexture->SetSamplerState((SKSamplerState*)SKSamplerState::GetLightFunctionSampler());
		m_pLightFunDiffuseTexture->m_pTex = SK_NEW VS2DTexture(m_uiLightMaterialRTWidth,m_uiLightMaterialRTWidth,SKRenderer::SFT_A8R8G8B8);
		m_pLightFunDiffuseRenderTarget = SKResourceManager::CreateRenderTarget(m_pLightFunDiffuseTexture->m_pTex);

		m_pLMSceneRender = SK_NEW SKLightMaterialSceneRender();
		m_pLMSceneRender->m_pMaterialInstacne = m_pLightMaterial;
		m_pLMSceneRender->SetParam(SKRenderer::CF_COLOR,SKColorRGBA(0.0f,0.0f,0.0f,0.0f),1.0f,0);

	}
	else
	{
		m_pLightFunDiffuseTexture = NULL;
		m_pLightMaterial = NULL;
		m_pLMSceneRender = NULL;
		m_pLightFunSpecularRenderTarget = NULL;
		m_pLightFunDiffuseRenderTarget = NULL;
	}
	m_bIsChanged = true;
	return true;
}
void SKLocalLight::UpdateAll(double dAppTime)
{
	SKLight::UpdateAll(dAppTime);
}
void SKLocalLight::DrawLightMaterial(double dAppTime)
{
	if (m_pLightMaterial && m_bEnable)
	{
		SKMaterial * pMaterial = m_pLightMaterial->GetMaterial();
		SKLightShaderFunction * pLightShaderFunction = pMaterial->GetLightShaderFunction();
		if (pLightShaderFunction)
		{
			SKDepthStencil * pDepthStencil = SKResourceManager::GetDepthStencil(m_uiLightMaterialRTWidth,m_uiLightMaterialRTWidth,SKRenderer::SFT_D24S8,0);
			m_pLMSceneRender->ClearRTAndDepth();
			m_pLMSceneRender->AddRenderTarget(m_pLightFunDiffuseRenderTarget);
			m_pLMSceneRender->SetDepthStencil(pDepthStencil);
			m_pLMSceneRender->m_pLight = this;
			SKCuller Temp;
			m_pLMSceneRender->Draw(Temp,dAppTime);
			SKResourceManager::DisableDepthStencil(pDepthStencil);
		}
	}
	
}
void SKLocalLight::GetCullerAABBArray(SKCuller & Culler,SKArray<SKAABB3> & AABBArray,unsigned int GetFlag)
{

	for (unsigned int i = 0 ; i < SKCuller::SKT_MAX ;i++)
	{

		Culler.GetAllVisibleAABB(i, SKCuller::RG_NORMAL, AABBArray, GetFlag);

	}
}
IMPLEMENT_RTTI_NoCreateFun(SKIndirectLight,SKLight)
BEGIN_ADD_PROPERTY(SKIndirectLight,SKLight)
REGISTER_PROPERTY(m_Range, Range, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKIndirectLight)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKIndirectLight::SKIndirectLight()
{
	m_bInheritScale = false;
	m_bInheritRotate = false;

	m_Range = SKVector3(999999.0f, 999999.0f, 999999.0f);
}
SKIndirectLight::~SKIndirectLight()
{

}
void SKIndirectLight::SetLocalRotate(const SKMatrix3X3 & Rotate)
{

}
bool SKIndirectLight::IsRelative(SKGeometry * pGeometry)
{
	if (!SKLight::IsRelative(pGeometry))
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
void SKIndirectLight::SetLocalTransform(const SKTransform & LocalTransform)
{
	SKVector3 Tranlation = LocalTransform.GetTranslate();
	SetLocalTranslate(Tranlation);
}
void SKIndirectLight::SetLocalMat(const SKMatrix3X3W SKMat)
{
	SKVector3 Tranlation = SKMat.GetTranslation();
	SetLocalTranslate(Tranlation);
}
void SKIndirectLight::SetLocalScale(const SKVector3 & fScale)
{

}
void SKIndirectLight::GetLightRange()
{
	SKVector3 Pos = GetWorldTranslate();
	m_WorldRenderBV = SKAABB3(Pos, m_Range.x, m_Range.y, m_Range.z);
}
bool SKIndirectLight::Cullby(SKCuller & Culler)
{
	unsigned int uiVSF = Culler.IsVisible(m_WorldRenderBV, true);
	if (uiVSF == SKCuller::SKF_ALL || uiVSF == SKCuller::SKF_PARTIAL)
	{
		m_bEnable = true;
		Culler.InsertLight(this);

	}	

	return true;
}
void SKLocalLight::BuildSceneInfo(SKScene * pScene)
{
	SKLight::BuildSceneInfo(pScene);
}

