#ifndef SKUSEBUFFER_H
#define SKUSEBUFFER_H
#include "SKObject.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKUseBuffer : public SKReference,public SKMemObject
	{
	public:
		
		SKUseBuffer(bool IsStatic,unsigned int uiBufferSize = 512);
		virtual ~SKUseBuffer();

	public:

		void * NewGet(unsigned int uiDateSize);
		bool Add(const void * pDate,unsigned int uiDateSize);

		void Clear();

		FORCEINLINE unsigned char * GetBuffer()const{return m_pBuffer;}
		FORCEINLINE unsigned int GetBufferSize()const {return m_uiBufferSize;}
		FORCEINLINE unsigned int GetCurSize()const {return m_uiCurSize;}
		

		unsigned int GetSizeByElementIndex(unsigned int i0,unsigned int i1);
		unsigned int GetSizeByElementIndex(unsigned int i);

		FORCEINLINE unsigned int GetOneAddLimitSize()const {return m_uiOneAddLimitSize;}
		FORCEINLINE void SetOneAddLimitSize(unsigned int uiOneAddLimitSize) {m_uiOneAddLimitSize = uiOneAddLimitSize;}

		
		class SKBufferElementInfo
		{
		public:
			SKBufferElementInfo()
			{
				m_uiStart = 0;
				m_uiEnd = 0;
			}
			static SKBufferElementInfo ElementNULL;
			unsigned int m_uiStart;
			unsigned int m_uiEnd;
			unsigned int GetSize()const
			{
				if (m_uiEnd > m_uiStart)
				{
					return m_uiEnd - m_uiStart;
				}
				else
				{
					return 0;
				}
			}
		
		};

		FORCEINLINE const SKBufferElementInfo *GetElementInfo(unsigned int uiElementIndex)const
		{
			if (uiElementIndex < m_BufferElemetnArray.GetNum())
			{
				return &m_BufferElemetnArray[uiElementIndex];
			}
			else
			{
				return NULL;
			}
		}
		FORCEINLINE unsigned int GetElementNum()const
		{
			return m_BufferElemetnArray.GetNum();
		}
	protected:
		unsigned int m_uiBufferSize;
		bool m_bIsStatic;
		unsigned char * m_pBuffer;
		unsigned int m_uiCurSize;
		SKArray<SKBufferElementInfo> m_BufferElemetnArray;
		unsigned int m_uiOneAddLimitSize;
	};
	DECLARE_Ptr(SKUseBuffer);
}
#endif