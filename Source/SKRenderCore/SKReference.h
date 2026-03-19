#ifndef SKREFERENCE_H
#define SKREFERENCE_H
#include "SKGraphic.h"
#include "SKMemManager.h"
#include "SKPointer.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKReference
	{
	public:
		template <class T>
		friend class SKPointer;
		friend class SKVariant;
		SKReference();
		virtual ~SKReference() = 0;
		//SmartPointer
	public:
		int GetRef()const { return m_iReference;}
	protected:
		void IncreRef()
		{
			SKLockedIncrement((long *)&m_iReference);
			//m_iReference++;
		}
		void DecreRef()
		{
			//m_iReference--; 
			SKLockedDecrement((long *)&m_iReference);
			if(!m_iReference) 
				SK_DELETE this;
		}
		int m_iReference;
	
	
	};

}
#endif