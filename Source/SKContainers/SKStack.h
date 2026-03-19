#ifndef SKSTACK
#define SKSTACK
#include "SKArray.h"
namespace SKEngine2
{
	template <class T,SKMemManagerFun MMFun = SKMemObject::GetMemManager>
	class SKStack
	{
	public:
		SKStack();
		~SKStack();
		bool GetTop(T & Element);
		void Push(const T & Element);
		bool Pop(T & Element);
		unsigned int GetNum();
	protected:
		SKArray<T,MMFun> m_Array;
	};
	template <class T,SKMemManagerFun MMFun>
	SKStack<T,MMFun>::SKStack()
	{
	}
	template <class T,SKMemManagerFun MMFun>
	SKStack<T,MMFun>::~SKStack()
	{

	}
	template <class T,SKMemManagerFun MMFun>
	bool SKStack<T,MMFun>::GetTop(T & Element)
	{
		if (m_Array.GetNum() == 0)
		{
			return false;
		}
		Element = m_Array[m_Array.GetNum() - 1];
		return true;
	}
	template <class T,SKMemManagerFun MMFun>
	void SKStack<T,MMFun>::Push(const T & Element)
	{
		m_Array.AddElement(Element);
	}
	template <class T,SKMemManagerFun MMFun>
	bool SKStack<T,MMFun>::Pop(T & Element)
	{
		if (m_Array.GetNum() == 0)
		{
			return false;
		}
		Element = m_Array[m_Array.GetNum() - 1];
		m_Array.Erase(m_Array.GetNum() - 1);
		return true;
	}
	template <class T,SKMemManagerFun MMFun>
	unsigned int SKStack<T,MMFun>::GetNum()
	{
		return m_Array.GetNum();
	}
}
#endif