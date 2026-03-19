#ifndef SKSHADERMAINFUNCTION_H
#define SKSHADERMAINFUNCTION_H
#include "SKShaderFunction.h"
#include "SKRenderState.h"
#include "SKShaderKey.h"
namespace SKEngine2
{	class SKLight;
	DECLARE_Ptr(SKLight);
	class SKGRAPHIC_API SKShaderMainFunction : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum
		{
			SM_PHONE,
			SM_OREN_NAYAR,
			SM_MINNAERT,
			SM_STRAUSS,
			SM_SHIRLEY,
			SM_SCHLICK,
			SM_COOKTOORANCE,
			SM_ISOTROPICWARD,
			SM_ANISOTROPICWARD,
			SM_CUSTOM,
			SM_MAX
		};
		
		enum // OutPut String Type
		{
			OST_MATERIAL,
			OST_NORMAL_DEPTH,
			OST_CUB_SHADOW,
			OST_SHADOW,
			OST_DUAL_PARABOLOID_SHADOW,
			OST_INDIRECT,
			OST_MAX
		};
		SKShaderMainFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKShaderMainFunction() = 0;
		virtual bool GetFuntionString(SKString &OutString)const = 0;
		virtual bool GetNormalDepthString(SKString &OutString)const;
		virtual bool GetIndirectRenderString(SKString &OutString)const;
		virtual bool GetCubShadowString(SKString &OutString)const;
		virtual bool GetShadowString(SKString &OutString)const;
		virtual bool GetSDualParaboloidhadowString(SKString &OutString)const;
		virtual FORCEINLINE unsigned int GetSMType()const = 0;
		FORCEINLINE void SetMaterialShaderPara(MaterialShaderPara &MSPara)
		{
			m_MSPara = MSPara;
		}
		FORCEINLINE void SetSRGBWrite(unsigned char uiVESRGBWrite)
		{
			m_uiVESRGBWrite = uiVESRGBWrite;
		}
		FORCEINLINE unsigned char GetSRGBWrite()const
		{
			return m_uiVESRGBWrite;
		}
		FORCEINLINE void SetPassId(unsigned char uiPassId)
		{
			m_uiPassId = uiPassId;
		}
		FORCEINLINE unsigned char GetPassId()const
		{
			return m_uiPassId;
		}
		virtual bool HasNormal()const
		{
			if (GetNormalNode()->GetOutputLink())
			{
				return true;
			}
			return false;
		}

		virtual SKInputNode * GetNormalNode()const = 0;
		virtual SKInputNode * GetAlphaNode()const = 0;
		virtual SKInputNode * GetEmissiveNode()const = 0;
		virtual SKInputNode * GetDiffuseNode()const = 0;
		virtual SKInputNode * GetReflectMipNode()const = 0;
		virtual SKInputNode * GetReflectPowNode()const = 0;
		virtual bool GetShaderTreeString(SKString &OutString,unsigned int uiOutPutStringType);
		virtual bool GetInputValueString(SKString &OutString,unsigned int uiOutPutStringType)const;
		FORCEINLINE SKRenderState & GetRenderState()
		{
			return m_RenderState;
		}
		FORCEINLINE void SetBlendState(SKBlendState * pBlendState)
		{
			m_RenderState.SetBlendState(pBlendState);
		}
		FORCEINLINE void SetDepthStencilState(SKDepthStencilState *pDepthStencilState)
		{
			m_RenderState.SetDepthStencilState(pDepthStencilState);
		}
		FORCEINLINE void SetRasterizerState(SKRasterizerState *pRasterizerState)
		{
			m_RenderState.SetRasterizerState(pRasterizerState);
		}
		FORCEINLINE void AddClipPlane(const SKPlane3 & Plane)
		{
			m_RenderState.AddPlane(Plane);
		}
		FORCEINLINE void AddScissorRect(const SKRect2 & Rect)
		{
			m_RenderState.AddRect(Rect);
		}
		virtual void SetGlobleValue(unsigned int uiOutPutStringType , SKVShader * pVShader , SKPShader * pPShader){}
		virtual void ResetInShaderName(){}
		virtual void GetLightShadow(const MaterialShaderPara& MSPara,SKArray<SKString> ShadowStringArray[SKLight::LT_MAX])const;
		virtual bool IsValidNodeToThis(SKShaderFunction * pShaderFunction);

		FORCEINLINE void SetAlphaTestValue(SKREAL AlphaTestValue)
		{
			m_fAlphaTestValue = AlphaTestValue;
		}
	protected:
		SKShaderMainFunction();
		MaterialShaderPara m_MSPara;
		unsigned char m_uiVESRGBWrite;
		unsigned char m_uiPassId;
		enum //Value Use String
		{
			VUS_WORLD_POS = BIT(1),
			VUS_VIEW_POS = BIT(2) | VUS_WORLD_POS,
			VUS_PROJ_POS = BIT(3) | VUS_VIEW_POS,

			VUS_WORLD_NORMAL = BIT(4),
			VUS_VIEW_NORMAL = BIT(5) | VUS_WORLD_NORMAL,

			VUS_VIEW_WORLD_DIR = BIT(6),

			VUS_ALL = VUS_PROJ_POS | VUS_VIEW_NORMAL | VUS_VIEW_WORLD_DIR,
		};
		void GetValueUseDeclareString(SKString &OutString, unsigned int uiValueUseString);
		SKRenderState	m_RenderState;

		SKREAL m_fAlphaTestValue;

		void GetValueUseString(SKString &OutString, unsigned int uiValueUseString);
		void GetAlphaTestString(SKString &OutString)const;
		void GetSRGBWriteString(SKString &OutString)const;

		void GetNormalString(SKString &OutString)const;
		
		void GetAlphaString(SKString &OutString)const;
		
		void GetEmissiveString(SKString &OutString)const;
		
		void GetDiffuseString(SKString &OutString)const;
		
		void GetReflectMipString(SKString &OutString)const;
		
		void GetReflectPowString(SKString &OutString)const;
		
		bool GetNormalInputValueString(SKString &OutString)const;
		
		bool GetAlphaInputValueString(SKString &OutString)const;
		
		bool GetEmissiveInputValueString(SKString &OutString)const;
		
		bool GetDiffuseInputValueString(SKString &OutString)const;
		
		bool GetReflectMipInputValueString(SKString &OutString)const;
		
		bool GetReflectPowInputValueString(SKString &OutString)const;;
		
	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		
		};
		
	public:
		
	};
	DECLARE_Ptr(SKShaderMainFunction);
	SKTYPE_MARCO(SKShaderMainFunction);
}
#endif