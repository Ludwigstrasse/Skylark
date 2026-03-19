#include "SKDLodTerrainGeometry.h"
#include "SKDLodTerrainNode.h"
#include "SKTriangleSet.h"
#include "SKVertexBuffer.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDLodTerrainGeometry,SKGeometry)
BEGIN_ADD_PROPERTY(SKDLodTerrainGeometry,SKGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDLodTerrainGeometry)
IMPLEMENT_INITIAL_END
SKDLodTerrainGeometry::SKDLodTerrainGeometry()
{

}
SKDLodTerrainGeometry::~SKDLodTerrainGeometry()
{

}
SKDLodTerrainNode * SKDLodTerrainGeometry::GetParentTerrainNode()const
{
	if (m_pParent)
	{
		return (SKDLodTerrainNode *)m_pParent->GetParent();
	}
	return NULL;
}
bool SKDLodTerrainGeometry::CreateMesh(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
									   unsigned int uiLevel,unsigned int uiTesselationLevel)
{
	SKDLodTerrainNode * pTerrainNode = GetParentTerrainNode();
	if (uiLevel >= uiTesselationLevel || !pTerrainNode)
	{
		return 0;
	}
	unsigned int uiVertexLenght = (1 << ( uiTesselationLevel - uiLevel) ) + 1;

	SKDataBuffer * pVertexDate = NULL ;
	pVertexDate = SK_NEW SKDataBuffer();
	if (!pVertexDate->CreateEmptyBuffer(uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 4,SKDataBuffer::DT_FLOAT32_3))
	{
		return 0;
	}


	SKVector3 * pVer = (SKVector3 *)pVertexDate->GetDate();
	SKVector3 * pVerTemp = pVer;
	unsigned int uiBiasX = uiIndexXInTerrain * (1 << uiTesselationLevel);
	unsigned int uiBiasZ = uiIndexZInTerrain * (1 << uiTesselationLevel);
	for (unsigned int i = 0 ; i < uiVertexLenght ; i++)
	{
		for(unsigned int j = 0 ; j < uiVertexLenght ; j++)
		{
			unsigned int uiIndex = i * uiVertexLenght + j;
			pVer[uiIndex].x = SKTerrainNode::WIDTH_SCALE * (uiBiasX + (i << uiLevel)) * 1.0f;
			pVer[uiIndex].y = pTerrainNode->GetHeight(uiBiasX + (i << uiLevel),uiBiasZ + (j << uiLevel));
			pVer[uiIndex].z = SKTerrainNode::WIDTH_SCALE * (uiBiasZ + (j << uiLevel)) * 1.0f;
			pVerTemp++;
		}
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * i + uiVertexLenght - 1;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(int i = uiVertexLenght - 1 ; i > 0 ; i--)
	{
		unsigned int uiIndex = uiVertexLenght * (uiVertexLenght - 1 )+ i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(int i = uiVertexLenght - 1 ; i > 0 ; i--)
	{
		unsigned int uiIndex = uiVertexLenght * i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}



	SKVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = SK_NEW SKVertexBuffer(true);
	if (!pVertexBuffer)
	{
		return 0;
	}
	pVertexBuffer->SetDate(pVertexDate,SKVertexFormat::VF_POSITION);

	SKDataBuffer* pIndexDate = NULL;
	pIndexDate = SK_NEW SKDataBuffer();
	if (!pIndexDate)
	{
		return 0;
	}
	if (!pIndexDate->CreateEmptyBuffer(
		(uiVertexLenght - 1) * (uiVertexLenght - 1) * 2 * 3 + (uiVertexLenght - 1) * 4 * 2 * 3,SKDataBuffer::DT_USHORT))
	{
		return 0;
	}
	
	SKUSHORT_INDEX * pIndex = (SKUSHORT_INDEX *)pIndexDate->GetDate();

	for (unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		for (unsigned int j = 0 ; j < uiVertexLenght - 1; j++)
		{
			*pIndex = i * uiVertexLenght + j;
			pIndex++;

			*pIndex = i * uiVertexLenght + j + 1;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j + 1;
			pIndex++;





			*pIndex = i * uiVertexLenght + j;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j + 1;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j;
			pIndex++;






		}
	}



	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = i;
		unsigned int uiIndex1= i + 1;
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 0 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 0 + i + 1;
		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * i + uiVertexLenght - 1;
		unsigned int uiIndex1 = uiVertexLenght * (i + 1) + uiVertexLenght - 1;
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 1 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 1 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * (uiVertexLenght - 1 ) + uiVertexLenght - 1 - i;
		unsigned int uiIndex1 = uiVertexLenght * (uiVertexLenght - 1 )+ uiVertexLenght - 1 - i - 1;
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 2 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 2 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(unsigned int i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		unsigned int uiIndex = uiVertexLenght * (uiVertexLenght - 1 - i);
		unsigned int uiIndex1 = uiVertexLenght * (uiVertexLenght - 1 - i - 1);
		unsigned int uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 3 + i;
		unsigned int uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 3 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		if (i == uiVertexLenght - 2)
		{
			*pIndex = uiVertexLenght * uiVertexLenght;
			pIndex++;
		}
		else
		{
			*pIndex = uiIndex3;
			pIndex++;
		}
	}

	
	SKIndexBuffer * pIndexBuffer = NULL;
	pIndexBuffer = SK_NEW SKIndexBuffer();
	if (!pIndexBuffer)
	{
		return 0;
	}
// 	pIndexBuffer->SetStatic(false);
// 	pIndexBuffer->SetLockFlag(SKBind::LF_DISCARD);
	pIndexBuffer->SetDate(pIndexDate);

	SKTriangleSet * pMeshDate = NULL;
	pMeshDate = SK_NEW SKTriangleSet();

	if (!pMeshDate)
	{
		return 0;
	}
	pMeshDate->SetVertexBuffer(pVertexBuffer);
	pMeshDate->SetIndexBuffer(pIndexBuffer);
	SetMeshDate(pMeshDate);
	return 1;
}