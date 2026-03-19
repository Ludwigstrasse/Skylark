#include "SKLightShaderFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKPosShaderFunction.h"
#include "SKNormalFunction.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKLightColor,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKLightColor,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKLightColor)
IMPLEMENT_INITIAL_END
SKLightColor::SKLightColor(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("LightColorOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKLightColor::SKLightColor()
{

}
SKLightColor::~SKLightColor()
{

}
void SKLightColor::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("LightColorOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKLightColor::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_LightColor+ _T(";\n");

	return 1;
}

IMPLEMENT_RTTI(SKLightSpecular,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKLightSpecular,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKLightSpecular)
IMPLEMENT_INITIAL_END
SKLightSpecular::SKLightSpecular(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("LightSpecularOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKLightSpecular::SKLightSpecular()
{

}
SKLightSpecular::~SKLightSpecular()
{

}
void SKLightSpecular::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("LightSpecularOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKLightSpecular::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_LightSpecular + _T(";\n");

	return 1;
}


IMPLEMENT_RTTI(SKLightDir,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKLightDir,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKLightDir)
IMPLEMENT_INITIAL_END
SKLightDir::SKLightDir(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("LightDirOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_3,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKLightDir::SKLightDir()
{

}
SKLightDir::~SKLightDir()
{

}
void SKLightDir::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("LightDirOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKLightDir::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_LightDir + _T(";\n");

	return 1;
}

IMPLEMENT_RTTI(SKLightShaderFunction,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKLightShaderFunction,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKLightShaderFunction)
IMPLEMENT_INITIAL_END
SKLightShaderFunction::SKLightShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString InputName = _T("DiffuseColor");
	SKInputNode * pInputNode = NULL;

	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	InputName = _T("Alpha");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_1,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	SKString OutputName = _T("OutputColor");
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
}
SKLightShaderFunction::SKLightShaderFunction()
{

}
SKLightShaderFunction::~SKLightShaderFunction()
{

}
bool SKLightShaderFunction::GetFuntionString(SKString &OutString)const
{


	SKString NodeStringA = SKRenderer::ms_pRenderer->GetValueElement(m_pInput[IN_DIFFUSE_COLOR],SKRenderer::VE_A);
	OutString += NodeStringA + _T(" = ") + m_pInput[IN_ALPHA]->GetNodeName().GetString();
	OutString += _T(";\n");
	OutString +=  SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString() + _T(";\n");

	return 1;
}
bool SKLightShaderFunction::IsValidNodeToThis(SKShaderFunction * pShaderFunction)
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