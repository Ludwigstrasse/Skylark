#include "SKTexAllState.h"
#include "SK2DTexture.h"
#include "SKResourceManager.h"
#include "SKImage.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKTexAllState,SKObject)
SKPointer<SKTexAllState> SKTexAllState::Default;
SKPointer<SKTexAllState> SKTexAllState::ms_pOrenNayarLookUpTable;
SKPointer<SKTexAllState> SKTexAllState::ms_pStraussFLookUpTable;
SKPointer<SKTexAllState> SKTexAllState::ms_pStraussSLookUpTable;
SKPointer<SKTexAllState> SKTexAllState::ms_pCookTorranceLookUpTable;
SKPointer<SKTexAllState> SKTexAllState::ms_pIsotropicWardLookUpTable;
bool SKTexAllState::ms_bIsEnableASYNLoader = true;
bool SKTexAllState::ms_bIsEnableGC = true;
BEGIN_ADD_PROPERTY(SKTexAllState,SKObject)
REGISTER_PROPERTY(m_pTex,Tex,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_SamplerDesc,SamplerDesc,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bSRGB,bSRGB,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKTexAllState)
ADD_PRIORITY(VS2DTexture)
ADD_PRIORITY(SKSamplerState)
ADD_PRIORITY(SKResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
//#define DEFAULT_16FLOAT_TEXTURE
bool SKTexAllState::InitialDefaultState()
{

	SKSamplerDesc SamplerDesc;
	SamplerDesc.m_uiMag = SKSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMin = SKSamplerDesc::FM_LINE;
	SamplerDesc.m_uiMip = SKSamplerDesc::FM_LINE;
	SKSamplerStatePtr pSamplerState = SKResourceManager::CreateSamplerState(SamplerDesc);

	SKFileName FileName = SKResourceManager::ms_TexturePath + _T("SKDefault.bmp");
	Default = SKResourceManager::Load2DTexture(FileName.GetBuffer(),pSamplerState);
	if(!Default)
		return 0;

	//
#ifdef DEFAULT_16FLOAT_TEXTURE
	unsigned int uiTextureFormat = SKRenderer::SFT_R16F;
#else
	unsigned int uiTextureFormat = SKRenderer::SFT_R32F;
#endif

	unsigned int uiOrenNayarTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	unsigned short * pBuffer = SK_NEW unsigned short[uiOrenNayarTexSize * uiOrenNayarTexSize];	
#else
	SKREAL * pBuffer = SK_NEW SKREAL[uiOrenNayarTexSize * uiOrenNayarTexSize];
#endif		
	for (unsigned int i = 0 ; i < uiOrenNayarTexSize ;i++)
	{
		SKREAL VdotN = (i * 1.0f / (uiOrenNayarTexSize - 1)) * 2.0f - 1.0f;
		SKREAL AngleViewNormal = ACOS(VdotN);
		for (unsigned int j = 0 ; j < uiOrenNayarTexSize ; j++)
		{			
			SKREAL LdotN = (j * 1.0f / (uiOrenNayarTexSize - 1)) * 2.0f - 1.0f;
			SKREAL AngleLightNormal = ACOS(LdotN);
			
			SKREAL Alpha = Max(AngleViewNormal,AngleLightNormal);
			SKREAL Beta = Min(AngleViewNormal,AngleLightNormal);
			SKREAL fResult = ABS(SIN(Alpha) * TAN(Beta));
#ifdef DEFAULT_16FLOAT_TEXTURE
			pBuffer[i * uiOrenNayarTexSize + j] = FloatToHalf(fResult);	
#else
			pBuffer[i * uiOrenNayarTexSize + j] = fResult;	
#endif
		}
	}
	ms_pOrenNayarLookUpTable = SKResourceManager::Create2DTexture(uiOrenNayarTexSize,uiOrenNayarTexSize,uiTextureFormat,1,pBuffer);
	SKMAC_DELETEA(pBuffer);

	
	ms_pOrenNayarLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pOrenNayarLookUpTable)
	{
		return false;
	}



	unsigned int uiStraussTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	pBuffer = SK_NEW unsigned short[uiStraussTexSize];
#else
	pBuffer = SK_NEW SKREAL[uiStraussTexSize];
#endif
	SKREAL Kf = 1.12f;
	for (unsigned int i = 0 ; i < uiStraussTexSize ;i++)
	{
		SKREAL VdotN = (i * 1.0f / (uiStraussTexSize - 1)) * 2.0f - 1.0f;
		SKREAL fShadow = ((1.0f / ((VdotN - Kf) * (VdotN - Kf))) - 1.0f/(Kf * Kf)) / ((1.0f / ((1.0f - Kf) * (1.0f - Kf))) - 1.0f/(Kf * Kf));
#ifdef DEFAULT_16FLOAT_TEXTURE
		pBuffer[i] = FloatToHalf(fShadow);	
#else
		pBuffer[i] = fShadow;	
#endif
	}

	ms_pStraussFLookUpTable = SKResourceManager::Create1DTexture(uiStraussTexSize,uiTextureFormat,1,pBuffer);


	ms_pStraussFLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pStraussFLookUpTable)
	{
		SKMAC_DELETEA(pBuffer);
		return false;
	}

	SKREAL Ks = 1.01f;
	for (unsigned int i = 0 ; i < uiStraussTexSize ;i++)
	{
		SKREAL VdotN = (i * 1.0f / (uiStraussTexSize - 1)) * 2.0f - 1.0f;
		SKREAL fShadow = (1.0f / ((1.0f - Ks) * (1.0f - Ks)) - 1.0f / ((VdotN - Ks) * (VdotN - Ks))) / (1.0f / ((1.0f - Ks) * (1.0f - Ks)) - 1.0f / (Ks * Ks));
#ifdef DEFAULT_16FLOAT_TEXTURE
		pBuffer[i] = FloatToHalf(fShadow);	
#else
		pBuffer[i] = fShadow;	
#endif
	}

	ms_pStraussSLookUpTable = SKResourceManager::Create1DTexture(uiStraussTexSize,uiTextureFormat,1,pBuffer);
	SKMAC_DELETEA(pBuffer);

	ms_pStraussSLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pStraussSLookUpTable)
	{
		return false;
	}



	unsigned int uiCookTorranceTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	pBuffer = SK_NEW unsigned short[uiCookTorranceTexSize * uiCookTorranceTexSize];
#else
	pBuffer = SK_NEW SKREAL[uiCookTorranceTexSize * uiCookTorranceTexSize];
#endif
	for (unsigned int i = 0 ; i < uiCookTorranceTexSize ;i++)
	{
		SKREAL HdotN = (i * 1.0f / (uiCookTorranceTexSize - 1)) * 2.0f - 1.0f;

		for (unsigned int j = 0 ; j < uiCookTorranceTexSize ; j++)
		{			
			SKREAL Roughness = (j * 1.0f / (uiCookTorranceTexSize - 1));

			SKREAL r_sq = Roughness * Roughness;

			float r_a = 1.0f / (4.0f * r_sq * POW(HdotN,4.0f));
			float r_b = HdotN * HdotN - 1.0f;
			float r_c = r_sq * HdotN * HdotN;
			SKREAL fResult = r_a * EXP(r_b / r_c);
#ifdef DEFAULT_16FLOAT_TEXTURE
			pBuffer[i * uiCookTorranceTexSize + j] = FloatToHalf(fResult);	
#else
			pBuffer[i * uiCookTorranceTexSize + j] = fResult;	
#endif
		}
	}
	ms_pCookTorranceLookUpTable = SKResourceManager::Create2DTexture(uiCookTorranceTexSize,uiCookTorranceTexSize,uiTextureFormat,1,pBuffer);
	SKMAC_DELETEA(pBuffer);


	ms_pCookTorranceLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pCookTorranceLookUpTable)
	{
		return false;
	}

	unsigned int uiIsotropicWardTexSize = 128;
#ifdef DEFAULT_16FLOAT_TEXTURE
	pBuffer = SK_NEW unsigned short[uiIsotropicWardTexSize];
#else
	pBuffer = SK_NEW SKREAL[uiIsotropicWardTexSize];
#endif
	for (unsigned int i = 0 ; i < uiIsotropicWardTexSize ;i++)
	{
		SKREAL HdotN = (i * 1.0f / (uiIsotropicWardTexSize - 1)) * 2.0f - 1.0f;
		SKREAL fResult = -POW(TAN(ACOS(HdotN)),2.0f);
#ifdef DEFAULT_16FLOAT_TEXTURE
		pBuffer[i] = FloatToHalf(fResult);	
#else
		pBuffer[i] = fResult;
#endif

	}
	ms_pIsotropicWardLookUpTable = SKResourceManager::Create1DTexture(uiIsotropicWardTexSize,uiTextureFormat,1,pBuffer);
	SKMAC_DELETEA(pBuffer);


	ms_pIsotropicWardLookUpTable->SetSamplerState(pSamplerState);
	if (!ms_pIsotropicWardLookUpTable)
	{
		return false;
	}
	return 1;
}
bool SKTexAllState::TerminalDefaultState()
{
	Default = NULL;
	ms_pOrenNayarLookUpTable = NULL;
	ms_pStraussSLookUpTable = NULL;
	ms_pStraussFLookUpTable = NULL;
	ms_pCookTorranceLookUpTable = NULL;
	ms_pIsotropicWardLookUpTable = NULL;
	return 1;
}
SKTexAllState::SKTexAllState(SKTexture * pTexture)
{
	m_pTex = pTexture;
	m_pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	m_bSRGB = false;
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
}
SKTexAllState::SKTexAllState()
{
	m_pTex			= NULL;
	m_pSamplerState = NULL;
	m_bSRGB = false;
}
SKTexAllState::~SKTexAllState()
{
	m_pTex			= NULL;

	m_pSamplerState = NULL;
}
bool SKTexAllState::PostLoad(void * pDate)
{
	SKObject::PostLoad(pDate);
	m_pSamplerState = SKResourceManager::CreateSamplerState(m_SamplerDesc);
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
	return true;
}
bool SKTexAllState::PostClone(SKObject * pObjectSrc)
{
	SKObject::PostClone(pObjectSrc);
	m_pSamplerState = SKResourceManager::CreateSamplerState(m_SamplerDesc);
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
	return true;
}
void SKTexAllState::SetSRGBEable(bool bEnable)
{	
	m_bSRGB = bEnable;
	if (m_pTex)
	{
		m_pTex->m_bSRGB = m_bSRGB;
	}
}

