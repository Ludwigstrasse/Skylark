#include "SKLog.h"
using namespace SKEngine2;
/********************************************************************************/
SKLog::SKLog()
{


}
/********************************************************************************/
SKLog::~SKLog()
{

}
/********************************************************************************/
bool SKLog::Open(const TCHAR * pFileName)
{
	return SKFile::Open(pFileName,OM_WT);
}
/********************************************************************************/
bool SKLog::WriteInfo(const TCHAR *pcString, ...)const
{

	char *pArgs;

	pArgs = (char*) &pcString + sizeof(pcString);
	_vstprintf_s(SKSystem::ms_sLogBuffer, LOG_BUFFER_SIZE,pcString, pArgs) ;
	_ftprintf(m_pFileHandle, SKSystem::ms_sLogBuffer);
	return 1;
}
/********************************************************************************/