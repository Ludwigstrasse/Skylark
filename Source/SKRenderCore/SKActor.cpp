#include "SKActor.h"
#include "SKAIState.h"
#include "SKSteer.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKWorld.h"
using namespace SKEngine2;
SKPointer<SKActor> SKActor::Default = NULL;
bool SKActor::ms_bIsEnableASYNLoader = true;
bool SKActor::ms_bIsEnableGC = true;
IMPLEMENT_RTTI(SKActor, SKObject)
BEGIN_ADD_PROPERTY(SKActor,SKObject)
REGISTER_PROPERTY(m_ChildActor, ChildActor, SKProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pOwner, Owner, SKProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pSceneMap, SceneMap, SKProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_ActorName, ActorName, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pNode, pNode, SKProperty::F_SAVE_LOAD_CLONE  | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pNodeComponentArray, NodeComponentArray, SKProperty::F_SAVE_LOAD_CLONE  | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKActor)
IMPLEMENT_INITIAL_END

SKActor::SKActor()
{
	m_pOwner = NULL;
	m_pNode = NULL;
	m_pSceneMap = NULL;
}
void SKActor::OnDestory()
{
	if (m_pSceneMap)
	{
		m_pSceneMap->DeleteActor(this);
	}
}
SKActor::~SKActor()
{
	m_pNode = NULL;
	m_pSceneMap = NULL;
}
void SKActor::ProcessInput(unsigned int uiInputType,unsigned int uiEvent,unsigned int uiKey,int x, int y, int z)
{
	return;
}
void SKActor::SetWorldPos(const SKVector3 & Pos)
{
	m_pNode->SetWorldTranslate(Pos);
}
void SKActor::SetWorldScale(const SKVector3 &Scale)
{
	m_pNode->SetWorldScale(Scale);
}
void SKActor::SetWorldRotate(const SKMatrix3X3 & Rotate)
{
	m_pNode->SetWorldRotate(Rotate);
}
void SKActor::SetLocalPos(const SKVector3 & Pos)
{

	m_pNode->SetLocalTranslate(Pos);

}
void SKActor::SetLocalScale(const SKVector3 &Scale)
{
	
	m_pNode->SetLocalScale(Scale);

}
void SKActor::SetLocalRotate(const SKMatrix3X3 & Rotate)
{

	m_pNode->SetLocalRotate(Rotate);

}
SKVector3 SKActor::GetWorldPos()
{

	return m_pNode->GetWorldTranslate();

}
SKVector3 SKActor::GetWorldScale()
{

	return m_pNode->GetWorldScale();

}
SKMatrix3X3 SKActor::GetWorldRotate()
{

	return m_pNode->GetWorldRotate();

}

SKVector3 SKActor::GetLocalPos()
{
	return m_pNode->GetLocalTranslate();
}
SKVector3 SKActor::GetLocalScale()
{
	return m_pNode->GetLocalScale();
}
SKMatrix3X3 SKActor::GetLocalRotate()
{
	return m_pNode->GetLocalRotate();
}

bool SKActor::HandleMessage(SKMessage & Message)
{
	if (m_pFSM)
	{
		m_pFSM->HandleMessage(Message);
	}
	return true;
}
void SKActor::Update(double dAppTime)
{

}
bool SKActor::PostClone(SKObject * pObjectSrc)
{
	SKResourceManager::AddGCObject(this);
	SKResourceManager::AddGCObject(m_pNode);
	for (unsigned int i = 0; i < m_pNodeComponentArray.GetNum();i++)
	{
		SKResourceManager::AddGCObject(m_pNodeComponentArray[i]);
	}
	return true;
}
void SKActor::AddActorNodeToNode(SKActor * pActor, SKNodeComponent * pNode)
{
	bool Can = false;
	for (unsigned int i = 0; i < m_pNodeComponentArray.GetNum(); i++)
	{
		if (m_pNodeComponentArray[i] == pNode)
		{
			Can = true;
			break;
		}
	}
	
	if (!Can)
	{
		return;
	}
	unsigned int i = 0;
	for (; i < m_ChildActor.GetNum();i++)
	{
		if (pActor == m_ChildActor[i])
		{
			break;
		}
	}
	if (i == m_ChildActor.GetNum())
	{
		m_pSceneMap->AddActor(pActor);
		m_ChildActor.AddElement(pActor);
	}
	

	m_pSceneMap->GetScene()->DeleteObject(pActor->GetActorNode());
	pNode->AddChild(pActor->GetActorNode());
	pActor->m_pOwner = this;

}
void SKActor::AddChildActor(SKActor * pActor)
{
	if (pActor)
	{
		unsigned int i = 0;
		for (; i < m_ChildActor.GetNum(); i++)
		{
			if (pActor == m_ChildActor[i])
			{
				break;
			}
		}
		if (i == m_ChildActor.GetNum())
		{
			m_pSceneMap->AddActor(pActor);
			m_ChildActor.AddElement(pActor);
		}
		SKNode * pNode = DynamicCast<SKNode>(m_pNode);
		m_pSceneMap->GetScene()->DeleteObject(pActor->GetActorNode());
		pNode->AddChild(pActor->GetActorNode());
		pActor->m_pOwner = this;

	}

}
void SKActor::DeleteChildActor(SKActor * pActor)
{
	for (unsigned int i = 0; i < m_ChildActor.GetNum(); i++)
	{
		if (m_ChildActor[i] == pActor)
		{
			
			SKNode * pNode = DynamicCast<SKNode>(pActor->GetActorNode()->GetParent());
			pNode->DeleteChild(pActor->GetActorNode());
			pActor->m_pOwner = NULL;
			m_ChildActor.Erase(i);
			return;
		}
	}
}
SKActor * SKActor::GetChildActor(unsigned int uiActorIndex)
{
	if (uiActorIndex < m_ChildActor.GetNum())
	{
		return m_ChildActor[uiActorIndex];
	}
	return NULL;
}
void SKActor::DeleteChildActor(unsigned int uiActorIndex)
{
	if (uiActorIndex < m_ChildActor.GetNum())
	{
		SKNode * pNode = DynamicCast<SKNode>(m_ChildActor[uiActorIndex]->GetActorNode()->GetParent());
		pNode->DeleteChild(m_ChildActor[uiActorIndex]->GetActorNode());
		m_ChildActor[uiActorIndex]->m_pOwner = NULL;
		m_ChildActor.Erase(uiActorIndex);
	}
}
SKActor *SKActor::GetOwner()
{
	return m_pOwner;
}
void SKActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKNodeComponent>();
}
void SKActor::AddToSceneMap(SKSceneMap * pSceneMap)
{
	m_pSceneMap = pSceneMap;
}
void SKActor::DeleteComponentNode(SKNodeComponent * pComponent)
{
	if (pComponent == m_pNode)
	{
		return;
	}

	SKNodeComponentPtr Temp = NULL;

	for (unsigned int i = 0; i < m_pNodeComponentArray.GetNum(); i++)
	{
		if (m_pNodeComponentArray[i] == pComponent)
		{
			Temp = pComponent;
			m_pNodeComponentArray.Erase(i);
			break;
		}
	}
	if (Temp == NULL)
	{
		return;
	}
	pComponent->SetFlag(SKObject::OF_PendingKill);
	pComponent->OnDestroy();
	SKArray<SKSpatialPtr> ChildList = *pComponent->GetChildList();
	pComponent->DeleteAllChild();
	for (unsigned int i = 0; i < ChildList.GetNum();i++)
	{
		SKNode *pNode = DynamicCast<SKNode>(pComponent->GetParent());
		pNode->AddChild(ChildList[i]);
	}
	
	
}
void SKActor::ChangeComponentNodeParent(SKNodeComponent * pSource, SKNode * pParent)
{
	if (pSource == m_pNode || pSource->GetParent() == pParent)
	{
		return;
	}
	if (m_pNodeComponentArray.FindElement(pSource) >= m_pNodeComponentArray.GetBufferNum())
	{
		return;
	}

	if (!pParent)
	{
		pParent = m_pNode;
	}

	if (pParent != m_pNode)
	{
		SKNodeComponent * pParentNode = DynamicCast<SKNodeComponent>(pParent);
		if (pParentNode)
		{
			if (m_pNodeComponentArray.FindElement(pParentNode) >= m_pNodeComponentArray.GetBufferNum())
			{
				return;
			}
		}
	}

	SKNode * pParentNode = DynamicCast<SKNode>(pSource->GetParent());
	SKSpatialPtr pTemp = pSource;
	pParentNode->DeleteChild(pSource);
	pParent->AddChild(pSource);
}






