#ifndef SKSYSTEM_H
#define SKSYSTEM_H
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>
#include <atlsimpstr.h>
#pragma warning(disable:4251) //ȥģ嵼ľ
#pragma warning(disable:4595)
#pragma warning(disable:4091)
#ifdef SKSYSTEM_EXPORTS
	#define SKSYSTEM_API __declspec(dllexport) 
#else 
	#define SKSYSTEM_API __declspec(dllimport) 
#endif
namespace SKEngine2
{
#if WIN32
#define SYSTEM_BIT_WIDTH 32
#else if WIN64
#define SYSTEM_BIT_WIDTH 64
#endif
#define THREAD_CALLBACK __stdcall
#if WIN32 || WIN64
#define WINDOWS
#endif


	#define LOG_BUFFER_SIZE 65536	 
	class SKSYSTEM_API SKSystem
	{
	public:
		static TCHAR ms_sLogBuffer[LOG_BUFFER_SIZE];
		static DWORD ms_dwMainThreadID;
	};
	FORCEINLINE bool SKMemcpy(void * pDest,const void * pSrc, unsigned int uiCountSize,unsigned int uiDestBufferSize = 0)
	{
		if (!uiDestBufferSize)
		{
			uiDestBufferSize = uiCountSize;
		}
#ifdef WINDOWS
		return (memcpy_s(pDest,uiDestBufferSize,pSrc,uiCountSize) == 0);
#else
		return false;
#endif

	}
	FORCEINLINE unsigned int SKStrLen(const TCHAR * pStr)
	{
#ifdef WINDOWS
		return (unsigned int)_tcslen(pStr);
#else
		return 0;
#endif
	}
	FORCEINLINE void SKLockedIncrement(long * pRefCount)
	{
#ifdef WINDOWS
		_InterlockedIncrement(pRefCount);
#else
		return;
#endif
	}
	FORCEINLINE void SKLockedDecrement(long * pRefCount)
	{
#ifdef WINDOWS
		_InterlockedDecrement(pRefCount);
#else
		return ;
#endif
	}
	FORCEINLINE void SKMemset(void *pDest,int iC,unsigned int uiCount)
	{
#ifdef WINDOWS
		memset(pDest,iC,uiCount);
#else
		return;
#endif
	}
	FORCEINLINE void SKStrcat(TCHAR * pDest,unsigned int uiCount,const TCHAR * pSource)
	{
#ifdef WINDOWS
		_tcscat_s(pDest,uiCount,pSource);
#else
		return ;
#endif
	}
	//Find the next token in a string.
	FORCEINLINE TCHAR * SKStrtok(TCHAR *strToken, const TCHAR *strDelimit ,TCHAR ** pContext)
	{
#ifdef WINDOWS
		return _tcstok_s(strToken, strDelimit,pContext);
#else
		return NULL;
#endif
	}
	//Scan a string for the last occurrence of a character.
	FORCEINLINE const TCHAR * SKCsrchr(const TCHAR *pString,int c )
	{
#ifdef WINDOWS
		return _tcsrchr(pString,c);
#else
		return NULL;
#endif
	}
	FORCEINLINE void SKOutPutDebugString(const TCHAR * pcString, ...)
	{
#ifdef WINDOWS
		char *pArgs;
		pArgs = (char*) &pcString + sizeof(pcString);
		_vstprintf_s(SKSystem::ms_sLogBuffer, LOG_BUFFER_SIZE,pcString, pArgs) ;
		OutputDebugString(SKSystem::ms_sLogBuffer);
#else
		return ;
#endif
	}
	FORCEINLINE void SKSprintf(TCHAR * _DstBuf, unsigned int _SizeInBytes,const TCHAR * _Format, ...)
	{
#ifdef WINDOWS
		char *pArgs;
		pArgs = (char*) &_Format + sizeof(_Format);
		_vstprintf_s(_DstBuf, _SizeInBytes,_Format, pArgs) ;
#else
		return ;
#endif
	}
	FORCEINLINE void SKSprintf(TCHAR * _DstBuf, unsigned int _SizeInBytes, const TCHAR * _Format, va_list pArgs)
	{
#ifdef WINDOWS
		_vstprintf_s(_DstBuf, _SizeInBytes, _Format, pArgs);
#else
		return;
#endif
	}
	FORCEINLINE void SKScanf(TCHAR * Buf, const TCHAR * _Format, va_list pArgs)
	{
#ifdef WINDOWS
		_stscanf_s(Buf, _Format, pArgs);
#else
		return;
#endif
	}
	FORCEINLINE void SKStrCopy(TCHAR * pDest,unsigned int uiCount,const TCHAR * pSource)
	{
#ifdef WINDOWS
		_tcscpy_s(pDest,uiCount,pSource);
#else
		return;
#endif
	}
	FORCEINLINE int SKStrCmp(const TCHAR *String1,const TCHAR *String2)
	{
#ifdef WINDOWS
		return _tcscmp(String1,String2);
#else
		return;
#endif
	}
	FORCEINLINE void SKMbsToWcs(wchar_t * Dest,unsigned int uiSizeInWord,const char * Source,unsigned int uiSizeInByte)
	{
#ifdef WINDOWS
		mbstowcs_s(0,Dest,uiSizeInWord,Source,uiSizeInByte);
#else
		return;
#endif
	}
	FORCEINLINE void SKWcsToMbs(char * Dest,unsigned int uiSizeInByte,const wchar_t * Source,unsigned int uiSizeInWord)
	{
#ifdef WINDOWS
		wcstombs_s(0,Dest,uiSizeInByte,Source,uiSizeInWord);
#else
		return;
#endif
	}
	FORCEINLINE unsigned int SKGetCpuNum()
	{
#ifdef WINDOWS
		SYSTEM_INFO SyetemInfo;
		GetSystemInfo(&SyetemInfo);
		return SyetemInfo.dwNumberOfProcessors;
#else
		return 1;
#endif
	}
	FORCEINLINE bool SKIsMainThread()
	{
#ifdef WINDOWS
		return SKSystem::ms_dwMainThreadID == GetCurrentThreadId();
#else
		return false;
#endif
	}
	FORCEINLINE void SKInitSystem()
	{
#ifdef WINDOWS
		SKSystem::ms_dwMainThreadID = GetCurrentThreadId();
#else
		return;
#endif
	}
	#define SKMAC_ASSERT(Expression)\
	{\
		 assert(Expression);\
	}
	

	//ֵ
	template<typename T>
	FORCEINLINE T ABS(T t) 
	{ 
		// 	if (t < 0) 
		// 		return -t; 
		return t < 0 ? -t:t;
		/*return t; */
	}
	template<typename T>
	FORCEINLINE T Min(T t0 , T t1)
	{ 
		return t0 < t1 ? t0 : t1;
	}
	template<typename T>
	FORCEINLINE T Max(T t0 , T t1)
	{
		return t0 > t1 ? t0 : t1;
	}
	template<typename T>
	FORCEINLINE T Clamp(T Value,T Max,T Min)
	{
		if (Value >= Max)
		{
			return Max;
		}
		if (Value <= Min)
		{
			return Min;
		}
		return Value;
	}
	template <class T>
	FORCEINLINE void Swap(T &t1, T &t2)
	{
		T temp;
		temp = t1;
		t1 = t2;
		t2 = temp;
	}
#define BIT(i) (1 << i)
}

#endif