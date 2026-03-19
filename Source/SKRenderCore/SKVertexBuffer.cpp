#include "SKVertexBuffer.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKResourceManager.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKVertexBuffer,SKBind)
BEGIN_ADD_PROPERTY(SKVertexBuffer,SKBind)
REGISTER_PROPERTY(m_pDate[0],PositionData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[1],TextureData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[2],NormalData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[3],TangetData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[4],BinormalData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[5],PSizeData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[6],ColorData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[7],FogData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[8],DepthData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[9],BlendWeightData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pDate[10],BlendIndicesData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiVertexNum,VertexNum,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiOneVertexSize,OneVertexSize,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKVertexBuffer)
IMPLEMENT_INITIAL_END
SKVertexBuffer::SKVertexBuffer(bool bIsStatic)
{
	for(unsigned int i = 0 ; i < SKVertexFormat::VF_MAX ; i++)
	{
		m_pDate[i].Clear();
	}
	m_uiVertexNum = 0;
	m_bIsStatic = bIsStatic;
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_pLockDate = NULL;
}
SKVertexBuffer::SKVertexBuffer()
{
	
	for(unsigned int i = 0 ; i < SKVertexFormat::VF_MAX ; i++)
	{
		m_pDate[i].Clear();
	}
	m_uiVertexNum = 0;
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_pLockDate = NULL;
}
SKVertexBuffer::~SKVertexBuffer()
{
	ReleaseResource();
	for(unsigned int i = 0 ; i < SKVertexFormat::VF_MAX ; i++)
	{
		m_pDate[i].Clear();
	}
	m_uiVertexNum = 0;
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_pLockDate = NULL;
}


bool SKVertexBuffer::SetDate(SKDataBuffer * pDate,unsigned int uiVF)
{
	if(!pDate || m_pVertexFormat || uiVF >= SKVertexFormat::VF_MAX)
		return 0;

	if(!pDate->GetDate())
		return 0;
	if(uiVF == SKVertexFormat::VF_POSITION)
	{
		
				
		m_pDate[uiVF].AddElement(pDate);
		

	}
	else if(uiVF == SKVertexFormat::VF_NORMAL)
	{

		m_pDate[uiVF].AddElement(pDate);
	}
	else if(uiVF == SKVertexFormat::VF_PSIZE)
	{
		if(!m_pDate[uiVF].GetNum())
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_COLOR)
	{
		if(m_pDate[uiVF].GetNum() < 2)
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_BLENDWEIGHT)
	{
		if(!m_pDate[uiVF].GetNum())
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_BLENDINDICES)
	{
		if(!m_pDate[uiVF].GetNum())
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_BINORMAL)
	{
		if(!m_pDate[uiVF].GetNum())
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_TANGENT)
	{
		if(!m_pDate[uiVF].GetNum())
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_FOG)
	{
		if(!m_pDate[uiVF].GetNum())
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_DEPTH)
	{
		if(!m_pDate[uiVF].GetNum())
			m_pDate[uiVF].AddElement(pDate);
		else
			return 0;
	}
	else if(uiVF == SKVertexFormat::VF_TEXCOORD)
	{
		m_pDate[uiVF].AddElement(pDate);
	}
	else	
		return 0;

	if(!m_uiVertexNum)
		m_uiVertexNum = pDate->GetNum();
	else
	{
		if(m_uiVertexNum != pDate->GetNum())
			return 0;
	}
	m_uiOneVertexSize += pDate->GetStride();
	return 1;
}
bool SKVertexBuffer::GetVertexFormat(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &FormatArray)
{
	
	if (m_pVertexFormat)
	{
		FormatArray = m_pVertexFormat->m_FormatArray;

	}
	else
	{
		SKDataBuffer * pDate;
		SKVertexFormat::VERTEXFORMAT_TYPE Element;
		unsigned int iVertexSize = 0;
		for(unsigned int i = 0 ; i < GetPositionLevel(); i++)
		{
			pDate = GetPositionDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = SKVertexFormat::VF_POSITION;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}

		}

		for(unsigned int i = 0 ; i < GetTexCoordLevel(); i++)
		{
			pDate = GetTexCoordDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = SKVertexFormat::VF_TEXCOORD;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}
		}

		for(unsigned int i = 0 ; i < GetNormalLevel(); i++)
		{
			pDate = GetNormalDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = SKVertexFormat::VF_NORMAL;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}
		}

		pDate = GetTangentDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = SKVertexFormat::VF_TANGENT;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pDate = GetBinormalDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = SKVertexFormat::VF_BINORMAL;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pDate = GetPSizeDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = SKVertexFormat::VF_PSIZE;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		for(unsigned int i = 0 ; i < GetColorLevel(); i++)
		{

			pDate = GetColorDate(i);
			if(pDate)
			{
				Element.OffSet = (WORD)iVertexSize;
				iVertexSize += pDate->GetStride();
				Element.DateType = pDate->GetDT();
				Element.Semantics = SKVertexFormat::VF_COLOR;
				Element.SemanticsIndex = i;
				FormatArray.AddElement(Element);
			}
		}

		pDate = GetFogDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = SKVertexFormat::VF_FOG;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pDate = GetDepthDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = SKVertexFormat::VF_DEPTH;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pDate = GetBlendWeightDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = SKVertexFormat::VF_BLENDWEIGHT;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}

		pDate = GetBlendIndicesDate();
		if(pDate)
		{
			Element.OffSet = (WORD)iVertexSize;
			iVertexSize += pDate->GetStride();
			Element.DateType = pDate->GetDT();
			Element.Semantics = SKVertexFormat::VF_BLENDINDICES;
			Element.SemanticsIndex = 0;
			FormatArray.AddElement(Element);
		}
	}
	
	
	return (FormatArray.GetNum() > 0);

	
}
void SKVertexBuffer::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			for (unsigned int i = 0 ; i < SKVertexFormat::VF_MAX ; i++)
			{
				m_pDate[i].Clear();
			}
		}
	}
}
bool SKVertexBuffer::LoadResource(SKRenderer * pRender)
{
	if (m_uiMemType == MT_RAM)
	{
		return 1;
	}
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
		return 1;
	if(!m_pVertexFormat)
	{
		//
		SKResourceManager::LoadVertexFormat(this);
	}
	if(!m_pVertexFormat->LoadResource(pRender))
		return 0;
	

	if(!SKBind::LoadResource(pRender))
		return 0;

	return 1;

}

bool SKVertexBuffer::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadVBufferDate(this,pID))
		return 0;
	return 1;

}
bool SKVertexBuffer::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseVBufferDate(pID))
		return 0;
	return 1;
}
unsigned int SKVertexBuffer::GetByteSize()const
{
	return m_uiVertexNum * m_uiOneVertexSize;
}

void *SKVertexBuffer::Lock()
{
	if (m_pLockDate || m_bIsStatic)
	{
		return NULL;
	}
	if (m_pUser)
	{
		m_pLockDate = m_pUser->Lock(this);
	}
	
	return m_pLockDate;
}
void SKVertexBuffer::UnLock()
{
	if (!m_pLockDate || m_bIsStatic)
	{
		return;
	}
	if (m_pUser)
	{
		m_pUser->UnLock(this);
	}
	m_pLockDate = NULL;
}
SKVertexBuffer::SKVertexBuffer(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE>& FormatArray,unsigned int uiNum)
{
	SKMAC_ASSERT(FormatArray.GetNum() && uiNum);
	m_pVertexFormat = NULL;
	m_uiOneVertexSize = 0;
	m_uiVertexNum = uiNum;
	m_pLockDate = NULL;
	for (unsigned int i = 0 ; i < FormatArray.GetNum() ; i++)
	{
		m_uiOneVertexSize += SKDataBuffer::ms_uiDateTypeByte[FormatArray[i].DateType];
	}
	m_pVertexFormat = SKResourceManager::LoadVertexFormat(this,&FormatArray);
	SKMAC_ASSERT(m_pVertexFormat);
}
unsigned int SKVertexBuffer::GetSemanticsNum(unsigned int uiSemantics)const
{
	if (!m_pVertexFormat)
	{
		return m_pDate[uiSemantics].GetNum();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0 ; i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				uiNum++;
			}
		}
		return uiNum;
	}
	
}
unsigned int SKVertexBuffer::GetSemanticsChannel(unsigned int uiSemantics,unsigned int uiLevel)const
{
	if (!m_pVertexFormat)
	{
		if (uiLevel >= m_pDate[uiSemantics].GetNum())
		{
			return 0;
		}
		return m_pDate[uiSemantics][uiLevel]->GetChannel();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0 ; i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				if (uiLevel == uiNum)
				{
					return SKDataBuffer::ms_uiDateTypeChannel[m_pVertexFormat->m_FormatArray[i].DateType];
				}
				uiNum++;
			}
		}
		return 0;
	}
}
unsigned int SKVertexBuffer::GetSemanticsDateType(unsigned int uiSemantics,unsigned int uiLevel)const
{
	if (!m_pVertexFormat)
	{
		if (uiLevel >= m_pDate[uiSemantics].GetNum())
		{
			return 0;
		}
		return m_pDate[uiSemantics][uiLevel]->GetDT();
	}
	else
	{
		unsigned int uiNum = 0;
		for (unsigned int i = 0 ; i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if (m_pVertexFormat->m_FormatArray[i].Semantics == uiSemantics)
			{
				if (uiLevel == uiNum)
				{
					return m_pVertexFormat->m_FormatArray[i].DateType;
				}
				uiNum++;
			}
		}
		return 0;
	}
}	