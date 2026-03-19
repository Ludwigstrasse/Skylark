#include "SKBlendState.h"
#include "SKShader.h"
#include "SKMaterial.h"
#include "SKGraphicInclude.h"
#include "SKResourceManager.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKBlendState,SKBind)
SKPointer<SKBlendState> SKBlendState::Default;
BEGIN_ADD_PROPERTY(SKBlendState,SKBind)
REGISTER_PROPERTY(m_BlendDesc,BlendDesc,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKBlendState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool SKBlendState::InitialDefaultState()
{
	SKBlendDesc BlendDesc;
	Default = SKResourceManager::CreateBlendState(BlendDesc);
	if(!Default)
		return 0;
	return 1;
}
bool SKBlendState::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
bool SKBlendState::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadBlendState(this,pID))
		return 0;
	return 1;
}
bool SKBlendState::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseBlendState(pID))
		return 0;
	return 1;
}
SKBlendState::SKBlendState()
{
	m_bIsStatic = 1;
}
SKBlendState::~SKBlendState()
{
	ReleaseResource();
}
IMPLEMENT_RTTI(SKBlendDesc,SKObject)
BEGIN_ADD_PROPERTY(SKBlendDesc,SKObject)
REGISTER_PROPERTY(bBlendEnable[0],BlendEnable0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[0],SrcBlend0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[0],DestBlend0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[0],BlendOp0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[0],AlphaBlendEnable0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[0],SrcBlendAlpha0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[0],DestBlendAlpha0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[0],BlendOpAlpha0,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[0],WriteMask0,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[1],BlendEnable1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[1],SrcBlend1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[1],DestBlend1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[1],BlendOp1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[1],AlphaBlendEnable1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[1],SrcBlendAlpha1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[1],DestBlendAlpha1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[1],BlendOpAlpha1,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[1],WriteMask1,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[2],BlendEnable2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[2],SrcBlend2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[2],DestBlend2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[2],BlendOp2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[2],AlphaBlendEnable2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[2],SrcBlendAlpha2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[2],DestBlendAlpha2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[2],BlendOpAlpha2,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[2],WriteMask2,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[3],BlendEnable3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[3],SrcBlend3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[3],DestBlend3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[3],BlendOp3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[3],AlphaBlendEnable3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[3],SrcBlendAlpha3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[3],DestBlendAlpha3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[3],BlendOpAlpha3,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[3],WriteMask3,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[4],BlendEnable4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[4],SrcBlend4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[4],DestBlend4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[4],BlendOp4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[4],AlphaBlendEnable4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[4],SrcBlendAlpha4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[4],DestBlendAlpha4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[4],BlendOpAlpha4,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[4],WriteMask4,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[5],BlendEnable5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[5],SrcBlend5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[5],DestBlend5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[5],BlendOp5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[5],AlphaBlendEnable5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[5],SrcBlendAlpha5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[5],DestBlendAlpha5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[5],BlendOpAlpha5,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[5],WriteMask5,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[6],BlendEnable6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[6],SrcBlend6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[6],DestBlend6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[6],BlendOp6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[6],AlphaBlendEnable6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[6],SrcBlendAlpha6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[6],DestBlendAlpha6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[6],BlendOpAlpha6,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[6],WriteMask6,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bBlendEnable[7],BlendEnable7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlend[7],SrcBlend7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlend[7],DestBlend7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOp[7],BlendOp7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bAlphaBlendEnable[7],AlphaBlendEnable7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucSrcBlendAlpha[7],SrcBlendAlpha7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucDestBlendAlpha[7],DestBlendAlpha7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucBlendOpAlpha[7],BlendOpAlpha7,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(ucWriteMask[7],WriteMask7,SKProperty::F_SAVE_LOAD_CLONE)

REGISTER_PROPERTY(bAlphaToCoverageEnable, AlphaToCoverageEnable, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(bIndependentBlendEnable, IndependentBlendEnable, SKProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[0], BlendColor0, SKProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[1], BlendColor1, SKProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[2], BlendColor2, SKProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(fBlendColor[3], BlendColor3, SKProperty::F_SAVE_LOAD_CLONE)
// REGISTER_PROPERTY(ucSampleMask, SampleMask, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKBlendDesc)
IMPLEMENT_INITIAL_END
