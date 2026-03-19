#include "SKBoneNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKBoneNode,SKNode)
BEGIN_ADD_PROPERTY(SKBoneNode,SKNode)
REGISTER_PROPERTY(m_OffSetMatrix, OffSetMatrix, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bIsEffector, IsEffector, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_TargetPosInWorld, TargetPosInWorld, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bAllowTranslation, AllowTranslation, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMinTranslation, MinTranslation, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMaxTranslation, MaxTranslation, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bAllowRotation, AllowRotation, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMinRotation, MinRotation, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fMaxRotation, MaxRotation, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fIKWeight,IKWeight,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_cName, Name, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKBoneNode)
IMPLEMENT_INITIAL_END
SKBoneNode::SKBoneNode()
{
	m_bIsEffector = false;
	m_bAllowTranslation.SetBufferNum(3);
	m_fMinTranslation.SetBufferNum(3);
	m_fMaxTranslation.SetBufferNum(3);
	m_bAllowRotation.SetBufferNum(3);
	m_fMinRotation.SetBufferNum(3);
	m_fMaxRotation.SetBufferNum(3);
	for (int i = 0; i < 3; i++)
	{
		m_bAllowTranslation[i] = false;
		m_fMinTranslation[i] = -SKMAX_REAL;
		m_fMaxTranslation[i] = SKMAX_REAL;
		m_bAllowRotation[i] = true;
		m_fMinRotation[i] = -SKPI;
		m_fMaxRotation[i] = SKPI;
	}
	m_fIKWeight = 1.0f;
}
SKBoneNode::~SKBoneNode()
{

}
void SKBoneNode::ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{
	return;
}
void SKBoneNode::GetIKMoveAxis(SKVector3 Axis[3])const
{
	if (m_pParent)
	{
		m_pParent->GetWorldRotate().GetUVN(Axis);
	}
	else
	{
		Axis[0] = SKVector3(1.0f,0.0f,0.0f);
		Axis[1] = SKVector3(0.0f,1.0f,0.0f);
		Axis[2] = SKVector3(0.0f,0.0f,1.0f);
	}

}
void SKBoneNode::ComputeIKLocalRotDelta(const SKMatrix3X3 & WorldRot,SKMatrix3X3 & LocalRotDelta)const
{
	if (m_pParent)
	{
		SKMatrix3X3 Inver;
		Inver.InverseOf(m_pParent->GetWorldRotate());
		LocalRotDelta = m_pParent->GetWorldRotate() * WorldRot * Inver;
	}
	else
	{
		LocalRotDelta = WorldRot;
	}

}
SKBoneNode * SKBoneNode::GetBoneNodeFromLevel(const SKUsedName & BoneName)
{
	SKBoneNode * pFind = NULL;
	if (m_cName == BoneName)
	{
		pFind = this;
	}
	else
	{
		for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			SKSpatial * pChild = m_pChild[i];
			pFind = ((SKBoneNode *)pChild)->GetBoneNodeFromLevel(BoneName);
			if (!pFind)
			{
				return pFind;
			}
		}
	}
	return pFind;
}
unsigned int SKBoneNode::GetAllBoneNum()const
{
	unsigned int uiLeafNum  = 1;
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{

		SKSpatial * pBone = m_pChild[i];
		uiLeafNum += ((SKBoneNode *)pBone)->GetAllBoneNum();


	}

	return uiLeafNum;
}
void SKBoneNode::SetLocalMat(const SKMatrix3X3W SKMat)
{
	m_bIsChanged = true;
	m_Local.SetMatrix(SKMat);
}
void SKBoneNode::GetAllBoneArray(SKArray<SKBoneNode *> & AllNodeArray)
{
	AllNodeArray.AddElement(this);
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		SKBoneNode * pBoneNode = DynamicCast<SKBoneNode>(m_pChild[i]);
		if (pBoneNode)
		{
			pBoneNode->GetAllBoneArray(AllNodeArray);
		}

		

	}
}