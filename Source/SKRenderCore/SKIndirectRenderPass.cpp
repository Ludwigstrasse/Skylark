#include "SKIndirectRenderPass.h"
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
#include "SKSkyLight.h"
#include "SKProfiler.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKIndirectRenderPass,SKPass)
BEGIN_ADD_PROPERTY(SKIndirectRenderPass,SKPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKIndirectRenderPass)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(IndirectRenderPassDraw, )
SKIndirectRenderPass::SKIndirectRenderPass()
{



}
SKIndirectRenderPass::~SKIndirectRenderPass()
{

}
bool SKIndirectRenderPass::Draw(SKRenderer * pRenderer)
{
	ADD_TIME_PROFILE(IndirectRenderPassDraw)
	if(!pRenderer || !m_pCamera || !m_pSpatial || !m_pMaterialInstance)
	{
		m_Light.Clear();
		return 0;
	}

	SKMaterial * pMaterial = m_pMaterialInstance->GetMaterial();
	if (!pMaterial)
	{
		return 0;
	}
	
	SKColorRGBA SkyLightUpColor = SKColorRGBA(0.0f,0.0f,0.0f,0.0f);
	SKColorRGBA SkyLightDownColor = SKColorRGBA(0.0f,0.0f,0.0f,0.0f);

	for (unsigned int i = 0 ; i < m_Light.GetNum() ; i++)
	{
		if (m_Light[i]->GetLightType() == SKLight::LT_SKY)
		{
			SkyLightUpColor += ((SKSkyLight *)m_Light[i])->m_UpColor;
			SkyLightDownColor +=((SKSkyLight *)m_Light[i])->m_DownColor;
		}
	}

	MSPara.pCamera = m_pCamera;
	MSPara.pGeometry = (SKGeometry *)m_pSpatial;
	MSPara.LightArray = m_Light;
	MSPara.pMaterialInstance = m_pMaterialInstance;
	MSPara.uiPassId = m_uiPassId;
	MSPara.m_SkyLightUpColor = SkyLightUpColor;
	MSPara.m_SkyLightDownColor = SkyLightDownColor;

	
	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	if (!GetVShader(MSPara,SKResourceManager::GetInnerVertexShaderMap(),SKUsedName::ms_cMaterialVertex))
	{
		m_Light.Clear();
		return 0;
	}
	if (!GetPShader(MSPara,SKResourceManager::GetIndirectShaderMap(),pMaterial->GetResourceName()))
	{
		m_Light.Clear();
		return 0;
	}


	pRenderer->SetMaterialVShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,GetPassType(),m_pMaterialInstance->m_pCurPShader[GetPassType()]);




	pMaterial->SetGlobleValue(this,m_uiPassId,m_pMaterialInstance->m_pCurVShader[GetPassType()],m_pMaterialInstance->m_pCurPShader[GetPassType()]);

	if(!pRenderer->DrawMesh((SKGeometry *)m_pSpatial,&pMaterial->GetRenderState(m_uiPassId),
		m_pMaterialInstance->m_pCurVShader[GetPassType()],
		m_pMaterialInstance->m_pCurPShader[GetPassType()],
		m_pMaterialInstance->m_pCurGShader[GetPassType()]))
	{
		return false;
	}
	


	m_Light.Clear();
	return 1;
}

