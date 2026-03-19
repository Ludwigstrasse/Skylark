#include "SKGShader.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKGShader,SKShader)
BEGIN_ADD_PROPERTY(SKGShader,SKShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKGShader)
IMPLEMENT_INITIAL_END
SKPointer<SKGShader> SKGShader::Default;
bool SKGShader::ms_bIsEnableASYNLoader = true;
bool SKGShader::ms_bIsEnableGC = true;
SKGShader::SKGShader()
{

}
SKGShader::~SKGShader()
{

}
SKGShader::SKGShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(pBuffer,MainFunName,IsFromFile)
{

}
SKGShader::SKGShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(Buffer,MainFunName,IsFromFile)
{

}
bool SKGShader::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadGShaderProgram(this,pID))
		return 0;

	return 1;
}
bool SKGShader::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseGShaderProgram(pID))
		return 0;


	return 1;
}