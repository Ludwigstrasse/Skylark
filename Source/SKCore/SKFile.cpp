#include "SKFile.h"
using namespace SKEngine2;
TCHAR SKFile::ms_cOpenMode[OM_MAX][5] = 
{
	_T("rb"),
	_T("wb"),
	_T("rt"),
	_T("wt"),
};
unsigned int SKFile::m_uiSeekFlag[] = 
{
	SEEK_CUR,
	SEEK_END,
	SEEK_SET
};
SKFile::SKFile()
{
	m_pFileHandle = NULL;
	m_uiOpenMode = OM_MAX;
	m_uiFileSize = 0;
}
SKFile::~SKFile()
{
	
	if (m_pFileHandle)
	{
		fclose(m_pFileHandle);
		m_pFileHandle = NULL;
	}
}
bool SKFile::Seek(unsigned int uiOffSet,unsigned int uiOrigin)
{
	if (!m_pFileHandle)
	{
		return false;
	}
	fseek(m_pFileHandle,uiOffSet,uiOrigin);
	return true;
}
bool SKFile::Open(const TCHAR * pFileName,unsigned int uiOpenMode)
{
	if (m_pFileHandle)
	{
		fclose(m_pFileHandle);
	}
	if (!pFileName || uiOpenMode >= OM_MAX)
	{
		return false;
	}
	unsigned int uiLen = SKStrLen(pFileName);
	if (uiLen < SKMAX_PATH - 1)
	{
		if(!SKMemcpy(m_tcFileName,pFileName,uiLen + 1))
			return false;
	}
	else
	{
		return false;
	}

	m_uiOpenMode = uiOpenMode;
	if (m_uiOpenMode == OM_RB || m_uiOpenMode == OM_RT)
	{
		struct _stat64i32 kStat;
		if (_tstat(pFileName,&kStat) != 0)
		{
			return false;
		}
		m_uiFileSize = kStat.st_size;
	}

	errno_t uiError = _tfopen_s(&m_pFileHandle,pFileName,ms_cOpenMode[m_uiOpenMode]);
	if (uiError)
	{
		return 0;
	}
	if (!m_pFileHandle)
	{
		return 0;
	}

	return true;
}
bool SKFile::Write(const void *pBuffer,unsigned int uiSize,unsigned int uiCount)
{
	if (!m_pFileHandle)
	{
		return false;
	}
	if (!pBuffer || !uiSize || !uiCount)
	{
		return false;
	}
	fwrite(pBuffer,uiSize,uiCount,m_pFileHandle);
	return true;
}
bool SKFile::GetLine(void * pBuffer,unsigned int uiSize)
{
	if (!m_pFileHandle)
	{
		return false;
	}
	if (!pBuffer || !uiSize)
	{
		return false;
	}
	if (!_fgetts((TCHAR *)pBuffer,uiSize, m_pFileHandle))
		return false;
	return true;
}
bool SKFile::Read(void *pBuffer,unsigned int uiSize,unsigned int uiCount)
{
	if (!m_pFileHandle)
	{
		return false;
	}
	if (!pBuffer || !uiSize || !uiCount)
	{
		return false;
	}
	fread(pBuffer,uiSize,uiCount,m_pFileHandle);
	return true;
}
bool SKFile::Flush()
{
	return(fflush(m_pFileHandle) == 0);
}