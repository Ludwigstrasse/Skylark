#ifndef SKMORPHSET_H
#define SKMORPHSET_H
#include "SKObject.h"
#include "SKVertexBuffer.h"
#include "SKName.h"
#include <SKDelegateList.h>
namespace SKEngine2
{
	/*
		ÿMorphݶжӦGeometryNodeGeometryNodeмڵ㣨GeometryMesh,ӦMorph
		ҲмBufferMorphÿBufferǺGeometryNodeĽڵһһӦġڰMorph뵽MorphSetʱеMorphBufferҪһ
		ҶӦBufferݣӦĶҲҪһ
		ΪGeometryNodeMorphSetʱҪGeometryNodeĽڵMorphSetBufferǷһҪÿMeshĶ
		ǷÿMorphBufferĶһBufferΪյʱ

	*/
	class SKStream;

	class SKGRAPHIC_API SKMorph : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		SKMorph();
		virtual ~SKMorph();
		FORCEINLINE unsigned int GetBufferNum()const
		{
			return m_pVertexBufferArray.GetNum();
		}
		FORCEINLINE unsigned int GetVertexNum(unsigned int uiBufferID)const
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return 0;
			}
			if (m_pVertexBufferArray[uiBufferID] == NULL)
			{
				return 0;
			}
			return m_pVertexBufferArray[uiBufferID]->GetVertexNum();
		}
		FORCEINLINE SKVertexBuffer * GetBuffer(unsigned int uiBufferID)const
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return NULL;
			}

			return m_pVertexBufferArray[uiBufferID];
		}
		void ReSizeBuffer(unsigned int uiBufferSize)
		{
			m_pVertexBufferArray.SetBufferNum(uiBufferSize);
		}
		void SetVertexBuffer(unsigned int uiBufferID,SKVertexBuffer * pBuffer)
		{
			if (uiBufferID >= m_pVertexBufferArray.GetNum())
			{
				return;
			}
			m_pVertexBufferArray[uiBufferID] = pBuffer;

		}
		SKUsedName m_cName;
	protected:
		//element is null that is this geometry no morph
		SKArray<SKVertexBufferPtr> m_pVertexBufferArray;
	};
	DECLARE_Ptr(SKMorph);
	SKTYPE_MARCO(SKMorph);

#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef SKDelegateEvent<void(void)> AddMorphEventType;
#else
	typedef SKDelegateEvent0<void> AddMorphEventType;
#endif
	class SKGRAPHIC_API SKMorphSet: public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		SKMorphSet();
		virtual ~SKMorphSet();
		bool SetMorph(SKMorph * pMorph);
		SKMorph * GetMorph(const SKUsedName & MorphName)const;
		SKMorph * GetMorph(unsigned int i)const;
		FORCEINLINE unsigned int GetMorphNum()const
		{
			return m_pMorphArray.GetNum();
		}
		FORCEINLINE unsigned int GetBufferNum()const
		{
			if (m_pMorphArray.GetNum())
			{
				return m_pMorphArray[0]->GetBufferNum();
			}
			else
				return 0;
		}
		AddMorphEventType m_AddMorphEvent;
	protected:
		SKArray<SKMorphPtr> m_pMorphArray;
	};
	DECLARE_Ptr(SKMorphSet);
	SKTYPE_MARCO(SKMorphSet);

}
#endif