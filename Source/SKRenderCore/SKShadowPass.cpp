#include "SKShadowPass.h"
#include "SKSpatial.h"
#include "SKBoneNode.h"
#include "SKShaderStringFactory.h"
#include "SKConstValue.h"
#include "SKGeometry.h"
#include "SKBoneNode.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKMaterial.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKCubeShadowPass,SKPass)
BEGIN_ADD_PROPERTY(SKCubeShadowPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCubeShadowPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(CubeShadowPassDraw, )
bool SKCubeShadowPass::InitialDefaultState()
{

	return 1;
}
bool SKCubeShadowPass::TerminalDefaultState()
{

	return 1;
}
SKCubeShadowPass::SKCubeShadowPass()
{

	

//	SKBlendDesc BlendDesc;
//	SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
//	m_RenderState.SetBlendState(pBlendState);
// 	SKRasterizerDesc RasterizerDesc;
// 	RasterizerDesc.m_uiCullType = SKRasterizerDesc::CT_CW;
// 	SKRasterizerState * pRasterizerState= SKResourceManager::CreateRasterizerState(RasterizerDesc);
// 	m_RenderState.SetRasterizerState(pRasterizerState);

}
SKCubeShadowPass::~SKCubeShadowPass()
{


}


bool SKCubeShadowPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(CubeShadowPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	SKMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (SKGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (SKLight*)m_pPointLight;


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, SKResourceManager::GetInnerVertexShaderMap(), SKUsedName::ms_cCubShadowVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetCubShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}
	
	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((SKGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
IMPLEMENT_RTTI(SKVolumeShadowPass,SKPass)
BEGIN_ADD_PROPERTY(SKVolumeShadowPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKVolumeShadowPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(VolumeShadowRenderPassDraw, )
bool SKVolumeShadowPass::InitialDefaultState()
{

	return 1;
}
bool SKVolumeShadowPass::TerminalDefaultState()
{

	return 1;
}
SKVolumeShadowPass::SKVolumeShadowPass()
{



	
	SKDepthStencilDesc DepthStencilDesc;
	DepthStencilDesc.m_bDepthWritable = false;
	DepthStencilDesc.m_uiDepthCompareMethod = SKDepthStencilDesc::CM_LESS;
	DepthStencilDesc.m_bStencilEnable = true;
	DepthStencilDesc.m_bTwoSideStencilMode = true;
	DepthStencilDesc.m_uiSPassZFailOP = SKDepthStencilDesc::OT_DECREMENT;
	DepthStencilDesc.m_uiCCW_SPassZFailOP = SKDepthStencilDesc::OT_INCREMENT;
	SKDepthStencilState * pDepthStencilState = SKResourceManager::CreateDepthStencilState(DepthStencilDesc);
	m_RenderState.SetDepthStencilState(pDepthStencilState);

	SKRasterizerDesc RasterizerDesc;
	RasterizerDesc.m_uiCullType = SKRasterizerDesc::CT_NONE;
	SKRasterizerState * pRasterizerState= SKResourceManager::CreateRasterizerState(RasterizerDesc);
	m_RenderState.SetRasterizerState(pRasterizerState);

	SKBlendDesc BlendDesc;
	BlendDesc.ucWriteMask[0] = SKBlendDesc::WM_NONE;
	SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);

	MSPara.m_GSShaderPath = _T("SKShadowVolume.txt");
	MSPara.m_GMainFunName = _T("GSMain");

}
SKVolumeShadowPass::~SKVolumeShadowPass()
{



}
void SKVolumeShadowPass::SetCustomConstant(MaterialShaderPara &MSPara, SKGShader * pGShader)
{
	if (!pGShader)
	{
		return;
	}
	static SKUsedName cViewProjectMatrix = _T("ViewProjectMatrix");
	static SKUsedName cLightInfo = _T("LightInfo");
	SKMatrix3X3W ViewProjectMatrix =  MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix();
	pGShader->SetParam(cViewProjectMatrix, &ViewProjectMatrix);
	SKVector3 LightInfo;
	if (MSPara.pShadowLight->GetLightType() == SKLight::LT_POINT)
	{
		LightInfo = MSPara.pShadowLight->GetWorldTranslate();
	}
	else
	{
		const SKMatrix3X3 &Rotator = MSPara.pShadowLight->GetWorldRotate();
		SKVector3 U, V, N;
		Rotator.GetUVN(U, V, N);

		LightInfo = N;
	}
	pGShader->SetParam(cLightInfo, &LightInfo);
}
bool SKVolumeShadowPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(VolumeShadowRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	SKMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}

	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (SKGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (SKLight*)m_pLocalLight;


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, SKResourceManager::GetInnerVertexShaderMap(), SKUsedName::ms_cVolumeShadowVertex))
	{
		return 0;
	}
	if (!GetGShader(MSPara, SKResourceManager::GetInnerGeometryShaderMap(), SKUsedName::ms_cVolumeShadowVertex))
	{
		return false;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetVolumeShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialGShaderConstant(MSPara, GetPassType(), m_pMaterialInstance->m_pCurGShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	SetCustomConstant(MSPara,m_pMaterialInstance->m_pCurGShader[GetPassType()]);
	if(!pRenderer->DrawMesh((SKGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}




IMPLEMENT_RTTI(SKShadowPass,SKPass)
BEGIN_ADD_PROPERTY(SKShadowPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKShadowPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(ShadowRenderPassDraw, )
bool SKShadowPass::InitialDefaultState()
{

	return 1;
}
bool SKShadowPass::TerminalDefaultState()
{

	return 1;
}
SKShadowPass::SKShadowPass()
{




}
SKShadowPass::~SKShadowPass()
{




}


bool SKShadowPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(ShadowRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	SKMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (SKGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (SKLight*)m_pLocalLight;


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, SKResourceManager::GetInnerVertexShaderMap(), SKUsedName::ms_cShadowVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((SKGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}



IMPLEMENT_RTTI(SKDualParaboloidShadowPass,SKPass)
BEGIN_ADD_PROPERTY(SKDualParaboloidShadowPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDualParaboloidShadowPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(DualParaboloidRenderPassDraw, )
bool SKDualParaboloidShadowPass::InitialDefaultState()
{

	return 1;
}
bool SKDualParaboloidShadowPass::TerminalDefaultState()
{

	return 1;
}
SKDualParaboloidShadowPass::SKDualParaboloidShadowPass()
{



}
SKDualParaboloidShadowPass::~SKDualParaboloidShadowPass()
{



}


bool SKDualParaboloidShadowPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(DualParaboloidRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;

	SKMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}

	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (SKGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.pShadowLight = (SKLight*)m_pPointLight;

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, SKResourceManager::GetInnerVertexShaderMap(), SKUsedName::ms_cDualParaboloidShadowVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetDualParaboloidShadowShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((SKGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
