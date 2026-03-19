#ifndef SKDEPTHSTENCIL_H
#define SKDEPTHSTENCIL_H
#include "SKBind.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKDepthStencil : public SKBind
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
		
	public:
		friend class SKResourceManager;
	protected:
		SKDepthStencil();
		SKDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
			unsigned int uiMulSample,unsigned int uiFormatType);
	public:
		
		virtual ~SKDepthStencil();
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{
			return m_uiHeight;
		}
		FORCEINLINE unsigned int GetMulSample()const
		{
			return m_uiMulSample;
		}
		FORCEINLINE unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		bool m_bUsed;
	protected:

		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiMulSample;
		unsigned int m_uiFormatType;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);

	};
	DECLARE_Ptr(SKDepthStencil);
	SKTYPE_MARCO(SKDepthStencil);
}
#endif