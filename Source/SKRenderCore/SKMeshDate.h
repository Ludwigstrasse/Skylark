#ifndef SKMESHDATE_H
#define SKMESHDATE_H
#include "SKVertexBuffer.h"
#include "SKCollapseRecord.h"
#include "SKIndexBuffer.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKMeshDate : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum //MeshDate Type
		{
			MDT_POINT,
			MDT_LINE,
			MDT_TRIANGLE,
			MDT_MAX
		};
		virtual ~SKMeshDate() = 0;
		SKMeshDate();
		bool SetIndexBuffer(SKIndexBuffer * pIndexBuffer);
		FORCEINLINE SKIndexBuffer * GetIndexBuffer()const;

		bool SetVertexBuffer(SKVertexBuffer *pVertexBuffer);
		FORCEINLINE SKVertexBuffer *GetVertexBuffer()const;




		virtual unsigned int GetTotleNum()const = 0;

		virtual unsigned int GetMeshDateType() = 0;
		virtual bool CreateCollapseRecord(){return 1;}
		virtual void UpDateClodMesh(){}
		virtual unsigned int GetActiveNum()const{ return GetTotleNum();}
		virtual void SetLodDesirePercent(SKREAL fDesirePercent);

		bool IsClodMesh()const {return m_pCollapseRecord != NULL;}
		FORCEINLINE void RemoveClodMesh()
		{
			m_pCollapseRecord = NULL;
		}
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const = 0;
	protected:
		
		friend class SKGeometry;
		enum
		{
			DRAW_MATH_ELEMENT_LENGTH = 10000
		};
		SKVertexBufferPtr	m_pVertexBuffer;
		SKIndexBufferPtr	m_pIndexBuffer;
		

	protected:
		SKCollapseRecordPtr m_pCollapseRecord;
	};
	DECLARE_Ptr(SKMeshDate);
	SKTYPE_MARCO(SKMeshDate);
#include "SKMeshDate.inl"
}
#endif