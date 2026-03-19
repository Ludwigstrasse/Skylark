#include "SKPEGray.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPEGray,SKPostEffectFunction)
BEGIN_ADD_PROPERTY(SKPEGray,SKPostEffectFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPEGray)
IMPLEMENT_INITIAL_END
SKPEGray::SKPEGray (const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet)
:SKPostEffectFunction(ShowName,pPostEffectSet)
{

	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::PET_OUT,_T("InputColor"),this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	SKOutputNode * pOutNode = NULL;
	pOutNode = SK_NEW SKOutputNode(SKPutNode::PET_OUT,_T("OutColor"),this);
	SKMAC_ASSERT(pOutNode);
	m_pOutput.AddElement(pOutNode);
}
SKPEGray::~SKPEGray ()
{

}
SKPEGray::SKPEGray ()
{
}
SKPostEffectSceneRender * SKPEGray::CreateSceneRender()
{

	if (!m_pPostEffectRender)
	{
		m_pPostEffectRender = SK_NEW SKPEGraySceneRender();
	}
	
	return m_pPostEffectRender;
}
void SKPEGray::OnDraw(SKCuller & Culler,double dAppTime)
{
	SKRenderTarget * pTarget = GetRenderTarget(m_pInput[INPUT_COLOR]);
	if (!pTarget)
	{
		return ;
	}
	if (m_pPostEffectRender)
	{
		SKPostEffectSceneRender * pTemp = m_pPostEffectRender;
		((SKPEGraySceneRender*) pTemp)->SetSourceTarget(pTarget->GetCreateBy());
		m_pPostEffectRender->Draw(Culler,dAppTime);
	}
}