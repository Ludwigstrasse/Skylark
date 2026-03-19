#include "SKNormalFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKWorldNormal,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKWorldNormal,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKWorldNormal)
IMPLEMENT_INITIAL_END
SKWorldNormal::SKWorldNormal(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("WorldNormalOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_3,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_R);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameR, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_G);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameG, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_B);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameB, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
}
SKWorldNormal::SKWorldNormal()
{

}
SKWorldNormal::~SKWorldNormal()
{

}
void SKWorldNormal::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("WorldNormalOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool SKWorldNormal::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_WorldNormal + _T(";\n");

	return 1;
}
bool SKWorldNormal::GetOutPutValueString(SKString &OutString)const
{
	SKString Temp;
	if (m_pOutput[0]->GetValueType() == SKPutNode::VT_1)
	{
		OutString += SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pOutput[0]->GetValueType() == SKPutNode::VT_2)
	{
		OutString += SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
		Temp = SKRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == SKPutNode::VT_3)
	{
		OutString += SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = SKRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == SKPutNode::VT_4)
	{
		OutString += SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}

IMPLEMENT_RTTI(SKViewNormal,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKViewNormal,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKViewNormal)
IMPLEMENT_INITIAL_END
SKViewNormal::SKViewNormal(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ViewNormalOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_R);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameR, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_G);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameG, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_B);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameB, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
}
SKViewNormal::SKViewNormal()
{

}
SKViewNormal::~SKViewNormal()
{

}
void SKViewNormal::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ViewNormalOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool SKViewNormal::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_ViewNormal + _T(";\n");

	return 1;
}
bool SKViewNormal::GetOutPutValueString(SKString &OutString)const
{
	SKString Temp;
	if (m_pOutput[0]->GetValueType() == SKPutNode::VT_1)
	{
		OutString += SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pOutput[0]->GetValueType() == SKPutNode::VT_2)
	{
		OutString += SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
		Temp = SKRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == SKPutNode::VT_3)
	{
		OutString += SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = SKRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pOutput[0]->GetValueType() == SKPutNode::VT_4)
	{
		OutString += SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}