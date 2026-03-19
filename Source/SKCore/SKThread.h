#ifndef SKTHREAD_H
#define SKTHREAD_H
//------------------------------------------------------------------------------
/**
@class	MAY::SKThread

@brief	̻߳.
ʵRun(), ͨIsStopRequested()ж߳Ƿ.
ҪVSʱʾ߳, GetThreadName().
*/
#include "SKSystem.h"
#include "SKSynchronize.h"
namespace SKEngine2
{

	//ֻһ߳̿߳࣬ṵ̂߳ȫԣ2߳ͬʱ߳Щط
	class SKSYSTEM_API SKThread
	{
	public:
		enum Priority
		{
			Low,
			Normal,
			High,
		};
		enum ThreadState
		{
			TS_START,
			TS_SUSPEND,
			TS_STOP,
		};
	public:
		SKThread();
		virtual ~SKThread();
		
		void SetPriority(Priority p);
		
		Priority GetPriority() const;
		
		void SetStackSize(unsigned int uiSize);
		
		unsigned int GetStackSize() const;


		void Start();

		void Suspend();

		bool IsRunning() const;
		void Sleep(DWORD dwMillseconds);

		bool IsStopTrigger();
		void Stop();
	public:
		
		static void SetThreadName(const char* name);
		FORCEINLINE ThreadState GetThreadState()
		{
			return m_ThreadState;
		}
	protected:
		virtual void Run() = 0;
		virtual const TCHAR* GetThreadName();
	private:
		static DWORD THREAD_CALLBACK ThreadProc(void* t);
	private:
		void* m_hThread;
		Priority m_priority;
		unsigned int m_stackSize;
	protected:
		ThreadState m_ThreadState;
		SKEvent m_StopEvent;
	};
}
#endif
