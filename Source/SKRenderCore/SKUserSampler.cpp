#include "SKUserSampler.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKUserSampler,SKObject)
BEGIN_ADD_PROPERTY(SKUserSampler,SKObject)
REGISTER_PROPERTY(m_uiTexType,TexType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Name,Name,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterIndex,RegisterIndex,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterNum,RegisterNum,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKUserSampler)
IMPLEMENT_INITIAL_END
SKUserSampler::SKUserSampler()
{
	m_uiTexType = SKTexture::TT_2D;
	m_pTextureArray.Clear();
	m_uiRegisterNum = 1;
}
SKUserSampler::~SKUserSampler()
{
	m_pTextureArray.Clear();

}
SKUserSampler::SKUserSampler(const SKUsedName & Name,unsigned int uiTexType,unsigned int uiRegisterIndex,unsigned int uiRegisterNum)
{
	m_Name = Name;
	m_uiTexType = uiTexType;
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;
	m_pTextureArray.SetBufferNum(uiRegisterNum);
}
bool SKUserSampler::PostLoad(void * pDate)
{
	SKObject::PostLoad(pDate);
	m_pTextureArray.SetBufferNum(m_uiRegisterNum);
	return true;
}
bool SKUserSampler::PostClone(SKObject * pObjectSrc)
{
	SKObject::PostClone(pObjectSrc);
	m_pTextureArray.SetBufferNum(m_uiRegisterNum);
	return true;
}
