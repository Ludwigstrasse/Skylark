#include "SKShader.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKShader,SKBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKShader)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKShader,SKBind)
REGISTER_PROPERTY(m_pUserConstant,UserConstant,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pUserSampler,UserSampler,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_ShaderKey,ShaderKey,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_Buffer,ShaderBuffer,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_MainFunName,MainFunName,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bCreatePara,bCreatePara,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pCacheBuffer,m_uiCacheBufferSize,CacheBuffer);
REGISTER_PROPERTY(m_uiArithmeticInstructionSlots,ArithmeticInstructionSlots,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiTextureInstructionSlots,TextureInstructionSlots,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiConstBufferSize, ConstBufferSize, SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKShader::SKShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile)
:SKBind(SKBind::MT_VRAM)
{
	m_pUserConstant.Clear();
	SKMAC_ASSERT(pBuffer);
	if (IsFromFile)
	{
		m_ResourceName = pBuffer;
	}
	else
	{
		m_Buffer = pBuffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	m_uiConstBufferSize = 0;
}
SKShader::SKShader()
:SKBind(SKBind::MT_VRAM)
{
	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	m_uiConstBufferSize = 0;
}
SKShader::SKShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile)
:SKBind(SKBind::MT_VRAM)
{
	m_pUserConstant.Clear();
	if (IsFromFile)
	{
		m_ResourceName = Buffer;
	}
	else
	{
		m_Buffer = Buffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
	m_bCreatePara = false;
	m_uiArithmeticInstructionSlots = 0;
	m_uiTextureInstructionSlots = 0;
	
}
SKShader::~SKShader()
{
	SKMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();
}
void SKShader::SetShaderString(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile)
{
	SKMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();

	SKMAC_ASSERT(pBuffer);
	if (IsFromFile)
	{
		m_ResourceName = pBuffer;
	}
	else
	{
		m_Buffer = pBuffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
}
void SKShader::SetShaderString(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile)
{
	SKMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = 0;
	ReleaseResource();

	if (IsFromFile)
	{
		m_ResourceName = Buffer;
	}
	else
	{
		m_Buffer = Buffer;
	}

	m_pCacheBuffer = NULL;
	m_uiCacheBufferSize = 0;
	m_MainFunName = MainFunName;
}
void SKShader::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			m_Buffer.Clear();
		}
	}
}
bool SKShader::SetParam(const SKUsedName &Name,SKTexAllState * pTexture,unsigned int uiIndex)
{
	if (!pTexture || !m_bCreatePara)
	{
		return false;
	}
	for (unsigned int i = 0 ; i < m_pUserSampler.GetNum() ;i++)
	{
		if (Name == m_pUserSampler[i]->GetName()
			&& pTexture->m_pTex->GetTexType() == m_pUserSampler[i]->GetTexType())
		{
			m_pUserSampler[i]->SetTex(pTexture,uiIndex);
			break;
		}	
	}
	return true;
}
bool SKShader::SetParam(const SKUsedName &Name,void * pDate)
{
	if (!pDate || !m_bCreatePara)
	{
		return false;
	}
	for (unsigned int i = 0 ; i < m_pUserConstant.GetNum() ;i++)
	{
		if (Name == m_pUserConstant[i]->GetShowName())
		{
			void * pConstanDate = m_pUserConstant[i]->GetDate();
			if (!pConstanDate)
			{
				return false;
			}
			SKMemcpy(pConstanDate,pDate,m_pUserConstant[i]->GetSize());
			break;
		}	
	}
	return true;
}
bool SKShader::SetCacheBuffer(void * pBuffer,unsigned int uiSize)
{
	if (!pBuffer || !uiSize)
	{
		return false;
	}
	SKMAC_DELETEA(m_pCacheBuffer);
	m_uiCacheBufferSize = uiSize;
	m_pCacheBuffer = SK_NEW UCHAR[uiSize];
	SKMemcpy(m_pCacheBuffer,pBuffer,uiSize);
	
	
	
	

	return 1;
}

