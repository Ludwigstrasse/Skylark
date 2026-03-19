#include "SKMaterialPass.h"
#include "SKSpatial.h"
#include "SKBoneNode.h"
#include "SKShaderStringFactory.h"
#include "SKConstValue.h"
#include "SKGeometry.h"
#include "SKBoneNode.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKMaterial.h"
#include "SKSortLight.h"
#include "SKProfiler.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKMaterialPass,SKPass)
BEGIN_ADD_PROPERTY(SKMaterialPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMaterialPass)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(MaterialRenderPassDraw, )
#define  MAX_DYNAMIC_LIGHT 3
SKMaterialPass::SKMaterialPass()
{

	

}
SKMaterialPass::~SKMaterialPass()
{


}
bool SKMaterialPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(MaterialRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance || !m_Light.GetNum())
	{
		m_Light.Clear();
		return 0;
	}

	SKMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}


	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (SKGeometry *)m_pSpatial;
	MSPara.LightArray = m_Light;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	SKArray<SKLight*> LightTemp;

	
	m_RenderSceondPassUsed = pMaterial->GetRenderState(m_uiPassId);

	SKBlendDesc BlendDesc = m_RenderSceondPassUsed.GetBlendState()->GetBlendDesc();
	if (BlendDesc.bBlendEnable[0] == true)
	{
		BlendDesc.ucDestBlend[0] = SKBlendDesc::BP_ONE;
	}
	else
	{
		BlendDesc.ucSrcBlend[0] = SKBlendDesc::BP_ONE;
		BlendDesc.ucDestBlend[0] = SKBlendDesc::BP_ONE;
	}
	BlendDesc.bBlendEnable[0] = true;		
	BlendDesc.bAlphaBlendEnable[0] = false;

	SKBlendState * pBlendState = SKResourceManager::CreateBlendState(BlendDesc);
	m_RenderSceondPassUsed.SetBlendState(pBlendState);




	for (unsigned int i = 0 ; i < m_Light.GetNum() ; i++)
	{
		LightTemp.AddElement(m_Light[i]);
		if (LightTemp.GetNum() >= MAX_DYNAMIC_LIGHT || i == m_Light.GetNum() - 1)
		{
			MSPara.LightArray = LightTemp;

			m_VShaderkey.Clear();
			m_PShaderkey.Clear();

			if (!GetVShader(MSPara, SKResourceManager::GetInnerVertexShaderMap(), SKUsedName::ms_cMaterialVertex))
			{
				m_Light.Clear();
				return 0;
			}
			if (!GetPShader(MSPara,SKResourceManager::GetMaterialShaderMap(),pMaterial->GetResourceName()))
			{
				m_Light.Clear();
				return 0;
			}


			pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
			pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);


			pMaterial->SetGlobleValue(this,m_uiPassId,m_pMaterialInstance->m_pCurVShader[GetPassType()],m_pMaterialInstance->m_pCurPShader[GetPassType()]);


			if(!pRenderer->DrawMesh((SKGeometry *)m_pSpatial,&m_RenderSceondPassUsed,
				m_pMaterialInstance->m_pCurVShader[GetPassType()],
				m_pMaterialInstance->m_pCurPShader[GetPassType()],
				m_pMaterialInstance->m_pCurGShader[GetPassType()]))
			{
				return false;
			}



			LightTemp.Clear();
		}

	}
	

	
	m_Light.Clear();
	return 1;
}

