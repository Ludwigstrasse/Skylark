#ifndef SKSHADERKEY_H
#define SKSHADERKEY_H
#include "SKName.h"
#include "SKLight.h"
namespace SKEngine2
{
	class SKGeometry;
	class SKVertexBuffer;
	class SKStream;
	DECLARE_Ptr(SKGeometry);
	DECLARE_Ptr(SKVertexBuffer);
	DECLARE_Ptr(SKMaterialInstance);
	DECLARE_Ptr(SKLight);

	struct MaterialShaderPara 
	{
		MaterialShaderPara()
		{
			pGeometry = NULL;
			pShadowLight = NULL;
			pCamera = NULL;
			pMaterialInstance = NULL;
			LightArray.Clear();
			uiPassId = 0;

		}
		~MaterialShaderPara()
		{
			pGeometry = NULL;
			pShadowLight = NULL;
			pCamera = NULL;
			pMaterialInstance = NULL;
			LightArray.Clear();
			uiPassId = 0;

		}
		SKCamera* pCamera;
		SKMaterialInstance* pMaterialInstance;
		SKArray<SKLight*>	LightArray;
		unsigned int uiPassId;
		SKGeometry* pGeometry;
		SKLight * pShadowLight;
		SKString m_VSShaderPath;
		SKString m_PSShaderPath;
		SKString m_GSShaderPath;
		SKString m_VMainFunName;
		SKString m_PMainFunName;
		SKString m_GMainFunName;
		SKColorRGBA m_SkyLightUpColor;
		SKColorRGBA m_SkyLightDownColor;
	};
	class SKStream;
	class SKGRAPHIC_API SKShaderKey : public SKObject
	{
		
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKShaderKey();
		~SKShaderKey();
	protected:
		SKMapOrder<SKUsedName,unsigned int> m_KeyMap;
	public:
		enum
		{

			MAX_LIGTH_TYPE_MASK = 3,
			MAX_LIGHT_TYPE_NUM = (1 << MAX_LIGTH_TYPE_MASK) - 1
		};
		bool IsHaveTheKey(const SKUsedName & Name,unsigned int &uiKeyId);
		void SetTheKey(const SKUsedName & Name,unsigned int Value);

	public:


		static void SetMaterialVShaderKey(SKShaderKey * pKey,MaterialShaderPara & MSPara,unsigned int uiPassType);

		static void SetMaterialPShaderKey(SKShaderKey * pKey,MaterialShaderPara & MSPara,unsigned int uiPassType);

		static void SetMaterialGShaderKey(SKShaderKey * pKey, MaterialShaderPara & MSPara, unsigned int uiPassType);
		struct ShadowKeyInfo
		{
			ShadowKeyInfo()
			{
				uiShadowNum = 0;
				uiVolumeShadowNum = 0;
				uiDualParaboloidShadowNum = 0 ;
				uiCSMShadowNum = 0;
			}
			unsigned int uiShadowNum;
			unsigned int uiVolumeShadowNum;
			unsigned int uiDualParaboloidShadowNum;
			unsigned int uiCSMShadowNum;
		};
		static unsigned int GenerateKey(const SKArray<SKLight*> & LightArray,ShadowKeyInfo ShadowInfo[SKLight::LT_MAX],unsigned int &uiLightFunctionKey);

		static unsigned int GetLightNum(unsigned int uiLightKey,unsigned int uiLightType);
		
		SKGRAPHIC_API friend bool operator == (const SKShaderKey & Key1,const SKShaderKey & Key2);
		SKGRAPHIC_API friend bool operator > (const SKShaderKey & Key1,const SKShaderKey & Key2);
		SKGRAPHIC_API friend bool operator < (const SKShaderKey & Key1,const SKShaderKey & Key2);

		//=
		void operator =(const SKShaderKey &ShaderKey);

		void GetDefine(SKMap<SKString,SKString> & Define);

		void Clear();
	};
	SKTYPE_MARCO(SKShaderKey);
}
#endif