#include "SKSocketNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSocketNode, SKSpatial)
BEGIN_ADD_PROPERTY(SKSocketNode, SKSpatial)
REGISTER_PROPERTY(m_cName, Name, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSocketNode)
IMPLEMENT_INITIAL_END
SKSocketNode::SKSocketNode()
{
	m_bIsStatic = false;
}
SKSocketNode::~SKSocketNode()
{

}
void SKSocketNode::UpdateNodeAll(double dAppTime)
{
	UpdateTransform(dAppTime);
}