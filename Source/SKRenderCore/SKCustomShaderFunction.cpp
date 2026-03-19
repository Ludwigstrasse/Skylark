#include "SKCustomShaderFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKNormalFunction.h"
#include "SKLightShaderFunction.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKCustomShaderFunction,SKShaderMainFunction)
BEGIN_ADD_PROPERTY(SKCustomShaderFunction,SKShaderMainFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCustomShaderFunction)
IMPLEMENT_INITIAL_END
SKCustomShaderFunction::SKCustomShaderFunction(const SKUsedName &ShowName,SKMaterial * pMaterial)
:SKShaderMainFunction(ShowName,pMaterial)
{

	SKString InputName = _T("EmissiveColor");
	SKInputNode * pInputNode = NULL;

	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);



	InputName = _T("Normal");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	InputName = _T("Alpha");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_1,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Custom");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("Diffuse");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("ReflectMip");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_1,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("ReflectPow");
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
bool SKCustomShaderFunction::GetFuntionString(SKString &OutString)const
{

	GetAlphaTestString(OutString);
	if(!m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") + SKRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));

	}
	else
	{

		int iLightNum[SKLight::LT_MAX] = { 0 };	
		for (unsigned int i = 0 ; i < m_MSPara.LightArray.GetNum() ; i++)
		{
			if (m_MSPara.LightArray[i])
			{
				for (unsigned int j = 0 ; j < SKLight::LT_MAX ; j++)
				{
					if (m_MSPara.LightArray[i]->GetLightType() == j)
					{

						iLightNum[j]++;
					}
				}
			}
		}

		SKRenderer::ms_pRenderer->TranLightToTemp(m_MSPara.LightArray,OutString);
		SKRenderer::ms_pRenderer->GetLightFunction(m_MSPara.LightArray,SKShaderStringFactory::ms_WorldPos,OutString);
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") +
			SKRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("0")); 

		
		SKArray<SKString> ShadowStringArray[SKLight::LT_MAX];
		GetLightShadow(m_MSPara,ShadowStringArray);
		for (unsigned int i = 0 ; i < SKLight::LT_MAX ; i++)
		{
			if (!iLightNum[i])
			{
				continue;
			}
			if(i == SKLight::LT_DIRECTION)
			{			


				SKRenderer::ms_pRenderer->CustomDirectionalLight(iLightNum[i],m_CustomDefine,ShadowStringArray[i],OutString);


			}
			else if(i == SKLight::LT_POINT)
			{


				SKRenderer::ms_pRenderer->CustomPointLight(iLightNum[i],m_CustomDefine,SKShaderStringFactory::ms_WorldPos,ShadowStringArray[i],OutString);

			}
			else if(i == SKLight::LT_SPOT)
			{

				SKRenderer::ms_pRenderer->CustomSpotLight(iLightNum[i],m_CustomDefine,SKShaderStringFactory::ms_WorldPos,ShadowStringArray[i],OutString);

			}

		}
		OutString += _T(";\n");
	}

	GetSRGBWriteString(OutString);

	SKString NodeStringA = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_A);
	OutString += NodeStringA + _T(" = ") + m_pInput[IN_ALPHA]->GetNodeName().GetString();
	OutString += _T(";\n");
	OutString +=  SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return 1;

}
SKCustomShaderFunction::~SKCustomShaderFunction()
{

}
SKCustomShaderFunction::SKCustomShaderFunction()
{
	
}
bool SKCustomShaderFunction::IsValidNodeToThis(SKShaderFunction * pShaderFunction)
{
	if(pShaderFunction->GetType().IsSameType(SKWorldNormal::ms_Type) || pShaderFunction->GetType().IsSameType(SKViewNormal::ms_Type))
	{
		if(GetNormalNode()->GetOutputLink())
		{
			SKShaderFunction * pOwner = (SKShaderFunction *)GetNormalNode()->GetOutputLink()->GetOwner();

			if (pOwner->HaveThisChild(pShaderFunction) == true)
			{
				return false;
			}
		}
	}

	if (pShaderFunction->GetType().IsSameType(SKLightDir::ms_Type) || pShaderFunction->GetType().IsSameType(SKLightColor::ms_Type) || pShaderFunction->GetType().IsSameType(SKLightSpecular::ms_Type))
	{
		if(m_pInput[IN_CUSTOM]->GetOutputLink())
		{
			SKShaderFunction * pOwner = (SKShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner();

			if (pOwner->HaveThisChild(pShaderFunction) == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}
void SKCustomShaderFunction::CreatLightFunctionString(unsigned int uiOutPutStringType)
{
	if (uiOutPutStringType != OST_MATERIAL)
	{
		return ;
	}
	SKString CustomFuntionString;
	SKArray<SKShaderFunction *>  NoLightFunctionParentNodeArray;
	m_LightFunctionString.Clear();
	m_CustomContentString.Clear();
	m_CustomDefine.Clear();
	if (m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		
		((SKShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner())->GetNoLightFunctionParentNode(NoLightFunctionParentNodeArray);

		for (unsigned int i = 0 ; i < NoLightFunctionParentNodeArray.GetNum() ; i++)
		{
			NoLightFunctionParentNodeArray[i]->GetShaderTreeString(m_CustomContentString);
		}
		

		((SKShaderFunction *)m_pInput[IN_CUSTOM]->GetOutputLink()->GetOwner())->GetShaderTreeString(CustomFuntionString);
	}
	
	unsigned int uiCustomValueType = m_pInput[IN_CUSTOM]->GetValueType();
	SKString Temp;
	if(uiCustomValueType == SKPutNode::VT_4)
	{
		CustomFuntionString +=SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		SKMAC_ASSERT(0);
	}
	if(!m_pInput[IN_CUSTOM]->GetOutputLink())
	{
		CustomFuntionString += m_pInput[IN_CUSTOM]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		CustomFuntionString += GetValueEqualString(m_pInput[IN_CUSTOM]->GetOutputLink(),m_pInput[IN_CUSTOM]);
	}

	SKString CustomDeclareString;
	for (unsigned int i = 0 ; i < NoLightFunctionParentNodeArray.GetNum() ; i++)
	{
		SKString NodeName = NoLightFunctionParentNodeArray[i]->GetOutputNode(0)->GetNodeName().GetString();
		unsigned int VTType = NoLightFunctionParentNodeArray[i]->GetOutputNode(0)->GetValueType();

		SKString TypeString;
		if(VTType == SKPutNode::VT_1)
		{
			TypeString +=SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		}
		else if(VTType == SKPutNode::VT_2)
		{
			TypeString +=SKRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;

		}
		else if(VTType == SKPutNode::VT_3)
		{
			TypeString +=SKRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/

		}
		else if(VTType == SKPutNode::VT_4)
		{
			TypeString +=SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/

		}
		CustomDeclareString += TypeString + NodeName + _T(","); 
		m_CustomDefine += NodeName + _T(","); 

	}
	SKString DirectionLightString;
	DirectionLightString = SKRenderer::ms_pRenderer->Float4() + _T(" CustomDirectionLightFun(") + CustomDeclareString + 
							SKRenderer::ms_pRenderer->Float4() + SKShaderStringFactory::ms_LightColor + _T(",") +
							SKRenderer::ms_pRenderer->Float4() + SKShaderStringFactory::ms_LightSpecular + _T(",") +
							SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_LightDir + _T(")");
	DirectionLightString += _T("\n{\n") + CustomFuntionString + SKRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + _T(";}\n");

	SKString PointLightString;
	PointLightString = SKRenderer::ms_pRenderer->Float4() + _T(" CustomPointLightFun(") + CustomDeclareString + 
							SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_WorldPos + _T(",") +
							SKRenderer::ms_pRenderer->Float4() + SKShaderStringFactory::ms_LightColor + _T(",") +
							SKRenderer::ms_pRenderer->Float4() + SKShaderStringFactory::ms_LightSpecular + _T(",") +
							SKRenderer::ms_pRenderer->Float() +_T("Range") + _T(",") +
							SKRenderer::ms_pRenderer->Float3() + _T("LightWorldPos") + _T(")");

	SKString  PointLightUseString ;
	SKString  PointAttenuationDivString ;
	SKRenderer::ms_pRenderer->CustomPointLightUseString(PointLightUseString,PointAttenuationDivString);
	PointLightString += _T("\n{\n") + PointLightUseString + CustomFuntionString + SKRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + PointAttenuationDivString + _T(";}\n");



	SKString SpotLightString;
	SpotLightString = SKRenderer::ms_pRenderer->Float4() + _T(" CustomSpotLightFun(") + CustomDeclareString + 
						SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_WorldPos + _T(",") +
						SKRenderer::ms_pRenderer->Float4() + SKShaderStringFactory::ms_LightColor + _T(",") +
						SKRenderer::ms_pRenderer->Float4() + SKShaderStringFactory::ms_LightSpecular + _T(",") +
						SKRenderer::ms_pRenderer->Float() +_T("Range") + _T(",") +
						SKRenderer::ms_pRenderer->Float() +_T("Phi") + _T(",") +
						SKRenderer::ms_pRenderer->Float() +_T("Theta") + _T(",") +
						SKRenderer::ms_pRenderer->Float() +_T("Falloff") + _T(",") +
						SKRenderer::ms_pRenderer->Float3() +_T("LightWorldPos") + _T(",") +
						SKRenderer::ms_pRenderer->Float3() + _T("LightWorldDirection") + _T(")");
	SKString  SpotLightUseString ;
	SKString  SpotAttenuationDivString ;
	SKRenderer::ms_pRenderer->CustomSpotLightUseString(SpotLightUseString,SpotAttenuationDivString);
	SpotLightString += _T("\n{\n") + SpotLightUseString + CustomFuntionString + SKRenderer::ms_pRenderer->Return() + m_pInput[IN_CUSTOM]->GetNodeName().GetString() + SpotAttenuationDivString + _T(";}\n");

	m_LightFunctionString = DirectionLightString + PointLightString + SpotLightString;
	
}
bool SKCustomShaderFunction::GetInputValueString(SKString &OutString,unsigned int uiOutPutStringType)const
{
	if(!SKRenderer::ms_pRenderer)
		return 0;
	SKString Temp;
	if (uiOutPutStringType == OST_MATERIAL)
	{
		for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
		{

			if (m_pInput[i] == GetNormalNode() || i == IN_CUSTOM)
			{
				continue;
			}
			if(m_pInput[i]->GetValueType() == SKPutNode::VT_1)
			{
				OutString +=SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
				if (m_pInput[i] == GetAlphaNode())
				{
					Temp = SKRenderer::ms_pRenderer->FloatConst(_T("1"));
				}
				else
				{
					Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
				}

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
				return 0;
			if(!m_pInput[i]->GetOutputLink())
			{
				OutString += m_pInput[i]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
				continue;
			}
			// 		if(m_pInput[i]->GetValueType() != m_pInput[i]->GetOutputLink()->GetValueType())
			// 			return 0;
			OutString += GetValueEqualString(m_pInput[i]->GetOutputLink(),m_pInput[i]);/*m_pInput[i]->GetNodeName() + _T(" = ") + m_pInput[i]->GetOutputLink()->GetNodeName() + _T(";\n");*/
		}
	}
	else if (uiOutPutStringType == OST_INDIRECT)
	{

		GetEmissiveInputValueString(OutString);
		GetAlphaInputValueString(OutString);
		GetDiffuseInputValueString(OutString);
	}
	else if (uiOutPutStringType == OST_NORMAL_DEPTH)
	{
		GetAlphaInputValueString(OutString);
		GetReflectMipInputValueString(OutString);
		GetReflectPowInputValueString(OutString);
	}
	else if (uiOutPutStringType == OST_CUB_SHADOW || uiOutPutStringType == OST_SHADOW || uiOutPutStringType == OST_DUAL_PARABOLOID_SHADOW)
	{
		GetAlphaInputValueString(OutString);
	}
	return 1;
}
bool SKCustomShaderFunction::GetShaderTreeString(SKString &OutString,unsigned int uiOutPutStringType)
{
	if(m_bIsVisited == 1)
		return 1;
	else
	{
		m_bIsVisited = 1;

		
		if (uiOutPutStringType == OST_MATERIAL)
		{
			GetValueUseDeclareString(OutString, VUS_ALL);
			// first get normal string
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_ALL);

			GetAlphaString(OutString);
			
			GetEmissiveString(OutString);
			if (m_pInput[IN_CUSTOM]->GetOutputLink())
			{
				OutString += m_CustomContentString;
			}
		}
		else if (uiOutPutStringType == OST_INDIRECT)
		{
			GetValueUseDeclareString(OutString, VUS_ALL);
			// first get normal string
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_ALL);

			GetAlphaString(OutString);

			GetEmissiveString(OutString);
			GetDiffuseString(OutString);
		}
		else if (uiOutPutStringType == OST_NORMAL_DEPTH)
		{

			GetValueUseDeclareString(OutString, VUS_ALL);
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_ALL);
			GetAlphaString(OutString);
			GetReflectMipString(OutString);
			GetReflectPowString(OutString);
		}
		else if (uiOutPutStringType == OST_CUB_SHADOW || uiOutPutStringType == OST_SHADOW || uiOutPutStringType == OST_DUAL_PARABOLOID_SHADOW)
		{
			GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL);
			GetAlphaString(OutString);
		}

		if(!GetInputValueString(OutString,uiOutPutStringType))
			return 0;
		if(!GetOutPutValueString(OutString))
			return 0;
		if (uiOutPutStringType == OST_MATERIAL)
		{
			if(!GetFuntionString(OutString))
				return 0;
		}
		else if (uiOutPutStringType == OST_INDIRECT)
		{
			if (!GetIndirectRenderString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_NORMAL_DEPTH)
		{
			if (!GetNormalDepthString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_CUB_SHADOW)
		{
			if (!GetCubShadowString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_SHADOW)
		{
			if (!GetShadowString(OutString))
			{
				return 0;
			}
		}
		else if (uiOutPutStringType == OST_DUAL_PARABOLOID_SHADOW)
		{
			if (!GetSDualParaboloidhadowString(OutString))
			{
				return 0;
			}
		}
		return 1;
	}
}