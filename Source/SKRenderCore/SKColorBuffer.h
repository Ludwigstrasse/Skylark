#ifndef SKCOLORBUFFER_H
#define SKCOLORBUFFER_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKColorBuffer : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKColorBuffer(const SKUsedName & ShowName, SKMaterial * pMaterial);
		virtual ~SKColorBuffer();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetInputValueString(SKString &InputString)const;
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		SKColorBuffer();

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
			OUT_MAX
		};
	};
	DECLARE_Ptr(SKColorBuffer);
	SKTYPE_MARCO(SKColorBuffer);
}
#endif