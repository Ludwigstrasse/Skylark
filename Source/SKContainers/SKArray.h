#ifndef SKARRAY_H
#define SKARRAY_H
#include "SKContainer.h"
namespace SKEngine2
{
	
	template <class T,SKMemManagerFun MMFun = SKMemObject::GetMemManager>
	class SKArray : public SKContainer<T,MMFun>
	{
	public:
		enum
		{
			DEFAULT_GROWBY = 10
		};
		SKArray(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~SKArray();
		SKArray(const SKArray& Array);
		void SetBufferNum(unsigned int uiBufferNum);
		void AddBufferNum(unsigned int uiBufferNum);
		void operator= (const SKArray<T,MMFun>& Array);


		FORCEINLINE unsigned int GetNum()const;

		FORCEINLINE unsigned int GetBufferNum()const;

		FORCEINLINE T * GetBuffer()const;
		FORCEINLINE void SetGrowBy(unsigned int uiGrowBy);
		void AddElement(const T & Element);
		template <class N,SKMemManagerFun MMFunN>
		void AddElement(const SKArray<N,MMFunN> & Array,unsigned int uiBegin,unsigned int uiEnd);
		T& operator[] (unsigned int i)const;

		void Clear();

		FORCEINLINE unsigned int GetSize()const;

		void Erase(unsigned int i);
	
		void Erase(unsigned int uiBegin,unsigned int uiEnd);

		template <class N>
		void Sort(unsigned int uiBegin,unsigned int uiEnd,N Compare);
	
		void Sort(unsigned int uiBegin,unsigned int uiEnd);
		void Destroy();
		//Ҫע⣺Tָ룬Element ָ뵽ָʽתָĴ
		//ٹУreference11ԭreferenceΪ0ٶ󣬵³
		unsigned int FindElement(const T & Element);
	protected:

		T * m_pBuffer;
		unsigned int m_uiGrowBy;
		unsigned int m_uiCurUse;
		unsigned int m_uiBufferNum;
		unsigned int m_uiAllocNum;

		
	private:

	};
	//
	template <class T,SKMemManagerFun MMFun = SKMemObject::GetMemManager>
	class SKArrayOrder : public SKArray<T,MMFun>
	{
	public:
		SKArrayOrder(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~SKArrayOrder();
		void AddElement(const T & Element);

		//Ҫע⣺Tָ룬Element ָ뵽ָʽתָĴ
		//ٹУreference11ԭreferenceΪ0ٶ󣬵³
		unsigned int FindElement(const T & Element);

		void ReSort(unsigned int i);
	protected:

		unsigned int Process(unsigned int uiIndex0,unsigned int uiIndex1,const T & Element);

		unsigned int FindElement(unsigned int uiIndex0,unsigned int uiIndex1,const T & Element);
	};
#include "SKArray.inl"
#define  ARRAY_ERASE(ARRAY,ELEMENT)  ARRAY.Erase(ARRAY.FindElement(ELEMENT));
	
}
#endif