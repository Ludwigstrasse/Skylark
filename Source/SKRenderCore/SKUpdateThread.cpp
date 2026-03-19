#include "SKUpdateThread.h"
using namespace SKEngine2;
SKUpdateContext::SKUpdateContext()
{

}
SKUpdateContext::~SKUpdateContext()
{
	
}
void SKUpdateContext::AddCommand(SKRenderCommand * pCommand)
{
	if (pCommand)
	{
		m_CommandArray.AddElement(pCommand);
	}
}
void SKUpdateContext::Clear()
{
	for (unsigned int i = 0 ; i < m_CommandArray.GetNum() ; i++)
	{
		m_CommandArray[i]->~SKRenderCommand();
	}
	m_CommandArray.Clear();
}
unsigned int SKUpdateContext::Execute()
{
	for (unsigned int i = 0 ; i < m_CommandArray.GetNum() ; i++)
	{
		m_CommandArray[i]->Execute();
	}
	return 1;
}
SKUpdateCenter::SKUpdateCenter(bool bIsMainRun,unsigned int uiThreadNum)
{
	m_uiContextNum = uiThreadNum;
	m_bIsMainRun = bIsMainRun;
	if (m_bIsMainRun)
	{
		m_uiContextNum++;
	}
	m_pContextArray = SK_NEW SKUpdateContext[m_uiContextNum];
	m_uiIndex = -1;
}
SKUpdateCenter::~SKUpdateCenter()
{
	SKMAC_DELETEA(m_pContextArray);
}
unsigned char * SKUpdateCenter::Assign(unsigned int uiSize)
{
	return m_ConstantBuffer.Assign(uiSize);
}
unsigned int SKUpdateCenter::GetIndex()
{
	m_uiIndex++;
	m_uiIndex = m_uiIndex % m_uiContextNum;
	return m_uiIndex;
}
SKUpdateContext * SKUpdateCenter::GetUpdateContext(unsigned int i)
{
	return &m_pContextArray[i];
}
unsigned int SKUpdateCenter::Execute()
{
	if (m_bIsMainRun)
	{
		m_pContextArray[m_uiContextNum - 1].Execute();
	}
	return 1;
}
void SKUpdateCenter::Clear()
{
	
	for (unsigned int i = 0 ; i < m_uiContextNum ; i++)
	{
		m_pContextArray[i].Clear();
	}
	m_ConstantBuffer.Clear();
	m_uiIndex = -1;
}
SKUpdateThread::SKUpdateThread()
{
	static unsigned int s_uiThreadID = 0;
	m_uiThreadId = s_uiThreadID;
	s_uiThreadID++;
	static TCHAR TempString[128];
	SKSprintf(TempString,128,_T("SKUpdateThread%d"),m_uiThreadId);
	m_ThreadName = TempString;
#ifdef EVENT_UPDATE
	SKSprintf(TempString,128,"SKUpdateEvent%d",m_uiThreadId);
	m_Event.Create(true,TempString);
	m_Event.Reset();
#endif
	m_pUpdateContex = NULL;
}

SKUpdateThread::~SKUpdateThread()
{
	m_pUpdateContex = NULL;
}
void SKUpdateThread::Run()
{
	while(!IsStopTrigger())
	{
		if (m_pUpdateContex)
		{
			#ifdef DEBUG_UPDATE_THREAD
				SKSynchronize::SKSafeOutPutDebugString("Execute Begin : %d\n",m_uiThreadId);
			#endif
			m_pUpdateContex->Execute();		
			m_pUpdateContex = NULL;	
			#ifdef DEBUG_UPDATE_THREAD
				SKSynchronize::SKSafeOutPutDebugString("Execute End : %d\n",m_uiThreadId);
			#endif
			#ifdef EVENT_UPDATE
				m_Event.Trigger();
			#endif
			
		}	
	}
}
void SKUpdateThread::SetUpdateContext(SKUpdateContext * pUpdateContex)
{
	m_pUpdateContex = pUpdateContex;
	#ifdef EVENT_UPDATE
		m_Event.Reset();
	#endif
}
#ifdef CONTEXT_UPDATE
bool SKUpdateThread::IsReady()const
{
	return !m_pUpdateContex;
}
#endif
SKUpdateThreadSys * SKUpdateThreadSys::ms_pUpdateThreadSys = NULL;
SKUpdateThreadSys::SKUpdateThreadSys(unsigned int uiThreadNum)
{
	SKMAC_ASSERT(uiThreadNum);
	SKMAC_ASSERT(!ms_pUpdateThreadSys);
	ms_pUpdateThreadSys = this;

	m_uiThreadNum = uiThreadNum;
	m_UpdateThread.SetBufferNum(m_uiThreadNum);
#ifdef DEBUG_UPDATE_THREAD
	for (unsigned int i = 0 ; i < m_UpdateThread.GetNum() ; i++)
	{
		m_UpdateThread[i].m_uiThreadId = i;
	}
#endif
#ifdef EVENT_UPDATE
	m_WaitEvent.SetBufferNum(m_uiThreadNum);
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_WaitEvent[i] = &m_UpdateThread[i].m_Event;
	}
#endif
	m_uiCurUpdateType = UPDATE_NODE;
	m_UpdateCenter[UPDATE_NODE] = SK_NEW SKUpdateCenter(true,m_uiThreadNum);
#ifdef CONTEXT_UPDATE
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
#endif
}
SKUpdateThreadSys::~SKUpdateThreadSys()
{
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Stop();
	}
	SKMAC_DELETE(m_UpdateCenter[UPDATE_NODE]);
}
void SKUpdateThreadSys::Begin()
{

#ifdef DEBUG_UPDATE_THREAD
	SKSynchronize::SKSafeOutPutDebugString("*************************Start****************\n");
#endif
	for (unsigned int i = 0 ; i < m_uiThreadNum; i++)
	{
		m_UpdateThread[i].SetUpdateContext(m_UpdateCenter[m_uiCurUpdateType]->GetUpdateContext(i));
	}
/*#ifdef EVENT_UPDATE*/
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
/*#endif*/
	m_UpdateCenter[m_uiCurUpdateType]->Execute();

	
}
void SKUpdateThreadSys::ExChange()
{
#ifdef EVENT_UPDATE
	// 	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	// 	{
	// 		m_UpdateThread[i].m_Event.Wait();
	// 
	// 	}
	SKSynchronize::WaitAll(m_WaitEvent.GetBuffer(),m_uiThreadNum,true);
#endif
#ifdef DEBUG_UPDATE_THREAD
	SKSynchronize::SKSafeOutPutDebugString("**Exchange**\n");
#endif
#ifdef CONTEXT_UPDATE
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		while(1)
		{
			if (m_UpdateThread[i].IsReady())
			{
				break;
			}
		}
	}
#endif
/*#ifdef EVENT_UPDATE*/
	for (unsigned int i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Suspend();
	}
/*#endif*/
#ifdef DEBUG_UPDATE_THREAD
	SKSynchronize::SKSafeOutPutDebugString("system clear\n");
#endif
	m_UpdateCenter[m_uiCurUpdateType]->Clear();
}
