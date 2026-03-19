#include "SKVertexColor.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKVertexColor,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKVertexColor,SKShaderFunction)
REGISTER_PROPERTY(m_uiColorIndex,ColorIndex,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKVertexColor)
IMPLEMENT_INITIAL_END
SKVertexColor::SKVertexColor(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("VertexColorOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
	m_uiColorIndex = 0;
}
SKVertexColor::SKVertexColor()
{
	m_uiColorIndex = 0;
}
SKVertexColor::~SKVertexColor()
{

}
void SKVertexColor::ResetInShaderName()
{
	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("VertexColorOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKVertexColor::GetFuntionString(SKString &OutString)const
{
	if (SKShaderStringFactory::ms_PSColor[m_uiColorIndex].GetLength() > 0)
	{
		OutString += GetOutputNode(OUT_COLOR)->GetNodeName().GetString() + _T(" = ") + SKShaderStringFactory::ms_PSColor[m_uiColorIndex] + _T(";\n");
	}
	return 1;
}
