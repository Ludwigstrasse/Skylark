#ifndef SKPOSSHADERFUNCTION_H
#define SKPOSSHADERFUNCTION_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKWorldPos : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKWorldPos(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKWorldPos();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		SKWorldPos();


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
	DECLARE_Ptr(SKWorldPos);
	SKTYPE_MARCO(SKWorldPos);


	class SKGRAPHIC_API SKViewPos : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKViewPos(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKViewPos();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		SKViewPos();


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
	DECLARE_Ptr(SKViewPos);
	SKTYPE_MARCO(SKViewPos);

	class SKGRAPHIC_API SKProjectPos : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKProjectPos(const SKUsedName & ShowName, SKMaterial * pMaterial);
		virtual ~SKProjectPos();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool GetOutPutValueString(SKString &OutString)const;
	protected:
		SKProjectPos();


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
	DECLARE_Ptr(SKProjectPos);
	SKTYPE_MARCO(SKProjectPos);
}
#endif