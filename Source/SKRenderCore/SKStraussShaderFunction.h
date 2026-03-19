#ifndef SKSTRAUSSSHADERFUNCTION_H
#define SKSTRAUSSSHADERFUNCTION_H
#include "SKShaderMainFunction.h"
namespace SKEngine2
{
	//smoothness and metalness , awide range of materials
	class SKGRAPHIC_API SKStraussShaderFunction : public SKShaderMainFunction
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
			IN_SMOOTHNESS,
			IN_METALNESS,
			IN_TRANSPARENCY,
			IN_K,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		SKStraussShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKStraussShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_STRAUSS;
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
		bool UseLookUpTable()const
		{
			return true;
		}
	protected:
		SKStraussShaderFunction();
		DECLARE_INITIAL

	};
	DECLARE_Ptr(SKStraussShaderFunction);
	SKTYPE_MARCO(SKStraussShaderFunction);
}
#endif