#ifndef VS2DTEXTURE_H
#define VS2DTEXTURE_H
#include "SKTexture.h"
namespace SKEngine2
{
	class SKRenderTarget;
	class SKGRAPHIC_API VS2DTexture:public SKTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VS2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
					unsigned int uiMipLevel /*= 0*/,bool bIsStatic/* = true*/);
		VS2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMipLevel = 1);
		virtual ~VS2DTexture();

		virtual unsigned int GetTexType()const{return TT_2D;}
		FORCEINLINE bool IsRenderTarget()const
		{
			return m_bRenderTarget;
		}
		virtual void SetMipLevel();
		bool SaveToFile(TCHAR * pSaveName);
		virtual void CreateRAMDate();

		DECLARE_INITIAL

		//֤rendertargetýȥtextureãʾ
		bool m_bRtUsed;
	public:
		friend class SKRenderer;
		friend class SKTexAllState;
		friend class SKRenderTarget;
		class SKRenderTarget *GetCurRenderTarget()const
		{
			return m_pCurRenderTarget;
		}
	protected:
		VS2DTexture();
		bool m_bRenderTarget;
		class SKRenderTarget * m_pCurRenderTarget;
	
	};
	DECLARE_Ptr(VS2DTexture);
	SKTYPE_MARCO(VS2DTexture);
}
#endif