#include "SKFont.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKFont,SKBind)
BEGIN_ADD_PROPERTY(SKFont,SKBind)
REGISTER_PROPERTY(m_Face,Face,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSize,Size,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bBold,Bold,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bItalic,Italic,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pFontTex, FontTex, SKProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY_FIXED_DATA(m_pCharacterData, ASCII_NUM + 1, CharacterData, true)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKFont)
ADD_PRIORITY(SKResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
SKPointer<SKFont> SKFont::Default;
bool SKFont::ms_bIsEnableASYNLoader = true;
bool SKFont::ms_bIsEnableGC = true;
bool SKFont::InitialDefaultState()
{
	SKFileName FileName = SKResourceManager::ms_FontPath + _T("SKArial.FONT");
	Default = SKResourceManager::NewLoadFont(FileName.GetBuffer());
	if (!Default)
	{
		return false;
	}
	return 1;
}
bool SKFont::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}
SKFont::SKFont(const TCHAR* acFace, unsigned int uiSize, bool bBold,
				bool bItalic)
{
	m_bItalic = bItalic;
	if(acFace)
	{
		m_Face = acFace;
	}
	else
	{
		m_Face = _T("Arial");
	}
	m_uiSize = uiSize;
	m_bBold = bBold;
	m_pFontTex = NULL;
	m_pCharacterData = NULL;
	m_pFontTexAllState = NULL;
}
SKFont::~SKFont()
{
	m_pFontTex = NULL;
	SKMAC_DELETEA(m_pCharacterData);
	m_pFontTexAllState = NULL;
}
SKFont::SKFont()
{
	m_bItalic = 1;
	m_Face = _T("Arial");
	m_uiSize = 20;
	m_bBold = 0;
	m_pCharacterData = NULL;
	m_pFontTexAllState = NULL;
	m_pFontTex = NULL;
}
void SKFont::SetFontData(VS2DTexture * pTexture, SKREAL * pCharacterData)
{
	if (!pTexture || !pCharacterData)
	{
		return;
	}
	m_pFontTex = pTexture;
	SKMAC_DELETEA(m_pCharacterData);
	m_pCharacterData = SK_NEW SKREAL[ASCII_NUM + 1];
	SKMemcpy(m_pCharacterData, pCharacterData, sizeof(SKREAL)* (ASCII_NUM + 1));
	return;
}
bool SKFont::PostLoad(void * pDate)
{
	SKObject::PostLoad(pDate);
	if (m_pFontTex)
	{
		SKSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = SKSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = SKSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = SKSamplerDesc::FM_LINE;
		SKSamplerStatePtr pSamplerState = SKResourceManager::CreateSamplerState(SamplerDesc);
		m_pFontTexAllState = SK_NEW SKTexAllState(m_pFontTex);
		m_pFontTexAllState->SetSamplerState(pSamplerState);
	}
	return true;
}
bool SKFont::PostClone(SKObject * pObjectSrc)
{
	SKObject::PostClone(pObjectSrc);
	if (m_pFontTex)
	{
		SKSamplerDesc SamplerDesc;
		SamplerDesc.m_uiMag = SKSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMin = SKSamplerDesc::FM_LINE;
		SamplerDesc.m_uiMip = SKSamplerDesc::FM_LINE;
		SKSamplerStatePtr pSamplerState = SKResourceManager::CreateSamplerState(SamplerDesc);
		m_pFontTexAllState = SK_NEW SKTexAllState(m_pFontTex);
		m_pFontTexAllState->SetSamplerState(pSamplerState);
	}
	return true;
}