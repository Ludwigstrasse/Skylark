#include "SKMaterial.h"
#include "SKTexSampler.h"
#include "SK2DTexSampler.h"
#include "SKSpatial.h"
#include "SKShaderStringFactory.h"
#include "SKConstFloatValue.h"
#include "SKBoneNode.h"
#include "SKResourceManager.h"
#include "SKVertexColor.h"
#include "SKGraphicInclude.h"
#include "SKPhoneShaderFunction.h"
#include "SKOrenNayarShaderFunction.h"
#include "SKStream.h"
#include "SKCustomShaderFunction.h"
#include "SKLightShaderFunction.h"
#include "SKPosShaderFunction.h"
#include "SKCameraShaderFunction.h"
#include "SKNormalFunction.h"
#include "SKArithmeticShaderFunction.h"
#include "SKMinnaertShaderFunction.h"
#include "SKShirleyShaderFunction.h"
#include "SKIsotropicWardShaderFunction.h"
#include "SKAnisotropicWardShaderFunction.h"
#include "SKSchlickShaderFunction.h"
#include "SKCookTorranceShaderFunction.h"
#include "SKStraussShaderFunction.h"
#include "SKColorBuffer.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(MaterialCustomPara,MaterialCustomPara)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(MaterialCustomPara)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(MaterialCustomPara,SKObject)
REGISTER_PROPERTY(ConstValueName,ConstValueName,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY

MaterialCustomPara::MaterialCustomPara()
{

}
MaterialCustomPara::~MaterialCustomPara()
{

}
IMPLEMENT_RTTI(CustomTexSampler,MaterialCustomPara)
BEGIN_ADD_PROPERTY(CustomTexSampler,MaterialCustomPara)
REGISTER_PROPERTY(m_pTexture,Texture,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(CustomTexSampler)
IMPLEMENT_INITIAL_END
CustomTexSampler::CustomTexSampler()
{
	m_pTexture = NULL;
}
CustomTexSampler::~CustomTexSampler()
{
	m_pTexture = NULL;
}
IMPLEMENT_RTTI(CustomFloatValue,MaterialCustomPara)
BEGIN_ADD_PROPERTY(CustomFloatValue,MaterialCustomPara)
REGISTER_PROPERTY(Value,Value,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(CustomFloatValue)
IMPLEMENT_INITIAL_END
CustomFloatValue::CustomFloatValue()
{

}
CustomFloatValue::~CustomFloatValue()
{

}

IMPLEMENT_RTTI_NoCreateFun(SKMaterialInterface,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKMaterialInterface)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKMaterialInterface,SKObject)
END_ADD_PROPERTY
SKMaterialInterface::SKMaterialInterface()
{
	
}
SKMaterialInterface::~SKMaterialInterface()
{
	
	
}



IMPLEMENT_RTTI(SKCustomMaterial,SKMaterialInterface)
BEGIN_ADD_PROPERTY(SKCustomMaterial,SKMaterialInterface)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCustomMaterial)
ADD_PRIORITY(SKResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pPostGray;
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pGammaCorrect;
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pPostScreenQuad;
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pPostVolumeShadowMap;
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pProjectShadowPre;
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pProjectShadow;
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pSSR;
SKPointer<SKCustomMaterial> SKCustomMaterial::ms_pFont;
SKShaderKeyPtr SKCustomMaterial::ms_NULLShaderKey;
bool SKCustomMaterial::InitialDefaultState()
{
	ms_pPostGray = SK_NEW SKCustomMaterial();
	ms_pGammaCorrect = SK_NEW SKCustomMaterial();
	ms_pPostScreenQuad = SK_NEW SKCustomMaterial();
	ms_pPostVolumeShadowMap = SK_NEW SKCustomMaterial();

	ms_pProjectShadowPre = SK_NEW SKCustomMaterial();
	ms_pProjectShadow = SK_NEW SKCustomMaterial();
	ms_pSSR = SK_NEW SKCustomMaterial();
	ms_pFont = SK_NEW SKCustomMaterial();
	ms_NULLShaderKey = SK_NEW SKShaderKey();
	{
		SKDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = SKDepthStencilDesc::CM_LESSEQUAL;
		DepthStencilDesc.m_uiReference = 0x10;
		SKDepthStencilState * pDepthStencilState = SKResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pPostVolumeShadowMap->m_RenderState.SetDepthStencilState(pDepthStencilState);
	}

	{
		SKDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthWritable = false;
		DepthStencilDesc.m_uiDepthCompareMethod = SKDepthStencilDesc::CM_LESS;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_bTwoSideStencilMode = true;
		DepthStencilDesc.m_uiSPassZFailOP = SKDepthStencilDesc::OT_DECREMENT;
		DepthStencilDesc.m_uiCCW_SPassZFailOP = SKDepthStencilDesc::OT_INCREMENT;
		SKDepthStencilState * pDepthStencilState = SKResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pProjectShadowPre->m_RenderState.SetDepthStencilState(pDepthStencilState);

		SKRasterizerDesc RasterizerDesc;
		RasterizerDesc.m_uiCullType = SKRasterizerDesc::CT_NONE;
		SKRasterizerState * pRasterizerState= SKResourceManager::CreateRasterizerState(RasterizerDesc);
		ms_pProjectShadowPre->m_RenderState.SetRasterizerState(pRasterizerState);

		SKBlendDesc BlendDesc;
		BlendDesc.ucWriteMask[0] = SKBlendDesc::WM_NONE;
		SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
		ms_pProjectShadowPre->m_RenderState.SetBlendState(pBlendState);
	}


	{
		SKDepthStencilDesc DepthStencilDesc;
		DepthStencilDesc.m_bDepthEnable = false;
		DepthStencilDesc.m_bStencilEnable = true;
		DepthStencilDesc.m_uiStencilCompareMethod = SKDepthStencilDesc::CM_LESSEQUAL;
		DepthStencilDesc.m_uiReference = 0x10;
		SKDepthStencilState * pDepthStencilState = SKResourceManager::CreateDepthStencilState(DepthStencilDesc);
		ms_pProjectShadow->m_RenderState.SetDepthStencilState(pDepthStencilState);

		SKRasterizerDesc RasterizerDesc;
		RasterizerDesc.m_uiCullType = SKRasterizerDesc::CT_CW;
		SKRasterizerState * pRasterizerState= SKResourceManager::CreateRasterizerState(RasterizerDesc);
		ms_pProjectShadow->m_RenderState.SetRasterizerState(pRasterizerState);

		SKBlendDesc BlendDesc;
		BlendDesc.ucSrcBlend[0] = SKBlendDesc::BP_DESTCOLOR;
		BlendDesc.ucDestBlend[0] = SKBlendDesc::BP_ZERO;

		BlendDesc.bBlendEnable[0] = true;		
		SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
		ms_pProjectShadow->m_RenderState.SetBlendState(pBlendState);
	}
	LoadDefault();
	return 1;
}
bool SKCustomMaterial::TerminalDefaultState()
{
	ms_pPostGray = NULL;
	ms_pGammaCorrect = NULL;
	ms_pPostScreenQuad = NULL;
	ms_pPostVolumeShadowMap = NULL;
	ms_pProjectShadowPre = NULL;
	ms_pProjectShadow = NULL;
	ms_pSSR = NULL;
	ms_NULLShaderKey = NULL;
	ms_pFont = NULL;
	return 1;
}
void SKCustomMaterial::LoadAllDeviceShader()
{
	ms_pPostGray->LoadDeviceShader();
	ms_pGammaCorrect->LoadDeviceShader();
	ms_pPostScreenQuad->LoadDeviceShader();
	ms_pPostVolumeShadowMap->LoadDeviceShader();
	ms_pProjectShadowPre->LoadDeviceShader();
	ms_pProjectShadow->LoadDeviceShader();
	ms_pSSR->LoadDeviceShader();
	ms_pFont->LoadDeviceShader();
}
void SKCustomMaterial::LoadDeviceShader()
{
	GetCurVShader(*ms_NULLShaderKey);
	GetCurPShader(*ms_NULLShaderKey);
}
bool SKCustomMaterial::LoadDefault()
{
	if (!ms_pPostGray)
	{
		return 0;
	}

	ms_pPostGray->PreLoad(SKUsedName::ms_cPostGray,_T("SKPostEffectVSKShader.txt"),_T("SKMain"),_T("SKGrayPSKShader.txt"),_T("PSMain"));

	if(!ms_pGammaCorrect)
	{
		return 0;
	}

	ms_pGammaCorrect->PreLoad(SKUsedName::ms_cGammaCorrect,_T("SKPostEffectVSKShader.txt"),_T("SKMain"),_T("SKGammaCorrectPSKShader.txt"),_T("PSMain"));

	if (!ms_pPostScreenQuad)
	{
		return 0;
	}
	ms_pPostScreenQuad->PreLoad(SKUsedName::ms_cPostScreenQuad,_T("SKPostEffectVSKShader.txt"),_T("SKMain"),_T("SKScreenQuadPSKShader.txt"),_T("PSMain"));

	if (!ms_pPostVolumeShadowMap)
	{
		return 0;
	}
	ms_pPostVolumeShadowMap->PreLoad(SKUsedName::ms_cPostVolumeShadowMap,_T("SKPostEffectVSKShader.txt"),_T("SKMain"),_T("VolumeShadowMapPSKShader.txt"),_T("PSMain"));

	if (!ms_pProjectShadowPre)
	{
		return 0;
	}
	ms_pProjectShadowPre->PreLoad(_T("ProjectShadowPre"),_T("SKProjectShadowPreVS.txt"),_T("SKMain"),_T("SKDefaultPS.txt"),_T("PSMain"));

	if (!ms_pProjectShadow)
	{
		return 0;
	}
	ms_pProjectShadow->PreLoad(_T("ProjectShadow"),_T("SKProjectShadowVS.txt"),_T("SKMain"),_T("SKProjectShadowPS.txt"),_T("PSMain"));

	if (!ms_pSSR)
	{
		return 0;
	}

	ms_pSSR->PreLoad(_T("ScreenSpaceReflection"),_T("SKPostEffectVSKShader.txt"),_T("SKMain"),_T("SKSSRPSKShader.txt"),_T("PSMain"));

	if (!ms_pFont)
	{
		return 0;
	}
	ms_pFont->PreLoad(_T("FontShader"), _T("SKFontVSKShader.txt"), _T("SKMain"), _T("SKFontPSKShader.txt"), _T("PSMain"));
	return 1;
}
void SKCustomMaterial::SetVShaderValue(const SKUsedName & Name,void *fValue)
{
	if (m_pVShader)
	{
		m_pVShader->SetParam(Name,fValue);
	}
}
void SKCustomMaterial::SetVShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllState * pTex)
{	
	if (m_pVShader)
	{
		m_pVShader->SetParam(TexSamplerNodeName,pTex);
	}
}
void SKCustomMaterial::SetPShaderValue(const SKUsedName & Name,void *fValue)
{
	if (m_pPShader)
	{
		m_pPShader->SetParam(Name,fValue);
	}
}
void SKCustomMaterial::SetPShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllState * pTex)
{
	if (m_pPShader)
	{
		m_pPShader->SetParam(TexSamplerNodeName,pTex);
	}
}
SKRenderState & SKCustomMaterial::GetRenderState()
{
	return m_RenderState;
}
void SKCustomMaterial::SetBlendState(SKBlendState * pBlendState)
{
	m_RenderState.SetBlendState(pBlendState);
}
void SKCustomMaterial::SetDepthStencilState(SKDepthStencilState *pDepthStencilState)
{
	m_RenderState.SetDepthStencilState(pDepthStencilState);
}
void SKCustomMaterial::SetRasterizerState(SKRasterizerState *pRasterizerState)
{
	m_RenderState.SetRasterizerState(pRasterizerState);
}
void SKCustomMaterial::AddClipPlane(const SKPlane3 & Plane)
{
	m_RenderState.AddPlane(Plane);
}
void SKCustomMaterial::AddScissorRect(const SKRect2 & Rect)
{
	m_RenderState.AddRect(Rect);
}
SKCustomMaterial::SKCustomMaterial(const SKUsedName & ResourceName,const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
				 const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName)
{
		m_pVShaderSet = NULL;
		m_pPShaderSet = NULL;
		m_pGShaderSet = NULL;
		m_pVShader = NULL;
		m_pPShader = NULL;
		m_pGShader = NULL;
		PreLoad(ResourceName,pVShaderFileName,pVMainFunName,pPShaderFileName,pPMainFunName);
	
}
SKCustomMaterial::SKCustomMaterial()
{
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	m_pGShaderSet = NULL;
}
SKCustomMaterial::~SKCustomMaterial()
{
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
}
void SKCustomMaterial::PreLoad(const SKUsedName & ResourceName,
							const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
							  const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName)
{
	m_VShaderName = pVShaderFileName;
	m_PShaderName = pPShaderFileName;
	m_VMainFunName = pVMainFunName;
	m_PMainFunName = pPMainFunName;
	m_ResourceName = ResourceName;

}
bool SKCustomMaterial::GetVShader(SKShaderKey & VShaderKey)
{
	SKVShader * pVertexShader = NULL;

	if (!m_pVShaderSet)
	{
		m_pVShaderSet = SKResourceManager::GetVertexShaderMap().GetShaderSet(m_VShaderName);
	}
	
	
	
	if (m_pVShader == NULL)
	{
		if (m_pVShaderSet)
		{
			unsigned int uiIndex = m_pVShaderSet->Find(VShaderKey);

			if (uiIndex != m_pVShaderSet->GetNum())
			{
				SKShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
				pVertexShader = (SKVShader *)(pTemp);
			}

		}
		if (pVertexShader == NULL)
		{
			pVertexShader = SK_NEW SKVShader(m_VShaderName.GetBuffer(),m_VMainFunName.GetBuffer(),true);
			if (!pVertexShader)
			{
				return 0;
			}
			pVertexShader->m_ShaderKey = VShaderKey;
			SKRenderer::ms_pRenderer->LoadVShaderProgram(pVertexShader);
			if (m_pVShaderSet)
			{
				m_pVShaderSet->AddElement(VShaderKey,pVertexShader);
			}
		}
		m_pVShader = pVertexShader;
	}
	else
	{
		if (m_pVShader->m_ShaderKey == VShaderKey)
		{

		}
		else
		{

			if (m_pVShaderSet)
			{
				unsigned int uiIndex = m_pVShaderSet->Find(VShaderKey);

				if (uiIndex != m_pVShaderSet->GetNum())
				{
					SKShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
					pVertexShader = (SKVShader *)(pTemp);
				}
			}
			if (pVertexShader == NULL)
			{
				pVertexShader = SK_NEW SKVShader(m_VShaderName.GetBuffer(),m_VMainFunName.GetBuffer(),true);
				if (!pVertexShader)
				{
					return 0;
				}
				pVertexShader->m_ShaderKey = VShaderKey;
				SKRenderer::ms_pRenderer->LoadVShaderProgram(pVertexShader);
				if (m_pVShaderSet)
				{
					m_pVShaderSet->AddElement(VShaderKey,pVertexShader);
				}
			}
			m_pVShader = pVertexShader;
		}
	}

	if (!m_pVShaderSet)
	{
		SKResourceManager::GetVertexShaderMap().SetShader(m_VShaderName,VShaderKey,pVertexShader);
		m_pVShaderSet = SKResourceManager::GetVertexShaderMap().GetShaderSet(m_VShaderName);
	}
	return 1;
}
bool SKCustomMaterial::GetGShader(SKShaderKey & GShaderKey)
{
	unsigned int uiGShaderNum = 0;
	SKGShader * pGShader = NULL;
	if (m_GShaderName.GetBuffer() == NULL)
	{
		return false;
	}
	if (!m_pGShaderSet)
	{
		m_pGShaderSet = SKResourceManager::GetGeometryShaderMap().GetShaderSet(m_GShaderName);
	}



	if (m_pGShaderSet)
	{
		uiGShaderNum = m_pGShaderSet->GetNum();
	}


	if (m_pGShader == NULL)
	{
		if (m_pGShaderSet)
		{
			unsigned int uiIndex = m_pGShaderSet->Find(GShaderKey);

			if (uiIndex != m_pGShaderSet->GetNum())
			{
				SKShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
				pGShader = (SKGShader *)(pTemp);
			}
		}
		if (pGShader == NULL)
		{
			pGShader = SK_NEW SKGShader(m_GShaderName.GetBuffer(), m_GMainFunName.GetBuffer(), true);

			if (!pGShader)
			{
				return 0;
			}
			pGShader->m_ShaderKey = GShaderKey;
			SKRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
			if (m_pGShaderSet)
			{
				m_pGShaderSet->AddElement(GShaderKey, pGShader);
			}
		}
		m_pGShader = pGShader;

	}
	else
	{
		if (m_pPShader->m_ShaderKey == GShaderKey)
		{

		}
		else
		{
			if (m_pGShaderSet)
			{
				unsigned int uiIndex = m_pGShaderSet->Find(GShaderKey);

				if (uiIndex != m_pGShaderSet->GetNum())
				{
					SKShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
					pGShader = (SKGShader *)(pTemp);
				}
			}
			if (pGShader == NULL)
			{
				pGShader = SK_NEW SKGShader(m_GShaderName.GetBuffer(), m_GMainFunName.GetBuffer(), true);

				if (!pGShader)
				{
					return 0;
				}
				pGShader->m_ShaderKey = GShaderKey;
				SKRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
				if (m_pGShaderSet)
				{
					m_pGShaderSet->AddElement(GShaderKey, pGShader);
				}
			}
			m_pGShader = pGShader;
		}
	}

	if (!m_pGShaderSet)
	{
		SKResourceManager::GetGeometryShaderMap().SetShader(m_GShaderName, GShaderKey, pGShader);
		m_pGShaderSet = SKResourceManager::GetGeometryShaderMap().GetShaderSet(m_GShaderName);
	}
	return 1;
}
bool SKCustomMaterial::GetPShader(SKShaderKey & PShaderKey)
{
	unsigned int uiPShaderNum = 0;
	SKPShader * pPixelShader = NULL;

	if (!m_pPShaderSet)
	{
		m_pPShaderSet = SKResourceManager::GetPixelShaderMap().GetShaderSet(m_PShaderName);
	}
	
	

	if (m_pPShaderSet)
	{
		uiPShaderNum = m_pPShaderSet->GetNum();
	}


	if (m_pPShader == NULL)
	{
		if (m_pPShaderSet)
		{
			unsigned int uiIndex = m_pPShaderSet->Find(PShaderKey);

			if (uiIndex != m_pPShaderSet->GetNum())
			{
				SKShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
				pPixelShader = (SKPShader *)(pTemp);
			}
		}
		if (pPixelShader == NULL)
		{
			pPixelShader = SK_NEW SKPShader(m_PShaderName.GetBuffer(),m_PMainFunName.GetBuffer(),true);
			
			if (!pPixelShader)
			{
				return 0;
			}
			pPixelShader->m_ShaderKey = PShaderKey;
			SKRenderer::ms_pRenderer->LoadPShaderProgram(pPixelShader);
			if (m_pPShaderSet)
			{
				m_pPShaderSet->AddElement(PShaderKey,pPixelShader);
			}
		}
		m_pPShader = pPixelShader;

	}
	else
	{
		if (m_pPShader->m_ShaderKey == PShaderKey)
		{

		}
		else
		{
			if (m_pPShaderSet)
			{
				unsigned int uiIndex = m_pPShaderSet->Find(PShaderKey);

				if (uiIndex != m_pPShaderSet->GetNum())
				{
					SKShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
					pPixelShader = (SKPShader *)(pTemp);
				}
			}
			if (pPixelShader == NULL)
			{
				pPixelShader = SK_NEW SKPShader(m_PShaderName.GetBuffer(),m_PMainFunName.GetBuffer(),true);
				
				if (!pPixelShader)
				{
					return 0;
				}
				pPixelShader->m_ShaderKey = PShaderKey;
				SKRenderer::ms_pRenderer->LoadPShaderProgram(pPixelShader);
				if (m_pPShaderSet)
				{
					m_pPShaderSet->AddElement(PShaderKey,pPixelShader);
				}
			}
			m_pPShader = pPixelShader;
		}
	}

	if (!m_pPShaderSet)
	{
		SKResourceManager::GetPixelShaderMap().SetShader(m_PShaderName,PShaderKey,pPixelShader);
		m_pPShaderSet = SKResourceManager::GetPixelShaderMap().GetShaderSet(m_PShaderName);
	}
	return 1;
}
SKGShader *SKCustomMaterial::GetCurGShader(SKShaderKey & GShaderKey)
{
	if (GetGShader(GShaderKey))
	{
		return m_pGShader;
	}
	return NULL;
}
SKVShader *SKCustomMaterial::GetCurVShader(SKShaderKey & VShaderKey)
{
	if (GetVShader(VShaderKey))
	{
		return m_pVShader;
	}
	return NULL;
}
SKPShader *SKCustomMaterial::GetCurPShader(SKShaderKey & PShaderKey)
{
	if(GetPShader(PShaderKey))
	{
		return m_pPShader;
	}
	return NULL;
}
bool  SKCustomMaterial::UseMaterial()
{

	GetCurVShader(*ms_NULLShaderKey);
	GetCurPShader(*ms_NULLShaderKey);
	GetCurGShader(*ms_NULLShaderKey);
	if(!SKRenderer::ms_pRenderer->SetVShader(m_pVShader))
		return 0;
	if (!SKRenderer::ms_pRenderer->SetGShader(m_pGShader))
		return 0;
	if(!SKRenderer::ms_pRenderer->SetPShader(m_pPShader))
		return 0;
	SKRenderer::ms_pRenderer->SetRenderState(m_RenderState);
	return 1;
}

IMPLEMENT_RTTI_NoCreateFun(SKMaterialBase,SKMaterialInterface)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKMaterialBase)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKMaterialBase,SKMaterialInterface)
END_ADD_PROPERTY
SKMaterialBase::SKMaterialBase()
{
	m_VShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();
	m_PShaderCustomValue.Clear();
	m_PShaderCustomTex.Clear();
}
SKMaterialBase::~SKMaterialBase()
{
	m_VShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();

	m_PShaderCustomValue.Clear();
	m_PShaderCustomTex.Clear();
}
void SKMaterialBase::SetVShaderValue(const SKUsedName & Name,void *fValue)
{
	if(!fValue)
		return;
	static CustomFloatValue TempValue;
	TempValue.ConstValueName = Name;
	unsigned int uiIndex = m_VShaderCustomValue.FindElement(TempValue);
	if (uiIndex != m_VShaderCustomValue.GetNum())
	{
		SKMemcpy(&m_VShaderCustomValue[uiIndex].Value[0],fValue,sizeof(SKREAL) * m_VShaderCustomValue[uiIndex].Value.GetNum());
		return;
	}


	return ;
}


void SKMaterialBase::SetVShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex)
{
	static CustomTexSampler TempValue;
	TempValue.ConstValueName = TexSamplerNodeName;
	unsigned int uiIndex = m_VShaderCustomTex.FindElement(TempValue);
	if (uiIndex != m_VShaderCustomTex.GetNum() )
	{
		m_VShaderCustomTex[uiIndex].m_pTexture = pTex;
		return ;
	}


}


void SKMaterialBase::SetPShaderValue(const SKUsedName & Name,void *fValue)
{
	if(!fValue)
		return;

	static CustomFloatValue TempValue;
	TempValue.ConstValueName = Name;
	unsigned int uiIndex = m_PShaderCustomValue.FindElement(TempValue);
	if (uiIndex != m_PShaderCustomValue.GetNum())
	{
		SKMemcpy(&m_PShaderCustomValue[uiIndex].Value[0],fValue,sizeof(SKREAL) * m_PShaderCustomValue[uiIndex].Value.GetNum());
	}

	return ;
}


void SKMaterialBase::SetPShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex)
{
	static CustomTexSampler TempValue;
	TempValue.ConstValueName = TexSamplerNodeName;
	unsigned int uiIndex = m_VShaderCustomTex.FindElement(TempValue);
	if (uiIndex != m_VShaderCustomTex.GetNum() )
	{
		m_PShaderCustomTex[uiIndex].m_pTexture = pTex;
	}

}
IMPLEMENT_RTTI(SKMaterial,SKMaterialBase)
SKPointer<SKMaterial> SKMaterial::Default;
SKPointer<SKMaterial> SKMaterial::DefaultOnlyColor;
SKPointer<SKMaterial> SKMaterial::DefaultOnlyVertexColor;
SKPointer<SKMaterial> SKMaterial::DefaultOnlyColorDisableDepth;
SKPointer<SKMaterial> SKMaterial::DefaultOnlyVertexColorDisableDepth;
BEGIN_ADD_PROPERTY(SKMaterial,SKMaterialBase)
REGISTER_PROPERTY(m_ShowName,ShowName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pShaderMainFunction,ShaderMainFunction,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pShaderFunctionArray,ShaderFunctionArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pLightShaderFunction, LightShaderFunction, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPostEffectShaderFunction, PostEffectShaderFunction, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bIsCombine,IsCombine,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCustomLayer,CustomLayer,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMaterial)
ADD_PRIORITY(SKTexAllState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool SKMaterial::ms_bIsEnableASYNLoader = true;
bool SKMaterial::ms_bIsEnableGC = true;
bool SKMaterial::InitialDefaultState()
{
	Default = SK_NEW SKMaterial(_T("DefaultMaterial"));
	if(!Default)
		return 0;
	
	DefaultOnlyColor = SK_NEW SKMaterial(_T("DefaultOnlyColorMaterial"));

	if(!DefaultOnlyColor)
		return 0;

	DefaultOnlyVertexColor = SK_NEW SKMaterial(_T("DefaultOnlyVertexColorMaterial"));

	if (!DefaultOnlyVertexColor)
	{
		return 0;
	}

	DefaultOnlyColorDisableDepth = SK_NEW SKMaterial(_T("DefaultOnlyColorDisableDepthMaterial"));

	if(!DefaultOnlyColorDisableDepth)
		return 0;

	DefaultOnlyVertexColorDisableDepth = SK_NEW SKMaterial(_T("DefaultOnlyVertexColorDisableDepthMaterial"));

	if (!DefaultOnlyVertexColorDisableDepth)
	{
		return 0;
	}
	return 1;
}
bool SKMaterial::TerminalDefaultState()
{
	Default = NULL;
	DefaultOnlyColor = NULL;
	DefaultOnlyVertexColor = NULL;
	DefaultOnlyColorDisableDepth = NULL;
	DefaultOnlyVertexColorDisableDepth = NULL;
	return 1;
}
bool SKMaterial::LoadDefault()
{
	{
		if(!Default)
		return 0;
		VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("TexSampler"),Default);
		p2DTexSamplerNode->SetTexture(SKResourceManager::ms_DefaultTextureResource);
		SKConstFloatValue * pSpecularColor = SK_NEW SKConstFloatValue(_T("SpecluarColor"),Default,4,false);
		pSpecularColor->SetValue(0,0.5f);
		pSpecularColor->SetValue(1,0.5f);
		pSpecularColor->SetValue(2,0.5f);
		pSpecularColor->SetValue(3,1.0f);
		SKConstFloatValue * pSpecularPow = SK_NEW SKConstFloatValue(_T("SpecularPow"),Default,1,false);
		pSpecularPow ->SetValue(0,5);
		Default->m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
		Default->m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
		Default->m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
		Default->m_ResourceName = _T("_Default");
	}
	{
		if(!DefaultOnlyColor)
			return 0;

		SKConstFloatValue * pEmissiveColor = SK_NEW SKConstFloatValue(_T("EmissiveColor"),DefaultOnlyColor,4,true);
		pEmissiveColor->SetValue(0,1.0f);
		pEmissiveColor->SetValue(1,1.0f);
		pEmissiveColor->SetValue(2,1.0f);
		pEmissiveColor->SetValue(3,1.0f);

		DefaultOnlyColor->m_pShaderMainFunction[0]->GetInputNode(SKPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(SKConstFloatValue::OUT_VALUE));
		DefaultOnlyColor->m_ResourceName = _T("_DefaultOnlyColor");
	}

	{
		if(!DefaultOnlyVertexColor)
			return 0;

		SKVertexColor * pVertexColor = SK_NEW SKVertexColor(_T("VertexColor"),DefaultOnlyVertexColor);

		DefaultOnlyVertexColor->m_pShaderMainFunction[0]->GetInputNode(SKPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pVertexColor->GetOutputNode(SKVertexColor::OUT_COLOR));
		DefaultOnlyVertexColor->m_ResourceName = _T("_DefaultOnlyVertexColor");
	}


	{
		if(!DefaultOnlyColorDisableDepth)
			return 0;
		SKDepthStencilDesc DepthStencilDest;
		DepthStencilDest.m_bDepthEnable = false;
		SKDepthStencilState * pState = SKResourceManager::CreateDepthStencilState(DepthStencilDest);
		DefaultOnlyColorDisableDepth->SetDepthStencilState(pState);
		SKConstFloatValue * pEmissiveColor = SK_NEW SKConstFloatValue(_T("EmissiveColor"),DefaultOnlyColorDisableDepth,4,true);
		pEmissiveColor->SetValue(0,1.0f);
		pEmissiveColor->SetValue(1,1.0f);
		pEmissiveColor->SetValue(2,1.0f);
		pEmissiveColor->SetValue(3,1.0f);

		DefaultOnlyColorDisableDepth->m_pShaderMainFunction[0]->GetInputNode(SKPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(SKConstFloatValue::OUT_VALUE));
		DefaultOnlyColorDisableDepth->m_ResourceName = _T("_DefaultOnlyColorDisableDepth");
	}

	{
		if(!DefaultOnlyVertexColorDisableDepth)
			return 0;

		SKDepthStencilDesc DepthStencilDest;
		DepthStencilDest.m_bDepthEnable = false;
		SKDepthStencilState * pState = SKResourceManager::CreateDepthStencilState(DepthStencilDest);
		DefaultOnlyVertexColorDisableDepth->SetDepthStencilState(pState);

		SKVertexColor * pVertexColor = SK_NEW SKVertexColor(_T("VertexColor"),DefaultOnlyVertexColorDisableDepth);

		DefaultOnlyVertexColorDisableDepth->m_pShaderMainFunction[0]->GetInputNode(SKPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pVertexColor->GetOutputNode(SKVertexColor::OUT_COLOR));
		DefaultOnlyVertexColorDisableDepth->m_ResourceName = _T("_DefaultOnlyVertexColorDisableDepth");
	}
	return 1;
}
SKMaterial::SKMaterial()
{	
	m_pShaderMainFunction.Clear();
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	m_bIsCombine = false;
	m_pShaderFunctionArray.Clear();
	m_bIsAllReadyLink = false;
	m_pPass[SKPass::PT_MATERIAL] = SK_NEW SKMaterialPass();
	m_pPass[SKPass::PT_NORMALDEPTH] = SK_NEW SKNormalDepthPass();
	m_pPass[SKPass::PT_POINT_CUBE_SHADOW] = SK_NEW SKCubeShadowPass();
	m_pPass[SKPass::PT_POINT_VOLUME_SHADOW] = SK_NEW SKVolumeShadowPass();
	m_pPass[SKPass::PT_PREZ] = SKPrezPass::GetDefault();
	m_pPass[SKPass::PT_SHADOW] = SK_NEW SKShadowPass();
	m_pPass[SKPass::PT_DUAL_PARABOLOID_SHADOW] = SK_NEW SKDualParaboloidShadowPass();
	m_pPass[SKPass::PT_LIGHT_FUNCTION] = SK_NEW SKLightFunPass();
	m_pPass[SKPass::PT_POSTEFFECT] = SK_NEW SKPostEffectPass();
	m_pPass[SKPass::PT_INDIRECT] = SK_NEW SKIndirectRenderPass();
	SKMAC_ASSERT(m_pPass[SKPass::PT_MATERIAL] && m_pPass[SKPass::PT_INDIRECT]);
	m_uiCustomLayer = 10;
	
}
SKMaterial::~SKMaterial()
{
	//ͷźڵ㣬VSMaterial::DeleteShaderFuntionm_pShaderFunctionArrayɾ,
	//m_pShaderFunctionArrayͻ٣FORѭi1m_pShaderFunctionArray.size()ȴ
	//Щڵ㲻ɾ
	//һʱȻm_pShaderFunctionArrayӦΪգɾʱҲڵ㣬ܰڵ
	//ͷţm_pShaderFunctionArray.size()ֲ䡣
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		SKShaderFunction * pShaderFunction = m_pShaderFunctionArray[i];
		m_pShaderFunctionArray[i] = NULL;
		SKMAC_DELETE(pShaderFunction);
	}
	m_pShaderFunctionArray.Clear();
	
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	m_pShaderMainFunction.Clear();
	m_bIsAllReadyLink = false;
	
	for (unsigned int i = 0 ; i < SKPass::PT_MAX ; i++)
	{	
		m_pPass[i] = NULL;
	}
	

}
SKMaterial::SKMaterial(const SKUsedName &ShowName, unsigned int uiMUT)
{

	m_ShowName = ShowName;
	//ClearҪshaderFunctionǰ
	m_pShaderFunctionArray.Clear();
	m_pShaderMainFunction.Clear();
	m_pLightShaderFunction = NULL;
	m_pPostEffectShaderFunction = NULL;
	if (uiMUT == MUT_PHONE)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKPhoneShaderFunction(_T("PSMain"),this));
		
	}
	else if (uiMUT == MUT_OREN_NAYAR)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKOrenNayarShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_MINNAERT)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKMinnaertShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_STRAUSS)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKStraussShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SHIRLEY)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKShirleyShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SCHLICK)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKSchlickShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_COOKTOORANCE)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKCookTorranceShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKIsotropicWardShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ANISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKAnisotropicWardShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_CUSTOM)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKCustomShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_LIGHT)
	{
		m_pLightShaderFunction = SK_NEW SKLightShaderFunction(_T("PSMain"),this);
	}
	else if (uiMUT == MUT_POSTEFFECT)
	{
		m_pPostEffectShaderFunction = SK_NEW SKPostEffectShaderFunction(_T("PSMain"), this);
	}
	m_bIsAllReadyLink = false;
	m_bIsCombine = false;
	m_pPass[SKPass::PT_MATERIAL] = SK_NEW SKMaterialPass();
	m_pPass[SKPass::PT_NORMALDEPTH] = SK_NEW SKNormalDepthPass();
	m_pPass[SKPass::PT_POINT_CUBE_SHADOW] = SK_NEW SKCubeShadowPass();
	m_pPass[SKPass::PT_POINT_VOLUME_SHADOW] = SK_NEW SKVolumeShadowPass();
	m_pPass[SKPass::PT_PREZ] = SKPrezPass::GetDefault();
	m_pPass[SKPass::PT_SHADOW] = SK_NEW SKShadowPass();
	m_pPass[SKPass::PT_DUAL_PARABOLOID_SHADOW] = SK_NEW SKDualParaboloidShadowPass();
	m_pPass[SKPass::PT_LIGHT_FUNCTION] = SK_NEW SKLightFunPass();
	m_pPass[SKPass::PT_POSTEFFECT] = SK_NEW SKPostEffectPass();
	m_pPass[SKPass::PT_INDIRECT] = SK_NEW SKIndirectRenderPass();
	SKMAC_ASSERT(m_pPass[SKPass::PT_MATERIAL] && m_pPass[SKPass::PT_INDIRECT]);
	m_uiCustomLayer = 10;
}
void SKMaterial::AddPass(unsigned int uiMUT)
{
	if (m_pLightShaderFunction || m_pPostEffectShaderFunction)
	{
		return ;
	}
	if (uiMUT == MUT_PHONE)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKPhoneShaderFunction(_T("PSMain"),this));

	}
	else if (uiMUT == MUT_OREN_NAYAR)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKOrenNayarShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_MINNAERT)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKMinnaertShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_STRAUSS)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKStraussShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SHIRLEY)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKShirleyShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_SCHLICK)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKSchlickShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_COOKTOORANCE)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKCookTorranceShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKIsotropicWardShaderFunction(_T("PSMain"),this));
	}
	else if (uiMUT == MUT_ANISOTROPICWARD)
	{
		m_pShaderMainFunction.AddElement(SK_NEW SKAnisotropicWardShaderFunction(_T("PSMain"),this));
	}
}
void SKMaterial::SetBlendState(SKBlendState * pBlendState,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->SetBlendState(pBlendState);
}
void SKMaterial::SetDepthStencilState(SKDepthStencilState *pDepthStencilState,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->SetDepthStencilState(pDepthStencilState);
}
void SKMaterial::SetRasterizerState(SKRasterizerState *pRasterizerState,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->SetRasterizerState(pRasterizerState);
}
void SKMaterial::AddClipPlane(const SKPlane3 & Plane,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->AddClipPlane(Plane);
}
void SKMaterial::AddScissorRect(const SKRect2 & Rect,unsigned int uiPassId)
{
	if (uiPassId >= m_pShaderMainFunction.GetNum())
	{
		return ;
	}
	m_pShaderMainFunction[uiPassId]->AddScissorRect(Rect);
}
void SKMaterial::AddShaderFuntion(SKShaderFunction *pShaderFuntion)
{
	if(pShaderFuntion)
	{
		m_pShaderFunctionArray.AddElement(pShaderFuntion);
	}

}
void SKMaterial::DeleteShaderFuntion(SKShaderFunction *pShaderFuntion)
{
	if(pShaderFuntion)
	{
		if (m_pLightShaderFunction && m_pLightShaderFunction == pShaderFuntion)
		{
			// can't delete LightShaderFunction;
			return ;
		}

		if (m_pPostEffectShaderFunction && m_pPostEffectShaderFunction == pShaderFuntion)
		{
			// can't delete PostEffectShaderFunction;
			return;
		}
		if (m_pShaderMainFunction.GetNum() > 0 && pShaderFuntion == m_pShaderMainFunction[0])
		{
			//can't delete first 
			return ;
		}
		for (unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
		{
			if(m_pShaderFunctionArray[i] == pShaderFuntion)
			{
				for (unsigned int j = 0 ; j < pShaderFuntion->GetOutputNodeNum() ;j++)
				{
					SKOutputNode * pOutputNode = pShaderFuntion->GetOutputNode(j);
					pOutputNode->UnConnection();
				}				
				m_pShaderFunctionArray.Erase(i);
				SKMAC_DELETE(pShaderFuntion);
				return ;
			}
		}
		
	}

}


void SKMaterial::LinkAllResource()
{
	if(m_bIsAllReadyLink)
		return ;
	m_bIsAllReadyLink = true;
	m_PShaderCustomTex.Clear();
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		SKTexSampler *Temp = DynamicCast<SKTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp)
			continue;
		
		CustomTexSampler CTex;
		CTex.ConstValueName = Temp->GetShowName();
		CTex.m_pTexture = Temp->GetTexture();
		m_PShaderCustomTex.AddElement(CTex);
		
	}
	m_PShaderCustomValue.Clear();
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		SKConstValue *Temp = DynamicCast<SKConstValue>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->IsCustom())
			continue;
		if (Temp->GetType() == SKUserConstant::VT_FLOAT)
		{
			CustomFloatValue CustomValueTemp;
			CustomValueTemp.ConstValueName = Temp->GetShowName();
			CustomValueTemp.Value = ((SKConstFloatValue *)Temp)->GetValue();
			m_PShaderCustomValue.AddElement(CustomValueTemp);
		}
		
	}

}
void SKMaterial::ClearShaderFuntionVisitFlagFalse()
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		m_pShaderFunctionArray[i]->m_bIsVisited = 0;
	}
}
bool SKMaterial::GetCustomDeclareString(SKString & OutString,unsigned char uPassId)
{
	SKCustomShaderFunction * pCustomShaderFunction = DynamicCast<SKCustomShaderFunction>(m_pShaderMainFunction[uPassId]);
	if (pCustomShaderFunction)
	{
		OutString = pCustomShaderFunction->m_LightFunctionString;
		return true;
	}
	return false;
}
bool SKMaterial::GetShaderTreeString(SKString & OutString,MaterialShaderPara &MSPara,unsigned int uiOST,unsigned char uPassId)
{
	SKMap<SKShaderFunction *, SKArray<SKShaderFunction *>> NoValidMap;
	SKShaderStringFactory::ms_ShaderValueIndex = 0;
	for (unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		m_pShaderFunctionArray[i]->ResetInShaderName();
	}
	if (m_pLightShaderFunction)
	{
		bool Temp = m_pLightShaderFunction->CheckChildNodeValidAll(NoValidMap);
		if (!Temp)
		{
			return false;
		}
		m_pLightShaderFunction->ClearShaderTreeStringFlag();
		return m_pLightShaderFunction->GetShaderTreeString(OutString);
	}
	else if (m_pPostEffectShaderFunction)
	{
		bool Temp = m_pPostEffectShaderFunction->CheckChildNodeValidAll(NoValidMap);
		if (!Temp)
		{
			return false;
		}
		m_pPostEffectShaderFunction->ClearShaderTreeStringFlag();
		return m_pPostEffectShaderFunction->GetShaderTreeString(OutString);
	}
	else
	{
		bool Temp = m_pShaderMainFunction[uPassId]->CheckChildNodeValidAll(NoValidMap);
		if (!Temp)
		{
			return false;
		}
		m_pShaderMainFunction[uPassId]->SetMaterialShaderPara(MSPara);
		m_pShaderMainFunction[uPassId]->ClearShaderTreeStringFlag();

		SKCustomShaderFunction * pCustomShaderFunction = DynamicCast<SKCustomShaderFunction>(m_pShaderMainFunction[uPassId]);
		if (pCustomShaderFunction)
		{
			pCustomShaderFunction->CreatLightFunctionString(uiOST);
		}	
		return m_pShaderMainFunction[uPassId]->GetShaderTreeString(OutString,uiOST);
	}
	
}
bool SKMaterial::PostClone(SKObject * pObjectSrc)
{
	SKMaterialBase::PostClone(pObjectSrc);
	LinkAllResource();
	return true;
}

bool SKMaterial::PostLoad(void * pDate)
{
	SKMaterialBase::PostLoad(pDate);
	LinkAllResource();
	return true;
}
void SKMaterial::SetGlobleValue(SKPass * pPass, unsigned int uiPassId,SKVShader * pVShader , SKPShader * pPShader)
{
	if (pPass == m_pPass[SKPass::PT_MATERIAL])
	{
		m_pShaderMainFunction[uiPassId]->SetGlobleValue(SKShaderMainFunction::OST_MATERIAL,pVShader,pPShader);
	}
	else if (pPass == m_pPass[SKPass::PT_NORMALDEPTH])
	{
		m_pShaderMainFunction[uiPassId]->SetGlobleValue(SKShaderMainFunction::OST_NORMAL_DEPTH,pVShader,pPShader);
	}
}
void SKMaterial::CreateCustomTexture(SKPShader * pShader)
{
	
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		SKTexSampler *Temp = DynamicCast<SKTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->m_bIsVisited)
			continue;
		unsigned int RegisterID = 0;
		if (pShader->m_pUserSampler.GetNum())
		{
			SKUserSampler * Last = pShader->m_pUserSampler[pShader->m_pUserSampler.GetNum() - 1];
			if (Last)
			{
				RegisterID = Last->GetRegisterIndex() + Last->GetRegisterNum();
			}
		}
		SKUserSampler * pSampler = SK_NEW SKUserSampler(Temp->GetShowName(),Temp->GetTexType(),RegisterID,1);
		pShader->m_pUserSampler.AddElement(pSampler);
	}
}
void SKMaterial::CreateTextureDeclare(SKString & OutString,unsigned int uiRegisterID)
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		SKTexSampler *Temp = DynamicCast<SKTexSampler>(m_pShaderFunctionArray[i]);
		if(!Temp|| !Temp->m_bIsVisited)
			continue;
		Temp->GetDeclareString(OutString,uiRegisterID);
		uiRegisterID++;
	}
}
void SKMaterial::CreateCustomValue(SKPShader * pShader)
{
	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum(); i++)
	{
		SKConstValue *Temp = DynamicCast<SKConstValue>(m_pShaderFunctionArray[i]);
		if(!Temp || !Temp->IsCustom() || !Temp->m_bIsVisited)
			continue;
		SKUserConstant * UserConstantTemp = NULL;
		unsigned int uiRegisterIndex = 0;
		if (pShader->m_pUserConstant.GetNum())
		{
			SKUserConstant * Last = pShader->m_pUserConstant[pShader->m_pUserConstant.GetNum() - 1];
			if (Last)
			{
				uiRegisterIndex = Last->GetRegisterIndex() + Last->GetRegisterNum();
			}
		}
		UserConstantTemp = SK_NEW SKUserConstant(Temp->GetShowName(), Temp->GetOutputNode(0)->GetNodeName(), NULL,Temp->GetSize(), uiRegisterIndex, 1, Temp->GetType());
		pShader->m_pUserConstant.AddElement(UserConstantTemp);
	}
}


void SKMaterial::CreateConstValueDeclare(SKString & OutString,unsigned int uiRegisterID)
{

	for(unsigned int i = 0 ; i < m_pShaderFunctionArray.GetNum() ; i++)
	{
		SKConstValue *Temp = DynamicCast<SKConstValue>(m_pShaderFunctionArray[i]);
		if (!Temp || !Temp->m_bIsVisited || !Temp->IsCustom())
			continue;
		Temp->GetDeclareString(OutString,uiRegisterID);
		uiRegisterID++;
	}
}
IMPLEMENT_RTTI(SKMaterialInstance,SKObject)
BEGIN_ADD_PROPERTY(SKMaterialInstance,SKObject)
REGISTER_PROPERTY(m_VShaderCustomValue,VShaderCustomValue,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_PShaderCustomValue,PShaderCustomValue,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_VShaderCustomTex,VShaderCustomTex,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_PShaderCustomTex,PShaderCustomTex,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pMaterial,Material,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMaterialInstance)
IMPLEMENT_INITIAL_END
SKMaterialInstance::SKMaterialInstance()
{
	m_pMaterial = NULL;
	m_VShaderCustomValue.Clear();
	m_PShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();
	m_PShaderCustomTex.Clear();
	for (unsigned int i = 0 ; i < SKPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
}
SKMaterialInstance::SKMaterialInstance(SKMaterialR * pMaterial)
{
	SKMAC_ASSERT(pMaterial);
	m_pMaterial = pMaterial;
	m_pMaterial->GetResource()->LinkAllResource();	
	GetAllMaterialPara();
	for (unsigned int i = 0 ; i < SKPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
}
void SKMaterialInstance::GetAllMaterialPara()
{
	if (!m_pMaterial)
	{
		return ;
	}
	m_VShaderCustomValue = m_pMaterial->GetResource()->m_VShaderCustomValue;
	m_PShaderCustomValue = m_pMaterial->GetResource()->m_PShaderCustomValue;
	m_VShaderCustomTex = m_pMaterial->GetResource()->m_VShaderCustomTex;
	m_PShaderCustomTex = m_pMaterial->GetResource()->m_PShaderCustomTex;
}
void SKMaterialInstance::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	for (unsigned int i = 0; i < SKPass::PT_MAX;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
}
SKMaterialInstance::~SKMaterialInstance()
{
	m_pMaterial = NULL;
	m_VShaderCustomValue.Clear();
	m_PShaderCustomValue.Clear();
	m_VShaderCustomTex.Clear();
	m_PShaderCustomTex.Clear();
	for (unsigned int i = 0 ; i < SKPass::PT_MAX ;i++)
	{
		m_pCurVShader[i] = NULL;
		m_pCurPShader[i] = NULL;
		m_pCurGShader[i] = NULL;
	}
	
}
void SKMaterialInstance::DeleteVShaderValue(const SKUsedName &Name)
{
	for(unsigned int i = 0 ; i < m_VShaderCustomValue.GetNum() ; i++)
	{
		if(m_VShaderCustomValue[i].ConstValueName == Name)
		{
			m_VShaderCustomValue.Erase(i);
			return ;
		}
	}
}

void SKMaterialInstance::SetVShaderValue(const SKUsedName & Name,void *fValue,unsigned int uiSize)
{
	if(!uiSize || !fValue)
		return;
	for(unsigned int i = 0 ; i < m_VShaderCustomValue.GetNum() ; i++)
	{
		if(m_VShaderCustomValue[i].ConstValueName == Name)
		{
			SKMemcpy(&m_VShaderCustomValue[i].Value[0],fValue,sizeof(SKREAL) * m_VShaderCustomValue[i].Value.GetNum());
			return;
		}
	}
	//û
	CustomFloatValue Temp;
	Temp.ConstValueName = Name;
	Temp.Value.SetBufferNum(uiSize);
	SKMemcpy(&Temp.Value[0],fValue,sizeof(SKREAL) * uiSize);
	m_VShaderCustomValue.AddElement(Temp);
	return ;
}
void SKMaterialInstance::DeleteVShaderTexture(const SKUsedName & TexSamplerNodeName)
{
	for(unsigned int i = 0 ; i < m_VShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_VShaderCustomTex[i].ConstValueName)
		{
			m_VShaderCustomTex.Erase(i);
			return ;
		}
	}
}

void SKMaterialInstance::SetVShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex)
{
	for(unsigned int i = 0 ; i < m_VShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_VShaderCustomTex[i].ConstValueName)
		{
			m_VShaderCustomTex[i].m_pTexture = pTex;
			return ;
		}
	}
	CustomTexSampler CTex;
	CTex.ConstValueName = TexSamplerNodeName;
	CTex.m_pTexture = pTex;
	m_VShaderCustomTex.AddElement(CTex);
}

void SKMaterialInstance::DeletePShaderValue(const SKUsedName &Name)
{
	for(unsigned int i = 0 ; i < m_PShaderCustomValue.GetNum() ; i++)
	{
		if(m_PShaderCustomValue[i].ConstValueName == Name)
		{
			m_PShaderCustomValue.Erase(i);
			return ;
		}
	}
}

void SKMaterialInstance::SetPShaderValue(const SKUsedName & Name,void *fValue,unsigned int uiSize)
{
	if(!uiSize || !fValue)
		return;
	for(unsigned int i = 0 ; i < m_PShaderCustomValue.GetNum() ; i++)
	{
		if(m_PShaderCustomValue[i].ConstValueName == Name)
		{
			SKMemcpy(&m_PShaderCustomValue[i].Value[0],fValue,sizeof(SKREAL) * m_PShaderCustomValue[i].Value.GetNum());
			return;
		}
	}
	//û
	CustomFloatValue Temp;
	Temp.ConstValueName = Name;
	Temp.Value.SetBufferNum(uiSize);
	SKMemcpy(&Temp.Value[0],fValue,sizeof(SKREAL) * uiSize);
	m_PShaderCustomValue.AddElement(Temp);
	return ;
}
void SKMaterialInstance::DeletePShaderTexture(const SKUsedName & TexSamplerNodeName)
{
	for(unsigned int i = 0 ; i < m_PShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_PShaderCustomTex[i].ConstValueName)
		{
			m_PShaderCustomTex.Erase(i);
			return ;
		}
	}
}

void SKMaterialInstance::SetPShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex)
{
	for(unsigned int i = 0 ; i < m_PShaderCustomTex.GetNum() ; i++)
	{
		if(TexSamplerNodeName == m_PShaderCustomTex[i].ConstValueName)
		{
			m_PShaderCustomTex[i].m_pTexture = pTex;
			return ;
		}
	}
	CustomTexSampler CTex;
	CTex.ConstValueName = TexSamplerNodeName;
	CTex.m_pTexture = pTex;
	m_PShaderCustomTex.AddElement(CTex);
}

void SKMaterialInstance::SetPShaderValue(SKPShader * pPShader)
{
	if (!pPShader)
	{
		return ;	
	}
	for (unsigned int i = 0 ; i < m_PShaderCustomValue.GetNum() ; i++)
	{
		pPShader->SetParam(m_PShaderCustomValue[i].ConstValueName,m_PShaderCustomValue[i].Value.GetBuffer());
	}
	for (unsigned int i = 0 ; i < m_PShaderCustomTex.GetNum() ; i++)
	{
		if (m_PShaderCustomTex[i].m_pTexture)
		{
			pPShader->SetParam(m_PShaderCustomTex[i].ConstValueName,m_PShaderCustomTex[i].m_pTexture->GetResource());
		}
		else
		{
			pPShader->SetParam(m_PShaderCustomTex[i].ConstValueName,(SKTexAllState *)NULL);
		}
		
	}
}
void SKMaterialInstance::SetVShaderValue(SKVShader * pVShader)
{
	if (!pVShader)
	{
		return ;
	}
	for (unsigned int i = 0 ; i < m_VShaderCustomValue.GetNum() ; i++)
	{
		pVShader->SetParam(m_VShaderCustomValue[i].ConstValueName,m_VShaderCustomValue[i].Value.GetBuffer());
	}
	for (unsigned int i = 0 ; i < m_VShaderCustomTex.GetNum() ; i++)
	{
		if (m_VShaderCustomTex[i].m_pTexture)
		{
			pVShader->SetParam(m_VShaderCustomTex[i].ConstValueName,m_VShaderCustomTex[i].m_pTexture->GetResource());
		}
		else
		{
			pVShader->SetParam(m_VShaderCustomTex[i].ConstValueName,(SKTexAllState *)NULL);
		}
	}
}

SKMaterialOnlyTexture::SKMaterialOnlyTexture()
{

}
SKMaterialOnlyTexture::SKMaterialOnlyTexture(const SKUsedName &ShowName,SKTexAllStateR * pTexture)
:SKMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pSpecularColor = SK_NEW SKConstFloatValue(_T("SpecluarColor"),this,4,false);
	pSpecularColor->SetValue(0,1.0f);
	pSpecularColor->SetValue(1,1.0f);
	pSpecularColor->SetValue(2,1.0f);
	pSpecularColor->SetValue(3,1.0f);
	SKConstFloatValue * pSpecularPow = SK_NEW SKConstFloatValue(_T("SpecularPow"),this,1,false);
	pSpecularPow->SetValue(0,300);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_ResourceName = _T("_OnlyTexture");

}
SKMaterialOnlyTexture::~SKMaterialOnlyTexture()
{

}
SKMaterialTextureAndNormal::SKMaterialTextureAndNormal()
{

}
SKMaterialTextureAndNormal::SKMaterialTextureAndNormal(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture)
:SKMaterial(ShowName)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pSpecularColor = SK_NEW SKConstFloatValue(_T("SpecluarColor"),this,4,false);
	pSpecularColor->SetValue(0,0.5f);
	pSpecularColor->SetValue(1,0.5f);
	pSpecularColor->SetValue(2,0.5f);
	pSpecularColor->SetValue(3,1.0f);
	SKConstFloatValue * pSpecularPow = SK_NEW SKConstFloatValue(_T("SpecularPow"),this,1,false);
	pSpecularPow ->SetValue(0,100.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularColor->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_ResourceName = _T("TextureAndNormal");
}
SKMaterialTextureAndNormal::~SKMaterialTextureAndNormal()
{

}

SKMaterialOnlyEmissive::SKMaterialOnlyEmissive(const SKUsedName &ShowName)
:SKMaterial(ShowName)
{

	SKConstFloatValue * pEmissiveColor = SK_NEW SKConstFloatValue(_T("EmissiveColor"),this,4,true);
	pEmissiveColor->SetValue(0,1.0f);
	pEmissiveColor->SetValue(1,1.0f);
	pEmissiveColor->SetValue(2,1.0f);
	pEmissiveColor->SetValue(3,1.0f);

	m_pShaderMainFunction[0]->GetInputNode(SKPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(pEmissiveColor->GetOutputNode(SKConstFloatValue::OUT_VALUE));
	m_ResourceName = _T("OnlyEmissive");
}
SKMaterialOnlyEmissive::~SKMaterialOnlyEmissive()
{

}
SKMaterialNoLight::SKMaterialNoLight(const SKUsedName &ShowName)
:SKMaterial(ShowName)
{
	// float One = 1.0f;
	SKConstFloatValue * pOne = SK_NEW SKConstFloatValue(_T("One"), this, 1, false);
	pOne->SetValue(0, 1.0f);

	// WorldPos
	SKWorldPos * pWorldPos = SK_NEW SKWorldPos(_T("WorldPos"), this);

	//WorldPos_One = float4(WorldPos,1.0f);
	SKMakeValue * pWorldPos_One = SK_NEW SKMakeValue(_T("WorldPos_One"), this, 2);
	pWorldPos_One->GetInputNode(SKMakeValue::IN_A)->Connection(pWorldPos->GetOutputNode(SKAdd::OUT_VALUE));
	pWorldPos_One->GetInputNode(SKMakeValue::IN_B)->Connection(pOne->GetOutputNode(SKAdd::OUT_VALUE));

	//float4X4 ReflectViewProject;
	SKConstFloatValue * pReflectViewProject_1 = SK_NEW SKConstFloatValue(_T("ReflectViewProject_1 "), this, 4, true);
	SKConstFloatValue * pReflectViewProject_2 = SK_NEW SKConstFloatValue(_T("ReflectViewProject_2 "), this, 4, true);
	SKConstFloatValue * pReflectViewProject_3 = SK_NEW SKConstFloatValue(_T("ReflectViewProject_3 "), this, 4, true);
	SKConstFloatValue * pReflectViewProject_4 = SK_NEW SKConstFloatValue(_T("ReflectViewProject_4 "), this, 4, true);

	//float Dot_1 = dot(WorldPos_One,ReflectViewProject_1);
	SKDot * pDot_1 = SK_NEW SKDot(_T("Dot_1"), this);
	pDot_1->GetInputNode(SKAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(SKWorldPos::OUT_VALUE));
	pDot_1->GetInputNode(SKAdd::IN_B)->Connection(pReflectViewProject_1->GetOutputNode(SKWorldPos::OUT_VALUE));

	//float Dot_2 = dot(WorldPos_One,ReflectViewProject_2);
	SKDot * pDot_2 = SK_NEW SKDot(_T("Dot_2"), this);
	pDot_2->GetInputNode(SKAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(SKWorldPos::OUT_VALUE));
	pDot_2->GetInputNode(SKAdd::IN_B)->Connection(pReflectViewProject_2->GetOutputNode(SKWorldPos::OUT_VALUE));

	//float Dot_3 = dot(WorldPos_One,ReflectViewProject_3);
	SKDot * pDot_3 = SK_NEW SKDot(_T("Dot_3"), this);
	pDot_3->GetInputNode(SKAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(SKWorldPos::OUT_VALUE));
	pDot_3->GetInputNode(SKAdd::IN_B)->Connection(pReflectViewProject_3->GetOutputNode(SKWorldPos::OUT_VALUE));

	//float Dot_4 = dot(WorldPos_One,ReflectViewProject_4);
	SKDot * pDot_4 = SK_NEW SKDot(_T("Dot_4"), this);
	pDot_4->GetInputNode(SKAdd::IN_A)->Connection(pWorldPos_One->GetOutputNode(SKWorldPos::OUT_VALUE));
	pDot_4->GetInputNode(SKAdd::IN_B)->Connection(pReflectViewProject_4->GetOutputNode(SKWorldPos::OUT_VALUE));

	//float2 Dot_12 = float2(Dot_1,Dot_2);
	SKMakeValue * pDot_12 = SK_NEW SKMakeValue(_T("Dot_12"), this, 2);
	pDot_12->GetInputNode(SKMakeValue::IN_A)->Connection(pDot_1->GetOutputNode(SKDot::OUT_VALUE));
	pDot_12->GetInputNode(SKMakeValue::IN_B)->Connection(pDot_2->GetOutputNode(SKDot::OUT_VALUE));

	//float2 Dot_44 = float2(Dot_4,Dot_4);
	SKMakeValue * pDot_44 = SK_NEW SKMakeValue(_T("Dot_44"), this, 2);
	pDot_44->GetInputNode(SKMakeValue::IN_A)->Connection(pDot_4->GetOutputNode(SKDot::OUT_VALUE));
	pDot_44->GetInputNode(SKMakeValue::IN_B)->Connection(pDot_4->GetOutputNode(SKDot::OUT_VALUE));

	// float2 Div = Dot_12 / Dot_44;
	SKDiv *pDiv = SK_NEW SKDiv(_T("Div"), this);
	pDiv->GetInputNode(SKDiv::IN_A)->Connection(pDot_12->GetOutputNode(SKMakeValue::OUT_VALUE));
	pDiv->GetInputNode(SKDiv::IN_B)->Connection(pDot_44->GetOutputNode(SKMakeValue::OUT_VALUE));


	SKConstFloatValue * pPointFive = SK_NEW SKConstFloatValue(_T("PointFive"), this, 1, false);
	pPointFive->SetValue(0, 0.5f);

	SKConstFloatValue * pNegPointFive = SK_NEW SKConstFloatValue(_T("NegPointFive"), this, 1, false);
	pNegPointFive->SetValue(0, -0.5f);

	//float2 FiveNegFive = float2(0.5f,-0.5f);
	SKMakeValue * pFiveNegFive = SK_NEW SKMakeValue(_T("FiveNegFive"), this, 2);
	pFiveNegFive->GetInputNode(SKMakeValue::IN_A)->Connection(pPointFive->GetOutputNode(SKConstFloatValue::OUT_VALUE));
	pFiveNegFive->GetInputNode(SKMakeValue::IN_B)->Connection(pNegPointFive->GetOutputNode(SKConstFloatValue::OUT_VALUE));

	// Div * FiveFive
	SKMul * pMul = SK_NEW SKMul(_T("Mul"), this);
	pMul->GetInputNode(SKMul::IN_A)->Connection(pDiv->GetOutputNode(SKDiv::OUT_VALUE));
	pMul->GetInputNode(SKMul::IN_B)->Connection(pFiveNegFive->GetOutputNode(SKMakeValue::OUT_VALUE));


	//float2 FiveFive = float2(0.5f,0.5f);
	SKMakeValue * pFiveFive = SK_NEW SKMakeValue(_T("FiveFive"), this, 2);
	pFiveFive->GetInputNode(SKMakeValue::IN_A)->Connection(pPointFive->GetOutputNode(SKConstFloatValue::OUT_VALUE));
	pFiveFive->GetInputNode(SKMakeValue::IN_B)->Connection(pPointFive->GetOutputNode(SKConstFloatValue::OUT_VALUE));

	SKAdd *pAdd = SK_NEW SKAdd(_T("Add"), this);
	pAdd->GetInputNode(SKAdd::IN_A)->Connection(pMul->GetOutputNode(SKMul::OUT_VALUE));
	pAdd->GetInputNode(SKAdd::IN_B)->Connection(pFiveFive->GetOutputNode(SKMakeValue::OUT_VALUE));

	

	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"), this);

	p2DTexSamplerNode->GetInputNode(VS2DTexSampler::IN_TEXCOORD)->Connection(pAdd->GetOutputNode(SKAdd::OUT_VALUE));

	m_pShaderMainFunction[0]->GetInputNode(SKPhoneShaderFunction::IN_EMISSIVE_COLOR)->Connection(p2DTexSamplerNode->GetOutputNode(VS2DTexSampler::OUT_COLOR));
	m_ResourceName = _T("NoLight");
}
SKMaterialNoLight::~SKMaterialNoLight()
{

}
SKMaterialOrenNayar::SKMaterialOrenNayar()
{

}
SKMaterialOrenNayar::SKMaterialOrenNayar(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
							   SKTexAllStateR * pEmissiveTexture):SKMaterial(ShowName,MUT_OREN_NAYAR)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);


	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pRoughnessSquared = SK_NEW SKConstFloatValue(_T("RoughnessSquared"),this,1,true);
	pRoughnessSquared ->SetValue(0,0.5f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
 	m_pShaderMainFunction[0]->GetInputNode(_T("RoughnessSquared"))->Connection(pRoughnessSquared->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
 	pFinalAlpah->SetValue(0,0.6f);
 
 	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	m_ResourceName = _T("_MaterialOrenNayar");
}
SKMaterialOrenNayar::~SKMaterialOrenNayar()
{

}
SKMaterialPhone::SKMaterialPhone()
{

}
SKMaterialPhone::SKMaterialPhone(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
										 SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture,bool bPhoneSpecular):SKMaterial(ShowName,MUT_PHONE)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pSpecularPow = SK_NEW SKConstFloatValue(_T("SpecularPow"),this,1,true);
	pSpecularPow ->SetValue(0,50.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));


	if(bPhoneSpecular)
	{
		((SKPhoneShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(SKPhoneShaderFunction::ST_Phong);
	}
	m_ResourceName = _T("_MaterialPhone");

}
SKMaterialPhone::~SKMaterialPhone()
{

}
SKMaterialPhoneTwoPass::SKMaterialPhoneTwoPass()
{

}
SKMaterialPhoneTwoPass::SKMaterialPhoneTwoPass(const SKUsedName &ShowName, SKTexAllStateR * pDiffuseTexture, SKTexAllStateR * pNormalTexture,
	SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture, bool bPhoneSpecular) :SKMaterial(ShowName, MUT_PHONE)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"), this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"), this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pSpecularPow = SK_NEW SKConstFloatValue(_T("SpecularPow"), this, 1, true);
	pSpecularPow->SetValue(0, 50.0f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);


	if (bPhoneSpecular)
	{
		((SKPhoneShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(SKPhoneShaderFunction::ST_Phong);
	}
	m_ResourceName = _T("_MaterialPhoneTwoPass");


	AddPass(MUT_PHONE);
	SKConstFloatValue * pFinalColor = SK_NEW SKConstFloatValue(_T("FinalColor"), this, 3, false);
	pFinalColor->SetValue(0, 1.0f);
	pFinalColor->SetValue(1, 0.0f);
	pFinalColor->SetValue(2, 0.0f);
	m_pShaderMainFunction[1]->GetInputNode(_T("EmissiveColor"))->Connection(pFinalColor->GetOutputNode(0));

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.5f);

	m_pShaderMainFunction[1]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	SKBlendDesc BlendDesc;
	BlendDesc.bBlendEnable[0] = true;
	BlendDesc.ucBlendOp[0] = SKBlendDesc::BO_ADD;
	BlendDesc.ucDestBlend[0] = SKBlendDesc::BP_INVSRCALPHA;
	BlendDesc.ucSrcBlend[0] = SKBlendDesc::BP_SRCALPHA;
	SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
	SetBlendState(pBlendState, 1);
}
SKMaterialPhoneTwoPass::~SKMaterialPhoneTwoPass()
{

}
SKMaterialMinnaert::SKMaterialMinnaert()
{

}
SKMaterialMinnaert::SKMaterialMinnaert(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
								SKTexAllStateR * pEmissiveTexture):SKMaterial(ShowName,MUT_MINNAERT)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pPower= SK_NEW SKConstFloatValue(_T("Power"),this,1,true);
	pPower ->SetValue(0,0.5f);
	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Power"))->Connection(pPower->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialMinnaert");

}
SKMaterialMinnaert::~SKMaterialMinnaert()
{

}
SKMaterialStrauss::SKMaterialStrauss()
{

}
SKMaterialStrauss::SKMaterialStrauss(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
									   SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture):SKMaterial(ShowName,MUT_STRAUSS)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pSmoothness= SK_NEW SKConstFloatValue(_T("Smoothness"),this,1,true);
	pSmoothness ->SetValue(0,0.5f);

	SKConstFloatValue * pMetalness= SK_NEW SKConstFloatValue(_T("Metalness"),this,1,true);
	pMetalness ->SetValue(0,0.5f);

	SKConstFloatValue * pTransparency= SK_NEW SKConstFloatValue(_T("Transparency"),this,1,true);
	pTransparency ->SetValue(0,0.5f);

	SKConstFloatValue * pK= SK_NEW SKConstFloatValue(_T("K"),this,1,true);
	pK ->SetValue(0,0.5f);

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Smoothness"))->Connection(pSmoothness->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Metalness"))->Connection(pMetalness->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Transparency"))->Connection(pTransparency->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("K"))->Connection(pK->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialStrauss");

}
SKMaterialStrauss::~SKMaterialStrauss()
{

}
SKMaterialShirley::SKMaterialShirley()
{

}
SKMaterialShirley::SKMaterialShirley(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
									 SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture):SKMaterial(ShowName,MUT_SHIRLEY)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pNU= SK_NEW SKConstFloatValue(_T("NU"),this,1,true);
	pNU ->SetValue(0,0.5f);

	SKConstFloatValue * pNV= SK_NEW SKConstFloatValue(_T("NV"),this,1,true);
	pNV ->SetValue(0,0.5f);

	SKConstFloatValue * pRs= SK_NEW SKConstFloatValue(_T("Rs"),this,1,true);
	pRs ->SetValue(0,0.5f);

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("NU"))->Connection(pNU->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("NV"))->Connection(pNV->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Rs"))->Connection(pRs->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialShirley");

}
SKMaterialShirley::~SKMaterialShirley()
{

}
SKMaterialSchlick::SKMaterialSchlick()
{

}
SKMaterialSchlick::SKMaterialSchlick(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
									 SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture):SKMaterial(ShowName,MUT_SCHLICK)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pSpecularPow= SK_NEW SKConstFloatValue(_T("SpecularPow"),this,1,true);
	pSpecularPow ->SetValue(0,0.5f);



	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularPow"))->Connection(pSpecularPow->GetOutputNode(0));



	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialSchlick");

}
SKMaterialSchlick::~SKMaterialSchlick()
{

}
SKMaterialCookTorrance::SKMaterialCookTorrance()
{

}
SKMaterialCookTorrance::SKMaterialCookTorrance(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
									 SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture,bool bSample):SKMaterial(ShowName,MUT_COOKTOORANCE)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pRoughness= SK_NEW SKConstFloatValue(_T("Roughness"),this,1,true);
	pRoughness ->SetValue(0,0.5f);
	SKConstFloatValue * pC= SK_NEW SKConstFloatValue(_T("C"),this,1,true);
	pC ->SetValue(0,0.5f);
	SKConstFloatValue * pR= SK_NEW SKConstFloatValue(_T("R"),this,1,true);
	pR ->SetValue(0,0.5f);


	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Roughness"))->Connection(pRoughness->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("C"))->Connection(pC->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("R"))->Connection(pR->GetOutputNode(0));



	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));


	if (bSample)
	{
		((SKCookTorranceShaderFunction *)m_pShaderMainFunction[0])->SetSpecularType(SKCookTorranceShaderFunction::ST_ALTERNATE_BLINN);
	}

	m_ResourceName = _T("_MaterialCookTorrance");

}
SKMaterialCookTorrance::~SKMaterialCookTorrance()
{

}
SKMaterialIsotropicWard::SKMaterialIsotropicWard()
{

}
SKMaterialIsotropicWard::SKMaterialIsotropicWard(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
											   SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture):SKMaterial(ShowName,MUT_ISOTROPICWARD)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pRoughness= SK_NEW SKConstFloatValue(_T("Roughness"),this,1,true);
	pRoughness ->SetValue(0,0.5f);



	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Roughness"))->Connection(pRoughness->GetOutputNode(0));




	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialIsotropicWard");

}
SKMaterialIsotropicWard::~SKMaterialIsotropicWard()
{

}
SKMaterialAnisotropicWard::SKMaterialAnisotropicWard()
{

}
SKMaterialAnisotropicWard::SKMaterialAnisotropicWard(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
												 SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture):SKMaterial(ShowName,MUT_ANISOTROPICWARD)
{
	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"),this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"),this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pSpecularNode = SK_NEW VS2DTexSampler(_T("SpecularTexture"),this);
	pSpecularNode->SetTexture(pSpecularTexture);

	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"),this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKConstFloatValue * pRoughnessX= SK_NEW SKConstFloatValue(_T("RoughnessX"),this,1,true);
	pRoughnessX ->SetValue(0,0.5f);

	SKConstFloatValue * pRoughnessY= SK_NEW SKConstFloatValue(_T("RoughnessY"),this,1,true);
	pRoughnessY ->SetValue(0,0.5f);

	m_pShaderMainFunction[0]->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("SpecularColor"))->Connection(pSpecularNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("RoughnessX"))->Connection(pRoughnessX->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("RoughnessY"))->Connection(pRoughnessY->GetOutputNode(0));



	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"),this,1,false);
	pFinalAlpah->SetValue(0,0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));



	m_ResourceName = _T("_MaterialAnIsotropicWard");

}
SKMaterialAnisotropicWard::~SKMaterialAnisotropicWard()
{

}
SKMaterialCustom::SKMaterialCustom()
{

}
SKMaterialCustom::SKMaterialCustom(const SKUsedName &ShowName, SKTexAllStateR * pDiffuseTexture, SKTexAllStateR * pNormalTexture, SKTexAllStateR * pEmissiveTexture)
:SKMaterial(ShowName, MUT_CUSTOM)
{
	SKLightDir * pLightDir = SK_NEW SKLightDir(_T("LightDir"), this);

	SKConstFloatValue * pValue = SK_NEW SKConstFloatValue(_T("ValueTest"), this, 1, false);
	pValue->SetValue(0, -1.0f);

	SKMul * pMul1 = SK_NEW SKMul(_T("Mul1"), this);

	pMul1->GetInputNode(0)->Connection(pLightDir->GetOutputNode(0));
	pMul1->GetInputNode(1)->Connection(pValue->GetOutputNode(0));

	SKWorldNormal * pWorldNormal = SK_NEW SKWorldNormal(_T("WorldNormal"), this);

	SKDot * pDot = SK_NEW SKDot(_T("Dot"), this);

	pDot->GetInputNode(0)->Connection(pWorldNormal->GetOutputNode(0));
	pDot->GetInputNode(1)->Connection(pMul1->GetOutputNode(0));

	SKSaturate *pSaturate = SK_NEW SKSaturate(_T("Saturate"), this);
	pSaturate->GetInputNode(0)->Connection(pDot->GetOutputNode(0));

	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	VS2DTexSampler *pNormalNode = SK_NEW VS2DTexSampler(_T("NormalTexture"), this);
	pNormalNode->SetTexture(pNormalTexture);
	pNormalNode->SetVEDecode(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);


	VS2DTexSampler *pEmissiveNode = SK_NEW VS2DTexSampler(_T("EmissiveTexture"), this);
	pEmissiveNode->SetTexture(pEmissiveTexture);

	SKMul* pMul = SK_NEW SKMul(_T("Mul"), this);
	pMul->GetInputNode(0)->Connection(pSaturate->GetOutputNode(0));
	pMul->GetInputNode(1)->Connection(p2DTexSamplerNode->GetOutputNode(0));

	m_pShaderMainFunction[0]->GetInputNode(_T("Normal"))->Connection(pNormalNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Custom"))->Connection(pMul->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("EmissiveColor"))->Connection(pEmissiveNode->GetOutputNode(0));
	m_pShaderMainFunction[0]->GetInputNode(_T("Diffuse"))->Connection(p2DTexSamplerNode->GetOutputNode(0));
	//m_pShaderMainFunction[0]->SetSRGBWrite(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);

	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.6f);

	m_pShaderMainFunction[0]->GetInputNode(_T("Alpha"))->Connection(pFinalAlpah->GetOutputNode(0));

	m_ResourceName = _T("_MaterialCustom");

	//m_pShaderMainFunction[0]->SetAlphaTestValue(0.9f);

}
SKMaterialCustom::~SKMaterialCustom()
{

}

SKLightMaterial::SKLightMaterial()
{

}
SKLightMaterial::SKLightMaterial(const SKUsedName &ShowName, SKTexAllStateR * pDiffuseTexture)
:SKMaterial(ShowName, MUT_LIGHT)
{




	VS2DTexSampler* p2DTexSamplerNode = SK_NEW VS2DTexSampler(_T("DiffuseTexture"), this);
	p2DTexSamplerNode->SetTexture(pDiffuseTexture);
	//p2DTexSamplerNode->SetVESRGB(SKRenderer::VE_R | SKRenderer::VE_G | SKRenderer::VE_B);



	m_pLightShaderFunction->GetInputNode(_T("DiffuseColor"))->Connection(p2DTexSamplerNode->GetOutputNode(0));


	SKConstFloatValue * pFinalAlpah = SK_NEW SKConstFloatValue(_T("FinalAlpha"), this, 1, false);
	pFinalAlpah->SetValue(0, 0.6f);

	m_pLightShaderFunction->GetInputNode(_T("Alpha"))->Connection(p2DTexSamplerNode->GetOutputNode(4));

	m_ResourceName = _T("_LightMaterial");

	//m_pShaderMainFunction[0]->SetAlphaTestValue(0.9f);

}
SKLightMaterial::~SKLightMaterial()
{

}

SKPostEffectMaterial::SKPostEffectMaterial()
{

}
SKPostEffectMaterial::SKPostEffectMaterial(const SKUsedName &ShowName)
:SKMaterial(ShowName, MUT_POSTEFFECT)
{




	SKColorBuffer* pColorBuffer = SK_NEW SKColorBuffer(_T("ColorBuffer"), this);
	SKConstFloatValue * pMulColor = SK_NEW SKConstFloatValue(_T("Color"), this, 4, false);
	pMulColor->SetValue(0, 1.0f);
	pMulColor->SetValue(1, 0.0f);
	pMulColor->SetValue(2, 0.0f);
	pMulColor->SetValue(3, 1.0f);

	SKMul* pMul = SK_NEW SKMul(_T("Mul"), this);
	pMul->GetInputNode(0)->Connection(pColorBuffer->GetOutputNode(0));
	pMul->GetInputNode(1)->Connection(pMulColor->GetOutputNode(0));

	m_pPostEffectShaderFunction->GetInputNode(SKPostEffectShaderFunction::IN_COLOR)->Connection(pMul->GetOutputNode(0));
	m_ResourceName = _T("_PostEffectMaterial");

}
SKPostEffectMaterial::~SKPostEffectMaterial()
{

}