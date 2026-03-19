#include "SKVertexFormat.h"
#include "SKVertexBuffer.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKVertexFormat,SKBind)
BEGIN_ADD_PROPERTY(SKVertexFormat,SKBind)
REGISTER_PROPERTY(m_uiVertexFormatCode,VertexFormatCode,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_FormatArray,FormatArray,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKVertexFormat)
IMPLEMENT_INITIAL_END
SKVertexFormat::~SKVertexFormat()
{
	ReleaseResource();
 	
}
SKVertexFormat::SKVertexFormat()
{
	m_bIsStatic = 1;

}


bool SKVertexFormat::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnLoadVBufferFormat (this,pID))
		return 0;

	return 1;
}
bool SKVertexFormat::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseVBufferFormat(pID))
		return 0;
	return 1;
}
