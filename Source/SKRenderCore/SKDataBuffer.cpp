#include "SKDataBuffer.h"
#include "SKMath.h"
#include "SKVector3.h"
#include "SKVector2.h"
#include "SKVector3W.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDataBuffer,SKObject)
BEGIN_ADD_PROPERTY(SKDataBuffer,SKObject)
REGISTER_PROPERTY(m_uiNum,Num,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDT,DataType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_pDate,m_uiSize,Date)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDataBuffer)
IMPLEMENT_INITIAL_END
unsigned int SKDataBuffer::ms_uiDateTypeByte[DT_MAXNUM] =
{
	4,
	8,
	12,
	16,

	2,
	4,

	4,
	4,

	4,
	4,
	4,

	8,
	8,
	8,

	4,
	8,

	4
};
unsigned int SKDataBuffer::ms_uiDateTypeChannel[DT_MAXNUM] =
{
	1,
	2,
	3,
	4,

	1,
	1,

	4,
	4,
	
	2,
	2,
	2,

	4,
	4,
	4,

	2,
	4,

	4

};
SKDataBuffer::SKDataBuffer()
{
	m_uiDT = DT_MAXNUM;

	m_uiNum = 0;
	m_pDate = NULL;

}
SKDataBuffer::~SKDataBuffer()
{
	SKMAC_DELETEA(m_pDate);
}

bool SKDataBuffer::SetDate(const void *pDate,unsigned int uiNum,unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !pDate || !uiNum)
		return 0;

	m_uiDT = uiDT;
	m_uiNum = uiNum;

	SKMAC_DELETEA(m_pDate);

	m_pDate = SK_NEW unsigned char[GetSize()];

	if(!m_pDate)
		return 0;
	SKMemcpy(m_pDate,pDate,GetSize());
	m_uiSize = GetSize();
	return 1;
}
bool SKDataBuffer::AddDate(const void *pDate,unsigned int uiNum,unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !pDate || !uiNum)
		return 0;
	if(m_uiDT != SKMAX_INTEGER && m_uiDT != uiDT)
		return 0;


	unsigned char * Temp = NULL;
	Temp = SK_NEW unsigned char[ ( uiNum + m_uiNum )* GetStride()];
	if(!Temp)
		return 0;
	SKMemcpy(Temp,m_pDate,GetStride() * m_uiNum);
	SKMemcpy(Temp + GetStride() * m_uiNum,pDate,GetStride() * uiNum);

	SKMAC_DELETEA(m_pDate);
	m_uiNum += uiNum;
	m_pDate = Temp;
	m_uiSize = GetSize();
	return 1;
}
bool SKDataBuffer::CreateEmptyBuffer(unsigned int uiNum,unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !uiNum)
		return 0;

	m_uiDT = uiDT;
	m_uiNum = uiNum;
	SKMAC_DELETEA(m_pDate);

	m_pDate = SK_NEW unsigned char[GetSize()];

	if(!m_pDate)
		return 0;
	SKMemset(m_pDate,0,GetSize());
	m_uiSize = GetSize();
	return 1;
}



