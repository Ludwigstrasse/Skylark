#include "SKConstValue.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKConstValue,SKShaderFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKConstValue)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKConstValue,SKShaderFunction)
REGISTER_PROPERTY(m_bIsCustom,IsCustom,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
SKConstValue::SKConstValue()
{

}
SKConstValue::SKConstValue(const SKUsedName & ShowName,SKMaterial * pMaterial,unsigned int uiValueNum,bool bIsCustom)
				:SKShaderFunction(ShowName,pMaterial)
{
	m_bIsCustom = bIsCustom;
}
SKConstValue::~SKConstValue()
{
}


