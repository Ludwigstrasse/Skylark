#ifndef SKPOINT_H
#define SKPOINT_H
#include "SKSynchronize.h"
namespace SKEngine2
{
	
	template <class T>
	class SKPointer
	{
	public:
		
		SKPointer (T* pObject = 0);
		SKPointer (const SKPointer& rPointer);
		~SKPointer ();
		
		
		T& operator* () const;
		T* operator-> () const;
		
		
		SKPointer& operator= (T* pObject);
		SKPointer& operator= (const SKPointer& rReference);
		
		
		bool operator== (T* pObject) const;
		bool operator!= (T* pObject) const;
		bool operator== (const SKPointer& rReference) const;
		bool operator!= (const SKPointer& rReference) const;
		operator T*()const;
		
		FORCEINLINE T * GetObject()const
		{
			return m_pObject;
		}

		friend class SKStream;
	private:
		T* m_pObject;
		FORCEINLINE void SetObject(T *  pObject)
		{
			m_pObject = pObject;
		}
		
	};

#include"SKPointer.inl"
#define DECLARE_Ptr(ClassName)\
	class ClassName;\
	typedef SKPointer<ClassName> ##ClassName##Ptr;
}
#endif