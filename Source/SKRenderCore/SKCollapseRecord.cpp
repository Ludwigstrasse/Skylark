#include "SKCollapseRecord.h"
#include "SKStream.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKCollapseRecord,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKCollapseRecord)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKCollapseRecord,SKObject)
END_ADD_PROPERTY
SKCollapseRecord::SKCollapseRecord()
{
	m_uiCurRecordID = 0;
	m_uiDesireRecordID = 0;
}
SKCollapseRecord::~SKCollapseRecord()
{
	m_uiCurRecordID = 0;
	m_uiDesireRecordID = 0;
}
IMPLEMENT_RTTI(RecordTriInfo,SKObject)
BEGIN_ADD_PROPERTY(RecordTriInfo,SKObject)
REGISTER_PROPERTY(m_uiKeep,Keep,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiThrow,Throw,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiLeftTraingleNum,LeftTraingleNum,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MapIndex,MapIndex,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(RecordTriInfo)
IMPLEMENT_INITIAL_END

IMPLEMENT_RTTI(SKCollapseRecordTri,SKCollapseRecord)
BEGIN_ADD_PROPERTY(SKCollapseRecordTri,SKCollapseRecord)
REGISTER_PROPERTY(m_RecordInfo,RecordInfo,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCollapseRecordTri)
IMPLEMENT_INITIAL_END
SKCollapseRecordTri::SKCollapseRecordTri()
{

	m_RecordInfo.Clear();
}
SKCollapseRecordTri::~SKCollapseRecordTri()
{
	m_RecordInfo.Clear();
}




IMPLEMENT_RTTI(SKCollapseRecordLine,SKCollapseRecord)
BEGIN_ADD_PROPERTY(SKCollapseRecordLine,SKCollapseRecord)
REGISTER_PROPERTY(m_RecordInfo,RecordInfo,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCollapseRecordLine)
IMPLEMENT_INITIAL_END
SKCollapseRecordLine::SKCollapseRecordLine()
{

	m_RecordInfo.Clear();
}
SKCollapseRecordLine::~SKCollapseRecordLine()
{
	m_RecordInfo.Clear();
}

