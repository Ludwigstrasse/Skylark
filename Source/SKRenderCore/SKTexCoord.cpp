#include "SKTexCoord.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKTexCoord, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKTexCoord, SKShaderFunction)
REGISTER_PROPERTY(m_uiTexCoordIndex, exCoordIndex, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKTexCoord)
IMPLEMENT_INITIAL_END
SKTexCoord::SKTexCoord(const SKUsedName & ShowName, SKMaterial * pMaterial)
:SKShaderFunction(ShowName, pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("TexCoordOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_2, OutputName, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
	m_uiTexCoordIndex = 0;
}
SKTexCoord::SKTexCoord()
{
	m_uiTexCoordIndex = 0;
}
SKTexCoord::~SKTexCoord()
{

}
void SKTexCoord::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("TexCoordOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKTexCoord::GetFuntionString(SKString &OutString)const
{
	if (SKShaderStringFactory::ms_PSTextureInputCoordValue[m_uiTexCoordIndex].GetLength() > 0)
	{
		OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_PSTextureInputCoordValue[m_uiTexCoordIndex] + _T(";\n");
	}
	return 1;
}
