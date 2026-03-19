#include "SKTimeEvent.h"
#include "SKStream.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKTimeEvent,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKTimeEvent)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKTimeEvent,SKObject)
REGISTER_PROPERTY(m_bEnable,Enable,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pController,Controller,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fTriggerPercent,TriggerPercent,SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKTimeEvent::SKTimeEvent()
{
	m_bEnable = true;
	m_pController = NULL;
	m_fTriggerPercent = 0.0f;
}
SKTimeEvent::~SKTimeEvent()
{
	m_pController = NULL;
}
SKTimeEvent::SKTimeEvent(SKController *pController)
{
	m_pController = pController;
}
void SKTimeEvent::Trigger()
{
	m_TriggerAnimEvent();
}