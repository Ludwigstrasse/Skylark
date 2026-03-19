#include "SKNormalDepthPass.h"
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
IMPLEMENT_RTTI(SKNormalDepthPass,SKPass)
BEGIN_ADD_PROPERTY(SKNormalDepthPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKNormalDepthPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(NormalDepthRenderPassDraw, )
bool SKNormalDepthPass::InitialDefaultState()
{

	return 1;
}
bool SKNormalDepthPass::TerminalDefaultState()
{
	return 1;
}
SKNormalDepthPass::SKNormalDepthPass()
{


	SKBlendDesc BlendDesc;
	SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);
	
}
SKNormalDepthPass::~SKNormalDepthPass()
{


}


bool SKNormalDepthPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(NormalDepthRenderPassDraw)
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

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara, SKResourceManager::GetInnerVertexShaderMap(), SKUsedName::ms_cNormalDepthVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetNormalDepthShaderMap(),pMaterial->GetResourceName()))
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

