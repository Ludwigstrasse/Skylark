#include "SKSkelecton.h"
#include "SKBoneNode.h"
#include "SKLineSet.h"
#include "SKSkelectonMeshNode.h"
#include "SKGeometryNode.h"
#include "SKGraphicInclude.h"
#include "SKViewFamily.h"
#include "SKDebugDraw.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSkelecton,SKNode)
SKREAL SKSkelecton::ms_fBoneAxisLength = 1.0f;
BEGIN_ADD_PROPERTY(SKSkelecton,SKNode)
REGISTER_PROPERTY(m_pBoneArray,BoneArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bIsDrawSkelecton,IsDrawSkelecton,SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_LocalBV, LocalAABB, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_OriginLocalBV, OriginLocalBV, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSkelecton)
IMPLEMENT_INITIAL_END
SKSkelecton::SKSkelecton()
{
	m_pBoneArray.Clear();
	m_bIsStatic = 0;
	m_bIsDrawSkelecton = false;


}
SKSkelecton::~SKSkelecton()
{
	m_pBoneArray.Clear();


}


unsigned int SKSkelecton::GetBoneNum()const
{

	return m_pBoneArray.GetNum();
}
int SKSkelecton::GetBoneIndex(const SKUsedName &Name)const
{

	for(unsigned int i = 0 ; i < m_pBoneArray.GetNum() ; i++)
	{
		if(m_pBoneArray[i]->m_cName == Name)
			return i;
	}
	return -1;
}
SKBoneNode *SKSkelecton::GetBoneNode(const SKUsedName& Name)const
{

	for(unsigned int i = 0 ; i < m_pBoneArray.GetNum() ; i++)
	{
		if(m_pBoneArray[i]->m_cName == Name)
			return m_pBoneArray[i];
	}
	return NULL;

}
SKBoneNode * SKSkelecton::GetBoneNode(unsigned int i)const
{
	if(i >= m_pBoneArray.GetNum())
		return NULL;
	return m_pBoneArray[i];
}
void SKSkelecton::CreateBoneArray()
{
	m_pBoneArray.Clear();
	SKArray<SKBoneNode *> Temp;
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ;i++)
	{
		SKBoneNode * pBoneNode = DynamicCast<SKBoneNode>(m_pChild[i]);
		if(pBoneNode)
		{				
			pBoneNode->GetAllBoneArray(Temp);
		}

	}
	for(unsigned int i = 0 ; i < Temp.GetNum() ; i++)
	{
		SKBoneNode * pBoneNode = DynamicCast<SKBoneNode>(Temp[i]);
		m_pBoneArray.AddElement(pBoneNode);

	}
}
void SKSkelecton::LinkBoneArray()
{
	m_pBoneArray.Clear();
	SKArray<SKBoneNode *> Temp;
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ;i++)
	{
		SKBoneNode * pBoneNode = DynamicCast<SKBoneNode>(m_pChild[i]);
		if(pBoneNode)
		{

			pBoneNode->GetAllBoneArray(Temp);
		}

	}
	for(unsigned int i = 0 ; i < Temp.GetNum() ; i++)
	{
		SKBoneNode * pBoneNode = DynamicCast<SKBoneNode>(Temp[i]);
		m_pBoneArray.AddElement(pBoneNode);

	}
}
void SKSkelecton::UpdateNoChild(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}


	UpdateTransform(dAppTime);

	if (m_bIsChanged)
	{
		UpdateWorldBound(dAppTime);
	}
	m_bIsChanged = false;
	
}
void SKSkelecton::UpdateWorldBound(double dAppTime)
{
	m_WorldBV.Transform(m_LocalBV, m_World.GetCombine());
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;
	}
}
void SKSkelecton::ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{

	UpDateView(Culler,dAppTime);
}
void SKSkelecton::UpDateView(SKCuller & Culler,double dAppTime)
{

	SKNode::UpDateView(Culler,dAppTime);
	
	if (Culler.GetCullerType() == SKCuller::CUT_MAIN)
	{
		Draw(Culler.GetCamera());
	}
	

	
}
void SKSkelecton::Draw(SKCamera * pCamera)
{
	if (!pCamera || !m_bIsDrawSkelecton)
	{
		return;
	}

	SKSkelectonMeshNode * pMesh = (SKSkelectonMeshNode *)m_pParent;
	if (!pMesh)
	{
		return ;
	}
	SKVector3 Dist = pCamera->GetWorldTranslate() - pMesh->GetWorldTranslate();
	ms_fBoneAxisLength = Dist.GetLength() * 0.05f;

	static SKArray<SKDebugDraw *> s_DebugDrawArray;
	s_DebugDrawArray.Clear();
	for (unsigned int i = 0 ; i < pCamera->GetViewFamilyNum() ;i++)
	{
		SKViewFamily * pViewFamily = pCamera->GetViewFamily(i);
		if (pViewFamily)
		{
			SKSceneRenderMethod * pRM = pViewFamily->m_pSceneRenderMethod;
			SKDebugDraw * pDebugDraw = pRM->GetDebugDraw(pMesh->GetRenderGroup());
			if (pDebugDraw && pDebugDraw->m_bEnable)
			{
				s_DebugDrawArray.AddElement(pDebugDraw);
			}
		}
	}
	
	for (unsigned int i = 0 ; i < m_pBoneArray.GetNum() ; i++)
	{
		SKBoneNode * pParent = DynamicCast<SKBoneNode>(m_pBoneArray[i]->GetParent());
		if(pParent)
		{
			SKVector3 P1 = m_pBoneArray[i]->GetWorldTranslate();

			SKVector3 P2 = m_pBoneArray[i]->GetParent()->GetWorldTranslate();
			for (unsigned int j = 0 ; j < s_DebugDrawArray.GetNum() ; j++)
			{
				s_DebugDrawArray[j]->AddDebugLine(P1,P2,SKColorRGBA(1.0f,1.0f,1.0f,1.0f).GetDWARGB(),false);
			}
		}
	}
	for (unsigned int i = 0 ; i < m_pBoneArray.GetNum() ; i++)
	{
		SKVector3 Axis[3];
		SKMatrix3X3 Rot = m_pBoneArray[i]->GetWorldRotate();
		Rot.GetUVN(Axis);
		Axis[0].Normalize();
		Axis[1].Normalize();
		Axis[2].Normalize();
		SKVector3 Pos = m_pBoneArray[i]->GetWorldTranslate();

		for (unsigned int j = 0 ; j < s_DebugDrawArray.GetNum() ; j++)
		{
			s_DebugDrawArray[j]->AddDebugLine(Pos,Pos + Axis[0] * ms_fBoneAxisLength
				,SKColorRGBA(1.0f,0.0f,0.0f,1.0f).GetDWARGB(),false);
		}

		for (unsigned int j = 0 ; j < s_DebugDrawArray.GetNum() ; j++)
		{
			s_DebugDrawArray[j]->AddDebugLine(Pos,Pos + Axis[1] * ms_fBoneAxisLength
				,SKColorRGBA(0.0f,1.0f,0.0f,1.0f).GetDWARGB(),false);
		}

		for (unsigned int j = 0 ; j < s_DebugDrawArray.GetNum() ; j++)
		{
			s_DebugDrawArray[j]->AddDebugLine(Pos,Pos + Axis[2] * ms_fBoneAxisLength
				,SKColorRGBA(0.0f,0.0f,1.0f,1.0f).GetDWARGB(),false);
		}
	}
}
const SKTransform &SKSkelecton::GetRootTransform()
{
	if (m_pChild.GetNum() > 0)
	{
		return m_pChild[0]->GetLocalTransform();
	}
	return SKTransform::ms_Indetity;
}
void SKSkelecton::CreateLocalAABB()
{
	SKVector3 MinPos(SKMAX_REAL, SKMAX_REAL, SKMAX_REAL);
	SKVector3 MaxPos(SKMIN_REAL, SKMIN_REAL, SKMIN_REAL);
	SKTransform SkelectonLocalT = GetLocalTransform();
	for (unsigned int j = 0; j < GetBoneNum(); j++)
	{
		SKBoneNode * pBone = GetBoneNode(j);
		if (pBone)
		{
			SKVector3 Pos = pBone->GetWorldTranslate() * SkelectonLocalT.GetCombineInverse();
			for (int t = 0; t < 3; t++)
			{
				if (MinPos.m[t] > Pos.m[t])
				{
					MinPos.m[t] = Pos.m[t];
				}

				if (MaxPos.m[t] < Pos.m[t])
				{
					MaxPos.m[t] = Pos.m[t];
				}
			}

		}
	}
	m_LocalBV.Set(MaxPos, MinPos);
	m_OriginLocalBV.Set(MaxPos, MinPos);
}