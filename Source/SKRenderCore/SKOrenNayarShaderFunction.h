#ifndef SKORENNAYARSHADERFUNCTION_H
#define SKORENNAYARSHADERFUNCTION_H
#include "SKShaderMainFunction.h"
namespace SKEngine2
{
	// good for rough surfaces , no specular
	class SKGRAPHIC_API SKOrenNayarShaderFunction : public SKShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_ROUGHNESS_SQUARED,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		SKOrenNayarShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKOrenNayarShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_OREN_NAYAR;
		}
		bool UseLookUpTable()const
		{
			return true;
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
		SKOrenNayarShaderFunction();
		DECLARE_INITIAL
	
	};
	DECLARE_Ptr(SKOrenNayarShaderFunction);
	SKTYPE_MARCO(SKOrenNayarShaderFunction);
}
#endif