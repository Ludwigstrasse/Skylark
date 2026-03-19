#include "SKUseBuffer.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;

SKUseBuffer:: ~SKUseBuffer()
{
	SKMAC_DELETEA(m_pBuffer);
	m_BufferElemetnArray.Clear();
}
SKUseBuffer::SKUseBuffer(bool IsStatic,unsigned int uiBufferSize)
{
	m_bIsStatic = IsStatic;
	SKMAC_ASSERT(uiBufferSize);
	m_uiBufferSize = uiBufferSize;
	m_uiCurSize = 0;
	m_pBuffer = NULL;
	m_pBuffer = SK_NEW unsigned char[uiBufferSize];
	SKMAC_ASSERT(m_pBuffer);
	m_BufferElemetnArray.Clear();
	m_uiOneAddLimitSize = 1024;
}
void * SKUseBuffer::NewGet(unsigned int uiDateSize)
{
	if (!uiDateSize || uiDateSize > m_uiOneAddLimitSize)
	{
		return NULL;
	}
	unsigned int uiAddSize = 0;
	unsigned int uiCount = 0;

	bool IsNeedReAlloc = false;
	while(uiDateSize > m_uiBufferSize - m_uiCurSize + uiAddSize)
	{
		if (!m_bIsStatic)
		{
			uiCount++;
			uiAddSize += m_uiBufferSize * uiCount;		
			IsNeedReAlloc = true;
		}
		else
		{
			return NULL;
		}


	}

	if (IsNeedReAlloc)
	{
		unsigned char * pBuffer = NULL;
		pBuffer = SK_NEW unsigned char[m_uiBufferSize + uiAddSize];
		if(!pBuffer)
			return false;
		SKMemcpy(pBuffer,m_pBuffer,m_uiCurSize);

		SKMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;

		m_uiBufferSize += uiAddSize;
	}

	SKMemset(m_pBuffer + m_uiCurSize , 0 , uiDateSize);
	SKBufferElementInfo Element;
	Element.m_uiStart = m_uiCurSize;
	m_uiCurSize += uiDateSize;
	Element.m_uiEnd = m_uiCurSize;

	m_BufferElemetnArray.AddElement(Element);
	return (void *)(m_pBuffer + Element.m_uiStart);
}
bool SKUseBuffer::Add(const void * pDate,unsigned int uiDateSize)
{
	if (!pDate || !uiDateSize || uiDateSize > m_uiOneAddLimitSize)
	{
		return false;
	}
	unsigned int uiAddSize = 0;
	unsigned int uiCount = 0;

	bool IsNeedReAlloc = false;
	while(uiDateSize > m_uiBufferSize - m_uiCurSize + uiAddSize)
	{
		if (!m_bIsStatic)
		{
			uiCount++;
			uiAddSize += m_uiBufferSize * uiCount;		
			IsNeedReAlloc = true;
		}
		else
		{
			return true;
		}
	
	
	}
	
	if (IsNeedReAlloc)
	{
		unsigned char * pBuffer = NULL;
		pBuffer = SK_NEW unsigned char[m_uiBufferSize + uiAddSize];
		if(!pBuffer)
			return false;
		SKMemcpy(pBuffer,m_pBuffer,m_uiCurSize);

		SKMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;

		m_uiBufferSize += uiAddSize;
	}
	

	SKMemcpy(m_pBuffer + m_uiCurSize , pDate , uiDateSize);
	SKBufferElementInfo Element;
	Element.m_uiStart = m_uiCurSize;
	m_uiCurSize += uiDateSize;
	Element.m_uiEnd = m_uiCurSize;

	m_BufferElemetnArray.AddElement(Element);
	return true;
}

void SKUseBuffer::Clear()
{
	m_BufferElemetnArray.Clear();
	m_uiCurSize = 0;
}
unsigned int SKUseBuffer::GetSizeByElementIndex(unsigned int i0,unsigned int i1)
{
	if (i1 < i0)
	{
		return 0;
	}
	if (i1 >= m_BufferElemetnArray.GetNum())
	{
		return 0;
	}
	unsigned int uiSize = 0;
	for (unsigned int i = i0 ; i = i1 ; i++)
	{
		uiSize += m_BufferElemetnArray[i].GetSize();
	}
	return uiSize;
}
unsigned int SKUseBuffer::GetSizeByElementIndex(unsigned int i)
{
	return GetSizeByElementIndex(i,i);
}
SKUseBuffer::SKBufferElementInfo ElementNULL;