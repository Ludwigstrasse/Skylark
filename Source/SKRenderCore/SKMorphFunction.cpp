#include "SKMorphFunction.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKMorphFunction,SKMorphBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKMorphFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKMorphFunction,SKMorphBaseFunction)
END_ADD_PROPERTY
SKMorphFunction::SKMorphFunction()
{

}
SKMorphFunction::SKMorphFunction(const SKUsedName & ShowName,SKMorphTree * pMorphTree)
:SKMorphBaseFunction(ShowName,pMorphTree)
{

}
SKMorphFunction::~SKMorphFunction()
{

}
void SKMorphFunction::UpdateGeometryDate(unsigned int GeometryIndex)
{
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			SKMorphFunction *pMorphFunction = (SKMorphFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pMorphFunction)
			{
				pMorphFunction->UpdateGeometryDate(GeometryIndex);
			}
		}
	}
}
void SKMorphFunction::UpdateVertexDate(unsigned int uiVertexIndex)
{
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			SKMorphFunction *pMorphFunction = (SKMorphFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if(pMorphFunction)
			{
				pMorphFunction->UpdateVertexDate(uiVertexIndex);
			}
		}
	}
}
void SKMorphFunction::ClearChangeFlag()
{
	for (unsigned int i = 0 ; i < MAX_NUM_POS3 ; i++)
	{
		m_bPosChange[i] = false;
	}
	

	for (unsigned int i = 0 ; i < MAX_NUM_NORMAL3 ; i++)
	{
		m_bNormalChange[i] = false;
	}

	m_bTangentChange = false;
	m_bBinormalChange = false;


	for (unsigned int i = 0 ; i < MAX_NUM_COLOR ; i++)
	{
		m_bColorChange[i] = false;
	}


}