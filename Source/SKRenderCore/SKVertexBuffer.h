#ifndef SKVERTEXBUFFER_H
#define SKVERTEXBUFFER_H
#include "SKObject.h"
#include "SKBind.h"
#include "SKDataBuffer.h"
#include "SKMath.h"
#include "SKVertexFormat.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKVertexBuffer : public SKBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class SKVertexFormat;
		friend class SKResourceManager;
		SKVertexBuffer(bool bIsStatic);
		SKVertexBuffer(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE>& FormatArray,unsigned int uiNum);
		virtual ~SKVertexBuffer();
		bool SetDate(SKDataBuffer * pDate,unsigned int uiVF);
		FORCEINLINE SKDataBuffer * GetDate(unsigned int uiVF,unsigned int uiLevel)const;
		FORCEINLINE unsigned int GetLevel(unsigned int uiVF)const;

		FORCEINLINE SKDataBuffer * GetPositionDate(unsigned int uiLevel)const;
		FORCEINLINE SKDataBuffer * GetNormalDate(unsigned int uiLevel)const;
		FORCEINLINE SKDataBuffer * GetPSizeDate()const;
		FORCEINLINE SKDataBuffer * GetColorDate(unsigned int uiLevel)const;
		FORCEINLINE SKDataBuffer * GetBlendWeightDate()const;
		FORCEINLINE SKDataBuffer * GetBlendIndicesDate()const;
		FORCEINLINE SKDataBuffer * GetTangentDate()const;
		FORCEINLINE SKDataBuffer * GetBinormalDate()const;
		FORCEINLINE SKDataBuffer * GetFogDate()const;
		FORCEINLINE SKDataBuffer * GetDepthDate()const;
		FORCEINLINE SKDataBuffer * GetTexCoordDate(unsigned int uiLevel)const;

		FORCEINLINE unsigned int GetPositionLevel()const;
		FORCEINLINE unsigned int GetColorLevel()const;
		FORCEINLINE unsigned int GetNormalLevel()const;
		FORCEINLINE unsigned int GetTexCoordLevel()const;

		FORCEINLINE unsigned int GetVertexNum()const;

		bool GetVertexFormat(SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> &FormatArray);

		FORCEINLINE SKVertexFormat * GetVertexFormat()const;

		FORCEINLINE unsigned int GetOneVertexSize()const;

		FORCEINLINE bool GetLockDateOffSet(unsigned int uiVF,unsigned int uiLevel,unsigned int &uiOffSet)const;
		FORCEINLINE void *GetLockDate(unsigned int uiVF,unsigned int uiLevel)const;

		FORCEINLINE void *GetLockPositionDate(unsigned int uiLevel)const;
		FORCEINLINE void *GetLockNormalDate(unsigned int uiLevel)const;
		FORCEINLINE void *GetLockPSizeDate()const;
		FORCEINLINE void *GetLockColorDate(unsigned int uiLevel)const;
		FORCEINLINE void *GetLockBlendWeightDate()const;
		FORCEINLINE void *GetLockBlendIndicesDate()const;
		FORCEINLINE void *GetLockTangentDate()const;
		FORCEINLINE void *GetLockBinormalDate()const;
		FORCEINLINE void *GetLockFogDate()const;
		FORCEINLINE void *GetLockDepthDate()const;
		FORCEINLINE void *GetLockTexCoordDate(unsigned int uiLevel)const;

		
		virtual unsigned int GetByteSize()const;

		unsigned int GetSemanticsNum(unsigned int uiSemantics)const;

		FORCEINLINE bool HavePositionInfo(unsigned int uiLevel)const;
		FORCEINLINE bool HaveNormalInfo(unsigned int uiLevel)const;
		FORCEINLINE bool HavePSizeInfo()const;
		FORCEINLINE bool HaveColorInfo(unsigned int uiLevel)const;
		FORCEINLINE bool HaveBlendWeightInfo()const;
		FORCEINLINE bool HaveBlendIndicesInfo()const;
		FORCEINLINE bool HaveTangentInfo()const;
		FORCEINLINE bool HaveBinormalInfo()const;
		FORCEINLINE bool HaveFogInfo()const;
		FORCEINLINE bool HaveDepthInfo()const;
		FORCEINLINE bool HaveTexCoordInfo(unsigned int uiLevel)const;

		unsigned int GetSemanticsChannel(unsigned int uiSemantics,unsigned int uiLevel)const;

		FORCEINLINE unsigned int PositionChannel(unsigned int uiLevel)const;
		FORCEINLINE unsigned int NormalChannel(unsigned int uiLevel)const;
		FORCEINLINE unsigned int PSizeChannel()const;
		FORCEINLINE unsigned int ColorChannel(unsigned int uiLevel)const;
		FORCEINLINE unsigned int BlendWeightChannel()const;
		FORCEINLINE unsigned int BlendIndicesChannel()const;
		FORCEINLINE unsigned int TangentChannel()const;
		FORCEINLINE unsigned int BinormalChannel()const;
		FORCEINLINE unsigned int FogChannel()const;
		FORCEINLINE unsigned int DepthChannel()const;
		FORCEINLINE unsigned int TexCoordChannel(unsigned int uiLevel)const;

		unsigned int GetSemanticsDateType(unsigned int uiSemantics,unsigned int uiLevel)const;

		FORCEINLINE unsigned int NormalDateType(unsigned int uiLevel)const;
		FORCEINLINE unsigned int TangentDateType()const;
		FORCEINLINE unsigned int BinormalDateType()const;

	protected:
		friend class SKVertexFormat;
		friend class SKGeometry;
		SKVertexBuffer();
		SKArray<SKDataBufferPtr> m_pDate[SKVertexFormat::VF_MAX];
		unsigned int m_uiVertexNum;
		unsigned int m_uiOneVertexSize;

		SKVertexFormatPtr m_pVertexFormat;
		
		void * m_pLockDate;
		//Ҫʵ
	public:
	

		virtual	bool LoadResource(SKRenderer * pRender);

		void *Lock();
		void UnLock();
		FORCEINLINE void * GetLockDatePtr()const
		{
			return m_pLockDate;
		}
		virtual void ClearInfo();
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);

		
	};
	DECLARE_Ptr(SKVertexBuffer);
	SKTYPE_MARCO(SKVertexBuffer);
	#include "SKVertexBuffer.inl"
	
}
#endif