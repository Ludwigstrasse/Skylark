#ifndef SKCONTROLLER_H
#define SKCONTROLLER_H
#include "SKObject.h"
#include "SKTimeEvent.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKStartAnimEvent : public SKTimeEvent
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKStartAnimEvent();
		~SKStartAnimEvent();
		SKStartAnimEvent(SKController *pController);
		
	};
	DECLARE_Ptr(SKStartAnimEvent);
	SKTYPE_MARCO(SKStartAnimEvent);

	class SKGRAPHIC_API SKEndAnimEvent : public SKTimeEvent
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKEndAnimEvent();
		~SKEndAnimEvent();
		SKEndAnimEvent(SKController *pController);

	};
	DECLARE_Ptr(SKEndAnimEvent);
	SKTYPE_MARCO(SKEndAnimEvent);

	class SKGRAPHIC_API SKController : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~SKController() = 0;
		FORCEINLINE SKObject * GetObject()const;
		virtual bool Update(double dAppTime);
		virtual bool UpdateEx(double aAppTime);

		enum // Repeat Type
		{
			RT_NONE,
			RT_CLAMP,
			RT_WRAP,
			RT_CYCLE,
			RT_REVEERSE_CLAMP,
			RT_REVEERSE_WRAP,
			RT_REVEERSE_CYCLE,
			RT_MAX
		};
		unsigned int m_uiRepeatType;

		double		m_dMinTime;
		double		m_dMaxTime;
		double		m_dPhase;
		double		m_dFrequency;
		
		static double GetTime(double fTime, double fMin,double fMax,unsigned int uiRepeatType);
	public:
		friend class SKObject;
		SKController();
		virtual bool SetObject(SKObject * pObject);
		double	GetControlTime(double dAppTime);

		SKObject* m_pObject;
		double	m_dNowAnimTime;
		double	m_dLastAppTime;
		double	m_dLastAnimTime;
		double  m_dIntervalTime;
		double  m_dIntervalAnimTime;
		double	m_dTimeSum;
		double	m_dStartSystemTime;
		bool	m_bStart;

		FORCEINLINE void ClearTime();
		void AddTimeEvent(SKTimeEvent * pTimeEvent);
		void DeleteTimeEvent(SKTimeEvent * pTimeEvent);
		void SetEnable(bool Enable);

		void AddTriggerStart(TriggerAnimEventType::Handler handler);

		void AddTriggerStop(TriggerAnimEventType::Handler handler);

		void AddTriggerEnd(TriggerAnimEventType::Handler handler);

		void AddTriggerBeginStart(TriggerAnimEventType::Handler handler);
	public:
		
		bool m_bIsVisibleUpdate;
	protected:
		SKArray<SKTimeEventPtr> m_TimeEventArray;
		void TimeEvent(double dAppTime);
		SKREAL m_fInnerTimeScale;
		bool m_bEnable;
		TriggerAnimEventType m_TriggerBeginStart;
		TriggerAnimEventType m_TriggerStop;
	};
#include "SKController.inl"
	DECLARE_Ptr(SKController);
	SKTYPE_MARCO(SKController);

}
#endif