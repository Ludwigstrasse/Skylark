#include "SKSkyLight.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSkyLight,SKIndirectLight)
BEGIN_ADD_PROPERTY(SKSkyLight,SKIndirectLight)
REGISTER_PROPERTY(m_UpColor, UpColor, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_DownColor, DownColor, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSkyLight)
IMPLEMENT_INITIAL_END
SKSkyLight::SKSkyLight()
{
	m_UpColor = SKColorRGBA(1.0f,1.0f,1.0f,1.0f);
	m_DownColor = SKColorRGBA(1.0f,1.0f,1.0f,1.0f);
}
SKSkyLight::~SKSkyLight()
{
}