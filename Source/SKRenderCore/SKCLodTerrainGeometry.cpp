#include "SKCLodTerrainGeometry.h"
#include "SKTerrainNode.h"
#include "SKTriangleSet.h"
#include "SKVertexBuffer.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKCLodTerrainGeometry,SKGeometry)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKCLodTerrainGeometry)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKCLodTerrainGeometry,SKGeometry)
REGISTER_PROPERTY(m_pNeighbor[NT_LEFT],NeighborLeft,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_RIGHT],NeighborRight,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_TOP],NeighborTFop,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_BOTTOM],NeighborBottom,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiIndexXInTerrain,IndexXInTerrain,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiIndexZInTerrain,IndexZInTerrain,SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKCLodTerrainGeometry::SKCLodTerrainGeometry()
{
	for (unsigned int i = 0 ; i < NT_MAX ; i++)
	{
		m_pNeighbor[i] = NULL;
	}
	m_uiIndexXInTerrain = 0;
	m_uiIndexZInTerrain = 0;
	m_bIsStatic = false;
	
}
SKCLodTerrainGeometry::~SKCLodTerrainGeometry()
{

}
bool SKCLodTerrainGeometry::CreateMeshDate(unsigned int uiIndexXInTerrain, unsigned int uiIndexZInTerrain,
									   unsigned int uiTesselationLevel)
{

	if (uiTesselationLevel < 0)
	{
		return 0 ;
	}
	SKTerrainNode * pTerrainNode = DynamicCast<SKTerrainNode>(m_pParent);
	if (!pTerrainNode)
	{
		return 0 ;
	}

	SKDataBuffer * pVertexDate = NULL ;
	pVertexDate = SK_NEW SKDataBuffer();
	unsigned int uiVertexLenght= (1 << uiTesselationLevel) + 1;
	if (!pVertexDate->CreateEmptyBuffer(uiVertexLenght * uiVertexLenght,SKDataBuffer::DT_FLOAT32_3))
	{
		return 0;
	}
	

	SKVector3 * pVer = (SKVector3 *)pVertexDate->GetDate();
	unsigned int uiBiasX = uiIndexXInTerrain * (1 << uiTesselationLevel);
	unsigned int uiBiasZ = uiIndexZInTerrain * (1 << uiTesselationLevel);
	for (unsigned int i = 0 ; i < uiVertexLenght ; i++)
	{
		for(unsigned int j = 0 ; j < uiVertexLenght ; j++)
		{
			unsigned int uiIndex = i * uiVertexLenght + j;
			pVer[uiIndex].x = 1.0f * (uiBiasX + i) * SKTerrainNode::WIDTH_SCALE;
			pVer[uiIndex].y = pTerrainNode->GetHeight(uiBiasX + i,uiBiasZ + j);
			pVer[uiIndex].z = 1.0f * (uiBiasZ + j) * SKTerrainNode::WIDTH_SCALE;
		}
	}
	
	SKVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = SK_NEW SKVertexBuffer(true);
	if (!pVertexBuffer)
	{
		return 0;
	}
	pVertexBuffer->SetDate(pVertexDate,SKVertexFormat::VF_POSITION);


 	unsigned int uiCurRenderTriNum = (uiVertexLenght - 1) * (uiVertexLenght - 1) * 2 * 3;

	SKIndexBuffer * pIndexBuffer = NULL;
	pIndexBuffer = SK_NEW SKIndexBuffer(uiCurRenderTriNum);
	if (!pIndexBuffer)
	{
		return 0;
	}
	//test mul thread render
	if (GetTerrainGeometryType() == TGT_ROAM)
	{
		pIndexBuffer->SetStatic(false,true);
	}
	else
	{
		pIndexBuffer->SetStatic(false);
	}
	
	pIndexBuffer->SetLockFlag(SKBind::LF_DISCARD);

	SKTriangleSet * pMeshDate = NULL;
	pMeshDate = SK_NEW SKTriangleSet();

	if (!pMeshDate)
	{
		return 0;
	}

	pMeshDate->SetVertexBuffer(pVertexBuffer);
	pMeshDate->SetIndexBuffer(pIndexBuffer);
	SetMeshDate(pMeshDate);
	m_uiIndexXInTerrain = uiIndexXInTerrain;
	m_uiIndexZInTerrain = uiIndexZInTerrain;
	return 1;
}
bool SKCLodTerrainGeometry::AddNeighbor(SKCLodTerrainGeometry * pTerrainGemetry,unsigned int uiNeighbor)
{
	if ( !pTerrainGemetry || uiNeighbor >= NT_MAX)
	{
		return 0;
	}
	SKTerrainNode * pTerrainNode = DynamicCast<SKTerrainNode>(m_pParent);
	if (!pTerrainNode)
	{
		return 0 ;
	}
	
	pTerrainNode->DeleteChild(m_pNeighbor[uiNeighbor]);
	m_pNeighbor[uiNeighbor] = pTerrainGemetry;

	return 1;
}
void SKCLodTerrainGeometry::UpDateView(SKCuller & Culler,double dAppTime)
{
	SKGeometry::UpDateView(Culler,dAppTime);
	SKCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == SKCuller::CUT_MAIN)
	{
		SKTerrainNode * pTerrainNode = DynamicCast<SKTerrainNode>(m_pParent);
		if (!pTerrainNode)
		{
			return ;
		}
		SKTransform Tran = pTerrainNode->GetWorldTransform();
		SKVector3 Loc = pCamera->GetWorldTranslate() * Tran.GetCombineInverse();


		Tessellate(Loc);
	}
}

