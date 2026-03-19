#include "SKCameraShaderFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;

IMPLEMENT_RTTI(SKViewWorldDir,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKViewWorldDir,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKViewWorldDir)
IMPLEMENT_INITIAL_END
SKViewWorldDir::SKViewWorldDir(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ViewWorldDirOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKViewWorldDir::SKViewWorldDir()
{

}
SKViewWorldDir::~SKViewWorldDir()
{

}
void SKViewWorldDir::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("ViewWorldDirOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKViewWorldDir::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_ViewWorldDir + _T(";\n");

	return 1;
}


IMPLEMENT_RTTI(SKCameraWorldPos,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKCameraWorldPos,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCameraWorldPos)
IMPLEMENT_INITIAL_END
SKCameraWorldPos::SKCameraWorldPos(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("CameraWorldPosOutPut") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKCameraWorldPos::SKCameraWorldPos()
{

}
SKCameraWorldPos::~SKCameraWorldPos()
{

}
void SKCameraWorldPos::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("CameraWorldPosOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKCameraWorldPos::GetFuntionString(SKString &OutString)const
{

	OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_CameraWorldPos + _T(";\n");

	return 1;
}

