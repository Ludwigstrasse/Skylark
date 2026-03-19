#include "SKThread.h"
using namespace SKEngine2;


//------------------------------------------------------------------------------
DWORD THREAD_CALLBACK SKThread::ThreadProc(void* t)
{
	SKThread* pThread = (SKThread*)(t);
	SetThreadName(pThread->GetThreadName());
	pThread->Run();
	return 0;
}

//------------------------------------------------------------------------------
SKThread::SKThread()
	: m_hThread(NULL)
	, m_priority(Normal)
	, m_stackSize(0)
{
	SKMAC_ASSERT(!IsRunning());
	m_hThread = ::CreateThread(0, m_stackSize, ThreadProc, this, CREATE_SUSPENDED, NULL);
	SKMAC_ASSERT(m_hThread);
	m_ThreadState = TS_SUSPEND;
	SetPriority(m_priority);
	m_StopEvent.Create(true);
	m_StopEvent.Reset();
}

//------------------------------------------------------------------------------
SKThread::~SKThread()
{
	if (IsRunning())
	{
		// force to exit
		TerminateThread(m_hThread, 0);
	}
}

//------------------------------------------------------------------------------
void SKThread::SetPriority(Priority p)
{
	int nPriority = THREAD_PRIORITY_NORMAL;

	if (p == Low)
		nPriority = THREAD_PRIORITY_BELOW_NORMAL;
	else if (p == Normal)
		nPriority = THREAD_PRIORITY_NORMAL;
	else if (p == High)
		nPriority = THREAD_PRIORITY_ABOVE_NORMAL;

	::SetThreadPriority(m_hThread, nPriority);
}

//------------------------------------------------------------------------------
void SKThread::Start()
{	
	if (m_ThreadState == TS_SUSPEND)
	{
		ResumeThread((HANDLE)m_hThread);
		m_ThreadState = TS_START;
	}
}
//------------------------------------------------------------------------------
void SKThread::Suspend()
{
	if (m_ThreadState == TS_START)
	{
		SuspendThread((HANDLE)m_hThread);
		m_ThreadState = TS_SUSPEND;
	}
	
}
void SKThread::Sleep(DWORD dwMillseconds)
{
	if (m_ThreadState == TS_START)
	{
		::Sleep(dwMillseconds);
	}
}
//------------------------------------------------------------------------------
bool SKThread::IsRunning() const
{
	if (NULL != m_hThread)
	{
		DWORD exitCode = 0;
		if (GetExitCodeThread(m_hThread, &exitCode))
		{
			if (STILL_ACTIVE == exitCode)
			{
				return true;
			}
		}
	}

	return false;
}

//------------------------------------------------------------------------------
void SKThread::SetThreadName(const TCHAR* name)
{
	// update the Windows thread name so that it shows up correctly
	// in the Debugger
	struct THREADNAME_INFO
	{
		DWORD dwType;     // must be 0x1000
		LPCSTR szName;    // pointer to name (in user address space)
		DWORD dwThreadID; // thread ID (-1 = caller thread)
		DWORD dwFlags;    // reserved for future use, must be zero
	};

	THREADNAME_INFO info;
	info.dwType     = 0x1000;
	info.szName     = name;
	info.dwThreadID = ::GetCurrentThreadId();
	info.dwFlags    = 0;
	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}

//------------------------------------------------------------------------------
const TCHAR* SKThread::GetThreadName()
{
	return _T("SKThread");
}
bool SKThread::IsStopTrigger()
{
	return m_StopEvent.IsTrigger();
}
//------------------------------------------------------------------------------
void SKThread::Stop()
{
	if (m_ThreadState == TS_START)
	{
		SKMAC_ASSERT(this->IsRunning());
		SKMAC_ASSERT(NULL != m_hThread);

		m_StopEvent.Trigger();
		m_ThreadState = TS_STOP;
		// wait for the thread to terminate
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}