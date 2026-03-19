#include "SKShaderStringFactory.h"
#include "SKGeometry.h"
#include "SKLog.h"
#include "SKResourceManager.h"
#include "SKBoneNode.h"
#include "SKDirectionLight.h"
#include "SKPointLight.h"
#include "SKSpotLight.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
unsigned int SKShaderStringFactory::ms_ShaderValueIndex = 0;
SKString SKShaderStringFactory::ms_PSTextureInputCoordValue[TEXLEVEL];
SKString SKShaderStringFactory::ms_PSColor[2];
SKString SKShaderStringFactory::ms_PSOutputColorValue;
SKString SKShaderStringFactory::ms_PSOutputColorValue1;
SKString SKShaderStringFactory::ms_PSInputLocalNormal;
SKString SKShaderStringFactory::ms_PSInputLocalBinormal;
SKString SKShaderStringFactory::ms_PSInputLocalTangent;
SKString SKShaderStringFactory::ms_PSInputLocalPos;
SKString SKShaderStringFactory::ms_PSInputProjectZ;
SKString SKShaderStringFactory::ms_PSInputWorldPos;
SKString SKShaderStringFactory::ms_PSInputProjectPos;

SKString SKShaderStringFactory::ms_PSOrenNayarLookUpTableSampler = _T("Tex_OrenNayarLookUpTable");
SKString SKShaderStringFactory::ms_PSStraussFLookUpTableSampler = _T("Tex_StraussFLookUpTable");
SKString SKShaderStringFactory::ms_PSStraussSLookUpTableSampler = _T("Tex_StraussSLookUpTable");
SKString SKShaderStringFactory::ms_PSCookTorranceLookUpTableSampler = _T("Tex_CookTorranceLookUpTable");
SKString SKShaderStringFactory::ms_PSIsotropicWardLookUpTableSampler = _T("Tex_IsotropicWardLookUpTable");
unsigned int SKShaderStringFactory::ms_uiCreateVShaderNum = 0;
unsigned int SKShaderStringFactory::ms_uiCreatePShaderNum = 0;

SKString SKShaderStringFactory::ms_WorldViewProjectMatrix = _T("WorldViewProjectMatrix");
SKString SKShaderStringFactory::ms_ViewProjectMatrix = _T("ViewProjectMatrix");
SKString SKShaderStringFactory::ms_ViewMatrix = _T("ViewMatrix");
SKString SKShaderStringFactory::ms_WorldViewMatrix = _T("WorldViewMatrix");
SKString SKShaderStringFactory::ms_BoneMatrix = _T("BoneVector");
SKString SKShaderStringFactory::ms_WorldMatrix = _T("WorldMatrix");
SKString SKShaderStringFactory::ms_ProjectMatrix = _T("ProjectMatrix");

SKString SKShaderStringFactory::ms_LightName[SKLight::LT_MAX] = {_T("PointLight"),_T("SpotLight"),_T("DirLight")};
SKString SKShaderStringFactory::ms_LightNameTemp[SKLight::LT_MAX] = {_T("PointLightTemp"),_T("SpotLightTemp"),_T("DirLightTemp")};
SKString SKShaderStringFactory::ms_LightColor = _T("LightColor");
SKString SKShaderStringFactory::ms_LightSpecular = _T("LightSpecular");
SKString SKShaderStringFactory::ms_LightDir = _T("LightDir");
SKString SKShaderStringFactory::ms_FarZ = _T("FarZ");
SKString SKShaderStringFactory::ms_PointLightRange =_T("PointLightRange");
SKString SKShaderStringFactory::ms_LightWorldPos = _T("LightWorldPos");
SKString SKShaderStringFactory::ms_LightWorldDirection = _T("LightWorldDirection");
SKString SKShaderStringFactory::ms_WorldNormal = _T("WorldNormal");
SKString SKShaderStringFactory::ms_ViewNormal = _T("ViewNormal");
SKString SKShaderStringFactory::ms_WorldPos = _T("WorldPos");
SKString SKShaderStringFactory::ms_ViewPos = _T("ViewPos");
SKString SKShaderStringFactory::ms_ProjectPos = _T("ProjectPos");
SKString SKShaderStringFactory::ms_ViewWorldDir = _T("ViewWorldDir");
SKString SKShaderStringFactory::ms_CameraWorldPos = _T("CameraWorldPos");

SKString SKShaderStringFactory::ms_PSConstantShadowSampler = _T("Tex_ConstantShadow");
SKString SKShaderStringFactory::ms_PSConstantLightFunSampler = _T("Tex_PSConstantLightFun");
SKString SKShaderStringFactory::ms_InvRTWidth = _T("InvRTWidth");
SKString SKShaderStringFactory::ms_SkyLightUpColor = _T("SkyLightUpColor");
SKString SKShaderStringFactory::ms_SkyLightDownColor = _T("SkyLightDownColor");

SKString SKShaderStringFactory::ms_PSColorBufferSampler = _T("Tex_ColorBufferSampler");
SKString SKShaderStringFactory::ms_PSDepthNormalBufferSampler = _T("Tex_DepthNormalBufferSampler");
void SKShaderStringFactory::ClearAllString()
{
	for(unsigned int i = 0 ; i < TEXLEVEL ; i++)
		ms_PSTextureInputCoordValue[i] = SKRenderer::ms_pRenderer->Float2Const(_T("-1"),_T("-1"));
	ms_PSOutputColorValue.Clear();
	ms_PSOutputColorValue1.Clear();
	ms_PSInputLocalNormal.Clear();
	ms_PSInputLocalBinormal.Clear();
	ms_PSInputLocalTangent.Clear();
	ms_PSInputLocalPos.Clear();

	ms_PSColor[0].Clear();
	ms_PSColor[1].Clear();

	ms_PSInputProjectZ.Clear();
}

bool SKShaderStringFactory::CreateVShaderString(SKVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,SKString & VShaderString
#ifdef _DEBUG
												,SKString &OutShaderFileName)
#else
												)
#endif
{
	if (!MSPara.pGeometry || !pVShader || !SKRenderer::ms_pRenderer || !MSPara.pMaterialInstance)
	{
		return 0;
	}
	SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}
	ClearAllString();
	ms_uiCreateVShaderNum++;

	SKString VInclude;
	SKString VDynamic;
	SKString VInputDeclare;
	SKString VOutputDeclare;
	SKString VUserConstantString;
	SKString VFuntionString;
	SKLog VShaderText;
	SKString VShaderTextName = SKResourceManager::ms_OutputShaderCodePath + SKRenderer::ms_pRenderer->GetRendererStringType() + _T("_");
	SKString ShaderID = IntToString(uiShaderID);
	SKRenderer::ms_pRenderer->GetIncludeShader(VInclude);
	SKRenderer::ms_pRenderer->GetDynamicShader(VDynamic);
	SKRenderer::ms_pRenderer->CreateVUserConstant(pVShader,MSPara,uiPassType,VUserConstantString);
	SKRenderer::ms_pRenderer->CreateVInputDeclare(MSPara,uiPassType,VInputDeclare);
	SKRenderer::ms_pRenderer->CreateVOutputDeclare(MSPara,uiPassType,VOutputDeclare);
	SKRenderer::ms_pRenderer->CreateVFuntion(MSPara,uiPassType,VFuntionString);
	if (uiPassType == SKPass::PT_MATERIAL || uiPassType == SKPass::PT_INDIRECT)
	{			
		
		VShaderTextName += pMaterial->GetShowName().GetString() + ShaderID + _T("VSKShader.txt");
		
	}
	else if (uiPassType == SKPass::PT_NORMALDEPTH)
	{
				
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_NormalDepth") + ShaderID + _T("VSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_PREZ)
	{

		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_PreZ") + ShaderID + _T("VSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_CubeShadow") + ShaderID + _T("VSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_PointVolumeShadow") + ShaderID + _T("VSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_DirectVolumeShadow") + ShaderID + _T("VSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_Shadow") + ShaderID + _T("VSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_DualParaboloidShadow") + ShaderID + _T("VSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION)
	{
		VShaderTextName += pMaterial->GetShowName().GetString() + _T("_LightFun") + ShaderID + _T("VSKShader.txt");
	}

	VShaderText.Open(VShaderTextName.GetBuffer());
	VShaderString = VInclude + VDynamic + VUserConstantString + VInputDeclare + VOutputDeclare + VFuntionString;
	VShaderText.WriteInfo(VShaderString.GetBuffer());
#ifdef _DEBUG
	OutShaderFileName = VShaderTextName;
#endif
	return 1;
}
bool SKShaderStringFactory::CreatePShaderString(SKPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,SKString & PShaderString
#ifdef _DEBUG
												,SKString &OutShaderFileName)
#else
												)
#endif
{
	if (!pPShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !SKRenderer::ms_pRenderer)
	{
		return 0;
	}
	SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();

	ClearAllString();
	ms_uiCreatePShaderNum++;
	SKString PInclude;
	SKString PDynamic;
	SKString PInputDeclare;
	SKString POutputDeclare;
	SKString PUserConstantstring;
	SKString PFuntionString;
	SKLog PShaderText;
	SKString PShaderTextName = SKResourceManager::ms_OutputShaderCodePath + SKRenderer::ms_pRenderer->GetRendererStringType() + _T("_");
	SKString ShaderID = IntToString(uiShaderID);

	

	SKRenderer::ms_pRenderer->GetIncludeShader(PInclude);
	SKRenderer::ms_pRenderer->GetDynamicShader(PDynamic);
	
	SKRenderer::ms_pRenderer->CreatePInputDeclare(MSPara,uiPassType,PInputDeclare);
	SKRenderer::ms_pRenderer->CreatePOutputDeclare(MSPara,uiPassType,POutputDeclare);
	
	//must first before CreatePUserConstant to decide unused shader funtion
	SKRenderer::ms_pRenderer->CreatePFuntion(MSPara,uiPassType,PFuntionString);

	SKRenderer::ms_pRenderer->CreatePUserConstant(pPShader,MSPara,uiPassType,PUserConstantstring);
	if (uiPassType == SKPass::PT_MATERIAL)
	{
		
	
		
		PShaderTextName += pMaterial->GetShowName().GetString() + ShaderID +_T("PSKShader.txt");


		
	}
	else if (uiPassType == SKPass::PT_NORMALDEPTH)
	{

		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_NormalDepth") + ShaderID + _T("PSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_PREZ)
	{


		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_PreZ") + ShaderID + _T("PSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{


		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_CubeShadow") + ShaderID + _T("PSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_PointVolumeShadow") + ShaderID + _T("PSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_DirectVolumeShadow") + ShaderID + _T("PSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_Shadow") + ShaderID + _T("PSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_DualParaboloidShadow") + ShaderID + _T("PSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_LightFun") + ShaderID + _T("PSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_INDIRECT)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_Indirect") + ShaderID + _T("PSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_POSTEFFECT)
	{
		PShaderTextName += pMaterial->GetShowName().GetString() + _T("_PostEffect") + ShaderID + _T("PSKShader.txt");
	}
	PShaderText.Open(PShaderTextName.GetBuffer());
	PShaderString = PInclude + PDynamic + PUserConstantstring + PInputDeclare + POutputDeclare + PFuntionString;
	PShaderText.WriteInfo(PShaderString.GetBuffer());
#ifdef _DEBUG
	OutShaderFileName = PShaderTextName;
#endif
	return 1;
}
bool SKShaderStringFactory::CreateGShaderString(SKGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID, SKString & GShaderString
#ifdef _DEBUG
	, SKString &OutShaderFileName)
#else
	)
#endif
{
	if (!pGShader || !MSPara.pGeometry || !MSPara.pMaterialInstance || !SKRenderer::ms_pRenderer)
	{
		return 0;
	}
	SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();

	ClearAllString();
	ms_uiCreatePShaderNum++;
	SKString GInclude;
	SKString GDynamic;
	SKString GInputDeclare;
	SKString GOutputDeclare;
	SKString GUserConstantstring;
	SKString GFuntionString;
	SKLog GShaderText;
	SKString GShaderTextName = SKResourceManager::ms_OutputShaderCodePath + SKRenderer::ms_pRenderer->GetRendererStringType() + _T("_");
	SKString ShaderID = IntToString(uiShaderID);



	SKRenderer::ms_pRenderer->GetIncludeShader(GInclude);
	SKRenderer::ms_pRenderer->GetDynamicShader(GDynamic);

	SKRenderer::ms_pRenderer->CreateGUserConstant(pGShader, MSPara, uiPassType, GUserConstantstring);
	SKRenderer::ms_pRenderer->CreateGInputDeclare(MSPara, uiPassType, GInputDeclare);
	SKRenderer::ms_pRenderer->CreateGOutputDeclare(MSPara, uiPassType, GOutputDeclare);
	SKRenderer::ms_pRenderer->CreateGFuntion(MSPara, uiPassType, GFuntionString);
	if (uiPassType == SKPass::PT_MATERIAL)
	{



		GShaderTextName += pMaterial->GetShowName().GetString() + ShaderID + _T("GSKShader.txt");



	}
	else if (uiPassType == SKPass::PT_NORMALDEPTH)
	{

		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_NormalDepth") + ShaderID + _T("GSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_PREZ)
	{


		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_PreZ") + ShaderID + _T("GSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{


		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_CubeShadow") + ShaderID + _T("GSKShader.txt");

	}
	else if (uiPassType == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_PointVolumeShadow") + ShaderID + _T("GSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_DirectVolumeShadow") + ShaderID + _T("GSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_Shadow") + ShaderID + _T("GSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_DualParaboloidShadow") + ShaderID + _T("GSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_LightFun") + ShaderID + _T("GSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_INDIRECT)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_Indirect") + ShaderID + _T("GSKShader.txt");
	}
	else if (uiPassType == SKPass::PT_POSTEFFECT)
	{
		GShaderTextName += pMaterial->GetShowName().GetString() + _T("_PostEffect") + ShaderID + _T("GSKShader.txt");
	}
	GShaderText.Open(GShaderTextName.GetBuffer());
	GShaderString = GInclude + GDynamic + GUserConstantstring + GInputDeclare + GOutputDeclare + GFuntionString;
	GShaderText.WriteInfo(GShaderString.GetBuffer());
#ifdef _DEBUG
	OutShaderFileName = GShaderTextName;
#endif
	return 1;
}




