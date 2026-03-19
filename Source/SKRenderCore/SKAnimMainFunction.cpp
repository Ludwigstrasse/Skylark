#include "SKAnimMainFunction.h"
#include "SKBoneNode.h"
#include "SKAnimFunction.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKAnimMainFunction,SKAnimBaseFunction)
BEGIN_ADD_PROPERTY(SKAnimMainFunction,SKAnimBaseFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimMainFunction)
IMPLEMENT_INITIAL_END
SKAnimMainFunction::SKAnimMainFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
:SKAnimBaseFunction(ShowName,pAnimTree)
{
	SKString InputName = _T("Anim");
	SKInputNode * pInputNode = NULL;

	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("IK");
	pInputNode = NULL;

	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_IK,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
}
SKAnimMainFunction::SKAnimMainFunction()
{

}
SKAnimMainFunction::~SKAnimMainFunction()
{
	
}
bool SKAnimMainFunction::Update(double dAppTime)
{
	if (!SKAnimBaseFunction::Update(dAppTime))
		return false;

	const SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	SKMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return false;
	}
	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return false;
	}
	if(m_pInput[0]->GetOutputLink())
	{
		SKAnimFunction *pAnimFunction = (SKAnimFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
		if(pAnimFunction)
		{
			pAnimFunction->Update(dAppTime);
			for(unsigned int i = 0 ; i < pSkelecton->GetBoneNum() ; i++)
			{
				SKBoneNode * pBone = pSkelecton->GetBoneNode(i);
				if(pBone)
				{
					SKMatrix3X3W Mat;
					pAnimFunction->m_BoneOutPut[i].GetMatrix(Mat);
					pBone->SetLocalMat(Mat);
				}
			}
			SKSpatial * pMeshComponent = pMesh->GetParent();
			if (pMeshComponent)
			{

				SKMatrix3X3W RootMatrix;
				pAnimFunction->m_RootAtom.GetMatrix(RootMatrix);
				pMeshComponent->SetLocalMat(RootMatrix);

			}
		}
	}
	
	if(m_pInput[1]->GetOutputLink())
	{
		SKAnimFunction *pAnimFunction = (SKAnimFunction *)m_pInput[1]->GetOutputLink()->GetOwner();
		if(pAnimFunction)
		{
			pAnimFunction->Update(dAppTime);
			
		}
	}
	
	return true;

}
// void SKAnimMainFunction::CreateNode(const SKString & NodeName,unsigned int uiNodeType)
// {
// 	if (uiNodeType > SKPutNode::AVT_ANIM && uiNodeType < SKPutNode::AVT_MAX)
// 	{
// 		SKString InputName = NodeName;
// 		SKInputNode * pInputNode = NULL;
// 
// 		pInputNode = SK_NEW SKInputNode(uiNodeType,InputName,this);
// 		SKMAC_ASSERT(pInputNode);
// 		m_pInput.AddElement(pInputNode);
// 	}
// 
// }