#include "SKDepthStencilState.h"
#include "SKShader.h"
#include "SKMaterial.h"
#include "SKGraphicInclude.h"
#include "SKResourceManager.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDepthStencilState,SKBind)
SKPointer<SKDepthStencilState> SKDepthStencilState::Default;
BEGIN_ADD_PROPERTY(SKDepthStencilState,SKBind)
REGISTER_PROPERTY(m_DepthStencilDesc,DepthStencilDesc,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDepthStencilState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool SKDepthStencilState::InitialDefaultState()
{
	SKDepthStencilDesc DepthStencilDesc;
	Default = SKResourceManager::CreateDepthStencilState(DepthStencilDesc);
	if(!Default)
		return 0;
	return 1;
}
bool SKDepthStencilState::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
bool SKDepthStencilState::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadDepthStencilState(this,pID))
		return 0;
	return 1;
}
bool SKDepthStencilState::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseDepthStencilState(pID))
		return 0;
	return 1;
}
SKDepthStencilState::SKDepthStencilState()
{
	m_bIsStatic = 1;
}
SKDepthStencilState::~SKDepthStencilState()
{
	ReleaseResource();
}
IMPLEMENT_RTTI(SKDepthStencilDesc,SKObject)
BEGIN_ADD_PROPERTY(SKDepthStencilDesc,SKObject)
REGISTER_PROPERTY(m_bDepthEnable,DepthEnable,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bDepthWritable,DepthWritable,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDepthCompareMethod,DepthCompareMethod,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bStencilEnable,StencilEnable,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiStencilCompareMethod,StencilCompareMethod,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiReference,Reference,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMask,Mask,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiWriteMask,WriteMask,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSPassZPassOP,SPassZPassOP,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSPassZFailOP,SPassZFailOP,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSFailZPassOP,SFailZPassOP,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_SPassZPassOP,CCW_SPassZPassOP,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_SPassZFailOP,CCW_SPassZFailOP,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_SFailZPassOP,CCW_SFailZPassOP,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bTwoSideStencilMode,TwoSideStencilMode,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCCW_StencilCompareMethod,CCW_StencilCompareMethod,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDepthStencilDesc)
IMPLEMENT_INITIAL_END
