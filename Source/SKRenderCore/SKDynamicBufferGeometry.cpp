#include "SKDynamicBufferGeometry.h"
#include "SKPointSet.h"
#include "SKLineSet.h"
#include "SKTriangleSet.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDynamicBufferGeometry,SKGeometry)
SKDynamicBufferGeometry::SKDynamicBufferGeometry()
{

}
SKDynamicBufferGeometry::~SKDynamicBufferGeometry()
{

}
void SKDynamicBufferGeometry::Draw(SKCamera * pCamera)
{
	if (HaveDate())
	{
		SKMaterialInstance *pMaterialInstance = GetUseMaterialInstance();
		SKMaterial *pMaterial = pMaterialInstance->GetMaterial();
		if(!pMaterialInstance || !pMaterial)
			return;
		for (unsigned int i = 0 ; i < pMaterial->GetShaderMainFunctionNum() ;i++)
		{

			SKIndirectRenderPass * pBasePass = pMaterialInstance->GetMaterial()->GetIndirectRenderPass();
			pBasePass->SetPassId(i);
			pBasePass->SetSpatial(this);
			pBasePass->SetMaterialInstance(pMaterialInstance);
			pBasePass->SetCamera(pCamera);
			pBasePass->Draw(SKRenderer::ms_pRenderer);
		}
	}
}
IMPLEMENT_RTTI(SKDVGeometry,SKDynamicBufferGeometry)
SKDVGeometry::SKDVGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_pVertexUseBufferRender = NULL;
}
SKDVGeometry::~SKDVGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pVertexUseBufferRender = NULL;
	
}
void SKDVGeometry::Create(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
						   unsigned int uiMeshDateType,
						   unsigned int uiVertexNum)
{
	SKMAC_ASSERT(uiMeshDateType < SKMeshDate::MDT_MAX && ForamtArray.GetNum() && uiVertexNum);

	SKMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == SKMeshDate::MDT_POINT)
	{
		pMeshDate = SK_NEW SKPointSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_LINE)
	{
		pMeshDate = SK_NEW SKLineSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = SK_NEW SKTriangleSet();
	}

	SKMAC_ASSERT(pMeshDate);


	SKVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = SK_NEW SKVertexBuffer(ForamtArray,uiVertexNum);
	SKMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(SKBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(SKBind::LF_DISCARD);
	pMeshDate->SetVertexBuffer(pVertexBuffer);

	SetMeshDate(pMeshDate);
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;

	m_pVertexUseBuffer = SK_NEW SKUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	if (SKResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = SK_NEW SKUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
	}
}


bool SKDVGeometry::Add(const void * pVeretexDate,unsigned int uiVertexSize)
{
	if (!pVeretexDate || !uiVertexSize)
	{
		return 0;
	}

	
	if(m_pVertexUseBuffer->Add(pVeretexDate,uiVertexSize) == false)
		return false;
	return 1;
}
void * SKDVGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
unsigned int SKDVGeometry::UpdateGeometry()
{

	SKVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
	if (!pVertexBuffer || !m_pVertexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const SKUseBuffer::SKBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned char * pVertexDate = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexDate)
	{
		return UGRI_FAIL;
	}

	SKMemcpy(pVertexDate,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	if (!uiActiveNum)
	{
		uiActiveNum = m_pMeshDate->GetGirdNum(uiVSize / pVertexBuffer->GetOneVertexSize());
	}
	
	SetActiveNum(uiActiveNum);

	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;
	
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void SKDVGeometry::ClearInfo()
{
	if (SKResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
	}
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
}
unsigned int SKDVGeometry::GetMaxRenderVertexNum()const
{
	return m_pMeshDate->GetVertexBuffer()->GetVertexNum();
}

IMPLEMENT_RTTI(SKDVDIGeometry,SKDynamicBufferGeometry)
SKDVDIGeometry::SKDVDIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;

	m_pVertexUseBufferRender = NULL;
	m_pIndexUseBufferRender = NULL;
}
SKDVDIGeometry::~SKDVDIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;

	m_pVertexUseBufferRender = NULL;
	m_pIndexUseBufferRender = NULL;
}
void SKDVDIGeometry::Create(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
												 unsigned int uiMeshDateType,
												 unsigned int uiVertexNum,
												 unsigned int uiIndexNum)
{
	SKMAC_ASSERT(uiMeshDateType < SKMeshDate::MDT_MAX && ForamtArray.GetNum() && uiVertexNum && uiIndexNum);

	SKMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == SKMeshDate::MDT_POINT)
	{
		pMeshDate = SK_NEW SKPointSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_LINE)
	{
		pMeshDate = SK_NEW SKLineSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = SK_NEW SKTriangleSet();
	}

	SKMAC_ASSERT(pMeshDate);


	SKVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = SK_NEW SKVertexBuffer(ForamtArray,uiVertexNum);
	SKMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(SKBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(SKBind::LF_DISCARD);
	pMeshDate->SetVertexBuffer(pVertexBuffer);

	SKIndexBuffer * pIndexBuffer = SK_NEW SKIndexBuffer(uiIndexNum);
	pIndexBuffer->SetStatic(false);
	pIndexBuffer->SetMemType(SKBind::MT_VRAM);
	pIndexBuffer->SetLockFlag(SKBind::LF_DISCARD);
	pMeshDate->SetIndexBuffer(pIndexBuffer);

	SetMeshDate(pMeshDate);
	m_pVertexUseBuffer = NULL;
	m_pIndexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;


	m_pVertexUseBuffer = SK_NEW SKUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());

	m_pIndexUseBuffer = SK_NEW SKUseBuffer(false);
	m_pIndexUseBuffer->SetOneAddLimitSize(pIndexBuffer->GetByteSize());

	if (SKResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = SK_NEW SKUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());

		m_pIndexUseBufferRender = SK_NEW SKUseBuffer(false);
		m_pIndexUseBufferRender->SetOneAddLimitSize(pIndexBuffer->GetByteSize());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
		m_pIndexUseBufferRender = m_pIndexUseBuffer;
	}
}

void * SKDVDIGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
void * SKDVDIGeometry::NewGetI(unsigned int uiIndexSize)
{
	if (!uiIndexSize)
	{
		return 0;
	}

	return m_pIndexUseBuffer->NewGet(uiIndexSize);
}
bool SKDVDIGeometry::Add(const void * pVeretexDate,unsigned int uiVertexSize,
								  const void * pIndexDate,unsigned int uiIndexSize)
{
	SKMAC_ASSERT(pVeretexDate && uiVertexSize && pIndexDate && uiIndexSize);
	if (!pVeretexDate || !uiVertexSize || !pIndexDate || !uiIndexSize)
	{
		return 0;
	}

	if(m_pVertexUseBuffer->Add(pVeretexDate,uiVertexSize) == false)
		return false;

	
		

	if(m_pIndexUseBuffer->Add(pIndexDate,uiIndexSize) == false)
		return false;

	unsigned int uiLastVertexNum = 0;
	unsigned int uiVElementNum = m_pVertexUseBuffer->GetElementNum();
	if (uiVElementNum)
	{
		SKVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
		if (pVertexBuffer)
		{
			return false;
		}
		const SKUseBuffer::SKBufferElementInfo * pElementInfo = m_pVertexUseBuffer->GetElementInfo(uiVElementNum - 1);
		uiLastVertexNum = pElementInfo->m_uiStart / pVertexBuffer->GetOneVertexSize();
	}
	unsigned char *pBuffer = m_pIndexUseBuffer->GetBuffer();
	unsigned int uiIElementNum = m_pIndexUseBuffer->GetElementNum();
	pBuffer += m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiStart;

	SKUSHORT_INDEX * pIndexDateBuffer = (SKUSHORT_INDEX *)pBuffer;
	for ( ; (unsigned int)pIndexDateBuffer < (unsigned int)m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiEnd ; pIndexDateBuffer++)
	{
		*pIndexDateBuffer += uiLastVertexNum;
	}
	if ((unsigned int)pIndexDateBuffer != (unsigned int)m_pIndexUseBuffer->GetElementInfo(uiIElementNum - 1)->m_uiEnd)
	{
		return false;
	}
	
	SKMAC_ASSERT(m_pVertexUseBuffer->GetElementNum() == m_pIndexUseBuffer->GetElementNum());
	return 1;
}
unsigned int SKDVDIGeometry::UpdateGeometry()
{
	SKMAC_ASSERT(m_pMeshDate && m_pVertexUseBufferRender && m_pIndexUseBufferRender);
	if (!m_pMeshDate || !m_pVertexUseBufferRender || !m_pIndexUseBufferRender)
	{
		return UGRI_FAIL;
	}


	SKVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
	SKIndexBuffer * pIndexBuffer = m_pMeshDate->GetIndexBuffer();
	if (!pVertexBuffer || !pIndexBuffer || !m_pVertexUseBufferRender->GetElementNum() || !m_pIndexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const SKUseBuffer::SKBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}




	unsigned int uiIElementEndIndex = m_uiCurIUseBufferElementIndex;
	unsigned int uiISize = 0;
	for (unsigned int i = m_uiCurIUseBufferElementIndex ; i < m_pIndexUseBufferRender->GetElementNum() ; i++)
	{
		const SKUseBuffer::SKBufferElementInfo * pIElement = m_pIndexUseBufferRender->GetElementInfo(i);
		if (pIElement)
		{

			if (uiISize + pIElement->GetSize() > pIndexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiIElementEndIndex = i;
				uiISize += pIElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned int uiVCount = uiVElementEndIndex - m_uiCurVUseBufferElementIndex;
	unsigned int uiICount = uiIElementEndIndex - m_uiCurIUseBufferElementIndex;
	if ( uiVCount > uiICount)
	{
		uiVSize = 0;
		uiVElementEndIndex = m_uiCurVUseBufferElementIndex + uiICount;
		for (unsigned int i = m_uiCurVUseBufferElementIndex ; i <= uiVElementEndIndex ; i++)
		{
			const SKUseBuffer::SKBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
			if (pVElement)
			{
				uiVSize += pVElement->GetSize();
			}
			else
			{
				return UGRI_FAIL;
			}
		}
	}
	else if(uiVCount < uiICount)
	{
		uiISize = 0;
		uiIElementEndIndex = m_uiCurIUseBufferElementIndex + uiVCount;
		for (unsigned int i = m_uiCurIUseBufferElementIndex ; i <= uiIElementEndIndex ; i++)
		{
			const SKUseBuffer::SKBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
			if (pVElement)
			{
				uiISize += pVElement->GetSize();
			}
			else
			{
				return UGRI_FAIL;
			}
		}
	}

	unsigned char * pIndexDate = (unsigned char *)pIndexBuffer->Lock();
	if (!pIndexDate)
	{
		return UGRI_FAIL;
	}
	SKMemcpy(pIndexDate,m_pIndexUseBufferRender->GetBuffer() + m_pIndexUseBufferRender->GetElementInfo(m_uiCurIUseBufferElementIndex)->m_uiStart,uiISize);
	pIndexBuffer->UnLock();
	uiActiveNum = m_pMeshDate->GetGirdNum(uiISize / sizeof(SKUSHORT_INDEX));

	m_uiCurIUseBufferElementIndex = uiIElementEndIndex + 1;

	
	unsigned char * pVertexDate = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexDate)
	{
		return UGRI_FAIL;
	}

	SKMemcpy(pVertexDate,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	if (!uiActiveNum)
	{
		uiActiveNum = m_pMeshDate->GetGirdNum(uiVSize / pVertexBuffer->GetOneVertexSize());
	}

	SetActiveNum(uiActiveNum);

	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;

	//ΪUseVBuffer  UseIBuffer Elementһ,ȾĸҲ֤һֻжһ
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		m_uiCurIUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void SKDVDIGeometry::ClearInfo()
{
	if (SKResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
		Swap(m_pIndexUseBuffer,m_pIndexUseBufferRender);
	}
	m_pIndexUseBuffer->Clear();
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
	m_uiCurIUseBufferElementIndex = 0;
}


IMPLEMENT_RTTI(SKSVDIGeometry,SKDynamicBufferGeometry)
SKSVDIGeometry::SKSVDIGeometry()
{

	m_pIndexUseBuffer = NULL;

	m_uiCurIUseBufferElementIndex = 0;

	m_pIndexUseBufferRender = NULL;
}
SKSVDIGeometry::~SKSVDIGeometry()
{

	m_pIndexUseBuffer = NULL;

	m_pIndexUseBufferRender = NULL;
}
void SKSVDIGeometry::Create(unsigned int uiMeshDateType,SKVertexBuffer * pVertexBuffer,unsigned int uiIndexNum)
{
	SKMAC_ASSERT(uiMeshDateType < SKMeshDate::MDT_MAX && uiIndexNum && pVertexBuffer && pVertexBuffer->IsStatic());
	SKMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == SKMeshDate::MDT_POINT)
	{
		pMeshDate = SK_NEW SKPointSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_LINE)
	{
		pMeshDate = SK_NEW SKLineSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = SK_NEW SKTriangleSet();
	}

	SKMAC_ASSERT(pMeshDate);

	pMeshDate->SetVertexBuffer(pVertexBuffer);

	SKIndexBuffer * pIndexBuffer = SK_NEW SKIndexBuffer(uiIndexNum);
	pIndexBuffer->SetStatic(false);
	pIndexBuffer->SetMemType(SKBind::MT_VRAM);
	pIndexBuffer->SetLockFlag(SKBind::LF_DISCARD);
	pMeshDate->SetIndexBuffer(pIndexBuffer);

	SetMeshDate(pMeshDate);

	m_pIndexUseBuffer = NULL;

	m_uiCurIUseBufferElementIndex = 0;


	m_pIndexUseBuffer = SK_NEW SKUseBuffer(false);
	m_pIndexUseBuffer->SetOneAddLimitSize(pIndexBuffer->GetByteSize());

	if (SKResourceManager::ms_bRenderThread)
	{

		m_pIndexUseBufferRender = SK_NEW SKUseBuffer(false);
		m_pIndexUseBufferRender->SetOneAddLimitSize(pIndexBuffer->GetByteSize());
	}
	else
	{
		m_pIndexUseBufferRender = m_pIndexUseBuffer;
	}
}

void * SKSVDIGeometry::NewGetI(unsigned int uiIndexSize)
{
	if (!uiIndexSize)
	{
		return 0;
	}

	return m_pIndexUseBuffer->NewGet(uiIndexSize);
}
bool SKSVDIGeometry::Add(const void * pIndexDate,unsigned int uiIndexSize)
{
	SKMAC_ASSERT(pIndexDate && uiIndexSize);
	if (!pIndexDate || !uiIndexSize)
	{
		return 0;
	}



	if(m_pIndexUseBuffer->Add(pIndexDate,uiIndexSize) == false)
		return false;

	return 1;
}
unsigned int SKSVDIGeometry::UpdateGeometry()
{
	SKMAC_ASSERT(m_pMeshDate && m_pIndexUseBufferRender);
	if (!m_pMeshDate || !m_pIndexUseBufferRender)
	{
		return UGRI_FAIL;
	}


	SKIndexBuffer * pIndexBuffer = m_pMeshDate->GetIndexBuffer();
	if (!pIndexBuffer || !m_pIndexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiActiveNum = 0;
	




	unsigned int uiIElementEndIndex = m_uiCurIUseBufferElementIndex;
	unsigned int uiISize = 0;
	for (unsigned int i = m_uiCurIUseBufferElementIndex ; i < m_pIndexUseBufferRender->GetElementNum() ; i++)
	{
		const SKUseBuffer::SKBufferElementInfo * pIElement = m_pIndexUseBufferRender->GetElementInfo(i);
		if (pIElement)
		{

			if (uiISize + pIElement->GetSize() > pIndexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiIElementEndIndex = i;
				uiISize += pIElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}


	unsigned char * pIndexDate = (unsigned char *)pIndexBuffer->Lock();
	if (!pIndexDate)
	{
		return UGRI_FAIL;
	}
	SKMemcpy(pIndexDate,m_pIndexUseBufferRender->GetBuffer() + m_pIndexUseBufferRender->GetElementInfo(m_uiCurIUseBufferElementIndex)->m_uiStart,uiISize);
	pIndexBuffer->UnLock();
	uiActiveNum = m_pMeshDate->GetGirdNum(uiISize / sizeof(SKUSHORT_INDEX));

	m_uiCurIUseBufferElementIndex = uiIElementEndIndex + 1;

	SetActiveNum(uiActiveNum);


	//ΪUseVBuffer  UseIBuffer ElementһÿȾĸҲ֤һֻжһ
	if (m_uiCurIUseBufferElementIndex >= m_pIndexUseBufferRender->GetElementNum())
	{
		m_uiCurIUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void SKSVDIGeometry::ClearInfo()
{
	if (SKResourceManager::ms_bRenderThread)
	{

		Swap(m_pIndexUseBuffer,m_pIndexUseBufferRender);
	}
	m_pIndexUseBuffer->Clear();

	m_uiCurIUseBufferElementIndex = 0;
}


IMPLEMENT_RTTI(SKDVSIGeometry,SKDynamicBufferGeometry)
SKDVSIGeometry::SKDVSIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;
	m_pVertexUseBufferRender = NULL;
}
SKDVSIGeometry::~SKDVSIGeometry()
{
	m_pVertexUseBuffer = NULL;
	m_pVertexUseBufferRender = NULL;

}
void SKDVSIGeometry::Create(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
							unsigned int uiMeshDateType,
							unsigned int uiVertexNum,
							SKIndexBuffer * pIndexBuffer)
{
	SKMAC_ASSERT(uiMeshDateType < SKMeshDate::MDT_MAX && ForamtArray.GetNum() && uiVertexNum && pIndexBuffer);
	SKMAC_ASSERT(pIndexBuffer->IsStatic());
	SKMeshDate * pMeshDate = NULL;
	if (uiMeshDateType == SKMeshDate::MDT_POINT)
	{
		pMeshDate = SK_NEW SKPointSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_LINE)
	{
		pMeshDate = SK_NEW SKLineSet();
	}
	else if ( uiMeshDateType == SKMeshDate::MDT_TRIANGLE)
	{
		pMeshDate = SK_NEW SKTriangleSet();
	}

	SKMAC_ASSERT(pMeshDate);


	SKVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = SK_NEW SKVertexBuffer(ForamtArray,uiVertexNum);
	SKMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetStatic(false);
	pVertexBuffer->SetMemType(SKBind::MT_VRAM);
	pVertexBuffer->SetLockFlag(SKBind::LF_DISCARD);
	pMeshDate->SetVertexBuffer(pVertexBuffer);
	pMeshDate->SetIndexBuffer(pIndexBuffer);
	SetMeshDate(pMeshDate);
	m_pVertexUseBuffer = NULL;
	m_uiCurVUseBufferElementIndex = 0;

	m_pVertexUseBuffer = SK_NEW SKUseBuffer(false);
	m_pVertexUseBuffer->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	if (SKResourceManager::ms_bRenderThread)
	{
		m_pVertexUseBufferRender = SK_NEW SKUseBuffer(false);
		m_pVertexUseBufferRender->SetOneAddLimitSize(pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum());
	}
	else
	{
		m_pVertexUseBufferRender = m_pVertexUseBuffer;
	}
}
bool SKDVSIGeometry::Add(const void * pVeretexDate,unsigned int uiVertexSize)
{
	if (!pVeretexDate || !uiVertexSize)
	{
		return 0;
	}


	if(m_pVertexUseBuffer->Add(pVeretexDate,uiVertexSize) == false)
		return false;
	return 1;
}
void * SKDVSIGeometry::NewGetV(unsigned int uiVertexSize)
{
	if (!uiVertexSize)
	{
		return 0;
	}

	return m_pVertexUseBuffer->NewGet(uiVertexSize);
}
unsigned int SKDVSIGeometry::UpdateGeometry()
{

	SKVertexBuffer * pVertexBuffer = m_pMeshDate->GetVertexBuffer();
	if (!pVertexBuffer || !m_pVertexUseBufferRender->GetElementNum())
	{
		return UGRI_FAIL;
	}
	unsigned int uiVElementEndIndex = m_uiCurVUseBufferElementIndex;
	unsigned int uiVSize = 0;
	for (unsigned int i = m_uiCurVUseBufferElementIndex ; i < m_pVertexUseBufferRender->GetElementNum() ; i++)
	{
		const SKUseBuffer::SKBufferElementInfo * pVElement = m_pVertexUseBufferRender->GetElementInfo(i);
		if (pVElement)
		{

			if (uiVSize + pVElement->GetSize() > pVertexBuffer->GetByteSize())
			{
				break;
			}
			else
			{
				uiVElementEndIndex = i;
				uiVSize += pVElement->GetSize();
			}
		}
		else
		{
			return UGRI_FAIL;
		}
	}

	unsigned char * pVertexDate = (unsigned char *)pVertexBuffer->Lock();

	if (!pVertexDate)
	{
		return UGRI_FAIL;
	}

	SKMemcpy(pVertexDate,m_pVertexUseBufferRender->GetBuffer() + m_pVertexUseBufferRender->GetElementInfo(m_uiCurVUseBufferElementIndex)->m_uiStart,uiVSize);
	pVertexBuffer->UnLock();
	m_uiCurVUseBufferElementIndex = uiVElementEndIndex + 1;

	//ΪUseVBuffer  UseIBuffer ElementһÿȾĸҲ֤һֻжһ
	if (m_uiCurVUseBufferElementIndex >= m_pVertexUseBufferRender->GetElementNum())
	{
		m_uiCurVUseBufferElementIndex = 0;
		return UGRI_END;
	}
	else
	{
		return UGRI_CONTINUME;
	}
	return UGRI_END;
}
void SKDVSIGeometry::ClearInfo()
{
	if (SKResourceManager::ms_bRenderThread)
	{
		Swap(m_pVertexUseBuffer,m_pVertexUseBufferRender);
	}
	m_pVertexUseBuffer->Clear();
	m_uiCurVUseBufferElementIndex = 0;
}
