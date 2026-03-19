#ifndef SKDYNAMICBUFFERGEOMETRY_H
#define SKDYNAMICBUFFERGEOMETRY_H
#include "SKGeometry.h"
#include "SKUseBuffer.h"
#include "SKVertexFormat.h"
namespace SKEngine2
{
	class SKVertexBuffer;
	class SKIndexBuffer;
	class SKGRAPHIC_API SKDynamicBufferGeometry : public SKGeometry
	{
		//RTTI
		DECLARE_RTTI;
	public:
		SKDynamicBufferGeometry();
		virtual ~SKDynamicBufferGeometry() = 0;
		virtual void ClearInfo() = 0 ;
		virtual bool HaveDate() = 0;
		virtual unsigned int UpdateGeometry() = 0;
		virtual void Draw(SKCamera * pCamera);
	};
	DECLARE_Ptr(SKDynamicBufferGeometry);

	class SKGRAPHIC_API SKDVGeometry : public SKDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		SKDVGeometry();
		void Create(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
								unsigned int uiMeshDateType,
								unsigned int uiVertexNum);
		virtual ~SKDVGeometry();
		bool Add(const void * pVeretexDate,unsigned int uiVertexSize);
		void * NewGetV(unsigned int uiVertexSize);
		//ºӵ޹صMesh Ϣ
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pVertexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
		unsigned int GetMaxRenderVertexNum()const;
	protected:
		
		SKUseBufferPtr m_pVertexUseBuffer;

		unsigned int m_uiCurVUseBufferElementIndex;
		SKUseBufferPtr m_pVertexUseBufferRender;


	};
	DECLARE_Ptr(SKDVGeometry);



	class SKGRAPHIC_API SKDVDIGeometry : public SKDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		SKDVDIGeometry();
		void Create(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
			unsigned int uiMeshDateType,
			unsigned int uiVertexNum,
			unsigned int uiIndexNum);
		virtual ~SKDVDIGeometry();
		bool Add(const void * pVeretexDate,unsigned int uiVertexSize,
			const void * pIndexDate,unsigned int uiIndexSize);

		void * NewGetV(unsigned int uiVertexSize);
		void * NewGetI(unsigned int uiIndexSize);
		//ºӵ޹صMesh Ϣ
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pVertexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
	protected:
		SKUseBufferPtr m_pVertexUseBuffer;
		SKUseBufferPtr m_pIndexUseBuffer;
		unsigned int m_uiCurVUseBufferElementIndex;
		unsigned int m_uiCurIUseBufferElementIndex;

		SKUseBufferPtr m_pVertexUseBufferRender;
		SKUseBufferPtr m_pIndexUseBufferRender;


	};
	DECLARE_Ptr(SKDVDIGeometry);



	class SKGRAPHIC_API SKSVDIGeometry : public SKDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		SKSVDIGeometry();
		void Create(unsigned int uiMeshDateType,SKVertexBuffer * pVertexBuffer,unsigned int uiIndexNum);
		virtual ~SKSVDIGeometry();
		bool Add(const void * pIndexDate,unsigned int uiIndexSize);
		void * NewGetI(unsigned int uiIndexSize);
		//ºӵ޹صMesh Ϣ
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pIndexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
	protected:
		
		SKUseBufferPtr m_pIndexUseBuffer;
		unsigned int m_uiCurIUseBufferElementIndex;
		SKUseBufferPtr m_pIndexUseBufferRender;

	};
	DECLARE_Ptr(SKSVDIGeometry);



	class SKGRAPHIC_API SKDVSIGeometry : public SKDynamicBufferGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		SKDVSIGeometry();
		void Create(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &ForamtArray,
					unsigned int uiMeshDateType,
					unsigned int uiVertexNum,
					SKIndexBuffer * pIndexBuffer);
		virtual ~SKDVSIGeometry();
		bool Add(const void * pVeretexDate,unsigned int uiVertexSize);
		void * NewGetV(unsigned int uiVertexSize);
		//ºӵ޹صMesh Ϣ
		virtual unsigned int UpdateGeometry();
		virtual void ClearInfo();
		virtual bool HaveDate()
		{
			if (!m_pVertexUseBuffer->GetElementNum())
			{
				return false;
			}
			return true;
		}
	protected:

		SKUseBufferPtr m_pVertexUseBuffer;

		unsigned int m_uiCurVUseBufferElementIndex;
		SKUseBufferPtr m_pVertexUseBufferRender;


	};
	DECLARE_Ptr(SKDVSIGeometry);
}
#endif