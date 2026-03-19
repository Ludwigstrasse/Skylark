#ifndef SKQUEUE_H
#define SKQUEUE_H
#include "SKSystem.h"
#include "SKMemManager.h"
#include "SKContainer.h"
namespace SKEngine2
{
	template <class T>
	struct QueueElement
	{
	public:
		QueueElement()
		{	
			m_pNext = NULL;
			m_pFront = NULL;
		}
		~QueueElement()
		{
			m_pNext = NULL;
			m_pFront = NULL;
		}
		QueueElement * m_pNext;
		QueueElement * m_pFront;
		T Element;
	};
	template <class T,SKMemManagerFun MMFun = SKMemObject::GetMemManager>
	class SKQueue : public SKContainer<QueueElement<T>,MMFun>
	{
	public:

		SKQueue(bool bUnique = false);
		~SKQueue(); 

		void operator= (const SKQueue& Queue);

		FORCEINLINE unsigned int GetNum()const;

		void Enqueue(const T & Element);

		bool Dequeue(T & Element);
		
		bool GetTop(T & Element);

		void Clear();

		void Erase(const T & Element);

		bool Has(const T & Element);

// 		template <class N>
// 		void Sort(unsigned int uiBegin,unsigned int uiEnd,N Compare);
// 		void Sort(unsigned int uiBegin,unsigned int uiEnd);

	protected:
		
		QueueElement<T> * m_pHead;	
		QueueElement<T> * m_pTail;	
		unsigned int m_uiNum;
		bool	m_bUnique;


	private:

	};
	template <class T,SKMemManagerFun MMFun>
	SKQueue<T,MMFun>::~SKQueue()
	{
		Clear();	
	}

	template <class T,SKMemManagerFun MMFun>
	SKQueue<T,MMFun>::SKQueue(bool bUnique)
	{
		m_pHead = NULL;
		m_pTail = NULL;
		m_bUnique = bUnique;
		m_uiNum = 0;
	}
	template <class T,SKMemManagerFun MMFun>
	void SKQueue<T,MMFun>::Clear()
	{
		QueueElement<T> * pTemp = m_pHead;
		while(pTemp)
		{
			QueueElement<T> * pNext = pTemp->m_pNext;
			Delete(pTemp,1);
			pTemp = pNext;
		}
		m_pHead = NULL;
		m_pTail = NULL;
		m_uiNum = 0;
	}
	template <class T,SKMemManagerFun MMFun>
	unsigned int SKQueue<T,MMFun>::GetNum()const
	{
		return m_uiNum;
	}
	template <class T,SKMemManagerFun MMFun>
	void SKQueue<T,MMFun>::operator= (const SKQueue& Queue)
	{
		QueueElement<T> * pTemp = Queue.m_pHead;
		while(pTemp)
		{
			Enqueue(pTemp->Element);
			pTemp = pTemp->m_pNext;

		}
	}
	template <class T,SKMemManagerFun MMFun>
	void SKQueue<T,MMFun>::Enqueue(const T & Element)
	{
		if (m_bUnique)
		{
			if (Has(Element))
			{
				return;
			}
		}
		QueueElement<T>* pElem = New(1);
		SK_NEW (pElem) QueueElement<T>();
		pElem->Element = Element;
		pElem->m_pFront = NULL;
		pElem->m_pNext = m_pHead;
		if (m_pHead)
		{
			m_pHead->m_pFront = pElem;
		}
		m_pHead = pElem;
		if (!m_pTail)
		{
			m_pTail = pElem;
		}
		m_uiNum++;
	}
	template <class T,SKMemManagerFun MMFun>
	bool SKQueue<T,MMFun>::GetTop(T & Element)
	{
		if (!m_pTail)
		{
			return false;
		}
		Element = m_pTail->Element;
		return true;
	}
	template <class T,SKMemManagerFun MMFun>
	bool SKQueue<T,MMFun>::Dequeue(T & Element)
	{
		if (!m_pTail)
		{
			return false;
		}
		Element = m_pTail->Element;
		
		if(m_pTail == m_pHead)
		{		
			Delete(m_pTail,1);
			m_pTail = m_pHead = NULL;
		}
		else
		{
			m_pTail->m_pFront->m_pNext = NULL;
			QueueElement<T> * pTemp = m_pTail;
			m_pTail = m_pTail->m_pFront;
			Delete(pTemp,1);
		}
		
		m_uiNum--;
		return true;
	}
	template <class T,SKMemManagerFun MMFun>
	void SKQueue<T,MMFun>::Erase(const T & Element)
	{
		QueueElement<T> * pTemp = m_pHead;
		while(pTemp)
		{
			if (pTemp->Element == Element)
			{
				if (!pTemp->m_pFront && !pTemp->m_pNext)
				{
					m_pHead = NULL;
					m_pTail = NULL;
				}
				else if (!pTemp->m_pFront)
				{
					pTemp->m_pNext->m_pFront = NULL;
				}
				else if(!pTemp->m_pNext)
				{
					pTemp->m_pNext->m_pFront = pTemp->m_pFront;
				}
				else
				{
					pTemp->m_pNext->m_pFront = pTemp->m_pFront;
					pTemp->m_pFront->m_pNext = pTemp->m_pNext;
				}
				Delete(pTemp,1);
				m_uiNum--;
				if (m_bUnique)
				{
					continue;
				}
				else
				{
					break;
				}
				
			}
			pTemp = pTemp->m_pNext;

		}
	}
	template <class T,SKMemManagerFun MMFun>
	bool SKQueue<T,MMFun>::Has(const T & Element)
	{
		QueueElement<T> * pTemp = m_pHead;
		while(pTemp)
		{
			if (pTemp->Element == Element)
			{
				return true;
			}
			pTemp = pTemp->m_pNext;
		}
		return false;
	}
}
#endif