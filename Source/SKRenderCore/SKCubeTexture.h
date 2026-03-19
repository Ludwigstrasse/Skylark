#ifndef SKCUBETEXTURE_H
#define SKCUBETEXTURE_H
#include "SKTexture.h"
#include "SK2DTexture.h"
namespace SKEngine2
{
	class SKRenderTarget;
	class SKGRAPHIC_API SKCubeTexture : public SKTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	//FACE
		{
			F_RIGHT,
			F_LEFT,	
			F_TOP,
			F_BOTTOM,
			F_FRONT,
			F_BACK,				
			F_MAX
		};
		SKCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,
					unsigned int uiMipLevel,bool bIsStatic);
		SKCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,unsigned int uiMipLevel = 1);
		SKCubeTexture(VS2DTexture * pTexture[SKCubeTexture::F_MAX]);
		virtual ~SKCubeTexture();

		virtual unsigned int GetTexType()const{return TT_CUBE;}

		unsigned char * GetFaceBuffer(unsigned int uiLevel,unsigned int uiFace)const;
		
		virtual unsigned int GetByteSize(unsigned int uiLevel)const;
		FORCEINLINE unsigned int GetFaceByteSize(unsigned int uiLevel)const
		{
			if (uiLevel > m_uiMipLevel)
			{
				return 0;
			}
			if (IsCompress())
			{
				return SKRenderer::GetBytesPerPixel(m_uiFormatType) 
					* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel)) 
					* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel)) 
					* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetLength(uiLevel));
			}
			return SKRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel) * GetHeight(uiLevel);
		}
		virtual void SetMipLevel();
		FORCEINLINE bool IsRenderTarget()const
		{
			return m_bRenderTarget;
		}
		
		bool m_bRtUsed;
		friend class SKRenderTarget;
		class SKRenderTarget *GetCurRenderTarget(unsigned int uiFace)const
		{
			return m_pCurRenderTarget[uiFace];
		}
		
	protected:
		SKCubeTexture();
		bool m_bRenderTarget;
		class SKRenderTarget * m_pCurRenderTarget[F_MAX];
		
	};
	DECLARE_Ptr(SKCubeTexture);
	SKTYPE_MARCO(SKCubeTexture);
}
#endif