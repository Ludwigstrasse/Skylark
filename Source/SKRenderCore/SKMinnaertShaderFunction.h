#ifndef SKMINNAERTSHADERFUNCTION_H
#define SKMINNAERTSHADERFUNCTION_H
#include "SKShaderMainFunction.h"
namespace SKEngine2
{
	//ޱ no specular
	class SKGRAPHIC_API SKMinnaertShaderFunction : public SKShaderMainFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY

	public:
		enum
		{
			IN_DIFFUSE_COLOR,
			IN_EMISSIVE_COLOR,
			IN_POWER,
			IN_NORMAL,
			IN_ALPHA,
			IN_REFLECT_MIP,
			IN_REFLECT_POW,
			IN_MAX
		};
		SKMinnaertShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKMinnaertShaderFunction();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const
		{
			return SM_MINNAERT;
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
		SKMinnaertShaderFunction();
		DECLARE_INITIAL

	};
	DECLARE_Ptr(SKMinnaertShaderFunction);
	SKTYPE_MARCO(SKMinnaertShaderFunction);
}
#endif