#include "SKEngineThread.h"
#include "SKUserConstant.h"
#include "SKBind.h"
using namespace SKEngine2;
SKConstantBuffer::SKConstantBuffer()
{
	m_uiCurBufferP = 0;
	m_Buffer.SetBufferNum(Constant_BUFFER_SIZE);
}
SKConstantBuffer::~SKConstantBuffer()
{

}
unsigned char * SKConstantBuffer::Assign(unsigned int uiSize)
{
	unsigned char * pTemp = NULL;
	m_Lock.Lock();
	m_uiCurBufferP += uiSize;
	if (m_uiCurBufferP > Constant_BUFFER_SIZE)
	{
		m_Lock.Unlock();
		SKMAC_ASSERT(0);
		return NULL;
	}

	pTemp = &m_Buffer[m_uiCurBufferP - uiSize];
	m_Lock.Unlock();
	return pTemp;
}
unsigned char * SKConstantBuffer::Assign(unsigned int VTType,unsigned int uiRegisterNum)
{
	unsigned int uiSize = 0;
	if(VTType == SKUserConstant::VT_FLOAT)
	{
		uiSize = sizeof(SKREAL) * 4 * uiRegisterNum;
	}
	else if (VTType == SKUserConstant::VT_INT)
	{
		uiSize = sizeof(int) * 4 * uiRegisterNum;
	}
	else if (VTType == SKUserConstant::VT_BOOL)
	{
		uiSize = sizeof(bool) * 4 * uiRegisterNum;
	}
	SKMAC_ASSERT(uiSize);
	return Assign(uiSize);
}
void SKConstantBuffer::Clear()
{
	m_uiCurBufferP = NULL;
}