#include "SKSamplerState.h"
#include "SKShader.h"
#include "SKMaterial.h"
#include "SKGraphicInclude.h"
#include "SKResourceManager.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSamplerState,SKBind)
SKPointer<SKSamplerState> SKSamplerState::Default;
SKPointer<SKSamplerState> SKSamplerState::DoubleLine;
SKPointer<SKSamplerState> SKSamplerState::TriLine;
SKPointer<SKSamplerState> SKSamplerState::ShadowMapSampler;
SKPointer<SKSamplerState> SKSamplerState::BorderARGB0Sampler;
SKPointer<SKSamplerState> SKSamplerState::LightFunctionSampler;
BEGIN_ADD_PROPERTY(SKSamplerState,SKBind)
REGISTER_PROPERTY(m_SamplerDesc,SamplerDesc,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSamplerState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool SKSamplerState::InitialDefaultState()
{
	SKSamplerDesc SamplerDesc;
	Default = SKResourceManager::CreateSamplerState(SamplerDesc);
	if(!Default)
		return 0;


	SamplerDesc.m_uiMag = SKSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = SKSamplerDesc::FM_LINE;
	DoubleLine = SKResourceManager::CreateSamplerState(SamplerDesc);
	if(!DoubleLine)
		return 0;


	SamplerDesc.m_uiMag = SKSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = SKSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMip = SKSamplerDesc::FM_LINE;
	TriLine = SKResourceManager::CreateSamplerState(SamplerDesc);
	if(!TriLine)
		return 0;


	SKSamplerDesc ShadowSamplerDesc;
	ShadowSamplerDesc.m_uiMag = SKSamplerDesc::FM_LINE;
	ShadowSamplerDesc.m_uiMin = SKSamplerDesc::FM_LINE;
	ShadowSamplerDesc.m_uiCoordU = SKSamplerDesc::CM_BORDER;
	ShadowSamplerDesc.m_uiCoordV = SKSamplerDesc::CM_BORDER;
	ShadowSamplerDesc.m_BorderColor = SKColorRGBA(1.0f,1.0f,1.0f,1.0f);
	ShadowMapSampler = SKResourceManager::CreateSamplerState(ShadowSamplerDesc);
	if (!ShadowMapSampler)
		return 0;

	SKSamplerDesc BorderARGB0Desc;
	BorderARGB0Desc.m_uiMag = SKSamplerDesc::FM_LINE;
	BorderARGB0Desc.m_uiMin = SKSamplerDesc::FM_LINE;
	BorderARGB0Desc.m_uiCoordU = SKSamplerDesc::CM_BORDER;
	BorderARGB0Desc.m_uiCoordV = SKSamplerDesc::CM_BORDER;
	BorderARGB0Desc.m_BorderColor = SKColorRGBA(1.0f,0.0f,0.0f,0.0f);
	BorderARGB0Sampler = SKResourceManager::CreateSamplerState(BorderARGB0Desc);
	if (!BorderARGB0Sampler)
		return 0;

	SKSamplerDesc LightFunctionDesc;
	LightFunctionDesc.m_uiMag = SKSamplerDesc::FM_LINE;
	LightFunctionDesc.m_uiMin = SKSamplerDesc::FM_LINE;
	LightFunctionDesc.m_uiCoordU = SKSamplerDesc::CM_BORDER;
	LightFunctionDesc.m_uiCoordV = SKSamplerDesc::CM_BORDER;
	LightFunctionSampler = SKResourceManager::CreateSamplerState(LightFunctionDesc);
	if (!LightFunctionSampler)
		return 0;
	return 1;
}
bool SKSamplerState::TerminalDefaultState()
{
	Default = NULL;
	DoubleLine = NULL;
	TriLine = NULL;
	ShadowMapSampler = NULL;
	BorderARGB0Sampler = NULL;
	LightFunctionSampler = NULL;
	return 1;
}
bool SKSamplerState::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadSamplerState(this,pID))
		return 0;
	return 1;
}
bool SKSamplerState::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseSamplerState(pID))
		return 0;
	return 1;
}
SKSamplerState::SKSamplerState()
{
	m_bIsStatic = 1;
}
SKSamplerState::~SKSamplerState()
{
	ReleaseResource();
}

IMPLEMENT_RTTI(SKSamplerDesc,SKObject)
BEGIN_ADD_PROPERTY(SKSamplerDesc,SKObject)
REGISTER_PROPERTY(m_uiMag,Mag,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMin,Min,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMip,Mip,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMipLevel,MipLevel,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiAniLevel,AniLevel,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordU,CoordU,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordV,CoordV,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCoordW,CoordW,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_BorderColor,BorderColor,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MipLODBias, MipLODBias, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSamplerDesc)
IMPLEMENT_INITIAL_END
