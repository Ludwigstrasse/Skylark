#ifndef SKSHADERCREATEFACTORY_H
#define SKSHADERCREATEFACTORY_H
#include "SKMaterial.h"
#include "SKVShader.h"
#include "SKPShader.h"
#include "SKGShader.h"
#include "SKDShader.h"
#include "SKHShader.h"
#include "SKLight.h"
#include "SKResourceManager.h"
namespace SKEngine2
{
#define ms_LightShadowMatrix ms_WorldViewProjectMatrix
	class SKGRAPHIC_API SKShaderStringFactory
	{
	public:	

		static SKString ms_PSTextureInputCoordValue[TEXLEVEL];
		static SKString ms_PSOutputColorValue;
		static SKString ms_PSOutputColorValue1;
		static SKString ms_PSColor[2];
		static SKString ms_PSInputProjectZ;
		static SKString ms_PSInputLocalNormal;
		static SKString ms_PSInputLocalBinormal;
		static SKString ms_PSInputLocalTangent;
		static SKString ms_PSInputLocalPos;
		static SKString ms_PSInputWorldPos;
		static SKString ms_PSInputProjectPos;

		static SKString ms_PSConstantShadowSampler;
		static SKString ms_PSConstantLightFunSampler;

		static SKString ms_PSOrenNayarLookUpTableSampler;
		static SKString ms_PSStraussFLookUpTableSampler;
		static SKString ms_PSStraussSLookUpTableSampler;
		static SKString ms_PSCookTorranceLookUpTableSampler;
		static SKString ms_PSIsotropicWardLookUpTableSampler;

		static SKString ms_PSColorBufferSampler;
		static SKString ms_PSDepthNormalBufferSampler;

		static SKString ms_WorldViewProjectMatrix;
		static SKString ms_WorldViewMatrix;
		static SKString ms_ViewProjectMatrix;
		static SKString ms_ViewMatrix;
		static SKString ms_BoneMatrix;
		static SKString ms_WorldMatrix;
		static SKString ms_ProjectMatrix;



		static SKString ms_LightWorldPos;
		static SKString ms_LightWorldDirection;

		static SKString ms_WorldNormal;
		static SKString ms_ViewNormal;
		static SKString ms_WorldPos;
		static SKString ms_ViewPos;
		static SKString ms_ProjectPos;

		static SKString ms_ViewWorldDir;
		static SKString ms_CameraWorldPos;

		static SKString ms_LightColor;
		static SKString ms_LightSpecular;
		static SKString ms_LightDir;
		static SKString ms_LightName[SKLight::LT_MAX];
		static SKString ms_LightNameTemp[SKLight::LT_MAX];

		static SKString ms_FarZ;
		static SKString ms_PointLightRange;

		static SKString ms_InvRTWidth;

		static SKString ms_SkyLightUpColor;
		static SKString ms_SkyLightDownColor;

		static unsigned int ms_uiCreateVShaderNum;
		static unsigned int ms_uiCreatePShaderNum;

		static unsigned int ms_ShaderValueIndex;

		
		//Create Shader


		static bool CreateVShaderString(SKVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,SKString & VShaderString
#ifdef _DEBUG
			,SKString &OutShaderFileName);
#else
			);
#endif
		static bool CreatePShaderString(SKPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID,SKString & PShaderString
#ifdef _DEBUG
			,SKString &OutShaderFileName);
#else
			);
#endif
		
		static bool CreateGShaderString(SKGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID, SKString & GShaderString
#ifdef _DEBUG
			, SKString &OutShaderFileName);
#else
			);
#endif
	protected:

 		static void ClearAllString();


		
	};

}
#endif