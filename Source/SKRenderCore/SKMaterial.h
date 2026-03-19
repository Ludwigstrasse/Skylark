#ifndef SKMaterial_H
#define SKMaterial_H
#include "SKObject.h"
#include "SKShaderMainFunction.h"
#include "SKTexAllState.h"
#include "SKLight.h"
#include "SKResource.h"
#include "SKMaterialPass.h"
#include "SKNormalDepthPass.h"
#include "SKShaderKey.h"
#include "SKRenderState.h"
#include "SKPass.h"
#include "SKShadowPass.h"
#include "SKPrezPass.h"
#include "SKLightShaderFunction.h"
#include "SKPostEffectShaderFunction.h"
#include "SKLightFunPass.h"
#include "SKIndirectRenderPass.h"
#include "SKPostEffectPass.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API MaterialCustomPara : public SKObject
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
		MaterialCustomPara();
		virtual ~MaterialCustomPara() = 0;

		SKUsedName ConstValueName;

		MaterialCustomPara & operator =(const MaterialCustomPara &Para)
		{
			ConstValueName = Para.ConstValueName;
			return *this;
		}
		SKGRAPHIC_API friend bool operator >(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName > Para1.ConstValueName;
		}
		SKGRAPHIC_API friend bool operator <(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName < Para1.ConstValueName;
		}
		SKGRAPHIC_API friend bool operator ==(const MaterialCustomPara &Para1,const MaterialCustomPara &Para2)
		{
			return Para2.ConstValueName == Para1.ConstValueName;
		}
	};
	DECLARE_Ptr(MaterialCustomPara);
	SKTYPE_MARCO(MaterialCustomPara);
	class SKGRAPHIC_API CustomFloatValue : public MaterialCustomPara
	{
	public:
		DECLARE_RTTI;
		//PRIORITY

		DECLARE_INITIAL
		CustomFloatValue();
		virtual ~CustomFloatValue();
		SKArray<SKREAL> Value;
		CustomFloatValue & operator =(const CustomFloatValue &Para)
		{
			MaterialCustomPara::operator =(Para);
			Value = Para.Value;
			return *this;
		}
		
	};
	DECLARE_Ptr(CustomFloatValue);
	SKTYPE_MARCO(CustomFloatValue);
	class CustomTexSampler : public MaterialCustomPara
	{
	public:
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
		CustomTexSampler();
		virtual ~CustomTexSampler();
		SKTexAllStateRPtr m_pTexture;
		CustomTexSampler & operator =(const CustomTexSampler &Para)
		{
			MaterialCustomPara::operator =(Para);
			m_pTexture = Para.m_pTexture;
			return *this;
		}
	};
	DECLARE_Ptr(CustomTexSampler);
	SKTYPE_MARCO(CustomTexSampler);
	class SKGRAPHIC_API SKMaterialInterface : public SKObject , public SKResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		
		SKMaterialInterface();
		virtual ~SKMaterialInterface() = 0;


		virtual unsigned int GetResourceType()const
		{
			return RT_MATERIAL;
		}
		
	};
	DECLARE_Ptr(SKMaterialInterface);
	SKTYPE_MARCO(SKMaterialInterface);
	class SKGRAPHIC_API SKCustomMaterial : public SKMaterialInterface
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKCustomMaterial(const SKUsedName & ResourceName,
						const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
						const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName);
		virtual ~SKCustomMaterial();
		bool UseMaterial();
		void PreLoad(const SKUsedName & ResourceName,
			const TCHAR * pVShaderFileName,const TCHAR * pVMainFunName,
			const TCHAR * pPShaderFileName,const TCHAR * pPMainFunName);
		friend class SKRenderer;
		static SKCustomMaterial * GetPostGray()
		{
			return ms_pPostGray;
		}
		static SKCustomMaterial * GetGammaCorrect()
		{
			return ms_pGammaCorrect;
		}
		static SKCustomMaterial * GetPostScreenQuad()
		{
			return ms_pPostScreenQuad;
		}
		static SKCustomMaterial * GetPostVolumeShadowMap()
		{
			return ms_pPostVolumeShadowMap;
		}
		static SKCustomMaterial * GetSSR()
		{
			return ms_pSSR;
		}
		static SKCustomMaterial * GetProjectShadowPre()
		{
			return ms_pProjectShadowPre;
		}
		static SKCustomMaterial * GetProjectShadow()
		{
			return ms_pProjectShadow;
		}
		static SKCustomMaterial * GetFont()
		{
			return ms_pFont;
		}
		virtual void SetVShaderValue(const SKUsedName & Name,void *fValue);

		virtual void SetVShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllState * pTex);

		virtual void SetPShaderValue(const SKUsedName & Name,void *fValue);

		virtual void SetPShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllState * pTex);

		
		SKVShader *GetCurVShader(SKShaderKey & VShaderKey);
		SKPShader *GetCurPShader(SKShaderKey & PShaderKey);
		SKGShader *GetCurGShader(SKShaderKey & GShaderKey);

		SKRenderState & GetRenderState();
		void SetBlendState(SKBlendState * pBlendState);
		void SetDepthStencilState(SKDepthStencilState *pDepthStencilState);
		void SetRasterizerState(SKRasterizerState *pRasterizerState);
		void AddClipPlane(const SKPlane3 & Plane);
		void AddScissorRect(const SKRect2 & Rect);
	protected:
		bool GetVShader(SKShaderKey & VShaderKey);
		bool GetPShader(SKShaderKey & PShaderKey);
		bool GetGShader(SKShaderKey & GShaderKey);
		SKCustomMaterial();
		SKVShaderPtr m_pVShader;
		SKPShaderPtr m_pPShader;
		SKGShaderPtr m_pGShader;
		SKRenderState	m_RenderState;

		SKUsedName m_VShaderName;
		SKUsedName m_PShaderName;
		SKUsedName m_GShaderName;
		SKString m_VMainFunName;
		SKString m_PMainFunName;
		SKString m_GMainFunName;
		SKShaderSet * m_pVShaderSet;
		SKShaderSet * m_pPShaderSet;
		SKShaderSet * m_pGShaderSet;
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static void LoadAllDeviceShader();
	protected:
		static SKPointer<SKCustomMaterial> ms_pPostGray;
		static SKPointer<SKCustomMaterial> ms_pPostScreenQuad;
		static SKPointer<SKCustomMaterial> ms_pPostVolumeShadowMap;
		static SKPointer<SKCustomMaterial> ms_pProjectShadowPre;
		static SKPointer<SKCustomMaterial> ms_pProjectShadow;
		static SKPointer<SKCustomMaterial> ms_pGammaCorrect;
		static SKPointer<SKCustomMaterial> ms_pSSR;
		static SKPointer<SKCustomMaterial> ms_pFont;
		static bool LoadDefault();
		void LoadDeviceShader();
		static SKShaderKeyPtr ms_NULLShaderKey;
	};
	DECLARE_Ptr(SKCustomMaterial);
	SKTYPE_MARCO(SKCustomMaterial);

	class SKGRAPHIC_API SKMaterialBase : public SKMaterialInterface
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		
		SKMaterialBase();
		virtual ~SKMaterialBase();

	protected:
		SKArrayOrder<CustomFloatValue> m_VShaderCustomValue;
		SKArrayOrder<CustomTexSampler>m_VShaderCustomTex;

		SKArrayOrder<CustomFloatValue> m_PShaderCustomValue;
		SKArrayOrder<CustomTexSampler>m_PShaderCustomTex;


		virtual void SetVShaderValue(const SKUsedName & Name,void *fValue);

		virtual void SetVShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex);

		virtual void SetPShaderValue(const SKUsedName & Name,void *fValue);

		virtual void SetPShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex);

	};
	DECLARE_Ptr(SKMaterialBase);
	SKTYPE_MARCO(SKMaterialBase);
	class SKGRAPHIC_API SKMaterial : public SKMaterialBase
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		
		DECLARE_INITIAL
	protected:
		SKMaterial();
		SKUsedName m_ShowName;
		SKArray<SKShaderMainFunction*> m_pShaderMainFunction;
		SKArray<SKShaderFunction *> m_pShaderFunctionArray;
		SKLightShaderFunction * m_pLightShaderFunction;
		SKPostEffectShaderFunction * m_pPostEffectShaderFunction;

		bool m_bIsAllReadyLink;
		
		SKPassPtr m_pPass[SKPass::PT_MAX];
		bool m_bIsCombine;
		
	public:
		void SetBlendState(SKBlendState * pBlendState,unsigned int uiPassId = 0);
		void SetDepthStencilState(SKDepthStencilState *pDepthStencilState,unsigned int uiPassId = 0);
		void SetRasterizerState(SKRasterizerState *pRasterizerState,unsigned int uiPassId = 0);
		void AddClipPlane(const SKPlane3 & Plane,unsigned int uiPassId = 0);
		void AddScissorRect(const SKRect2 & Rect,unsigned int uiPassId = 0);
		void AddPass(unsigned int uiMUT);
		FORCEINLINE SKRenderState & GetRenderState(unsigned int uiPassId)
		{
			return m_pShaderMainFunction[uiPassId]->GetRenderState();
		}
		FORCEINLINE void SetCombine(bool IsCombine)
		{
			m_bIsCombine = IsCombine;
		}
		FORCEINLINE bool GetCombine()const
		{
			return m_bIsCombine;
		}
		friend class SKRenderer;
		friend class SKMaterialInstance;
		enum //MATERIAL USED TYPE
		{
			MUT_PHONE,
			MUT_OREN_NAYAR,
			MUT_MINNAERT,
			MUT_STRAUSS,
			MUT_SHIRLEY,
			MUT_SCHLICK,
			MUT_COOKTOORANCE,
			MUT_ISOTROPICWARD,
			MUT_ANISOTROPICWARD,
			MUT_CUSTOM,
			MUT_LIGHT,
			MUT_POSTEFFECT,
			MUT_MAX
		};
		SKMaterial(const SKUsedName &ShowName, unsigned int uiMUT = MUT_PHONE);
		virtual ~SKMaterial();
		FORCEINLINE SKIndirectRenderPass * GetIndirectRenderPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_INDIRECT];
			return (SKIndirectRenderPass *)pPass;
		}
		FORCEINLINE SKMaterialPass * GetMaterialPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_MATERIAL];
			return (SKMaterialPass *)pPass;
		}
		FORCEINLINE SKNormalDepthPass * GetNormalDepthPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_NORMALDEPTH];
			return (SKNormalDepthPass *)pPass;
		}
		FORCEINLINE SKCubeShadowPass *GetCubeShadowPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_POINT_CUBE_SHADOW];
			return (SKCubeShadowPass *)pPass;

		}
		FORCEINLINE SKPrezPass * GetPrezPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_PREZ];
			return (SKPrezPass *)pPass;
		}
		FORCEINLINE SKVolumeShadowPass * GetVolumeShadowPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_POINT_VOLUME_SHADOW];
			return (SKVolumeShadowPass *)pPass;
		}
		FORCEINLINE SKShadowPass * GetShadowMapPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_SHADOW];
			return (SKShadowPass *)pPass;
		}
		FORCEINLINE SKDualParaboloidShadowPass * GetDualParaboloidShadowPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_DUAL_PARABOLOID_SHADOW];
			return (SKDualParaboloidShadowPass *)pPass;
		}
		FORCEINLINE SKLightFunPass * GetLightFunPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_LIGHT_FUNCTION];
			return (SKLightFunPass *)pPass;
		}
		FORCEINLINE SKPostEffectPass * GetPostEffectPass()const
		{
			SKPass * pPass = m_pPass[SKPass::PT_POSTEFFECT];
			return (SKPostEffectPass *)pPass;
		}
		void AddShaderFuntion(SKShaderFunction *pShaderFuntion);
		void DeleteShaderFuntion(SKShaderFunction *pShaderFuntion);
		FORCEINLINE SKShaderMainFunction * GetShaderMainFunction(unsigned char uPassId = 0)const
		{
			return m_pShaderMainFunction[uPassId];
		}
		FORCEINLINE SKLightShaderFunction * GetLightShaderFunction()const
		{
			return m_pLightShaderFunction;
		}

		FORCEINLINE SKPostEffectShaderFunction * GetPostEffectShaderFunction()const
		{
			return m_pPostEffectShaderFunction;
		}
		unsigned int GetShaderMainFunctionNum()const
		{
			return m_pShaderMainFunction.GetNum();
		}
		FORCEINLINE const SKUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		FORCEINLINE bool IsAllReadyLink()const
		{
			return m_bIsAllReadyLink;
		}

		void LinkAllResource();
		void ClearShaderFuntionVisitFlagFalse();
		bool GetShaderTreeString(SKString & OutString,MaterialShaderPara &MSPara,unsigned int uiOST,unsigned char uPassId = 0);
		bool GetCustomDeclareString(SKString & OutString,unsigned char uPassId = 0);
		void CreateCustomValue(SKPShader * pShader);

		void CreateConstValueDeclare(SKString & OutString,unsigned int uiRegisterID);

		void CreateCustomTexture(SKPShader * pShader);
		void CreateTextureDeclare(SKString & OutString,unsigned int uiRegisterID);

		void SetGlobleValue(SKPass * pPass, unsigned int uiPassId,SKVShader * pVShader , SKPShader * pPShader);
		
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		
		
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
	protected:
		static SKPointer<SKMaterial> Default;
		static SKPointer<SKMaterial> DefaultOnlyColor;
		static SKPointer<SKMaterial> DefaultOnlyVertexColor;

		static SKPointer<SKMaterial> DefaultOnlyColorDisableDepth;
		static SKPointer<SKMaterial> DefaultOnlyVertexColorDisableDepth;
		static bool LoadDefault();
	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		friend class SKRenderer;
		friend class SKResourceManager;
		static const SKMaterial *GetDefalut()
		{
			return Default;
		}
		
		static const SKMaterial *GetDefaultOnlyColor()
		{
			return DefaultOnlyColor;
		}
		
		static const SKMaterial *GetDefaultOnlyVertexColor()
		{
			return DefaultOnlyVertexColor;
		}
		
		static const SKMaterial *GetDefaultOnlyColorDisableDepth()
		{
			return DefaultOnlyColorDisableDepth;
		}

		static const SKMaterial *GetDefaultOnlyVertexColorDisableDepth()
		{
			return DefaultOnlyVertexColorDisableDepth;
		}
		// more bigger more later render
		unsigned int m_uiCustomLayer;
	};
	DECLARE_Ptr(SKMaterial);
	DECLARE_Proxy(SKMaterial);
	SKTYPE_MARCO(SKMaterial);

	class SKGRAPHIC_API SKMaterialInstance: public SKObject
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		SKMaterialInstance(SKMaterialR * pMaterial);
		virtual ~SKMaterialInstance();
	protected:
		SKMaterialInstance();
		SKArrayOrder<CustomFloatValue> m_VShaderCustomValue;
		SKArrayOrder<CustomFloatValue> m_PShaderCustomValue;
		SKArrayOrder<CustomTexSampler>	m_VShaderCustomTex;		
		SKArrayOrder<CustomTexSampler>	m_PShaderCustomTex;		


		SKVShaderPtr m_pCurVShader[SKPass::PT_MAX];
		SKPShaderPtr m_pCurPShader[SKPass::PT_MAX];
		SKGShaderPtr m_pCurGShader[SKPass::PT_MAX];
		SKMaterialRPtr m_pMaterial;

	public:
		friend class SKSpatial;

		void SetPShaderValue(SKPShader * pPShader);
		void SetVShaderValue(SKVShader * pVShader);
		void SetVShaderValue(const SKUsedName & Name,void *fValue,unsigned int uiSize);
		void DeleteVShaderValue(const SKUsedName &Name);

		void SetVShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex);
		void DeleteVShaderTexture(const SKUsedName & TexSamplerNodeName);

		void SetPShaderValue(const SKUsedName & Name,void *fValue,unsigned int uiSize);
		void DeletePShaderValue(const SKUsedName &Name);

		void SetPShaderTexture(const SKUsedName & TexSamplerNodeName,SKTexAllStateR * pTex);
		void DeletePShaderTexture(const SKUsedName & TexSamplerNodeName);

		void GetAllMaterialPara();
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);
		FORCEINLINE SKMaterial* GetMaterial()const
		{
			return m_pMaterial->GetResource();
		}
		FORCEINLINE SKMaterialR * GetMaterialR()const
		{
			return m_pMaterial;
		}
		friend class SKMaterialPass;
		friend class SKNormalDepthPass;
		friend class SKPrezPass;
		friend class SKCubeShadowPass;
		friend class SKVolumeShadowPass;
		friend class SKShadowPass;
		friend class SKDualParaboloidShadowPass;
		friend class SKLightFunPass;
		friend class SKProjectPreShadowPass;
		friend class SKProjectShadowPass;
		friend class SKPass;
		friend class SKIndirectRenderPass;
		friend class SKPostEffectPass;
	};
	DECLARE_Ptr(SKMaterialInstance);
	SKTYPE_MARCO(SKMaterialInstance);
	class SKGRAPHIC_API SKMaterialOnlyTexture : public SKMaterial
	{
	protected:
		SKMaterialOnlyTexture();
	public:
		SKMaterialOnlyTexture(const SKUsedName &ShowName,SKTexAllStateR * pTexture);
		virtual ~SKMaterialOnlyTexture();
		
	};
	DECLARE_Ptr(SKMaterialOnlyTexture);
	SKTYPE_MARCO(SKMaterialOnlyTexture);
	class SKGRAPHIC_API SKMaterialTextureAndNormal : public SKMaterial
	{
	protected:
		SKMaterialTextureAndNormal();
	public:
		SKMaterialTextureAndNormal(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture);
		virtual ~SKMaterialTextureAndNormal();
	};
	DECLARE_Ptr(SKMaterialTextureAndNormal);
	SKTYPE_MARCO(SKMaterialTextureAndNormal);
	class SKGRAPHIC_API SKMaterialOnlyEmissive : public SKMaterial
	{
	protected:
		SKMaterialOnlyEmissive();
	public:
		SKMaterialOnlyEmissive(const SKUsedName &ShowName);
		virtual ~SKMaterialOnlyEmissive();

	};
	DECLARE_Ptr(SKMaterialOnlyEmissive);
	SKTYPE_MARCO(SKMaterialOnlyEmissive);
	class SKGRAPHIC_API SKMaterialNoLight : public SKMaterial
	{
	protected:
		SKMaterialNoLight();
	public:
		SKMaterialNoLight(const SKUsedName &ShowName);
		virtual ~SKMaterialNoLight();

	};
	DECLARE_Ptr(SKMaterialNoLight);
	SKTYPE_MARCO(SKMaterialNoLight);
	class SKGRAPHIC_API SKMaterialOrenNayar : public SKMaterial
	{
	protected:
		SKMaterialOrenNayar();
	public:
		SKMaterialOrenNayar(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialOrenNayar();

	};
	DECLARE_Ptr(SKMaterialOrenNayar);
	SKTYPE_MARCO(SKMaterialOrenNayar);
	class SKGRAPHIC_API SKMaterialPhone : public SKMaterial
	{
	protected:
		SKMaterialPhone();
	public:
		SKMaterialPhone(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture,bool bPhoneSpecular);
		virtual ~SKMaterialPhone();

	};
	DECLARE_Ptr(SKMaterialPhone);
	SKTYPE_MARCO(SKMaterialPhone);
	class SKGRAPHIC_API SKMaterialPhoneTwoPass : public SKMaterial
	{
	protected:
		SKMaterialPhoneTwoPass();
	public:
		SKMaterialPhoneTwoPass(const SKUsedName &ShowName, SKTexAllStateR * pDiffuseTexture, SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture, bool bPhoneSpecular);
		virtual ~SKMaterialPhoneTwoPass();

	};
	DECLARE_Ptr(SKMaterialPhoneTwoPass);
	SKTYPE_MARCO(SKMaterialPhoneTwoPass)
	class SKGRAPHIC_API SKMaterialMinnaert : public SKMaterial
	{
	protected:
		SKMaterialMinnaert();
	public:
		SKMaterialMinnaert(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialMinnaert();

	};
	DECLARE_Ptr(SKMaterialMinnaert);
	SKTYPE_MARCO(SKMaterialMinnaert);
	class SKGRAPHIC_API SKMaterialStrauss : public SKMaterial
	{
	protected:
		SKMaterialStrauss();
	public:
		SKMaterialStrauss(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialStrauss();

	};
	DECLARE_Ptr(SKMaterialStrauss);
	SKTYPE_MARCO(SKMaterialStrauss);
	class SKGRAPHIC_API SKMaterialShirley : public SKMaterial
	{
	protected:
		SKMaterialShirley();
	public:
		SKMaterialShirley(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialShirley();

	};
	DECLARE_Ptr(SKMaterialShirley);
	SKTYPE_MARCO(SKMaterialShirley);
	class SKGRAPHIC_API SKMaterialSchlick : public SKMaterial
	{
	protected:
		SKMaterialSchlick();
	public:
		SKMaterialSchlick(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialSchlick();

	};
	DECLARE_Ptr(SKMaterialSchlick);
	SKTYPE_MARCO(SKMaterialSchlick);
	class SKGRAPHIC_API SKMaterialCookTorrance : public SKMaterial
	{
	protected:
		SKMaterialCookTorrance();
	public:
		SKMaterialCookTorrance(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture,bool bSample);
		virtual ~SKMaterialCookTorrance();

	};
	DECLARE_Ptr(SKMaterialCookTorrance);
	SKTYPE_MARCO(SKMaterialCookTorrance);
	class SKGRAPHIC_API SKMaterialIsotropicWard : public SKMaterial
	{
	protected:
		SKMaterialIsotropicWard();
	public:
		SKMaterialIsotropicWard(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialIsotropicWard();

	};
	DECLARE_Ptr(SKMaterialIsotropicWard);
	SKTYPE_MARCO(SKMaterialIsotropicWard);
	class SKGRAPHIC_API SKMaterialAnisotropicWard : public SKMaterial
	{
	protected:
		SKMaterialAnisotropicWard();
	public:
		SKMaterialAnisotropicWard(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,
			SKTexAllStateR * pSpecularTexture, SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialAnisotropicWard();

	};
	DECLARE_Ptr(SKMaterialAnisotropicWard);
	SKTYPE_MARCO(SKMaterialAnisotropicWard);
	class SKGRAPHIC_API SKMaterialCustom : public SKMaterial
	{
	protected:
		SKMaterialCustom();
	public:
		SKMaterialCustom(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture,SKTexAllStateR * pNormalTexture,SKTexAllStateR * pEmissiveTexture);
		virtual ~SKMaterialCustom();

	};
	DECLARE_Ptr(SKMaterialCustom);
	SKTYPE_MARCO(SKMaterialCustom);
	class SKGRAPHIC_API SKLightMaterial : public SKMaterial
	{
	protected:
		SKLightMaterial();
	public:
		SKLightMaterial(const SKUsedName &ShowName,SKTexAllStateR * pDiffuseTexture);
		virtual ~SKLightMaterial();

	};
	DECLARE_Ptr(SKLightMaterial);
	SKTYPE_MARCO(SKLightMaterial);
	class SKGRAPHIC_API SKPostEffectMaterial : public SKMaterial
	{
	protected:
		SKPostEffectMaterial();
	public:
		SKPostEffectMaterial(const SKUsedName &ShowName);
		virtual ~SKPostEffectMaterial();

	};
	DECLARE_Ptr(SKPostEffectMaterial);
	SKTYPE_MARCO(SKPostEffectMaterial);
}
#endif