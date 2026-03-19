#include "SKUserConstant.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKUserConstant,SKObject)
BEGIN_ADD_PROPERTY(SKUserConstant,SKObject)
REGISTER_PROPERTY(m_uiValueType,ValueType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterIndex,RegisterIndex,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterNum,RegisterNum,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Name,Name,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_NameInShader, NameInShader, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_pDate,m_uiSize,Date);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKUserConstant)
IMPLEMENT_INITIAL_END
SKUserConstant::SKUserConstant()
{
	m_pDate = NULL;
	m_uiRegisterIndex = 0;
	m_uiValueType = SKMAX_INTEGER;
	m_uiRegisterNum = 0;
	m_uiSize = 0;
}
SKUserConstant::SKUserConstant(const SKUsedName & Name,const void * pDate,
							   unsigned int uiSize,
							  unsigned int uiRegisterIndex,unsigned int uiRegisterNum,
							   unsigned int uiValueType)
{
	SKMAC_ASSERT(uiValueType < VT_MAX && uiSize > 0 && uiRegisterNum > 0);

	m_Name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pDate = new unsigned char[uiSize];
	m_uiRegisterIndex = uiRegisterIndex;	
	m_uiRegisterNum = uiRegisterNum;
	SKMAC_ASSERT(m_pDate);

	if(pDate)
	{
		SKMemcpy(m_pDate,pDate,uiSize);
	}
	else
	{
		SKMemset(m_pDate,0,uiSize);
	}
	m_NameInShader = Name;
}
SKUserConstant::SKUserConstant(const SKUsedName & Name, const SKUsedName & NameInShader, const void * pDate,
	unsigned int uiSize,
	unsigned int uiRegisterIndex, unsigned int uiRegisterNum,
	unsigned int uiValueType)
{
	SKMAC_ASSERT(uiValueType < VT_MAX && uiSize > 0 && uiRegisterNum > 0);

	m_Name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pDate = new unsigned char[uiSize];
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;
	SKMAC_ASSERT(m_pDate);

	if (pDate)
	{
		SKMemcpy(m_pDate, pDate, uiSize);
	}
	else
	{
		SKMemset(m_pDate, 0, uiSize);
	}
	m_NameInShader = NameInShader;
}
SKUserConstant::~SKUserConstant()
{
	SKMAC_DELETEA(m_pDate);

}


bool SKUserConstant::SetDate(void *pElementDate)
{
	if(!pElementDate)
		return 0;
	void * pDest =(unsigned char * )m_pDate;
	SKMemcpy(pDest,pElementDate,m_uiSize);

	return 1;
}
