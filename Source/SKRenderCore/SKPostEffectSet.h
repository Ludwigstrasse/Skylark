#ifndef SKPOSTEFFECTSET_H
#define SKPOSTEFFECTSET_H
#include "SKObject.h"
#include "SKResource.h"
#include "SKCuller.h"
namespace SKEngine2
{
	class SKPostEffectFunction;
	class SKPEBeginFunction;
	class SKPEEndFunction;
	class SKScene;
	class SKCamera;
	class VS2DTexture;
	class SKRenderTarget;
	class SKStream;
	class SKGRAPHIC_API SKPostEffectSet : public SKObject , public SKResource
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	protected:
		SKPostEffectSet();
	public:
		SKPostEffectSet(const SKUsedName &ShowName);
		virtual ~SKPostEffectSet();

	public:


	
		void AddPostEffectFunction(SKPostEffectFunction * pPostEffectFunction);
		void DeletePostEffectFunction(SKPostEffectFunction * pPostEffectFunction);
		virtual unsigned int GetResourceType()const
		{
			return RT_POSTEFFECT;
		}
		void Draw(SKCuller & Culler,double dAppTime);
		SKRenderTarget * GetEndTarget(unsigned int i);
		void SetBeginTargetArray(SKArray<SKRenderTarget *> * pBeginTargetArray);
		void GetRT(unsigned int uiWidth,unsigned int uiHeight);
		void DisableRT();
		static const SKPostEffectSet *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		SKPostEffectFunction * GetPEFunctionFromShowName(const SKUsedName & ShowName);

		bool SetEndTarget(SKRenderTarget *pEndTarget);
	protected:
		SKArray<SKPostEffectFunction *> m_pPostEffectFunctionArray;
		SKPEBeginFunction*  m_pPEBeginFunc;
		SKPEEndFunction * m_pPEEndFunc;
		SKUsedName m_ShowName;
		static SKPointer<SKPostEffectSet> Default;
	};
	DECLARE_Ptr(SKPostEffectSet);
	SKTYPE_MARCO(SKPostEffectSet);
	DECLARE_Proxy(SKPostEffectSet);
	class SKPEGray;
	class SKGRAPHIC_API SKPESetGray : public SKPostEffectSet
	{
	public:
		SKPESetGray(const SKUsedName &ShowName);
		virtual ~SKPESetGray();
	};
	DECLARE_Ptr(SKPESetGray);
	SKTYPE_MARCO(SKPESetGray);

	DECLARE_Ptr(SKMaterial);
	DECLARE_Proxy(SKMaterial);
	class SKGRAPHIC_API SKPESetMaterialAndGray : public SKPostEffectSet
	{
	public:
		SKPESetMaterialAndGray(const SKUsedName &ShowName,SKMaterialR * pMaterial);
		virtual ~SKPESetMaterialAndGray();
	};
	DECLARE_Ptr(SKPESetMaterialAndGray);
	SKTYPE_MARCO(SKPESetMaterialAndGray);

}
#endif