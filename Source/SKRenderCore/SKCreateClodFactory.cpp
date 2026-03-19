#include "SKCreateClodFactory.h"
#include "SKSegment3.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
SKArray<SKCreateClodTriFactory::Vertex3DAttr> SKCreateClodTriFactory::ms_V3Attr;
SKArray<SKCreateClodTriFactory::Edge3DAttr> SKCreateClodTriFactory::ms_E3Attr;
SKArray<SKCreateClodTriFactory::Triangle3DAttr> SKCreateClodTriFactory::ms_T3Attr;
SKTriangleSet* SKCreateClodTriFactory::ms_pTriangleSet = NULL;
SKCollapseRecordTri *SKCreateClodTriFactory::ms_pCRT = NULL;
SKArray<unsigned int> SKCreateClodTriFactory::ms_RecordTriangle;
unsigned int SKCreateClodTriFactory::ms_uiLeftTrangleNum = 0;
SKCollapseRecordTri * SKCreateClodTriFactory::CreateClodTriangle( SKTriangleSet * pTriangleSet)
{
	if (!pTriangleSet)
	{
		return 0;
	}
	ms_pTriangleSet = pTriangleSet;
	if (!ms_pTriangleSet->GetVertexBuffer())
	{
		return 0;

	}

	SKDataBuffer * pVertexDate = ms_pTriangleSet->GetVertexBuffer()->GetPositionDate(0);

	if (!pVertexDate)
	{
		return 0;
	}

	if (!pVertexDate->GetDate())
	{
		return 0;
	}
	unsigned int uiVerNum = pVertexDate->GetNum();

	if ( !uiVerNum)
	{
		return 0;
	}

	if (!ms_pTriangleSet->GetIndexBuffer())
	{
		return 0;
	}

	if (!ms_pTriangleSet->GetIndexBuffer()->GetIndexDate() || !ms_pTriangleSet->GetIndexBuffer()->GetNum())
	{
		return 0;
	}

	ms_V3Attr.Clear();
	ms_E3Attr.Clear();
	ms_T3Attr.Clear();

	SKMAC_DELETE(ms_pCRT);

	ms_pCRT = SK_NEW SKCollapseRecordTri();

	if (!ms_pCRT)
	{
		return 0;
	}
	ms_RecordTriangle.Clear();

	InitalDate();
	//ԭʼϢΪ 0  record 滻mapindexΪ0
	//keep  throw Ч
	RecordTriInfo CollapseRecord;
	CollapseRecord.m_uiKeep = SKMAX_INTEGER;
	CollapseRecord.m_uiThrow = SKMAX_INTEGER;
	CollapseRecord.m_uiLeftTraingleNum = ms_uiLeftTrangleNum;
	ms_pCRT->m_RecordInfo.AddElement(CollapseRecord);
	unsigned int uiE3DIndex = 0 , uiKeepV3DIndex = 0, uiThrowV3DIndex = 0;

	while(GetCollapseEV(uiE3DIndex,uiKeepV3DIndex,uiThrowV3DIndex))
	{
		Collapse(uiE3DIndex,uiKeepV3DIndex,uiThrowV3DIndex);
// 		for(unsigned int i = 0 ; i < ms_E3Attr.GetNum() ; i++)
// 		{
// 			if(!ms_E3Attr[i].m_bIsDelete)
// 				ms_E3Attr[i].m_fWeight = GetCollapseWeight(ms_E3Attr[i]);
// 		}
	}

	//Σɾģ˳򶼷ں
	unsigned int uiExChangeIndex = 0;

	SKDataBuffer * pIndexDate = ms_pTriangleSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}

	SKUSHORT_INDEX *pIndex = (SKUSHORT_INDEX *)pIndexDate->GetDate();
	SKDataBufferPtr pCopeIndexBuffer = SK_NEW SKDataBuffer();
	SKObject::CloneObject(pIndexDate,pCopeIndexBuffer);
	SKUSHORT_INDEX *pCopyIndex = (SKUSHORT_INDEX *)pCopeIndexBuffer->GetDate();
	SKArray<unsigned int> MapTriangeIndex;
	for (unsigned int i = 0 ; i < ms_T3Attr.GetNum() ; i++)
	{
		if (!ms_T3Attr[i].m_bIsDelete)
		{
			pIndex[3 * uiExChangeIndex + 0] = pCopyIndex[3 * i + 0];

			pIndex[3 * uiExChangeIndex + 1] = pCopyIndex[3 * i + 1];

			pIndex[3 * uiExChangeIndex + 2] = pCopyIndex[3 * i + 2];

			uiExChangeIndex++;
			MapTriangeIndex.AddElement(i);
		}
	}
	for (int i = ms_RecordTriangle.GetNum() - 1 ; i >= 0 ; i--)
	{
		pIndex[3 * uiExChangeIndex + 0] = pCopyIndex[3 * ms_RecordTriangle[i] + 0];

		pIndex[3 * uiExChangeIndex + 1] = pCopyIndex[3 * ms_RecordTriangle[i] + 1];

		pIndex[3 * uiExChangeIndex + 2] = pCopyIndex[3 * ms_RecordTriangle[i] + 2];
		MapTriangeIndex.AddElement(ms_RecordTriangle[i]);
		uiExChangeIndex++;
	}

	//ӳ
	for (int i = ms_pCRT->m_RecordInfo.GetNum() - 1 ;i >=0  ; i--)
	{
		RecordTriInfo & RecordInfoRef = ms_pCRT->m_RecordInfo[i];
		for (unsigned int j = 0 ; j < RecordInfoRef.m_MapIndex.GetNum() ; j++)
		{
			for (unsigned int k = 0 ; k < MapTriangeIndex.GetNum() ; k++)
			{
				bool IsFind = false;
				for (unsigned int s = 0 ; s < 3 ; s++)
				{
					if (RecordInfoRef.m_MapIndex[j] == (MapTriangeIndex[k] * 3 + s))
					{
						RecordInfoRef.m_MapIndex[j] = k * 3 + s;
						IsFind = true;
						break;
					}
				}
				if (IsFind)
				{
					break;
				}
				
			}
		}
	}
	SKCollapseRecordTri * PReturn = ms_pCRT;
	ms_pCRT = NULL;
	//ֻһԭʼϢļ¼ϢûУɾ,ʾMeshѾ޷
	if (PReturn->m_RecordInfo.GetNum() == 1)
	{
		SKMAC_DELETE(PReturn);
	}
// 	SKLog RecordLod;
// 	RecordLod.FOpen("RecordLog.txt");
// 	for (unsigned int i = 0 ; i < PReturn->m_RecordInfo.GetNum() ; i++)
// 	{
// 		RecordLod.WriteInfo("%dRecord\n",i);
// 		RecordLod.WriteInfo("Keep %d,Throw %d,Left %d\n",PReturn->m_RecordInfo[i].m_uiKeep,
// 							PReturn->m_RecordInfo[i].m_uiThrow,PReturn->m_RecordInfo[i].m_uiLeftTraingleNum);
// 		RecordLod.WriteInfo("Index:");
// 		for (unsigned int j = 0 ; j < PReturn->m_RecordInfo[i].m_MapIndex.GetNum() ; j++)
// 		{
// 			RecordLod.WriteInfo("%d ",PReturn->m_RecordInfo[i].m_MapIndex[j]);
// 		}
// 		RecordLod.WriteInfo("\n");
// 	}
	
	return PReturn;
}
void SKCreateClodTriFactory::InitalDate()
{

	SKDataBuffer * pIndexDate = ms_pTriangleSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return ;
	}
	unsigned int uiVerNum = ms_pTriangleSet->GetVertexBuffer()->GetVertexNum();

	unsigned int uiTriNum = ms_pTriangleSet->GetTotleNum();
	ms_uiLeftTrangleNum = uiTriNum;
	SKDataBuffer * pVertexDate = ms_pTriangleSet->GetVertexBuffer()->GetPositionDate(0);
	ms_V3Attr.SetBufferNum(uiVerNum);
	SKVector3 * pVer = (SKVector3 *)pVertexDate->GetDate();

	for (unsigned int i = 0 ; i < uiVerNum ; i++)
	{
		ms_V3Attr[i].m_bIsDelete = false;
		ms_V3Attr[i].m_Position = *pVer;
		pVer++;
	}


	ms_T3Attr.SetBufferNum(uiTriNum);

	

	SKUSHORT_INDEX * pIndex = (SKUSHORT_INDEX *)pIndexDate->GetDate();

	for(unsigned int i = 0 ; i < uiTriNum ; i++)
	{
		ms_T3Attr[i].m_bIsDelete = false;
		ms_T3Attr[i].m_ContainVertexIndex[0] = *pIndex;
		ms_V3Attr[*pIndex].m_InTriangleIndexArray.AddElement(i);
		pIndex++;

		ms_T3Attr[i].m_ContainVertexIndex[1] = *pIndex;
		ms_V3Attr[*pIndex].m_InTriangleIndexArray.AddElement(i);
		pIndex++;

		ms_T3Attr[i].m_ContainVertexIndex[2] = *pIndex;
		ms_V3Attr[*pIndex].m_InTriangleIndexArray.AddElement(i);
		pIndex++;


	}



	for(unsigned int i = 0 ; i < uiTriNum ; i++)
	{
		for (unsigned int j = 0 ; j < 3 ; j++)
		{
			unsigned int uiVertexIndex0 = ms_T3Attr[i].m_ContainVertexIndex[j];
			unsigned int uiVertexIndex1 = ms_T3Attr[i].m_ContainVertexIndex[(j + 1) % 3];

			unsigned int uiEdge = 0;
			for ( uiEdge = 0 ; uiEdge < ms_E3Attr.GetNum() ; uiEdge++)
			{
				if ((ms_E3Attr[uiEdge].m_ContainVertexIndex[0] == uiVertexIndex0 && 
					ms_E3Attr[uiEdge].m_ContainVertexIndex[1] == uiVertexIndex1) ||
					(ms_E3Attr[uiEdge].m_ContainVertexIndex[1] == uiVertexIndex0 && 
					ms_E3Attr[uiEdge].m_ContainVertexIndex[0] == uiVertexIndex1))
				{
					break;
				}
			}

			if (uiEdge == ms_E3Attr.GetNum())
			{
				Edge3DAttr NewEdge;
				NewEdge.m_ContainVertexIndex[0] = uiVertexIndex0;
				NewEdge.m_ContainVertexIndex[1] = uiVertexIndex1;
				ms_E3Attr.AddElement(NewEdge);

				ms_V3Attr[uiVertexIndex0].m_InEdgeIndexArray.AddElement(uiEdge);
				ms_V3Attr[uiVertexIndex1].m_InEdgeIndexArray.AddElement(uiEdge);
			}
			ms_E3Attr[uiEdge].m_InTriangleIndexArray.AddElement(i);
			ms_T3Attr[i].m_ContainEdgeIndex[j] = uiEdge;
		}

	}

// 	SKLog LogEdge;
// 	LogEdge.FOpen("LogEdge.txt");
	for(unsigned int i = 0 ; i < ms_E3Attr.GetNum() ; i++)
	{
		ms_E3Attr[i].m_bIsDelete = false;
		ms_E3Attr[i].m_fWeight = GetCollapseWeight(ms_E3Attr[i]);
//		LogEdge.WriteInfo("%dIndex %d,%d Weight: %f\n",i,ms_E3Attr[i].m_ContainVertexIndex[0],ms_E3Attr[i].m_ContainVertexIndex[1],ms_E3Attr[i].m_fWeight);
	}


}
SKREAL SKCreateClodTriFactory::GetCollapseWeight(const Edge3DAttr & Edge3D)
{
	if (Edge3D.m_bIsDelete == true)
	{
		return SKMAX_REAL;
	}
	const SKREAL cfLengthWeight = 10.0f;
	const SKREAL cfAngleWeight = 1.0f;
	if (Edge3D.m_InTriangleIndexArray.GetNum() == 2)
	{
		unsigned int uiIndex0 = Edge3D.m_ContainVertexIndex[0];
		unsigned int uiIndex1 = Edge3D.m_ContainVertexIndex[1];
		SKMAC_ASSERT(uiIndex0 < ms_V3Attr.GetNum() && uiIndex1 < ms_V3Attr.GetNum());
		SKMAC_ASSERT(!ms_V3Attr[uiIndex0].m_bIsDelete && !ms_V3Attr[uiIndex1].m_bIsDelete );
		// 		if (uiIndex0 >= ms_V3Attr.GetNum() || uiIndex1 >= ms_V3Attr.GetNum())
		// 		{
		// 			return SKMAX_REAL;
		// 		}
		SKVector3 V0 = ms_V3Attr[uiIndex0].m_Position;
		SKVector3 V1 = ms_V3Attr[uiIndex1].m_Position;

		SKVector3 Diff = V0 - V1;

		SKREAL fWeight = cfLengthWeight * Diff.GetLength();



		unsigned int uiTri0 = Edge3D.m_InTriangleIndexArray[0];
		unsigned int uiTri1 = Edge3D.m_InTriangleIndexArray[1];

		SKMAC_ASSERT(uiTri0 < ms_T3Attr.GetNum() && uiTri1 < ms_T3Attr.GetNum());
		SKMAC_ASSERT(!ms_T3Attr[uiTri0].m_bIsDelete && !ms_T3Attr[uiTri1].m_bIsDelete);
		// 		if (uiTri0 >= ms_T3Attr.GetNum() || uiTri1 >= ms_T3Attr.GetNum() )
		// 		{
		// 			return SKMAX_REAL;
		// 		}
		SKMAC_ASSERT(ms_T3Attr[uiTri0].m_ContainVertexIndex[0] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri0].m_ContainVertexIndex[1] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri0].m_ContainVertexIndex[2] < ms_V3Attr.GetNum());

		SKMAC_ASSERT(!ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[0]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[1]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[2]].m_bIsDelete );

		// 		if (ms_T3Attr[uiTri0].m_ContainVertexIndex[0] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri0].m_ContainVertexIndex[1] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri0].m_ContainVertexIndex[2] >= ms_V3Attr.GetNum())
		// 		{
		// 			return SKMAX_REAL;
		// 		}

		SKVector3 T0V0 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[0]].m_Position;
		SKVector3 T0V1 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[1]].m_Position;
		SKVector3 T0V2 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[2]].m_Position;

		SKVector3 DiffT0_01 = T0V1 - T0V0;
		SKVector3 DiffT0_02 = T0V2 - T0V0;

		SKVector3 T0N;
		T0N.Cross(DiffT0_01,DiffT0_02);

		SKMAC_ASSERT(ms_T3Attr[uiTri1].m_ContainVertexIndex[0] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri1].m_ContainVertexIndex[1] < ms_V3Attr.GetNum() &&
			ms_T3Attr[uiTri1].m_ContainVertexIndex[2] < ms_V3Attr.GetNum());

		SKMAC_ASSERT(!ms_V3Attr[ms_T3Attr[uiTri1].m_ContainVertexIndex[0]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri1].m_ContainVertexIndex[1]].m_bIsDelete &&
			!ms_V3Attr[ms_T3Attr[uiTri1].m_ContainVertexIndex[2]].m_bIsDelete );
		// 		if (ms_T3Attr[uiTri1].m_ContainVertexIndex[0] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri1].m_ContainVertexIndex[1] >= ms_V3Attr.GetNum() ||
		// 			ms_T3Attr[uiTri1].m_ContainVertexIndex[2] >= ms_V3Attr.GetNum())
		// 		{
		// 			return SKMAX_REAL;
		// 		}

		SKVector3 T1V0 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[0]].m_Position;
		SKVector3 T1V1 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[1]].m_Position;
		SKVector3 T1V2 = ms_V3Attr[ms_T3Attr[uiTri0].m_ContainVertexIndex[2]].m_Position;

		SKVector3 DiffT1_01 = T1V1 - T1V0;
		SKVector3 DiffT1_02 = T1V2 - T1V0;

		SKVector3 T1N;
		T1N.Cross(DiffT1_01,DiffT1_02);

		//2ԽֱȨԽc = |a| |b| sin; ҲǺ2μнйأ
		SKVector3 CrossAngle;
		CrossAngle.Cross(T0N,T1N);
		fWeight += CrossAngle.GetLength() * cfAngleWeight;

		return fWeight;
	}
	return SKMAX_REAL;
}
bool SKCreateClodTriFactory::GetCollapseEV(unsigned int &uiE3DIndex,unsigned int &uiKeepV3DIndex,unsigned int &uiThrowV3DIndex)
{
	while(1)
	{
		SKREAL fFindMinWeight = SKMAX_REAL;
		Edge3DAttr * pDesireEdge3D = NULL;
		for (unsigned int i = 0 ; i < ms_E3Attr.GetNum() ;i++)
		{
			if (ms_E3Attr[i].m_bIsDelete)
			{
				continue;
			}
			if (fFindMinWeight > ms_E3Attr[i].m_fWeight)
			{
				fFindMinWeight = ms_E3Attr[i].m_fWeight;
				pDesireEdge3D = &ms_E3Attr[i];
				uiE3DIndex = i;
			}
		}

		if (fFindMinWeight >= SKMAX_REAL || pDesireEdge3D == NULL)
		{
			return 0;
		}

		//⹲϶бߣб߶2ιthrowҲ
		//ߵȨظĳVSMAX_REALңֱСȨΪVSMAX_REALClodMesh
		unsigned int j = 0;
		for (j = 0 ; j < 2 ; j++)
		{
			unsigned int uiV3DIndex = pDesireEdge3D->m_ContainVertexIndex[j];
			SKMAC_ASSERT(uiV3DIndex < ms_V3Attr.GetNum());
			SKMAC_ASSERT(!ms_V3Attr[uiV3DIndex].m_bIsDelete);
			unsigned int k = 0;
			for ( k = 0 ; k < ms_V3Attr[uiV3DIndex].m_InEdgeIndexArray.GetNum() ; k++)
			{
				unsigned int uiE3DIndex = ms_V3Attr[uiV3DIndex].m_InEdgeIndexArray[k];

				SKMAC_ASSERT(uiE3DIndex < ms_E3Attr.GetNum());
				SKMAC_ASSERT(!ms_E3Attr[uiE3DIndex].m_bIsDelete);
				if (ms_E3Attr[uiE3DIndex].m_InTriangleIndexArray.GetNum() != 2)
				{
					break;
				}
			}
			if (k == ms_V3Attr[uiV3DIndex].m_InEdgeIndexArray.GetNum())
			{
				break;
			}
		}
		if (j < 2)
		{
			uiThrowV3DIndex = pDesireEdge3D->m_ContainVertexIndex[j];
			uiKeepV3DIndex = pDesireEdge3D->m_ContainVertexIndex[1 -j];
			return 1;
		}
		else
		{
			pDesireEdge3D->m_fWeight = SKMAX_REAL;

		}
	}
	return 1;
}
bool SKCreateClodTriFactory::Collapse(unsigned int uiE3DIndex,unsigned int uiKeepV3DIndex,unsigned int uiThrowV3DIndex)
{
	SKMAC_ASSERT(uiE3DIndex < ms_E3Attr.GetNum());
	SKMAC_ASSERT(!ms_E3Attr[uiE3DIndex].m_bIsDelete);

	SKMAC_ASSERT(uiKeepV3DIndex < ms_V3Attr.GetNum());
	SKMAC_ASSERT(!ms_V3Attr[uiKeepV3DIndex].m_bIsDelete);
	SKMAC_ASSERT(uiThrowV3DIndex < ms_V3Attr.GetNum());
	SKMAC_ASSERT(!ms_V3Attr[uiThrowV3DIndex].m_bIsDelete);

	RecordTriInfo CollapseRecord;
	CollapseRecord.m_uiKeep = uiKeepV3DIndex;
	CollapseRecord.m_uiThrow = uiThrowV3DIndex;
	CollapseRecord.m_uiLeftTraingleNum = ms_uiLeftTrangleNum;
	//1.ɾuiE3DIndexߵΣҪͬʱѹεĶͱбɾ
	for (unsigned int i = 0 ; i < ms_E3Attr[uiE3DIndex].m_InTriangleIndexArray.GetNum() ; i++)
	{
		//ɾ
		unsigned int uiT3DIndex = ms_E3Attr[uiE3DIndex].m_InTriangleIndexArray[i];
		SKMAC_ASSERT(uiT3DIndex < ms_T3Attr.GetNum());
		SKMAC_ASSERT(!ms_T3Attr[uiT3DIndex].m_bIsDelete);
		ms_T3Attr[uiT3DIndex].m_bIsDelete = true;
		ms_uiLeftTrangleNum--;
		//ɾε3жӦϢ
		for (unsigned int j = 0 ; j < 3 ; j++)
		{
			unsigned int uiEdgeIndex = ms_T3Attr[uiT3DIndex].m_ContainEdgeIndex[j];
			SKMAC_ASSERT(uiEdgeIndex < ms_E3Attr.GetNum());
			SKMAC_ASSERT(!ms_E3Attr[uiEdgeIndex].m_bIsDelete);
			if(uiE3DIndex == uiEdgeIndex)
			{
				i--;
			}
			unsigned int k = 0;
			for (k = 0 ; k < ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray.GetNum() ; k++)
			{
				if (ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray[k] == uiT3DIndex)
				{
					break;
				}	
			}
			//ҵ,߲Σɾ
			SKMAC_ASSERT(k < ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray.GetNum());
			ms_E3Attr[uiEdgeIndex].m_InTriangleIndexArray.Erase(k);
		}
		//ɾζӦ3Ϣ
		for (unsigned int j = 0 ; j < 3 ;j ++)
		{
			unsigned int uiVIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[j];
			SKMAC_ASSERT(uiVIndex < ms_V3Attr.GetNum());
			SKMAC_ASSERT(!ms_V3Attr[uiVIndex].m_bIsDelete);

			unsigned int k = 0;
			for (k = 0 ; k < ms_V3Attr[uiVIndex].m_InTriangleIndexArray.GetNum() ; k++)
			{
				if (ms_V3Attr[uiVIndex].m_InTriangleIndexArray[k] == uiT3DIndex)
				{
					break;
				}	
			}
			//ҵΣ㲻Σɾ
			SKMAC_ASSERT(k < ms_V3Attr[uiVIndex].m_InTriangleIndexArray.GetNum());
			ms_V3Attr[uiVIndex].m_InTriangleIndexArray.Erase(k);
		}
		ms_RecordTriangle.AddElement(uiT3DIndex);
	}

	//2.ɾϢͬʱ߶бϢɾбϢΪѾɾ
	ms_E3Attr[uiE3DIndex].m_bIsDelete = true;
	for (unsigned int i = 0 ; i < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum() ; i++)
	{
		if (ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[i] == uiE3DIndex)
		{
			ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.Erase(i);
			break;
		}
	}
	for (unsigned int i = 0 ; i < ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.GetNum() ; i++)
	{
		if (ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[i] == uiE3DIndex)
		{
			ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.Erase(i);
			break;
		}
	}

	//3.ҪɾΣ滻2
	for (unsigned int i = 0 ; i < ms_V3Attr[uiThrowV3DIndex].m_InTriangleIndexArray.GetNum() ; i++)
	{
		unsigned int uiT3DIndex = ms_V3Attr[uiThrowV3DIndex].m_InTriangleIndexArray[i];
		SKMAC_ASSERT(uiT3DIndex < ms_T3Attr.GetNum());
		SKMAC_ASSERT(!ms_T3Attr[uiT3DIndex].m_bIsDelete);

		bool bFound = false;
		unsigned int uiFoundIndex;
		for (unsigned int j = 0 ; j < 3 ; j++)
		{
			if(ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[j] == uiThrowV3DIndex)
			{
				SKMAC_ASSERT(!bFound);
				uiFoundIndex = j;
				bFound = true;
			}

		}
		SKMAC_ASSERT(bFound);

		unsigned int V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[0];
		SKMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		SKMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		SKVector3 V0 = ms_V3Attr[V3DIndex].m_Position;

		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[1];
		SKMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		SKMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		SKVector3 V1 = ms_V3Attr[V3DIndex].m_Position;


		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[2];
		SKMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		SKMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		SKVector3 V2 = ms_V3Attr[V3DIndex].m_Position;


		//滻¶
		ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[uiFoundIndex] = uiKeepV3DIndex;

		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[0];
		SKMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		SKMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		SKVector3 NewV0 = ms_V3Attr[V3DIndex].m_Position;

		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[1];
		SKMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		SKMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		SKVector3 NewV1 = ms_V3Attr[V3DIndex].m_Position;


		V3DIndex = ms_T3Attr[uiT3DIndex].m_ContainVertexIndex[2];
		SKMAC_ASSERT(V3DIndex < ms_V3Attr.GetNum());
		SKMAC_ASSERT(!ms_V3Attr[V3DIndex].m_bIsDelete);
		SKVector3 NewV2 = ms_V3Attr[V3DIndex].m_Position;

		SKVector3 Normal;
		Normal.Cross(V2 - V0 , V1 - V0);

		SKVector3 NewNormal;
		NewNormal.Cross(NewV2 - NewV0 , NewV1 - NewV0);

		
		
		ms_V3Attr[uiKeepV3DIndex].m_InTriangleIndexArray.AddElement(uiT3DIndex);
		CollapseRecord.m_MapIndex.AddElement(uiT3DIndex * 3 + uiFoundIndex);
		
	}
	
	SKMAC_ASSERT(CollapseRecord.m_uiLeftTraingleNum !=ms_uiLeftTrangleNum);

	CollapseRecord.m_uiLeftTraingleNum = ms_uiLeftTrangleNum;

	//4.滻
	for (unsigned int i = 0 ; i < ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.GetNum(); i++)
	{
		unsigned int uiE3DIndex = ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[i];
		for (unsigned int j = 0 ; j < 2 ; j++)
		{
			if(ms_E3Attr[uiE3DIndex].m_ContainVertexIndex[j] == uiThrowV3DIndex)
			{
				ms_E3Attr[uiE3DIndex].m_ContainVertexIndex[j] = uiKeepV3DIndex;
				ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.AddElement(uiE3DIndex);
				break;
			}
		}
		
	}
	

	ms_V3Attr[uiThrowV3DIndex].m_bIsDelete = true;
	//5.滻ıϢ,ϲϢȻ߹ΣЩεıϢ
	for (unsigned int i = 0 ; i < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum(); i++)
	{
		unsigned int uiEIndex1 = ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[i];

		for (unsigned int j = 0 ; j < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum(); j++)
		{
			unsigned int uiEIndex2 = ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[j];

			if (uiEIndex1 != uiEIndex2)
			{

				if (ms_E3Attr[uiEIndex1] == ms_E3Attr[uiEIndex2])
				{
					ms_E3Attr[uiEIndex2].m_bIsDelete = true;
					//ߵΣҪ滻
					for (unsigned int k = 0 ; k < ms_E3Attr[uiEIndex2].m_InTriangleIndexArray.GetNum(); k++)
					{
						unsigned int uiTIndex = ms_E3Attr[uiEIndex2].m_InTriangleIndexArray[k];
						ms_E3Attr[uiEIndex1].m_InTriangleIndexArray.AddElement(uiTIndex);
						for (unsigned int m = 0 ; m < 3 ; m++)
						{
							if (ms_T3Attr[uiTIndex].m_ContainEdgeIndex[m] == uiEIndex2)
							{
								ms_T3Attr[uiTIndex].m_ContainEdgeIndex[m] = uiEIndex1;
							}
						}
						
					}
					//ߵĶ㣬Ҫɾ
					ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.Erase(j);
					
					unsigned int uiAnotherV = ms_E3Attr[uiEIndex2].m_ContainVertexIndex[0];
					if(uiAnotherV == uiKeepV3DIndex)
					{
						uiAnotherV = ms_E3Attr[uiEIndex2].m_ContainVertexIndex[1];
					}
					
					for(unsigned int s = 0 ; s < ms_V3Attr[uiAnotherV].m_InEdgeIndexArray.GetNum(); s++)
					{
						if (uiEIndex2 == ms_V3Attr[uiAnotherV].m_InEdgeIndexArray[s])
						{
							ms_V3Attr[uiAnotherV].m_InEdgeIndexArray.Erase(s);
							break;
						}
					
					}
					j--;
					
				}

				

			}
		}	
	}
	for (unsigned int i = 0 ; i < ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray.GetNum(); i++)
	{
		unsigned int uiEIndex1 = ms_V3Attr[uiKeepV3DIndex].m_InEdgeIndexArray[i];
		ms_E3Attr[uiEIndex1].m_fWeight = GetCollapseWeight(ms_E3Attr[uiEIndex1]);
	}
	ms_pCRT->m_RecordInfo.AddElement(CollapseRecord);
	return 1;
}



SKArray<SKCreateClodLineFactory::Vertex3DAttr> SKCreateClodLineFactory::ms_V3Attr;
SKArray<SKCreateClodLineFactory::Edge3DAttr> SKCreateClodLineFactory::ms_E3Attr;
SKLineSet* SKCreateClodLineFactory::ms_pLineSet = NULL;
SKCollapseRecordLine *SKCreateClodLineFactory::ms_pCRL = NULL;
SKArray<unsigned int> SKCreateClodLineFactory::ms_RecordLine;
unsigned int SKCreateClodLineFactory::ms_uiLeftLineNum = 0;
SKCollapseRecordLine * SKCreateClodLineFactory::CreateClodLine( SKLineSet * pLineSet)
{
	if (!pLineSet)
	{
		return 0;
	}
	ms_pLineSet = pLineSet;
	if (!ms_pLineSet->GetVertexBuffer())
	{
		return 0;

	}

	SKDataBuffer * pVertexDate = ms_pLineSet->GetVertexBuffer()->GetPositionDate(0);

	if (!pVertexDate)
	{
		return 0;
	}

	if (!pVertexDate->GetDate())
	{
		return 0;
	}
	unsigned int uiVerNum = pVertexDate->GetNum();

	if ( !uiVerNum)
	{
		return 0;
	}

	if (!ms_pLineSet->GetIndexBuffer())
	{
		return 0;
	}

	if (!ms_pLineSet->GetIndexBuffer()->GetIndexDate() || !ms_pLineSet->GetIndexBuffer()->GetNum())
	{
		return 0;
	}

	ms_V3Attr.Clear();
	ms_E3Attr.Clear();

	SKMAC_DELETE(ms_pCRL);

	ms_pCRL = SK_NEW SKCollapseRecordLine();

	if (!ms_pCRL)
	{
		return 0;
	}
	ms_RecordLine.Clear();

	InitalDate();
	//ԭʼϢΪ 0  record 滻mapindexΪ0
	//keep  throw Ч
	RecordLineInfo CollapseRecord;
	CollapseRecord.m_uiKeep = SKMAX_INTEGER;
	CollapseRecord.m_uiThrow = SKMAX_INTEGER;
	CollapseRecord.m_uiLeftEdgeNum = ms_uiLeftLineNum;
	ms_pCRL->m_RecordInfo.AddElement(CollapseRecord);
	unsigned int uiThrowV3DIndex = 0;

	while(GetCollapseV(uiThrowV3DIndex))
	{
		Collapse(uiThrowV3DIndex);

	}

	//ߣɾģ˳򶼷ں
	unsigned int uiExChangeIndex = 0;

	SKDataBuffer * pIndexDate = ms_pLineSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return 0;
	}

	SKUSHORT_INDEX *pIndex = (SKUSHORT_INDEX *)pIndexDate->GetDate();
	SKDataBufferPtr pCopeIndexBuffer = SK_NEW SKDataBuffer();
	SKObject::CloneObject(pIndexDate,pCopeIndexBuffer);
	SKUSHORT_INDEX *pCopyIndex = (SKUSHORT_INDEX *)pCopeIndexBuffer->GetDate();
	SKArray<unsigned int> MapLineIndex;
	for (unsigned int i = 0 ; i < ms_E3Attr.GetNum() ; i++)
	{
		if (!ms_E3Attr[i].m_bIsDelete)
		{
			pIndex[2 * uiExChangeIndex + 0] = pCopyIndex[2 * i + 0];

			pIndex[2 * uiExChangeIndex + 1] = pCopyIndex[2 * i + 1];

			uiExChangeIndex++;
			MapLineIndex.AddElement(i);
		}
	}
	for (int i = ms_RecordLine.GetNum() - 1 ; i >= 0 ; i--)
	{
		pIndex[2 * uiExChangeIndex + 0] = pCopyIndex[2 * ms_RecordLine[i] + 0];

		pIndex[2 * uiExChangeIndex + 1] = pCopyIndex[2 * ms_RecordLine[i] + 1];
		MapLineIndex.AddElement(ms_RecordLine[i]);
		uiExChangeIndex++;
	}
 
	//ӳ
	for (int i = ms_pCRL->m_RecordInfo.GetNum() - 1 ;i >= 0  ; i--)
	{
		RecordLineInfo & RecordInfoRef = ms_pCRL->m_RecordInfo[i];
		
		for (unsigned int k = 0 ; k < MapLineIndex.GetNum() ; k++)
		{
			bool IsFind = false;
			for (unsigned int s = 0 ; s < 2 ; s++)
			{
				if (RecordInfoRef.m_MapIndex == (MapLineIndex[k] * 2 + s))
				{
					RecordInfoRef.m_MapIndex = k * 2 + s;
					IsFind = true;
					break;
				}
			}
			if (IsFind)
			{
				break;
			}

		}
	}
	SKCollapseRecordLine * PReturn = ms_pCRL;
	ms_pCRL = NULL;
	//ֻһԭʼϢļ¼ϢûУɾ,ʾMeshѾ޷
	if (PReturn->m_RecordInfo.GetNum() == 1)
	{
		SKMAC_DELETE(PReturn);
	}

	return PReturn;
}

void SKCreateClodLineFactory::InitalDate()
{

	SKDataBuffer * pIndexDate = ms_pLineSet->GetIndexBuffer()->GetIndexDate();
	if (!pIndexDate)
	{
		return ;
	}
	unsigned int uiVerNum = ms_pLineSet->GetVertexBuffer()->GetVertexNum();

	unsigned int uiLineNum = ms_pLineSet->GetTotleNum();

	ms_uiLeftLineNum = uiLineNum;
	SKDataBuffer * pVertexDate = ms_pLineSet->GetVertexBuffer()->GetPositionDate(0);
	ms_V3Attr.SetBufferNum(uiVerNum);
	SKVector3 * pVer = (SKVector3 *)pVertexDate->GetDate();

	for (unsigned int i = 0 ; i < uiVerNum ; i++)
	{
		ms_V3Attr[i].m_bIsDelete = false;
		ms_V3Attr[i].m_Position = *pVer;
		ms_V3Attr[i].m_uiIndexInVertexArray = i;
		pVer++;
	}


	ms_E3Attr.SetBufferNum(uiLineNum);

	

	SKUSHORT_INDEX * pIndex = (SKUSHORT_INDEX *)pIndexDate->GetDate();

	for(unsigned int i = 0 ; i < uiLineNum ; i++)
	{
		ms_E3Attr[i].m_bIsDelete = false;
		ms_E3Attr[i].m_ContainVertexIndex[0] = *pIndex;
		ms_V3Attr[*pIndex].m_InEdgeIndexArray.AddElement(i);
		pIndex++;

		ms_E3Attr[i].m_ContainVertexIndex[1] = *pIndex;
		ms_V3Attr[*pIndex].m_InEdgeIndexArray.AddElement(i);
		pIndex++;

	}

	// 	SKLog LogEdge;
	// 	LogEdge.FOpen("LogEdge.txt");
	for(unsigned int i = 0 ; i < ms_V3Attr.GetNum() ; i++)
	{
		ms_V3Attr[i].m_bIsDelete = false;
		ms_V3Attr[i].m_fWeight = GetCollapseWeight(ms_V3Attr[i]);
		//		LogEdge.WriteInfo("%dIndex %d,%d Weight: %f\n",i,ms_E3Attr[i].m_ContainVertexIndex[0],ms_E3Attr[i].m_ContainVertexIndex[1],ms_E3Attr[i].m_fWeight);
	}


}
SKREAL SKCreateClodLineFactory::GetCollapseWeight(const Vertex3DAttr & Vertex3D)
{
	if (Vertex3D.m_bIsDelete == true)
	{
		return SKMAX_REAL;
	}
	if (Vertex3D.m_InEdgeIndexArray.GetNum() == 2)
	{
		unsigned int uiIndex0 = Vertex3D.m_InEdgeIndexArray[0];
		unsigned int uiIndex1 = Vertex3D.m_InEdgeIndexArray[1];

		
		SKMAC_ASSERT(uiIndex0 < ms_E3Attr.GetNum() && uiIndex1 < ms_E3Attr.GetNum());
		SKMAC_ASSERT(!ms_E3Attr[uiIndex0].m_bIsDelete && !ms_E3Attr[uiIndex1].m_bIsDelete );
		
		SKVector3 V0;
		SKVector3 V1;
		if (ms_E3Attr[uiIndex0].m_ContainVertexIndex[0] == Vertex3D.m_uiIndexInVertexArray)
		{
			V0 = ms_V3Attr[ms_E3Attr[uiIndex0].m_ContainVertexIndex[1]].m_Position;
		}
		else if(ms_E3Attr[uiIndex0].m_ContainVertexIndex[1] == Vertex3D.m_uiIndexInVertexArray)
		{
			V0 = ms_V3Attr[ms_E3Attr[uiIndex0].m_ContainVertexIndex[0]].m_Position;
		}
		else
		{
			SKMAC_ASSERT(0);
		}
		
		if (ms_E3Attr[uiIndex1].m_ContainVertexIndex[0] == Vertex3D.m_uiIndexInVertexArray)
		{
			V1 = ms_V3Attr[ms_E3Attr[uiIndex1].m_ContainVertexIndex[1]].m_Position;
		}
		else if(ms_E3Attr[uiIndex1].m_ContainVertexIndex[1] == Vertex3D.m_uiIndexInVertexArray)
		{
			V1 = ms_V3Attr[ms_E3Attr[uiIndex1].m_ContainVertexIndex[0]].m_Position;
		}
		else
		{
			SKMAC_ASSERT(0);
		}

		SKVector3 Diff = V0 - V1;

		SKREAL fLength = Diff.GetLength();
		SKSegment3 Segment3(V0,V1);
		SKREAL fParam;
		SKREAL fDistan = Vertex3D.m_Position.SquaredDistance(Segment3,fParam);
		
		return fDistan / fLength;
	}
	return SKMAX_REAL;
}
bool SKCreateClodLineFactory::GetCollapseV(unsigned int &uiThrowV3DIndex)
{

	SKREAL fFindMinWeight = SKMAX_REAL;
	Vertex3DAttr * pDesireVertex3D = NULL;
	for (unsigned int i = 0 ; i < ms_V3Attr.GetNum() ;i++)
	{
		if (ms_V3Attr[i].m_bIsDelete)
		{
			continue;
		}
		if (fFindMinWeight > ms_V3Attr[i].m_fWeight)
		{
			fFindMinWeight = ms_V3Attr[i].m_fWeight;
			pDesireVertex3D = &ms_V3Attr[i];
			uiThrowV3DIndex = i;
		}
	}

	if (fFindMinWeight >= SKMAX_REAL || pDesireVertex3D == NULL)
	{
		return 0;
	}

	return 1;
}
bool SKCreateClodLineFactory::Collapse(unsigned int uiThrowV3DIndex)
{

	SKMAC_ASSERT(uiThrowV3DIndex < ms_V3Attr.GetNum());
	SKMAC_ASSERT(!ms_V3Attr[uiThrowV3DIndex].m_bIsDelete);

	SKMAC_ASSERT(ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray.GetNum() == 2);
	unsigned int uiIndex0 = ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[0];
	unsigned int uiIndex1 = ms_V3Attr[uiThrowV3DIndex].m_InEdgeIndexArray[1];

	SKMAC_ASSERT(uiIndex0 < ms_E3Attr.GetNum() && uiIndex1 < ms_E3Attr.GetNum());
	SKMAC_ASSERT(!ms_E3Attr[uiIndex0].m_bIsDelete && !ms_E3Attr[uiIndex1].m_bIsDelete );

	//ɾͱ
	ms_V3Attr[uiThrowV3DIndex].m_bIsDelete = true;
	ms_uiLeftLineNum--;
	ms_E3Attr[uiIndex1].m_bIsDelete = true;

	//ҵɾߵĺthrowͬһ
	unsigned int uiEdgeV = ms_E3Attr[uiIndex1].m_ContainVertexIndex[0];

	if (uiEdgeV == uiThrowV3DIndex)
	{
		uiEdgeV = ms_E3Attr[uiIndex1].m_ContainVertexIndex[1];
	}

	ms_RecordLine.AddElement(uiIndex1);



	SKMAC_ASSERT(uiEdgeV < ms_V3Attr.GetNum());

	SKMAC_ASSERT(!ms_V3Attr[uiEdgeV].m_bIsDelete);

	//滻
	unsigned int uiEdgeV1 = ms_E3Attr[uiIndex0].m_ContainVertexIndex[0];
	unsigned int uiMapInde = 0;
	if (uiEdgeV1 == uiThrowV3DIndex)
	{
		uiEdgeV1 = ms_E3Attr[uiIndex0].m_ContainVertexIndex[1];

		uiMapInde = 0;
		ms_E3Attr[uiIndex0].m_ContainVertexIndex[0] = uiEdgeV;
	}
	else
	{
	
		uiMapInde = 1;
		ms_E3Attr[uiIndex0].m_ContainVertexIndex[1] = uiEdgeV;
	}

	//滻ı
	unsigned int i ;
	for ( i = 0 ; i < ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.GetNum() ;i++)
	{
		if (ms_V3Attr[uiEdgeV].m_InEdgeIndexArray[i] == uiIndex1)
		{
			
			break;
		}
	}
	SKMAC_ASSERT(i <ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.GetNum());
	ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.Erase(i);
	ms_V3Attr[uiEdgeV].m_InEdgeIndexArray.AddElement(uiIndex0);

	SKMAC_ASSERT(uiMapInde < 2);
	RecordLineInfo CollapseRecord;
	CollapseRecord.m_uiThrow = uiThrowV3DIndex;
	CollapseRecord.m_MapIndex = uiIndex0 * 2 + uiMapInde;
	CollapseRecord.m_uiKeep = uiEdgeV;
	CollapseRecord.m_uiLeftEdgeNum = ms_uiLeftLineNum;
	ms_pCRL->m_RecordInfo.AddElement(CollapseRecord);

	ms_V3Attr[uiEdgeV].m_fWeight = GetCollapseWeight(ms_V3Attr[uiEdgeV]);
	ms_V3Attr[uiEdgeV1].m_fWeight = GetCollapseWeight(ms_V3Attr[uiEdgeV1]);
	return 1;
}