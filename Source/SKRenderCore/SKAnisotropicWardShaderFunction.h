#ifndef SKANISOTROPICWARDSHADERFUNCTION_H
#define SKANISOTROPICWARDSHADERFUNCTION_H
#include "SKShaderMainFunction.h"
namespace SKEngine2
{
	// good for plastics
	class SKGRAPHIC_API SKAnisotropicWardShaderFunction : public SKShaderMainFunction
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
			IN_ROUGHNESSX,
			IN_ROUGHNESSY,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};

		SKAnisotropicWardShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKAnisotropicWardShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_ANISOTROPICWARD;
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
	protected:
		SKAnisotropicWardShaderFunction();

		DECLARE_INITIAL

	};
	DECLARE_Ptr(SKAnisotropicWardShaderFunction);
	SKTYPE_MARCO(SKAnisotropicWardShaderFunction);
}
#endif