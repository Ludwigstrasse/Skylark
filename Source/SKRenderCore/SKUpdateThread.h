#ifndef SKUPDATETHREAD_H
#define SKUPDATETHREAD_H
#include "SKObject.h"
#include "SKThread.h"
#include "SKEngineThread.h"
#include "SKDelegateList.h"
//#define DEBUG_UPDATE_THREAD
//#define EVENT_UPDATE
#define CONTEXT_UPDATE
namespace SKEngine2
{
	class SKUpdateContext
	{
	public:
		SKUpdateContext();
		virtual ~SKUpdateContext();
		unsigned int Execute();
		void Clear();
		void AddCommand(SKRenderCommand * pCommand);
	protected:
		SKArray<SKRenderCommand *> m_CommandArray;
	};
	class SKUpdateCenter
	{
	public:
		SKUpdateCenter(bool bIsMainRun,unsigned int uiThreadNum);
		virtual ~SKUpdateCenter();
		SKUpdateContext * GetUpdateContext(unsigned int i);
		FORCEINLINE bool IsMainRun()const
		{
			return m_bIsMainRun;
		}
		unsigned int Execute();
		void Clear();
		template<class T>
		SKRenderCommand * AssignCommand(int iIndex = -1);
	protected:
		SKUpdateContext *m_pContextArray;
		SKConstantBuffer m_ConstantBuffer;
		unsigned char * Assign(unsigned int uiSize);
		bool m_bIsMainRun;
		unsigned int m_uiContextNum;
		virtual unsigned int GetIndex();
		int m_uiIndex;
	};
	template<class T>
	SKRenderCommand * SKUpdateCenter::AssignCommand(int iIndex)
	{	
		SKRenderCommand* pRenderCommand = (SKRenderCommand *)Assign(sizeof(T));
		if (iIndex == -1)
		{
			m_pContextArray[GetIndex()].AddCommand(pRenderCommand);
		}
		else
		{
			SKMAC_ASSERT(iIndex < (int)m_uiContextNum);
			m_pContextArray[iIndex].AddCommand(pRenderCommand);
		}
		return pRenderCommand;
	}
	class SKUpdateThread : public SKThread
	{
	public:
		SKUpdateThread();
		~SKUpdateThread();
		friend class SKUpdateThreadSys;	
	protected:
		SKUpdateContext * m_pUpdateContex;
		virtual const TCHAR* GetThreadName()
		{
			return m_ThreadName.GetBuffer();
		}
		virtual void Run();	
		void SetUpdateContext(SKUpdateContext * pUpdateContex);
#ifdef CONTEXT_UPDATE
		bool IsReady()const;
#endif
#ifdef EVENT_UPDATE
		SKEvent m_Event;
#endif
		
		unsigned int m_uiThreadId;
		SKString m_ThreadName;
	private:
	};

	class SKGRAPHIC_API SKUpdateThreadSys : public SKMemObject
	{
	public:
		enum //UpdateType
		{
			UPDATE_NODE,
			UPDATE_MAX
		};
		SKUpdateThreadSys(unsigned int uiThreadNum);
		~SKUpdateThreadSys();
		FORCEINLINE void SetUpdateType(unsigned int uiUpdateType)
		{
			m_uiCurUpdateType = uiUpdateType;
		}
		void Begin();
		void ExChange();
		template<class T>
		SKRenderCommand * AssignCommand(int iIndex = -1);
		static SKUpdateThreadSys* ms_pUpdateThreadSys;
		FORCEINLINE int GetMainThreadID()
		{
			if (m_UpdateCenter[m_uiCurUpdateType]->IsMainRun())
			{
				return m_uiThreadNum;
			}
			else
			{
				return 0;
			}
		}
		FORCEINLINE unsigned int GetThreadNum()
		{
			return m_uiThreadNum;
		}
	protected:
		unsigned int m_uiThreadNum;
		SKArray<SKUpdateThread>m_UpdateThread;
#ifdef EVENT_UPDATE
		SKArray<SKSynchronize*>m_WaitEvent;
#endif
		unsigned int m_uiCurUpdateType;
		SKUpdateCenter* m_UpdateCenter[UPDATE_MAX];

	};
	template<class T>
	SKRenderCommand * SKUpdateThreadSys::AssignCommand(int iIndex)
	{	
		SKRenderCommand* pRenderCommand = (SKRenderCommand *)m_UpdateCenter[m_uiCurUpdateType]->AssignCommand<T>(iIndex);
		return pRenderCommand;
	};
#define ENQUEUE_UPDATE_COMMAND(TypeName,Params) \
	if(SKResourceManager::ms_bUpdateThread) \
	{ \
		TypeName * pCommand = (TypeName *)SKUpdateThreadSys::ms_pUpdateThreadSys->AssignCommand<TypeName>(); \
		SK_NEW(pCommand)TypeName Params; \
	} \
	else \
	{

#define ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,Params,ThreadID) \
	if(SKResourceManager::ms_bUpdateThread) \
	{ \
		TypeName * pCommand = (TypeName *)SKUpdateThreadSys::ms_pUpdateThreadSys->AssignCommand<TypeName>(ThreadID); \
		SK_NEW(pCommand)TypeName Params; \
	} \
	else \
	{

#define ENQUEUE_UNIQUE_RENDER_COMMAND_END }

#define ENQUEUE_UNIQUE_UPDATE_COMMAND(TypeName,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,)

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_THREADID(TypeName,ThreadID,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,,ThreadID)
/**
 * Declares a rendering command type with 1 parameters.
 */
#define ENQUEUE_UNIQUE_UPDATE_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Value1,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,(Value1))

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_ONEPARAMETER_THREADID(TypeName,ParamType1,ParamName1,Value1,ThreadID,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,(Value1),ThreadID)
/**
 * Declares a rendering command type with 2 parameters.
 */
#define ENQUEUE_UNIQUE_UPDATE_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,(Value1,Value2))

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_TWOPARAMETER_THREADID(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ThreadID,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,(Value1,Value2),ThreadID)
/**
 * Declares a rendering command type with 3 parameters.
 */
#define ENQUEUE_UNIQUE_UPDATE_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ParamType3,ParamName3,Value3,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,(Value1,Value2,Value3))

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_THREEPARAMETER_THREADID(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ParamType3,ParamName3,Value3,ThreadID,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,(Value1,Value2,Value3),ThreadID)
}
#endif