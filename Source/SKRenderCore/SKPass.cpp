#include "SKPass.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKPass,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKPass)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKPass,SKObject)
END_ADD_PROPERTY
SKPass::~SKPass()
{
	m_pMaterialInstance = NULL;
	m_pSpatial = NULL;
	m_pCamera = NULL;
	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	m_pGShaderSet = NULL;
}
SKPass::SKPass()
{
	m_pSpatial = NULL;
	m_pCamera = NULL;
	m_uiPassId = 0 ;
	m_pMaterialInstance = NULL;
	m_pVShaderSet = NULL;
	m_pPShaderSet = NULL;
	m_pGShaderSet = NULL;
}
bool SKPass::GetGShader(MaterialShaderPara& MSPara, SKShaderMap & ShaderMap, const SKUsedName &Name)
{
	if (!SKRenderer::ms_pRenderer->IsSupportGS())
	{
		return true;
	}
	SKGShader * pGeometryShader = NULL;

	unsigned int uiGShaderNum = 0;


	m_pGShaderSet = ShaderMap.GetShaderSet(Name);

	if (m_pGShaderSet)
	{

		uiGShaderNum = m_pGShaderSet->GetNum();
	}

	SKShaderKey::SetMaterialGShaderKey(&m_GShaderkey, MSPara, GetPassType());
	if (m_pMaterialInstance->m_pCurGShader[GetPassType()] == NULL)
	{
		if (m_pGShaderSet)
		{
			unsigned int uiIndex = m_pGShaderSet->Find(m_GShaderkey);

			if (uiIndex != m_pGShaderSet->GetNum())
			{
				SKShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
				pGeometryShader = (SKGShader *)(pTemp);
			}

		}
		if (pGeometryShader == NULL)
		{
			pGeometryShader = SKResourceManager::CreateGShader(MSPara, GetPassType(), uiGShaderNum);
			if (!pGeometryShader)
			{
				return 0;
			}
			if (m_pGShaderSet)
			{
				m_pGShaderSet->AddElement(m_GShaderkey, pGeometryShader);
			}
		}
		m_pMaterialInstance->m_pCurGShader[GetPassType()] = pGeometryShader;
	}
	else
	{
		if (m_pMaterialInstance->m_pCurGShader[GetPassType()]->m_ShaderKey == m_GShaderkey)
		{

		}
		else
		{

			if (m_pGShaderSet)
			{
				unsigned int uiIndex = m_pGShaderSet->Find(m_GShaderkey);

				if (uiIndex != m_pGShaderSet->GetNum())
				{
					SKShader * pTemp = (*m_pGShaderSet)[uiIndex].Value;
					pGeometryShader = (SKGShader *)(pTemp);
				}
			}
			if (pGeometryShader == NULL)
			{
				pGeometryShader = SKResourceManager::CreateGShader(MSPara, GetPassType(), uiGShaderNum);
				if (!pGeometryShader)
				{
					return 0;
				}
				if (m_pGShaderSet)
				{
					m_pGShaderSet->AddElement(m_GShaderkey, pGeometryShader);
				}
			}
			m_pMaterialInstance->m_pCurGShader[GetPassType()] = pGeometryShader;
		}
	}

	if (!m_pGShaderSet)
	{
		ShaderMap.SetShader(Name, m_GShaderkey, pGeometryShader);
		m_pGShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
bool SKPass::GetVShader(MaterialShaderPara& MSPara,SKShaderMap & ShaderMap,const SKUsedName &Name)
{

	SKVShader * pVertexShader = NULL;

	unsigned int uiVShaderNum = 0;


	m_pVShaderSet = ShaderMap.GetShaderSet(Name);	
	
	if (m_pVShaderSet)
	{

		uiVShaderNum = m_pVShaderSet->GetNum();
	}

	SKShaderKey::SetMaterialVShaderKey(&m_VShaderkey,MSPara,GetPassType());
	if (m_pMaterialInstance->m_pCurVShader[GetPassType()] == NULL)
	{
		if (m_pVShaderSet)
		{
			unsigned int uiIndex = m_pVShaderSet->Find(m_VShaderkey);

			if (uiIndex != m_pVShaderSet->GetNum())
			{
				SKShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
				pVertexShader = (SKVShader *)(pTemp);
			}

		}
		if (pVertexShader == NULL)
		{
			pVertexShader = SKResourceManager::CreateVShader(MSPara,GetPassType(),uiVShaderNum);
			if (!pVertexShader)
			{
				return 0;
			}
			if (m_pVShaderSet)
			{
				m_pVShaderSet->AddElement(m_VShaderkey,pVertexShader);
			}
		}
		m_pMaterialInstance->m_pCurVShader[GetPassType()] = pVertexShader;
	}
	else
	{
		if (m_pMaterialInstance->m_pCurVShader[GetPassType()]->m_ShaderKey == m_VShaderkey)
		{

		}
		else
		{

			if (m_pVShaderSet)
			{
				unsigned int uiIndex = m_pVShaderSet->Find(m_VShaderkey);

				if (uiIndex != m_pVShaderSet->GetNum())
				{
					SKShader * pTemp = (*m_pVShaderSet)[uiIndex].Value;
					pVertexShader = (SKVShader *)(pTemp);
				}
			}
			if (pVertexShader == NULL)
			{
				pVertexShader = SKResourceManager::CreateVShader(MSPara,GetPassType(),uiVShaderNum);
				if (!pVertexShader)
				{
					return 0;
				}
				if (m_pVShaderSet)
				{
					m_pVShaderSet->AddElement(m_VShaderkey,pVertexShader);
				}
			}
			m_pMaterialInstance->m_pCurVShader[GetPassType()] = pVertexShader;
		}
	}

	if (!m_pVShaderSet)
	{
		ShaderMap.SetShader(Name,m_VShaderkey,pVertexShader);
		m_pVShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
bool SKPass::GetPShader(MaterialShaderPara& MSPara,SKShaderMap & ShaderMap,const SKUsedName &Name)
{

	unsigned int uiPShaderNum = 0;
	SKPShader * pPixelShader = NULL;


	m_pPShaderSet = ShaderMap.GetShaderSet(Name);
	
	if (m_pPShaderSet)
	{
		uiPShaderNum = m_pPShaderSet->GetNum();
	}

	SKShaderKey::SetMaterialPShaderKey(&m_PShaderkey,MSPara,GetPassType());

	if (m_pMaterialInstance->m_pCurPShader[GetPassType()] == NULL)
	{
		if (m_pPShaderSet)
		{
			unsigned int uiIndex = m_pPShaderSet->Find(m_PShaderkey);

			if (uiIndex != m_pPShaderSet->GetNum())
			{
				SKShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
				pPixelShader = (SKPShader *)(pTemp);
			}
		}
		if (pPixelShader == NULL)
		{
			pPixelShader = SKResourceManager::CreatePShader(MSPara,GetPassType(),uiPShaderNum);
			if (!pPixelShader)
			{
				return 0;
			}
			if (m_pPShaderSet)
			{
				m_pPShaderSet->AddElement(m_PShaderkey,pPixelShader);
			}
		}
		m_pMaterialInstance->m_pCurPShader[GetPassType()] = pPixelShader;
	}
	else
	{
		if (m_pMaterialInstance->m_pCurPShader[GetPassType()]->m_ShaderKey == m_PShaderkey)
		{

		}
		else
		{
			if (m_pPShaderSet)
			{
				unsigned int uiIndex = m_pPShaderSet->Find(m_PShaderkey);

				if (uiIndex != m_pPShaderSet->GetNum())
				{
					SKShader * pTemp = (*m_pPShaderSet)[uiIndex].Value;
					pPixelShader = (SKPShader *)(pTemp);
				}
			}
			if (pPixelShader == NULL)
			{
				pPixelShader = SKResourceManager::CreatePShader(MSPara,GetPassType(),uiPShaderNum);
				if (!pPixelShader)
				{
					return 0;
				}
				if (m_pPShaderSet)
				{
					m_pPShaderSet->AddElement(m_PShaderkey,pPixelShader);
				}
			}
			m_pMaterialInstance->m_pCurPShader[GetPassType()] = pPixelShader;
		}
	}

	if (!m_pPShaderSet)
	{
		ShaderMap.SetShader(Name,m_PShaderkey,pPixelShader);
		m_pPShaderSet = ShaderMap.GetShaderSet(Name);
	}
	return 1;
}
