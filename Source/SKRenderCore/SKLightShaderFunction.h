#ifndef SKLIGHTSHADERFUNCTION_H
#define SKLIGHTSHADERFUNCTION_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKLightColor : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKLightColor(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKLightColor();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		SKLightColor();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKLightColor);
	SKTYPE_MARCO(SKLightColor);

	class SKGRAPHIC_API SKLightSpecular : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKLightSpecular(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKLightSpecular();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		SKLightSpecular();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKLightSpecular);
	SKTYPE_MARCO(SKLightSpecular);


	class SKGRAPHIC_API SKLightDir : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKLightDir(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKLightDir();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		SKLightDir();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKLightDir);
	SKTYPE_MARCO(SKLightDir);


	class SKGRAPHIC_API SKLightShaderFunction : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_ALPHA,
			IN_MAX
		};
		SKLightShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKLightShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName(){};
		virtual bool IsValidNodeToThis(SKShaderFunction * pShaderFunction);


	protected:
		SKLightShaderFunction();
	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};

	};
	DECLARE_Ptr(SKLightShaderFunction);
	SKTYPE_MARCO(SKLightShaderFunction);
}
#endif