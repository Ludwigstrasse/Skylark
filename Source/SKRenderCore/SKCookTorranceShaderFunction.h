#ifndef SKCOOKTORRANCESHADERFUNCTION_H
#define SKCOOKTORRANCESHADERFUNCTION_H
#include "SKShaderMainFunction.h"
namespace SKEngine2
{
	// good for rough surfaces , no specular
	class SKGRAPHIC_API SKCookTorranceShaderFunction : public SKShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY

	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_SPECULAR_COLOR,
			IN_ROUGHNESS,
			IN_C, //use in alternate blinn
			IN_R,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		enum //Specluar type
		{
			ST_BACKMANN_LOOKUP,
			ST_BACKMANN,
			ST_ALTERNATE_BLINN,
			ST_MAX

		};
		SKCookTorranceShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKCookTorranceShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_COOKTOORANCE;
		}
		FORCEINLINE void SetSpecularType(unsigned int uiSpecularType)
		{
			m_uiSpecularType = uiSpecularType;
		}
		FORCEINLINE unsigned int GetSpecularType()const
		{
			return m_uiSpecularType;
		}
		virtual SKInputNode * GetDiffuseNode()const
		{
			return m_pInput[IN_DIFFUSE_COLOR];
		}
		virtual SKInputNode * GetNormalNode()const
		{
			return m_pInput[IN_NORMAL];
		}
		virtual SKInputNode * GetAlphaNode()const
		{
			return m_pInput[IN_ALPHA];
		}
		virtual SKInputNode * GetEmissiveNode()const
		{
			return m_pInput[IN_EMISSIVE_COLOR];
		}
		virtual SKInputNode * GetReflectMipNode()const
		{
			return m_pInput[IN_REFLECT_MIP];
		}
		virtual SKInputNode * GetReflectPowNode()const
		{
			return m_pInput[IN_REFLECT_POW];
		}
		virtual void SetGlobleValue(unsigned int uiOutPutStringType , SKVShader * pVShader , SKPShader * pPShader);
	protected:
		SKCookTorranceShaderFunction();
		DECLARE_INITIAL
		unsigned int m_uiSpecularType;

	};
	DECLARE_Ptr(SKCookTorranceShaderFunction);
	SKTYPE_MARCO(SKCookTorranceShaderFunction);
}
#endif