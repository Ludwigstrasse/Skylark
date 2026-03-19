#include "SKIndexBuffer.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKIndexBuffer,SKBind)
BEGIN_ADD_PROPERTY(SKIndexBuffer,SKBind)
REGISTER_PROPERTY(m_pDate,IndexData,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNum,Num,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDT,DataType,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKIndexBuffer)
IMPLEMENT_INITIAL_END
SKIndexBuffer::SKIndexBuffer()
{
	m_pDate = NULL;
	m_uiNum = 0;
	m_uiDT = SKDataBuffer::DT_USHORT;
	m_pLockDate = NULL;
}
SKIndexBuffer::~SKIndexBuffer()
{
	ReleaseResource();
	m_pDate = NULL;
	m_pLockDate = NULL;
}
void SKIndexBuffer::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			m_pDate = NULL;
		}
	}
}
bool SKIndexBuffer::LoadResource(SKRenderer * pRender)
{
	if(!SKBind::LoadResource(pRender))
	{
		return 0;
	}
	
	return 1;
}
bool SKIndexBuffer::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadIBuffer(this,pID))
		return 0;
	return 1;
}
bool SKIndexBuffer::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;

	if(!m_pUser->OnReleaseIBuffer(pID))
		return 0;
	return 1;
}
void *SKIndexBuffer::Lock()
{
	if (m_pLockDate || m_bIsStatic)
	{
		return NULL;
	}
	if (m_pUser)
	{
		m_pLockDate = m_pUser->Lock(this);
	}

	return m_pLockDate;
}
void SKIndexBuffer::UnLock()
{
	if (!m_pLockDate || m_bIsStatic)
	{
		return;
	}
	if (m_pUser)
	{
		m_pUser->UnLock(this);
	}
	m_pLockDate = NULL;
}
bool SKIndexBuffer::SetDate(SKDataBuffer * pDate)
{
	if ((pDate->GetDT() != SKDataBuffer::DT_USHORT && pDate->GetDT() != SKDataBuffer::DT_UINT)
		|| !pDate->GetNum() || !pDate->GetDate())
	{
		return 0;
	}
	m_pDate = pDate;
	m_uiNum = pDate->GetNum();
	m_uiDT = pDate->GetDT();
	return 1;
}

unsigned int SKIndexBuffer::GetNum()const
{
	return m_uiNum;
}
unsigned int SKIndexBuffer::GetByteSize()const
{
	return SKDataBuffer::ms_uiDateTypeByte[m_uiDT] * m_uiNum;
}
SKIndexBuffer::SKIndexBuffer(unsigned int uiNum,unsigned int uiDT)
{
	SKMAC_ASSERT(uiNum);
	SKMAC_ASSERT(uiDT == SKDataBuffer::DT_USHORT || uiDT == SKDataBuffer::DT_UINT)
	m_pDate = NULL;
	m_uiNum = uiNum;
	m_uiDT = uiDT;
	m_pLockDate = NULL;
}