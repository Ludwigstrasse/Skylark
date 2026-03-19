#include "SKLineSet.h"
#include "SKCreateClodFactory.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKLineSet,SKMeshDate)
BEGIN_ADD_PROPERTY(SKLineSet,SKMeshDate)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKLineSet)
IMPLEMENT_INITIAL_END
SKLineSet::SKLineSet()
{

}
SKLineSet::~SKLineSet()
{

}
bool SKLineSet::CreateIndex(unsigned int uiLineType)
{
	if(m_pVertexBuffer == NULL)
		return 0;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 2)
		return 0;
	if(uiLineType == LT_OPEN)
	{
		Num = (Num - 1) * 2;

	}
	else if(uiLineType == LT_CLOSE)
	{
		Num = Num * 2;
	}
	else if(uiLineType == LT_SEGMENT)
	{
		if(Num % 2)
		{
			return 0;
		}

	}
	else
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

	if(uiLineType == LT_OPEN)
	{
		for(unsigned int i = 0 ; i < Num / 2 ; i++)
		{
			pIndexBuffer[2 * i] = i;
			pIndexBuffer[2 * i + 1] = i + 1;
		}
	}
	else if(uiLineType == LT_CLOSE)
	{	
		unsigned int i;
		for( i = 0; i < Num / 2 ; i++)
		{
			pIndexBuffer[2 * i] = i;
			pIndexBuffer[2 * i + 1] = i + 1;
		}
		//iѾһӦ0޸ 2 * ( i - 1) + 1 =  2 * i - 1
		pIndexBuffer[2 * i - 1] = 0;
	}
	else if(uiLineType == LT_SEGMENT)
	{
		for(unsigned int i = 0 ; i < Num ; i++)
		{
			pIndexBuffer[i] = i;

		}
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
unsigned int SKLineSet::GetTotleNum()const
{	
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 2;
	}
	else
		return m_pIndexBuffer->GetNum() / 2;
}
unsigned int SKLineSet::GetGirdNum(unsigned int uiInputNum)const
{
	return uiInputNum / 2;
}
bool SKLineSet::CreateCollapseRecord()
{
	if (!m_pIndexBuffer)
	{
		return false;
	}
	m_pCollapseRecord = NULL;
	m_pCollapseRecord = SKCreateClodLineFactory::CreateClodLine(this);
	if (m_pCollapseRecord)
	{
		m_pIndexBuffer->SetStatic(false);
		return 1;
	}
	else
		return 0;
}
void SKLineSet::UpDateClodMesh()
{
	if (!m_pCollapseRecord)
	{
		return;
	}
	if (!m_pIndexBuffer)
	{
		return ;
	}
	
	SKCollapseRecord * pCollapseRecord = m_pCollapseRecord;
	if (pCollapseRecord->m_uiCurRecordID == pCollapseRecord->DesireRecordID())
	{
		return;
	}
	if (!m_pIndexBuffer->GetIdentifier())
	{
		return;
	}
	SKUSHORT_INDEX *pIndex = (SKUSHORT_INDEX *)m_pIndexBuffer->Lock();

	if (!pIndex)
	{
		return;
	}

	while(pCollapseRecord->m_uiCurRecordID < pCollapseRecord->DesireRecordID())
	{
		pCollapseRecord->m_uiCurRecordID++;
		const RecordLineInfo& Record = ((SKCollapseRecordLine *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];	
		pIndex[Record.m_MapIndex] = Record.m_uiKeep;		

	}
	while(pCollapseRecord->m_uiCurRecordID > pCollapseRecord->DesireRecordID())
	{

		const RecordLineInfo& Record = ((SKCollapseRecordLine *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		pIndex[Record.m_MapIndex] = Record.m_uiThrow;
		pCollapseRecord->m_uiCurRecordID--;
	}
	m_pIndexBuffer->UnLock();
	//pCollapseRecord->m_uiCurRecordID = pCollapseRecord->DesireRecordID();
}
unsigned int SKLineSet::GetActiveNum()const
{
	if (m_pCollapseRecord)
	{
		SKCollapseRecord * pCollapseRecord = m_pCollapseRecord;
		const RecordLineInfo& Record = ((SKCollapseRecordLine *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		return Record.m_uiLeftEdgeNum;
	}
	else
		return GetTotleNum();
}