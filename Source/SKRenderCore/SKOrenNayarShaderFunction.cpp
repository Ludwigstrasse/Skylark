#include "SKOrenNayarShaderFunction.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOrenNayarShaderFunction,SKShaderMainFunction)
BEGIN_ADD_PROPERTY(SKOrenNayarShaderFunction,SKShaderMainFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOrenNayarShaderFunction)
IMPLEMENT_INITIAL_END
SKOrenNayarShaderFunction::SKOrenNayarShaderFunction(const SKUsedName &ShowName,SKMaterial * pMaterial)
:SKShaderMainFunction(ShowName,pMaterial)
{
	
	SKString InputName = _T("DiffuseColor");
	SKInputNode * pInputNode = NULL;

	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("EmissiveColor");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_4,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("RoughnessSquared");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::VT_1,InputName,this);
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

bool SKOrenNayarShaderFunction::GetFuntionString(SKString &OutString)const
{
	GetAlphaTestString(OutString);
	if(!m_pInput[IN_DIFFUSE_COLOR]->GetOutputLink())
	{
		OutString +=  m_pOutput[OUT_COLOR]->GetNodeName().GetString() + _T(" = ") + SKRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));
		OutString += _T(";\n");

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


		OutString += SKRenderer::ms_pRenderer->Float3() + _T("WorldCameraDir = ");
		SKRenderer::ms_pRenderer->ComputeDir(SKShaderStringFactory::ms_CameraWorldPos,SKShaderStringFactory::ms_WorldPos,OutString);
		OutString += _T(";\n");

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


				SKRenderer::ms_pRenderer->OrenNayarDirectionalLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_ROUGHNESS_SQUARED]->GetNodeName().GetString(),SKShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),UseLookUpTable(),ShadowStringArray[i],OutString);


			}
			else if(i == SKLight::LT_POINT)
			{


				SKRenderer::ms_pRenderer->OrenNayarPointLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_ROUGHNESS_SQUARED]->GetNodeName().GetString(),SKShaderStringFactory::ms_WorldPos,SKShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),UseLookUpTable(),ShadowStringArray[i],OutString);

			}
			else if(i == SKLight::LT_SPOT)
			{

				SKRenderer::ms_pRenderer->OrenNayarSpotLight(iLightNum[i],m_pInput[IN_DIFFUSE_COLOR]->GetNodeName().GetString()
					,m_pInput[IN_ROUGHNESS_SQUARED]->GetNodeName().GetString(),SKShaderStringFactory::ms_WorldPos,SKShaderStringFactory::ms_WorldNormal, _T("WorldCameraDir"),UseLookUpTable(),ShadowStringArray[i],OutString);

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
void SKOrenNayarShaderFunction::SetGlobleValue(unsigned int uiOutPutStringType , SKVShader * pVShader , SKPShader * pPShader)
{
	if (uiOutPutStringType == OST_MATERIAL)
	{
		if (pPShader && UseLookUpTable())
		{
			SKTexAllState * pTex = (SKTexAllState *)SKTexAllState::GetOrenNayarLookUpTable();
			static SKUsedName PSOrenNayarLookUpTableSampler = SKShaderStringFactory::ms_PSOrenNayarLookUpTableSampler;
			pPShader->SetParam(PSOrenNayarLookUpTableSampler,pTex);
			
		}
	}
}
SKOrenNayarShaderFunction::~SKOrenNayarShaderFunction()
{

}
SKOrenNayarShaderFunction::SKOrenNayarShaderFunction()
{

}