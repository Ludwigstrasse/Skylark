#include "SKArithmeticShaderFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;

IMPLEMENT_RTTI(SKMul,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKMul,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMul)
IMPLEMENT_INITIAL_END
SKMul::SKMul(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{

	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("MulInputA") + InputID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
	
	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("MulInputB") + InputID;
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("MulOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKMul::SKMul()
{

}
SKMul::~SKMul()
{

}
bool SKMul::ResetValueType()
{
	unsigned int MaxType = SKPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);

	return true;
}
void SKMul::ResetInShaderName()
{
	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("MulInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("MulInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("MulOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKMul::GetFuntionString(SKString &OutString)const
{

	OutString += SKRenderer::ms_pRenderer->Mul(this);

	return 1;
}


IMPLEMENT_RTTI(SKDot,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKDot,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDot)
IMPLEMENT_INITIAL_END
SKDot::SKDot(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("DotInputA") + InputID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DotInputB") + InputID;
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;



	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("DotOutPut") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKDot::SKDot()
{

}
SKDot::~SKDot()
{

}
void SKDot::ResetInShaderName()
{

	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("DotInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DotInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("DotOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKDot::GetFuntionString(SKString &OutString)const
{

	OutString += SKRenderer::ms_pRenderer->Dot(this);

	return 1;
}
bool SKDot::ResetValueType()
{
	unsigned int MaxType = SKPutNode::VT_2;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}

	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	return true;
}

IMPLEMENT_RTTI(SKSub,SKShaderFunction)
BEGIN_ADD_PROPERTY(SKSub,SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSub)
IMPLEMENT_INITIAL_END
SKSub::SKSub(const SKUsedName & ShowName,SKMaterial * pMaterial)
:SKShaderFunction(ShowName,pMaterial)
{


	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("SubInputA") + InputID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("SubInputB") + InputID;
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("SubOutPut") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

}
SKSub::SKSub()
{

}
SKSub::~SKSub()
{

}
void SKSub::ResetInShaderName()
{

	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("SubInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("SubInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("SubOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKSub::GetFuntionString(SKString &OutString)const
{

	OutString += SKRenderer::ms_pRenderer->Sub(this);

	return 1;
}
bool SKSub::ResetValueType()
{
	unsigned int MaxType = SKPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);
	return true;
}

IMPLEMENT_RTTI(SKAdd, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKAdd, SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAdd)
IMPLEMENT_INITIAL_END
SKAdd::SKAdd(const SKUsedName & ShowName, SKMaterial * pMaterial)
:SKShaderFunction(ShowName, pMaterial)
{


	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("AddInputA") + InputID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("AddInputB") + InputID;
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("AddOutPut") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputName, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

}
SKAdd::SKAdd()
{

}
SKAdd::~SKAdd()
{

}
void SKAdd::ResetInShaderName()
{

	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("AddInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("AddInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("AddOutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKAdd::GetFuntionString(SKString &OutString)const
{

	OutString += SKRenderer::ms_pRenderer->Add(this);

	return 1;
}
bool SKAdd::ResetValueType()
{
	unsigned int MaxType = SKPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);
	return true;
}
IMPLEMENT_RTTI(SKMakeValue, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKMakeValue, SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMakeValue)
IMPLEMENT_INITIAL_END
SKMakeValue::SKMakeValue(const SKUsedName & ShowName, SKMaterial * pMaterial, unsigned int uiInputNum)
:SKShaderFunction(ShowName, pMaterial)
{

	if (uiInputNum >= 1)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputA") + InputID;
		SKInputNode * pInputNode = NULL;
		pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
		SKMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (uiInputNum >= 2)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputB") + InputID;
		SKInputNode * pInputNode = NULL;
		pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
		SKMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (uiInputNum >= 3)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputC") + InputID;
		SKInputNode * pInputNode = NULL;
		pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
		SKMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (uiInputNum >= 4)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputD") + InputID;
		SKInputNode * pInputNode = NULL;
		pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
		SKMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}
	

	

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("OutPut") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_1, OutputName, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;


}
SKMakeValue::SKMakeValue()
{

}
SKMakeValue::~SKMakeValue()
{

}
void SKMakeValue::ResetInShaderName()
{
	if (m_pInput.GetNum() >= 1)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputA") + InputID;
		m_pInput[IN_A]->SetNodeName(InputName);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (m_pInput.GetNum() >= 2)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputB") + InputID;
		m_pInput[IN_B]->SetNodeName(InputName);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}

	if (m_pInput.GetNum() >= 3)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputC") + InputID;
		m_pInput[IN_B]->SetNodeName(InputName);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}
	if (m_pInput.GetNum() >= 4)
	{
		SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
		SKString InputName = _T("InputD") + InputID;
		m_pInput[IN_B]->SetNodeName(InputName);
		SKShaderStringFactory::ms_ShaderValueIndex++;
	}
	

	

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("OutPut") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKMakeValue::GetFuntionString(SKString &OutString)const
{
	OutString += SKRenderer::ms_pRenderer->MakeValue(this);
	return 1;
}
bool SKMakeValue::ResetValueType()
{
	unsigned int MaxType = 0;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			m_pInput[i]->SetValueType(m_pInput[i]->GetOutputLink()->GetValueType());
		}
		else
		{
			m_pInput[i]->SetValueType(SKPutNode::VT_1);
		}
		MaxType += m_pInput[i]->GetValueType() + 1;
	}

	m_pOutput[0]->SetValueType(MaxType - 1);
	return true;
}
IMPLEMENT_RTTI(SKDiv, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKDiv, SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDiv)
IMPLEMENT_INITIAL_END
SKDiv::SKDiv(const SKUsedName & ShowName, SKMaterial * pMaterial)
:SKShaderFunction(ShowName, pMaterial)
{

	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("DivInputA") + InputID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DivInputB") + InputID;
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;



	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("DivOutput") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4, OutputName, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKDiv::SKDiv()
{

}
SKDiv::~SKDiv()
{

}
bool SKDiv::ResetValueType()
{
	unsigned int MaxType = SKPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);

	return true;
}
void SKDiv::ResetInShaderName()
{
	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("DivInputA") + InputID;
	m_pInput[IN_A]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	InputName = _T("DivInputB") + InputID;
	m_pInput[IN_B]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;

	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("DivOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKDiv::GetFuntionString(SKString &OutString)const
{

	OutString += SKRenderer::ms_pRenderer->Div(this);
	return 1;
}


IMPLEMENT_RTTI(SKSaturate, SKShaderFunction)
BEGIN_ADD_PROPERTY(SKSaturate, SKShaderFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSaturate)
IMPLEMENT_INITIAL_END
SKSaturate::SKSaturate(const SKUsedName & ShowName, SKMaterial * pMaterial)
:SKShaderFunction(ShowName, pMaterial)
{

	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("Input") + InputID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;



	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("Output") + OutputID;
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::VT_4, OutputName, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
SKSaturate::SKSaturate()
{

}
SKSaturate::~SKSaturate()
{

}
bool SKSaturate::ResetValueType()
{
	unsigned int MaxType = SKPutNode::VT_1;
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			if (MaxType < m_pInput[i]->GetOutputLink()->GetValueType())
			{
				MaxType = m_pInput[i]->GetOutputLink()->GetValueType();
			}
		}
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		m_pInput[i]->SetValueType(MaxType);
	}
	m_pOutput[0]->SetValueType(MaxType);

	return true;
}
void SKSaturate::ResetInShaderName()
{
	SKString InputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString InputName = _T("Input") + InputID;
	m_pInput[IN_VALUE]->SetNodeName(InputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;


	SKString OutputID = IntToString(SKShaderStringFactory::ms_ShaderValueIndex);
	SKString OutputName = _T("Output") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	SKShaderStringFactory::ms_ShaderValueIndex++;
}
bool SKSaturate::GetFuntionString(SKString &OutString)const
{

	OutString += SKRenderer::ms_pRenderer->Saturate(this);
	return 1;
}