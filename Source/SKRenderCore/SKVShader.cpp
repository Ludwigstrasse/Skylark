#include "SKVShader.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKVShader,SKShader)
BEGIN_ADD_PROPERTY(SKVShader,SKShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKVShader)
IMPLEMENT_INITIAL_END
SKPointer<SKVShader> SKVShader::Default;
bool SKVShader::ms_bIsEnableASYNLoader = true;
bool SKVShader::ms_bIsEnableGC = true;
SKVShader::SKVShader()
{

}
SKVShader::~SKVShader()
{

}
SKVShader::SKVShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(pBuffer,MainFunName,IsFromFile)
{

}
SKVShader::SKVShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile)
:SKShader(Buffer,MainFunName,IsFromFile)
{

}
bool SKVShader::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadVShaderProgram(this,pID))
		return 0;

	return 1;
}
bool SKVShader::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseVShaderProgram(pID))
		return 0;


	return 1;
}