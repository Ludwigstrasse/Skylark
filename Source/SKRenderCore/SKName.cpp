#include "SKName.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
SKName::~SKName()
{

	m_uiID = 0;
	m_String.Clear();
	
}
SKName::SKName(const TCHAR * pChar,unsigned int uiID)
{
	SKMAC_ASSERT(pChar);
	m_uiID = uiID;
	m_String.Clear();
	m_String = pChar;
}
SKName::SKName(const SKString & String,unsigned int uiID)
{
	m_uiID = uiID;
	m_String.Clear();
	m_String = String;

}

SKUsedName::~SKUsedName()
{
	m_pName = NULL;
}
SKUsedName::SKUsedName()
{
	m_pName = NULL;
}
SKUsedName::SKUsedName(const TCHAR * pChar)
{
	m_pName = NULL;

	m_pName = SKResourceManager::CreateName(pChar);
}
SKUsedName::SKUsedName(const SKString & String)
{
	m_pName = NULL;
	m_pName = SKResourceManager::CreateName(String);
}
void SKUsedName::operator =(const SKString &String)
{
	m_pName = NULL;
	m_pName = SKResourceManager::CreateName(String);
		
}
void SKUsedName::operator =(const TCHAR *pChar)
{
	m_pName = NULL;

	m_pName = SKResourceManager::CreateName(pChar);
}
void SKUsedName::operator =(const SKUsedName & Name)
{
	m_pName = Name.m_pName;
}
SKUsedName SKUsedName::ms_cPrezBeUsedBone;
SKUsedName SKUsedName::ms_cMaterialVertexFormat;
SKUsedName SKUsedName::ms_cVolumeVertexFormat;
SKUsedName SKUsedName::ms_cLightFunKey;
SKUsedName SKUsedName::ms_cMaterialLightKey;
SKUsedName SKUsedName::ms_cLighted;
SKUsedName SKUsedName::ms_cPointLightShadowNum;
SKUsedName SKUsedName::ms_cPointLightVolumeShadowNum;
SKUsedName SKUsedName::ms_cPointLightParaboloidShadowNum;
SKUsedName SKUsedName::ms_cDirectionLightShadowNum;
SKUsedName SKUsedName::ms_cDirectionLightVolumeShadowNum;
SKUsedName SKUsedName::ms_cDirectionLightCSMShadowNum;
SKUsedName SKUsedName::ms_cBlinnPhong;
SKUsedName SKUsedName::ms_cOrenNayarLookUpTable;
SKUsedName SKUsedName::ms_cStraussLookUpTable;
SKUsedName SKUsedName::ms_cMaterialVertex;
SKUsedName SKUsedName::ms_cPrezVertex;
SKUsedName SKUsedName::ms_cPrezPiexl;
SKUsedName SKUsedName::ms_cPostGray;
SKUsedName SKUsedName::ms_cGammaCorrect;
SKUsedName SKUsedName::ms_cPostScreenQuad;
SKUsedName SKUsedName::ms_cPostVolumeShadowMap;
SKUsedName SKUsedName::ms_cPostInputTexture;
SKUsedName SKUsedName::ms_cPostInv_Width;
SKUsedName SKUsedName::ms_cPostInv_Height;
SKUsedName SKUsedName::ms_cNormalDepthVertex;
SKUsedName SKUsedName::ms_cCubShadowVertex;
SKUsedName SKUsedName::ms_cVolumeShadowVertex;
SKUsedName SKUsedName::ms_cShadowVertex;
SKUsedName SKUsedName::ms_cDualParaboloidShadowVertex;
SKUsedName SKUsedName::ms_cPassID;
SKUsedName SKUsedName::ms_cMainScene;
SKUsedName SKUsedName::ms_cCookTorranceSpecluarType;
SKUsedName SKUsedName::ms_cIsotropicWarLookUpTable;
SKUsedName SKUsedName::ms_cCubShadowFov;
SKUsedName SKUsedName::ms_cSpotLightShadowNum;
SKUsedName SKUsedName::ms_cLightFunVertex;
SKUsedName SKUsedName::ms_cPostEffectVertex;
SKUsedName SKUsedName::ms_cProjectShadowPreVertex;
SKUsedName SKUsedName::ms_cProjectShadowPrePixel;
SKUsedName SKUsedName::ms_cProjectShadowVertex;
SKUsedName SKUsedName::ms_cProjectShadowPixel;
SKUsedName SKUsedName::ms_cFontColor;
IMPLEMENT_PRIORITY(SKUsedName)
IMPLEMENT_INITIAL_ONLY_BEGIN(SKUsedName)
ADD_PRIORITY(SKResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState);
IMPLEMENT_INITIAL_ONLY_END
bool SKUsedName::InitialDefaultState()
{
	ms_cPrezBeUsedBone = _T("PrezBeUsedBone");
	ms_cMaterialVertexFormat = _T("MaterialVertexFormat");
	ms_cLightFunKey= _T("LightFunKey");
	ms_cMaterialLightKey = _T("MaterialLightKey");
	ms_cMaterialVertex =_T("MaterialVertex");
	ms_cNormalDepthVertex = _T("NormalDepthVertex");
	ms_cCubShadowVertex = _T("CubShadowVertex");
	ms_cVolumeShadowVertex = _T("VolumeShadowVertex");
	ms_cVolumeVertexFormat = _T("VolumeVertexFormat");
	ms_cShadowVertex = _T("ShadowVertex");
	ms_cDualParaboloidShadowVertex = _T("DualParaboloidShadowVertex");
	ms_cLightFunVertex = _T("LightFunVertex");
	ms_cPostEffectVertex = _T("PostEffectVertex");

	ms_cPrezVertex = _T("PrezVertex");
	ms_cPrezPiexl = _T("PrezPiexl");
	ms_cPostGray =_T("PostGray");
	ms_cGammaCorrect = _T("GammaCorrect");
	ms_cPostScreenQuad = _T("PostScreenQuad");
	ms_cPostVolumeShadowMap = _T("PostVolumeShadowMap");
	ms_cPostInputTexture = _T("PostInputTexture");
	ms_cPostInv_Width = _T("PostInv_Width");
	ms_cPostInv_Height = _T("PostInv_Height");
	ms_cBlinnPhong = _T("BlinnPhone");
	ms_cOrenNayarLookUpTable = _T("OrenNayarLookUpTable");
	ms_cStraussLookUpTable = _T("StraussLookUpTable");
	ms_cCookTorranceSpecluarType = _T("CookTorranceSpecluarType");
	ms_cIsotropicWarLookUpTable = _T("IsotropicWarLookUpTable");
	ms_cPassID = _T("PassID");
	ms_cMainScene = _T("Main");
	ms_cLighted = _T("bLighted");
	ms_cPointLightShadowNum = _T("PointLightShadowNum");
	ms_cPointLightVolumeShadowNum = _T("PointLightVolumeShadowNum");
	ms_cPointLightParaboloidShadowNum = _T("PointLightParaboloidShadowNum");
	ms_cDirectionLightShadowNum = _T("DirectionLightShadowNum");
	ms_cDirectionLightVolumeShadowNum = _T("DirectionLightVolumeShadowNum");
	ms_cDirectionLightCSMShadowNum = _T("DirectionLightCSMShadowNum");
	ms_cSpotLightShadowNum = _T("SpotLightShadowNum");

	ms_cCubShadowFov = _T("CubShadowFov");


	ms_cProjectShadowPreVertex = _T("ProjectShadowPreVertex");
	ms_cProjectShadowPrePixel = _T("ProjectShadowPrePixel");

	ms_cProjectShadowVertex = _T("ProjectShadowVertex");
	ms_cProjectShadowPixel = _T("ProjectShadowPixel");

	ms_cFontColor = _T("FontColor");
	return 1;
}
bool SKUsedName::TerminalDefaultState()
{
	return 1;
}
void SKUsedName::Archive(SKStream & Stream)
{
	if (Stream.GetStreamFlag() == SKStream::AT_LOAD)
	{
		SKString String;
		Stream.ReadString(String);
		if (String == SKString::ms_StringNULL)
		{
			m_pName = NULL;
		}
		else
		{
			m_pName = SKResourceManager::CreateName(String);
		}
	}
	else if (Stream.GetStreamFlag() == SKStream::AT_SAVE)
	{
		if (m_pName)
		{
			Stream.WriteString(m_pName->GetString());
		}
		else
		{
			Stream.WriteString(SKString::ms_StringNULL);
		}
	}
	else if (Stream.GetStreamFlag() == SKStream::AT_SIZE)
	{
		if (m_pName)
		{

			Stream.AddBufferSize(SKStream::GetStrDistUse(m_pName->GetString()));
		}
		else
		{

			Stream.AddBufferSize(SKStream::GetStrDistUse(SKString::ms_StringNULL));
		}
	}
	
}
void SKUsedName::CopyFrom(SKCustomArchiveObject * pObject,SKMap<SKObject *,SKObject*>& CloneMap)
{
	SKUsedName * pUsedName = (SKUsedName *)pObject;
	*this = *pUsedName;
}
namespace SKEngine2
{
bool operator ==(const SKUsedName &Name1,const SKUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 1;
	}
	else if (!Name1.m_pName || !Name2.m_pName)
	{
		return 0;
	}
	return Name1.m_pName->GetID() == Name2.m_pName->GetID();
}


bool operator !=(const SKUsedName &Name1,const SKUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (!Name1.m_pName || !Name2.m_pName)
	{
		return 1;
	}
	return Name1.m_pName->GetID() != Name2.m_pName->GetID();
}

bool operator ==(const SKUsedName &Name,const SKString & String)
{
	if (!Name.m_pName)
	{
		return 0;
	}
	return Name.m_pName->GetString() == String;
}


bool operator !=(const SKUsedName &Name,const SKString & String)
{
	if (!Name.m_pName)
	{
		return 1;
	}
	return Name.m_pName->GetString() != String;
}

bool operator ==(const SKUsedName &Name,const TCHAR * pChar)
{
	if (!Name.m_pName && !pChar)
	{
		return 1;
	}
	else if (!Name.m_pName || !pChar)
	{
		return 0;
	}
	return Name.m_pName->GetString() == pChar;
}


bool operator !=(const SKUsedName &Name,const TCHAR * pChar)
{
	if (!Name.m_pName && !pChar)
	{
		return 0;
	}
	else if (!Name.m_pName || !pChar)
	{
		return 1;
	}
	return Name.m_pName->GetString() != pChar;
}

bool operator >(const SKUsedName &Name1,const SKUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (Name1.m_pName && !Name2.m_pName)
	{
		return 1;
	}
	else if (!Name1.m_pName && Name2.m_pName)
	{
		return 0;
	}
	return Name1.m_pName->GetID() > Name2.m_pName->GetID();
}

bool operator <(const SKUsedName &Name1,const SKUsedName &Name2)
{
	if (!Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (Name1.m_pName && !Name2.m_pName)
	{
		return 0;
	}
	else if (!Name1.m_pName && Name2.m_pName)
	{
		return 1;
	}
	return Name1.m_pName->GetID() < Name2.m_pName->GetID();
}
}