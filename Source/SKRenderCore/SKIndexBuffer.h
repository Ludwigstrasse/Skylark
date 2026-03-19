#ifndef SKINDEXBUFFER_H
#define SKINDEXBUFFER_H
#include "SKObject.h"
#include "SKBind.h"
#include "SKDataBuffer.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKIndexBuffer : public SKBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		virtual ~SKIndexBuffer();
		SKIndexBuffer();
		SKIndexBuffer(unsigned int uiNum,unsigned int uiDT = SKDataBuffer::DT_USHORT);
		bool SetDate(SKDataBuffer * pDate);
	public:
		

		virtual void *Lock();
		virtual void UnLock();

		virtual	bool LoadResource(SKRenderer * pRender);
		virtual void ClearInfo();
		FORCEINLINE SKDataBuffer *GetIndexDate()const {return m_pDate;}

		unsigned int GetNum()const;
		virtual unsigned int GetByteSize()const;
		FORCEINLINE void * GetLockDatePtr()const
		{
			return m_pLockDate;
		}
		FORCEINLINE unsigned int GetDateType()const
		{
			return m_uiDT;
		}
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		SKDataBufferPtr m_pDate;
		unsigned int m_uiNum;
		unsigned int m_uiDT;
		void * m_pLockDate;
	};
	DECLARE_Ptr(SKIndexBuffer);
	SKTYPE_MARCO(SKIndexBuffer);
}
#endif