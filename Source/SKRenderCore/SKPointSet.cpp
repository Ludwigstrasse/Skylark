#include "SKPointSet.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPointSet,SKMeshDate)
BEGIN_ADD_PROPERTY(SKPointSet,SKMeshDate)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPointSet)
IMPLEMENT_INITIAL_END
SKPointSet::SKPointSet()
{

}
SKPointSet::~SKPointSet()
{

}
bool SKPointSet::CreateIndex()
{
	if(m_pVertexBuffer == NULL)
		return 0;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 1)
		return 0;
	SKDataBuffer * pIndex = NULL;
	pIndex = new SKDataBuffer();
	if(!pIndex)
		return 0;
	if(!pIndex->CreateEmptyBuffer(Num,SKDataBuffer::DT_USHORT))
		return 0;

	SKUSHORT_INDEX * pIndexBuffer = (SKUSHORT_INDEX *)pIndex->GetDate();
	if(!pIndexBuffer)
		return 0;
	for(unsigned int i = 0 ; i < Num ; i++)
	{
		pIndexBuffer[i] = i;
	}

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();
		if (!m_pIndexBuffer)
		{
			return 0;
		}
	}

	m_pIndexBuffer->SetDate(pIndex);
	return 1;
}
unsigned int SKPointSet::GetTotleNum()const
{
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum();
	}
	else
	{
		return m_pIndexBuffer->GetNum();
	}
}
unsigned int SKPointSet::GetGirdNum(unsigned int uiInputNum)const
{
	return uiInputNum;
}
SKPointSet::SKPointSet(const SKVector3 & Point,SKREAL fSize)
{
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;

	pVertex->SetDate(&Point, 1,SKDataBuffer::DT_FLOAT32_3);
	
	SKDataBuffer *PSize = SK_NEW SKDataBuffer;
	PSize->SetDate(&fSize,1,SKDataBuffer::DT_FLOAT32_1);

	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);
	m_pVertexBuffer->SetDate(PSize,SKVertexFormat::VF_PSIZE);

	bool bResult = CreateIndex();
	SKMAC_ASSERT(bResult);
}