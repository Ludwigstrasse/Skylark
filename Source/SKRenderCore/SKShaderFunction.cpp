#include "SKShaderFunction.h"
#include "SKMaterial.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKLightShaderFunction.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKShaderFunction,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKShaderFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKShaderFunction,SKObject)
REGISTER_PROPERTY(m_ShowName,ShowName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pOwner,Owner,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pInput,Input,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pOutput,Output,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
SKShaderFunction::SKShaderFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_bIsVisited = 0;
	m_pOwner = NULL;
}
SKShaderFunction::SKShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial)
{
	m_bIsVisited = 0;
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	SKMAC_ASSERT(pMaterial);
	m_pOwner = pMaterial;
	m_pOwner->AddShaderFuntion(this);
}
SKShaderFunction::~SKShaderFunction()
{
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pInput[i]);
	}
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pOutput[i]);
	}
	m_pOwner->DeleteShaderFuntion(this);
}
bool SKShaderFunction::GetInputValueString(SKString &OutString)const
{
	if(!SKRenderer::ms_pRenderer)
		return 0;
	SKString Temp;
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		
		if(m_pInput[i]->GetValueType() == SKPutNode::VT_1)
		{
			OutString +=SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
			Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
		}
		else if(m_pInput[i]->GetValueType() == SKPutNode::VT_2)
		{
			OutString +=SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ");*/
			Temp = SKRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
		}
		else if(m_pInput[i]->GetValueType() == SKPutNode::VT_3)
		{
			OutString +=SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
			Temp = SKRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
		}
		else if(m_pInput[i]->GetValueType() == SKPutNode::VT_4)
		{
			OutString +=SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
			Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
		}
		else
		{
			SKMAC_ASSERT(0);
		}
		if(!m_pInput[i]->GetOutputLink())
		{
			OutString += m_pInput[i]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
			continue;
		}
// 		if(m_pInput[i]->GetValueType() != m_pInput[i]->GetOutputLink()->GetValueType())
// 			return 0;
		OutString += GetValueEqualString(m_pInput[i]->GetOutputLink(),m_pInput[i]);/*m_pInput[i]->GetNodeName() + _T(" = ") + m_pInput[i]->GetOutputLink()->GetNodeName() + _T(";\n");*/
	}
	return 1;
}
SKString SKShaderFunction::GetValueEqualString(const SKOutputNode * pOutPutNode,const SKInputNode * pInputNode)const
{
	if(!pInputNode || !pOutPutNode)
		return SKString();
	unsigned int uiMaxElement = pInputNode->GetValueType();
	if(uiMaxElement == pOutPutNode->GetValueType())
	{
		
		return pInputNode->GetNodeName().GetString() + _T(" = ") + pOutPutNode->GetNodeName().GetString() + _T(";\n");
	}
	
	SKString OutString;
	OutString = pInputNode->GetNodeName().GetString() + _T(" = ");
	SKString Value[4];
	unsigned int Mask[4];
	Mask[0] = SKRenderer::VE_R;
	Mask[1] = SKRenderer::VE_G;
	Mask[2] = SKRenderer::VE_B;
	Mask[3] = SKRenderer::VE_A;
	for(unsigned int i = 0 ; i < 4 ; i ++)
	{
		if ( i > pOutPutNode->GetValueType())
		{
			Value[i] = SKRenderer::ms_pRenderer->GetValueElement(pOutPutNode,Mask[pOutPutNode->GetValueType()]);
		}
		else
		{
			Value[i] = SKRenderer::ms_pRenderer->GetValueElement(pOutPutNode,Mask[i]);
		}

	}
	if (pInputNode->GetValueType() == SKPutNode::VT_1)
	{
		OutString +=SKRenderer::ms_pRenderer->FloatConst(Value[0]);
	}
	else if (pInputNode->GetValueType() == SKPutNode::VT_2)
	{
		OutString +=SKRenderer::ms_pRenderer->Float2Const(Value[0],Value[1]);
	}
	else if (pInputNode->GetValueType() == SKPutNode::VT_3)
	{
		OutString +=SKRenderer::ms_pRenderer->Float3Const(Value[0],Value[1],Value[2]);
	}
	else if (pInputNode->GetValueType() == SKPutNode::VT_4)
	{
		OutString +=SKRenderer::ms_pRenderer->Float4Const(Value[0],Value[1],Value[2],Value[3]);
	}
	else
		return SKString();
	OutString += _T(";\n");
	return OutString;
}
bool SKShaderFunction::GetOutPutValueString(SKString &OutString)const
{
	if(!SKRenderer::ms_pRenderer)
		return 0;
	SKString Temp;
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{

		if(m_pOutput[i]->GetValueType() == SKPutNode::VT_1)
		{
			OutString +=SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
			Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
		}
		else if(m_pOutput[i]->GetValueType() == SKPutNode::VT_2)
		{
			OutString +=SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
			Temp = SKRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
		}
		else if(m_pOutput[i]->GetValueType() == SKPutNode::VT_3)
		{
			OutString +=SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
			Temp = SKRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
		}
		else if(m_pOutput[i]->GetValueType() == SKPutNode::VT_4)
		{
			OutString +=SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
			Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
		}
		else
			return 0;

		OutString += m_pOutput[i]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	return 1;
}
bool SKShaderFunction::GetShaderTreeString(SKString &OutString)
{
	if(m_bIsVisited == 1)
		return 1;
	else
	{
		m_bIsVisited = 1;
		for(unsigned int i = 0 ; i < m_pInput.GetNum(); i++)
		{
			if(m_pInput[i]->GetOutputLink() == NULL)
				continue;
			else
			{
				((SKShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
			
			}
		
		}
		if (!ResetValueType())
		{
			return 0;
		}
		if(!GetInputValueString(OutString))
			return 0;
		if(!GetOutPutValueString(OutString))
			return 0;
		if(!GetFuntionString(OutString))
			return 0;
		return 1;
	}
}
bool SKShaderFunction::ClearShaderTreeStringFlag()
{
	if(m_bIsVisited == 0)
		return 1;
	else
	{
		m_bIsVisited = 0;
		for(unsigned int i = 0 ; i < m_pInput.GetNum(); i++)
		{
			if(m_pInput[i]->GetOutputLink() == NULL)
				continue;
			else
			{
				((SKShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->ClearShaderTreeStringFlag();

			}

		}
		return 1;
	}
}
SKInputNode * SKShaderFunction::GetInputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pInput.GetNum())
		return NULL;
	return m_pInput[uiNodeID];
}
SKInputNode * SKShaderFunction::GetInputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

SKOutputNode * SKShaderFunction::GetOutputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pOutput.GetNum())
		return NULL;
	return m_pOutput[uiNodeID];
}
SKOutputNode * SKShaderFunction::GetOutputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}
bool SKShaderFunction::IsValidNodeToThis(SKShaderFunction * pShaderFunction)
{
	if (pShaderFunction == this)
	{
		return false;
	}
	return true;
}
bool SKShaderFunction::GetAllChildNode(SKArray<SKShaderFunction *> & ChildNodeArray)
{

	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			SKShaderFunction * pOwner = (SKShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			bool bFind = false;
			for (unsigned int j = 0 ; j < ChildNodeArray.GetNum() ; j++)
			{
				if (ChildNodeArray[j] == pOwner)
				{
					bFind = true;
				}
			}
			if(!bFind)
			{
				ChildNodeArray.AddElement(pOwner);
				pOwner->GetAllChildNode(ChildNodeArray);
			}
		}
	}
	return ChildNodeArray.GetNum() > 0;
}
bool SKShaderFunction::CheckChildNodeValidToThis(SKArray<SKShaderFunction *> & NoValidShaderFunctionArray)
{
	SKArray<SKShaderFunction *>  ChildNodeArray;
	if (GetAllChildNode(ChildNodeArray))
	{
		for (unsigned int i = 0 ;  i < ChildNodeArray.GetNum() ; i++)
		{
			if (!IsValidNodeToThis(ChildNodeArray[i]))
			{
				NoValidShaderFunctionArray.AddElement(ChildNodeArray[i]);
			}
		}
	}

	return NoValidShaderFunctionArray.GetNum() == 0;
}
bool SKShaderFunction::CheckChildNodeValidAll(SKMap<SKShaderFunction *,SKArray<SKShaderFunction *>> & NoValidMap)
{
	SKArray<SKShaderFunction *>  NoValidShaderFunctionArray;
	if (!CheckChildNodeValidToThis(NoValidShaderFunctionArray))
	{
		NoValidMap.AddElement(this,NoValidShaderFunctionArray);
	}
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			SKShaderFunction * pOwner = (SKShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			pOwner->CheckChildNodeValidAll(NoValidMap);
		}
	}

	return NoValidMap.GetNum() == 0;
}
bool SKShaderFunction::HaveThisChild(SKShaderFunction * pShaderFunction)
{
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			SKShaderFunction * pOwner = (SKShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pShaderFunction == pOwner)
			{
				return true;
			}
			else
			{
				if (pOwner->HaveThisChild(pShaderFunction) == true)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void SKShaderFunction::GetNoLightFunctionParentNode(SKArray<SKShaderFunction *> & NoLightFunctionParentNodeArray)
{
	SKArray<SKShaderFunction *> ChildNodeArray;
	GetAllChildNode(ChildNodeArray);

	bool bHaveLightNode = false;
	for (unsigned int i = 0 ; i < ChildNodeArray.GetNum() ; i++)
	{
		SKShaderFunction * pShaderFunction = ChildNodeArray[i];
		if (pShaderFunction->GetType().IsSameType(SKLightDir::ms_Type) || pShaderFunction->GetType().IsSameType(SKLightColor::ms_Type) || pShaderFunction->GetType().IsSameType(SKLightSpecular::ms_Type))
		{
			bHaveLightNode = true;
			break;
		}
		
	}
	if (!bHaveLightNode)
	{
		if (GetType().IsSameType(SKLightDir::ms_Type) || GetType().IsSameType(SKLightColor::ms_Type) || GetType().IsSameType(SKLightSpecular::ms_Type))
		{
			return;
		}	
		else
		{
			NoLightFunctionParentNodeArray.AddElement(this);
		}
		return ;
	}
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			SKShaderFunction * pOwner = (SKShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pOwner)
			{
				pOwner->GetNoLightFunctionParentNode(NoLightFunctionParentNodeArray);

			}
		}
	}
}
bool SKShaderFunction::ResetValueType()
{
	return true;
}
