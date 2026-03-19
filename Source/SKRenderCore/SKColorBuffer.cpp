#include "SKColorBuffer.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKColorBuffer, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKColorBuffer, SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKColorBuffer)
IMPLEMENT_INITIAL_END
SKColorBuffer::SKColorBuffer(const SKUsedName & ShowName, SKMaterial * pMaterial)
:SKShaderFunction(ShowName, pMaterial)
{


	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("ColorBufferInput") + InputID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_2, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ColorBufferOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4, OutputName, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_R);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameR, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_G);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameG, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_B);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameB, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	SKString OutputNameA = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_A);

	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputNameA, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
}
SKColorBuffer::SKColorBuffer()
{

}
SKColorBuffer::~SKColorBuffer()
{

}
void SKColorBuffer::ResetInShaderName()
{
	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("ColorBufferInput") + InputID;
	m_pInput[0]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ColorBufferOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;


	SKString OutputNameR = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	SKString OutputNameG = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	SKString OutputNameB = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);

	SKString OutputNameA = SKRenderer::ms_pRenderer->GetValueElement(GetOutputNode(OUT_COLOR), SKRenderer::VE_A);
	m_pOutput[4]->SetNodeName(OutputNameA);
}
bool SKColorBuffer::GetFuntionString(SKString &OutString)const
{
	if (SKRenderer::ms_pRenderer)
		OutString += SKRenderer::ms_pRenderer->TexColorBuffer(this);
	return 1;
}
bool SKColorBuffer::GetInputValueString(SKString &OutString)const
{
	if (!SKRenderer::ms_pRenderer)
		return 0;
	SKString Temp;
	if (m_pInput[0]->GetValueType() == SKPutNode::VT_1)
	{
		OutString += SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if (m_pInput[0]->GetValueType() == SKPutNode::VT_2)
	{
		OutString += SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ");*/
		Temp = SKRenderer::ms_pRenderer->Float2Const(_T("0"), _T("0"));/*_T("float2(0,0)");*/
	}
	else if (m_pInput[0]->GetValueType() == SKPutNode::VT_3)
	{
		OutString += SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = SKRenderer::ms_pRenderer->Float3Const(_T("0"), _T("0"), _T("0"));/*_T("float3(0,0,0)");*/
	}
	else if (m_pInput[0]->GetValueType() == SKPutNode::VT_4)
	{
		OutString += SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;
	if (!m_pInput[0]->GetOutputLink())
	{
		OutString += m_pInput[0]->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_PSTextureInputCoordValue[0] + _T(";\n");
		return 1;
	}

	OutString += GetValueEqualString(m_pInput[0]->GetOutputLink(), m_pInput[0]);

	return 1;
}
bool SKColorBuffer::GetOutPutValueString(SKString &OutString)const
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