#include "SKTexSampler.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKTexSampler,SKShaderFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKTexSampler)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKTexSampler,SKShaderFunction)
REGISTER_PROPERTY(m_pTexAllState,Texture,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiTexCoordLevel,TexCoordLevel,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiVEDecode,VEDecode,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiVESRGB,VESRGB,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
SKTexSampler::~SKTexSampler()
{
	m_pTexAllState = NULL;
	m_uiTexCoordLevel = 0;
}
SKTexSampler::SKTexSampler(const SKUsedName & ShowName,SKMaterial * pMaterial)
			 :SKShaderFunction(ShowName,pMaterial)
{
	m_pTexAllState = NULL;
	m_uiTexCoordLevel = 0;
	m_uiVEDecode = 0;
	m_uiVESRGB = 0;
}
SKTexSampler::SKTexSampler()
{
	m_pTexAllState = NULL;
	m_uiTexCoordLevel = 0;
	m_uiVEDecode = 0;
	m_uiVESRGB = 0;
}
bool SKTexSampler::GetDeclareString(SKString &OutString,unsigned int uiRegisterID)const
{
	OutString += SKRenderer::ms_pRenderer->GetDeclareSampler(m_ShowName.GetString(), GetTexType(), uiRegisterID);
	return true;
}
bool SKTexSampler::GetInputValueString(SKString &OutString)const
{
	if(!SKRenderer::ms_pRenderer)
		return 0;
	SKString Temp;
	if(m_pInput[0]->GetValueType() == SKPutNode::VT_1)
	{
		OutString +=SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if(m_pInput[0]->GetValueType() == SKPutNode::VT_2)
	{
		OutString +=SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ");*/
		Temp = SKRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
	}
	else if(m_pInput[0]->GetValueType() == SKPutNode::VT_3)
	{
		OutString +=SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = SKRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
	}
	else if(m_pInput[0]->GetValueType() == SKPutNode::VT_4)
	{
		OutString +=SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;
	if(!m_pInput[0]->GetOutputLink())
	{
// 		if(!m_pTexAllState)
// 		{
// 			OutString += m_pInput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
// 			return 1;
// 		}
// 		else
// 		{
			OutString += m_pInput[0]->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_PSTextureInputCoordValue[m_uiTexCoordLevel] + _T(";\n");
			return 1;
/*		}*/
	}
// 	if(m_pInput[0]->GetValueType() != m_pInput[0]->GetOutputLink()->GetValueType())
// 		return 0;
	OutString += GetValueEqualString(m_pInput[0]->GetOutputLink(),m_pInput[0]);/*m_pInput[0]->GetNodeName() + _T(" = ") + m_pInput[0]->GetOutputLink()->GetNodeName() + _T(";\n");*/

	return 1;
}
