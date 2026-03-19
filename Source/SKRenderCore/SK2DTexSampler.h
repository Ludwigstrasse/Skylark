#ifndef VS2DTEXSAMPLER_H
#define VS2DTEXSAMPLER_H
#include "SKTexSampler.h"
#include "SKViewFamily.h"
namespace SKEngine2
{
	class SKGRAPHIC_API VS2DTexSampler : public SKTexSampler
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VS2DTexSampler(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~VS2DTexSampler();
		virtual void SetTexture(SKTexAllStateR * pTexAllState);
		
		
		virtual unsigned int GetTexType()const
		{
			return SKTexture::TT_2D;
		}

		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		VS2DTexSampler();
		
	public:
		enum
		{
			IN_TEXCOORD,
			IN_MAX
		};
		enum
		{
			OUT_COLOR,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_COLOR_A,
			OUT_MAX
		};
	};
	DECLARE_Ptr(VS2DTexSampler);
	SKTYPE_MARCO(VS2DTexSampler);
}
#endif