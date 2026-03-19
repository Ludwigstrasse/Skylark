#include "SKDLodTerrainNode.h"
#include "SKDLodTerrainGeometry.h"
#include "SKDLodTerrainSwitchNode.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDLodTerrainNode,SKTerrainNode)
BEGIN_ADD_PROPERTY(SKDLodTerrainNode,SKTerrainNode)
REGISTER_PROPERTY(m_uiDLodExtend, DLodExtend, SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fDLodScale, DLodScale, SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDLodTerrainNode)
IMPLEMENT_INITIAL_END
SKDLodTerrainNode::SKDLodTerrainNode()
{
	m_uiDLodExtend = 500;
	m_fDLodScale = 50000000.0f;
}

SKDLodTerrainNode::~SKDLodTerrainNode()
{
}
bool SKDLodTerrainNode::CreateChild()
{
	m_pChild.Clear();
	unsigned int uiChildNumX = GetChildNumX();
	unsigned int uiChildNumZ = GetChildNumZ();
	for (unsigned int i = 0 ; i < uiChildNumX ; i++)
	{
		for ( unsigned int j = 0 ; j < uiChildNumZ ; j++)
		{

			SKDLodTerrainSwitchNode * pDTS = NULL;
			pDTS = SK_NEW SKDLodTerrainSwitchNode(i,j);
			if (!pDTS)
			{
				return 0;
			}
			AddChild(pDTS);
			for (unsigned int k = 0 ; k < m_uiTesselationLevel ; k++)
			{
				SKDLodTerrainGeometry * pChild = NULL;
				pChild = SK_NEW SKDLodTerrainGeometry();
				if (!pChild)
				{
					return 0;
				}
				pDTS->AddChild(pChild);
				pChild->CreateMesh(i,j,k,m_uiTesselationLevel);

				pChild->AddMaterialInstance(SKResourceManager::ms_DefaultOnlyColorMaterialResource);
				SKREAL green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
				pChild->GetMaterialInstance(0)->SetPShaderValue(_T("EmissiveColor"), green, 4);
			}

		}
	}

	return 1;
}

void SKDLodTerrainNode::UpdateNodeAll(double dAppTime)
{
	
	SKTerrainNode::UpdateNodeAll(dAppTime);

}