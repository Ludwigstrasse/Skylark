#include "SKSkelectonActor.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSkelectonActor,SKActor)
BEGIN_ADD_PROPERTY(SKSkelectonActor,SKActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSkelectonActor)
IMPLEMENT_INITIAL_END
SKSkelectonActor::SKSkelectonActor()
{

}
SKSkelectonActor::~SKSkelectonActor()
{

}
void SKSkelectonActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKSkelectonMeshComponent>();
}
