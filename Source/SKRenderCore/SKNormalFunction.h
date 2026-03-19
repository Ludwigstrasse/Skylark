#ifndef SKNORMALSHADERFUNCTION_H
#define SKNORMALSHADERFUNCTION_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKWorldNormal : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKWorldNormal(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKWorldNormal();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		SKWorldNormal();


	public:
		enum
		{
			OUT_VALUE,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKWorldNormal);
	SKTYPE_MARCO(SKWorldNormal);


	class SKGRAPHIC_API SKViewNormal : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKViewNormal(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKViewNormal();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		SKViewNormal();


	public:
		enum
		{
			OUT_VALUE,
			OUT_COLOR_R,
			OUT_COLOR_G,
			OUT_COLOR_B,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKViewNormal);
	SKTYPE_MARCO(SKViewNormal);
}
#endif