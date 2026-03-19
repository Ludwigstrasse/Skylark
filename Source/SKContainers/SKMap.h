#ifndef SKMAP_H
#define SKMAP_H
#include "SKContainer.h"
namespace SKEngine2
{
	
	template<class KEY,class VALUE,SKMemManagerFun MMFun = SKMemObject::GetMemManager>
	class SKMap : public SKContainer<MapElement<KEY,VALUE>,MMFun>
	{
	public:
		enum
		{
			DEFAULT_GROWBY = 10
		};
		
		SKMap(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~SKMap();

		void SetBufferNum(unsigned int uiBufferNum);
		void AddBufferNum(unsigned int uiBufferNum);
		void operator= (const SKMap<KEY,VALUE,MMFun>& Map);


		FORCEINLINE unsigned int GetNum()const;

		FORCEINLINE unsigned int GetBufferNum()const;

		FORCEINLINE MapElement<KEY,VALUE> * GetBuffer()const;
		FORCEINLINE void SetGrowBy(unsigned int uiGrowBy);
		
		template<class KEY1,class VALUE1>
		void AddElement(const MapElement<KEY1,VALUE1> & Element);
		void AddElement(const KEY& Key , const VALUE &Value);

		template<class KEY1,class VALUE1,SKMemManagerFun MMFun1>
		void AddElement(const SKMap<KEY1,VALUE1,MMFun1> & Map,unsigned int uiBegin,unsigned int uiEnd);

		MapElement<KEY,VALUE>& operator[] (unsigned int i)const;

		void Clear();

		FORCEINLINE unsigned int GetSize()const;

		void Erase(unsigned int i);

		void Erase(unsigned int uiBegin,unsigned int uiEnd);

		template <class N>
		void Sort(unsigned int uiBegin,unsigned int uiEnd,N Compare);
	
		void Sort(unsigned int uiBegin,unsigned int uiEnd);
		unsigned int Find(const KEY& Key)const ;

		unsigned int FindValueIndex(const VALUE &Value)const;

		void Destroy();
	protected:

		MapElement<KEY,VALUE> * m_pBuffer;
		unsigned int m_uiGrowBy;
		unsigned int m_uiCurUse;
		unsigned int m_uiBufferNum;
		unsigned int m_uiAllocNum;
	};

	template<class KEY,class VALUE,SKMemManagerFun MMFun = SKMemObject::GetMemManager>
	class SKMapOrder : public SKMap<KEY,VALUE,MMFun>
	{
	public:
		SKMapOrder(unsigned int uiGrowBy = DEFAULT_GROWBY);
		~SKMapOrder();

		template<class KEY1,class VALUE1>
		void AddElement(const MapElement<KEY1,VALUE1> & Element);

		void AddElement(const KEY& Key , const VALUE &Value);


		unsigned int Find(const KEY& Key)const ;

	protected:
		template<class KEY1,class VALUE1>
		unsigned int Process(unsigned int uiIndex0,unsigned int uiIndex1,const MapElement<KEY1,VALUE1> & Element);

		unsigned int FindElement(unsigned int uiIndex0,unsigned int uiIndex1,const KEY& Key)const;
	};
	#include "SKMap.inl"
#define  MAP_ERASE(MAP,KEY)  MAP.Erase(MAP.Find(KEY));
}
#endif