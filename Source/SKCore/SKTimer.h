#ifndef SKTIMER_H
#define SKTIMER_H
#include "SKSystem.h"
#include "SKMath.h"
namespace SKEngine2
{
class SKSYSTEM_API SKTimer
{
private:
	bool m_bUseLargeTime;   //ʹôʱ־
	 __int64 m_int64OneSecondTicks;  //һڵĵδ
	 __int64 m_int64TimeTickStartCounts;  //ʼĵδֵ
	unsigned long m_ulTimeStart;  //timeGetTimeʼʱ
	int m_iFrameCount;
	double m_fFPS;
	double m_fTime,m_fLastFPSTime,m_fTimeSlice;
	double m_fDetTime, m_fLastTime;
	void InitGameTime();
public:
	SKTimer();
	~SKTimer();
	
	double GetGamePlayTime();
	void UpdateFPS();
	FORCEINLINE double GetFPS(){return m_fFPS;}
	static SKTimer * ms_pTimer;
	double GetDetTime(){ return m_fDetTime; }
	int GetRandSeed();
};
}
#endif