#ifndef SKACTOR_H
#define SKACTOR_H
#include "SKLogic.h"
#include "SKObject.h"
#include "SKVector3.h"
#include "SKQuat.h"
#include "SKNode.h"
#include "SKNodeComponent.h"
#define GET_TYPE_NODE(nodeclass)\
	friend class nodeclass;\
	nodeclass * GetTypeNode()const\
	{\
		SKNode * pNode = m_pNode; \
		return(nodeclass*)pNode;\
	}
namespace SKEngine2
{
	DECLARE_Ptr(SKFSM);
	DECLARE_Ptr(SKSteer);
	class SKSceneMap;

	class SKGRAPHIC_API SKActor : public SKObject,public SKResource
	{
		//RTTI
		DECLARE_RTTI;

	public:
		SKActor();
		virtual ~SKActor();

		DECLARE_INITIAL
	public:
		virtual void SetWorldPos(const SKVector3 & Pos);
		virtual void SetWorldScale(const SKVector3 &Scale);
		virtual void SetWorldRotate(const SKMatrix3X3 & Rotate);

		virtual void SetLocalPos(const SKVector3 & Pos);
		virtual void SetLocalScale(const SKVector3 &Scale);
		virtual void SetLocalRotate(const SKMatrix3X3 & Rotate);

		SKVector3 GetWorldPos();
		SKVector3 GetWorldScale();
		SKMatrix3X3 GetWorldRotate();

		SKVector3 GetLocalPos();
		SKVector3 GetLocalScale();
		SKMatrix3X3 GetLocalRotate();

		
		virtual void ProcessInput(unsigned int uiInputType,unsigned int uiEvent,unsigned int uiKey,int x, int y, int z);
		
		virtual bool HandleMessage(SKMessage & Message);



		SKNode * GetActorNode()const
		{
			return m_pNode;
		}
		
		SKVector3 GetVelocity()const
		{
			return m_Velocity;
		}
		SKREAL GetMaxVelocity()const
		{
			return m_fMaxVelocity;
		}
		SKREAL GetMaxDriverForce()const
		{
			return m_fMaxDriverForce;
		}

		virtual void Update(double dAppTime);
		virtual bool PostClone(SKObject * pObjectSrc);
		friend class SKWorld;
		friend class SKSceneMap;
	protected:
		SKVector3	m_Velocity;
		SKREAL		m_fMaxVelocity;
		SKREAL		m_fMaxDriverForce;

		SKNodePtr		m_pNode;
		SKFSMPtr		m_pFSM;
		SKSteerPtr		m_pSteerPtr;
		
		SKActor *		m_pOwner;
	public:
		SKUsedName		m_ActorName;
		SKSceneMap *	m_pSceneMap;

	public:
		virtual void OnDestory();
		SKActor *GetOwner();
		virtual void AddChildActor(SKActor * pActor);
		virtual void DeleteChildActor(SKActor * pActor);
		virtual SKActor * GetChildActor(unsigned int uiActorIndex);
		virtual void DeleteChildActor(unsigned int uiActorIndex);
		template<typename T>
		T * AddComponentNode();
		template<typename T>
		void GetComponentNode(SKArray<T*>& Node);
		void DeleteComponentNode(SKNodeComponent * pComponent);
		void ChangeComponentNodeParent(SKNodeComponent * pSource, SKNode * pParent = NULL);
		void AddActorNodeToNode(SKActor * pActor, SKNodeComponent * pNode);
	protected:
		virtual void AddToSceneMap(SKSceneMap * pSceneMap);
		virtual void CreateDefaultComponentNode();

	protected:
		SKArray<SKActor *> m_ChildActor;
		
		SKArray<SKNodeComponentPtr> m_pNodeComponentArray; // not include root node
		static SKPointer<SKActor> Default;
	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual unsigned int GetResourceType()const
		{
			return RT_ACTOR;
		}
		static const SKActor *GetDefalut()
		{
			return Default;
		}
	};
	DECLARE_Ptr(SKActor);
	SKTYPE_MARCO(SKActor);
	DECLARE_Proxy(SKActor);
	template<typename T>
	T * SKActor::AddComponentNode()
	{
		T * pNode = SKNodeComponent::CreateComponet<T>();
		m_pNode->AddChild(pNode);
		m_pNodeComponentArray.AddElement(pNode);
		return pNode;
	}
	template<typename T>
	void SKActor::GetComponentNode(SKArray<T*>& Node)
	{
		for (unsigned int i = 0; i < m_pNodeComponentArray.GetNum();i++)
		{
			if (m_pNodeComponentArray[i]->GetType().IsSameType(T::GetType()))
			{
				Node.AddElement(m_pNodeComponentArray[i]);
			}
		}
	}
}
#endif