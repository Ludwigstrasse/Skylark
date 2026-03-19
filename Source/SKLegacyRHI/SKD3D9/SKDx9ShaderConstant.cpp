#include "SKDx9Renderer.h"
#include "SK2DTexSampler.h"
#include "SKGeometry.h"
#include "SKString.h"
#include "SKResourceManager.h"
#include "SKShaderStringFactory.h"
#include "SKDirectionLight.h"
#include "SKPointLight.h"
#include "SKSpotLight.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
void SKDX9Renderer::SetUserConstantWorldViewProjectMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKMatrix3X3W WorldViewProjectMat;
	SKTransform World = MSPara.pGeometry->GetWorldTransform();
	WorldViewProjectMat = World.GetCombine()* MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix();
	SKMatrix3X3W  * TempMatrix  = (SKMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = WorldViewProjectMat;
	ID++;
}
void SKDX9Renderer::SetUserLightShadowMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(MSPara.pShadowLight);
	if (pLocalLight)
	{
		SKTransform World = MSPara.pGeometry->GetWorldTransform();
		SKMatrix3X3W  * TempMatrix  = (SKMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
		*TempMatrix = World.GetCombine() * pLocalLight->m_LightShadowMatrix;
		ID++;
	}
	else
	{
		SKMAC_ASSERT(0);
	}
}
void SKDX9Renderer::SetUserConstantWorldViewMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKMatrix3X3W WorldViewMat;

	SKTransform World = MSPara.pGeometry->GetWorldTransform();

	WorldViewMat = World.GetCombine()* MSPara.pCamera->GetViewMatrix();

	SKMatrix3X3W  * TempMatrix  = (SKMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = WorldViewMat;
	ID++;
}
void SKDX9Renderer::SetUserConstantSkin(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKVertexBuffer * pVBuffer = MSPara.pGeometry->GetMeshDate()->GetVertexBuffer();
	if(MSPara.pGeometry->GetAffectBoneNum() && pVBuffer->GetBlendWeightDate() && pVBuffer->GetBlendIndicesDate())
	{
		SKVector3W  * TempVector  = (SKVector3W  *)pShader->m_pUserConstant[ID]->GetDate();
		SKArray<SKVector3W> & Buffer = MSPara.pGeometry->GetSkinWeightBuffer();
		SKMemcpy(TempVector, Buffer.GetBuffer(),Buffer.GetNum() * sizeof(SKVector3W));		
		ID++;
	}
}
void SKDX9Renderer::SetUserConstantLightWorldPos(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKVector3 * pLightWorldPos = (SKVector3 *) pShader->m_pUserConstant[ID]->GetDate();
	*pLightWorldPos= MSPara.pShadowLight->GetWorldTranslate();
	ID++;
}
void SKDX9Renderer::SetUserConstantLightWorldDirection(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKVector3 * pLightWorldDirection = (SKVector3 *) pShader->m_pUserConstant[ID]->GetDate();

	const SKMatrix3X3 &Rotator = MSPara.pShadowLight->GetWorldRotate();
	SKVector3 U,V,N;
	Rotator.GetUVN(U,V,N);

	*pLightWorldDirection= N;
	ID++;
}
void SKDX9Renderer::SetMaterialVShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,SKVShader *pVShader)
{
	unsigned int ID = 0;

	if (uiPassType == SKPass::PT_MATERIAL 
		|| uiPassType == SKPass::PT_PREZ 
		|| uiPassType == SKPass::PT_NORMALDEPTH 
		|| uiPassType == SKPass::PT_INDIRECT)
	{	
		SetUserConstantWorldViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	else if (uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{

		SetUserConstantWorldViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	else if (uiPassType == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pVShader,ID);
		SetUserConstantViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantLightWorldPos(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);

	}
	else if (uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pVShader,ID);
		SetUserConstantViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantLightWorldDirection(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);

	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		SetUserLightShadowMatrix(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	else if (uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		SetUserConstantWorldViewProjectMatrix(MSPara,pVShader,ID);
		SetUserConstantPointLightRange(MSPara,pVShader,ID);
		SetUserConstantSkin(MSPara,pVShader,ID);
	}
	if (uiPassType == SKPass::PT_NORMALDEPTH)
	{
		SetUserConstantWorldViewMatrix(MSPara,pVShader,ID);
		
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION)
	{
		SetUserConstantInvRTWidth(MSPara,pVShader,ID);
	}

}
void SKDX9Renderer::SetUserConstantWorldMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKTransform World = MSPara.pGeometry->GetWorldTransform();
	SKMatrix3X3W  *TempMatrix  = (SKMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = World.GetCombine();
	ID++;
}
void SKDX9Renderer::SetUserConstantCameraPos(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKVector3 * pCameraPos = (SKVector3 *) pShader->m_pUserConstant[ID]->GetDate();
	*pCameraPos= MSPara.pCamera->GetWorldTranslate();
	ID++;
}
void SKDX9Renderer::SetUserConstantSkyUpColor(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKColorRGBA * pColor = (SKColorRGBA *) pShader->m_pUserConstant[ID]->GetDate();
	*pColor= MSPara.m_SkyLightUpColor;
	ID++;
}
void SKDX9Renderer::SetUserConstantSkyDownColor(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKColorRGBA * pColor = (SKColorRGBA *) pShader->m_pUserConstant[ID]->GetDate();
	*pColor= MSPara.m_SkyLightDownColor;
	ID++;	
}
void SKDX9Renderer::SetUserConstantLight(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKArray<SKLight*> & LightArray = MSPara.LightArray;

	int iLightNum[SKLight::LT_MAX] = { 0 };	
	SKVector3W * pLightBuffer[SKLight::LT_MAX] = { NULL };
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

	for(unsigned int i = 0 ; i < SKLight::LT_MAX ; i++)
	{
		if (!iLightNum[i])
		{
			continue;
		}
		pLightBuffer[i] = (SKVector3W *) pShader->m_pUserConstant[ID]->GetDate();
		ID++;
	}
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			SKLight* pLight = LightArray[i];
			if (LightArray[i]->GetLightType() == SKLight::LT_DIRECTION)
			{

				*pLightBuffer[SKLight::LT_DIRECTION] = ((SKDirectionLight *)pLight)->m_Diffuse;
				pLightBuffer[SKLight::LT_DIRECTION]++;
				*pLightBuffer[SKLight::LT_DIRECTION] = ((SKDirectionLight *)pLight)->m_Specular;
				pLightBuffer[SKLight::LT_DIRECTION]++;
				const SKMatrix3X3 &Rotator = ((SKDirectionLight *)pLight)->GetWorldRotate();
				SKVector3 U,V,N;
				Rotator.GetUVN(U,V,N);

				pLightBuffer[SKLight::LT_DIRECTION]->x = N.x;
				pLightBuffer[SKLight::LT_DIRECTION]->y = N.y;
				pLightBuffer[SKLight::LT_DIRECTION]->z = N.z;
				pLightBuffer[SKLight::LT_DIRECTION]->w = ((SKDirectionLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[SKLight::LT_DIRECTION]++;

				pLightBuffer[SKLight::LT_DIRECTION]->x = ((SKDirectionLight *)pLight)->m_ZBias;

				pLightBuffer[SKLight::LT_DIRECTION]++;

				pLightBuffer[SKLight::LT_DIRECTION]->x = ((SKDirectionLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[SKLight::LT_DIRECTION]->y = ((SKDirectionLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[SKLight::LT_DIRECTION]->z = ((SKDirectionLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[SKLight::LT_DIRECTION]->w = ((SKDirectionLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[SKLight::LT_DIRECTION]++;

				SKMatrix3X3W MVP = ((SKLocalLight *)pLight)->GetMVP();
				SKVector3W MVPRow[4];
				MVP.GetRowVector(MVPRow[0],MVPRow[1],MVPRow[2],MVPRow[3]);
				*pLightBuffer[SKLight::LT_DIRECTION] = MVPRow[0];
				pLightBuffer[SKLight::LT_DIRECTION]++;
				*pLightBuffer[SKLight::LT_DIRECTION] = MVPRow[1];
				pLightBuffer[SKLight::LT_DIRECTION]++;
				*pLightBuffer[SKLight::LT_DIRECTION] = MVPRow[2];
				pLightBuffer[SKLight::LT_DIRECTION]++;
				*pLightBuffer[SKLight::LT_DIRECTION] = MVPRow[3];
				pLightBuffer[SKLight::LT_DIRECTION]++;
				if (((SKDirectionLight *)pLight)->GetShadowType() == SKDirectionLight::ST_VOLUME)
				{
					unsigned int uiRTWidth = m_uiCurRTWidth;
					unsigned int uiRTHeight = m_uiCurRTHeight;
					SKREAL fOffsetX = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
					SKREAL fOffsetY = 0.5f + (0.5f / (SKREAL)(uiRTHeight));
					SKMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					SKMatrix3X3W Mat = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix() * texScaleBiasMat;

					SKVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[0];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[1];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[2];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[3];
					pLightBuffer[SKLight::LT_DIRECTION]++;

					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;

					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
				}
				else if (((SKDirectionLight *)pLight)->GetShadowType() == SKDirectionLight::ST_CSM)
				{
					//shadow
					unsigned int uiRTWidth = ((SKDirectionLight *)pLight)->GetShadowResolution();

					SKREAL fOffsetX = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
					SKREAL fOffsetY = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
					SKMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					SKMatrix3X3W Mat = ((SKDirectionLight *)pLight)->m_CSMLightShadowMatrix[0] * texScaleBiasMat;

					SKVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[0];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[1];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[2];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[3];
					pLightBuffer[SKLight::LT_DIRECTION]++;

					Mat = ((SKDirectionLight *)pLight)->m_CSMLightShadowMatrix[1] * texScaleBiasMat;

					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[0];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[1];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[2];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[3];
					pLightBuffer[SKLight::LT_DIRECTION]++;

					Mat = ((SKDirectionLight *)pLight)->m_CSMLightShadowMatrix[2] * texScaleBiasMat;

					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[0];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[1];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[2];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[3];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					
				}
				else
				{
					//shadow
					unsigned int uiRTWidth = ((SKDirectionLight *)pLight)->GetShadowResolution();

					SKREAL fOffsetX = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
					SKREAL fOffsetY = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
					SKMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
						0.0f,    -0.5f,     0.0f,       0.0f,
						0.0f,     0.0f,     1.0f,		0.0f,
						fOffsetX, fOffsetY, 0.0f,       1.0f );
					SKMatrix3X3W Mat = ((SKDirectionLight *)pLight)->m_LightShadowMatrix * texScaleBiasMat;

					SKVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[0];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[1];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[2];
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = Row[3];
					pLightBuffer[SKLight::LT_DIRECTION]++;

					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;

					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;
					*pLightBuffer[SKLight::LT_DIRECTION] = SKVector3W();
					pLightBuffer[SKLight::LT_DIRECTION]++;	
				}
			}
			else if (LightArray[i]->GetLightType() == SKLight::LT_POINT)
			{
				*pLightBuffer[SKLight::LT_POINT] = ((SKPointLight *)pLight)->m_Diffuse;
				pLightBuffer[SKLight::LT_POINT]++;
				*pLightBuffer[SKLight::LT_POINT] = ((SKPointLight *)pLight)->m_Specular;
				pLightBuffer[SKLight::LT_POINT]++;
				pLightBuffer[SKLight::LT_POINT]->x = ((SKPointLight *)pLight)->GetWorldTranslate().x;
				pLightBuffer[SKLight::LT_POINT]->y = ((SKPointLight *)pLight)->GetWorldTranslate().y;
				pLightBuffer[SKLight::LT_POINT]->z = ((SKPointLight *)pLight)->GetWorldTranslate().z;
				pLightBuffer[SKLight::LT_POINT]->w = ((SKPointLight *)pLight)->GetRange();
				pLightBuffer[SKLight::LT_POINT]++;
				pLightBuffer[SKLight::LT_POINT]->x = ((SKPointLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[SKLight::LT_POINT]->y = ((SKPointLight *)pLight)->m_ZBias;
				pLightBuffer[SKLight::LT_POINT]++;

				pLightBuffer[SKLight::LT_POINT]->x = ((SKPointLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[SKLight::LT_POINT]->y = ((SKPointLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[SKLight::LT_POINT]->z = ((SKPointLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[SKLight::LT_POINT]->w = ((SKPointLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[SKLight::LT_POINT]++;

				SKMatrix3X3W MVP = ((SKLocalLight *)pLight)->GetMVP();
				SKVector3W MVPRow[4];
				MVP.GetRowVector(MVPRow[0],MVPRow[1],MVPRow[2],MVPRow[3]);
				*pLightBuffer[SKLight::LT_POINT] = MVPRow[0];
				pLightBuffer[SKLight::LT_POINT]++;
				*pLightBuffer[SKLight::LT_POINT] = MVPRow[1];
				pLightBuffer[SKLight::LT_POINT]++;
				*pLightBuffer[SKLight::LT_POINT] = MVPRow[2];
				pLightBuffer[SKLight::LT_POINT]++;
				*pLightBuffer[SKLight::LT_POINT] = MVPRow[3];
				pLightBuffer[SKLight::LT_POINT]++;

				if (((SKPointLight *)pLight)->GetShadowType() == SKPointLight::ST_VOLUME)
				{

					unsigned int uiRTWidth = m_uiCurRTWidth;
					unsigned int uiRTHeight = m_uiCurRTHeight;
					SKREAL fOffsetX = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
					SKREAL fOffsetY = 0.5f + (0.5f / (SKREAL)(uiRTHeight));
					SKMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
													0.0f,    -0.5f,     0.0f,       0.0f,
													0.0f,     0.0f,     1.0f,		0.0f,
													fOffsetX, fOffsetY, 0.0f,       1.0f );
					SKMatrix3X3W Mat = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix() * texScaleBiasMat;

					SKVector3W Row[4];
					Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
					*pLightBuffer[SKLight::LT_POINT] = Row[0];
					pLightBuffer[SKLight::LT_POINT]++;
					*pLightBuffer[SKLight::LT_POINT] = Row[1];
					pLightBuffer[SKLight::LT_POINT]++;
					*pLightBuffer[SKLight::LT_POINT] = Row[2];
					pLightBuffer[SKLight::LT_POINT]++;
					*pLightBuffer[SKLight::LT_POINT] = Row[3];
					pLightBuffer[SKLight::LT_POINT]++;


				}
				else
				{
					//shadow
					*pLightBuffer[SKLight::LT_POINT] = SKVector3W();
					pLightBuffer[SKLight::LT_POINT]++;
					*pLightBuffer[SKLight::LT_POINT] = SKVector3W();
					pLightBuffer[SKLight::LT_POINT]++;
					*pLightBuffer[SKLight::LT_POINT] = SKVector3W();
					pLightBuffer[SKLight::LT_POINT]++;
					*pLightBuffer[SKLight::LT_POINT] = SKVector3W();
					pLightBuffer[SKLight::LT_POINT]++;
				}
				
			}
			else if (LightArray[i]->GetLightType() == SKLight::LT_SPOT)
			{
				*pLightBuffer[SKLight::LT_SPOT] = ((SKSpotLight *)pLight)->m_Diffuse;
				pLightBuffer[SKLight::LT_SPOT]++;
				*pLightBuffer[SKLight::LT_SPOT] = ((SKSpotLight *)pLight)->m_Specular;
				pLightBuffer[SKLight::LT_SPOT]++;
				pLightBuffer[SKLight::LT_SPOT]->x = ((SKSpotLight *)pLight)->GetWorldTranslate().x;
				pLightBuffer[SKLight::LT_SPOT]->y = ((SKSpotLight *)pLight)->GetWorldTranslate().y;
				pLightBuffer[SKLight::LT_SPOT]->z = ((SKSpotLight *)pLight)->GetWorldTranslate().z;
				pLightBuffer[SKLight::LT_SPOT]->w = ((SKSpotLight *)pLight)->GetRange();
				pLightBuffer[SKLight::LT_SPOT]++;
				const SKMatrix3X3 &Rotator = ((SKSpotLight *)pLight)->GetWorldRotate();
				SKVector3 U,V,N;
				Rotator.GetUVN(U,V,N);
				pLightBuffer[SKLight::LT_SPOT]->x = N.x;
				pLightBuffer[SKLight::LT_SPOT]->y = N.y;
				pLightBuffer[SKLight::LT_SPOT]->z = N.z;
				pLightBuffer[SKLight::LT_SPOT]->w = ((SKDirectionLight *)pLight)->GetShadowResolution() * 1.0f;
				pLightBuffer[SKLight::LT_SPOT]++;

				pLightBuffer[SKLight::LT_SPOT]->x = ((SKSpotLight *)pLight)->GetFalloff();
				pLightBuffer[SKLight::LT_SPOT]->y = ((SKSpotLight *)pLight)->GetTheta();
				pLightBuffer[SKLight::LT_SPOT]->z = ((SKSpotLight *)pLight)->GetPhi();
				pLightBuffer[SKLight::LT_SPOT]->w = ((SKSpotLight *)pLight)->m_ZBias;
				pLightBuffer[SKLight::LT_SPOT]++;

				pLightBuffer[SKLight::LT_SPOT]->x = ((SKSpotLight *)pLight)->m_LightFunScale.x;
				pLightBuffer[SKLight::LT_SPOT]->y = ((SKSpotLight *)pLight)->m_LightFunScale.y;
				pLightBuffer[SKLight::LT_SPOT]->z = ((SKSpotLight *)pLight)->m_LightFunOffset.x;
				pLightBuffer[SKLight::LT_SPOT]->w = ((SKSpotLight *)pLight)->m_LightFunOffset.y;
				pLightBuffer[SKLight::LT_SPOT]++;

				SKMatrix3X3W MVP = ((SKLocalLight *)pLight)->GetMVP();
				SKVector3W MVPRow[4];
				MVP.GetRowVector(MVPRow[0],MVPRow[1],MVPRow[2],MVPRow[3]);
				*pLightBuffer[SKLight::LT_SPOT] = MVPRow[0];
				pLightBuffer[SKLight::LT_SPOT]++;
				*pLightBuffer[SKLight::LT_SPOT] = MVPRow[1];
				pLightBuffer[SKLight::LT_SPOT]++;
				*pLightBuffer[SKLight::LT_SPOT] = MVPRow[2];
				pLightBuffer[SKLight::LT_SPOT]++;
				*pLightBuffer[SKLight::LT_SPOT] = MVPRow[3];
				pLightBuffer[SKLight::LT_SPOT]++;


				//shadow
				unsigned int uiRTWidth = ((SKSpotLight *)pLight)->GetShadowResolution();

				SKREAL fOffsetX = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
				SKREAL fOffsetY = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
				SKMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
					0.0f,    -0.5f,     0.0f,       0.0f,
					0.0f,     0.0f,     1.0f,		0.0f,
					fOffsetX, fOffsetY, 0.0f,       1.0f );
				SKMatrix3X3W Mat = ((SKSpotLight *)pLight)->m_LightShadowMatrix * texScaleBiasMat;

				SKVector3W Row[4];
				Mat.GetRowVector(Row[0],Row[1],Row[2],Row[3]);
				*pLightBuffer[SKLight::LT_SPOT] = Row[0];
				pLightBuffer[SKLight::LT_SPOT]++;
				*pLightBuffer[SKLight::LT_SPOT] = Row[1];
				pLightBuffer[SKLight::LT_SPOT]++;
				*pLightBuffer[SKLight::LT_SPOT] = Row[2];
				pLightBuffer[SKLight::LT_SPOT]++;
				*pLightBuffer[SKLight::LT_SPOT] = Row[3];
				pLightBuffer[SKLight::LT_SPOT]++;

			}
		}
	}
}
void SKDX9Renderer::SetUserConstantLightFuntionSampler(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKArray<SKLight*> & LightArray = MSPara.LightArray;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->HaveLightFun())
		{

			pShader->m_pUserSampler[ID]->SetTex(pLocalLight->GetLightFunDiffuseTexture(),0);
			ID++;

		}
	}
}
void SKDX9Renderer::SetUserConstantShadowSampler(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKArray<SKLight*> & LightArray = MSPara.LightArray;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(LightArray[i]);
		if (pLocalLight && pLocalLight->GetCastShadow())
		{
			for (unsigned int j = 0 ; j < pLocalLight->GetShadowTextureNum() ; j++)
			{
				pShader->m_pUserSampler[ID]->SetTex(pLocalLight->GetShadowTexture(j),j);
				
			}
			ID++;
			
		}
	}
}
void SKDX9Renderer::SetUserConstantFarZ(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKREAL  * Temp  = (SKREAL *)pShader->m_pUserConstant[ID]->GetDate();
	*Temp = MSPara.pCamera->GetZFar();	
	ID++;
}
void SKDX9Renderer::SetUserConstantPointLightRange(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKREAL  * Temp  = (SKREAL *)pShader->m_pUserConstant[ID]->GetDate();
	SKPointLight * pPointLight = DynamicCast<SKPointLight>(MSPara.pShadowLight);
	SKMAC_ASSERT(pPointLight);
	*Temp = pPointLight->GetRange();
	ID++;
}
void SKDX9Renderer::SetUserConstantInvRTWidth(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKREAL  * Temp  = (SKREAL *)pShader->m_pUserConstant[ID]->GetDate();
	SKLocalLight * pLight = DynamicCast<SKLocalLight>(MSPara.pShadowLight);
	SKMAC_ASSERT(pLight);
	*Temp = 1.0f / pLight->GetLightMaterialRtWidth();	
	ID++;
}
void SKDX9Renderer::SetUserConstantProjectMatrix(MaterialShaderPara &MSPara, SKShader * pShader, unsigned int& ID)
{
	SKMatrix3X3W  *TempMatrix = (SKMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = MSPara.pCamera->GetProjMatrix();
	ID++;
}
void SKDX9Renderer::SetUserConstantViewMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKMatrix3X3W  *TempMatrix  = (SKMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = MSPara.pCamera->GetViewMatrix();
	ID++;
}
void SKDX9Renderer::SetUserConstantViewProjectMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID)
{
	SKMatrix3X3W  *TempMatrix  = (SKMatrix3X3W  *)pShader->m_pUserConstant[ID]->GetDate();
	*TempMatrix = MSPara.pCamera->GetViewMatrix() * MSPara.pCamera->GetProjMatrix();
	ID++;
}
void SKDX9Renderer::SetMaterialPShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,SKPShader *pPShader)
{
	unsigned int ID = 0;
	if (uiPassType == SKPass::PT_MATERIAL)
	{

		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantViewMatrix(MSPara,pPShader,ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);
		SetUserConstantFarZ(MSPara,pPShader,ID);
		if (MSPara.LightArray.GetNum() > 0)
		{
			SetUserConstantLight(MSPara,pPShader,ID);	
		}
		
		unsigned int uiTexSamplerID = 0;
		SetUserConstantShadowSampler(MSPara,pPShader,uiTexSamplerID);	
		SetUserConstantLightFuntionSampler(MSPara,pPShader,uiTexSamplerID);
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);

	}
	else if (uiPassType == SKPass::PT_INDIRECT)
	{
		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantViewMatrix(MSPara,pPShader,ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);
		SetUserConstantSkyUpColor(MSPara,pPShader,ID);
		SetUserConstantSkyDownColor(MSPara,pPShader,ID);

		unsigned int uiTexSamplerID = 0;
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == SKPass::PT_NORMALDEPTH)
	{
		SetUserConstantWorldMatrix(MSPara, pPShader, ID);
		SetUserConstantViewMatrix(MSPara, pPShader, ID);
		SetUserConstantProjectMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara, pPShader, ID);
		SetUserConstantFarZ(MSPara,pPShader,ID);
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == SKPass::PT_POINT_CUBE_SHADOW)
	{
		SetUserConstantWorldMatrix(MSPara,pPShader,ID);
		SetUserConstantCameraPos(MSPara,pPShader,ID);

		SetUserConstantPointLightRange(MSPara,pPShader,ID);
		
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == SKPass::PT_PREZ 
		|| uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW
		|| uiPassType == SKPass::PT_POINT_VOLUME_SHADOW)
	{
		
	}
	else if (uiPassType == SKPass::PT_SHADOW)
	{
		SetUserLightShadowMatrix(MSPara, pPShader, ID);
		SetUserConstantWorldMatrix(MSPara, pPShader, ID);
		SetUserConstantCameraPos(MSPara, pPShader, ID);
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	else if (uiPassType == SKPass::PT_LIGHT_FUNCTION
		|| uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW 
		|| uiPassType == SKPass::PT_POSTEFFECT)
	{
		MSPara.pMaterialInstance->SetPShaderValue(pPShader);
	}
	
}
void SKDX9Renderer::SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKGShader *pGShader)
{

}