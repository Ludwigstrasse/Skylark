#include "SKSceneRender.h"
#include "SKGraphicInclude.h"
#include "SKNormalDepthPass.h"
#include "SKPrezPass.h"
#include "SKMaterial.h"
#include "SKMeshDate.h"
#include "SKProfiler.h"
using namespace SKEngine2;
SKSceneRenderInterface::SKSceneRenderInterface()
{
	m_uiClearFlag = SKRenderer::CF_USE_MAX;
	m_ClearColorRGBA = SKColorRGBA(0.0f,0.0f,0.0f,1.0f);
	m_fClearDepth = 1.0f;
	m_uiClearStencil = 0;
	
	m_bUseViewPortClear = false;
	m_uiRTWidth = 0;
	m_uiRTHeight = 0;
}
SKSceneRenderInterface::~SKSceneRenderInterface()
{
	m_pTargetArray.Clear();
}
SKRenderTarget * SKSceneRenderInterface::GetRenderTarget(unsigned int uiIndex)
{
	if (uiIndex >= m_pTargetArray.GetNum())
	{
		return NULL;
	}
	return m_pTargetArray[uiIndex];
}

bool SKSceneRenderInterface::AddRenderTarget(SKRenderTarget * pTarget)
{
	for (unsigned int i = 0 ; i < m_pTargetArray.GetNum() ; i++)
	{
		if (pTarget == m_pTargetArray[i])
		{
			return false;
		}
	}
	if (!pTarget)
	{
		return false;
	}
	if (!pTarget->GetWidth() || !pTarget->GetHeight())
	{
		return false;
	}
	if (!m_uiRTWidth && !m_uiRTHeight)
	{
		m_uiRTWidth = pTarget->GetWidth();
		m_uiRTHeight = pTarget->GetHeight();
		m_uiMulSample = pTarget->GetMulSample();
	}
	else if (m_uiRTHeight && m_uiRTWidth)
	{
		if (m_uiRTWidth != pTarget->GetWidth() || m_uiRTHeight != pTarget->GetHeight() || m_uiMulSample != pTarget->GetMulSample())
		{
			SKMAC_ASSERT(false);
			return false;
		}
	}
	else
	{
		SKMAC_ASSERT(false);
		return false;
	}

	m_pTargetArray.AddElement(pTarget);

	return true;
}
void SKSceneRenderInterface::SetRenderTargets()
{

	SKRenderTarget * pRenderTarget[16] = { NULL };
	unsigned int uiRTNum = m_pTargetArray.GetNum() > SKRenderer::ms_pRenderer->GetMaxRTNum() ? SKRenderer::ms_pRenderer->GetMaxRTNum() : m_pTargetArray.GetNum();

	for (unsigned int i = 0; i < uiRTNum; i++)
	{
		pRenderTarget[i] = m_pTargetArray[i];
		//SKRenderer::ms_pRenderer->SetRenderTarget(m_pTargetArray[i],i);
	}
	if (uiRTNum > 0)
	{
		SKRenderer::ms_pRenderer->SetRenderTargets(pRenderTarget, uiRTNum);
	}

}
void SKSceneRenderInterface::EndRenderTargets()
{
	SKRenderTarget * pRenderTarget[16] = { NULL };
	unsigned int uiRTNum = m_pTargetArray.GetNum() > SKRenderer::ms_pRenderer->GetMaxRTNum() ? SKRenderer::ms_pRenderer->GetMaxRTNum() : m_pTargetArray.GetNum();

	for (unsigned int i = 0; i < uiRTNum; i++)
	{
		pRenderTarget[i] = m_pTargetArray[i];
		//SKRenderer::ms_pRenderer->EndRenderTarget(m_pTargetArray[i],i);
	}
	if (uiRTNum > 0)
	{
		SKRenderer::ms_pRenderer->EndRenderTargets(pRenderTarget, uiRTNum);
	}
}
SKSceneRender::SKSceneRender()
{
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
	m_uiRenderStateInheritFlag = 0;
}
SKSceneRender::~SKSceneRender()
{
	
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
}
void SKSceneRender::SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_UseState.GetAll(&RenderState);
	m_uiRenderStateInheritFlag = uiRenderStateInheritFlag;
}
void SKSceneRender::ClearUseState()
{
	m_uiRenderStateInheritFlag = 0;
}
bool SKSceneRender::SetDepthStencil(SKDepthStencil * pDepthStencil,unsigned int uiRenderGroup)
{
	if (uiRenderGroup >= SKCuller::RG_MAX)
	{
		return false;
	}
	if(pDepthStencil)
	{
		if (!pDepthStencil->GetWidth() || !pDepthStencil->GetHeight())
		{
			return false;
		}
		if (!m_uiRTHeight && !m_uiRTWidth)
		{
			m_uiRTWidth = pDepthStencil->GetWidth();
			m_uiRTHeight = pDepthStencil->GetHeight();
			m_uiMulSample = pDepthStencil->GetMulSample();
		}
		else if (m_uiRTHeight && m_uiRTWidth)
		{
			if (m_uiRTWidth != pDepthStencil->GetWidth() || m_uiRTHeight != pDepthStencil->GetHeight() || m_uiMulSample != pDepthStencil->GetMulSample())
			{
				SKMAC_ASSERT(false);
				return false;
			}
		}
		else
		{
			SKMAC_ASSERT(false);
			return false;
		}
	}
	
	
	
	
	m_pDepthStencil[uiRenderGroup] = pDepthStencil;
	return true;
}
void SKSceneRender::ClearRTAndDepth()
{
	m_pTargetArray.Clear();
	m_uiRTWidth = 0;
	m_uiRTHeight = 0;
	for (unsigned int i = 0 ; i < SKCuller::RG_MAX ; i++)
	{
		m_pDepthStencil[i] = NULL;
	}
}
void SKSceneRender::DrawUseCurView(SKCuller & Culler,double dAppTime)
{

}
void SKSceneRender::DisableUseCurView(SKCuller & Culler,double dAppTime)
{

}
bool SKSceneRender::Draw(SKCuller & Culler,double dAppTime)
{

	if (m_uiClearFlag <= SKRenderer::CF_USE_MAX)
	{

		SKColorRGBA ClearColorRGBA = SKRenderer::ms_pRenderer->GetClearColor();
		SKREAL fClearDepth = SKRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = SKRenderer::ms_pRenderer->GetClearStencil();

		m_SaveRenderState = SKRenderer::ms_pRenderer->GetUseState();
		unsigned int uiSaveRenderStateInheritFlag = SKRenderer::ms_pRenderer->GetRenderStateInheritFlag();

		SKRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		if (m_uiRenderStateInheritFlag)
		{			
			SKRenderer::ms_pRenderer->SetUseState(m_UseState,m_uiRenderStateInheritFlag);		
		}

		if (Culler.GetCamera()->GetViewPortNum())
		{
			if (!m_bUseViewPortClear)
			{
				SetRenderTargets();
				SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag & SKRenderer::CF_COLOR);
				EndRenderTargets();
			}
			for (unsigned int i = 0 ; i< Culler.GetCamera()->GetViewPortNum() ;i++)
			{

				SKViewPort * pViewPort = Culler.GetCamera()->GetViewPort(i);
				unsigned int uiRtWidth = m_pTargetArray[0]->GetWidth();
				unsigned int uiRtHeight = m_pTargetArray[0]->GetHeight();
				unsigned int X = Rounding(pViewPort->XMin * uiRtWidth);
				unsigned int Y = Rounding(pViewPort->YMin * uiRtHeight);
				unsigned int Width = Rounding(pViewPort->XMax * uiRtWidth) - X;
				unsigned int Height = Rounding(pViewPort->YMax * uiRtHeight) - Y;

				
				
				SKREAL fAspect = Culler.GetCamera()->GetAspect();
				if (pViewPort && pViewPort->bChangeAspect)
				{	
					SKREAL NewRatio = (Width * 1.0f) / (Height) ;
					Culler.GetCamera()->SetAspect(NewRatio);
				}

				DrawUseCurView(Culler,dAppTime);

				SetRenderTargets();			
				SKRenderer::ms_pRenderer->SetViewPort(pViewPort);
				if (m_bUseViewPortClear)
				{		
					SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag & SKRenderer::CF_COLOR,X,Y,Width,Height);
				}
				for (unsigned int uiRenderGroup = 0 ; uiRenderGroup < SKCuller::RG_MAX ;uiRenderGroup++)
				{	
					if (!Culler.GetRenderGroupVisibleNum(uiRenderGroup))
					{
						continue;
					}
					if (m_pDepthStencil[uiRenderGroup])
					{
						SKRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
					
						if (!m_bUseViewPortClear)
						{
							SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL));
						}
						else
						{
							SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL),X,Y,Width,Height);
						}
						OnDraw(Culler,uiRenderGroup,dAppTime);
						SKRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
					}
					else
					{
						if (!m_bUseViewPortClear)
						{
							SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL));
						}
						else
						{
							SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL),X,Y,Width,Height);
						}
						OnDraw(Culler,uiRenderGroup,dAppTime);
					}
				}		
				EndRenderTargets();
				DisableUseCurView(Culler,dAppTime);
				if (pViewPort && pViewPort->bChangeAspect)
				{
					Culler.GetCamera()->SetAspect(fAspect);
				}
			}

		}
		else
		{
			DrawUseCurView(Culler,dAppTime);
			SetRenderTargets();
			SKRenderer::ms_pRenderer->SetViewPort(NULL);
			SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
			for (unsigned int uiRenderGroup = 0 ; uiRenderGroup < SKCuller::RG_MAX ;uiRenderGroup++)
			{
				if (!Culler.GetRenderGroupVisibleNum(uiRenderGroup))
				{
					continue;
				}
				if (m_pDepthStencil[uiRenderGroup])
				{
					SKRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
					SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL));									
					OnDraw(Culler,uiRenderGroup,dAppTime);
					SKRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
				}
				else
				{
					SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL));									
					OnDraw(Culler,uiRenderGroup,dAppTime);
				}
			}
			EndRenderTargets();
			DisableUseCurView(Culler,dAppTime);
		}
		
		SKRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);
		if (m_uiRenderStateInheritFlag)
		{
			SKRenderer::ms_pRenderer->SetUseState(m_SaveRenderState,uiSaveRenderStateInheritFlag);
		}
	}




	return true;
}
SKDebugDrawSceneRender::SKDebugDrawSceneRender()
{
	for (unsigned int i =0 ; i < SKCuller::RG_MAX ;i++)
	{
		m_pDebugDraw[i] = SK_NEW SKDebugDraw();

	}
}
SKDebugDrawSceneRender::~SKDebugDrawSceneRender()
{
	for (unsigned int i =0 ; i < SKCuller::RG_MAX ;i++)
	{
		m_pDebugDraw[i] = NULL;
	}
}
bool SKDebugDrawSceneRender::OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	if (m_pDebugDraw[uiRenderGroup])
	{
		m_pDebugDraw[uiRenderGroup]->DrawDebugInfo(Culler.GetCamera());
	}
	return true;
}
SKMaterialSceneRender::SKMaterialSceneRender()
{
	m_pNormalDepthTexture = NULL;
}
SKMaterialSceneRender::~SKMaterialSceneRender()
{

}
void SKMaterialSceneRender::DrawUseCurView(SKCuller & Culler,double dAppTime)
{
	for (unsigned int i = 0 ;i < Culler.GetLightNum() ; i++)
	{
		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(Culler.GetLight(i));
		if (pLocalLight)
		{
			pLocalLight->DrawDepandenceShadowMap(Culler,dAppTime);
			pLocalLight->DrawNoDepandenceShadowMap(Culler, dAppTime);
		}

	}
}
void SKMaterialSceneRender::DisableUseCurView(SKCuller & Culler,double dAppTime)
{
	for (unsigned int i = 0 ;i < Culler.GetLightNum() ; i++)
	{
		SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(Culler.GetLight(i));
		if (pLocalLight)
		{
			pLocalLight->DisableDepandenceShadowMap(Culler,dAppTime);
		}

	}
}
void SKMaterialSceneRender::GetGroup(SKCuller & Culler, unsigned int uiRenderGroup)
{
	m_NormalAndDepth.Clear();
	m_NoNormalOrDepth.Clear();
	m_Combine.Clear();
	m_AlphaBlend.Clear();
	for (unsigned int t = 0; t <= SKCuller::SKT_COMBINE; t++)
	{	
		for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
		{
			
			SKRenderContext& VisibleContext = Culler.GetVisibleSpatial(j, t, uiRenderGroup);
			SKGeometry * pGeometry = VisibleContext.m_pGeometry;
			SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;
			SKMaterial * pMaterial = pMaterialInstance->GetMaterial();
			if (t < SKCuller::SKT_ALPHABLEND)
			{
				const SKDepthStencilDesc & DepthStencilDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetDepthStencilState()->GetDepthStencilDesc();
				unsigned int uiNormalLevel = pGeometry->GetMeshDate()->GetVertexBuffer()->GetNormalLevel();
				// cull no write depth or no normal
				if (!DepthStencilDest.m_bDepthWritable || !uiNormalLevel)
				{
					m_NoNormalOrDepth.AddElement(&VisibleContext);
					continue;
				}
				else
				{
					m_NormalAndDepth.AddElement(&VisibleContext);
				}
			}
			else if (t == SKCuller::SKT_ALPHABLEND)
			{
				m_AlphaBlend.AddElement(&VisibleContext);
			}
			else if (t == SKCuller::SKT_COMBINE)
			{
				m_Combine.AddElement(&VisibleContext);
			}
		}
	}
}
void SKMaterialSceneRender::DrawGroup(SKCuller & Culler, unsigned int uiRenderGroup, SKArray<SKRenderContext *> & Group, double dAppTime)
{
	for (unsigned int i = 0; i < Group.GetNum(); i++)
	{
		SKRenderContext& VisibleContext = *Group[i];

		SKGeometry * pGeometry = VisibleContext.m_pGeometry;

		SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

		SKIndirectRenderPass * pIndirectRenderPass = pMaterialInstance->GetMaterial()->GetIndirectRenderPass();
		for (unsigned int l = 0; l < VisibleContext.m_pInDirectLight.GetNum(); l++)
		{
			pIndirectRenderPass->AddLight(VisibleContext.m_pInDirectLight[l]);
		}
		pIndirectRenderPass->SetPassId(VisibleContext.m_uiPassId);
		pIndirectRenderPass->SetSpatial(pGeometry);
		pIndirectRenderPass->SetMaterialInstance(pMaterialInstance);
		pIndirectRenderPass->SetCamera(Culler.GetCamera());
		pIndirectRenderPass->Draw(SKRenderer::ms_pRenderer);
	}
	if (&Group == &m_NormalAndDepth)
	{
		DrawVolumeShadow(Culler, uiRenderGroup, dAppTime);
	}
	
	for (unsigned int i = 0; i < Group.GetNum(); i++)
	{
		SKRenderContext& VisibleContext = *Group[i];

		SKGeometry * pGeometry = VisibleContext.m_pGeometry;

		SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

		SKMaterialPass * pMaterialPass = pMaterialInstance->GetMaterial()->GetMaterialPass();
		for (unsigned int l = 0; l < VisibleContext.m_pDirectLight.GetNum(); l++)
		{
			pMaterialPass->AddLight(VisibleContext.m_pDirectLight[l]);
		}
		pMaterialPass->SetPassId(VisibleContext.m_uiPassId);
		pMaterialPass->SetSpatial(pGeometry);
		pMaterialPass->SetMaterialInstance(pMaterialInstance);
		pMaterialPass->SetCamera(Culler.GetCamera());
		pMaterialPass->Draw(SKRenderer::ms_pRenderer);
	}
}
void SKMaterialSceneRender::DrawVolumeShadow(SKCuller & Culler, unsigned int uiRenderGroup, double dAppTime)
{
	if (uiRenderGroup == SKCuller::RG_NORMAL)
	{
		if (Culler.GetCamera()->GetViewPortNum())
		{
			return;
		}
		for (unsigned int i = 0; i < Culler.GetLightNum(); i++)
		{
			SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(Culler.GetLight(i));
			if (pLocalLight)
			{
				pLocalLight->DrawVolumeShadow(Culler, dAppTime);
			}
		}
	}
}
void SKMaterialSceneRender::DrawProjectShadow(SKCuller & Culler, unsigned int uiRenderGroup, double dAppTime)
{
	if (uiRenderGroup == SKCuller::RG_NORMAL && m_pNormalDepthTexture)
	{
		if (Culler.GetCamera()->GetViewPortNum())
		{
			return;
		}
		for (unsigned int i = 0; i < Culler.GetLightNum(); i++)
		{
			SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(Culler.GetLight(i));
			if (pLocalLight)
			{
				pLocalLight->DrawPorjectShadow(Culler, dAppTime, m_pNormalDepthTexture);
			}
		}
	}
}
bool SKMaterialSceneRender::OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	GetGroup(Culler, uiRenderGroup);
	DrawGroup(Culler, uiRenderGroup, m_NormalAndDepth, dAppTime);
	DrawProjectShadow(Culler, uiRenderGroup, dAppTime);
	DrawGroup(Culler, uiRenderGroup, m_NoNormalOrDepth, dAppTime);
	DrawGroup(Culler, uiRenderGroup, m_Combine, dAppTime);
	DrawGroup(Culler, uiRenderGroup, m_AlphaBlend, dAppTime);
	
	return true;
}
SKNormalDepthSceneRender::SKNormalDepthSceneRender()
{

}
SKNormalDepthSceneRender::~SKNormalDepthSceneRender()
{

}
bool SKNormalDepthSceneRender::OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	

	for (unsigned int t = 0 ; t < SKCuller::SKT_MAX ; t++)
	{
		// cull alpha geometry and combine
		if (t == SKCuller::SKT_ALPHABLEND || t == SKCuller::SKT_COMBINE)
		{
			continue;
		}
		for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
		{
			SKRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
			if (!VisibleContext.m_pGeometry || !VisibleContext.m_pMaterialInstance)
			{
				continue ;
			}
			SKMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
			if (!pMaterial)
			{
				continue;
			}
// 			// cull Combine Group no write depth
// 			const SKBlendDesc & BlendDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetBlendState()->GetBlendDesc();
// 			if (BlendDest.IsBlendUsed())
// 			{
// 				continue;
// 			}
			const SKDepthStencilDesc & DepthStencilDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetDepthStencilState()->GetDepthStencilDesc();

			SKGeometry * pGeometry = DynamicCast<SKGeometry>(VisibleContext.m_pGeometry);
			if (!pGeometry)
			{
				continue;
			}
			unsigned int uiNormalLevel = pGeometry->GetMeshDate()->GetVertexBuffer()->GetNormalLevel();
			// cull no write depth or no normal
			if (!DepthStencilDest.m_bDepthWritable || !uiNormalLevel)
			{
				continue;
			}
			

			SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;			
			SKNormalDepthPass * pNormalDepthPass = pMaterialInstance->GetMaterial()->GetNormalDepthPass();
			pNormalDepthPass->SetPassId(VisibleContext.m_uiPassId);
			pNormalDepthPass->SetSpatial(pGeometry);
			pNormalDepthPass->SetMaterialInstance(pMaterialInstance);	
			pNormalDepthPass->SetCamera(Culler.GetCamera());
			pNormalDepthPass->Draw(SKRenderer::ms_pRenderer);
		}
	}
	return true;
}

SKPrezSceneRender::SKPrezSceneRender()
{

}
SKPrezSceneRender::~SKPrezSceneRender()
{

}
bool SKPrezSceneRender::OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	//PreZ
	for (unsigned int t = 0 ; t < SKCuller::SKT_MAX ; t++)
	{
		// cull alpha geometry
		if (t == SKCuller::SKT_ALPHABLEND)
		{
			continue;
		}
		for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
		{
			SKRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
			if (!VisibleContext.m_pGeometry || !VisibleContext.m_pMaterialInstance)
			{
				continue ;
			}
			SKMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
			if (!pMaterial)
			{
				continue;
			}

			//cull geometry , the SKT_COMBINE has alpha 
			const SKBlendDesc & BlendDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetBlendState()->GetBlendDesc();
			if (BlendDest.IsBlendUsed())
			{
				continue;
			}
			const SKDepthStencilDesc & DepthStencilDest = pMaterial->GetRenderState(VisibleContext.m_uiPassId).GetDepthStencilState()->GetDepthStencilDesc();

			SKGeometry * pGeometry = VisibleContext.m_pGeometry;


			// cull no write depth
			if (!DepthStencilDest.m_bDepthWritable)
			{
				continue;
			}

			SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;			

			SKPrezPass * pPrezPass = SKPrezPass::GetDefault();
			pPrezPass->SetSpatial(pGeometry);
			pPrezPass->SetMaterialInstance(pMaterialInstance);	
			pPrezPass->SetCamera(Culler.GetCamera());
			pPrezPass->Draw(SKRenderer::ms_pRenderer);
		}
	}
	return true;
}
SKPostEffectSceneRender::SKPostEffectSceneRender()
{
	m_pTexAllState = SK_NEW SKTexAllState();
	m_pTexAllState->SetSamplerState((SKSamplerState*)SKSamplerState::GetDoubleLine());
	m_uiClearFlag = SKRenderer::CF_COLOR;
	m_pDepthStencil = NULL;
	m_pCustomMaterial = NULL;
}
SKPostEffectSceneRender::~SKPostEffectSceneRender()
{
	m_pDepthStencil = NULL;
	m_pTexAllState = NULL;
	m_pCustomMaterial = NULL;
}
bool SKPostEffectSceneRender::OnDraw(SKCuller & Culler)
{
	SKMAC_ASSERT(m_pCustomMaterial);

	if (!m_uiRTWidth || !m_uiRTHeight)
	{
		return false;
	}

	SKREAL Inv_Width = 1.0f / m_uiRTWidth;
	SKREAL Inv_Height = 1.0f / m_uiRTHeight;
	m_pCustomMaterial->SetVShaderValue(SKUsedName::ms_cPostInv_Width,&Inv_Width);
	m_pCustomMaterial->SetVShaderValue(SKUsedName::ms_cPostInv_Height,&Inv_Height);
	if(!m_pCustomMaterial->UseMaterial())
		return false;
	SKRenderer::ms_pRenderer->DrawScreen(NULL);

	m_pTexAllState->m_pTex = NULL;
	return true;
}
bool SKPostEffectSceneRender::SetDepthStencil(SKDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
	{
		return false;
	}
	if (!pDepthStencil->GetWidth() || !pDepthStencil->GetHeight())
	{
		return false;
	}
	if (m_uiRTHeight && m_uiRTWidth)
	{
		if (m_uiRTWidth != pDepthStencil->GetWidth() || m_uiRTHeight != pDepthStencil->GetHeight())
		{
			return false;
		}
	}
	else
	{	
		m_uiRTWidth = pDepthStencil->GetWidth();
		m_uiRTHeight = pDepthStencil->GetHeight();
	}
	m_pDepthStencil = pDepthStencil;
	return true;
}
void SKPostEffectSceneRender::GetRT(unsigned int uiWidth,unsigned int uiHeight)
{
	m_pTargetArray.Clear();
	m_pDepthStencil = NULL;

	SKRenderTarget * pRenderTarget = SKResourceManager::Get2DRenderTarget(uiWidth,uiHeight,SKRenderer::SFT_A8R8G8B8,0);
	AddRenderTarget(pRenderTarget);

	m_uiRTHeight = uiHeight;
	m_uiRTWidth = uiWidth;
}
void SKPostEffectSceneRender::SetNoUseRTRenderSize(unsigned int uiWidth,unsigned int uiHeight)
{
	m_uiRTHeight = uiHeight;
	m_uiRTWidth = uiWidth;
}
void SKPostEffectSceneRender::DisableRT()
{
	for (unsigned int i = 0 ; i < m_pTargetArray.GetNum() ;i++)
	{
		if (m_pTargetArray[i])
		{
			SKResourceManager::Disable2DRenderTarget(m_pTargetArray[i]);
		}
		
	}
	
}
void SKPostEffectSceneRender::SetSourceTarget(SKTexture * pTexture)
{
	m_pTexAllState->m_pTex = pTexture;
}
void SKPostEffectSceneRender::ClearRTAndDepth()
{
	m_pTargetArray.Clear();
	m_uiRTWidth = 0;
	m_uiRTHeight = 0;

	m_pDepthStencil = NULL;

}
bool SKPostEffectSceneRender::Draw(SKCuller & Culler,double dAppTime)
{

	SetRenderTargets();

	if (m_pDepthStencil)
	{
		if(!SKRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil))
		{
			SKMAC_ASSERT(0);
			return false;
		}
	}

	if (m_uiClearFlag > 0 && m_uiClearFlag <= SKRenderer::CF_USE_MAX)
	{

		SKColorRGBA ClearColorRGBA = SKRenderer::ms_pRenderer->GetClearColor();
		SKREAL fClearDepth = SKRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = SKRenderer::ms_pRenderer->GetClearStencil();


		SKRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);


		
		SKRenderer::ms_pRenderer->SetViewPort(NULL);
		SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler);
		


		SKRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);

	}


	if (m_pDepthStencil)
	{
		if(!SKRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil))
		{
			SKMAC_ASSERT(0);
			return false;
		}
	}
	EndRenderTargets();
	return true;
}
SKPEBlurSceneRender::SKPEBlurSceneRender()
{
	
};
SKPEBlurSceneRender::~SKPEBlurSceneRender()
{

};
SKPESSRSceneRender::SKPESSRSceneRender()
{
	m_pCustomMaterial = SKCustomMaterial::GetSSR();
	m_pNormalDepthTexture = SK_NEW SKTexAllState();
	m_pNormalDepthTexture->SetSamplerState((SKSamplerState*)SKSamplerState::GetShadowMapSampler());
	m_pSceneColorTexture = SK_NEW SKTexAllState();
	m_pSceneColorTexture->SetSamplerState((SKSamplerState*)SKSamplerState::GetBorderARGB0Sampler());
};
SKPESSRSceneRender::~SKPESSRSceneRender()
{
	m_pNormalDepthTexture = NULL;
	m_pSceneColorTexture = NULL;
};
void SKPESSRSceneRender::SetUseTexture(SKTexture * pNormalDepthTexture, SKTexture *pSceneColorTexture)
{
	m_pNormalDepthTexture->m_pTex = pNormalDepthTexture;
	m_pSceneColorTexture->m_pTex = pSceneColorTexture;

	static SKUsedName NormalDepthSamplerString = _T("NormalDepthSampler");
	static SKUsedName SceneColorSamplerString = _T("SceneColorSampler");

	m_pCustomMaterial->SetPShaderTexture(NormalDepthSamplerString,m_pNormalDepthTexture);
	m_pCustomMaterial->SetPShaderTexture(SceneColorSamplerString,m_pSceneColorTexture);
}
bool SKPESSRSceneRender::OnDraw(SKCuller & Culler)
{
	SKMAC_ASSERT(m_pCustomMaterial);

	if (!m_uiRTWidth || !m_uiRTHeight)
	{
		return false;
	}

	SKREAL Inv_Width = 1.0f / m_uiRTWidth;
	SKREAL Inv_Height = 1.0f / m_uiRTHeight;
	m_pCustomMaterial->SetVShaderValue(SKUsedName::ms_cPostInv_Width,&Inv_Width);
	m_pCustomMaterial->SetVShaderValue(SKUsedName::ms_cPostInv_Height,&Inv_Height);
	
	static SKUsedName ProjectMatrixString = _T("ProjectMatrix");
	SKMatrix3X3W ProjectMatrix = Culler.GetCamera()->GetProjMatrix();
	m_pCustomMaterial->SetPShaderValue(ProjectMatrixString,&ProjectMatrix);



	static SKUsedName InvRtWidthString = _T("InvRtWidth");
	unsigned int uiWidth = m_pNormalDepthTexture->m_pTex->GetWidth(0);
	unsigned int uiHeight = m_pNormalDepthTexture->m_pTex->GetHeight(0);
	SKVector2 InvRtWidth = SKVector2(1.0f / uiWidth , 1.0f / uiHeight);
	m_pCustomMaterial->SetPShaderValue(InvRtWidthString, &InvRtWidth);


	static SKUsedName FarZString = _T("FarZ");
	SKREAL FarZ = Culler.GetCamera()->GetZFar();
	m_pCustomMaterial->SetPShaderValue(FarZString, &FarZ);

	if(!m_pCustomMaterial->UseMaterial())
		return false;
	SKRenderer::ms_pRenderer->DrawScreen(NULL);

	m_pNormalDepthTexture->m_pTex = NULL;
	m_pSceneColorTexture->m_pTex = NULL;
	return true;
}

SKPEGraySceneRender::SKPEGraySceneRender()
{
	m_pCustomMaterial = SKCustomMaterial::GetPostGray();
};
SKPEGraySceneRender::~SKPEGraySceneRender()
{

};
void SKPEGraySceneRender::SetSourceTarget(SKTexture * pTexture)
{
	SKPostEffectSceneRender::SetSourceTarget(pTexture);
	m_pCustomMaterial->SetPShaderTexture(SKUsedName::ms_cPostInputTexture,m_pTexAllState);
}



SKGammaCorrectSceneRender::SKGammaCorrectSceneRender()
{
	m_pCustomMaterial = SKCustomMaterial::GetGammaCorrect();
};
SKGammaCorrectSceneRender::~SKGammaCorrectSceneRender()
{

};
void SKGammaCorrectSceneRender::SetSourceTarget(SKTexture * pTexture)
{
	SKPostEffectSceneRender::SetSourceTarget(pTexture);
	m_pCustomMaterial->SetPShaderTexture(SKUsedName::ms_cPostInputTexture,m_pTexAllState);
}


SKPEScreenQuadSceneRender::SKPEScreenQuadSceneRender()
{
	m_pCustomMaterial = SKCustomMaterial::GetPostScreenQuad();

};
SKPEScreenQuadSceneRender::~SKPEScreenQuadSceneRender()
{
	
};
void SKPEScreenQuadSceneRender::SetSourceTarget(SKTexture * pTexture)
{
	SKPostEffectSceneRender::SetSourceTarget(pTexture);
	m_pCustomMaterial->SetPShaderTexture(SKUsedName::ms_cPostInputTexture,m_pTexAllState);
}

SKPEMaterialSceneRender::SKPEMaterialSceneRender()
{


};
SKPEMaterialSceneRender::~SKPEMaterialSceneRender()
{

};
bool SKPEMaterialSceneRender::OnDraw(SKCuller & Culler)
{
	SKMaterial * pMaterial = m_pMaterialInstacne->GetMaterial();
	SKPostEffectPass * pPostEffectPass = pMaterial->GetPostEffectPass();
	pPostEffectPass->SetPassId(0);
	pPostEffectPass->SetMaterialInstance(m_pMaterialInstacne);
	pPostEffectPass->SetCamera(NULL);
	pPostEffectPass->SetSpatial(SKGeometry::GetDefaultQuad());
	pPostEffectPass->m_uiRTWidth = m_uiRTWidth;
	pPostEffectPass->m_uiRTHeight = m_uiRTHeight;
	pPostEffectPass->m_PColorBuffer = m_pTexAllState;
	pPostEffectPass->Draw(SKRenderer::ms_pRenderer);
	m_pTexAllState->m_pTex = NULL;
	return true;
}
SKPEVolumeShadowMapSceneRender::SKPEVolumeShadowMapSceneRender()
{
	m_pCustomMaterial = SKCustomMaterial::GetPostVolumeShadowMap();

};
SKPEVolumeShadowMapSceneRender::~SKPEVolumeShadowMapSceneRender()
{
	
};


SKShadowMapSceneRender::SKShadowMapSceneRender(unsigned int uiShadowMapType)
{

	m_uiShadowMapType = uiShadowMapType;
}
SKShadowMapSceneRender::~SKShadowMapSceneRender()
{

}
bool SKShadowMapSceneRender::Draw(SKCuller & Culler,double dAppTime)
{
	if (m_uiClearFlag <= SKRenderer::CF_USE_MAX)
	{

		SKColorRGBA ClearColorRGBA = SKRenderer::ms_pRenderer->GetClearColor();
		SKREAL fClearDepth = SKRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = SKRenderer::ms_pRenderer->GetClearStencil();


		SKRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);
	
		SetRenderTargets();
		SKRenderer::ms_pRenderer->SetViewPort(NULL);
		SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag & SKRenderer::CF_COLOR);
		for (unsigned int uiRenderGroup = 0 ; uiRenderGroup < SKCuller::RG_MAX ;uiRenderGroup++)
		{
			if (!Culler.GetRenderGroupVisibleNum(uiRenderGroup))
			{
				continue;
			}
			if (m_pDepthStencil[uiRenderGroup])
			{
				SKRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
				SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL));									
				OnDraw(Culler,uiRenderGroup,dAppTime);
				SKRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil[uiRenderGroup]);
			}
			else
			{
				SKRenderer::ms_pRenderer->ClearBuffers((m_uiClearFlag & SKRenderer::CF_DEPTH) | (m_uiClearFlag & SKRenderer::CF_STENCIL));									
				OnDraw(Culler,uiRenderGroup,dAppTime);
			}
		}
		EndRenderTargets();
 
		SKRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);
	}

	return true;
}
bool SKShadowMapSceneRender::OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
	
	if (uiRenderGroup != SKCuller::RG_NORMAL)
	{
		return true;
	}
	if(m_uiShadowMapType == SMT_CUB)
	{
		for (unsigned int t = 0 ; t < SKCuller::SKT_MAX ; t++)
		{
			for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
			{

				SKRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
				if (!VisibleContext.m_pGeometry || !VisibleContext.m_pMaterialInstance)
				{
					continue ;
				}
				SKMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
				if (!pMaterial)
				{
					continue;
				}

				SKGeometry * pGeometry = VisibleContext.m_pGeometry;




				SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

				SKCubeShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetCubeShadowPass();
				pShadowPass->m_pPointLight = (SKPointLight *)m_pLocalLight;

				pShadowPass->SetPassId(VisibleContext.m_uiPassId);
				pShadowPass->SetSpatial(pGeometry);
				pShadowPass->SetMaterialInstance(pMaterialInstance);	
				pShadowPass->SetCamera(Culler.GetCamera());
				pShadowPass->Draw(SKRenderer::ms_pRenderer);

			}
		}
	}
	else if (m_uiShadowMapType == SMT_SHADOWMAP)
	{
		for (unsigned int t = 0 ; t < SKCuller::SKT_MAX ; t++)
		{
			for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
			{

				SKRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
				if (!VisibleContext.m_pGeometry || !VisibleContext.m_pMaterialInstance)
				{
					continue ;
				}
				SKMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
				if (!pMaterial)
				{
					continue;
				}
				SKGeometry * pGeometry = VisibleContext.m_pGeometry;


				SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

				SKShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetShadowMapPass();
				pShadowPass->m_pLocalLight = m_pLocalLight;

				pShadowPass->SetPassId(VisibleContext.m_uiPassId);
				pShadowPass->SetSpatial(pGeometry);
				pShadowPass->SetMaterialInstance(pMaterialInstance);	
				pShadowPass->SetCamera(Culler.GetCamera());
				pShadowPass->Draw(SKRenderer::ms_pRenderer);

			}
		}
	}
	else if (m_uiShadowMapType == SMT_DUAL_PARABOLOID)
	{
		for (unsigned int t = 0 ; t < SKCuller::SKT_MAX ; t++)
		{
			for(unsigned int j = 0; j < Culler.GetVisibleNum(t,uiRenderGroup) ; j++)
			{

				SKRenderContext& VisibleContext = Culler.GetVisibleSpatial(j,t,uiRenderGroup);
				if (!VisibleContext.m_pGeometry || !VisibleContext.m_pMaterialInstance)
				{
					continue ;
				}
				SKMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
				if (!pMaterial)
				{
					continue;
				}

				SKGeometry * pGeometry = VisibleContext.m_pGeometry;
	


				SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

				SKDualParaboloidShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetDualParaboloidShadowPass();
				pShadowPass->m_pPointLight = (SKPointLight *)m_pLocalLight;

				pShadowPass->SetPassId(VisibleContext.m_uiPassId);
				pShadowPass->SetSpatial(pGeometry);
				pShadowPass->SetMaterialInstance(pMaterialInstance);	
				pShadowPass->SetCamera(Culler.GetCamera());
				pShadowPass->Draw(SKRenderer::ms_pRenderer);

			}
		}
	}

	return true;
}
SKLightMaterialSceneRender::SKLightMaterialSceneRender()
{

}
SKLightMaterialSceneRender::~SKLightMaterialSceneRender()
{

}
void SKLightMaterialSceneRender::ClearRTAndDepth()
{
	m_pTargetArray.Clear();

	m_pDepthStencil = NULL;

}
bool SKLightMaterialSceneRender::Draw(SKCuller & Culler,double dAppTime)
{
	SetRenderTargets();

	if (m_pDepthStencil)
	{
		if(!SKRenderer::ms_pRenderer->SetDepthStencilBuffer(m_pDepthStencil))
		{
			SKMAC_ASSERT(0);
			return false;
		}
	}

	if (m_uiClearFlag > 0 && m_uiClearFlag <= SKRenderer::CF_USE_MAX)
	{

		SKColorRGBA ClearColorRGBA = SKRenderer::ms_pRenderer->GetClearColor();
		SKREAL fClearDepth = SKRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = SKRenderer::ms_pRenderer->GetClearStencil();


		unsigned int uiSaveRenderStateInheritFlag = SKRenderer::ms_pRenderer->GetRenderStateInheritFlag();

		SKRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		SKRenderer::ms_pRenderer->SetViewPort(NULL);
		SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler);



		SKRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);
	}


	if (m_pDepthStencil)
	{
		if(!SKRenderer::ms_pRenderer->EndDepthStencilBuffer(m_pDepthStencil))
		{
			SKMAC_ASSERT(0);
			return false;
		}
	}
	EndRenderTargets();
	return true;
}
bool SKLightMaterialSceneRender::OnDraw(SKCuller & Culler)
{

	if (!m_pTargetArray.GetNum())
	{
		return false;
	}
	if (!m_pTargetArray[0])
	{
		return false;
	}
	SKMaterial * pMaterial = m_pMaterialInstacne->GetMaterial();
	SKLightFunPass * pLightFunPass = pMaterial->GetLightFunPass();
	pLightFunPass->SetPassId(0);
	pLightFunPass->SetMaterialInstance(m_pMaterialInstacne);
	pLightFunPass->SetCamera(NULL);
	pLightFunPass->SetSpatial(SKGeometry::GetDefaultQuad());
	pLightFunPass->m_pLight = m_pLight;
	pLightFunPass->Draw(SKRenderer::ms_pRenderer);

	return true;
}
bool SKLightMaterialSceneRender::SetDepthStencil(SKDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
	{
		return false;
	}
	if (!pDepthStencil->GetWidth() || !pDepthStencil->GetHeight())
	{
		return false;
	}
	if (m_uiRTHeight && m_uiRTWidth)
	{
		if (m_uiRTWidth != pDepthStencil->GetWidth() || m_uiRTHeight != pDepthStencil->GetHeight())
		{
			return false;
		}
	}
	else
	{	
		m_uiRTWidth = pDepthStencil->GetWidth();
		m_uiRTHeight = pDepthStencil->GetHeight();
	}
	m_pDepthStencil = pDepthStencil;
	return true;
}
SKProjectShadowSceneRender::SKProjectShadowSceneRender()
{
	m_pTexAllState = SK_NEW SKTexAllState();
	m_pTexAllState->SetSamplerState((SKSamplerState*)SKSamplerState::GetDoubleLine());
	m_pNormalDepthTexture = NULL;
	static SKUsedName LightTypeString = _T("LIGHT_TYPE");
	m_DirectionLightShaderKey.SetTheKey(LightTypeString, 0);
	m_PointLightShaderKey.SetTheKey(LightTypeString, 1);
	m_SpotLightShaderKey.SetTheKey(LightTypeString, 2);
}
SKProjectShadowSceneRender::~SKProjectShadowSceneRender()
{

}
bool SKProjectShadowSceneRender::Draw(SKCuller & Culler,double dAppTime)
{
	if (m_uiClearFlag <= SKRenderer::CF_USE_MAX)
	{

		SKColorRGBA ClearColorRGBA = SKRenderer::ms_pRenderer->GetClearColor();
		SKREAL fClearDepth = SKRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = SKRenderer::ms_pRenderer->GetClearStencil();



		SKRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		SKRenderer::ms_pRenderer->SetViewPort(NULL);
		SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler, 0, dAppTime);

		SKRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);

	}
	return true;
}
bool SKProjectShadowSceneRender::OnDraw(SKCuller & Culler,unsigned int uiRenderGroup,double dAppTime)
{
		SKCamera * pCamera = Culler.GetCamera();
		SKVShader * pVShader = NULL;
		SKPShader * pPShader = NULL;
		{
			SKCustomMaterial * pCustomMaterial = SKCustomMaterial::GetProjectShadowPre();

			
			pVShader = pCustomMaterial->GetCurVShader(m_NULLShaderKey);
			pPShader = pCustomMaterial->GetCurPShader(m_NULLShaderKey);

			
			//vs
			static SKUsedName ProjectShadowMatrixString = _T("ProjectShadowMatrix");
			static SKUsedName WorldViewProjectMatrixString = _T("WorldViewProjectMatrix");

			SKMatrix3X3W ProjectShadowMatrix = m_pLocalLight->m_LightShadowMatrix.GetInverse();
			pVShader->SetParam(ProjectShadowMatrixString, &ProjectShadowMatrix);

			SKMatrix3X3W WorldViewProjectMatrix = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
			pVShader->SetParam(WorldViewProjectMatrixString,&WorldViewProjectMatrix);

			if(!SKRenderer::ms_pRenderer->DrawMesh(SKGeometry::GetDefaultCub(),&pCustomMaterial->GetRenderState(),pVShader,pPShader,NULL))
			{
				return false;
			}
		}

		{
			SKCustomMaterial * pCustomMaterial = SKCustomMaterial::GetProjectShadow();
			pVShader = pCustomMaterial->GetCurVShader(m_NULLShaderKey);

			if (m_pLocalLight->GetLightType() == SKLight::LT_POINT)
			{
				pPShader = pCustomMaterial->GetCurPShader(m_PointLightShaderKey);
			}
			else if (m_pLocalLight->GetLightType() == SKLight::LT_SPOT)
			{
				pPShader = pCustomMaterial->GetCurPShader(m_SpotLightShaderKey);
			}
			else
			{				
				pPShader = pCustomMaterial->GetCurPShader(m_DirectionLightShaderKey);
			}
			

			m_pTexAllState->m_pTex = m_pNormalDepthTexture;

			//VS
			static SKUsedName ProjectShadowMatrixString = _T("ProjectShadowMatrix");
			static SKUsedName WorldViewProjectMatrixString = _T("WorldViewProjectMatrix");

			SKMatrix3X3W ProjectShadowMatrix = m_pLocalLight->m_LightShadowMatrix.GetInverse();
			pVShader->SetParam(ProjectShadowMatrixString, &ProjectShadowMatrix);

			SKMatrix3X3W WorldViewProjectMatrix = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
			pVShader->SetParam(WorldViewProjectMatrixString,&WorldViewProjectMatrix);

			//ps
			unsigned int uiRTWidth = m_pLocalLight->GetShadowResolution();
			SKREAL fOffsetX = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
			SKREAL fOffsetY = 0.5f + (0.5f / (SKREAL)(uiRTWidth));
			SKMatrix3X3W texScaleBiasMat(	0.5f,     0.0f,     0.0f,       0.0f,
				0.0f,    -0.5f,     0.0f,       0.0f,
				0.0f,     0.0f,     1.0f,		0.0f,
				fOffsetX, fOffsetY, 0.0f,       1.0f );
			static SKUsedName ShadowMatrixString = _T("ShadowMatrix");
			SKMatrix3X3W ShadowMatrix = m_pLocalLight->m_LightShadowMatrix * texScaleBiasMat;
			pPShader->SetParam(ShadowMatrixString, &ShadowMatrix);


			static SKUsedName InvViewString = _T("InvView");
			SKMatrix3X3W InvView = pCamera->GetViewMatrix().GetInverse();
			pPShader->SetParam(InvViewString, &InvView);


			static SKUsedName ProjectString = _T("Project");
			SKVector2 Project = SKVector2(pCamera->GetProjMatrix()._00,pCamera->GetProjMatrix()._11);
			pPShader->SetParam(ProjectString, &Project);


			static SKUsedName FarZString = _T("FarZ");
			SKREAL FarZ = pCamera->GetZFar();
			pPShader->SetParam(FarZString, &FarZ);

			static SKUsedName ZBiasString = _T("ZBias");
			SKREAL ZBias = m_pLocalLight->m_ZBias;
			pPShader->SetParam(ZBiasString, &ZBias);

			static SKUsedName NormalDepthInvRtWidthString = _T("NormalDepthInvRtWidth");
			SKVector2 NormalDepthInvRtWidth = SKVector2(1.0f / m_pTexAllState->m_pTex->GetWidth(0) * 1.0f,1.0f / m_pTexAllState->m_pTex->GetHeight(0) * 1.0f);
			pPShader->SetParam(NormalDepthInvRtWidthString, &NormalDepthInvRtWidth);

			static SKUsedName ShadowRtWidthString = _T("ShadowRtWidth");
			SKREAL ShadowRtWidth = m_pLocalLight->GetShadowResolution() * 1.0f;
			pPShader->SetParam(ShadowRtWidthString, &ShadowRtWidth);

			static SKUsedName ShadowSamplerString = _T("ShadowTexture");
			pPShader->SetParam(ShadowSamplerString,m_pLocalLight->GetShadowTexture());

			static SKUsedName NormalDepthSamplerString = _T("NormalDepthTexture");
			pPShader->SetParam(NormalDepthSamplerString,m_pTexAllState);

			static SKUsedName ProjectShadowColorString = _T("ProjectShadowColor");
			pPShader->SetParam(ProjectShadowColorString,&m_pLocalLight->m_ProjectShadowColor);

			static SKUsedName ProjectShadowFallOffString = _T("ProjectShadowFallOff");
			pPShader->SetParam(ProjectShadowFallOffString, &m_pLocalLight->m_ProjectShadowFallOff);
			if (m_pLocalLight->GetLightType() == SKLight::LT_POINT || m_pLocalLight->GetLightType() == SKLight::LT_SPOT)
			{
				static SKUsedName LightPosString = _T("LightWorldPos");
				SKVector3 LightPos = m_pLocalLight->GetWorldTranslate();
				pPShader->SetParam(LightPosString, &LightPos);

				static SKUsedName LightRangeString = _T("LightRange");
				pPShader->SetParam(LightRangeString, &m_fLightRange);

				if (m_pLocalLight->GetLightType() == SKLight::LT_SPOT)
				{
					static SKUsedName LightWorldDirectionString = _T("LightWorldDirection");
					pPShader->SetParam(LightWorldDirectionString, &m_LightWorldDirection);

					static SKUsedName FalloffString = _T("Falloff");
					pPShader->SetParam(FalloffString, &m_Falloff);

					static SKUsedName ThetaString = _T("Theta");
					pPShader->SetParam(ThetaString, &m_Theta);

					static SKUsedName PhiString = _T("Phi");
					pPShader->SetParam(PhiString, &m_Phi);
				}
			}
			

			if (!SKRenderer::ms_pRenderer->DrawMesh(SKGeometry::GetDefaultCub(), &pCustomMaterial->GetRenderState(), pVShader, pPShader, NULL))
			{
				return false;
			}
		}
			
			
	return true;
}
SKVolumeShadowSceneRender::SKVolumeShadowSceneRender()
{

}

SKVolumeShadowSceneRender::~SKVolumeShadowSceneRender()
{

}
bool SKVolumeShadowSceneRender::Draw(SKCuller & Culler, double dAppTime)
{
	if (m_uiClearFlag <= SKRenderer::CF_USE_MAX)
	{

		SKColorRGBA ClearColorRGBA = SKRenderer::ms_pRenderer->GetClearColor();
		SKREAL fClearDepth = SKRenderer::ms_pRenderer->GetClearDepth();
		unsigned int uiClearStencil = SKRenderer::ms_pRenderer->GetClearStencil();



		SKRenderer::ms_pRenderer->SetClearColor(m_ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(m_fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(m_uiClearStencil);

		SKRenderer::ms_pRenderer->SetViewPort(NULL);
		SKRenderer::ms_pRenderer->ClearBuffers(m_uiClearFlag);
		OnDraw(Culler, SKCuller::RG_NORMAL, dAppTime);

		SKRenderer::ms_pRenderer->SetClearColor(ClearColorRGBA);
		SKRenderer::ms_pRenderer->SetClearDepth(fClearDepth);
		SKRenderer::ms_pRenderer->SetClearStencil(uiClearStencil);

	}
	return true;
}
bool SKVolumeShadowSceneRender::OnDraw(SKCuller & Culler, unsigned int uiRenderGroup, double dAppTime)
{
	for (unsigned int t = 0; t < SKCuller::SKT_MAX; t++)
	{
		for (unsigned int j = 0; j < Culler.GetVisibleNum(t, uiRenderGroup); j++)
		{

			SKRenderContext& VisibleContext = Culler.GetVisibleSpatial(j, t, uiRenderGroup);
			if (!VisibleContext.m_pGeometry || !VisibleContext.m_pMaterialInstance)
			{
				continue;
			}
			SKMaterial * pMaterial = VisibleContext.m_pMaterialInstance->GetMaterial();
			if (!pMaterial)
			{
				continue;
			}

			SKGeometry * pGeometry = VisibleContext.m_pGeometry;


			SKMaterialInstance * pMaterialInstance = VisibleContext.m_pMaterialInstance;

			SKVolumeShadowPass * pShadowPass = pMaterialInstance->GetMaterial()->GetVolumeShadowPass();
			pShadowPass->m_pLocalLight = m_pLocalLight;

			pShadowPass->SetPassId(VisibleContext.m_uiPassId);
			pShadowPass->SetSpatial(pGeometry);
			pShadowPass->SetMaterialInstance(pMaterialInstance);
			pShadowPass->SetCamera(Culler.GetCamera());
			pShadowPass->Draw(SKRenderer::ms_pRenderer);


		}
	}
	return true;
}