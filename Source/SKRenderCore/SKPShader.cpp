#include "SKPShader.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPShader,SKShader)
BEGIN_ADD_PROPERTY(SKPShader,SKShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPShader)
IMPLEMENT_INITIAL_END
SKPointer<SKPShader> SKPShader::Default;
bool SKPShader::ms_bIsEnableASYNLoader = true;
bool SKPShader::ms_bIsEnableGC = true;
SKPShader::SKPShader()
{

}
SKPShader::SKPShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(pBuffer,MainFunName,IsFromFile)
{

}
SKPShader::SKPShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(Buffer,MainFunName,IsFromFile)
{

}
SKPShader::~SKPShader()
{

}


bool SKPShader::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadPShaderProgram (this,pID))
		return 0;

	return 1;
}
bool SKPShader::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleasePShaderProgram(pID))
		return 0;


	return 1;
}