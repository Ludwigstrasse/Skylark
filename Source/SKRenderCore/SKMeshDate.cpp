#include "SKMeshDate.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKMeshDate,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKMeshDate)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKMeshDate,SKObject)
REGISTER_PROPERTY(m_pVertexBuffer,VertexBuffer,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pIndexBuffer,IndexBuffer,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pCollapseRecord,CollapseRecord,SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKMeshDate::SKMeshDate()
{

	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pCollapseRecord = NULL;

}
SKMeshDate::~SKMeshDate()
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pCollapseRecord = NULL;
}
bool SKMeshDate::SetIndexBuffer(SKIndexBuffer * pIndexBuffer)
{
	if(!pIndexBuffer)
		return 0;

	if(pIndexBuffer->GetNum())
	{
		m_pIndexBuffer = pIndexBuffer;
		return 1;

	}
	else
	{
		return 0;
	}
}
bool SKMeshDate::SetVertexBuffer(SKVertexBuffer *pVertexBuffer)
{
	if(!pVertexBuffer)
		return 0;
	if(pVertexBuffer->GetVertexNum())
	{
		m_pVertexBuffer = pVertexBuffer;
		return 1;
	}
	else
	{
		return 0;
	}
}
void SKMeshDate::SetLodDesirePercent(SKREAL fDesirePercent)
{
	if (m_pCollapseRecord)
	{
		if (m_pCollapseRecord->GetRecordNum() > 0)
		{
			unsigned int uiDesireRecordID = (unsigned int )( (1.0f- fDesirePercent) * (m_pCollapseRecord->GetRecordNum() - 1));
			m_pCollapseRecord->SetDesireRecordID(uiDesireRecordID);
		}
		
	}

}