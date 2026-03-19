#include "SKLightFunPass.h"
#include "SKSpatial.h"
#include "SKBoneNode.h"
#include "SKShaderStringFactory.h"
#include "SKConstValue.h"
#include "SKGeometry.h"
#include "SKBoneNode.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKMaterial.h"
#include "SKProfiler.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKLightFunPass,SKPass)
BEGIN_ADD_PROPERTY(SKLightFunPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKLightFunPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(LightFunPRenderPassDraw, )
bool SKLightFunPass::InitialDefaultState()
{

	return 1;
}
bool SKLightFunPass::TerminalDefaultState()
{
	return 1;
}
SKLightFunPass::SKLightFunPass()
{



}
SKLightFunPass::~SKLightFunPass()
{


}


bool SKLightFunPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(LightFunPRenderPassDraw)
	if(!pRenderer || !m_pSpatial || !m_pMaterialInstance)
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
	MSPara.pShadowLight = m_pLight;

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, SKResourceManager::GetInnerVertexShaderMap(), SKUsedName::ms_cLightFunVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetMaterialShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	pMaterial->SetGlobleValue(this,m_uiPassId,m_pMaterialInstance->m_pCurVShader[GetPassType()],m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	if(!pRenderer->DrawMesh((SKGeometry *)m_pSpatial,&m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}

