#ifndef SKRENDERTARGET_H
#define SKRENDERTARGET_H
#include "SKBind.h"
#include "SKTexture.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKRenderTarget : public SKBind
	{

		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
	public:
		friend class VS2DTexture;
		friend class SKCubeTexture;
		friend class SKResourceManager;
		
		virtual ~SKRenderTarget();
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{

			return m_uiHeight;
		}
		FORCEINLINE unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		FORCEINLINE unsigned int GetMulSample()const
		{
			return m_uiMulSample;
		}
		FORCEINLINE unsigned int GetLevel()const
		{
			return m_uiLevel;
		}
		FORCEINLINE unsigned int GetParam()const
		{
			return m_uiParam;
		}
		FORCEINLINE SKTexture * GetCreateBy()const
		{
			return m_pCreateBy;
		}
		bool m_bUsed;
	protected:
		SKRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMulSample);
		SKRenderTarget(SKTexture * pCreateBy,unsigned int uiMulSample = SKRenderer::MS_NONE
			,unsigned int uiLevel = 0,unsigned int Param = 0);
		SKRenderTarget();
		SKTexturePtr m_pCreateBy; 
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiFormatType;
		unsigned int m_uiMulSample;
		unsigned int m_uiLevel;
		unsigned int m_uiParam;
		DECLARE_INITIAL
	};
	DECLARE_Ptr(SKRenderTarget);
	SKTYPE_MARCO(SKRenderTarget);
}
#endif