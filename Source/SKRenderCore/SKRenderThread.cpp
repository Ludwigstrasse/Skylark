#include "SKRenderThread.h"
#include "SKUserConstant.h"
#include "SKBind.h"
#include "SKGraphicInclude.h"
#include "SKResourceManager.h"
#include "SKProfiler.h"
using namespace SKEngine2;
//#define DEBUG_RENDER_THREAD
SKRenderThreadBuffer::SKRenderThreadBuffer()
{

}
SKRenderThreadBuffer::~SKRenderThreadBuffer()
{
	
}
unsigned char * SKRenderThreadBuffer::Assign(unsigned int uiSize)
{
	return m_ConstantBuffer.Assign(uiSize);
}
unsigned char * SKRenderThreadBuffer::Assign(unsigned int VTType,unsigned int uiRegisterNum)
{
	return m_ConstantBuffer.Assign(VTType,uiRegisterNum);
}
void SKRenderThreadBuffer::Clear()
{
#ifdef DEBUG_RENDER_THREAD
	SKSynchronize::SKSafeOutPutDebugString("**Clear**\n");
#endif
	for (unsigned int i = 0 ; i < m_CommandList.GetNum() ; i++)
	{
		m_CommandList[i]->~SKRenderCommand();
	}
	m_ConstantBuffer.Clear();
	m_CommandList.Clear();
}

void SKRenderThreadBuffer::Execute()
{
#ifdef DEBUG_RENDER_THREAD
	SKSynchronize::SKSafeOutPutDebugString("==========\n");
	SKSynchronize::SKSafeOutPutDebugString("Command Num : %d\n",m_CommandList.GetNum());
#endif
	for (unsigned int i = 0 ; i < m_CommandList.GetNum() ; i++)
	{
		m_CommandList[i]->Execute();
#ifdef DEBUG_RENDER_THREAD	
		SKSynchronize::SKSafeOutPutDebugString("%d_%s\n",i,m_CommandList[i]->DescribeCommand());
#endif
	}
#ifdef DEBUG_RENDER_THREAD
	SKSynchronize::SKSafeOutPutDebugString("==========\n");
#endif
}
SKRenderThreadSys * SKRenderThreadSys::ms_pRenderThreadSys = NULL;
SKRenderThreadSys::SKRenderThreadSys()
{
	SKMAC_ASSERT(!ms_pRenderThreadSys);
	ms_pRenderThreadSys = this;
	m_bIsRunning = false;
	m_RenderBuffer = SK_NEW SKRenderThreadBuffer();
	m_UpdateBuffer = SK_NEW SKRenderThreadBuffer();
	#ifdef 	RENDERBUFFER_UPDATE
		m_RenderThread.Start();
	#endif
}
SKRenderThreadSys::~SKRenderThreadSys()
{
	Clear();
	SKMAC_DELETE(m_RenderBuffer);
	SKMAC_DELETE(m_UpdateBuffer);
	m_RenderThread.Start();
	m_RenderThread.Stop();
}
unsigned char * SKRenderThreadSys::Assign(unsigned int uiSize)
{
	return m_UpdateBuffer->Assign(uiSize);
}
unsigned char * SKRenderThreadSys::Assign(unsigned int VTType,unsigned int uiRegisterNum)
{
	return m_UpdateBuffer->Assign(VTType,uiRegisterNum);
}
DECLEAR_TIME_PROFILENODE(RenderThreadsysExChange, ApplicationUpdate)
void SKRenderThreadSys::ExChange()
{
	ADD_TIME_PROFILE(RenderThreadsysExChange)
#ifdef EVENT_UPDATE	
	m_RenderThread.m_Event.Wait();
#endif
#ifdef 	RENDERBUFFER_UPDATE
	while(1)
	{
#ifdef SLEEP_FLAG
		Sleep(0);
#endif
		if (m_RenderThread.IsReady())
		{
			break;
		}
	}
#endif
#ifdef DEBUG_RENDER_THREAD
	SKSynchronize::SKSafeOutPutDebugString("**Exchange**\n");
#endif
/*#ifdef EVENT_UPDATE*/
	m_RenderThread.Suspend();
/*#endif*/
	m_RenderBuffer->Clear();
	Swap(m_UpdateBuffer,m_RenderBuffer);
	for (unsigned int i = 0 ; i < SKBind::ms_DynamicTwoBindArray.GetNum() ;i++)
	{
		SKBind::ms_DynamicTwoBindArray[i]->ExChange();
	}
	m_bIsRunning = false;
}
void SKRenderThreadSys::Begin()
{
#ifdef DEBUG_RENDER_THREAD
	SKSynchronize::SKSafeOutPutDebugString("*************************Start****************\n");
#endif
	m_bIsRunning = true;
	m_RenderThread.SetRender(m_RenderBuffer);
/*#ifdef EVENT_UPDATE*/
	m_RenderThread.Start();
/*#endif*/
}
void SKRenderThreadSys::Clear()
{
#ifdef DEBUG_RENDER_THREAD
	SKSynchronize::SKSafeOutPutDebugString("system clear\n");
#endif
	m_RenderBuffer->Clear();
	m_UpdateBuffer->Clear();
}
SKRenderThread::SKRenderThread()
{
#ifdef EVENT_UPDATE
	m_Event.Create(true,_T("RenderEvent"));
	m_Event.Reset();
#endif
	m_pRenderBuffer = NULL;
}
SKRenderThread::~SKRenderThread()
{

}

void SKRenderThread::Run()
{
	while(!IsStopTrigger())
	{
		if (m_pRenderBuffer)
		{
			m_pRenderBuffer->Execute();
			m_pRenderBuffer = NULL;
		#ifdef EVENT_UPDATE
			m_Event.Trigger();
		#endif
		}	
	}
}
void SKRenderThread::SetRender(SKRenderThreadBuffer * pRenderBuffer)
{
	m_pRenderBuffer = pRenderBuffer;
	#ifdef EVENT_UPDATE
		m_Event.Reset();
	#endif
	
}
#ifdef 	RENDERBUFFER_UPDATE
bool SKRenderThread::IsReady()
{
#ifndef SLEEP_FLAG
	m_Lock.Lock();
#endif
	return !m_pRenderBuffer;
#ifndef SLEEP_FLAG
	m_Lock.Unlock();
#endif
}
#endif
