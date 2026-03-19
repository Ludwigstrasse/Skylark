#include "SKStaticMeshNode.h"
#include "SKGraphicInclude.h"
#include "SKModelSwitchNode.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKStaticMeshNode,SKModelMeshNode)
BEGIN_ADD_PROPERTY(SKStaticMeshNode,SKModelMeshNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKStaticMeshNode)
ADD_PRIORITY(SKGeometry)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
SKStaticMeshNodePtr SKStaticMeshNode::Default = NULL;
bool SKStaticMeshNode::ms_bIsEnableASYNLoader = true;
bool SKStaticMeshNode::ms_bIsEnableGC = true;
SKStaticMeshNode::SKStaticMeshNode()
{
}
SKStaticMeshNode::~SKStaticMeshNode()
{

}
bool SKStaticMeshNode::InitialDefaultState()
{
	Default = SK_NEW SKStaticMeshNode();
	SKGeometryNodePtr GeometryNode = SK_NEW SKGeometryNode();
	Default->AddChild(GeometryNode);
	SKGeometryPtr Geometry = (SKGeometry *)SKObject::CloneCreateObject(SKGeometry::GetDefaultRenderCube());
	GeometryNode->AddChild(Geometry);
	Default->CreateLocalAABB();
	GeometryNode->SetLocalScale(SKVector3(100.0f,100.0f,100.0f));
	
	return true;
}
bool SKStaticMeshNode::TerminalDefaultState()
{
	Default = NULL;
	return true;
}
void SKStaticMeshNode::AddLodMesh(SKStaticMeshNodeR * pStaticMeshResource)
{
	if (pStaticMeshResource)
	{
		SKModelSwitchNode* LodNode = DynamicCast<SKModelSwitchNode>(m_pChild[0]);
		if (!LodNode)
		{
			LodNode = SK_NEW SKModelSwitchNode();
			SKSpatialPtr GeoNode = m_pChild[0];	
			DeleteAllChild();
			LodNode->AddChild(GeoNode);
			AddChild(LodNode);
		}
		while (!pStaticMeshResource->IsLoaded())
		{
		}
		SKGeometryNode * pGeoNode = pStaticMeshResource->GetResource()->GetGeometryNode(0);
		LodNode->AddChild(pGeoNode);
	}
}
void SKStaticMeshNode::SetLodMesh(unsigned int i, SKStaticMeshNodeR * pStaticMeshResource)
{
	if (pStaticMeshResource)
	{
		SKModelSwitchNode* LodNode = DynamicCast<SKModelSwitchNode>(m_pChild[0]);
		if (!LodNode)
		{
			return;
		}
		if (i >= LodNode->GetNodeNum())
		{
			return;
		}
		while (!pStaticMeshResource->IsLoaded())
		{
		}
		SKGeometryNode * pGeoNode = pStaticMeshResource->GetResource()->GetGeometryNode(0);
		(*LodNode->GetChildList())[i] = pGeoNode;
	}
}
void SKStaticMeshNode::DeleteLodMesh(unsigned int i)
{
	SKModelSwitchNode* LodNode = DynamicCast<SKModelSwitchNode>(m_pChild[0]);
	if (!LodNode)
	{
		return;
	}
	if (i >= LodNode->GetNodeNum())
	{
		return;
	}
	LodNode->GetChildList()->Erase(i);
}