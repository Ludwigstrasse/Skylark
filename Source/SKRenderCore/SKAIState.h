#ifndef SKAISTATE_H
#define SKAISTATE_H
#include "SKObject.h"
#include "SKLogic.h"
#include "SKPutNode.h"
#include "SKName.h"
#include "SKResource.h"
namespace SKEngine2
{
	class SKStream;
	//״̬任һڲ任һġ
	//ͨupdate任ͨϢ任ǿȷϢ任״̬ǰ״̬ܲҪ任
	//¾Ͳܱ任
	class  SKAIStateInputNode : public SKInputNode
	{
		//RTTI
		DECLARE_RTTI;
	public:
		SKAIStateInputNode();
		virtual ~SKAIStateInputNode();
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		
	public:	

		virtual bool CheckState() = 0;

	};
	DECLARE_Ptr(SKAIStateInputNode);
	SKTYPE_MARCO(SKAIStateInputNode);
	class SKFSM;
	class  SKAIState : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		SKAIState(const SKUsedName &StateName,SKFSM * pOwner);
		virtual ~SKAIState() = 0;
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		
	public:	

		virtual bool BeginState() = 0;
		virtual bool EndState() = 0;
		virtual bool HandleMessage(SKMessage & Message);
		virtual bool Update(double Time);
		const SKUsedName & GetStateName()const
		{
			return m_StateName;
		}
	protected:
		SKOutputNode * m_pStateOutPut;
		SKArray<SKAIStateInputNode*> m_pStateInputNode;
		SKUsedName	m_StateName;
		SKAIState();
		SKFSM * m_pOwner;

	};
	DECLARE_Ptr(SKAIState);
	SKTYPE_MARCO(SKAIState);

	class  SKBeginState : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		SKBeginState(const SKUsedName &StateName);
		virtual ~SKBeginState() = 0;
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		
	public:	

		virtual bool BeginState();
		virtual bool EndState();
		virtual bool Update(double Time);
	protected:
		SKBeginState();
	};
	DECLARE_Ptr(SKBeginState);
	SKTYPE_MARCO(SKBeginState);

	DECLARE_Ptr(SKActor);

	class  SKFSM : public SKObject,public SKResource
	{
		//RTTI
		DECLARE_RTTI;
	public:
		SKFSM();
		virtual ~SKFSM() = 0;
	public:

		DECLARE_INITIAL_NO_CLASS_FACTORY

		

	public:	
		friend class SKAIState;
		virtual unsigned int GetResourceType()const
		{
			return RT_FSM;
		}
		virtual bool HandleMessage(SKMessage & Message)
		{
			if (m_pGlobleState)
			{
				m_pGlobleState->HandleMessage(Message);
			}
			if (m_pCurState)
			{
				m_pCurState->HandleMessage(Message);
			}	
		}
		virtual bool Update(double Time)
		{
			if (m_pGlobleState)
			{
				m_pGlobleState->Update(Time);
			}
			if (m_pCurState)
			{
				m_pCurState->Update(Time);
			}
		}

		SKAIState * GetState(const SKUsedName	& StateName)
		{
			for (unsigned int i = 0 ; i < m_AllState.GetNum() ; i++)
			{
				if (StateName == m_AllState[i]->GetStateName())
				{
					return m_AllState[i];
				}
			}
			return NULL;
		}
		SKAIState * GetBeginState()
		{
			for (unsigned int i = 0 ; i < m_AllState.GetNum() ; i++)
			{
				SKBeginState * pState = DynamicCast<SKBeginState>(m_AllState[i]);
				if (pState)
				{
					return m_AllState[i];
				}
			}
			return NULL;
		}

		virtual bool Reset()
		{
			m_pCurState = GetBeginState();
			m_pCurState->BeginState();
		}

	protected:
		bool ChangeState(SKAIState *m_pState)
		{
			if (m_pState)
			{
				m_pCurState->EndState();
				m_pState->BeginState();
				m_pCurState = m_pState;
				return true;
			}
			return false;
		}
		bool ChangeState(const SKUsedName & StateName)
		{
			SKAIState * pState = GetState(StateName);
			if (pState)
			{
				m_pCurState->EndState();
				pState->BeginState();
				m_pCurState = pState;
				return true;
			}
			return false;
		}
		SKArray<SKAIState *> m_AllState;
		SKAIState *m_pCurState;
		SKAIState *m_pGlobleState;
		SKAIState *m_pBeginState;
		SKUsedName	m_FSMName;
		SKActorPtr m_pActor;
		
	};
	DECLARE_Ptr(SKFSM);
	SKTYPE_MARCO(SKFSM);

}
#endif