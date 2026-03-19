#include "SKTerrainActor.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKCLodTerrainNode.h"
#include "SKDLodTerrainNode.h"
using namespace SKEngine2;

IMPLEMENT_RTTI_NoCreateFun(SKTerrainActor,SKActor)
BEGIN_ADD_PROPERTY(SKTerrainActor,SKActor)
ADD_PROPERTY_FLAG(pNode,SKProperty::F_NO_USE_GC)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKTerrainActor)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKTerrainActor::SKTerrainActor()
{

}
SKTerrainActor::~SKTerrainActor()
{

}
IMPLEMENT_RTTI(SKCLodTerrainActor,SKTerrainActor)
BEGIN_ADD_PROPERTY(SKCLodTerrainActor,SKTerrainActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCLodTerrainActor)
IMPLEMENT_INITIAL_END
SKCLodTerrainActor::SKCLodTerrainActor()
{
	
}
SKCLodTerrainActor::~SKCLodTerrainActor()
{

}
void SKCLodTerrainActor::CreateDefaultComponentNode()
{
	m_pNode = SK_NEW SKCLodTerrainNode();
	GetTypeNode()->CreateTerrain(10, 10, 7);
}

IMPLEMENT_RTTI(SKDLodTerrainActor,SKTerrainActor)
BEGIN_ADD_PROPERTY(SKDLodTerrainActor,SKTerrainActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDLodTerrainActor)
IMPLEMENT_INITIAL_END
SKDLodTerrainActor::SKDLodTerrainActor()
{
}
SKDLodTerrainActor::~SKDLodTerrainActor()
{

}
void SKDLodTerrainActor::CreateDefaultComponentNode()
{
	m_pNode = SK_NEW SKDLodTerrainNode();
	GetTypeNode()->CreateTerrain(10, 10, 7);
}