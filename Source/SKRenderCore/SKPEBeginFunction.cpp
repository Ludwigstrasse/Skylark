#include "SKPEBeginFunction.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPEBeginFunction,SKPostEffectFunction)
BEGIN_ADD_PROPERTY(SKPEBeginFunction,SKPostEffectFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPEBeginFunction)
IMPLEMENT_INITIAL_END
SKPostEffectSceneRender * SKPEBeginFunction::CreateSceneRender()
{
	return NULL;
}
void SKPEBeginFunction::OnDraw(SKCuller & Culler,double dAppTime)
{

}
SKPEBeginFunction::SKPEBeginFunction(const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet)
:SKPostEffectFunction(ShowName,pPostEffectSet)
{

	SKOutputNode * pOutNode = NULL;
	pOutNode = SK_NEW SKOutputNode(SKPutNode::PET_OUT,_T("OutColor"),this);
	SKMAC_ASSERT(pOutNode);
	m_pOutput.AddElement(pOutNode);
	m_pBeginTargetArray = NULL;
}
SKPEBeginFunction::~SKPEBeginFunction()
{
	m_pBeginTargetArray = NULL;
}
SKPEBeginFunction::SKPEBeginFunction()
{
	m_pBeginTargetArray = NULL;
}