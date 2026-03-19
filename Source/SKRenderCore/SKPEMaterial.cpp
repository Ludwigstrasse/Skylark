#include "SKPEMaterial.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPEMaterial, SKPostEffectFunction)
BEGIN_ADD_PROPERTY(SKPEMaterial, SKPostEffectFunction)
REGISTER_PROPERTY(m_pMaterialInstance, MaterialInstance, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPEMaterial)
IMPLEMENT_INITIAL_END
SKPEMaterial::SKPEMaterial(const SKUsedName & ShowName, SKPostEffectSet * pPostEffectSet)
:SKPostEffectFunction(ShowName, pPostEffectSet)
{

	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::PET_OUT, _T("InputColor"), this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	SKOutputNode * pOutNode = NULL;
	pOutNode = SK_NEW SKOutputNode(SKPutNode::PET_OUT, _T("OutColor"), this);
	SKMAC_ASSERT(pOutNode);
	m_pOutput.AddElement(pOutNode);
	m_pMaterialInstance = NULL;
}
SKPEMaterial::~SKPEMaterial()
{

}
SKPEMaterial::SKPEMaterial()
{
	m_pMaterialInstance = NULL;
}
SKPostEffectSceneRender * SKPEMaterial::CreateSceneRender()
{

	if (!m_pPostEffectRender)
	{
		m_pPostEffectRender = SK_NEW SKPEMaterialSceneRender();
	}

	return m_pPostEffectRender;
}
void SKPEMaterial::SetMaterial(SKMaterialR * pMaterial)
{
	m_pMaterialInstance = SK_NEW SKMaterialInstance(pMaterial);
}
void SKPEMaterial::OnDraw(SKCuller & Culler, double dAppTime)
{
	SKRenderTarget * pTarget = GetRenderTarget(m_pInput[INPUT_COLOR]);
	if (!pTarget || !m_pMaterialInstance)
	{
		return;
	}
	if (m_pPostEffectRender)
	{
		SKPostEffectSceneRender * pTemp = m_pPostEffectRender;
		((SKPEMaterialSceneRender*)pTemp)->SetSourceTarget(pTarget->GetCreateBy());
		((SKPEMaterialSceneRender*)pTemp)->m_pMaterialInstacne = m_pMaterialInstance;
		m_pPostEffectRender->Draw(Culler, dAppTime);
	}
}