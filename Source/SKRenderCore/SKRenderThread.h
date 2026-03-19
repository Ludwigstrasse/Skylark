#ifndef SKRENDERTHREAD_H
#define SKRENDERTHREAD_H
#include "SKObject.h"
#include "SKThread.h"
#include "SKEngineThread.h"
//#define EVENT_UPDATE
#define RENDERBUFFER_UPDATE
#ifdef RENDERBUFFER_UPDATE
#define SLEEP_FLAG
#endif
namespace SKEngine2
{
	
	class SKGRAPHIC_API SKRenderThreadBuffer : public SKMemObject
	{
	public:
		SKRenderThreadBuffer();
		~SKRenderThreadBuffer();
		friend class SKRenderThreadSys;
		friend class SKRenderThread;
	protected:
		unsigned char * Assign(unsigned int uiSize);
		unsigned char * Assign(unsigned int VTType,unsigned int uiRegisterNum);
		void Clear();
		template<class T>
		SKRenderCommand * AssignCommand();
		void Execute();
		SKConstantBuffer m_ConstantBuffer;
		SKArray<SKRenderCommand *> m_CommandList;
		SKCriticalSection m_Lock;
	};
	template<class T>
	SKRenderCommand * SKRenderThreadBuffer::AssignCommand()
	{	
		m_Lock.Lock();
		SKRenderCommand* pRenderCommand = (SKRenderCommand *)Assign(sizeof(T));
		m_CommandList.AddElement(pRenderCommand);
		m_Lock.Unlock();
		return pRenderCommand;
	}
	class SKGRAPHIC_API SKRenderThread : public SKThread
	{
	public:
		SKRenderThread();
		~SKRenderThread();
		friend class SKRenderThreadSys;
	protected:
		virtual const TCHAR* GetThreadName()
		{
			return _T("SKRenderThread");
		}
		virtual void Run();	
		void SetRender(SKRenderThreadBuffer * pRenderBuffer);
#ifdef EVENT_UPDATE
		SKEvent m_Event;
#endif	
#ifdef 	RENDERBUFFER_UPDATE
	#ifndef SLEEP_FLAG
		SKCriticalSection m_Lock;
	#endif
		bool IsReady();
#endif
		SKRenderThreadBuffer * m_pRenderBuffer;
	};
	class SKGRAPHIC_API SKRenderThreadSys : public SKMemObject
	{
	public:
		SKRenderThreadSys();
		~SKRenderThreadSys();
		unsigned char * Assign(unsigned int uiSize);
		unsigned char * Assign(unsigned int VTType,unsigned int uiRegisterNum);
		template<class T>
		SKRenderCommand * AssignCommand();
		void ExChange();
		void Begin();
		void Clear();
		static SKRenderThreadSys* ms_pRenderThreadSys;
		bool IsRunning()
		{
			return m_bIsRunning;
		}
	protected:
		SKRenderThreadBuffer * m_RenderBuffer;
		SKRenderThreadBuffer * m_UpdateBuffer;
		SKRenderThread			m_RenderThread;
		bool					m_bIsRunning;


	};
	template<class T>
	SKRenderCommand * SKRenderThreadSys::AssignCommand()
	{
		return m_UpdateBuffer->AssignCommand<T>();
	}
/**
 * Declares a rendering command type with 0 parameters.
 */

#define ENQUEUE_RENDER_COMMAND(TypeName,Params) \
	if(SKResourceManager::ms_bRenderThread && SKRenderThreadSys::ms_pRenderThreadSys->IsRunning()) \
	{ \
		TypeName * pCommand = (TypeName *)SKRenderThreadSys::ms_pRenderThreadSys->AssignCommand<TypeName>(); \
		SK_NEW(pCommand)TypeName Params; \
	} \
	else \
	{

#define ENQUEUE_UNIQUE_RENDER_COMMAND_END }

#define ENQUEUE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,)


/**
 * Declares a rendering command type with 1 parameters.
 */
#define ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Value1,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,(Value1))


/**
 * Declares a rendering command type with 2 parameters.
 */
#define ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,(Value1,Value2))


/**
 * Declares a rendering command type with 3 parameters.
 */
#define ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ParamType3,ParamName3,Value3,Code) \
	DECLARE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,(Value1,Value2,Value3))
}
#endif