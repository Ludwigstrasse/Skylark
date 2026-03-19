#include "SKRasterizerState.h"
#include "SKShader.h"
#include "SKMaterial.h"
#include "SKGraphicInclude.h"
#include "SKResourceManager.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKRasterizerState,SKBind)
SKPointer<SKRasterizerState> SKRasterizerState::Default;
SKPointer<SKRasterizerState> SKRasterizerState::ScissorEnable;
BEGIN_ADD_PROPERTY(SKRasterizerState,SKBind)
REGISTER_PROPERTY(m_RasterizerDesc,RasterizerDesc,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKRasterizerState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool SKRasterizerState::InitialDefaultState()
{
	SKRasterizerDesc RasterizerDesc;
	Default = SKResourceManager::CreateRasterizerState(RasterizerDesc);
	if(!Default)
		return 0;

	RasterizerDesc.m_bScissorTestEnable = true;
	ScissorEnable = SKResourceManager::CreateRasterizerState(RasterizerDesc);
	if (!ScissorEnable)
		return 0;

	return 1;
}
bool SKRasterizerState::TerminalDefaultState()
{
	Default = NULL;
	ScissorEnable = NULL;
	return 1;
}
bool SKRasterizerState::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadRasterizerState(this,pID))
		return 0;
	return 1;
}
bool SKRasterizerState::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseRasterizerState(pID))
		return 0;
	return 1;
}
SKRasterizerState::SKRasterizerState()
{
	m_bIsStatic = 1;
}
SKRasterizerState::~SKRasterizerState()
{
	ReleaseResource();
}

IMPLEMENT_RTTI(SKRasterizerDesc,SKObject)
BEGIN_ADD_PROPERTY(SKRasterizerDesc,SKObject)
REGISTER_PROPERTY(m_bWireEnable,WireEnable,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCullType,CullType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bClipPlaneEnable,ClipPlaneEnable,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bScissorTestEnable,ScissorTestEnable,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fDepthBias,DepthBias,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKRasterizerDesc)
IMPLEMENT_INITIAL_END
