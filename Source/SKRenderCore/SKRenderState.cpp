#include "SKRenderState.h"
#include "SKShader.h"
#include "SKMaterial.h"
#include "SKGraphicInclude.h"
#include "SKResourceManager.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKRenderState,SKObject)
BEGIN_ADD_PROPERTY(SKRenderState,SKObject)
REGISTER_PROPERTY(m_DepthStencilDesc,DepthStencilDesc,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RasterizerDesc,RasterizerDesc,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_BlendDesc,BlendDesc,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RectArray,RectArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Plane,Plane,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKRenderState)
IMPLEMENT_INITIAL_END
bool SKRenderState::PostLoad(void * pDate)
{
	SKObject::PostLoad(pDate);
	m_pDepthStencilState = SKResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	m_pBlendState = SKResourceManager::CreateBlendState(m_BlendDesc);
	m_pRasterizerState = SKResourceManager::CreateRasterizerState(m_RasterizerDesc);
	return true;
}
bool SKRenderState::PostClone(SKObject * pObjectSrc)
{
	SKObject::PostClone(pObjectSrc);
	m_pDepthStencilState = SKResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	m_pBlendState = SKResourceManager::CreateBlendState(m_BlendDesc);
	m_pRasterizerState = SKResourceManager::CreateRasterizerState(m_RasterizerDesc);
	return true;
}
void SKRenderState::Inherit(const SKRenderState * pRenderState,unsigned int uiInheritFlag)
{
	if (!uiInheritFlag)
	{
		return ;
	}
	SKMAC_ASSERT(pRenderState);
	bool bReCreateDepthStencil = false;
	bool bReCreateRasterizer = false;
	bool bReCreateBlend = false;
	if (uiInheritFlag & IF_WIRE_ENABLE)
	{
		if (m_pRasterizerState->GetRasterizerDesc().m_bWireEnable != 
			pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable)
		{
			bReCreateRasterizer = true;
			m_RasterizerDesc.m_bWireEnable = pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable;
		}
	}
	if (bReCreateRasterizer)
	{
		m_pRasterizerState = SKResourceManager::CreateRasterizerState(m_RasterizerDesc);
	}
	if (bReCreateDepthStencil)
	{
		m_pDepthStencilState = SKResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	}
	if (bReCreateBlend)
	{
		m_pBlendState = SKResourceManager::CreateBlendState(m_BlendDesc);
	}
	return;
}
void SKRenderState::SwapCull()
{
	unsigned int uiChangeType[3] = {SKRasterizerDesc::CT_NONE,SKRasterizerDesc::CT_CCW,SKRasterizerDesc::CT_CW};
	m_RasterizerDesc.m_uiCullType = uiChangeType[m_RasterizerDesc.m_uiCullType];
	m_pRasterizerState = SKResourceManager::CreateRasterizerState(m_RasterizerDesc);
}