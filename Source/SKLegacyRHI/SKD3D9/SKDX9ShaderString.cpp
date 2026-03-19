#include "SKDx9Renderer.h"
#include "SK2DTexSampler.h"
#include "SK1DTexSampler.h"
#include "SKGeometry.h"
#include "SKString.h"
#include "SKResourceManager.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKOrenNayarShaderFunction.h"
#include "SKStraussShaderFunction.h"
#include "SKCookTorranceShaderFunction.h"
#include "SKIsotropicWardShaderFunction.h"
#include "SKPointLight.h"
#include "SKDirectionLight.h"
#include "SKArithmeticShaderFunction.h"
#include "SKSpotLight.h"
#include "SKColorBuffer.h"
using namespace SKEngine2;
SKString SKDX9Renderer::GetDeclareValue(const SKString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const
{
	SKString OutString;
	if (uiValueType == SKPutNode::VT_1)
	{
		OutString += _T("float ");
	}
	else if (uiValueType == SKPutNode::VT_2)
	{
		OutString += _T("float2 ");
	}
	else if (uiValueType == SKPutNode::VT_3)
	{
		OutString += _T("float3 ");
	}
	else if (uiValueType == SKPutNode::VT_4)
	{
		OutString += _T("float4 ");
	}

	OutString += ShowName + SetRegister(RT_C, uiRegisterIndex) + _T(";\n");
	return OutString;
}
SKString SKDX9Renderer::GetDeclareSampler(const SKString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const
{
	SKString OutString = Sampler(SamplerType) + _T(" ");
	OutString += ShowName
		+ SetRegister(RT_S, uiRegisterIndex) + _T(";\n");
	return OutString;
}
SKString SKDX9Renderer::TexColorBuffer(const SKColorBuffer * pColorBuffer) const
{
	if (!pColorBuffer)
		return SKString();
	return pColorBuffer->GetOutputNode(SKColorBuffer::OUT_COLOR)->GetNodeName().GetString()
		+ _T(" = tex2D(") + SKShaderStringFactory::ms_PSColorBufferSampler + _T(",") +
		pColorBuffer->GetInputNode(SKColorBuffer::IN_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
}
SKString SKDX9Renderer::Tex2D(const VS2DTexSampler * p2DTexSampler) const
{
	SKMAC_ASSERT(p2DTexSampler);
	if (!p2DTexSampler)
		return SKString();
	return p2DTexSampler->GetOutputNode(VS2DTexSampler::OUT_COLOR)->GetNodeName().GetString()
		+ _T(" = tex2D(") + p2DTexSampler->GetShowName().GetString() + _T(",") + 
			p2DTexSampler->GetInputNode(VS2DTexSampler::IN_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
}
SKString SKDX9Renderer::Tex1D(const VS1DTexSampler * p1DTexSampler) const
{
	SKMAC_ASSERT(p1DTexSampler);
	if (!p1DTexSampler)
		return SKString();
	return p1DTexSampler->GetOutputNode(VS1DTexSampler::OUT_COLOR)->GetNodeName().GetString()
		+ _T(" = tex2D(") + p1DTexSampler->GetShowName().GetString() + _T(",") + 
		p1DTexSampler->GetInputNode(VS1DTexSampler::IN_TEXCOORD)->GetNodeName().GetString() + _T(");\n");
}
SKString SKDX9Renderer::Mul(const SKMul * pMul) const
{
	return pMul->GetOutputNode(SKMul::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pMul->GetInputNode(SKMul::IN_A)->GetNodeName().GetString() + _T(" * ") + 
		pMul->GetInputNode(SKMul::IN_B)->GetNodeName().GetString() + _T(";\n");
}
SKString SKDX9Renderer::Saturate(const SKSaturate * pSaturate) const
{
	return pSaturate->GetOutputNode(SKDiv::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = saturate(") + pSaturate->GetInputNode(SKSaturate::IN_VALUE)->GetNodeName().GetString() + _T(");\n");
}
SKString SKDX9Renderer::Div(const SKDiv * pDiv) const
{
	return pDiv->GetOutputNode(SKDiv::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pDiv->GetInputNode(SKDiv::IN_A)->GetNodeName().GetString() + _T(" / ") +
		pDiv->GetInputNode(SKDiv::IN_B)->GetNodeName().GetString() + _T(";\n");
}
SKString SKDX9Renderer::Sub(const SKSub * pSub) const
{
	return pSub->GetOutputNode(SKSub::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pSub->GetInputNode(SKSub::IN_A)->GetNodeName().GetString() + _T(" - ") + 
		pSub->GetInputNode(SKSub::IN_B)->GetNodeName().GetString() + _T(";\n");
}
SKString SKDX9Renderer::Add(const SKAdd * pAdd) const
{
	return pAdd->GetOutputNode(SKSub::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = ") + pAdd->GetInputNode(SKSub::IN_A)->GetNodeName().GetString() + _T(" + ") +
		pAdd->GetInputNode(SKSub::IN_B)->GetNodeName().GetString() + _T(";\n");
}
SKString SKDX9Renderer::Dot(const SKDot * pDot) const
{
	return pDot->GetOutputNode(SKDot::OUT_VALUE)->GetNodeName().GetString()
		+ _T(" = dot(") + pDot->GetInputNode(SKDot::IN_A)->GetNodeName().GetString() + _T(",") + 
		pDot->GetInputNode(SKDot::IN_B)->GetNodeName().GetString() + _T(");\n");
}
SKString SKDX9Renderer::MakeValue(const SKMakeValue * pMakeValue)const
{
	SKString Temp;
	if (pMakeValue->GetOutputNode(SKMakeValue::OUT_VALUE)->GetValueType() == SKPutNode::VT_1)
	{
		Temp = _T("float(");
	}
	else if (pMakeValue->GetOutputNode(SKMakeValue::OUT_VALUE)->GetValueType() == SKPutNode::VT_2)
	{
		Temp = _T("float2(");		
	}
	else if (pMakeValue->GetOutputNode(SKMakeValue::OUT_VALUE)->GetValueType() == SKPutNode::VT_3)
	{
		Temp = _T("float3(");
	}
	else if (pMakeValue->GetOutputNode(SKMakeValue::OUT_VALUE)->GetValueType() == SKPutNode::VT_4)
	{
		Temp = _T("float4(");
	}
	for (unsigned int i = 0; i < pMakeValue->GetInputNodeNum(); i++)
	{
		Temp += pMakeValue->GetInputNode(i)->GetNodeName().GetString();
		if (i == pMakeValue->GetInputNodeNum() - 1)
		{
			Temp += _T(")");
		}
		else
		{
			Temp += _T(",");
		}
	}


	return pMakeValue->GetOutputNode(SKDot::OUT_VALUE)->GetNodeName().GetString() + _T(" = ") + Temp +  _T(";\n");
}
SKString SKDX9Renderer::Sampler(unsigned int uiType)const
{
	if (uiType == SKTexture::TT_1D)
	{
		return SKString(_T("sampler1D "));
	}
	else if (uiType == SKTexture::TT_2D)
	{
		return SKString(_T("sampler2D "));
	}
	else if (uiType == SKTexture::TT_3D)
	{
		return SKString(_T("sampler3D "));
	}
	else if (uiType == SKTexture::TT_CUBE)
	{
		return SKString(_T("samplerCUBE "));
	}
	return SKString::ms_StringNULL;
}
SKString SKDX9Renderer::Float()const
{
	return SKString(_T("float "));
}
SKString SKDX9Renderer::Float2()const
{
	return SKString(_T("float2 "));
}
SKString SKDX9Renderer::Float3()const
{
	return SKString(_T("float3 "));
}
SKString SKDX9Renderer::Float4()const
{
	return SKString(_T("float4 "));
}
SKString SKDX9Renderer::Return()const
{
	return SKString(_T("return "));
}
SKString SKDX9Renderer::FloatConst(const SKString & Value1)const
{
	return Value1;
}
SKString SKDX9Renderer::Float2Const(const SKString &Value1,const SKString &Value2)const
{
	return SKString(_T("float2")) + _T("(") + Value1 + _T(",") + Value2 + _T(")");
}
SKString SKDX9Renderer::Float3Const(const SKString & Value1,
							 const SKString& Value2,const SKString &Value3)const
{
	return SKString(_T("float3")) + _T("(") + Value1 + _T(",") + Value2 + _T(",") + Value3 + _T(")");
}
SKString SKDX9Renderer::Float4Const(const SKString &Value1,const SKString &Value2,
							 const SKString &Value3,const SKString &Value4)const
{
	return SKString(_T("float4")) + _T("(") + Value1 + _T(",") + Value2 + _T(",") + Value3 + _T(",") + Value4 + _T(")");
}
void SKDX9Renderer::CustomPointLightUseString(SKString & PointLightUseString ,SKString & PointAttenuationDivString)
{
	PointLightUseString = _T("float3 ") + SKShaderStringFactory::ms_LightDir + _T(" = WorldPos - LightWorldPos;\n")
		+ _T("float  fDistance = length(") + SKShaderStringFactory::ms_LightDir + _T(");\n")
		+ SKShaderStringFactory::ms_LightDir + _T(" = ") + SKShaderStringFactory::ms_LightDir + _T(" / fDistance;\n") 
		+ _T("float fLightAttenuationDiv = saturate(1.0f - fDistance / Range);\n");
	PointAttenuationDivString = _T("* fLightAttenuationDiv");
}
void SKDX9Renderer::CustomSpotLightUseString(SKString & SpotLightUseString ,SKString & SpotAttenuationDivString)
{
	SpotLightUseString = _T("float3 ") + SKShaderStringFactory::ms_LightDir + _T(" = WorldPos - LightWorldPos;\n")
		+ _T("float  fDistance = length(") + SKShaderStringFactory::ms_LightDir + _T(");\n")
		+ SKShaderStringFactory::ms_LightDir + _T(" = ") + SKShaderStringFactory::ms_LightDir + _T(" / fDistance;\n")  
		+ _T("float fLightAttenuationDiv = saturate(1.0f - fDistance / Range);\n")
		+ _T("float fSpotLightCos = dot(") + SKShaderStringFactory::ms_LightDir + _T(",LightWorldDirection);\n")
		+ _T("float fLightIf = saturate((fSpotLightCos - cos(Phi/ 2)) / (cos(Theta/ 2) - cos(Phi / 2)));\n")
		+ _T("float fLightEffect = pow(fLightIf,Falloff);\n");

	SpotAttenuationDivString = _T(" * fLightAttenuationDiv  * fLightEffect");
}
SKString SKDX9Renderer::GetValueElement(const SKString & InputString,
	unsigned char uiVE)const
{
	SKString Temp = InputString;
	if (uiVE > 0)
	{
		SKString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		unsigned int Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp += _T(".");

		for (unsigned int i = 0; i < 4; i++)
		{
			if (uiVE & Mask[i])
			{
				Temp += Value[i];
			}
		}

	}


	return Temp;
}
SKString SKDX9Renderer::GetValueElement(const SKPutNode * pPutNode,
								 unsigned char uiVE)const
{
	SKMAC_ASSERT(pPutNode);
	if (!pPutNode)
		return SKString::ms_StringNULL;
	SKString Temp = pPutNode->GetNodeName().GetString();
	if (uiVE > 0)
	{
		if (pPutNode->GetValueType() == SKPutNode::VT_1 && (uiVE & VE_R))
		{
			return Temp;
		}
		else if(pPutNode->GetValueType() == SKPutNode::VT_1)
		{
			SKMAC_ASSERT(0);
			return SKString::ms_StringNULL;
		}
		SKString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		unsigned int Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp +=  _T(".");
		
		for (unsigned int i = 0 ; i < 4 ; i++)
		{
			if ( i <= pPutNode->GetValueType())
			{
				if (uiVE & Mask[i])
				{
					Temp += Value[i];
				}
			}
			
		}

	}

	
	return Temp;
}
SKString SKDX9Renderer::SetRegister(unsigned int uiRegisterType,unsigned int uiRegisterIndex)const
{

	SKString RegisterID = IntToString(uiRegisterIndex);
	if(uiRegisterType == RT_B)
		return _T(" : register(b") + RegisterID + _T(")");
	else if(uiRegisterType == RT_C)
		return _T(" : register(c") + RegisterID + _T(")");
	if(uiRegisterType == RT_S)
		return _T(" : register(s") + RegisterID + _T(")");
	else
		SKMAC_ASSERT(1);
	return SKString();
}
void SKDX9Renderer::CreateVInputDeclarePosition(SKVertexBuffer * pVBuffer,
								 SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetPositionLevel(); i++)
	{	
		if(pVBuffer->HavePositionInfo(i))
		{
			SKString VertexID = IntToString(i);		
			OutString += _T("float3 Position") + VertexID + _T(":POSITION") + VertexID + _T(";\n");
		}

	}
}
void SKDX9Renderer::CreateVInputDeclareTexCoord(SKVertexBuffer * pVBuffer,
								 SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(i))
		{
			SKString TextureID = IntToString(i);
			OutString += _T("float2 TexCoord") + TextureID+ _T(":TEXCOORD") + TextureID + _T(";\n");
		}
	}
}
void SKDX9Renderer::CreateVInputDeclareNormal(SKVertexBuffer * pVBuffer,
							   SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetNormalLevel(); i++)
	{
		if(pVBuffer->HaveNormalInfo(i))
		{
			SKString NomalID = IntToString(i);
			OutString += _T("float3 Normal") + NomalID + _T(":NORMAL") + NomalID + _T(";\n");
		}
	}

	if(pVBuffer->HaveTangentInfo())
	{
		OutString += _T("float4 Tangent : TANGENT;\n");
		if (pVBuffer->HaveBinormalInfo())
		{
			OutString += _T("float3 Binormal : BINORMAL;\n");
		}
	}
}
void SKDX9Renderer::CreateVInputDeclareColor(SKVertexBuffer * pVBuffer,
							  SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(0))
		{
			SKString ColorID = IntToString(i);
			OutString += _T("float4 Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
		}
	}
}
void SKDX9Renderer::CreateVInputDeclareSkin(SKVertexBuffer * pVBuffer,
							 SKString & OutString)
{
	if(pVBuffer->HaveBlendWeightInfo())
	{
		OutString += _T("float4 BlendWeight :BLENDWEIGHT;\n");
	}

	if(pVBuffer->HaveBlendIndicesInfo())
	{
		OutString += _T("uint4 BlendIndices :BLENDINDICES;\n");
	}
}
void SKDX9Renderer::CreateVInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString)
{

	SKString TempDeclare;
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	CreateVInputDeclarePosition(pVBuffer,TempDeclare);
	CreateVInputDeclareTexCoord(pVBuffer,TempDeclare);
	CreateVInputDeclareNormal(pVBuffer,TempDeclare);
	CreateVInputDeclareColor(pVBuffer,TempDeclare);
	CreateVInputDeclareSkin(pVBuffer,TempDeclare);
	
	OutString += _T("struct SK_INPUT \n{ \n") + TempDeclare + _T("};\n");
	
}
void SKDX9Renderer::CreateVOutputDeclarePosition(
								  SKString & OutString)
{
	OutString += _T("float4 Position:POSITION;\n");
}
void SKDX9Renderer::CreateVOutputDeclareTexCoord(SKVertexBuffer * pVBuffer,unsigned int& ID,
								  SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{

		if(pVBuffer->HaveTexCoordInfo(i))
		{
			SKString TextureID = IntToString(ID);
			OutString += _T("float2 TexCoord") + TextureID+ _T(":TEXCOORD") + TextureID + _T(";\n");
			ID++;
		}
	}
}
void SKDX9Renderer::CreateVOutputDeclareNormal(SKVertexBuffer * pVBuffer,unsigned int& ID,
								SKString & OutString)
{
	if(pVBuffer->HaveNormalInfo(0))
	{		
		SKString TextureID = IntToString(ID);

		OutString += _T("float3 Normal:TEXCOORD") + TextureID + _T(";\n");
		ID++;
	}




	if(pVBuffer->HaveTangentInfo())
	{
		{
			SKString TextureID = IntToString(ID);

			OutString += _T("float3 Tangent:TEXCOORD") + TextureID + _T(";\n");
			ID++;
		}
		{
			SKString TextureID = IntToString(ID);

			OutString += _T("float3 Binormal:TEXCOORD") + TextureID + _T(";\n");
			ID++;
		}
	}
}
void SKDX9Renderer::CreateVOutputDeclareColor(SKVertexBuffer * pVBuffer,
							   SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{
		if(pVBuffer->HaveColorInfo(i))
		{
			SKString ColorID = IntToString(i);
			OutString += _T("float4 Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
		}
	}
}
void SKDX9Renderer::CreateVOutputDeclareLocalPos(unsigned int& ID,
								  SKString & OutString)
{
	SKString TextureID  = IntToString(ID);
	OutString += _T("float3 Pos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
}
void SKDX9Renderer::CreateVOutputDeclareWorldPos(unsigned int& ID,
												 SKString & OutString)
{
	SKString TextureID  = IntToString(ID);
	OutString += _T("float3 WorldPos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
}
void SKDX9Renderer::CreateVOutputDeclareProjectPos(unsigned int& ID,
												 SKString & OutString)
{
	SKString TextureID  = IntToString(ID);
	OutString += _T("float4 ProjectPos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
}
void SKDX9Renderer::CreateVOutputDeclareProjectZ(unsigned int& ID,
								  SKString & OutString)
{
	SKString TextureID = IntToString(ID);
	OutString += _T("float ProjectZ:TEXCOORD") + TextureID + _T(";\n");
	ID++;
}
void SKDX9Renderer::CreateVOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										 SKString & OutString)
{
	SKString TempDeclare;
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	unsigned int ID = 0;
	CreateVOutputDeclarePosition(TempDeclare);
	CreateVOutputDeclareTexCoord(pVBuffer,ID,TempDeclare);
	CreateVOutputDeclareNormal(pVBuffer,ID,TempDeclare);
	CreateVOutputDeclareColor(pVBuffer,TempDeclare);
	if (uiPassType == SKPass::PT_MATERIAL 
		|| uiPassType == SKPass::PT_INDIRECT
		|| uiPassType == SKPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == SKPass::PT_SHADOW
		|| uiPassType == SKPass::PT_NORMALDEPTH)
	{		
		CreateVOutputDeclareLocalPos(ID,TempDeclare);
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateVOutputDeclareLocalPos(ID, TempDeclare);
		CreateVOutputDeclareProjectZ(ID,TempDeclare);
	}
	OutString += _T("struct SK_OUTPUT \n{\n") + TempDeclare + _T("};\n");
}
void SKDX9Renderer::CreateUserConstantWorldViewProjectMatrix(SKShader * pShader,unsigned int& ID,
											  SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + SKShaderStringFactory::ms_WorldViewProjectMatrix + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_WorldViewProjectMatrix,NULL,sizeof(SKREAL) * 16,ID,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantLightShadowMatrix(SKShader * pShader,unsigned int& ID,
															 SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + SKShaderStringFactory::ms_LightShadowMatrix + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_LightShadowMatrix, NULL, sizeof(SKREAL)* 16, ID, 4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}

void SKDX9Renderer::CreateUserConstantLightWorldPos(SKShader * pShader,unsigned int& ID,
									 SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("float3 ") + SKShaderStringFactory::ms_LightWorldPos + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_LightWorldPos,NULL,sizeof(SKREAL) * 3,ID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantLightDirection(SKShader * pShader,unsigned int& ID,
									  SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("float3 ") + SKShaderStringFactory::ms_LightWorldDirection + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_LightWorldDirection,NULL,sizeof(SKREAL) * 3,ID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantViewMatrix(SKShader * pShader,unsigned int& ID,
												 SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + SKShaderStringFactory::ms_ViewMatrix + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_ViewMatrix,NULL,sizeof(SKREAL) * 16,ID,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantProjectMatrix(SKShader * pShader, unsigned int& ID,
	SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + SKShaderStringFactory::ms_ProjectMatrix + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_ProjectMatrix, NULL, sizeof(SKREAL)* 16, ID, 4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantViewProjectMatrix(SKShader * pShader,unsigned int& ID,
															 SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + SKShaderStringFactory::ms_ViewProjectMatrix + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_ViewProjectMatrix,NULL,sizeof(SKREAL) * 16,ID,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantWorldViewMatrix(SKShader * pShader,unsigned int& ID,
									   SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + SKShaderStringFactory::ms_WorldViewMatrix + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_WorldViewMatrix,NULL,sizeof(SKREAL) * 16,ID,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}

void SKDX9Renderer::CreateUserConstantSkin(SKGeometry * pGeometry,SKShader * pShader,unsigned int& ID,
							SKString & OutString)
{
	SKVertexBuffer * pVBuffer = pGeometry->GetMeshDate()->GetVertexBuffer();
	if(pGeometry->GetAffectBoneNum() && pVBuffer->GetBlendWeightDate() && pVBuffer->GetBlendIndicesDate() )
	{

		SKString RegisterID = IntToString(ID);
		unsigned int uiBoneNum = SKResourceManager::GetGpuSkinBoneNum();
		SKString BoneNum = IntToString(uiBoneNum * 3);
		OutString += _T("float4 ") + SKShaderStringFactory::ms_BoneMatrix + _T("[") + BoneNum + _T("]") + _T(" : register(c") + RegisterID + _T(");\n");

		SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_BoneMatrix,NULL,sizeof(SKREAL) * 4 * uiBoneNum * 3,ID,uiBoneNum * 3);
		pShader->m_pUserConstant.AddElement(pUserConstant);
		ID += pUserConstant->GetRegisterNum();
	}
}
void SKDX9Renderer::CreateVUserConstant(SKVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString)
{
	unsigned int uiRegisterID = 0;
	if (uiPassType == SKPass::PT_MATERIAL 
		|| uiPassType == SKPass::PT_PREZ 
		|| uiPassType == SKPass::PT_NORMALDEPTH 
		|| uiPassType == SKPass::PT_INDIRECT
		|| uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{
		CreateUserConstantWorldViewProjectMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantViewProjectMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantLightWorldPos(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreateUserConstantWorldMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantViewProjectMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantLightDirection(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		CreateUserConstantLightShadowMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateUserConstantLightShadowMatrix(pVShader,uiRegisterID,OutString);
		CreateUserConstantPointLightRange(pVShader,uiRegisterID,OutString);
		CreateUserConstantSkin(MSPara.pGeometry,pVShader,uiRegisterID,OutString);
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION)
	{
		CreateUserConstantInvRTWidth(pVShader,uiRegisterID,OutString);
	}
	if (uiPassType == SKPass::PT_NORMALDEPTH)
	{
		CreateUserConstantWorldViewMatrix(pVShader,uiRegisterID,OutString);
	}
	pVShader->m_bCreatePara = true;

}
void SKDX9Renderer::CreateVFuntionDualParaboloidPositionAndNormal(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	CreateVFuntionPositionAndNormal(MSPara,FuntionBody);
	FuntionBody += _T("float fLength = length(Out.Position.xyz);\n");
	FuntionBody += _T("Out.Position.xyz /= fLength;\n");
 	FuntionBody += _T("Out.Position.x /= Out.Position.z + 1.01f;\n");
 	FuntionBody += _T("Out.Position.y /= Out.Position.z + 1.01f;\n");
	FuntionBody += _T("Out.Position.z = saturate(fLength / ") + SKShaderStringFactory::ms_PointLightRange + _T(");\n");
	

}
void SKDX9Renderer::CreateVFuntionPositionAndNormal(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	FuntionBody +=_T("float3 Position;\n");


	if(pVBuffer->HaveNormalInfo(0))
	{
		FuntionBody +=_T("float3 Normal;\n");
	}

	if(pVBuffer->HaveTangentInfo())
	{
		FuntionBody +=_T("float4 Tangent;\n");
		FuntionBody +=_T("float3 Binormal;\n");
	}



	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo())
	{
		FuntionBody +=_T("float4 U = 0;\n");
		FuntionBody +=_T("float4 V = 0;\n");
		FuntionBody +=_T("float4 N = 0;\n");

		SKRenderer::ms_pRenderer->ComputeBoneVector("Input.BlendIndices","Input.BlendWeight",SKShaderStringFactory::ms_BoneMatrix
			,"U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");

		FuntionBody += "Position = ";
		SKRenderer::ms_pRenderer->BoneTranPos("float4(Input.Position0,1)","U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");



		if(pVBuffer->HaveNormalInfo(0))
		{
			FuntionBody += "Normal = Input.Normal0;\n";
			if (pVBuffer->NormalDateType(0) == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Normal = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Normal",FuntionBody);
				FuntionBody +=_T(";\n");
			}


		}	

		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody += "Tangent = Input.Tangent;\n";
			if (pVBuffer->TangentDateType() == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Tangent = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Tangent",FuntionBody);
				FuntionBody +=_T(";\n");
			}
			if(pVBuffer->HaveBinormalInfo())
			{
				FuntionBody += "Binormal = Input.Binormal;\n";
				if (pVBuffer->BinormalDateType() == SKDataBuffer::DT_UBYTE4N)
				{
					FuntionBody += "Binormal = ";
					SKRenderer::ms_pRenderer->DecodeNormal1("Binormal",FuntionBody);
					FuntionBody +=_T(";\n");
				}

			}
			else
			{
				FuntionBody += "Binormal = ";
				SKRenderer::ms_pRenderer->DecodeNormal2(_T("Normal"),_T("Tangent"),FuntionBody);
				FuntionBody +=_T(";\n");
			}

		}



		if(pVBuffer->HaveNormalInfo(0))
		{
			FuntionBody += "Normal = ";
			SKRenderer::ms_pRenderer->BoneTranNormal("Normal","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");
		}

		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody += "Tangent.xyz = ";
			SKRenderer::ms_pRenderer->BoneTranNormal("Tangent.xyz","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");

			FuntionBody += "Binormal = ";
			SKRenderer::ms_pRenderer->BoneTranNormal("Binormal","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");
		}

	}
	else
	{	
		FuntionBody +=_T("Position = Input.Position0;\n");


		if(pVBuffer->HaveNormalInfo(0))
		{	
			FuntionBody +=_T("Normal = Input.Normal0;\n");
			if (pVBuffer->NormalDateType(0) == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Normal = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Normal",FuntionBody);
				FuntionBody +=_T(";\n");
			}
		}



		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody +=_T("Tangent = Input.Tangent;\n");
			if (pVBuffer->TangentDateType() == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Tangent = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Tangent",FuntionBody);
				FuntionBody +=_T(";\n");
			}
			if(pVBuffer->HaveBinormalInfo())
			{
				FuntionBody +=_T("Binormal = Input.Binormal;\n");
			}
			else
			{
				FuntionBody += "Binormal = ";
				SKRenderer::ms_pRenderer->DecodeNormal2(_T("Normal"),_T("Tangent"),FuntionBody);
				FuntionBody +=_T(";\n");
			}
		}



	}
	FuntionBody += _T("Out.Position = mul(float4(Position,1), ") +
		SKShaderStringFactory::ms_WorldViewProjectMatrix + _T(");\n");


	if(pVBuffer->HaveNormalInfo(0))
	{	
		FuntionBody +=_T("Out.Normal = Normal;\n");

	}


	if(pVBuffer->HaveTangentInfo())
	{
		FuntionBody +=_T("Out.Tangent = Tangent.xyz;\n");
		FuntionBody +=_T("Out.Binormal = Binormal;\n");
	}



	
}
void SKDX9Renderer::CreateVFuntionVolumeShadowPositionAndNormal(MaterialShaderPara &MSPara,SKString & FuntionBody,unsigned int uiPass)
{
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	FuntionBody +=_T("float3 Position;\n");


	if(pVBuffer->HaveNormalInfo(0))
	{
		FuntionBody +=_T("float3 Normal;\n");
	}

	if(pVBuffer->HaveTangentInfo())
	{
		FuntionBody +=_T("float4 Tangent;\n");
		FuntionBody +=_T("float3 Binormal;\n");
	}



	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo())
	{
		FuntionBody +=_T("float4 U = 0;\n");
		FuntionBody +=_T("float4 V = 0;\n");
		FuntionBody +=_T("float4 N = 0;\n");

		SKRenderer::ms_pRenderer->ComputeBoneVector("Input.BlendIndices","Input.BlendWeight",SKShaderStringFactory::ms_BoneMatrix
			,"U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");

		FuntionBody += "Position = ";
		SKRenderer::ms_pRenderer->BoneTranPos("float4(Input.Position0,1)","U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");



		if(pVBuffer->HaveNormalInfo(0))
		{
			FuntionBody += "Normal = Input.Normal0;\n";
			if (pVBuffer->NormalDateType(0) == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Normal = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Normal",FuntionBody);
				FuntionBody +=_T(";\n");
			}


		}	

		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody += "Tangent = Input.Tangent;\n";
			if (pVBuffer->TangentDateType() == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Tangent = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Tangent",FuntionBody);
				FuntionBody +=_T(";\n");
			}
			if(pVBuffer->HaveBinormalInfo())
			{
				FuntionBody += "Binormal = Input.Binormal;\n";
				if (pVBuffer->BinormalDateType() == SKDataBuffer::DT_UBYTE4N)
				{
					FuntionBody += "Binormal = ";
					SKRenderer::ms_pRenderer->DecodeNormal1("Binormal",FuntionBody);
					FuntionBody +=_T(";\n");
				}

			}
			else
			{
				FuntionBody += "Binormal = ";
				SKRenderer::ms_pRenderer->DecodeNormal2(_T("Normal"),_T("Tangent"),FuntionBody);
				FuntionBody +=_T(";\n");
			}

		}



		if(pVBuffer->HaveNormalInfo(0))
		{
			FuntionBody += "Normal = ";
			SKRenderer::ms_pRenderer->BoneTranNormal("Normal","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");
		}

		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody += "Tangent.xyz = ";
			SKRenderer::ms_pRenderer->BoneTranNormal("Tangent.xyz","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");

			FuntionBody += "Binormal = ";
			SKRenderer::ms_pRenderer->BoneTranNormal("Binormal","U","V","N",FuntionBody);
			FuntionBody +=_T(";\n");
		}

	}
	else
	{	
		FuntionBody +=_T("Position = Input.Position0;\n");


		if(pVBuffer->HaveNormalInfo(0))
		{	
			FuntionBody +=_T("Normal = Input.Normal0;\n");
			if (pVBuffer->NormalDateType(0) == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Normal = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Normal",FuntionBody);
				FuntionBody +=_T(";\n");
			}
		}



		if(pVBuffer->HaveTangentInfo())
		{
			FuntionBody +=_T("Tangent = Input.Tangent;\n");
			if (pVBuffer->TangentDateType() == SKDataBuffer::DT_UBYTE4N)
			{
				FuntionBody += "Tangent = ";
				SKRenderer::ms_pRenderer->DecodeNormal1("Tangent",FuntionBody);
				FuntionBody +=_T(";\n");
			}
			if(pVBuffer->HaveBinormalInfo())
			{
				FuntionBody +=_T("Binormal = Input.Binormal;\n");
			}
			else
			{
				FuntionBody += "Binormal = ";
				SKRenderer::ms_pRenderer->DecodeNormal2(_T("Normal"),_T("Tangent"),FuntionBody);
				FuntionBody +=_T(";\n");
			}
		}



	}
	
	FuntionBody += _T("Position = mul(float4(Position,1), ") +
		SKShaderStringFactory::ms_WorldMatrix + _T(").xyz;\n");

	FuntionBody += _T("float3 WorldNormal = mul(Normal, (float3x3)") +
		SKShaderStringFactory::ms_WorldMatrix + _T(").xyz;\n");

	if ( uiPass == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		FuntionBody += _T("float3 Dir = ") + SKShaderStringFactory::ms_LightWorldPos + _T(" - Position;\n");

		FuntionBody += _T("Dir = normalize(Dir);\n");
	}
	else if( uiPass == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		FuntionBody += _T("float3 Dir = -") + SKShaderStringFactory::ms_LightWorldDirection + _T(";\n");

	}
	
	
	FuntionBody += _T("Position = dot(Dir,WorldNormal) > 0.0f ? Position: Position - Dir * 5000.0f;\n");

	FuntionBody += _T("Out.Position = mul(float4(Position,1), ") +
		SKShaderStringFactory::ms_ViewProjectMatrix + _T(");\n");

	FuntionBody += _T("Out.Position.z += 0.0005f; \n");
	if(pVBuffer->HaveNormalInfo(0))
	{	
		FuntionBody +=_T("Out.Normal = Normal;\n");

	}


	if(pVBuffer->HaveTangentInfo())
	{
		FuntionBody +=_T("Out.Tangent = Tangent.xyz;\n");
		FuntionBody +=_T("Out.Binormal = Binormal;\n");
	}




}
void SKDX9Renderer::CreateVFuntionLocalPosition(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	FuntionBody +=_T("Out.Pos = Position;\n");	
}
void SKDX9Renderer::CreateVFuntionColor(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(i))
		{
			SKString ColorID = IntToString(i);
			FuntionBody += _T("Out.Color") + ColorID + _T(" = Input.Color") + ColorID + _T(";\n");
		}
	}
}
void SKDX9Renderer::CreateVFuntionTexCoord(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(0))
		{
			SKString TextureID = IntToString(i);
			FuntionBody += _T("Out.TexCoord") + TextureID + _T(" = Input.TexCoord") + TextureID + _T(";\n");
		}
	}
}
void SKDX9Renderer::CreateVFuntionWorldPosition(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	FuntionBody += _T("Out.WorldPos = WorldPos.xyz;\n");
}
void SKDX9Renderer::CreateVFuntionProjectPosition(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	FuntionBody += _T("Out.ProjectPos = Out.Position;\n");
}
void SKDX9Renderer::CreateVFuntionPosition(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	FuntionBody +=_T("float3 Position;\n");

	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo())
	{
		FuntionBody +=_T("float4 U = 0;\n");
		FuntionBody +=_T("float4 V = 0;\n");
		FuntionBody +=_T("float4 N = 0;\n");

		SKRenderer::ms_pRenderer->ComputeBoneVector("Input.BlendIndices","Input.BlendWeight",SKShaderStringFactory::ms_BoneMatrix
			,"U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");

		FuntionBody += "Position = ";
		SKRenderer::ms_pRenderer->BoneTranPos("float4(Input.Position0,1)","U","V","N",FuntionBody);
		FuntionBody +=_T(";\n");
	}
	else
	{	
		FuntionBody +=_T("Position = Input.Position0;\n");
	}
	FuntionBody += _T("Out.Position = mul(float4(Position,1), ") +
		SKShaderStringFactory::ms_WorldViewProjectMatrix + _T(");\n");
}
void SKDX9Renderer::CreateVFuntionProjectZ(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	FuntionBody += _T("Out.ProjectZ = Out.Position.z / Out.Position.w;\n");
}
void SKDX9Renderer::CreateVFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
								   SKString & OutString)
{
	SKString FuntionBody;
	if (uiPassType == SKPass::PT_MATERIAL 
		|| uiPassType == SKPass::PT_INDIRECT
		|| uiPassType == SKPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == SKPass::PT_SHADOW
		|| uiPassType == SKPass::PT_NORMALDEPTH)
	{
		CreateVFuntionPositionAndNormal(MSPara,FuntionBody);
		CreateVFuntionColor(MSPara,FuntionBody);
		CreateVFuntionTexCoord(MSPara,FuntionBody);
		CreateVFuntionLocalPosition(MSPara,FuntionBody);		
	}
	else if (uiPassType == SKPass::PT_POINT_VOLUME_SHADOW || uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreateVFuntionVolumeShadowPositionAndNormal(MSPara,FuntionBody,uiPassType);	
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreateVFuntionDualParaboloidPositionAndNormal(MSPara,FuntionBody);
		CreateVFuntionColor(MSPara,FuntionBody);
		CreateVFuntionTexCoord(MSPara,FuntionBody);
		CreateVFuntionLocalPosition(MSPara, FuntionBody);
		CreateVFuntionProjectZ(MSPara,FuntionBody);
	}
	else if (uiPassType == SKPass::PT_PREZ)
	{	
		CreateVFuntionPosition(MSPara,FuntionBody);
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION)
	{
		CreateVFuntionPost(MSPara,FuntionBody);
	}
	OutString += _T("SK_OUTPUT ") + ms_VShaderProgramMain + _T("( SK_INPUT Input)\n{\nVS_OUTPUT Out = (SK_OUTPUT) 0; \n") + FuntionBody + _T("return Out;\n};\n");	
}
void SKDX9Renderer::CreateVFuntionPost(MaterialShaderPara &MSPara,SKString & FuntionBody)
{
	FuntionBody += _T("Out.Position = float4(Input.Position0.xy, 0, 1);\n");
	FuntionBody += _T("Out.TexCoord0.xy = 0.5 * (1 + Input.Position0.xy * float2(1,-1) + ") + SKShaderStringFactory::ms_InvRTWidth + _T(");\n");

}
void SKDX9Renderer::CreatePInputDeclareTexCoord(SKVertexBuffer * pVBuffer,unsigned int& ID,
								 SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	{
		if(pVBuffer->HaveTexCoordInfo(i))
		{
			SKString TextureID = IntToString(ID);
			OutString += _T("float2 TexCoord") + TextureID+ _T(":TEXCOORD") + TextureID + _T(";\n");
			SKShaderStringFactory::ms_PSTextureInputCoordValue[i] = _T("ps_Input.TexCoord") + TextureID;
			ID++;
		}
	}
}
void SKDX9Renderer::CreatePInputDeclareNormal(SKVertexBuffer * pVBuffer,unsigned int& ID,
							   SKString & OutString)
{
	if(pVBuffer->HaveNormalInfo(0))
	{
		SKString TextureID = IntToString(ID);
		OutString += _T("float3 Normal:TEXCOORD") + TextureID + _T(";\n");
		SKShaderStringFactory::ms_PSInputLocalNormal = _T("ps_Input.Normal");
		ID++;
	}



	if(pVBuffer->HaveTangentInfo())
	{
		{
			SKString TextureID = IntToString(ID);
			OutString += _T("float3 Tangent:TEXCOORD") + TextureID + _T(";\n");
			SKShaderStringFactory::ms_PSInputLocalTangent = _T("ps_Input.Tangent");
			ID++;
		}

		{
			SKString TextureID = IntToString(ID);
			OutString += _T("float3 Binormal:TEXCOORD") + TextureID + _T(";\n");
			SKShaderStringFactory::ms_PSInputLocalBinormal = _T("ps_Input.Binormal");
			ID++;
		}
	}
}
void SKDX9Renderer::CreatePInputDeclareColor(SKVertexBuffer * pVBuffer,
							  SKString & OutString)
{
	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	{

		if(pVBuffer->HaveColorInfo(i))
		{
			SKString ColorID = IntToString(i);
			OutString += _T("float4 Color") + ColorID + _T(":COLOR") + ColorID + _T(";\n");
			SKShaderStringFactory::ms_PSColor[i] = _T("ps_Input.Color") + ColorID;
		}
	}
}

void SKDX9Renderer::CreatePInputDeclareLocalPos(unsigned int& ID,
								 SKString & OutString)
{
	SKString TextureID = IntToString(ID);
	OutString += _T("float3 Pos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
	SKShaderStringFactory::ms_PSInputLocalPos = _T("ps_Input.Pos");
}

void SKDX9Renderer::CreatePInputDeclareWorldPos(unsigned int& ID,
												SKString & OutString)
{
	SKString TextureID = IntToString(ID);
	OutString += _T("float3 WorldPos:TEXCOORD") + TextureID + _T(";\n");
	ID++;
	SKShaderStringFactory::ms_PSInputWorldPos = _T("ps_Input.WorldPos");
}

void SKDX9Renderer::CreatePInputDeclareProjectPos(unsigned int& ID,
							  SKString & OutString)
{
	SKString TextureID = IntToString(ID);
	OutString += _T("float4 ProjectPos:TEXCOORD") + TextureID + _T(";\n");
	SKShaderStringFactory::ms_PSInputProjectPos = _T("ps_Input.ProjectPos");
	ID++;
}
void SKDX9Renderer::CreatePInputDeclareProjectZ(unsigned int& ID,
								 SKString & OutString)
{
	SKString TextureID = IntToString(ID);
	OutString += _T("float ProjectZ:TEXCOORD") + TextureID + _T(";\n");
	SKShaderStringFactory::ms_PSInputProjectZ = _T("ps_Input.ProjectZ");
	ID++;
}
void SKDX9Renderer::CreatePInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString)
{
	SKString TempDeclare;
	unsigned int j = 0;
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	if (uiPassType == SKPass::PT_MATERIAL
		|| uiPassType == SKPass::PT_INDIRECT
		|| uiPassType == SKPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == SKPass::PT_SHADOW
		|| uiPassType == SKPass::PT_NORMALDEPTH)
	{

		CreatePInputDeclareTexCoord(pVBuffer, j, TempDeclare);
		CreatePInputDeclareNormal(pVBuffer, j, TempDeclare);
		CreatePInputDeclareColor(pVBuffer, TempDeclare);
		CreatePInputDeclareLocalPos(j, TempDeclare);

		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == SKPass::PT_POINT_VOLUME_SHADOW || uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		CreatePInputDeclareTexCoord(pVBuffer, j, TempDeclare);
		CreatePInputDeclareNormal(pVBuffer, j, TempDeclare);
		CreatePInputDeclareColor(pVBuffer, TempDeclare);
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		CreatePInputDeclareTexCoord(pVBuffer,j,TempDeclare);
		CreatePInputDeclareNormal(pVBuffer,j,TempDeclare);	
		CreatePInputDeclareColor(pVBuffer,TempDeclare);
		CreatePInputDeclareLocalPos(j, TempDeclare);
		CreatePInputDeclareProjectZ(j,TempDeclare);	
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == SKPass::PT_PREZ)
	{
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION || uiPassType == SKPass::PT_POSTEFFECT)
	{
		CreatePInputDeclareTexCoord(pVBuffer,j,TempDeclare);
		OutString += _T("struct PS_INPUT \n{ \n") + TempDeclare + _T("};\n");
	}

}
void SKDX9Renderer::CreatePOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										 SKString & OutString)
{

	SKString TempDeclare;
	TempDeclare += _T("float4 Color0:COLOR0;");
	SKShaderStringFactory::ms_PSOutputColorValue = _T("Out.Color0");
	OutString += _T("struct PS_OUTPUT \n{ \n") + TempDeclare + _T("\n};\n");

}
void SKDX9Renderer::CreateUserConstantWorldMatrix(SKShader * pShader,unsigned int& ID,
												  SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("row_major float4x4 ") + SKShaderStringFactory::ms_WorldMatrix + _T(" : register(c") + RegisterID + _T(");\n");
	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_WorldMatrix,NULL,sizeof(SKREAL) * 16,ID,4);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
	
}
void SKDX9Renderer::CreateUserConstantCameraWorldPos(SKShader * pShader,unsigned int& ID,
													 SKString & OutString)
{
	SKString RegisterID = IntToString(ID);
	OutString += _T("float3 ") + SKShaderStringFactory::ms_CameraWorldPos + _T(" : register(c") + RegisterID + _T(");\n");		 
	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_CameraWorldPos,NULL,sizeof(SKREAL) * 4,ID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	ID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantDirectionLight(SKShader * pShader,unsigned int& ID,unsigned int uiLightNum,
													 SKString & OutString)
{
	if (uiLightNum > 0)
	{
		SKString TypeString;
		GetLightType(SKLight::LT_DIRECTION,TypeString);
		SKString RegisterID = IntToString(ID);
		OutString += TypeString + _T(" ") + SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + IntToString(uiLightNum) + _T("] : register(c") + RegisterID + _T(");\n");

		SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION],NULL,sizeof(SKREAL) * 84 * uiLightNum,ID,21 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);
		ID += pUserConstant->GetRegisterNum();
	}
	

}
void SKDX9Renderer::CreateUserConstantPointLight(SKShader * pShader,unsigned int& ID,unsigned int uiLightNum,
												 SKString & OutString)
{
	if (uiLightNum > 0)
	{
		SKString TypeString;
		GetLightType(SKLight::LT_POINT,TypeString);
		SKString RegisterID = IntToString(ID);
		OutString += TypeString + _T(" ") + SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + IntToString(uiLightNum) + _T("] : register(c") + RegisterID + _T(");\n");

		SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_LightName[SKLight::LT_POINT],NULL,sizeof(SKREAL) * 52 * uiLightNum,ID,13 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);
		ID += pUserConstant->GetRegisterNum();
	}
}
void SKDX9Renderer::CreateUserConstantSpotLight(SKShader * pShader,unsigned int& ID,unsigned int uiLightNum,
												SKString & OutString)
{
	if (uiLightNum > 0)
	{
		SKString TypeString;
		GetLightType(SKLight::LT_SPOT,TypeString);
		SKString RegisterID = IntToString(ID);
		OutString += TypeString + _T(" ") + SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + IntToString(uiLightNum) + _T("] : register(c") + RegisterID + _T(");\n");

		SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT],NULL,sizeof(SKREAL) * 56 * uiLightNum,ID,14 * uiLightNum);
		pShader->m_pUserConstant.AddElement(pUserConstant);
		ID += pUserConstant->GetRegisterNum();
	}
}
void SKDX9Renderer::CreateUserConstantLightFuntion(SKPShader* pPShader,SKArray<SKLight*> & LightArray,unsigned int &ID,SKString & OutString)
{
	unsigned int uiLightFuntion = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->HaveLightFun())
		{
			OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
			OutString += SKShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFuntion)
				+ SetRegister(RT_S,ID) +_T(";\n");


			SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFuntion),SKTexture::TT_2D,ID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiLightFuntion++;
			ID++;
		}
	}
}
void SKDX9Renderer::CreateUserConstantShadow(SKPShader* pPShader,SKArray<SKLight*> & LightArray,unsigned int &ID,SKString & OutString)
{

	unsigned int uiShadowNum = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			if (pLocalLight->GetLightType() == SKLight::LT_POINT)
			{
				if(((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_CUB)
				{
					OutString +=Sampler(SKTexture::TT_CUBE) + _T(" ");
					OutString += SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum)
						+ SetRegister(RT_S,ID) +_T(";\n");
					

					SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKTexture::TT_CUBE,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
					
				}
				if(((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_VOLUME)
				{
					OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
					OutString += SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum)
						+ SetRegister(RT_S,ID) +_T(";\n");


					SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;

				}
				if(((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_DUAL_PARABOLOID)
				{
					OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
					OutString += SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("[2]")
						+ SetRegister(RT_S,ID) +_T(";\n");


					SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKTexture::TT_2D,ID,2);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;

// 					OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
// 					OutString += SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum)
// 						+ SetRegister(RT_S,ID) +_T(";\n");
// 
// 
// 					pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKTexture::TT_2D,ID);
// 					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;

				}
			}
			else if (pLocalLight->GetLightType() == SKLight::LT_DIRECTION)
			{
				if(((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_CSM)
				{
					OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
					OutString += SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum) + _T("[3]")
						+ SetRegister(RT_S,ID) +_T(";\n");


					SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKTexture::TT_2D,ID,3);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
					uiShadowNum++;
					ID++;
					uiShadowNum++;
					ID++;
				}
				else if (((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_PROJECT)
				{

				}
				else
				{
					OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
					OutString += SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum)
						+ SetRegister(RT_S,ID) +_T(";\n");


					SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
				}
				
			}
			else if (pLocalLight->GetLightType() == SKLight::LT_SPOT)
			{
				if (((SKSpotLight*)pLocalLight)->GetShadowType() == SKSpotLight::ST_NORMAL)
				{
					OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
					OutString += SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum)
						+ SetRegister(RT_S,ID) +_T(";\n");


					SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSConstantShadowSampler + IntToString(uiShadowNum),SKTexture::TT_2D,ID,1);
					pPShader->m_pUserSampler.AddElement(pSampler);
					uiShadowNum++;
					ID++;
				}
			}
			
		}
	}

}
void SKDX9Renderer::CreateGBufferSampler(SKPShader* pPShader, MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID, SKString & OutString)
{
	SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (pMaterial->GetPostEffectShaderFunction())
	{
		OutString += Sampler(SKTexture::TT_2D) + _T(" ");
		OutString += SKShaderStringFactory::ms_PSColorBufferSampler
			+ SetRegister(RT_S, uiTexRegisterID) + _T(";\n");


		SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSColorBufferSampler, SKTexture::TT_2D, uiTexRegisterID, 1);
		pPShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;

		OutString += Sampler(SKTexture::TT_2D) + _T(" ");
		OutString += SKShaderStringFactory::ms_PSDepthNormalBufferSampler
			+ SetRegister(RT_S, uiTexRegisterID) + _T(";\n");


		pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSDepthNormalBufferSampler, SKTexture::TT_2D, uiTexRegisterID, 1);
		pPShader->m_pUserSampler.AddElement(pSampler);
		uiTexRegisterID++;
	}
}
void SKDX9Renderer::CreateLightModleLookUpTable(SKPShader* pPShader,MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID,SKString & OutString)
{
	SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_OREN_NAYAR)
	{
		SKOrenNayarShaderFunction * pOrenNayarShaderFunction = (SKOrenNayarShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pOrenNayarShaderFunction->UseLookUpTable())
		{
			OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
			OutString += SKShaderStringFactory::ms_PSOrenNayarLookUpTableSampler
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");


			SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSOrenNayarLookUpTableSampler,SKTexture::TT_2D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
	else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_STRAUSS)
	{
		SKStraussShaderFunction * pStraussShaderFunction = (SKStraussShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pStraussShaderFunction->UseLookUpTable())
		{
			OutString +=Sampler(SKTexture::TT_1D) + _T(" ");
			OutString += SKShaderStringFactory::ms_PSStraussFLookUpTableSampler
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");


			SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSStraussFLookUpTableSampler,SKTexture::TT_1D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;

			OutString +=Sampler(SKTexture::TT_1D) + _T(" ");
			OutString += SKShaderStringFactory::ms_PSStraussSLookUpTableSampler
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");


			pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSStraussSLookUpTableSampler,SKTexture::TT_1D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
	else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_COOKTOORANCE)
	{
		SKCookTorranceShaderFunction * pCookTorranceShaderFunction = (SKCookTorranceShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pCookTorranceShaderFunction->GetSpecularType() == SKCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			OutString +=Sampler(SKTexture::TT_2D) + _T(" ");
			OutString += SKShaderStringFactory::ms_PSCookTorranceLookUpTableSampler
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");


			SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSCookTorranceLookUpTableSampler,SKTexture::TT_2D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;
		}
	}
	else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_ISOTROPICWARD)
	{
		SKIsotropicWardShaderFunction * pIsotropicWardShaderFunction = (SKIsotropicWardShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
		if (pIsotropicWardShaderFunction->UseLookUpTable())
		{
			OutString +=Sampler(SKTexture::TT_1D) + _T(" ");
			OutString += SKShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler
				+ SetRegister(RT_S,uiTexRegisterID) +_T(";\n");


			SKUserSampler * pSampler = SK_NEW SKUserSampler(SKShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler,SKTexture::TT_1D,uiTexRegisterID,1);
			pPShader->m_pUserSampler.AddElement(pSampler);
			uiTexRegisterID++;


		}
	}
}
void SKDX9Renderer::CreateUserConstantFarZ(SKShader * pShader,unsigned int& uiRegisterID,
							SKString & OutString)
{
	SKString RegisterID = IntToString(uiRegisterID);
	OutString += _T("float ") + SKShaderStringFactory::ms_FarZ + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_FarZ,NULL,sizeof(SKREAL) * 1,uiRegisterID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	uiRegisterID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantSkyLightUpColor(SKShader * pShader,unsigned int& uiRegisterID,
									   SKString & OutString)
{
	SKString RegisterID = IntToString(uiRegisterID);
	OutString += _T("float4 ") + SKShaderStringFactory::ms_SkyLightUpColor + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_SkyLightUpColor,NULL,sizeof(SKREAL) * 4,uiRegisterID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	uiRegisterID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantSkyLightDownColor(SKShader * pShader,unsigned int& uiRegisterID,
										 SKString & OutString)
{
	SKString RegisterID = IntToString(uiRegisterID);
	OutString += _T("float4 ") + SKShaderStringFactory::ms_SkyLightDownColor + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_SkyLightDownColor,NULL,sizeof(SKREAL) * 4,uiRegisterID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	uiRegisterID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantPointLightRange(SKShader * pShader,unsigned int& uiRegisterID,
									   SKString & OutString)
{
	SKString RegisterID = IntToString(uiRegisterID);
	OutString += _T("float ") + SKShaderStringFactory::ms_PointLightRange + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_PointLightRange,NULL,sizeof(SKREAL) * 1,uiRegisterID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	uiRegisterID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantInvRTWidth(SKShader * pShader,unsigned int& uiRegisterID,
							   SKString & OutString)
{
	SKString RegisterID = IntToString(uiRegisterID);
	OutString += _T("float ") + SKShaderStringFactory::ms_InvRTWidth + _T(" : register(c") + RegisterID + _T(");\n");

	SKUserConstant * pUserConstant = SK_NEW SKUserConstant(SKShaderStringFactory::ms_InvRTWidth,NULL,sizeof(SKREAL) * 1,uiRegisterID,1);
	pShader->m_pUserConstant.AddElement(pUserConstant);
	uiRegisterID += pUserConstant->GetRegisterNum();
}
void SKDX9Renderer::CreateUserConstantLight(SKShader * pShader,MaterialShaderPara &MSPara,unsigned int& uiRegisterID,
							 SKString & OutString)
{
	SKArray<SKLight*> & LightArray = MSPara.LightArray;

	int iLightNum[SKLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < SKLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;
				}
			}
		}
	}
	
	CreateUserConstantPointLight(pShader,uiRegisterID,iLightNum[SKLight::LT_POINT],OutString);
	CreateUserConstantSpotLight(pShader,uiRegisterID,iLightNum[SKLight::LT_SPOT],OutString);
	CreateUserConstantDirectionLight(pShader,uiRegisterID,iLightNum[SKLight::LT_DIRECTION],OutString);
}
void SKDX9Renderer::CreatePUserConstant(SKPShader* pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString)
{

	if (uiPassType == SKPass::PT_MATERIAL)
	{
		unsigned int uiRegisterID = 0;
		unsigned int uiLightNum = MSPara.LightArray.GetNum();
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantViewMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantFarZ(pPShader,uiRegisterID,OutString);
		if (uiLightNum)
		{			
			CreateUserConstantLight(pPShader,MSPara,uiRegisterID,OutString);	
		}
		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;
		CreateUserConstantShadow(pPShader,MSPara.LightArray,uiTexRegisterID,OutString);
		CreateUserConstantLightFuntion(pPShader,MSPara.LightArray,uiTexRegisterID,OutString);
		CreateLightModleLookUpTable(pPShader,MSPara,uiTexRegisterID,OutString);
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == SKPass::PT_INDIRECT)
	{
		unsigned int uiRegisterID = 0;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantViewMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantSkyLightUpColor(pPShader,uiRegisterID,OutString);
		CreateUserConstantSkyLightDownColor(pPShader,uiRegisterID,OutString);

		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == SKPass::PT_NORMALDEPTH)
	{
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantViewMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantProjectMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);
		CreateUserConstantFarZ(pPShader,uiRegisterID,OutString);
			
		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
		
	}
	else if (uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{

		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;
		
		CreateUserConstantWorldMatrix(pPShader,uiRegisterID,OutString);
		CreateUserConstantCameraWorldPos(pPShader,uiRegisterID,OutString);
		CreateUserConstantPointLightRange(pPShader,uiRegisterID,OutString);
		

		pMaterial->CreateConstValueDeclare(OutString,uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString,uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
		
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		unsigned int uiRegisterID = 0;

		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);


		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION
		|| uiPassType == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		unsigned int uiRegisterID = 0;

		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;

		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		unsigned int uiRegisterID = 0;

		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		CreateUserConstantLightShadowMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantWorldMatrix(pPShader, uiRegisterID, OutString);
		CreateUserConstantCameraWorldPos(pPShader, uiRegisterID, OutString);

		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;

		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}
	else if (uiPassType == SKPass::PT_POSTEFFECT)
	{
		unsigned int uiRegisterID = 0;

		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->CreateConstValueDeclare(OutString, uiRegisterID);
		pMaterial->CreateCustomValue(pPShader);

		unsigned uiTexRegisterID = 0;
		CreateGBufferSampler(pPShader, MSPara, uiTexRegisterID, OutString);
		pMaterial->CreateTextureDeclare(OutString, uiTexRegisterID);
		pMaterial->CreateCustomTexture(pPShader);
	}

	pPShader->m_bCreatePara = true;
}
void SKDX9Renderer::CreatePFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
								   SKString & OutString)
{
	if (uiPassType == SKPass::PT_MATERIAL)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,SKShaderMainFunction::OST_MATERIAL,MSPara.uiPassId);
		
		SKString SKCustomDeclareString;
		pMaterial->GetCustomDeclareString(SKCustomDeclareString,MSPara.uiPassId);
		OutString += SKCustomDeclareString + _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == SKPass::PT_INDIRECT)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,SKShaderMainFunction::OST_INDIRECT,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == SKPass::PT_NORMALDEPTH)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,SKShaderMainFunction::OST_NORMAL_DEPTH,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,SKShaderMainFunction::OST_CUB_SHADOW,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,SKShaderMainFunction::OST_SHADOW,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,SKShaderMainFunction::OST_DUAL_PARABOLOID_SHADOW,MSPara.uiPassId);

		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");	
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		SKMAC_ASSERT(pMaterial->GetLightShaderFunction());
		pMaterial->GetShaderTreeString(FuntionBody,MSPara,SKShaderMainFunction::OST_MAX,MSPara.uiPassId);
		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");		
	}
	else if (uiPassType == SKPass::PT_POSTEFFECT)
	{
		SKString FuntionBody;
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		SKMAC_ASSERT(pMaterial->GetPostEffectShaderFunction());
		pMaterial->GetShaderTreeString(FuntionBody, MSPara, SKShaderMainFunction::OST_MAX, MSPara.uiPassId);
		OutString += _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\nPS_OUTPUT Out = (PS_OUTPUT) 0;\n") + FuntionBody + _T("return Out;\n};\n");
	}
	else if (uiPassType == SKPass::PT_PREZ || uiPassType == SKPass::PT_POINT_VOLUME_SHADOW || uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		OutString = _T("PS_OUTPUT ") + ms_PShaderProgramMain + _T("(PS_INPUT ps_Input)\n{\n PS_OUTPUT Out = (PS_OUTPUT) 0;\nOut.Color0 = float4(0.0f,0.0f,0.0f,1.0f);\nreturn Out;\n};");
	}
}
void SKDX9Renderer::CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString)
{}
void SKDX9Renderer::CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString)
{}
void SKDX9Renderer::CreateGUserConstant(SKGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString)
{}
void SKDX9Renderer::CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString)
{}

void SKDX9Renderer::SkyLight(const SKString &WorldNormal,const SKString &UpColor,const SKString & DownColor,const SKString & DiffuseColor,SKString & OutString)
{
	OutString += _T(" + SkyLight(") + WorldNormal + _T(",") + UpColor + _T(",") + DownColor + _T(") * ") + DiffuseColor;
}
void SKDX9Renderer::CustomDirectionalLight(int iLightNum,const SKString &CustomString,SKArray<SKString> ShadowString,SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		//OutString += _T("+ float4(1.0f,1.0f,1.0f,1.0f)") + ShadowString[i];
		OutString += _T(" + CustomDirectionLightFun(") + CustomString
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void SKDX9Renderer::DirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
							  const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + DirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
					+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
					+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
					+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
					+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}

void SKDX9Renderer::SchlickDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
									 const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + SchlickDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void SKDX9Renderer::IsotropicWardDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
										   const SKString &Roughness ,
										   const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
										   SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + IsotropicWardDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",")
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + IsotropicWardDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",")
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T(")") + ShadowString[i];
		}

	}
}
void SKDX9Renderer::AnisotropicWardDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
											 const SKString &RoughnessX ,const SKString &RoughnessY ,
											 const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
											 SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + AnisotropicWardDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") + RoughnessX + _T(",") + RoughnessY + _T(",")
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	}
}
void SKDX9Renderer::CookTorranceDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
										  const SKString &Roughness , const SKString &C , const SKString &R,
										  const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
										  SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (uiSpecularType != SKCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			OutString += _T(" + CookTorranceDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",") + C + _T(",") + R + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + CookTorranceDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",") 
				+ Roughness + _T(",") + C + _T(",") + R + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,")
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T(")") + ShadowString[i];
		}

	}
}
void SKDX9Renderer::OrenNayarDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
									 const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + OrenNayarDirectionalLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + OrenNayarDirectionalLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T(")") + ShadowString[i];
		}
		
	}
}
void SKDX9Renderer::MinnaertDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
									  const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
									  SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);

		OutString += _T(" + MinnaertDirectionalLightFun(") + Diffuse + _T(",") + Power + _T(",") 
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")  
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];


	}
}
void SKDX9Renderer::StraussDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
									 const SKString &Metalness,const SKString &Transparency,const SKString &K,
									 const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
									 SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + StraussDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",")  
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + StraussDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T(",") 
				+ SKShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T(")") + ShadowString[i];
		}

	}
}
void SKDX9Renderer::ShirleyDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
									 const SKString &NU,const SKString &NV,const SKString &Rs,
									 const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
									 SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);

		OutString += _T(" + ShirleyDirectionalLightFun(") + Diffuse + _T(",") + Specular + _T(",")
			+ NU + _T(",") + NV + _T(",") + Rs + _T(",")
			+ WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightDiffuse,")
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i];
	

	}
}
void SKDX9Renderer::PointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
							const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + PointLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}
	
}
void SKDX9Renderer::CustomPointLight(int iLightNum, const SKString &CustomString,const SKString & WorldPos,SKArray<SKString> ShadowString,SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + CustomPointLightFun(") + CustomString
			+ WorldPos + _T(",")
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}
}
void SKDX9Renderer::SchlickPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
							   const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							   SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + SchlickPointLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)")  + ShadowString[i];
	}

}
void SKDX9Renderer::OrenNayarPointLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
										 const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
										 SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + OrenNayarPointLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 	 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i];
		}
		else
		{
			OutString += _T(" + OrenNayarPointLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,")
				+ SKShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T(")")  + ShadowString[i];
		}
	}
}
void SKDX9Renderer::MinnaertPointLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);

		OutString += _T(" + MinnaertPointLightFun(") + Diffuse + _T(",") + Power + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;

	}
}
void SKDX9Renderer::StraussPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
										   const SKString &Metalness,const SKString &Transparency,const SKString &K,
										   const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
										   SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + StraussPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
		}
		else
		{
			OutString += _T(" + StraussPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,")
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,")
				+ SKShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T(",") 
				+ SKShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T(")") + ShadowString[i] ;
		}
	}
}
void SKDX9Renderer::IsotropicWardPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
									 const SKString &Roughness ,
									 const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
									 SKString & OutString)const
{

	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (bLookUpTable == false)
		{
			OutString += _T(" + IsotropicWardPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
		}
		else
		{
			OutString += _T(" + IsotropicWardPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,")
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T(")") + ShadowString[i] ;
		}
	}
}
void SKDX9Renderer::AnisotropicWardPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
									   const SKString &RoughnessX ,const SKString &RoughnessY ,
									   const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
									   SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + AnisotropicWardPointLightFun(") + Diffuse + _T(",") + Specular + _T(",") + RoughnessX + _T(",") + RoughnessY + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
	}
}
void SKDX9Renderer::CookTorrancePointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
									const SKString &Roughness , const SKString &C , const SKString &R,
									const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
									SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		if (uiSpecularType != SKCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			OutString += _T(" + CookTorrancePointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
		}
		else
		{
			OutString += _T(" + CookTorrancePointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,")
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T(")") + ShadowString[i] ;
		}
	}
}
void SKDX9Renderer::ShirleyPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
							   const SKString &NU,const SKString &NV,const SKString &Rs,
							   const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							   SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);

		OutString += _T(" + ShirleyPointLightFun(") + Diffuse + _T(",") + Specular + _T(",")
			+ NU + _T(",") + NV + _T(",") + Rs + _T(",")
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_POINT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.w,") 				 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_POINT] + _T("[") + ID + _T("].LightWorldPos.xyz)") + ShadowString[i] ;
	
	}
}
void SKDX9Renderer::SpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
					   const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
					   SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + SpotLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void SKDX9Renderer::CustomSpotLight(int iLightNum,const SKString &CustomString,const SKString & WorldPos,SKArray<SKString> ShadowString,SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + CustomSpotLightFun(") + CustomString
			+ WorldPos + _T(",")
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void SKDX9Renderer::SchlickSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
							  const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							  SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + SchlickSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",") + SpecularPow + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,")  
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;

	}
}
void SKDX9Renderer::OrenNayarSpotLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
								SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (bLookUpTable == false)
		{
			SKString ID = IntToString(i);
			OutString += _T(" + OrenNayarSpotLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			SKString ID = IntToString(i);
			OutString += _T(" + OrenNayarSpotLightFun(") + Diffuse + _T(",") + RoughnessSquared + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSOrenNayarLookUpTableSampler + _T(")") + ShadowString[i];
		}

	}
}
void SKDX9Renderer::MinnaertSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{

		SKString ID = IntToString(i);
		OutString += _T(" + MinnaertSpotLightFun(") + Diffuse + _T(",") + Power + _T(",") 
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 	 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
	

	}
}
void SKDX9Renderer::StraussSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
							  const SKString &Metalness,const SKString &Transparency,const SKString &K,
							  const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
							  SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (bLookUpTable == false)
		{
			SKString ID = IntToString(i);
			OutString += _T(" + StraussSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			SKString ID = IntToString(i);
			OutString += _T(" + StraussSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Smoothness + _T(",") + Metalness + _T(",") + Transparency + _T(",") + K + _T(",")  
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSStraussFLookUpTableSampler + _T(",") 
				+ SKShaderStringFactory::ms_PSStraussSLookUpTableSampler + _T(")") + ShadowString[i] ;
		}

	}
}
void SKDX9Renderer::IsotropicWardSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
									const SKString &Roughness ,
									const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
									SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (bLookUpTable == false)
		{
			SKString ID = IntToString(i);
			OutString += _T(" + IsotropicWardSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 			 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			SKString ID = IntToString(i);
			OutString += _T(" + IsotropicWardSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 		 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler + _T(")") + ShadowString[i] ;
		}

	}
}
void SKDX9Renderer::AnisotropicWardSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
									  const SKString &RoughnessX ,const SKString &RoughnessY ,
									  const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
									  SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		SKString ID = IntToString(i);
		OutString += _T(" + AnisotropicWardSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",") + RoughnessX + _T(",")  + RoughnessY + _T(",")
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 	 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
	}

}
void SKDX9Renderer::CookTorranceSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
								   const SKString &Roughness , const SKString &C , const SKString &R,
								   const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
								   SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{
		if (uiSpecularType != SKCookTorranceShaderFunction::ST_BACKMANN_LOOKUP)
		{
			SKString ID = IntToString(i);
			OutString += _T(" + CookTorranceSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",")
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		}
		else
		{
			SKString ID = IntToString(i);
			OutString += _T(" + CookTorranceSpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
				+ Roughness + _T(",") + C + _T(",") + R + _T(",") 
				+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
				+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
				+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz,")
				+ SKShaderStringFactory::ms_PSCookTorranceLookUpTableSampler + _T(")") + ShadowString[i] ;
		}

	}
}
void SKDX9Renderer::ShirleySpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
							  const SKString &NU,const SKString &NV,const SKString &Rs,
							  const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							  SKString & OutString)const
{
	for (int i = 0 ; i < iLightNum ; i++)
	{

		SKString ID = IntToString(i);
		OutString += _T(" + ShirleySpotLightFun(") + Diffuse + _T(",") + Specular + _T(",")
			+ NU + _T(",") + NV + _T(",") + Rs + _T(",")
			+ WorldPos + _T(",") + WorldNormal + _T(",") + WorldCameraDir + _T(",") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightDiffuse,") 
			+ SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_DIRECTION] + _T("[") + ID + _T("].LightSpecular,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.w,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.x,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.y,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightParam.z,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldPos.xyz,") 
			+ SKShaderStringFactory::ms_LightName[SKLight::LT_SPOT] + _T("[") + ID + _T("].LightWorldDirection.xyz)") + ShadowString[i] ;
		

	}
}
void SKDX9Renderer::LocalToWorldPos(const SKString & LocalPos,SKString &OutString)const
{
	OutString += _T("TransPos(") + LocalPos + _T(",") + 
					SKShaderStringFactory::ms_WorldMatrix + _T(")");
}
void SKDX9Renderer::WorldToViewPos(const SKString & WorldPos,SKString &OutString)const
{
	OutString += _T("TransPos(") + WorldPos + _T(",") + 
		SKShaderStringFactory::ms_ViewMatrix + _T(")");
}
void SKDX9Renderer::TransProjPos(const SKString & Pos,const SKString & Matrix,SKString &OutString)const
{
	OutString += _T("TransProjPos(") + Pos + _T(",") + Matrix + _T(")");
}
void SKDX9Renderer::LocalToViewNormal(const SKString & LocalNormal,SKString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") + 
		SKShaderStringFactory::ms_WorldViewMatrix + _T(")");
}
void SKDX9Renderer::LocalToWorldNormal(const SKString & LocalNormal,SKString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") + 
		SKShaderStringFactory::ms_WorldMatrix + _T(")");
}
void SKDX9Renderer::GetWorldViewDir(SKString &OutString)const
{
	OutString +=  _T("GetZDir(") + SKShaderStringFactory::ms_ViewMatrix + _T(")");
	
}
void SKDX9Renderer::WorldToViewNormal(const SKString & WorldNormal,SKString &OutString)const
{
	OutString += _T("TransNormal(") + WorldNormal + _T(",") + 
		SKShaderStringFactory::ms_ViewMatrix + _T(")");
}
void SKDX9Renderer::BumpNormal(const SKString &TexNormal,SKString &OutString)const
{
	OutString += _T("BumpNormal(") + SKShaderStringFactory::ms_PSInputLocalTangent + _T(",") 
				+ SKShaderStringFactory::ms_PSInputLocalBinormal + _T(",")
				+ SKShaderStringFactory::ms_PSInputLocalNormal + _T(",")
				+ TexNormal + _T(")");
}
void SKDX9Renderer::ComputeDir(const SKString & Orgin,const SKString & End,SKString &OutString)const
{
	OutString += _T("ComputeDir(") + Orgin + _T(",")
				+ End + _T(")");
}
void SKDX9Renderer::ComputeLength(const SKString & Orgin,const SKString & End,SKString &OutString)const
{
	OutString += _T("ComputeLength(") + Orgin + _T(",")
		+ End + _T(")");
}
void SKDX9Renderer::GetLightType(unsigned int i,SKString &OutString)const
{
	if (i == SKLight::LT_DIRECTION)
	{ 
		OutString = _T("DirLightType");
	}
	else if (i == SKLight::LT_POINT)
	{
		OutString = _T("PointLightType");
	}
	else if (i == SKLight::LT_SPOT)
	{
		OutString = _T("SpotLightType");
	}
}
void SKDX9Renderer::GetLightTypeTemp(SKString &OutString)const
{

	OutString = _T("LightTypeTemp");

}
void SKDX9Renderer::ComputeBoneVector(const SKString& BoneIndex, const SKString & BoneWeight,const SKString &BoneVector,
							  const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const
{
	OutString += _T("ComputeBoneVector(") + BoneIndex + _T(",")
		+ BoneWeight + _T(",") + BoneVector + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
void SKDX9Renderer::BoneTranPos( const SKString & LocalPos,const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const
{
	OutString += _T("TransPos(") + LocalPos + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
 void SKDX9Renderer::BoneTranNormal( const SKString & LocalNormal,const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const
{
	OutString += _T("TransNormal(") + LocalNormal + _T(",") +  U + _T(",") + V + _T(",") + N  + _T(")");
}
void SKDX9Renderer::DecodeNormal1(const SKString & Normal,SKString &OutString) const
{
	OutString += _T("DecodeNormal1(") + Normal + _T(")");
}
void SKDX9Renderer::RecodeNormal1(const SKString & Normal,SKString &OutString) const
{
	OutString += _T("RecodeNormal1(") + Normal + _T(")");
}
void SKDX9Renderer::DecodeNormal2(const SKString & Normal,const SKString & Tengent,SKString &OutString) const
{
	OutString += _T("DecodeNormal2(") + Normal + _T(",") + Tengent + _T(")");
}
void SKDX9Renderer::DecodeNormal3(const SKString & Normal,SKString &OutString) const
{
	OutString += _T("DecodeNormal3(") + Normal + _T(")");
}
void SKDX9Renderer::RecodeNormal3(const SKString & Normal,SKString &OutString) const
{
	OutString += _T("RecodeNormal3(") + Normal + _T(")");
}
void SKDX9Renderer::EncodeReflect(const SKString & Mip , const SKString & Pow , SKString& OutString)
{
	OutString += _T("EncodeReflect(") + Mip + _T(",") + Pow + _T(")");
}
void SKDX9Renderer::DecodeReflect(const SKString & Mip , const SKString & Pow , const SKString & Value,SKString& OutString)
{
	OutString += _T("DecodeReflect(") + Mip + _T(",") + Pow + _T(",") + Value + _T(")");
}
void SKDX9Renderer::GreaterZeroPow(const SKString & Value,SKREAL Exp,SKString &OutString) const
{
	SKString SExp = RealToString(Exp);
	OutString += _T("GreaterZeroPow(") + Value + _T(",") + SExp + _T(")");
}
void SKDX9Renderer::Reciprocal(const SKString & Value, SKString &OutString)const
{
	OutString += _T("1.0 / (") + Value + _T(")");
}
void SKDX9Renderer::Clip(const SKString & ClipValue,SKString &OutString)const
{
	OutString += _T("clip(") + ClipValue + _T(")");
}
void SKDX9Renderer::SaturatePow(const SKString & Value,SKREAL Exp,SKString &OutString) const
{
	SKString SExp = RealToString(Exp);
	OutString += _T("SaturatePow(") + Value + _T(",") + SExp + _T(")");
}
void SKDX9Renderer::Saturate(const SKString & Value,SKString &OutString)const
{
	OutString += _T("saturate(") + Value + _T(")");
}
void SKDX9Renderer::PointLightCubeShadow(const SKString & PointLightName , const SKString & CubeShadowSampler , const SKString & ViewMatrix,
										 const SKString & WorldPos, SKString &OutString)const
{

	OutString += _T("PointLightCubShadow(") + PointLightName +_T(",") + WorldPos + _T(",") + ViewMatrix + _T(",") + CubeShadowSampler + _T(")");
	
}
void SKDX9Renderer::PointLightCubeShadow(const SKString & PointLightName , const SKString & CubeShadowSampler ,
										 const SKString & WorldPos, SKString &OutString)const
{

	OutString += _T("PointLightCubShadow(") + PointLightName +_T(",") + WorldPos + _T(",") + CubeShadowSampler + _T(")");

}
void SKDX9Renderer::PointLightVolumeShadow(const SKString & PointLightName , const SKString & VolumeShadowSampler , 
									const SKString & WorldPos, SKString& OutString)const
{
	OutString += _T("PointLightVolumeShadow(") + PointLightName +_T(",") + WorldPos + _T(",") + VolumeShadowSampler + _T(")");
}
void SKDX9Renderer::PointLightDualParaboloidShadow(const SKString & PointLightName , const SKString & ShadowSampler ,  
											const SKString & WorldPos, SKString& OutString)const
{
	OutString += _T("PointLightDualParaboloidShadow(") + PointLightName +_T(",") + WorldPos + _T(",") + ShadowSampler +  _T(")");
}
void SKDX9Renderer::DirectionLightVolumeShadow(const SKString & DirLightName , const SKString & VolumeShadowSampler , 
										const SKString & WorldPos, SKString& OutString)const
{
	OutString += _T("DirectionLightVolumeShadow(") + DirLightName +_T(",") + WorldPos + _T(",") + VolumeShadowSampler + _T(")");
}
void SKDX9Renderer::DirectionLightCSMShadow(const SKString & DirLightName , const SKString & ShadowSampler , 
									 const SKString & WorldPos, const SKString & ViewPos,const SKString &FarZ,SKString& OutString)const
{
	OutString += _T("DirLightCSMShadow(") + DirLightName +_T(",") + WorldPos + _T(",") +  ViewPos + _T(",") + FarZ + _T(",") + ShadowSampler +_T(")");
}
void SKDX9Renderer::DirectionLightShadow(const SKString & DirLightName , const SKString & ShadowSampler , 
								  const SKString & WorldPos, SKString& OutString)const
{
	OutString += _T("DirLightShadow(") + DirLightName +_T(",") + WorldPos + _T(",") + ShadowSampler + _T(")");
}
void SKDX9Renderer::SpotLightShadow(const SKString & SpotLightName , const SKString & ShadowSampler , 
							 const SKString & WorldPos, SKString& OutString)const
{
	OutString += _T("SpotLightShadow(") + SpotLightName +_T(",") + WorldPos + _T(",") + ShadowSampler + _T(")");
}
void SKDX9Renderer::GetLightFunction(SKArray<SKLight *> LightArray,const SKString & WorldPos,SKString & OutString)const
{
	unsigned int iLightNum[SKLight::LT_MAX] = { 0 };
	unsigned int uiLightFunSampler = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{

		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(LightArray[i]);
		if (pLocalLight)
		{
			for (unsigned int j = 0 ; j < SKLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{
					if (pLocalLight->HaveLightFun())
					{
						OutString += _T("float4 FunReslut = float4(0.0f,0.0f,0.0f,1.0f);\n");
						
						if (j == SKLight::LT_POINT)
						{
							OutString += _T("FunReslut = PointLightFunction(")
								+ SKShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightWorldPos.xyz,")
								+ WorldPos + _T(",")
								+ SKShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightFunParam,")
								+ SKShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFunSampler) + _T(");\n");
						}
						else
						{
							OutString += _T("FunReslut = LightFunction(")
								+ SKShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].WVP,")
								+ WorldPos + _T(",")
								+ SKShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightFunParam,")
								+ SKShaderStringFactory::ms_PSConstantLightFunSampler + IntToString(uiLightFunSampler) + _T(");\n");
						}
						
						OutString += SKShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightDiffuse *= FunReslut;\n");
						OutString += SKShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("].LightSpecular *= dot(FunReslut,float4(0.299,0.587,0.184,0));\n");
						uiLightFunSampler++;
					}
					
					iLightNum[j]++;
				}
			}
			
		}
	}
}
void SKDX9Renderer::TranLightToTemp(SKArray<SKLight *> LightArray,SKString & OutString)const
{
	unsigned int iLightNum[SKLight::LT_MAX] = { 0 };	
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < SKLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{

					iLightNum[j]++;

				}
			}
		}
	}
	SKString LightTypeTempString;
	GetLightTypeTemp(LightTypeTempString);
	for (unsigned int j = 0 ; j < SKLight::LT_MAX ; j++)
	{
		if (iLightNum[j] > 0)
		{
			OutString += LightTypeTempString + _T(" ") + SKShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(iLightNum[j]) + _T("];\n"); 
		}
		
	}
	for (unsigned int j = 0 ; j < SKLight::LT_MAX ; j++)
	{

		for (unsigned int k = 0 ; k < iLightNum[j] ; k++)
		{
			OutString += SKShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(k) + _T("].LightDiffuse = ") + 
				SKShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(k) + _T("].LightDiffuse;\n");  

			OutString += SKShaderStringFactory::ms_LightNameTemp[j] + _T("[") + IntToString(k) + _T("].LightSpecular = ") + 
				SKShaderStringFactory::ms_LightName[j] + _T("[") + IntToString(k) + _T("].LightSpecular;\n");  
		}

		
	}
}