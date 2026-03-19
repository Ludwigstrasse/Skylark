#ifndef SKFONT_H
#define SKFONT_H
#include "SKObject.h"
#include "SKTexAllState.h"
#include "SK2DTexture.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKFont : public SKObject, public SKResource
	{
		//PRIORITY
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum
		{
			ASCII_START = 32,
			ASCII_END = 126,
			ASCII_NUM = ASCII_END - ASCII_START + 1
		};
		SKFont(const TCHAR* acFace, unsigned int uiSize, bool bBold,
				bool bItalic);
		virtual ~SKFont();
		virtual unsigned int GetResourceType()const
		{
			return RT_FONTTYPE;
		}
	protected:
		SKFont();
	public:


		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	protected:
		
		static SKPointer<SKFont> Default;
	public:
		friend class SKRenderer;
		static const SKFont *GetDefalut()
		{
			return Default;
		}
		FORCEINLINE const SKString & GetFace()const {return m_Face;}
		FORCEINLINE unsigned int GetSize()const {return m_uiSize;}
		FORCEINLINE bool GetBold()const {return m_bBold;}
		FORCEINLINE bool GetItalic()const {return m_bItalic;}
		FORCEINLINE bool IsValid()const
		{
			return m_pFontTex != NULL && !m_pCharacterData && m_pFontTexAllState != NULL;
		}
		void SetFontData(VS2DTexture * pTexture,SKREAL * pCharacterData);
		FORCEINLINE SKREAL GetFontData(unsigned char Index)const
		{
			return m_pCharacterData[Index];
		}
		SKTexAllState * GetTexAllState()const
		{
			return m_pFontTexAllState;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
	protected:
		SKString m_Face;
		unsigned int m_uiSize;
		bool m_bBold;
		bool m_bItalic;
		VS2DTexturePtr m_pFontTex;
		SKREAL *m_pCharacterData;
		SKTexAllStatePtr m_pFontTexAllState;
	};
	DECLARE_Ptr(SKFont);
	DECLARE_Proxy(SKFont);
	SKTYPE_MARCO(SKFont);


}
#endif