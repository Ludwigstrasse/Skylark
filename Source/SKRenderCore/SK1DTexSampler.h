#ifndef VS1DTEXSAMPLER_H
#define VS1DTEXSAMPLER_H
#include "SKTexSampler.h"
#include "SKViewFamily.h"
namespace SKEngine2
{
	class SKGRAPHIC_API VS1DTexSampler : public SKTexSampler
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		VS1DTexSampler(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~VS1DTexSampler();
		virtual void SetTexture(SKTexAllStateR * pTexAllState);


		virtual unsigned int GetTexType()const
		{
			return SKTexture::TT_1D;
		}

		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		VS1DTexSampler();

	public:
		enum
		{
			IN_TEXCOORD,
			IN_MAX
		};
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};
	};
	DECLARE_Ptr(VS1DTexSampler);
	SKTYPE_MARCO(VS1DTexSampler);
}
#endif