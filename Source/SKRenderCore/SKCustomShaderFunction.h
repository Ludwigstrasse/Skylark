#ifndef SKCUSTOMSHADERFUNCTION_H
#define SKCUSTOMSHADERFUNCTION_H
#include "SKShaderMainFunction.h"
namespace SKEngine2
{
	// good for plastics
	class SKGRAPHIC_API SKCustomShaderFunction : public SKShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY

	public:
		enum
		{
			IN_EMISSIVE_COLOR,
			IN_NORMAL,
			IN_ALPHA,
			IN_CUSTOM,
			IN_DIFFUSE_COLOR,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		SKCustomShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKCustomShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual bool GetShaderTreeString(SKString &OutString,unsigned int uiOutPutStringType);
		virtual bool GetInputValueString(SKString &OutString,unsigned int uiOutPutStringType)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_CUSTOM;
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
		virtual bool IsValidNodeToThis(SKShaderFunction * pShaderFunction);

		virtual void CreatLightFunctionString(unsigned int uiOutPutStringType);

		SKString m_LightFunctionString;
		SKString m_CustomContentString;
		SKString m_CustomDefine;
	protected:
		SKCustomShaderFunction();

		DECLARE_INITIAL

	};
	DECLARE_Ptr(SKCustomShaderFunction);
	SKTYPE_MARCO(SKCustomShaderFunction);
}
#endif