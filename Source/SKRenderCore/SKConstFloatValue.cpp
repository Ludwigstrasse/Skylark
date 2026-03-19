#include "SKConstFloatValue.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKConstFloatValue,SKConstValue)
BEGIN_ADD_PROPERTY(SKConstFloatValue,SKConstValue)
REGISTER_PROPERTY(m_Value,Value,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKConstFloatValue)
IMPLEMENT_INITIAL_END


SKConstFloatValue::SKConstFloatValue()
{

}
SKConstFloatValue::SKConstFloatValue(const SKUsedName & ShowName,SKMaterial * pMaterial,unsigned int uiValueNum,bool bIsCustom)
:SKConstValue(ShowName,pMaterial,uiValueNum,bIsCustom)
{
	SKMAC_ASSERT(uiValueNum >0 && uiValueNum < 5);

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ConstFloatValue") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(uiValueNum - 1,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
	if(uiValueNum >=1)
	{
		SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_R);/*OutputName + _T(".x") ;*/
		SKOutputNode * pOutputNode = NULL;
		pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1,OutputNameR,this);
		SKMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);
	}

	if(uiValueNum >=2)
	{
		SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_G);/*OutputName + _T(".y");*/
		SKOutputNode * pOutputNode = NULL;
		pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1,OutputNameG,this);
		SKMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);
	}

	if(uiValueNum >=3)
	{
		SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_B);/*OutputName + _T(".z");*/
		SKOutputNode * pOutputNode = NULL;
		pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1,OutputNameB,this);
		SKMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);
	}

	if(uiValueNum >=4)
	{

		SKString OutputNameA = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_A);/*OutputName + _T(".w") ;*/
		SKOutputNode * pOutputNode = NULL;
		pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1,OutputNameA,this);
		SKMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);

	}
	m_Value.Clear();
	m_Value.SetBufferNum(uiValueNum);
	for (unsigned int i = 0 ; i < uiValueNum ; i++)
	{
		m_Value[i] = 0;
	}
	m_bIsCustom = bIsCustom;
}
SKConstFloatValue::~SKConstFloatValue()
{
	m_Value.Clear();
}
void SKConstFloatValue::ResetInShaderName()
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ConstFloatValue") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
	if (m_Value.GetNum() >= 1)
	{
		SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_R);
		m_pOutput[1]->SetNodeName(OutputNameR);
	}

	if (m_Value.GetNum() >= 2)
	{
		SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_G);
		m_pOutput[2]->SetNodeName(OutputNameG);
	}

	if (m_Value.GetNum() >= 3)
	{
		SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_B);
		m_pOutput[3]->SetNodeName(OutputNameB);
	}

	if (m_Value.GetNum() >= 4)
	{
		SKString OutputNameA = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE),SKRenderer::VE_A);
		m_pOutput[4]->SetNodeName(OutputNameA);
	}

}
bool SKConstFloatValue::GetDeclareString(SKString &OutString,unsigned int uiRegisterID)const
{
	if(!m_bIsCustom)
		return 1;


	OutString += SKRenderer::ms_pRenderer->GetDeclareValue(m_pOutput[0]->GetNodeName().GetString(), m_pOutput[0]->GetValueType(), uiRegisterID);


	return 1;
}
bool SKConstFloatValue::GetOutPutValueString(SKString &OutString)const
{
	if(m_bIsCustom)
		return 1;
	if(m_pOutput[0]->GetValueType() == SKPutNode::VT_1)
	{
		OutString +=SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
	}
	else if(m_pOutput[0]->GetValueType() == SKPutNode::VT_2)
	{
		OutString +=SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
	}
	else if(m_pOutput[0]->GetValueType() == SKPutNode::VT_3)
	{
		OutString +=SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
	}
	else if(m_pOutput[0]->GetValueType() == SKPutNode::VT_4)
	{
		OutString +=SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
	}
	else
		return 0;

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(";\n");

	return 1;
}
bool SKConstFloatValue::GetFuntionString(SKString &OutString)const
{
	if(m_bIsCustom)
		return 1;
	SKString Value[4];
	for(unsigned int i = 0 ; i < m_Value.GetNum() ; i++)
	{
		Value[i] = RealToString(m_Value[i]);
	}
	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ");
	if(m_pOutput[0]->GetValueType() == SKPutNode::VT_1)
	{
		OutString +=SKRenderer::ms_pRenderer->FloatConst(Value[0]);
	}
	else if(m_pOutput[0]->GetValueType() == SKPutNode::VT_2)
	{
		OutString +=SKRenderer::ms_pRenderer->Float2Const(Value[0],Value[1]);
	}
	else if(m_pOutput[0]->GetValueType() == SKPutNode::VT_3)
	{
		OutString +=SKRenderer::ms_pRenderer->Float3Const(Value[0],Value[1],Value[2]);
	}
	else if(m_pOutput[0]->GetValueType() == SKPutNode::VT_4)
	{
		OutString +=SKRenderer::ms_pRenderer->Float4Const(Value[0],Value[1],Value[2],Value[3]);
	}

	OutString += _T(";\n");

	return 1;
}
void SKConstFloatValue::SetValue(unsigned int uiIndex,SKREAL Value)
{
	if(uiIndex >= m_Value.GetNum())
		return ;
	m_Value[uiIndex] = Value;
}