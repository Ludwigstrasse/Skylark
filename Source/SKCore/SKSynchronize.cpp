#include "SKSynchronize.h"
using namespace SKEngine2;
SKCriticalSection g_SafeCS;
void SKSynchronize::SKSafeOutPutDebugString(const TCHAR * pcString, ...)
{
	g_SafeCS.Lock();
	char *pArgs;
	pArgs = (char*) &pcString + sizeof(pcString);
	_vstprintf_s(SKSystem::ms_sLogBuffer, LOG_BUFFER_SIZE,pcString, pArgs) ;
	OutputDebugString(SKSystem::ms_sLogBuffer);
	g_SafeCS.Unlock();
}
unsigned int SKSynchronize::WaitAll(SKSynchronize * * pSynchronize, unsigned int uiNum, bool bWaitAll,DWORD dwMilliseconds)
{
	SKMAC_ASSERT(uiNum >=1 && uiNum <= MAXIMUM_WAIT_OBJECTS);
	static HANDLE handle[MAXIMUM_WAIT_OBJECTS];
	for (unsigned int i = 0 ; i < uiNum ; i++)
	{
		handle[i] = (HANDLE)pSynchronize[i]->GetHandle();
	}
	DWORD dw = WaitForMultipleObjects(uiNum,handle,bWaitAll,dwMilliseconds);
	switch(dw)
	{
	case WAIT_FAILED:
		return WF_FAILED;
	case WAIT_TIMEOUT:
		return WF_TIMEOUT;
	case WAIT_OBJECT_0:
		return WF_OBJECT0;

	}
	return WF_FAILED;
}
SKEvent::SKEvent(void)
{
	Event = NULL;
}

/**
* Cleans up the event handle if valid
*/
SKEvent::~SKEvent(void)
{
	if (Event != NULL)
	{
		CloseHandle(Event);
	}
}

/**
* Waits for the event to be signaled before returning
*/
void SKEvent::Lock(void)
{
	WaitForSingleObject(Event,INFINITE);
}

/**
* Triggers the event so any waiting threads are allowed access
*/
void SKEvent::Unlock(void)
{
	PulseEvent(Event);
}

/**
* Creates the event. Manually reset events stay triggered until reset.
* Named events share the same underlying event.
*
* @param bIsManualReset Whether the event requires manual reseting or not
* @param InName Whether to use a commonly shared event or not. If so this
* is the name of the event to share.
*
* @return Returns TRUE if the event was created, FALSE otherwise
*/
bool SKEvent::Create(bool bIsManualReset,const TCHAR* InName)
{
	// Create the event and default it to non-signaled

	Event = CreateEvent(NULL,bIsManualReset,0,InName);
	return Event != NULL;
}

/**
* Triggers the event so any waiting threads are activated
*/
void SKEvent::Trigger(void)
{
	SetEvent(Event);
}

/**
* Resets the event to an untriggered (waitable) state
*/
void SKEvent::Reset(void)
{
	ResetEvent(Event);
}

/**
* Triggers the event and resets the triggered state NOTE: This behaves
* differently for auto-reset versus manual reset events. All threads
* are released for manual reset events and only one is for auto reset
*/
void SKEvent::Pulse(void)
{
	PulseEvent(Event);
}

/**
* Waits for the event to be triggered
*
* @param WaitTime Time in milliseconds to wait before abandoning the event
* (DWORD)-1 is treated as wait infinite
*
* @return TRUE if the event was signaled, FALSE if the wait timed out
*/
bool SKEvent::Wait(DWORD WaitTime)
{
	return WaitForSingleObject(Event,WaitTime) == WAIT_OBJECT_0;
}
bool SKEvent::IsTrigger()
{
	return Wait(0);
}
SKMutex::SKMutex()
{
	m_Mutex = CreateMutex(NULL, FALSE, NULL);
	SKMAC_ASSERT(m_Mutex);
}
//----------------------------------------------------------------------------
SKMutex::~SKMutex()
{
	BOOL closed = CloseHandle((HANDLE)m_Mutex);
	m_Mutex = NULL;
	SKMAC_ASSERT(closed);
}
//----------------------------------------------------------------------------
void SKMutex::Enter ()
{
	DWORD result = WaitForSingleObject((HANDLE)m_Mutex, INFINITE);
	SKMAC_ASSERT(result != WAIT_FAILED);
	// result:
	//   WAIT_ABANDONED (0x00000080)
	//   WAIT_OBJECT_0  (0x00000000), signaled
	//   WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
	//   WAIT_FAILED    (0xFFFFFFFF), not signaled
}
//----------------------------------------------------------------------------
void SKMutex::Leave ()
{
	BOOL released = ReleaseMutex((HANDLE)m_Mutex);
	SKMAC_ASSERT(released);
}

SKSemaphore::SKSemaphore(unsigned int uiCount,unsigned int MaxCount)
{
	SKMAC_ASSERT(uiCount <= MaxCount);
	Semaphore = CreateSemaphore(NULL,uiCount,MaxCount,NULL);
	m_uiMaxCount =MaxCount;
	SKMAC_ASSERT(Semaphore);
}
//----------------------------------------------------------------------------
SKSemaphore::~SKSemaphore()
{
	BOOL closed = CloseHandle((HANDLE)Semaphore);
	Semaphore = NULL;
	SKMAC_ASSERT(closed);
}
//----------------------------------------------------------------------------
void SKSemaphore::Enter ()
{
	DWORD result = WaitForSingleObject((HANDLE)Semaphore, INFINITE);
	SKMAC_ASSERT(result);
	// result:
	//   WAIT_ABANDONED (0x00000080)
	//   WAIT_OBJECT_0  (0x00000000), signaled
	//   WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
	//   WAIT_FAILED    (0xFFFFFFFF), not signaled
}
//----------------------------------------------------------------------------
void SKSemaphore::Leave (unsigned int uiReleaseCount)
{
	BOOL released = ReleaseSemaphore((HANDLE)Semaphore,uiReleaseCount,NULL);
	SKMAC_ASSERT(released);
}