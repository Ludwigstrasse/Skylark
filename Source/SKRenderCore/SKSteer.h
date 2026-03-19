#ifndef SKSTEER_H
#define SKSTEER_H
#include "SKActor.h"
namespace SKEngine2
{
	
	class SKSteerComponent : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		friend class SKSteer;
	public:
		SKSteerComponent();
		virtual ~SKSteerComponent() = 0;
		FORCEINLINE SKSteer * GetOwner()const
		{
			return m_pSteer;
		}
		//
		virtual SKVector3 Compute() = 0;
	protected:
		SKSteer * m_pSteer;
		unsigned int m_uiPrior;
		SKREAL		 m_fWeight;
	};
	DECLARE_Ptr(SKSteerComponent);

	class SKSteer : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		friend class SKActor;
		SKSteer();
		virtual	~SKSteer();
		FORCEINLINE SKActor * GetOwner()const
		{
			return m_pActor;
		}
		
		template<class T>
		T * GetSteerComponet();
		template<class T>
		void AddSteerComponent(unsigned int uiPrior ,SKREAL fWeight);

		template<class T>
		void DeleteSteerComponent();
		virtual SKVector3 Compute();
	protected:
		SKActor * m_pActor;
		SKArray<SKSteerComponentPtr> m_ComponentArray;
		bool AddForce(SKVector3 & CurForce,SKVector3 & AddForce);
		class PriorityCompare
		{
		public:
			FORCEINLINE bool operator()(SKSteerComponentPtr & e1,SKSteerComponentPtr& e2)
			{
				
				return e1->m_uiPrior <= e2->m_uiPrior;
			}

		};
		
	};
	DECLARE_Ptr(SKSteer);
	template<class T>
	void SKSteer::AddSteerComponent(unsigned int uiPrior ,SKREAL fWeight)
	{
		if (!T::ms_Type.IsDerived(SKSteerComponent::ms_Type))
		{
			return ;
		}
		for (unsigned int i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				continue;
			}
		}
		T * pComponent = SK_NEW T();
		SKSteerComponent * pTemp = StaticCast<SKSteerComponent>(pComponent);
		pTemp->m_pSteer = this;
		pTemp->m_fWeight = fWeight;
		pTemp->m_uiPrior = uiPrior;
		m_ComponentArray.AddElement(pComponent);
		m_ComponentArray.Sort(0,m_ComponentArray.GetNum(),PriorityCompare())
	}
	template<class T>
	T * SKSteer::GetSteerComponet()
	{
		for (unsigned int i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				return StaticCast<T>(m_ComponentArray[i]);
			}
		}
	}

	template<class T>
	void SKSteer::DeleteSteerComponent()
	{
		for (unsigned int i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				m_ComponentArray[i] = NULL;
				m_ComponentArray.Erase(i);
				return ;
			}
		}
	}
}
#endif