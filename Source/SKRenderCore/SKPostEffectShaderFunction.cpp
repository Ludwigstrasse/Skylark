#include "SKPostEffectShaderFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKPosShaderFunction.h"
#include "SKNormalFunction.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPostEffectShaderFunction, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKPostEffectShaderFunction, SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPostEffectShaderFunction)
IMPLEMENT_INITIAL_END
SKPostEffectShaderFunction::SKPostEffectShaderFunction(const SKUsedName & ShowName, SKMaterial * pMaterial)
:SKShaderFunction(ShowName, pMaterial)
{


	SKString InputName = _T("InColor");
	SKInputNode * pInputNode = NULL;

	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

}
SKPostEffectShaderFunction::SKPostEffectShaderFunction()
{

}
SKPostEffectShaderFunction::~SKPostEffectShaderFunction()
{

}

bool SKPostEffectShaderFunction::GetFuntionString(SKString &OutString)const
{


	OutString += SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pInput[IN_COLOR]->GetNodeName().GetString() + _T(";\n");

	return 1;
}
bool SKPostEffectShaderFunction::IsValidNodeToThis(SKShaderFunction * pShaderFunction)
{
	if (pShaderFunction->GetType().IsSameType(SKLightDir::ms_Type)
		|| pShaderFunction->GetType().IsSameType(SKLightColor::ms_Type)
		|| pShaderFunction->GetType().IsSameType(SKLightSpecular::ms_Type)
		|| pShaderFunction->GetType().IsSameType(SKWorldNormal::ms_Type)
		|| pShaderFunction->GetType().IsSameType(SKViewNormal::ms_Type)
		|| pShaderFunction->GetType().IsSameType(SKWorldPos::ms_Type)
		|| pShaderFunction->GetType().IsSameType(SKViewPos::ms_Type))
	{
		return false;
	}
	return true;
}