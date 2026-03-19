#include "SKAnimTree.h"
#include "SKAnimFunction.h"
#include "SKAnimMainFunction.h"
#include "SKAnimBaseFunction.h"
#include "SKAnimSequenceFunc.h"
#include "SKOneParamSmoothAnimBlend.h"
#include "SKRectAnimBlend.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKOneParamSmoothAnimBlendSequence.h"
#include "SKOneParamImmediateAnimBlend.h"
#include "SKPartialAnimBlend.h"
using namespace SKEngine2;
SKPointer<SKAnimTree> SKAnimTree::Default = NULL;
bool SKAnimTree::ms_bIsEnableASYNLoader = true;
bool SKAnimTree::ms_bIsEnableGC = true;
IMPLEMENT_RTTI(SKAnimTree,SKController)
BEGIN_ADD_PROPERTY(SKAnimTree,SKController)
REGISTER_PROPERTY(m_pAnimMainFunction,pAnimMainFunction,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pAnimFunctionArray,pAnimFunctionArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ShowName,ShowName,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimTree)
IMPLEMENT_INITIAL_END
SKAnimTree::~SKAnimTree()
{
	for(unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		SKAnimBaseFunction * pAnimFunction = m_pAnimFunctionArray[i];
		m_pAnimFunctionArray[i] = NULL;
		SKMAC_DELETE(pAnimFunction);
	}
	
	m_pAnimFunctionArray.Clear();

	m_pAnimMainFunction = NULL;
}
SKAnimTree::SKAnimTree(const SKUsedName &ShowName)
{
	m_ShowName = ShowName;

	m_pAnimFunctionArray.Clear();
	m_pAnimMainFunction = NULL;

	m_pAnimMainFunction = SK_NEW SKAnimMainFunction(_T("AnimTree"),this);
	SKMAC_ASSERT(m_pAnimMainFunction);



}

SKAnimTree::SKAnimTree()
{
	m_pAnimFunctionArray.Clear();
	m_pAnimMainFunction = NULL;

}
SKSkelectonMeshNode * SKAnimTree::GetSkelectonMeshNode()const
{
	return DynamicCast<SKSkelectonMeshNode>(m_pObject);
}
void SKAnimTree::AddAnimFunction(SKAnimBaseFunction * pAnimFunction)
{
	if(pAnimFunction)
	{
		m_pAnimFunctionArray.AddElement(pAnimFunction);
	}
}
void SKAnimTree::DeleteAnimFunction(SKAnimBaseFunction * pAnimFunction)
{
	if(pAnimFunction)
	{
		for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
		{
			if(m_pAnimFunctionArray[i] == pAnimFunction)
			{
				m_pAnimFunctionArray.Erase(i);
				return ;
			}
		}

	}
}
bool SKAnimTree::Update(double dAppTime)
{
	if (!SKController::Update(dAppTime))
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

	if (!pMesh->GetAnimSet())
	{
		return false;
	}
	for(unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		m_pAnimFunctionArray[i]->ClearFlag();
	}
	if(!m_pAnimMainFunction->Update(dAppTime))
		return false;
	return true;
}
bool SKAnimTree::SetObject(SKObject * pObject)
{
	SKSkelectonMeshNode* Temp = DynamicCast<SKSkelectonMeshNode>(pObject); 
	if(!Temp)
		return 0;
	m_pObject = pObject;
	for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		m_pAnimFunctionArray[i]->SetObject(pObject);
	}
	return 1;
}
void SKAnimTree::ResetAnimFunction()
{
	for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		SKAnimSequenceFunc * pAnimSequence = DynamicCast<SKAnimSequenceFunc>(m_pAnimFunctionArray[i]);
		if (pAnimSequence)
		{
			
			pAnimSequence->SetAnim(pAnimSequence->GetAnimName());

			
		}
	}
}
void SKAnimTree::SetNodePara(const SKUsedName & ShowName, void * pPara)
{
	SKAnimBaseFunction * pAnimNode = GetAnimFunctionFromShowName(ShowName);
	if (!pAnimNode)
	{
		return;
	}
	pAnimNode->SetPara(pPara);
}
SKAnimBaseFunction *  SKAnimTree::GetAnimFunctionFromShowName(const SKUsedName & ShowName)
{
	for (unsigned int i = 0 ; i < m_pAnimFunctionArray.GetNum() ; i++)
	{
		if (m_pAnimFunctionArray[i]->GetShowName() == ShowName)
		{
			return m_pAnimFunctionArray[i];
		}
	}
	return NULL;
}

IMPLEMENT_RTTI(SKAnimTreeOneAnim,SKAnimTree)
BEGIN_ADD_PROPERTY(SKAnimTreeOneAnim,SKAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimTreeOneAnim)
IMPLEMENT_INITIAL_END
SKAnimTreeOneAnim::SKAnimTreeOneAnim()
{

}
SKAnimTreeOneAnim::SKAnimTreeOneAnim(const SKUsedName &ShowName,const SKString& AnimName)
:SKAnimTree(ShowName)
{
	SKAnimSequenceFunc* pAnimSequenceFunc = SK_NEW SKAnimSequenceFunc(_T("Squence"),this);
	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimSequenceFunc->GetOutputNode(0));
	pAnimSequenceFunc->SetAnim(AnimName);

}
SKAnimTreeOneAnim::~SKAnimTreeOneAnim()
{

}
IMPLEMENT_RTTI(SKAnimTreeTwoSmoothBlendAnim,SKAnimTree)
BEGIN_ADD_PROPERTY(SKAnimTreeTwoSmoothBlendAnim,SKAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimTreeTwoSmoothBlendAnim)
IMPLEMENT_INITIAL_END
SKAnimTreeTwoSmoothBlendAnim::SKAnimTreeTwoSmoothBlendAnim()
{

}
SKAnimTreeTwoSmoothBlendAnim::SKAnimTreeTwoSmoothBlendAnim(const SKUsedName &ShowName,const SKString &AnimName1,const SKString &AnimName2)
:SKAnimTree(ShowName)
{
	SKOneParamSmoothAnimBlendSequence* pAnimBlend = SK_NEW SKOneParamSmoothAnimBlendSequence(_T("OneParamSmoothAnimBlendSequence"), this);
 	pAnimBlend->CreateSlot(2);
 	pAnimBlend->SetAnim(0,AnimName1);
	pAnimBlend->SetAnim(1, AnimName2);
	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
SKAnimTreeTwoSmoothBlendAnim::~SKAnimTreeTwoSmoothBlendAnim()
{

}

IMPLEMENT_RTTI(SKAnimTreeTwoImmediateAnim, SKAnimTree)
BEGIN_ADD_PROPERTY(SKAnimTreeTwoImmediateAnim, SKAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimTreeTwoImmediateAnim)
IMPLEMENT_INITIAL_END
SKAnimTreeTwoImmediateAnim::SKAnimTreeTwoImmediateAnim()
{

}
SKAnimTreeTwoImmediateAnim::SKAnimTreeTwoImmediateAnim(const SKUsedName &ShowName, const SKString &AnimName1, const SKString &AnimName2)
:SKAnimTree(ShowName)
{
	SKAnimSequenceFunc* pAnimSequenceFunc1 = SK_NEW SKAnimSequenceFunc(_T("Squence1"), this);

	pAnimSequenceFunc1->SetAnim(AnimName1);

	SKAnimSequenceFunc* pAnimSequenceFunc2 = SK_NEW SKAnimSequenceFunc(_T("Squence2"), this);

	pAnimSequenceFunc2->SetAnim(AnimName2);

	SKOneParamImmediateAnimBlend * pAnimBlend = SK_NEW SKOneParamImmediateAnimBlend(_T("OneParamBlend"), this);

	pAnimBlend->SetNodeCrossFadingTime(0, 1000.0f);
	pAnimBlend->SetNodeCrossFadingTime(1, 1000.0f);

	pAnimBlend->GetInputNode(0)->Connection(pAnimSequenceFunc1->GetOutputNode(0));
	pAnimBlend->GetInputNode(1)->Connection(pAnimSequenceFunc2->GetOutputNode(0));

	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
SKAnimTreeTwoImmediateAnim::~SKAnimTreeTwoImmediateAnim()
{

}

IMPLEMENT_RTTI(SKAnimTreePartialAnim, SKAnimTree)
BEGIN_ADD_PROPERTY(SKAnimTreePartialAnim, SKAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimTreePartialAnim)
IMPLEMENT_INITIAL_END
SKAnimTreePartialAnim::SKAnimTreePartialAnim()
{

}
SKAnimTreePartialAnim::SKAnimTreePartialAnim(const SKUsedName &ShowName, const SKString &AnimName1, const SKString &AnimName2)
:SKAnimTree(ShowName)
{
	SKAnimSequenceFunc* pAnimSequenceFunc1 = SK_NEW SKAnimSequenceFunc(_T("Squence1"), this);

	pAnimSequenceFunc1->SetAnim(AnimName1);

	SKAnimSequenceFunc* pAnimSequenceFunc2 = SK_NEW SKAnimSequenceFunc(_T("Squence2"), this);

	pAnimSequenceFunc2->SetAnim(AnimName2);

	SKPartialAnimBlend * pAnimBlend = SK_NEW SKPartialAnimBlend(_T("PartialAnimBlend"), this);

	

	pAnimBlend->GetInputNode(0)->Connection(pAnimSequenceFunc1->GetOutputNode(0));
	pAnimBlend->GetInputNode(1)->Connection(pAnimSequenceFunc2->GetOutputNode(0));

	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
SKAnimTreePartialAnim::~SKAnimTreePartialAnim()
{

}

IMPLEMENT_RTTI(SKAnimTreeRectBlendAnim,SKAnimTree)
BEGIN_ADD_PROPERTY(SKAnimTreeRectBlendAnim,SKAnimTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimTreeRectBlendAnim)
IMPLEMENT_INITIAL_END
SKAnimTreeRectBlendAnim::SKAnimTreeRectBlendAnim()
{

}
SKAnimTreeRectBlendAnim::SKAnimTreeRectBlendAnim(const SKUsedName &ShowName,
												 const SKString &AnimName1,
												 const SKString &AnimName2,
												 const SKString &AnimName3,
												 const SKString &AnimName4)
:SKAnimTree(ShowName)
{
	SKAnimSequenceFunc* pAnimSequenceFunc1 = SK_NEW SKAnimSequenceFunc(_T("Squence1"),this);

	pAnimSequenceFunc1->SetAnim(AnimName1);

	SKAnimSequenceFunc* pAnimSequenceFunc2 = SK_NEW SKAnimSequenceFunc(_T("Squence2"),this);

	pAnimSequenceFunc2->SetAnim(AnimName2);

	SKAnimSequenceFunc* pAnimSequenceFunc3 = SK_NEW SKAnimSequenceFunc(_T("Squence3"),this);

	pAnimSequenceFunc1->SetAnim(AnimName3);

	SKAnimSequenceFunc* pAnimSequenceFunc4 = SK_NEW SKAnimSequenceFunc(_T("Squence4"),this);

	pAnimSequenceFunc2->SetAnim(AnimName4);


	SKRectAnimBlend * pAnimBlend = SK_NEW SKRectAnimBlend(_T("RectBlend"),this);


	pAnimBlend->GetInputNode(0)->Connection(pAnimSequenceFunc1->GetOutputNode(0));
	pAnimBlend->GetInputNode(1)->Connection(pAnimSequenceFunc2->GetOutputNode(0));
	pAnimBlend->GetInputNode(2)->Connection(pAnimSequenceFunc3->GetOutputNode(0));
	pAnimBlend->GetInputNode(3)->Connection(pAnimSequenceFunc4->GetOutputNode(0));
	m_pAnimMainFunction->GetInputNode(_T("Anim"))->Connection(pAnimBlend->GetOutputNode(0));

}
SKAnimTreeRectBlendAnim::~SKAnimTreeRectBlendAnim()
{

}