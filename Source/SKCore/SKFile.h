#ifndef SKFILE_H
#define SKFILE_H
#include "SKSystem.h"
namespace SKEngine2
{
	class SKSYSTEM_API SKFile
	{
	public:
		enum //Open Mode
		{
			OM_RB,
			OM_WB,
			OM_RT,
			OM_WT,
			OM_MAX
		};
		enum 
		{
			SKMAX_PATH = 256
		};
		enum	//Seek Flag
		{
			SF_CUR,
			SF_END,
			SF_SET,
			SF_MAX

		};
		SKFile();
		~SKFile();
		bool Flush();

		bool Seek(unsigned int uiOffSet,unsigned int uiOrigin);
		bool Open(const TCHAR * pFileName,unsigned int uiOpenMode);
		bool Write(const void *pBuffer,unsigned int uiSize,unsigned int uiCount);
		bool Read(void *pBuffer,unsigned int uiSize,unsigned int uiCount);
		//pbufferУbufferСȡõַһַǻس
		//2 LF    (Line Feed)
		bool GetLine(void * pBuffer,unsigned int uiSize);
		FORCEINLINE unsigned int GetFileSize()const
		{
			return m_uiFileSize;
		}
	protected:
		static TCHAR ms_cOpenMode[OM_MAX][5];
		static unsigned int m_uiSeekFlag[];
		FILE  *m_pFileHandle;
		unsigned int m_uiOpenMode;
		unsigned int m_uiFileSize;
		TCHAR m_tcFileName[SKMAX_PATH];
		
	};
}
#endif