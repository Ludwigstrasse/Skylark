#ifndef SKARITHMETICSHADERFUNCTION_H
#define SKARITHMETICSHADERFUNCTION_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;

	class SKGRAPHIC_API SKMul : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKMul(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKMul();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		SKMul();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKMul);
	SKTYPE_MARCO(SKMul);


	class SKGRAPHIC_API SKDot : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKDot(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKDot();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		SKDot();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};

	};
	DECLARE_Ptr(SKDot);
	SKTYPE_MARCO(SKDot);

	class SKGRAPHIC_API SKSub : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKSub(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKSub();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		SKSub();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKSub);
	SKTYPE_MARCO(SKSub);

	class SKGRAPHIC_API SKAdd : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKAdd(const SKUsedName & ShowName, SKMaterial * pMaterial);
		virtual ~SKAdd();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		SKAdd();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKAdd);
	SKTYPE_MARCO(SKAdd);

	class SKGRAPHIC_API SKMakeValue : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKMakeValue(const SKUsedName & ShowName, SKMaterial * pMaterial,unsigned int uiInputNum);
		virtual ~SKMakeValue();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		SKMakeValue();
	public:
		enum
		{
			IN_A,
			IN_B,
			IN_C,
			IN_D,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};
	};
	DECLARE_Ptr(SKMakeValue);
	SKTYPE_MARCO(SKMakeValue);

	class SKGRAPHIC_API SKDiv : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKDiv(const SKUsedName & ShowName, SKMaterial * pMaterial);
		virtual ~SKDiv();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		SKDiv();


	public:
		enum
		{
			IN_A,
			IN_B,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKDiv);
	SKTYPE_MARCO(SKDiv);


	class SKGRAPHIC_API SKSaturate : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKSaturate(const SKUsedName & ShowName, SKMaterial * pMaterial);
		virtual ~SKSaturate();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
		virtual bool ResetValueType();
	protected:
		SKSaturate();


	public:
		enum
		{
			IN_VALUE,
			IN_MAX
		};
		enum
		{
			OUT_VALUE,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKSaturate);
	SKTYPE_MARCO(SKSaturate);
}
#endif