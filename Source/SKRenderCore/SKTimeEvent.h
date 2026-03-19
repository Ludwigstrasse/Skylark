#ifndef SKTIMEEVENT_H
#define SKTIMEEVENT_H
#include "SKObject.h"
#include "SKDelegateList.h"
namespace SKEngine2
{
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef SKDelegateEvent<void(void)> TriggerAnimEventType;
#else
	typedef SKDelegateEvent0<void> TriggerAnimEventType;
#endif
	class SKStream;
	class SKController;

	class SKGRAPHIC_API SKTimeEvent : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~SKTimeEvent() = 0;
		FORCEINLINE SKController * GetObject()const;

		SKTimeEvent();
		SKTimeEvent(SKController *pController);
		virtual void Trigger();
	public:
		bool m_bEnable;
		SKREAL m_fTriggerPercent;
		TriggerAnimEventType m_TriggerAnimEvent;
	protected:
		SKController * m_pController;
		
	};
	DECLARE_Ptr(SKTimeEvent);
	SKTYPE_MARCO(SKTimeEvent);

}
#endif