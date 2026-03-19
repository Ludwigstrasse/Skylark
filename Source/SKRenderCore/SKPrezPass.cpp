#include "SKPrezPass.h"
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
IMPLEMENT_RTTI(SKPrezPass,SKPass)
SKPointer<SKPrezPass> SKPrezPass::Default;
BEGIN_ADD_PROPERTY(SKPrezPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPrezPass)
ADD_PRIORITY(SKBlendState)
ADD_PRIORITY(SKDepthStencilState)
ADD_PRIORITY(SKRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(PrezRenderPassDraw, )
bool SKPrezPass::InitialDefaultState()
{
	Default = SK_NEW SKPrezPass();
	if(!Default)
		return 0;
	return 1;
}
bool SKPrezPass::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
SKPrezPass::SKPrezPass()
{


	SKBlendDesc BlendDesc;
	BlendDesc.ucWriteMask[0] = SKBlendDesc::WM_NONE;
	SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
	m_RenderState.SetBlendState(pBlendState);

}
SKPrezPass::~SKPrezPass()
{


}


bool SKPrezPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(PrezRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
		return 0;


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (SKGeometry *)m_pSpatial;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara,SKResourceManager::GetInnerVertexShaderMap(),SKUsedName::ms_cPrezVertex))
	{
		return 0;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetInnerPixelShaderMap(),SKUsedName::ms_cPrezPiexl))
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

