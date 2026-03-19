#ifndef SKSAFEQUEUE_H
#define SKSAFEQUEUE_H
#include "SKQueue.h"
#include "SKSynchronize.h"
namespace SKEngine2
{

	template <class T>
	class SKSafeQueue : public SKMemObject
	{
	public:

		SKSafeQueue(bool bUnique = false);
		~SKSafeQueue(); 
		void Enqueue(const T & Element);
		void Dequeue(T & Element);
		void GetTop(T & Element);
		void Clear();
		void Erase(const T & Element);
		bool IsEmpty();
	protected:
		SKQueue<T> m_Queue; 
		SKCriticalSection m_CriticalSec;

	private:

	};
	template <class T>
	SKSafeQueue<T>::~SKSafeQueue()
	{
		
	}

	template <class T>
	SKSafeQueue<T>::SKSafeQueue(bool bUnique)
	{
	}
	template <class T>
	void SKSafeQueue<T>::Clear()
	{
		m_CriticalSec.Lock();
		m_Queue.Clear();
		m_CriticalSec.Unlock();
	}

	template <class T>
	void SKSafeQueue<T>::Enqueue(const T & Element)
	{

		m_CriticalSec.Lock();
		m_Queue.Enqueue(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	void SKSafeQueue<T>::GetTop(T & Element)
	{
		m_CriticalSec.Lock();
		m_Queue.GetTop(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	void SKSafeQueue<T>::Dequeue(T & Element)
	{
		m_CriticalSec.Lock();
		m_Queue.Dequeue(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	void SKSafeQueue<T>::Erase(const T & Element)
	{
		m_CriticalSec.Lock();
		m_Queue.Erase(Element);
		m_CriticalSec.Unlock();
	}
	template <class T>
	bool SKSafeQueue<T>::IsEmpty()
	{
		bool bEmpty = false;
		m_CriticalSec.Lock();
		if (m_Queue.GetNum() == 0)
		{
			bEmpty = true;
		}
		m_CriticalSec.Unlock();
		return bEmpty;
		
	}
}
#endif