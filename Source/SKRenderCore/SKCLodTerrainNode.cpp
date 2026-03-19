#include "SKCLodTerrainNode.h"
#include "SKCLodTerrainGeometry.h"
#include "SKRoamTerrainGemotry.h"
#include "SKQuadTerrainGeometry.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKCLodTerrainNode,SKTerrainNode)
BEGIN_ADD_PROPERTY(SKCLodTerrainNode,SKTerrainNode)
REGISTER_PROPERTY(m_uiTerrainNodeType,TerrainNodeType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fCLODScale, CLODScale, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCLodTerrainNode)
IMPLEMENT_INITIAL_END
SKCLodTerrainNode::SKCLodTerrainNode()
{
	
	m_fCLODScale = 50000000.0f;

	m_uiTerrainNodeType = TNT_ROAM;
}
void SKCLodTerrainNode::SetTerrainNodeType(unsigned int uiTerrainNodeType)
{
	if (uiTerrainNodeType < TNT_MAX)
	{
		m_uiTerrainNodeType = uiTerrainNodeType;
	}
	else
	{
		m_uiTerrainNodeType = TNT_ROAM;
	}
}
SKCLodTerrainNode::~SKCLodTerrainNode()
{

}
unsigned int SKCLodTerrainNode::AddChild(SKSpatial * pChild)
{
	if (DynamicCast<SKCLodTerrainGeometry>(pChild) == NULL)
	{
		return SKMAX_INTEGER;
	}
	return SKTerrainNode::AddChild(pChild);
}

bool SKCLodTerrainNode::CreateChild()
{
	m_pChild.Clear();
	unsigned int uiChildNumX = GetChildNumX();
	unsigned int uiChildNumZ = GetChildNumZ();
	for (unsigned int i = 0 ; i < uiChildNumX ; i++)
	{
		for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
		{
			SKCLodTerrainGeometry * pTerrainGeo = NULL;
			if (m_uiTerrainNodeType == TNT_ROAM)
			{
				pTerrainGeo = SK_NEW SKRoamTerrainGeometry();
			}
			else if (m_uiTerrainNodeType == TNT_QUAD)
			{
				pTerrainGeo = SK_NEW SKQuadTerrainGeometry();
			}

			if (!pTerrainGeo)
			{
				return 0;
			}
			AddChild(pTerrainGeo);
			if (!pTerrainGeo->CreateMeshDate(i,j,m_uiTesselationLevel))
			{
				return 0;
			}
			pTerrainGeo->AddMaterialInstance(SKResourceManager::ms_DefaultOnlyColorMaterialResource);
			SKREAL green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			pTerrainGeo->GetMaterialInstance(0)->SetPShaderValue(_T("EmissiveColor"), green, 4);
			

		}
	}

	LinkNeighbor();
	ComputeVariance();
	return 1;
}


void SKCLodTerrainNode::ComputeVariance()
{
	SKCLodTerrainGeometry * pTerrainGeo = NULL;
	unsigned int uiCurIndex;
	unsigned int uiChildNumX = GetChildNumX();
	unsigned int uiChildNumZ = GetChildNumZ();
	if (m_uiTerrainNodeType == TNT_ROAM)
	{
		for (unsigned int i = 0 ; i < uiChildNumX ; i++)
		{
			for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
			{

				uiCurIndex = i * uiChildNumZ + j;
				pTerrainGeo = (SKCLodTerrainGeometry *)(GetChild(uiCurIndex));		
				pTerrainGeo->ComputeVariance();
			}
		}
	}
	else if(m_uiTerrainNodeType == TNT_QUAD)
	{
		for (int uiLevel = m_uiTesselationLevel - 1 ; uiLevel >= 0 ; uiLevel--)
		{
			for (unsigned int i = 0 ; i < uiChildNumX ; i++)
			{
				for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
				{

					uiCurIndex = i * uiChildNumZ + j;
					pTerrainGeo = (SKCLodTerrainGeometry *)(GetChild(uiCurIndex));		
					pTerrainGeo->ComputeVariance(uiLevel);
				}
			}
		}
		
		
	}
	
}
void SKCLodTerrainNode::LinkNeighbor()
{
	unsigned int uiLeft;
	unsigned int uiRight;
	unsigned int uiTop;
	unsigned int uiBottom;
	unsigned int uiChildNumX = GetChildNumX();
	unsigned int uiChildNumZ = GetChildNumZ();
	unsigned int uiCurIndex;
	SKCLodTerrainGeometry * pTerrainGeo = NULL;
	for (unsigned int i = 0 ; i < uiChildNumX ; i++)
	{
		for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
		{
			
			uiCurIndex = i * uiChildNumZ + j;
			if (i == 0)
			{
				uiLeft = SKMAX_INTEGER;

			}
			else
			{
				uiLeft = (i - 1) * uiChildNumZ + j;

			}

			if (i == uiChildNumX - 1)
			{
				uiRight = SKMAX_INTEGER;

			}
			else
			{
				uiRight = (i + 1) * uiChildNumZ + j;

			}

			if (j == 0)
			{
				uiBottom = SKMAX_INTEGER;

			}
			else
			{
				uiBottom = i * uiChildNumZ + j - 1;

			}

			if (j == uiChildNumZ - 1)
			{
				uiTop = SKMAX_INTEGER;
			}
			else
			{
				uiTop = i * uiChildNumZ + j + 1;
			}

			 
			pTerrainGeo = (SKCLodTerrainGeometry *)(GetChild(uiCurIndex));
			if (uiTop != SKMAX_INTEGER)
			{
				pTerrainGeo->AddNeighbor((SKCLodTerrainGeometry *)GetChild(uiTop),SKCLodTerrainGeometry::NT_TOP);
			}
			
			if (uiBottom != SKMAX_INTEGER)
			{
				pTerrainGeo->AddNeighbor((SKCLodTerrainGeometry *)GetChild(uiBottom),SKCLodTerrainGeometry::NT_BOTTOM);
			}

			if (uiLeft != SKMAX_INTEGER)
			{
				pTerrainGeo->AddNeighbor((SKCLodTerrainGeometry *)GetChild(uiLeft),SKCLodTerrainGeometry::NT_LEFT);
			}

			if (uiRight != SKMAX_INTEGER)
			{
				pTerrainGeo->AddNeighbor((SKCLodTerrainGeometry *)GetChild(uiRight),SKCLodTerrainGeometry::NT_RIGHT);
			}
			pTerrainGeo->LinkNeighbor();
		}
	}

}
void SKCLodTerrainNode::SetCLODScale(SKREAL fCLODScale)
{
	if (fCLODScale <= 0.0f)
	{
		return ;
	}
	m_fCLODScale = fCLODScale;
}


void SKCLodTerrainNode::UpdateNodeAll(double dAppTime)
{
	SKTerrainNode::UpdateNodeAll(dAppTime);
	if (dAppTime > 0.0f)
	{
		for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			SKSpatial * pChild = m_pChild[i];
			((SKCLodTerrainGeometry *)pChild)->ClearInfo();
		}
	}
}

