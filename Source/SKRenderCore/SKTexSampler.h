#ifndef SKTEXSAMPLER_H
#define SKTEXSAMPLER_H
#include "SKShaderFunction.h"
#include "SKTexAllState.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKTexSampler : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:

		virtual ~SKTexSampler() = 0;
		virtual unsigned int GetTexType()const = 0 ;
		SKTexSampler(const SKUsedName & ShowName,SKMaterial * pMaterial);
		FORCEINLINE SKTexAllStateR* GetTexture()const
		{
			return m_pTexAllState;
		}
		FORCEINLINE void SetTexture(SKTexAllStateR* pTexture)
		{
			m_pTexAllState = pTexture;
		}
		virtual bool GetInputValueString(SKString &InputString)const;
		virtual bool GetDeclareString(SKString &OutString,unsigned int uiRegisterID)const;
		virtual void SetTexCoordLevel(unsigned int uiTexCoordLevel)
		{
			m_uiTexCoordLevel = uiTexCoordLevel;
		}
		FORCEINLINE unsigned int GetTexCoordLevel()const
		{
			return m_uiTexCoordLevel;
		}
		FORCEINLINE void SetVEDecode(unsigned char uiVEDecode)
		{
			m_uiVEDecode = uiVEDecode;
		}
		FORCEINLINE void SetVESRGB(unsigned char uiVESRGB )
		{
			m_uiVESRGB = uiVESRGB ;
		}
		
		FORCEINLINE unsigned char GetVEDecode()const
		{
			return m_uiVEDecode;
		}
		FORCEINLINE unsigned char GetVESRGB()const
		{
			return m_uiVESRGB;
		}	
		virtual bool GetOutPutValueString(SKString &OutString)const = 0;
	protected:
		
		SKTexSampler();
	protected:
		SKTexAllStateRPtr m_pTexAllState;
		unsigned char m_uiTexCoordLevel;
		unsigned char m_uiVEDecode;
		unsigned char m_uiVESRGB;

	};
	DECLARE_Ptr(SKTexSampler);
	SKTYPE_MARCO(SKTexSampler);
}
#endif