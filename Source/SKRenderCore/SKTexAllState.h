#ifndef SKTEXALLSTATE_H
#define SKTEXALLSTATE_H
#include "SKTexture.h"
#include "SKResource.h"
#include "SKSamplerState.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKTexAllState : public SKObject , public SKResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

	public:
		SKTexAllState(SKTexture * Texture);
		virtual ~SKTexAllState();
		SKTexAllState();
		virtual unsigned int GetResourceType()const
		{
			return RT_TEXTURE;
		}
	

		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		SKTexturePtr		m_pTex;
	protected:
		SKSamplerDesc		m_SamplerDesc;
		SKSamplerStatePtr	m_pSamplerState;
		bool				m_bSRGB;
	protected:
		static SKPointer<SKTexAllState> Default;
		static SKPointer<SKTexAllState> ms_pOrenNayarLookUpTable;
		static SKPointer<SKTexAllState> ms_pStraussFLookUpTable;
		static SKPointer<SKTexAllState> ms_pStraussSLookUpTable;
		static SKPointer<SKTexAllState> ms_pCookTorranceLookUpTable;
		static SKPointer<SKTexAllState> ms_pIsotropicWardLookUpTable;
	public:
		friend class SKMaterial;
		friend class SKRenderer;
		FORCEINLINE bool GetSRGB()const
		{
			return m_bSRGB;
		}
		void SetSRGBEable(bool bEnable);
		static const SKTexAllState *GetDefalut()
		{
			return Default;
		}
		static const SKTexAllState * GetOrenNayarLookUpTable()
		{
			return ms_pOrenNayarLookUpTable;
		}
		static const SKTexAllState * GetStraussFLookUpTable()
		{
			return ms_pStraussFLookUpTable;
		}
		static const SKTexAllState * GetStraussSLookUpTable()
		{
			return ms_pStraussSLookUpTable;
		}
		static const SKTexAllState * GetCookTorranceTable()
		{
			return ms_pCookTorranceLookUpTable;
		}
		static const SKTexAllState * GetIsotropicWardLookUpTable()
		{
			return ms_pIsotropicWardLookUpTable;
		}
		void SetSamplerState(SKSamplerState * pSamplerState)
		{
			if (!pSamplerState)
			{
				pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
			}
			if (m_pSamplerState != pSamplerState)
			{
				m_pSamplerState = pSamplerState;
				m_SamplerDesc = pSamplerState->GetSamplerDesc();
			}
		}
		const SKSamplerState * GetSamplerState()const
		{
			return m_pSamplerState;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
	};
	DECLARE_Ptr(SKTexAllState);
	DECLARE_Proxy(SKTexAllState);
	SKTYPE_MARCO(SKTexAllState);
}
#endif