#include "SKShaderKey.h"
#include "SKGeometry.h"
#include "SKBoneNode.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKOrenNayarShaderFunction.h"
#include "SKPhoneShaderFunction.h"
#include "SKStraussShaderFunction.h"
#include "SKCookTorranceShaderFunction.h"
#include "SKIsotropicWardShaderFunction.h"
#include "SKStream.h"
#include "SKPointLight.h"
#include "SKDirectionLight.h"
#include "SKSpotLight.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKShaderKey,SKObject)
BEGIN_ADD_PROPERTY(SKShaderKey,SKObject)
REGISTER_PROPERTY(m_KeyMap,KeyMap,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKShaderKey)
IMPLEMENT_INITIAL_END
unsigned int SKShaderKey::GetLightNum(unsigned int uiLightKey,unsigned int uiLightType)
{
	unsigned int uiNum = 0;
	if (uiLightType == SKLight::LT_MAX)
	{
		for (unsigned int i = 0 ; i < SKLight::LT_MAX ; i++)
		{
			uiNum += (uiLightKey >> (i * MAX_LIGTH_TYPE_MASK)) & MAX_LIGHT_TYPE_NUM;
		}
	}
	else
	{
		uiNum += (uiLightKey >> (uiLightType * MAX_LIGTH_TYPE_MASK)) & MAX_LIGHT_TYPE_NUM;
	}
	return uiNum;
}
unsigned int SKShaderKey::GenerateKey(const SKArray<SKLight*> & LightArray,ShadowKeyInfo ShadowInfo[SKLight::LT_MAX],unsigned int &uiLightFunctionKey)
{
	unsigned int uiLightNum[SKLight::LT_MAX] = { 0 };
	unsigned char DirLightFunKey = 0;
	unsigned char PointLightFunKey = 0;
	unsigned char SpotLightFunKey = 0;
	for (unsigned int i = 0 ; i < LightArray.GetNum() ; i++)
	{
		if (LightArray[i])
		{
			for (unsigned int j = 0 ; j < SKLight::LT_MAX ; j++)
			{
				if (LightArray[i]->GetLightType() == j)
				{
					
					uiLightNum[j]++;

					SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(LightArray[i]);
					if (pLocalLight && pLocalLight->HaveLightFun())
					{
						if (j == SKLight::LT_POINT)
						{
							PointLightFunKey |= 1 << uiLightNum[j];
						}
						else if (j == SKLight::LT_DIRECTION)
						{
							DirLightFunKey |= 1 << uiLightNum[j];
						}
						else if (j == SKLight::LT_SPOT)
						{
							SpotLightFunKey |= 1 << uiLightNum[j];
						}
					}
					if (pLocalLight && pLocalLight->GetCastShadow())
					{
						ShadowInfo[j].uiShadowNum++;
						if (j == SKLight::LT_POINT)
						{
							if(((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_VOLUME)
							{
								ShadowInfo[j].uiVolumeShadowNum++;
							}
							else if (((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_DUAL_PARABOLOID)
							{
								ShadowInfo[j].uiDualParaboloidShadowNum++;
							}
							else if (((SKPointLight*)pLocalLight)->GetShadowType() == SKPointLight::ST_PROJECT)
							{
								ShadowInfo[j].uiShadowNum--;
							}
						}
						else if (j == SKLight::LT_DIRECTION)
						{
							if(((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_VOLUME)
							{
								ShadowInfo[j].uiVolumeShadowNum++;
							}
							else if (((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_CSM)
							{
								ShadowInfo[j].uiCSMShadowNum++;
							}
							else if (((SKDirectionLight*)pLocalLight)->GetShadowType() == SKDirectionLight::ST_PROJECT)
							{
								ShadowInfo[j].uiShadowNum--;
							}
						}
						else if (j == SKLight::LT_SPOT)
						{
							if (((SKSpotLight*)pLocalLight)->GetShadowType() == SKSpotLight::ST_PROJECT)
							{
								ShadowInfo[j].uiShadowNum--;
							}
						}
					}
					break;
				}
			}
		}
	}
	unsigned int uiKey = 0;
	for (unsigned int i = 0 ; i < SKLight::LT_MAX ; i++)
	{
		uiKey += uiLightNum[i] << (i * MAX_LIGTH_TYPE_MASK);
	}
	uiLightFunctionKey  = (PointLightFunKey) | (SpotLightFunKey << (1 << MAX_LIGTH_TYPE_MASK)) | (DirLightFunKey << ((1 << MAX_LIGTH_TYPE_MASK) * 2 ));
	return uiKey;
}
void SKShaderKey::SetMaterialGShaderKey(SKShaderKey * pKey, MaterialShaderPara &MSPara, unsigned int uiPassType)
{
	if (!pKey)
	{
		return;
	}
	if (uiPassType == SKPass::PT_MATERIAL || uiPassType == SKPass::PT_NORMALDEPTH
		|| uiPassType == SKPass::PT_POINT_CUBE_SHADOW || uiPassType == SKPass::PT_POINT_VOLUME_SHADOW
		|| uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW || uiPassType == SKPass::PT_SHADOW
		|| uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW || uiPassType == SKPass::PT_INDIRECT)
	{

		unsigned int uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshDate())
			{

				SKVertexFormat *pVertexFormat = SKResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshDate()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(SKUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
		if (uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW)
		{
			pKey->SetTheKey(SKUsedName::ms_cVolumeVertexFormat, 1);
		}
	}
	else if (uiPassType == SKPass::PT_PREZ)
	{
		unsigned uiValue = 0;
		if (MSPara.pGeometry)
		{

			if (MSPara.pGeometry->GetAffectBoneNum() > 0)
			{
				uiValue = 1;
			}

		}
		pKey->SetTheKey(SKUsedName::ms_cPrezBeUsedBone, uiValue);
	}

}
void SKShaderKey::SetMaterialVShaderKey(SKShaderKey * pKey,MaterialShaderPara &MSPara,unsigned int uiPassType)
{
	if (!pKey)
	{
		return;
	}
	if (uiPassType == SKPass::PT_MATERIAL || uiPassType == SKPass::PT_NORMALDEPTH  
		|| uiPassType == SKPass::PT_POINT_CUBE_SHADOW || uiPassType == SKPass::PT_POINT_VOLUME_SHADOW 
		|| uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW|| uiPassType == SKPass::PT_SHADOW
		|| uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW || uiPassType == SKPass::PT_INDIRECT)
	{
	
		unsigned int uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshDate())
			{
				
				SKVertexFormat *pVertexFormat = SKResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshDate()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}
						
			}
			
		}

		pKey->SetTheKey(SKUsedName::ms_cMaterialVertexFormat,uiVertexFormatCode);
		if(uiPassType == SKPass::PT_DIRECT_VOLUME_SHADOW && !SKRenderer::ms_pRenderer->IsSupportGS())
		{
			pKey->SetTheKey(SKUsedName::ms_cVolumeVertexFormat,1);
		}
	}
	else if (uiPassType == SKPass::PT_PREZ)
	{
		unsigned uiValue = 0;
		if (MSPara.pGeometry)
		{

			if (MSPara.pGeometry->GetAffectBoneNum() > 0)
			{
				uiValue = 1;
			}

		}
		pKey->SetTheKey(SKUsedName::ms_cPrezBeUsedBone,uiValue);
	}

}

void SKShaderKey::SetMaterialPShaderKey(SKShaderKey * pKey,MaterialShaderPara & MSPara,unsigned int uiPassType)
{
	if (!pKey)
	{
		return;
	}
	pKey->SetTheKey(SKUsedName::ms_cPassID,MSPara.uiPassId);
	if (uiPassType == SKPass::PT_MATERIAL)
	{
		unsigned int uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshDate())
			{

				SKVertexFormat *pVertexFormat = SKResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshDate()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}
		pKey->SetTheKey(SKUsedName::ms_cMaterialVertexFormat,uiVertexFormatCode);



		
		ShadowKeyInfo ShadowInfo[SKLight::LT_MAX];
		unsigned int uiLightKey = 0;
		unsigned int uiLightFunKey = 0;
		if (MSPara.LightArray.GetNum() > 0)
		{
			uiLightKey = GenerateKey(MSPara.LightArray,ShadowInfo,uiLightFunKey);
		}
		
		pKey->SetTheKey(SKUsedName::ms_cLightFunKey,uiLightFunKey);
		pKey->SetTheKey(SKUsedName::ms_cMaterialLightKey,uiLightKey);
		

		if (ShadowInfo[SKLight::LT_POINT].uiShadowNum > 0)
		{
			pKey->SetTheKey(SKUsedName::ms_cPointLightShadowNum,ShadowInfo[SKLight::LT_POINT].uiShadowNum);
			if (ShadowInfo[SKLight::LT_POINT].uiVolumeShadowNum > 0)
			{
				pKey->SetTheKey(SKUsedName::ms_cPointLightVolumeShadowNum,ShadowInfo[SKLight::LT_POINT].uiVolumeShadowNum);
			}
			else if (ShadowInfo[SKLight::LT_POINT].uiDualParaboloidShadowNum > 0)
			{
				pKey->SetTheKey(SKUsedName::ms_cPointLightParaboloidShadowNum,ShadowInfo[SKLight::LT_POINT].uiDualParaboloidShadowNum);
			}
			else
			{

				if(MSPara.pCamera->GetFov() <= AngleToRadian(90.0f))
				{
					pKey->SetTheKey(SKUsedName::ms_cCubShadowFov,1);
				}

			}
		}
		if (ShadowInfo[SKLight::LT_DIRECTION].uiShadowNum > 0)
		{
			pKey->SetTheKey(SKUsedName::ms_cDirectionLightShadowNum,ShadowInfo[SKLight::LT_DIRECTION].uiShadowNum);
			if (ShadowInfo[SKLight::LT_DIRECTION].uiVolumeShadowNum > 0)
			{
				pKey->SetTheKey(SKUsedName::ms_cDirectionLightVolumeShadowNum,ShadowInfo[SKLight::LT_DIRECTION].uiVolumeShadowNum);
			}
			else if (ShadowInfo[SKLight::LT_DIRECTION].uiCSMShadowNum > 0)
			{
				pKey->SetTheKey(SKUsedName::ms_cDirectionLightCSMShadowNum,ShadowInfo[SKLight::LT_DIRECTION].uiCSMShadowNum);
			}
		}
		if (ShadowInfo[SKLight::LT_SPOT].uiShadowNum > 0)
		{
			pKey->SetTheKey(SKUsedName::ms_cSpotLightShadowNum,ShadowInfo[SKLight::LT_SPOT].uiShadowNum);
		}
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		
		if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_PHONE)
		{
			SKPhoneShaderFunction * pPhoneShaderFunction = (SKPhoneShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
			if (pPhoneShaderFunction->GetSpecularType() == SKPhoneShaderFunction::ST_BlinnPhong)
			{
				pKey->SetTheKey(SKUsedName::ms_cBlinnPhong,1);
			}
		}
		else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_OREN_NAYAR)
		{
			SKOrenNayarShaderFunction * pOrenNayarShaderFunction = (SKOrenNayarShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
			if (pOrenNayarShaderFunction->UseLookUpTable())
			{
				pKey->SetTheKey(SKUsedName::ms_cOrenNayarLookUpTable,1);
			}		
		}
		else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_STRAUSS)
		{
			SKStraussShaderFunction * pStraussShaderFunction = (SKStraussShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
			if (pStraussShaderFunction->UseLookUpTable())
			{
				pKey->SetTheKey(SKUsedName::ms_cStraussLookUpTable,1);
			}
		}
		else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_COOKTOORANCE)
		{
			SKCookTorranceShaderFunction * pCookTorranceShaderFunction = (SKCookTorranceShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
			pKey->SetTheKey(SKUsedName::ms_cCookTorranceSpecluarType,pCookTorranceShaderFunction->GetSpecularType());
	
		}
		else if (pMaterial->GetShaderMainFunction(MSPara.uiPassId)->GetSMType() == SKShaderMainFunction::SM_ISOTROPICWARD)
		{
			SKIsotropicWardShaderFunction * pIsotropicWardShaderFunction = (SKIsotropicWardShaderFunction *)pMaterial->GetShaderMainFunction(MSPara.uiPassId);
			if (pIsotropicWardShaderFunction->UseLookUpTable())
			{
				pKey->SetTheKey(SKUsedName::ms_cIsotropicWarLookUpTable,1);
			}
		}
	}
	else if (uiPassType == SKPass::PT_NORMALDEPTH)
	{
		unsigned int uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshDate())
			{

				SKVertexFormat *pVertexFormat = SKResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshDate()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(SKUsedName::ms_cMaterialVertexFormat,uiVertexFormatCode);
	}
	else if (uiPassType == SKPass::PT_INDIRECT)
	{
		bool bLighted = false;
		SKMeshNode * pMeshNode = MSPara.pGeometry->GetMeshNode();
		if (pMeshNode && pMeshNode->m_bLighted)
		{
			bLighted = true;
		}
		pKey->SetTheKey(SKUsedName::ms_cLighted,bLighted);

		unsigned int uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshDate())
			{

				SKVertexFormat *pVertexFormat = SKResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshDate()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(SKUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
	}
	else if (uiPassType == SKPass::PT_SHADOW || uiPassType == SKPass::PT_POINT_CUBE_SHADOW
		|| uiPassType == SKPass::PT_POINT_CUBE_SHADOW || uiPassType == SKPass::PT_DUAL_PARABOLOID_SHADOW)
	{
		unsigned int uiVertexFormatCode = 0;
		if (MSPara.pGeometry)
		{
			if (MSPara.pGeometry->GetMeshDate())
			{

				SKVertexFormat *pVertexFormat = SKResourceManager::LoadVertexFormat(MSPara.pGeometry->GetMeshDate()->GetVertexBuffer());
				if (pVertexFormat)
				{
					uiVertexFormatCode = pVertexFormat->m_uiVertexFormatCode;
				}

			}

		}

		pKey->SetTheKey(SKUsedName::ms_cMaterialVertexFormat, uiVertexFormatCode);
	}
}

SKShaderKey::SKShaderKey()
{

}
SKShaderKey::~SKShaderKey()
{
	m_KeyMap.Clear();
}
bool SKShaderKey::IsHaveTheKey(const SKUsedName & Name,unsigned int &uiKeyId)
{
	uiKeyId = m_KeyMap.Find(Name);
	if(uiKeyId == m_KeyMap.GetNum())
	{
		return false;
	}

	return true;
}
void SKShaderKey::SetTheKey(const SKUsedName & Name,unsigned int Value)
{
	unsigned int uiKeyId;
	if (IsHaveTheKey(Name,uiKeyId))
	{
		m_KeyMap[uiKeyId].Value = Value;
		return;
	}
	m_KeyMap.AddElement(Name,Value);
}
void SKShaderKey::GetDefine(SKMap<SKString,SKString> & Define)
{
	Define.Clear();
	for (unsigned int i = 0 ; i < m_KeyMap.GetNum() ; i++)
	{
		MapElement<SKUsedName,unsigned int>& Element =  m_KeyMap[i];
		Define.AddElement(Element.Key.GetString(),IntToString(Element.Value));
	}
}
void SKShaderKey::Clear()
{
	m_KeyMap.Clear();
}
void SKShaderKey::operator =(const SKShaderKey &ShaderKey)
{
	m_KeyMap = ShaderKey.m_KeyMap;
}
namespace SKEngine2
{
	bool operator == (const SKShaderKey & Key1,const SKShaderKey & Key2)
	{

		if (Key1.m_KeyMap.GetNum() != Key2.m_KeyMap.GetNum())
		{
			return 0;
		}

		//
		for (unsigned int i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<SKUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<SKUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Key != Element2.Key || Element1.Value != Element2.Value)
			{
				return false;

			}		

		}
		return true;
	}

	bool operator > (const SKShaderKey & Key1,const SKShaderKey & Key2)
	{

		//
		if (Key1.m_KeyMap.GetNum() > Key2.m_KeyMap.GetNum())
		{
			return 1;
		}
		else if(Key1.m_KeyMap.GetNum() < Key2.m_KeyMap.GetNum())
		{
			return 0;
		}

		//ȣkeyֵ
		for (unsigned int i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<SKUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<SKUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Key > Element2.Key)
			{
				return 1;

			}
			else if (Element1.Key < Element2.Key)
			{
				return 0;
			}			
		}

		//valueֵ
		for (unsigned int i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<SKUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<SKUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Value > Element2.Value)
			{
				return 1;

			}
			else if (Element1.Value < Element2.Value)
			{
				return 0;
			}			
		}
		return 0;
	}

	bool operator < (const SKShaderKey & Key1,const SKShaderKey & Key2)
	{

		//
		if (Key1.m_KeyMap.GetNum() > Key2.m_KeyMap.GetNum())
		{
			return 0;
		}
		else if(Key1.m_KeyMap.GetNum() < Key2.m_KeyMap.GetNum())
		{
			return 1;
		}

		//ȣkeyֵ
		for (unsigned int i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<SKUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<SKUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Key > Element2.Key)
			{
				return 0;

			}
			else if (Element1.Key < Element2.Key)
			{
				return 1;
			}			
		}

		//valueֵ
		for (unsigned int i = 0 ; i < Key1.m_KeyMap.GetNum() ;i++)
		{
			MapElement<SKUsedName,unsigned int> & Element1 = Key1.m_KeyMap[i];
			MapElement<SKUsedName,unsigned int> & Element2 = Key2.m_KeyMap[i];
			if (Element1.Value > Element2.Value)
			{
				return 0;

			}
			else if (Element1.Value < Element2.Value)
			{
				return 1;
			}			
		}
		return 0;
	}

}