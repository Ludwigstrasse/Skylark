#ifndef SKPOSTSHADERSHADERFUNCTION_H
#define SKPOSTSHADERSHADERFUNCTION_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKPostEffectShaderFunction : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		enum
		{
			IN_COLOR,
			IN_MAX
		};
		SKPostEffectShaderFunction(const SKUsedName & ShowName, SKMaterial * pMaterial);
		virtual ~SKPostEffectShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName(){};
		virtual bool IsValidNodeToThis(SKShaderFunction * pShaderFunction);

	protected:
		SKPostEffectShaderFunction();
	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};

	};
	DECLARE_Ptr(SKPostEffectShaderFunction);
	SKTYPE_MARCO(SKPostEffectShaderFunction);
}
#endif