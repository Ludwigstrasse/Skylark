#include "SKStaticActor.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKStaticActor,SKActor)
BEGIN_ADD_PROPERTY(SKStaticActor,SKActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKStaticActor)
IMPLEMENT_INITIAL_END
SKStaticActor::SKStaticActor()
{
	
}
SKStaticActor::~SKStaticActor()
{

}
void SKStaticActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKStaticMeshComponent>();
}
