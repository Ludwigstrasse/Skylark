#ifndef SKTEXTURE_H
#define SKTEXTURE_H
#include "SKBind.h"
#include "SKRenderer.h"
#include "SKBitArray.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKTexture : public SKBind
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum
		{
			MAX_MIP_LEVEL = 20
		};
		enum	//Tex Type
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_CUBE,
			TT_MAX
		};
	protected:
		SKTexture(unsigned int uiFormatType,unsigned int uiWidth,
					unsigned int uiHeight,unsigned int uiLength,
					unsigned int uiMipLevel = 0,bool bIsStatic = true);
		SKTexture();
	public:
		virtual ~SKTexture()= 0;

		virtual unsigned int GetTexType()const = 0;
		virtual void SetMipLevel() = 0;
		FORCEINLINE unsigned int GetFormatType()const;
		FORCEINLINE unsigned int GetBytePerPixel()const;
		FORCEINLINE unsigned int GetChannelPerPixel()const;
		FORCEINLINE unsigned char * GetBuffer(unsigned int uiLevel)const;
		unsigned char * GetBuffer(unsigned int uiLevel,unsigned int i)const;
		virtual unsigned int GetByteSize(unsigned int uiLevel)const;
		virtual unsigned int GetMemPitch(unsigned int uiLevel)const;
		virtual unsigned int GetMemSlicePitch(unsigned int uiLevel)const;

		unsigned int GetWidth(unsigned int uiLevel)const;
		unsigned int GetHeight(unsigned int uiLevel)const;
		unsigned int GetLength(unsigned int uiLevel)const;
		FORCEINLINE unsigned int GetMipLevel()const;
		FORCEINLINE bool IsCompress()const
		{
			if (m_uiFormatType == SKRenderer::SFT_DXT3 || m_uiFormatType == SKRenderer::SFT_DXT5)
			{
				return true;
			}
			return false;
		}
		virtual void CreateRAMDate();
		FORCEINLINE bool IsHasAlpha()const
		{
			if(m_uiFormatType == SKRenderer::SFT_A8R8G8B8 ||
				m_uiFormatType == SKRenderer::SFT_A16B16G16R16F || m_uiFormatType == SKRenderer::SFT_A32B32G32R32F
				|| m_uiFormatType == SKRenderer::SFT_DXT3 || m_uiFormatType == SKRenderer::SFT_DXT5)
			{
				return true;
			}
			return false;
		}
	public:

		bool				m_bSRGB;
		virtual	bool LoadResource(SKRenderer * pRender);
		void *Lock(unsigned int uiLevel = 0,unsigned int uiFace = 0);
		void UnLock(unsigned int uiLevel = 0,unsigned int uiFace = 0);

		FORCEINLINE void * GetLockDatePtr(unsigned int uiLevel = 0,unsigned int uiFace = 0)const
		{
			SKMAC_ASSERT(uiLevel < GetMipLevel() && uiFace < 6)
			return m_pLockDate[uiLevel][uiFace];
		}
		virtual void ClearInfo();
		
		SKBit<unsigned short> m_VSTexSlot;
		SKBit<unsigned short> m_PSTexSlot;
		SKBit<unsigned short> m_GSTexSlot;
		SKBit<unsigned short> m_DSTexSlot;
		SKBit<unsigned short> m_HSTexSlot;
		void ClearAllSlot();
		bool HasAnySlot();
	protected:
		
		unsigned char * m_pBufferArray[MAX_MIP_LEVEL];
		unsigned int m_BufferSize[MAX_MIP_LEVEL];

		unsigned int m_uiFormatType;
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiLenght;

		unsigned int m_uiMipLevel;
		
		void * m_pLockDate[MAX_MIP_LEVEL][6];
		
		

	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
	};
	#include "SKTexture.inl"
	DECLARE_Ptr(SKTexture);
	SKTYPE_MARCO(SKTexture);
}
#endif