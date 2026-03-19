#include "SKPEEndFunction.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPEEndFunction,SKPostEffectFunction)
BEGIN_ADD_PROPERTY(SKPEEndFunction,SKPostEffectFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPEEndFunction)
IMPLEMENT_INITIAL_END
SKPostEffectSceneRender * SKPEEndFunction::CreateSceneRender()
{
	return NULL;
}
void SKPEEndFunction::OnDraw(SKCuller & Culler,double dAppTime)
{
	return;
}
SKPEEndFunction::SKPEEndFunction(const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet)
:SKPostEffectFunction(ShowName,pPostEffectSet)
{

	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::PET_OUT,_T("InputColor"),this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
}
SKPEEndFunction::~SKPEEndFunction()
{

}
SKPEEndFunction::SKPEEndFunction()
{

}