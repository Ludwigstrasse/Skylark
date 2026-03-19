#include "SKMorphTree.h"
#include "SKMorphMainFunction.h"
#include "SKMorphBaseFunction.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKOneParamSmoothMorphBlend.h"
#include "SKMorphSequenceFunc.h"
#include "SKSwitchNode.h"
using namespace SKEngine2;
SKPointer<SKMorphTree> SKMorphTree::Default = NULL;
bool SKMorphTree::ms_bIsEnableASYNLoader = true;
bool SKMorphTree::ms_bIsEnableGC = true;
IMPLEMENT_RTTI(SKMorphTree,SKController)
BEGIN_ADD_PROPERTY(SKMorphTree,SKController)
REGISTER_PROPERTY(m_pMorphMainFunction,MorphMainFunction,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pMorphFunctionArray,MorphFunctionArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ShowName, ShowName, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMorphTree)
IMPLEMENT_INITIAL_END
SKMorphTree::~SKMorphTree()
{
	for(unsigned int i = 0 ; i < m_pMorphFunctionArray.GetNum() ; i++)
	{
		SKMorphBaseFunction * pMorphFunction = m_pMorphFunctionArray[i];
		m_pMorphFunctionArray[i] = NULL;
		SKMAC_DELETE(pMorphFunction);
	}

	m_pMorphFunctionArray.Clear();
	m_pMorphMainFunction = NULL;
}
SKMorphTree::SKMorphTree()
{
	m_pMorphFunctionArray.Clear();
	m_pMorphMainFunction = NULL;
	m_bCompressData = false;
}
SKMorphTree::SKMorphTree(const SKUsedName &ShowName)
{
	m_ShowName = ShowName;
	m_pMorphFunctionArray.Clear();
	m_pMorphMainFunction = NULL;
	m_bCompressData = false;
	m_pMorphMainFunction = SK_NEW SKMorphMainFunction(_T("MorphTree"), this);
	SKMAC_ASSERT(m_pMorphMainFunction);
}
void SKMorphTree::AddMorphFunction(SKMorphBaseFunction * pMorphFunction)
{
	if(pMorphFunction)
	{
		m_pMorphFunctionArray.AddElement(pMorphFunction);
	}
}
void SKMorphTree::DeleteMorphFunction(SKMorphBaseFunction * pMorphFunction)
{
	if(pMorphFunction)
	{
		for (unsigned int i = 0 ; i < m_pMorphFunctionArray.GetNum() ; i++)
		{
			if(m_pMorphFunctionArray[i] == pMorphFunction)
			{
				m_pMorphFunctionArray.Erase(i);
				return ;
			}
		}

	}
}
void SKMorphTree::ClearChangeFlag()
{
	for (unsigned int i = 0; i < m_pMorphFunctionArray.GetNum(); i++)
	{
		m_pMorphFunctionArray[i]->ClearChangeFlag();
	}
}
bool SKMorphTree::Update(double dAppTime)
{
	if(!SKController::Update(dAppTime))
		return false;
	SKModelMeshNode* pMeshNode = GetMeshNode();

	if (!pMeshNode)
	{
		return false;
	}
	SKGeometryNode * pGeomeNode = NULL;
	if (pMeshNode->GetLodType() == SKModelMeshNode::LT_DLOD)
	{
		if (pMeshNode->GetDlodNode())
		{
			pGeomeNode = DynamicCast<SKGeometryNode>(pMeshNode->GetDlodNode()->GetActiveNode());
		}
	}
	else
	{
		pGeomeNode = DynamicCast<SKGeometryNode>(pMeshNode->GetChild(0));
	}
	if (!pGeomeNode)
	{
		return false;
	}
	for(unsigned int i = 0 ; i < m_pMorphFunctionArray.GetNum() ; i++)
	{
		m_pMorphFunctionArray[i]->SetCurGeometryNodeNode(pGeomeNode);
		m_pMorphFunctionArray[i]->ClearFlag();
	}
	if(!m_pMorphMainFunction->Update(dAppTime))
		return false;
	return true;
}
bool SKMorphTree::SetObject(SKObject * pObject)
{
	SKMeshNode* Temp = DynamicCast<SKMeshNode>(pObject); 
	if(!Temp)
		return 0;
	m_pObject = pObject;
	for (unsigned int i = 0; i < m_pMorphFunctionArray.GetNum(); i++)
	{
		m_pMorphFunctionArray[i]->SetObject(pObject);
	}

	SKModelMeshNode* pMeshNode = GetMeshNode();
	if (!pMeshNode)
	{
		return false;
	}
	SKGeometryNode * pGeomeNode = NULL;
	if (pMeshNode->GetLodType() == SKModelMeshNode::LT_DLOD)
	{
		if (pMeshNode->GetDlodNode())
		{
			pGeomeNode = DynamicCast<SKGeometryNode>(pMeshNode->GetDlodNode()->GetActiveNode());
		}
	}
	else
	{
		pGeomeNode = DynamicCast<SKGeometryNode>(pMeshNode->GetChild(0));
	}
	if (!pGeomeNode)
	{
		return false;
	}

	m_bCompressData = (pGeomeNode->GetNormalGeometry(0)->GetOriginMeshDate()->GetVertexBuffer()->GetNormalDate(0)->GetDT() == SKDataBuffer::DT_UBYTE4N);
	return 1;
}
void SKMorphTree::SetNodePara(const SKUsedName & ShowName, void * pPara)
{
	SKMorphBaseFunction * pAnimNode = GetMorphFunctionFromShowName(ShowName);
	pAnimNode->SetPara(pPara);
}
SKMorphBaseFunction *  SKMorphTree::GetMorphFunctionFromShowName(const SKUsedName & ShowName)
{
	for (unsigned int i = 0; i < m_pMorphFunctionArray.GetNum(); i++)
	{
		if (m_pMorphFunctionArray[i]->GetShowName() == ShowName)
		{
			return m_pMorphFunctionArray[i];
		}
	}
	return NULL;
}
IMPLEMENT_RTTI(SKMorphTreeTwoSmoothBlendAnim, SKMorphTree)
BEGIN_ADD_PROPERTY(SKMorphTreeTwoSmoothBlendAnim, SKMorphTree)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMorphTreeTwoSmoothBlendAnim)
IMPLEMENT_INITIAL_END
SKMorphTreeTwoSmoothBlendAnim::SKMorphTreeTwoSmoothBlendAnim()
{

}
SKMorphTreeTwoSmoothBlendAnim::SKMorphTreeTwoSmoothBlendAnim(const SKUsedName &ShowName, const SKString &MorphName)
:SKMorphTree(ShowName)
{
	SKOneParamSmoothMorphBlend* pMorphBlend = SK_NEW SKOneParamSmoothMorphBlend(_T("OneParamSmoothMorphBlend"), this);
	
	SKMorphSequenceFunc * pMorphS1 = SK_NEW SKMorphSequenceFunc(_T("Squence1"), this);
	SKMorphSequenceFunc * pMorphS2 = SK_NEW SKMorphSequenceFunc(_T("Squence2"), this);
	pMorphS2->SetMorph(MorphName);

	pMorphBlend->GetInputNode(0)->Connection(pMorphS1->GetOutputNode(0));
	pMorphBlend->GetInputNode(1)->Connection(pMorphS2->GetOutputNode(0));
	m_pMorphMainFunction->GetInputNode(_T("Morph"))->Connection(pMorphBlend->GetOutputNode(0));

}
SKMorphTreeTwoSmoothBlendAnim::~SKMorphTreeTwoSmoothBlendAnim()
{

}