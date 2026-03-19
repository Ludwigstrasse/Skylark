#include "SKPosShaderFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKWorldPos,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKWorldPos,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKWorldPos)
IMPLEMENT_INITIAL_END
SKWorldPos::SKWorldPos(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("WorldPosOutput") + OutputID;
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
SKWorldPos::SKWorldPos()
{

}
SKWorldPos::~SKWorldPos()
{

}
void SKWorldPos::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("WorldPosOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool SKWorldPos::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_WorldPos + _T(";\n");

	return 1;
}
bool SKWorldPos::GetOutPutValueString(SKString &OutString)const
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

IMPLEMENT_RTTI(SKViewPos,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKViewPos,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKViewPos)
IMPLEMENT_INITIAL_END
SKViewPos::SKViewPos(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ViewPosOutput") + OutputID;
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
SKViewPos::SKViewPos()
{

}
SKViewPos::~SKViewPos()
{

}
void SKViewPos::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ViewPosOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool SKViewPos::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_ViewPos + _T(";\n");

	return 1;
}
bool SKViewPos::GetOutPutValueString(SKString &OutString)const
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
IMPLEMENT_RTTI(SKProjectPos, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKProjectPos, SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKProjectPos)
IMPLEMENT_INITIAL_END
SKProjectPos::SKProjectPos(const SKUsedName & ShowName, SKMaterial * pMaterial)
:SKShaderFunction(ShowName, pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ProjectPosOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_3, OutputName, this);
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
SKProjectPos::SKProjectPos()
{

}
SKProjectPos::~SKProjectPos()
{

}
void SKProjectPos::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ProjectPosOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_VALUE), SKRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);
}
bool SKProjectPos::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_ProjectPos + _T(";\n");

	return 1;
}
bool SKProjectPos::GetOutPutValueString(SKString &OutString)const
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