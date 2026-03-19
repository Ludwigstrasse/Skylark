#include "SKAnimFunction.h"
#include "SKBoneNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKAnimFunction,SKAnimBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKAnimFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKAnimFunction,SKAnimBaseFunction)
END_ADD_PROPERTY
SKAnimFunction::SKAnimFunction()
{
	m_BoneOutPut.Clear();

}
SKAnimFunction::SKAnimFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
:SKAnimBaseFunction(ShowName,pAnimTree)
{
	
}
bool SKAnimFunction::SetObject(SKObject * pObject)
{
	if(SKAnimBaseFunction::SetObject(pObject))
	{
		const SKSkelectonMeshNode* pMesh = GetSkelectonMeshNode(); 
		SKMAC_ASSERT(pMesh);

		SKSkelecton * pSkelecton = pMesh->GetSkelecton();
		SKMAC_ASSERT(pSkelecton);

		unsigned int BoneNum = pSkelecton->GetBoneNum();
		m_BoneOutPut.SetBufferNum(BoneNum);
		for(unsigned int i = 0 ; i < m_BoneOutPut.GetNum() ; i++)
		{
			m_BoneOutPut[i].FromTransfrom(pSkelecton->GetBoneNode(i)->GetLocalTransform());

		}
		return true;
	}

	
	return false;
}
SKAnimFunction::~SKAnimFunction()
{
	m_BoneOutPut.Clear();
}

bool SKAnimFunction::Update(double dAppTime)
{
	if (!SKAnimBaseFunction::Update(dAppTime))
		return false;

	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			SKAnimBaseFunction *pAnimBaseFunction = (SKAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pAnimBaseFunction)
			{
				pAnimBaseFunction->Update(dAppTime);
			}
		}
	}
	return true;
}
SKAnimAtom& SKAnimFunction::GetRootTranfform()
{
	return m_RootAtom;
}
