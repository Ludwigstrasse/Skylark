#include "SKPostEffectPass.h"
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
IMPLEMENT_RTTI(SKPostEffectPass, SKPass)
BEGIN_ADD_PROPERTY(SKPostEffectPass, SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPostEffectPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(PostEffectRenderPassDraw, )
bool SKPostEffectPass::InitialDefaultState()
{

	return 1;
}
bool SKPostEffectPass::TerminalDefaultState()
{
	return 1;
}
SKPostEffectPass::SKPostEffectPass()
{


	MSPara.m_VSShaderPath = _T("SKPostEffectVSKShader.txt");
	MSPara.m_VMainFunName = _T("SKMain");

}
SKPostEffectPass::~SKPostEffectPass()
{


}


bool SKPostEffectPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(PostEffectRenderPassDraw)
	if (!pRenderer || !m_pSpatial || !m_pMaterialInstance)
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
	if (!GetVShader(MSPara, SKResourceManager::GetVertexShaderMap(), SKUsedName::ms_cPostEffectVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara, SKResourceManager::GetMaterialShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara, GetPassType(), m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara, GetPassType(), m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	SetCustomConstant(MSPara, m_pMaterialInstance->m_pCurVShader[GetPassType()], m_pMaterialInstance->m_pCurPShader[GetPassType()]);
	if (!pRenderer->DrawMesh((SKGeometry *)m_pSpatial, &m_RenderState,
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}
void SKPostEffectPass::SetCustomConstant(MaterialShaderPara &MSPara, SKVShader * pVShader, SKPShader * pPShader)
{
	//VS
	SKREAL Inv_Width = 1.0f / m_uiRTWidth;
	SKREAL Inv_Height = 1.0f / m_uiRTHeight;
	pVShader->SetParam(SKUsedName::ms_cPostInv_Width, &Inv_Width);
	pVShader->SetParam(SKUsedName::ms_cPostInv_Height, &Inv_Height);
	static SKUsedName PSColorBufferSampler = SKShaderStringFactory::ms_PSColorBufferSampler;
	pPShader->SetParam(PSColorBufferSampler, m_PColorBuffer);
}

