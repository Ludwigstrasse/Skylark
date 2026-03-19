#include "SKHShader.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKHShader,SKShader)
BEGIN_ADD_PROPERTY(SKHShader,SKShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKHShader)
IMPLEMENT_INITIAL_END
SKPointer<SKHShader> SKHShader::Default;
bool SKHShader::ms_bIsEnableASYNLoader = true;
bool SKHShader::ms_bIsEnableGC = true;
SKHShader::SKHShader()
{

}
SKHShader::~SKHShader()
{

}
SKHShader::SKHShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(pBuffer,MainFunName,IsFromFile)
{

}
SKHShader::SKHShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(Buffer,MainFunName,IsFromFile)
{

}
bool SKHShader::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadHShaderProgram(this,pID))
		return 0;

	return 1;
}
bool SKHShader::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseHShaderProgram(pID))
		return 0;


	return 1;
}