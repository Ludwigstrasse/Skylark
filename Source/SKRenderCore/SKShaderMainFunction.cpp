#include "SKShaderMainFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKLight.h"
#include "SKPointLight.h"
#include "SKDirectionLight.h"
#include "SKNormalFunction.h"
#include "SKPosShaderFunction.h"
#include "SKNormalFunction.h"
#include "SKCameraShaderFunction.h"
#include "SKLightShaderFunction.h"
#include "SKSpotLight.h"
#include "SKColorBuffer.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKShaderMainFunction,SKShaderFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKShaderMainFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKShaderMainFunction,SKShaderFunction)
REGISTER_PROPERTY(m_uiVESRGBWrite,VESRGBWrite,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiPassId,PassId,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RenderState,RenderState,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY


SKShaderMainFunction::SKShaderMainFunction(const SKUsedName &ShowName,SKMaterial * pMaterial)
					:SKShaderFunction(ShowName,pMaterial)
{
	

	m_uiVESRGBWrite = false;
	m_uiPassId = 0;
	m_fAlphaTestValue = 1.0f;
}

SKShaderMainFunction::~SKShaderMainFunction()
{

}
SKShaderMainFunction::SKShaderMainFunction()
{
	m_uiVESRGBWrite = false;
	m_uiPassId = 0;
	m_fAlphaTestValue = 1.0f;
}

bool SKShaderMainFunction::GetShaderTreeString(SKString &OutString,unsigned int uiOutPutStringType)
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
			// second get other
			for(unsigned int i = 0 ; i < m_pInput.GetNum(); i++)
			{
				if(m_pInput[i]->GetOutputLink() == NULL)
					continue;
				else if (m_pInput[i] == GetNormalNode())
				{
					continue;
				}
				else
				{
					((SKShaderFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);

				}

			}
		}
		else if (uiOutPutStringType == OST_INDIRECT)
		{
			GetValueUseDeclareString(OutString, VUS_ALL);
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_ALL);

			GetEmissiveString(OutString);

			GetAlphaString(OutString);
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
		else if (uiOutPutStringType == OST_CUB_SHADOW || uiOutPutStringType == OST_DUAL_PARABOLOID_SHADOW)
		{
			GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL);
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL);
			GetAlphaString(OutString);
		}
		else if (uiOutPutStringType == OST_SHADOW)
		{
			GetValueUseDeclareString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL);
			GetNormalString(OutString);
			GetValueUseString(OutString, VUS_WORLD_POS | VUS_WORLD_NORMAL);
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
void SKShaderMainFunction::GetAlphaTestString(SKString &OutString)const
{
	if (m_fAlphaTestValue < 1.0f)
	{
		SKString AlphaName = GetAlphaNode()->GetNodeName().GetString();
		SKString AlphaTestValue = RealToString(m_fAlphaTestValue);

		SKRenderer::ms_pRenderer->Clip(AlphaName + _T(" - ") + AlphaTestValue, OutString);
		OutString += _T(";\n");

	}
}
void SKShaderMainFunction::GetSRGBWriteString(SKString &OutString)const
{
	if (m_uiVESRGBWrite)
	{
		SKString NodeString = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR], m_uiVESRGBWrite);

		OutString += NodeString + _T(" = ");
		SKRenderer::ms_pRenderer->GreaterZeroPow(NodeString, 1.0f / 2.2f, OutString);
		OutString += _T(";\n");
	}
}
bool SKShaderMainFunction::GetIndirectRenderString(SKString &OutString)const
{
	
	GetAlphaTestString(OutString);
	OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") +
		GetEmissiveNode()->GetNodeName().GetString();

	SKRenderer::ms_pRenderer->SkyLight(SKShaderStringFactory::ms_WorldNormal,SKShaderStringFactory::ms_SkyLightUpColor,SKShaderStringFactory::ms_SkyLightDownColor,GetDiffuseNode()->GetNodeName().GetString(),OutString);
	OutString += _T(";\n");

	GetSRGBWriteString(OutString);

	SKString NodeStringA = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_A);
	OutString += NodeStringA + _T(" = ") + GetAlphaNode()->GetNodeName().GetString();
	OutString += _T(";\n");
	OutString +=  SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return true;
}
void SKShaderMainFunction::GetValueUseDeclareString(SKString &OutString, unsigned int uiValueUseString)
{

	SKString DefaultValue = SKRenderer::ms_pRenderer->Float3Const(_T("0"), _T("1"), _T("0"));
	if ((uiValueUseString & VUS_WORLD_POS) == VUS_WORLD_POS)
		OutString += SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_WorldPos + _T(" = ") + DefaultValue + _T(";\n");
	if ((uiValueUseString & VUS_VIEW_POS) == VUS_VIEW_POS)
		OutString += SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_ViewPos + _T(" = ") + DefaultValue + _T(";\n");
	if ((uiValueUseString & VUS_PROJ_POS) == VUS_PROJ_POS)
		OutString += SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_ProjectPos + _T(" = ") + DefaultValue + _T(";\n");

	if ((uiValueUseString & VUS_WORLD_NORMAL) == VUS_WORLD_NORMAL)
		OutString += SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_WorldNormal + _T(" = ") + DefaultValue + _T(";\n");
	if ((uiValueUseString & VUS_VIEW_NORMAL) == VUS_VIEW_NORMAL)
		OutString += SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_ViewNormal + _T(" = ") + DefaultValue + _T(";\n");


	if ((uiValueUseString & VUS_VIEW_WORLD_DIR) == VUS_VIEW_WORLD_DIR)
		OutString += SKRenderer::ms_pRenderer->Float3() + SKShaderStringFactory::ms_ViewWorldDir + _T(" = ") + DefaultValue + _T(";\n");

}
bool SKShaderMainFunction::GetSDualParaboloidhadowString(SKString &OutString)const
{
	GetAlphaTestString(OutString);

	SKString NodeStringR = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_R);
	OutString += NodeStringR + _T(" = ") + SKShaderStringFactory::ms_PSInputProjectZ;
	OutString += _T(";\n");
// 	SKString NodeStringG = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_G);
// 	OutString += NodeStringG + _T(" = ") + GetAlphaNode()->GetNodeName().GetString();
// 	OutString += _T(";\n");

	
	OutString +=  SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return true;	
}
bool SKShaderMainFunction::GetShadowString(SKString &OutString)const
{

	GetAlphaTestString(OutString);
	OutString += SKRenderer::ms_pRenderer->Float3() + _T("LightProj = ");
	SKRenderer::ms_pRenderer->TransProjPos(SKShaderStringFactory::ms_PSInputLocalPos, SKShaderStringFactory::ms_LightShadowMatrix, OutString);
	OutString += _T(";\n");
	SKString NodeStringR = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR], SKRenderer::VE_R);
	OutString += NodeStringR + _T(" = ") + SKRenderer::ms_pRenderer->GetValueElement(_T("LightProj"), SKRenderer::VE_B);
	OutString += _T(";\n");
// 	SKString NodeStringG = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_G);
// 	OutString += NodeStringG + _T(" = ") + GetAlphaNode()->GetNodeName().GetString();
// 	OutString += _T(";\n");

	
	OutString +=  SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	return true;
}
bool SKShaderMainFunction::GetCubShadowString(SKString &OutString)const
{


	GetAlphaTestString(OutString);

	OutString += SKShaderStringFactory::ms_WorldPos + _T(" = ");
	SKRenderer::ms_pRenderer->LocalToWorldPos(SKShaderStringFactory::ms_PSInputLocalPos,OutString);
	OutString += _T(";\n");
	OutString += SKRenderer::ms_pRenderer->Float() + _T("WorldCameraLength = ");
	SKRenderer::ms_pRenderer->ComputeLength(SKShaderStringFactory::ms_CameraWorldPos,SKShaderStringFactory::ms_WorldPos,OutString);
	OutString += _T(";\n");

	SKString NodeStringR = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_R);
	SKString SaturateString;
	SKRenderer::ms_pRenderer->Saturate(_T("WorldCameraLength /") + SKShaderStringFactory::ms_PointLightRange,SaturateString);
	OutString += NodeStringR + _T(" = ") + SaturateString;
	OutString += _T(";\n");
// 	SKString NodeStringG = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_G);
// 	OutString += NodeStringG + _T(" = ") + GetAlphaNode()->GetNodeName().GetString();
// 	OutString += _T(";\n");

	

	OutString +=  SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");
	//OutString += "Out.Color.rgb = ps_Input.ViewZ/FarZ;\n";
	
	return true;
}
bool SKShaderMainFunction::GetNormalDepthString(SKString &OutString)const
{
	GetAlphaTestString(OutString);
	SKString NodeStringRG = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_R | SKRenderer::VE_G);
	OutString +=  NodeStringRG + _T(" = ");
	SKRenderer::ms_pRenderer->DecodeNormal3(SKShaderStringFactory::ms_ViewNormal,OutString);
	OutString += _T(";\n");

	SKString NodeStringB = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_B);
	SKString StringViewZ = SKRenderer::ms_pRenderer->GetValueElement(SKShaderStringFactory::ms_ViewPos, SKRenderer::VE_B);
	SKString SaturateString;
	SKRenderer::ms_pRenderer->Saturate(StringViewZ + _T("/") + SKShaderStringFactory::ms_FarZ, SaturateString);
	OutString += NodeStringB + _T(" = ") + SaturateString;
	OutString += _T(";\n");

	SKString NodeStringA = SKRenderer::ms_pRenderer->GetValueElement(m_pOutput[OUT_COLOR],SKRenderer::VE_A);
	OutString +=  NodeStringA + _T(" = ");
	SKRenderer::ms_pRenderer->EncodeReflect(GetReflectMipNode()->GetNodeName().GetString(),GetReflectPowNode()->GetNodeName().GetString(),OutString);
	OutString += _T(";\n");

	OutString +=  SKShaderStringFactory::ms_PSOutputColorValue + _T(" = ") + m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(";\n");

	return true;
}
bool SKShaderMainFunction::GetInputValueString(SKString &OutString,unsigned int uiOutPutStringType)const
{
	if(!SKRenderer::ms_pRenderer)
		return 0;
	SKString Temp;
	if (uiOutPutStringType == OST_MATERIAL)
	{
		for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
		{

			if (m_pInput[i] == GetNormalNode())
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
	else if(uiOutPutStringType == OST_NORMAL_DEPTH)
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
void SKShaderMainFunction::GetLightShadow(const MaterialShaderPara& MSPara,SKArray<SKString> ShadowStringArray[SKLight::LT_MAX])const
{
	SKArray<SKLight *> pLightArray = MSPara.LightArray;
	unsigned int uiShadowNum = 0;
	unsigned int uiLightIndex[SKLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i< pLightArray.GetNum() ; i++)
	{
		SKString ShadowString;
		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(pLightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			if (pLocalLight->GetLightType() == SKLight::LT_POINT)
			{
				if( ((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_CUB )
				{
					ShadowString = _T(" * ");
					if (MSPara.pCamera->GetFov() <= AngleToRadian(90.0f))
					{
						SKRenderer::ms_pRenderer->PointLightCubeShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[SKLight::LT_POINT]) + _T("]"),
							SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKShaderStringFactory::ms_ViewMatrix,SKShaderStringFactory::ms_WorldPos,ShadowString);
					}
					else
					{
						SKRenderer::ms_pRenderer->PointLightCubeShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[SKLight::LT_POINT]) + _T("]"),
							SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKShaderStringFactory::ms_WorldPos,ShadowString);
					}
					
					uiShadowNum++;
				}
				else if (((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_VOLUME)
				{
					ShadowString = _T(" * ");
					SKRenderer::ms_pRenderer->PointLightVolumeShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[SKLight::LT_POINT]) + _T("]"),
						SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}
				else if (((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_DUAL_PARABOLOID)
				{
					ShadowString = _T(" * ");
					unsigned int uiIndex = uiShadowNum;
					SKRenderer::ms_pRenderer->PointLightDualParaboloidShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + IntToString(uiLightIndex[SKLight::LT_POINT]) + _T("]"),
						SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiIndex),SKShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
					uiShadowNum++;
				}
			}
			else if (pLocalLight->GetLightType() == SKLight::LT_DIRECTION)
			{	
				if (((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_VOLUME)
				{
					ShadowString = _T(" * ");
					SKRenderer::ms_pRenderer->DirectionLightVolumeShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + IntToString(uiLightIndex[SKLight::LT_DIRECTION]) + _T("]"),
						SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}
				else if (((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_CSM)
				{
					ShadowString = _T(" * ");
					unsigned int uiIndex = uiShadowNum;
					SKRenderer::ms_pRenderer->DirectionLightCSMShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + IntToString(uiLightIndex[SKLight::LT_DIRECTION]) + _T("]"),
						SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiIndex),
						SKShaderStringFactory::ms_WorldPos,SKShaderStringFactory::ms_ViewPos,SKShaderStringFactory::ms_FarZ,ShadowString);
					uiShadowNum++;
					uiShadowNum++;
					uiShadowNum++;
				}
				else if (((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_PROJECT)
				{
				}
				else
				{
					ShadowString = _T(" * ");
					SKRenderer::ms_pRenderer->DirectionLightShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + IntToString(uiLightIndex[SKLight::LT_DIRECTION]) + _T("]"),
						SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}

			}
			else if (pLocalLight->GetLightType() == SKLight::LT_SPOT)
			{	
				if (((SKSpotLight*)pLocalLight)->GetShadowType() == SKSpotLight::ST_NORMAL)
				{
					// use the same to dirlightshadow
					ShadowString = _T(" * ");
					SKRenderer::ms_pRenderer->SpotLightShadow(SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + IntToString(uiLightIndex[SKLight::LT_SPOT]) + _T("]"),
						SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKShaderStringFactory::ms_WorldPos,ShadowString);
					uiShadowNum++;
				}
			}
			
		}

		ShadowStringArray[pLightArray[i]->GetLightType()].AddElement(ShadowString);

		uiLightIndex[pLightArray[i]->GetLightType()]++;
	}
}
bool SKShaderMainFunction::IsValidNodeToThis(SKShaderFunction * pShaderFunction)
{
	if(pShaderFunction->GetType().IsSameType(SKWorldNormal::ms_Type) || pShaderFunction->GetType().IsSameType(SKViewNormal::ms_Type))
	{
		if(GetNormalNode()->GetOutputLink())
		{
			SKShaderFunction * pOwner = (SKShaderFunction *)GetNormalNode()->GetOutputLink()->GetOwner();
			if (pOwner == pShaderFunction)
			{
				return false;
			}
			if (pOwner->HaveThisChild(pShaderFunction) == true)
			{
				return false;
			}
		}
	}

	if (pShaderFunction->GetType().IsSameType(SKLightDir::ms_Type) || pShaderFunction->GetType().IsSameType(SKLightColor::ms_Type) || pShaderFunction->GetType().IsSameType(SKLightSpecular::ms_Type))
	{
		return false;
	}
	if (pShaderFunction->GetType().IsSameType(SKColorBuffer::ms_Type))
	{
		return false;
	}
	return true;
}
void SKShaderMainFunction::GetValueUseString(SKString &OutString, unsigned int uiValueUseString)
{
	if ((uiValueUseString & VUS_WORLD_NORMAL) == VUS_WORLD_NORMAL)
	{
		if (SKShaderStringFactory::ms_PSInputLocalNormal.GetLength() == 0)
		{
			//OutString += SKShaderStringFactory::ms_WorldNormal + _T(" = ") + SKRenderer::ms_pRenderer->Float3Const(_T("0"), _T("1"), _T("0")) + _T(";\n");
		}
		else if (!GetNormalNode()->GetOutputLink() || SKShaderStringFactory::ms_PSInputLocalTangent.GetLength() == 0)
		{

			OutString += SKShaderStringFactory::ms_WorldNormal + _T(" = ");
			SKRenderer::ms_pRenderer->LocalToWorldNormal(SKShaderStringFactory::ms_PSInputLocalNormal, OutString);
			OutString += _T(";\n");

		}
		else
		{

			GetNormalInputValueString(OutString);
			OutString += SKRenderer::ms_pRenderer->Float3() + _T("LocalNormal = ");
			SKRenderer::ms_pRenderer->BumpNormal(GetNormalNode()->GetNodeName().GetString(), OutString);
			OutString += _T(";\n");

			OutString += SKShaderStringFactory::ms_WorldNormal + _T(" = ");
			SKRenderer::ms_pRenderer->LocalToWorldNormal(_T("LocalNormal"), OutString);
			OutString += _T(";\n");
		}
	}
	if ((uiValueUseString & VUS_VIEW_NORMAL) == VUS_VIEW_NORMAL)
	{
		OutString += SKShaderStringFactory::ms_ViewNormal + _T(" = ");
		SKRenderer::ms_pRenderer->WorldToViewNormal(SKShaderStringFactory::ms_WorldNormal, OutString);
		OutString += _T(";\n");
	}


	if ((uiValueUseString & VUS_WORLD_POS) == VUS_WORLD_POS)
	{
		OutString += SKShaderStringFactory::ms_WorldPos + _T(" = ");
		SKRenderer::ms_pRenderer->LocalToWorldPos(SKShaderStringFactory::ms_PSInputLocalPos, OutString);
		OutString += _T(";\n");
	}

	if ((uiValueUseString & VUS_VIEW_POS) == VUS_VIEW_POS)
	{
		OutString += SKShaderStringFactory::ms_ViewPos + _T(" = ");
		SKRenderer::ms_pRenderer->WorldToViewPos(SKShaderStringFactory::ms_WorldPos, OutString);
		OutString += _T(";\n");
	}

	if ((uiValueUseString & VUS_PROJ_POS) == VUS_PROJ_POS)
	{
		OutString += SKShaderStringFactory::ms_ProjectPos + _T(" = ");
		SKRenderer::ms_pRenderer->TransProjPos(SKShaderStringFactory::ms_ViewPos, SKShaderStringFactory::ms_ProjectMatrix, OutString);
		OutString += _T(";\n");
	}


	if ((uiValueUseString & VUS_VIEW_WORLD_DIR) == VUS_VIEW_WORLD_DIR)
	{
		SKString ViewWorldDir;
		SKRenderer::ms_pRenderer->GetWorldViewDir(ViewWorldDir);
		OutString += SKShaderStringFactory::ms_ViewWorldDir + _T(" = ") + ViewWorldDir;
		OutString += _T(";\n");
	}

}
void SKShaderMainFunction::GetNormalString(SKString &OutString)const
{
	if (GetNormalNode()->GetOutputLink())
	{
		((SKShaderFunction *)GetNormalNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
	}
}
void SKShaderMainFunction::GetAlphaString(SKString &OutString)const
{
	if (GetAlphaNode()->GetOutputLink())
	{
		((SKShaderFunction *)GetAlphaNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
	}
}
void SKShaderMainFunction::GetEmissiveString(SKString &OutString)const
{
	if (GetEmissiveNode()->GetOutputLink())
	{
		((SKShaderFunction *)GetEmissiveNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
	}
}
void SKShaderMainFunction::GetDiffuseString(SKString &OutString)const
{
	if (GetDiffuseNode()->GetOutputLink())
	{
		((SKShaderFunction *)GetDiffuseNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
	}
}
void SKShaderMainFunction::GetReflectMipString(SKString &OutString)const
{
	if (GetReflectMipNode()->GetOutputLink())
	{
		((SKShaderFunction *)GetReflectMipNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
	}
}
void SKShaderMainFunction::GetReflectPowString(SKString &OutString)const
{
	if (GetReflectPowNode()->GetOutputLink())
	{
		((SKShaderFunction *)GetReflectPowNode()->GetOutputLink()->GetOwner())->GetShaderTreeString(OutString);
	}
}
bool SKShaderMainFunction::GetNormalInputValueString(SKString &OutString)const
{
	SKString Temp;
	unsigned int uiNormalValueType = GetNormalNode()->GetValueType();
	if (uiNormalValueType == SKPutNode::VT_4)
	{
		OutString += SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		SKMAC_ASSERT(0);
		return false;
	}
	if (!GetNormalNode()->GetOutputLink())
	{
		OutString += GetNormalNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetNormalNode()->GetOutputLink(), GetNormalNode());
	}
	return true;
}
bool SKShaderMainFunction::GetAlphaInputValueString(SKString &OutString)const
{
	SKString Temp;
	unsigned int uiAplhaValueType = GetAlphaNode()->GetValueType();
	if (uiAplhaValueType == SKPutNode::VT_1)
	{
		OutString += SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		Temp = SKRenderer::ms_pRenderer->FloatConst(_T("1"));
	}
	else
	{
		SKMAC_ASSERT(0);
		return 0;
	}
	if (!GetAlphaNode()->GetOutputLink())
	{
		OutString += GetAlphaNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetAlphaNode()->GetOutputLink(), GetAlphaNode());
	}
	return true;
}
bool SKShaderMainFunction::GetEmissiveInputValueString(SKString &OutString)const
{
	SKString Temp;
	unsigned int uiEmissiveType = GetEmissiveNode()->GetValueType();
	if (uiEmissiveType == SKPutNode::VT_4)
	{
		OutString += SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		SKMAC_ASSERT(0);
		return 0;
	}
	if (!GetEmissiveNode()->GetOutputLink())
	{
		OutString += GetEmissiveNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetEmissiveNode()->GetOutputLink(), GetEmissiveNode());
	}
	return true;
}
bool SKShaderMainFunction::GetDiffuseInputValueString(SKString &OutString)const
{
	SKString Temp;
	unsigned int uiDiffuseType = GetDiffuseNode()->GetValueType();
	if (uiDiffuseType == SKPutNode::VT_4)
	{
		OutString += SKRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = SKRenderer::ms_pRenderer->Float4Const(_T("0"), _T("0"), _T("0"), _T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
	{
		SKMAC_ASSERT(0);
		return 0;
	}

	if (!GetDiffuseNode()->GetOutputLink())
	{
		OutString += GetDiffuseNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetDiffuseNode()->GetOutputLink(), GetDiffuseNode());
	}
	return true;
}
bool SKShaderMainFunction::GetReflectMipInputValueString(SKString &OutString)const
{
	SKString Temp;
	unsigned int uiReflectMipValueType = GetReflectMipNode()->GetValueType();
	if (uiReflectMipValueType == SKPutNode::VT_1)
	{
		OutString += SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else
	{
		SKMAC_ASSERT(0);
		return 0;
	}
	if (!GetReflectMipNode()->GetOutputLink())
	{
		OutString += GetReflectMipNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetReflectMipNode()->GetOutputLink(), GetReflectMipNode());
	}
	return true;
}
bool SKShaderMainFunction::GetReflectPowInputValueString(SKString &OutString)const
{
	SKString Temp;
	unsigned int uiReflectPowValueType = GetReflectPowNode()->GetValueType();
	if (uiReflectPowValueType == SKPutNode::VT_1)
	{
		OutString += SKRenderer::ms_pRenderer->Float() + _T(" ");/*_T("SKREAL ");*/
		Temp = SKRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else
	{
		SKMAC_ASSERT(0);
		return 0;
	}
	if (!GetReflectPowNode()->GetOutputLink())
	{
		OutString += GetReflectPowNode()->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
	}
	else
	{
		OutString += GetValueEqualString(GetReflectPowNode()->GetOutputLink(), GetReflectPowNode());
	}
	return true;
}
