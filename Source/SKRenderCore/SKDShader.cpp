#include "SKDShader.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDShader,SKShader)
BEGIN_ADD_PROPERTY(SKDShader,SKShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDShader)
IMPLEMENT_INITIAL_END
SKPointer<SKDShader> SKDShader::Default;
bool SKDShader::ms_bIsEnableASYNLoader = true;
bool SKDShader::ms_bIsEnableGC = true;
SKDShader::SKDShader()
{

}
SKDShader::~SKDShader()
{

}
SKDShader::SKDShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(pBuffer,MainFunName,IsFromFile)
{

}
SKDShader::SKDShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(Buffer,MainFunName,IsFromFile)
{

}
bool SKDShader::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadDShaderProgram(this,pID))
		return 0;

	return 1;
}
bool SKDShader::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseDShaderProgram(pID))
		return 0;


	return 1;
}