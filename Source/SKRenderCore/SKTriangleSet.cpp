#include "SKTriangleSet.h"
#include "SKNode.h"
#include "SKCreateClodFactory.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKTriangleSet,SKMeshDate)
BEGIN_ADD_PROPERTY(SKTriangleSet,SKMeshDate)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKTriangleSet)
IMPLEMENT_INITIAL_END
SKTriangleSet::SKTriangleSet()
{

}
SKTriangleSet::~SKTriangleSet()
{

}
bool SKTriangleSet::CreateFanIndex()
{
	if(m_pVertexBuffer == NULL)
		return 0;
	unsigned int Num = m_pVertexBuffer->GetPositionDate(0)->GetNum();
	if(Num < 3)
		return 0;
	Num = (Num - 1) * 3;
	SKDataBuffer * pIndex = NULL;
	pIndex = new SKDataBuffer();
	if(!pIndex)
		return 0;
	if(!pIndex->CreateEmptyBuffer(Num,SKDataBuffer::DT_USHORT))
		return 0;

	SKUSHORT_INDEX * pIndexBuffer = (SKUSHORT_INDEX *)pIndex->GetDate();
	if(!pIndexBuffer)
		return 0;
	for(unsigned int i = 0 ; i < Num / 3; i++)
	{
		pIndexBuffer[i * 3] = 0;
		pIndexBuffer[i * 3 + 1] = i + 1;
		pIndexBuffer[i * 3 + 2] = i + 2;
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
bool SKTriangleSet::CreateNormal(unsigned int uiVertexLevel)
{
	if(m_pVertexBuffer == NULL)
		return 0;
	if(m_pVertexBuffer->GetNormalDate(uiVertexLevel))
		return 1;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 3)
		return 0;

	if(m_pIndexBuffer == NULL)
		return 0;

	//Noraml
	SKDataBuffer * pNormal= NULL;
	pNormal = new SKDataBuffer();
	if(!pNormal)
		return 0;
	if(!pNormal->CreateEmptyBuffer(Num,SKDataBuffer::DT_FLOAT32_3))
		return 0;

	SKVector3 * pNormalBuffer = (SKVector3 *)pNormal->GetDate();

	//
	unsigned int * pVertexTemp = SK_NEW unsigned int[Num];
	SKMemset((void *)pVertexTemp, 0, sizeof(unsigned int) * Num);


	unsigned int TriangleNum = GetTotleNum();

	SKDataBuffer * pIndexDate = m_pIndexBuffer->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}
	// ȡָ
	SKUSHORT_INDEX * pIndexBuffer = (SKUSHORT_INDEX *)pIndexDate->GetDate();
	if(!pIndexBuffer)
		return 0;
	
	SKDataBuffer * pDate = m_pVertexBuffer->GetPositionDate(uiVertexLevel);
	if(!pDate)
		return 0;
	if(pDate->GetChannel() != 3)
		return 0;
	//ȡָ
	SKVector3 * pVertexBuffer = (SKVector3 *)pDate->GetDate();
	if(!pVertexBuffer)
		return 0;
	
	//
	for (unsigned int poly = 0 ; poly < TriangleNum; poly++)
	{

		SKUSHORT_INDEX i0 = pIndexBuffer[poly*3+0];
		SKUSHORT_INDEX i1 = pIndexBuffer[poly*3+1];
		SKUSHORT_INDEX i2 = pIndexBuffer[poly*3+2];

		SKVector3 u = pVertexBuffer[i1] - pVertexBuffer[i0];
		SKVector3 v = pVertexBuffer[i2] - pVertexBuffer[i0];

		SKVector3 n;
		n.Cross(u,v);

		pVertexTemp[i0]++;
		pVertexTemp[i1]++;
		pVertexTemp[i2]++;

		pNormalBuffer[i0] += n;
		pNormalBuffer[i1] += n;
		pNormalBuffer[i2] += n;



	} // end for poly


	for (unsigned int vertex = 0; vertex < Num; vertex++)
	{


		if (pVertexTemp[vertex] > 1)
		{
			pNormalBuffer[vertex] /= (SKREAL)1.0 * pVertexTemp[vertex];
		
			
		} // end if
		pNormalBuffer[vertex].Normalize();
	} // end for
	SKMAC_DELETEA(pVertexTemp);

	m_pVertexBuffer->SetDate(pNormal,SKVertexFormat::VF_NORMAL);
	return 1;
}
/*
	(P0 - P1) = (u0 - u1) U + (v0 - v1) V
	(P0 - P2) = (u0 - u2) U + (v0 - v2) V

	(v0 - v2)(P0 - P1) = (v0 - v2)(u0 - u1) U + (v0 - v2)(v0 - v1) V
	(v0 - v1)(P0 - P2) = (v0 - v1)(u0 - u2) U + (v0 - v2)(v0 - v1) V

	U = ((v0 - v2)(P0 - P1) - (v0 - v1)(P0 - P2)) / ((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))

	(u0 - u2)(P0 - P1) = (u0 - u2)(u0 - u1) U + (u0 - u2)(v0 - v1) V
	(u0 - u1)(P0 - P2) = (u0 - u2)(u0 - u1) U + (v0 - v2)(u0 - u1) V

	V = ((u0 - u1)(P0 - P2) - (u0 - u2)(P0 - P1))/((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
*/
bool SKTriangleSet::CreateTangent(unsigned int uiTexCoordLevel)
{
	if(!m_pVertexBuffer || !m_pVertexBuffer->GetTexCoordDate(uiTexCoordLevel) || !m_pIndexBuffer)
		return 0;
	if(!m_pVertexBuffer->GetPositionDate(0))
		return 0;
	if(m_pVertexBuffer->GetTangentDate() && m_pVertexBuffer->GetBinormalDate())
		return 1;
	else if(m_pVertexBuffer->GetTangentDate() || m_pVertexBuffer->GetBinormalDate())
		return 0;
	unsigned int Num = m_pVertexBuffer->GetVertexNum();
	if(Num < 3)
		return 0;

	if(!m_pVertexBuffer->GetNormalDate(0))
	{
		if(!CreateNormal(0))
			return 0;
	}
	
	//TangentNoraml
	SKDataBuffer * pTangent = NULL;
	pTangent = new SKDataBuffer();
	if(!pTangent)
		return 0;
	if(!pTangent->CreateEmptyBuffer(Num,SKDataBuffer::DT_FLOAT32_3))
		return 0;

	SKVector3 * pTangentBuffer = (SKVector3 *)pTangent->GetDate();

	//BiNoraml
	SKDataBuffer * pBiNormal = NULL;
	pBiNormal = new SKDataBuffer();
	if(!pBiNormal)
		return 0;
	if(!pBiNormal->CreateEmptyBuffer(Num,SKDataBuffer::DT_FLOAT32_3))
		return 0;

	SKVector3 * pBiNormalBuffer = (SKVector3 *)pBiNormal->GetDate();


	unsigned int TriangleNum = GetTotleNum();


	SKDataBuffer * pIndexDate = m_pIndexBuffer->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}

	// ȡָ
	SKUSHORT_INDEX * pIndexBuffer = (SKUSHORT_INDEX *)pIndexDate->GetDate();
	if(!pIndexBuffer)
		return 0;

	//ȡ
	SKDataBuffer * pDate = m_pVertexBuffer->GetPositionDate(0);
	if(!pDate)
		return 0;
	if(pDate->GetChannel() != 3)
		return 0;
	//ȡָ
	SKVector3 * pVertexBuffer = (SKVector3 *)pDate->GetDate();
	if(!pVertexBuffer)
		return 0;
	
	// get normal 
	pDate = m_pVertexBuffer->GetNormalDate(0);
	if(!pDate)
		return 0;
	SKVector3 * pNormalBuffer = (SKVector3 *)pDate->GetDate();
	if(!pNormalBuffer)
		return 0;

	// get texcoord
	pDate = m_pVertexBuffer->GetTexCoordDate(uiTexCoordLevel);
	if(!pDate)
		return 0;
	SKVector2 * pTexCoordBuffer = (SKVector2 *)pDate->GetDate();
	if(!pTexCoordBuffer)
		return 0;

	for (unsigned int poly = 0 ; poly < TriangleNum; poly++)
	{
		SKUSHORT_INDEX i0 = pIndexBuffer[poly*3+0];
		SKUSHORT_INDEX i1 = pIndexBuffer[poly*3+1];
		SKUSHORT_INDEX i2 = pIndexBuffer[poly*3+2];
		
		//U = ((v0 - v2)(P0 - P1) - (v0 - v1)(P0 - P2)) / ((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
		SKVector3 TangentNoraml;
		TangentNoraml = 
			(	(pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) - 
				(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y))/
			(	(pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y) - 
				(pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y));
		TangentNoraml.Normalize();
		pTangentBuffer[i0] += TangentNoraml;
		pTangentBuffer[i1] += TangentNoraml;
		pTangentBuffer[i2] += TangentNoraml;

		//V = ((u0 - u1)(P0 - P2) - (u0 - u2)(P0 - P1))/((v0 - v2)(u0 - u1) - (v0 - v1)(u0 - u2))
		SKVector3 BiNoraml;
		BiNoraml =
			(	(pVertexBuffer[i0] - pVertexBuffer[i1]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) - 
				(pVertexBuffer[i0] - pVertexBuffer[i2]) * (pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x))/
			(	(pTexCoordBuffer[i0].x - pTexCoordBuffer[i2].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i1].y) -
				(pTexCoordBuffer[i0].x - pTexCoordBuffer[i1].x) * (pTexCoordBuffer[i0].y - pTexCoordBuffer[i2].y));
		BiNoraml.Normalize();
		pBiNormalBuffer[i0] += BiNoraml;
		pBiNormalBuffer[i1] += BiNoraml;
		pBiNormalBuffer[i2] += BiNoraml;

	}
	for (unsigned int i = 0 ;i < m_pVertexBuffer->GetVertexNum() ;i++)
	{
		SKVector3 N1 = pTangentBuffer[i];
		SKVector3 N2 = pBiNormalBuffer[i];
		SKVector3 Normal = pNormalBuffer[i];

		SKVector3 Tangent = N1 - Normal * (( N1 * Normal) / (Normal * Normal));

		SKVector3 Binromal = N2 - Normal * (( N2 * Normal) / (Normal * Normal)) - Tangent * (( N2 * Tangent) / (Tangent * Tangent));
		
		Tangent.Normalize();
		Binromal.Normalize();
		pTangentBuffer[i] = Tangent;
		pBiNormalBuffer[i] = Binromal;	
	}
	m_pVertexBuffer->SetDate(pBiNormal,SKVertexFormat::VF_BINORMAL);
	m_pVertexBuffer->SetDate(pTangent,SKVertexFormat::VF_TANGENT);
	return 1;
}
unsigned int SKTriangleSet::GetTotleNum()const
{
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 3;
	}
	else
	{
		return m_pIndexBuffer->GetNum() / 3;
	}
}
unsigned int SKTriangleSet::GetGirdNum(unsigned int uiInputNum)const
{

	return uiInputNum / 3;
}
bool SKTriangleSet::CreateCollapseRecord()
{
	if (!m_pIndexBuffer)
	{
		return false;
	}
	m_pCollapseRecord = NULL;
	m_pCollapseRecord = SKCreateClodTriFactory::CreateClodTriangle(this);
	if (m_pCollapseRecord)
	{
		m_pIndexBuffer->SetStatic(false);
		return 1;
	}
	else
		return 0;
}
void SKTriangleSet::UpDateClodMesh()
{
	if (!m_pCollapseRecord)
	{
		return;
	}
	if (!m_pIndexBuffer)
	{
		return ;
	}
	if (!m_pIndexBuffer->GetIdentifier())
	{
		return;
	}
	SKCollapseRecord * pCollapseRecord = m_pCollapseRecord;
	if (pCollapseRecord->m_uiCurRecordID == pCollapseRecord->DesireRecordID())
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
		const RecordTriInfo & Record = ((SKCollapseRecordTri *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		for (unsigned int i = 0 ; i < Record.m_MapIndex.GetNum() ; i++)
		{
			pIndex[Record.m_MapIndex[i]] = Record.m_uiKeep;
		}
		
		
	}
	while(pCollapseRecord->m_uiCurRecordID > pCollapseRecord->DesireRecordID())
	{
		
		const RecordTriInfo & Record = ((SKCollapseRecordTri *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		for (unsigned int i = 0 ; i < Record.m_MapIndex.GetNum() ; i++)
		{
			pIndex[Record.m_MapIndex[i]] = Record.m_uiThrow;
		}
		pCollapseRecord->m_uiCurRecordID--;
	}
	m_pIndexBuffer->UnLock();
}
unsigned int SKTriangleSet::GetActiveNum()const
{
	if (m_pCollapseRecord)
	{
		SKCollapseRecord * pCollapseRecord = m_pCollapseRecord;
		const RecordTriInfo & Record = ((SKCollapseRecordTri *)pCollapseRecord)->m_RecordInfo[pCollapseRecord->m_uiCurRecordID];
		return Record.m_uiLeftTraingleNum;
	}
	else
		return GetTotleNum();
}