#ifndef SKCAMERASHADERFUNCTION_H
#define SKCAMERASHADERFUNCTION_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;

	class SKGRAPHIC_API SKViewWorldDir : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKViewWorldDir(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKViewWorldDir();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		SKViewWorldDir();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKViewWorldDir);
	SKTYPE_MARCO(SKViewWorldDir);


	class SKGRAPHIC_API SKCameraWorldPos : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKCameraWorldPos(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKCameraWorldPos();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		SKCameraWorldPos();


	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};


	};
	DECLARE_Ptr(SKCameraWorldPos);
	SKTYPE_MARCO(SKCameraWorldPos);

}
#endif