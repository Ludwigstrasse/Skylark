#include "SKSkelectonMeshNode.h"
#include "SKBoneNode.h"
#include "SKAnimTree.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSkelectonMeshNode,SKModelMeshNode)
BEGIN_ADD_PROPERTY(SKSkelectonMeshNode,SKModelMeshNode)
REGISTER_PROPERTY(m_pSkelecton,Skelecton,SKProperty::F_SAVE_LOAD_CLONE| SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pAnimSet, AnimSet, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pAnimTree, pAnimTree, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pSocketArray, SocketArray, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSkelectonMeshNode)
ADD_PRIORITY(SKGeometry)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
SKSkelectonMeshNodePtr SKSkelectonMeshNode::Default = NULL;
bool SKSkelectonMeshNode::ms_bIsEnableASYNLoader = true;
bool SKSkelectonMeshNode::ms_bIsEnableGC = true;
SKSkelectonMeshNode::SKSkelectonMeshNode()
{
	m_pSkelecton = NULL;
	m_pAnimSet = NULL;
	m_pAnimSequence = NULL;

	m_pAnimTree = NULL;
	m_pAnimTreeInstance = NULL;
	
}
SKSkelectonMeshNode::~SKSkelectonMeshNode()
{
	if (m_pAnimSet)
	{
		m_pAnimSet->m_AddAnimEvent.RemoveMethod<SKSkelectonMeshNode, &SKSkelectonMeshNode::UpdateLocalAABB>(&(*this));
	}
	m_pSkelecton = NULL;
	m_pAnimSet = NULL;
	m_pAnimSequence = NULL;
	m_pAnimTree = NULL;
	m_pAnimTreeInstance = NULL;

}
bool SKSkelectonMeshNode::InitialDefaultState()
{
	Default = SK_NEW SKSkelectonMeshNode();
	SKGeometryNodePtr GeometryNode = SK_NEW SKGeometryNode();
	Default->AddChild(GeometryNode);
	SKGeometryPtr Geometry = (SKGeometry *)SKObject::CloneCreateObject(SKGeometry::GetDefaultRenderCube());
	GeometryNode->AddChild(Geometry);
	Default->CreateLocalAABB();
	GeometryNode->SetLocalScale(SKVector3(100.0f, 100.0f, 100.0f));

	return true;
}
bool SKSkelectonMeshNode::TerminalDefaultState()
{
	Default = NULL;
	return true;
}
void SKSkelectonMeshNode::SetSkelecton(SKSkelecton * pSkelecton)
{	
	if(m_pSkelecton)
	{
		m_pSkelecton->m_pParent = NULL;
	}
	m_pSkelecton = pSkelecton;
	m_pSkelecton->m_pParent = this;
	
}
SKSocketNode * SKSkelectonMeshNode::GetSocket(const SKUsedName &SocketName)
{
	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			return m_pSocketArray[i];
		}
	}
	return NULL;
}
SKSocketNode * SKSkelectonMeshNode::CreateSocket(const SKUsedName & BoneName, const SKUsedName &SocketName)
{
	if (!m_pSkelecton)
	{
		return NULL;
	}
	SKBoneNode * pBone = m_pSkelecton->GetBoneNode(BoneName);
	if (!pBone)
	{
		return NULL;
	}

	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			return NULL;
		}
	}

	SKSocketNode * pSocketNode = SK_NEW SKSocketNode();
	pBone->AddChild(pSocketNode);
	pSocketNode->m_cName = SocketName;
	m_pSocketArray.AddElement(pSocketNode);
	return pSocketNode;
}
void SKSkelectonMeshNode::DeleteSocket(const SKUsedName &SocketName)
{
	for (unsigned int i = 0; i < m_pSocketArray.GetNum(); i++)
	{
		if (m_pSocketArray[i]->m_cName == SocketName)
		{
			SKBoneNode * pSocketParent = (SKBoneNode *)(m_pSocketArray[i]->GetParent());
			SKMAC_ASSERT(pSocketParent);
			pSocketParent->DeleteChild(m_pSocketArray[i]);
			m_pSocketArray.Erase(i);
			return;
		}
	}
}
void SKSkelectonMeshNode::SetAnimSet(SKAnimSet * pAnimSet)
{
	if (m_pAnimSet == pAnimSet)
	{
		return;
	}
	if (m_pAnimSet)
	{
		m_pAnimSet->m_AddAnimEvent.RemoveMethod<SKSkelectonMeshNode, &SKSkelectonMeshNode::UpdateLocalAABB>(&(*this));
	}
	m_pAnimSet = pAnimSet;
	m_pAnimSet->m_AddAnimEvent.AddMethod<SKSkelectonMeshNode, &SKSkelectonMeshNode::UpdateLocalAABB>(&(*this));
	if (m_pAnimTreeInstance)
	{
		m_pAnimTreeInstance->ResetAnimFunction();
	}
	UpdateLocalAABB();
}
void SKSkelectonMeshNode::UpdateLocalAABB()
{
	if (m_pAnimSet && m_pSkelecton)
	{
		SKArray<SKMatrix3X3W> SaveBoneMatrix;
		SaveBoneMatrix.AddBufferNum(m_pSkelecton->GetBoneNum());
		for (unsigned int i = 0; i < m_pSkelecton->GetBoneNum(); i++)
		{
			SKBoneNode * pBone = m_pSkelecton->GetBoneNode(i);
			if (pBone)
			{
				SKTransform T = pBone->GetLocalTransform();
				SaveBoneMatrix[i] = T.GetCombine();
			}
		}

		
		SKVector3 MaxPos = m_pSkelecton->m_OriginLocalBV.GetMaxPoint();
		SKVector3 MinPos = m_pSkelecton->m_OriginLocalBV.GetMinPoint();
		SKTransform SkelectonLocalT = m_pSkelecton->GetLocalTransform();
		for (unsigned int i = 0; i < m_pAnimSet->GetAnimNum(); i++)
		{
			SKAnimR * pAnimR = m_pAnimSet->GetAnim(i);
			while (!pAnimR->IsLoaded()){};

			SKAnim * pAnim = pAnimR->GetResource();
			SKREAL AnimLength = pAnim->GetAnimLength();

			PlayAnim(pAnim->m_cName.GetString(), 1.0f, SKController::RT_CLAMP);

			for (SKREAL f = 0.0f; f < AnimLength + 0.05f; f += 0.05f)
			{
				m_pAnimSequence->Update(f);
				m_pSkelecton->UpdateAll(0.0f);
				for (unsigned int j = 0; j < m_pSkelecton->GetBoneNum(); j++)
				{
					SKBoneNode * pBone = m_pSkelecton->GetBoneNode(j);
					if (pBone)
					{
						//change to local space
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
				
			}
		}
		m_pAnimSequence = NULL;
		for (unsigned int i = 0; i < m_pSkelecton->GetBoneNum(); i++)
		{
			SKBoneNode * pBone = m_pSkelecton->GetBoneNode(i);
			if (pBone)
			{
				pBone->SetLocalMat(SaveBoneMatrix[i]);
			}
		}
		SKAABB3 SkelectonBV;
		SkelectonBV.Set(MaxPos, MinPos);
		m_pSkelecton->SetLocalBV(SkelectonBV);
	}
}
void SKSkelectonMeshNode::SetAnimTree(SKAnimTreeR * pAnimTree)
{
	if(pAnimTree)
	{
		m_pAnimTree = pAnimTree;
		m_pAnimTree->AddLoadEventObject(this);
	}
}

void SKSkelectonMeshNode::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	if (m_pAnimTree == pResourceProxy)
	{
		m_pAnimTreeInstance = (SKAnimTree *)SKObject::CloneCreateObject(m_pAnimTree->GetResource());
		m_pAnimTreeInstance->SetObject(this);
	}
}
bool SKSkelectonMeshNode::PostLoad(void * pDate)
{
	SKModelMeshNode::PostLoad(pDate);
	if (m_pAnimTree)
	{
		m_pAnimTree->AddLoadEventObject(this);
	}
	
	return true;
}
bool SKSkelectonMeshNode::PostClone(SKObject * pObjectSrc)
{
	SKModelMeshNode::PostClone(pObjectSrc);
	if (m_pAnimTree)
	{
		m_pAnimTree->AddLoadEventObject(this);
	}
	return true;
}
void SKSkelectonMeshNode::SetAnimTreeNodePara(const SKUsedName & ShowName, void * pPara)
{
	if (m_pAnimTreeInstance)
	{
		m_pAnimTreeInstance->SetNodePara(ShowName, pPara);
	}
}
void SKSkelectonMeshNode::UpdateAll(double dAppTime)
{
	SKModelMeshNode::UpdateAll(dAppTime);
}
void SKSkelectonMeshNode::UpdateNodeAll(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}
	
	
	UpdateTransform(dAppTime);

	// no visible no update
	if (m_bEnable && m_pSkelecton)
	{
		m_pSkelecton->UpdateNodeAll(dAppTime);
	}
	else if (m_pSkelecton)
	{
		m_pSkelecton->UpdateNoChild(dAppTime);
	}
	
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)	
	{ 
		if (m_pChild[i]) 
			m_pChild[i]->UpdateNodeAll(dAppTime); 
	} 

	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;

}
void SKSkelectonMeshNode::ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{

	if (!Culler.CullConditionNode(this))
	{
		UpDateView(Culler,dAppTime);
		for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			if(m_pChild[i])
			{

				m_pChild[i]->ComputeVisibleSet(Culler,true,dAppTime);


			}

		}
		if (m_pSkelecton)
		{
			m_pSkelecton->ComputeNodeVisibleSet(Culler, bNoCull, dAppTime);
		}
		
	}

}
void SKSkelectonMeshNode::UpdateWorldBound(double dAppTime)
{
	bool bFoundFirstBound = false; 
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			if(!bFoundFirstBound)
			{
				m_WorldBV = m_pChild[i]->m_WorldBV;
				bFoundFirstBound = true; 
			}
			else
			{

				m_WorldBV = m_WorldBV.MergAABB(m_pChild[i]->m_WorldBV);
			}
		}

	}
	if (m_pSkelecton)
	{
		if(!bFoundFirstBound)
		{
			m_WorldBV = m_pSkelecton->m_WorldBV;
			bFoundFirstBound = true; 
		}
		else
		{
			SKVector3 MaxPos = m_pSkelecton->m_WorldBV.GetMaxPoint();
			SKVector3 MinPos = m_pSkelecton->m_WorldBV.GetMinPoint();

			SKREAL fA[3];
			m_WorldBV.GetfA(fA);

			MaxPos = MaxPos + SKVector3(fA[0], fA[1], fA[2]);
			MinPos = MinPos - SKVector3(fA[0], fA[1], fA[2]);
			m_WorldBV.Set(MaxPos, MinPos);
		}
	}
	if (!bFoundFirstBound)
	{
		SKREAL fA[3];
		m_WorldBV.GetfA(fA);
		m_WorldBV.Set(GetWorldTranslate(), fA);
	}
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;
	}
}
bool SKSkelectonMeshNode::PlayAnim(const SKString & AnimName,SKREAL fRatio,unsigned int uiRepeatType)
{
	if (m_pAnimSequence == NULL)
	{
		m_pAnimSequence = SK_NEW SKAnimSequenceFunc(this);

		m_bIsStatic = false;

		

	}

	m_pAnimSequence->m_bEnable = true;
	m_pAnimSequence->SetAnim(AnimName);
	m_pAnimSequence->m_uiRepeatType = uiRepeatType;
	m_pAnimSequence->m_dFrequency = fRatio;

	return 1;;
}
void SKSkelectonMeshNode::StopAnim()
{
	if (m_pAnimSequence)
	{
		m_pAnimSequence->m_bEnable = false;
	}
}
void SKSkelectonMeshNode::UpdateController(double dAppTime)
{
	SKModelMeshNode::UpdateController(dAppTime);
	if (m_pAnimTreeInstance && m_bEnable)
	{
		if (m_pAnimSequence)
		{
			m_pAnimSequence->m_bEnable = false;
		}
		m_pAnimTreeInstance->Update(dAppTime);
	}
	if(m_pAnimSequence && m_bEnable && m_pAnimSequence->m_bEnable)
	{
		m_pAnimSequence->ClearFlag();
		m_pAnimSequence->Update(dAppTime);
		m_pAnimSequence->UpDateBone();
	}

}
