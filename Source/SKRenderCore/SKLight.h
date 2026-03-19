#ifndef SKLIGHT_H
#define SKLIGHT_H
#include "SKNodeComponent.h"
#include "SKVector3.h"
#include "SKVector3W.h"
#include "SKResource.h"
#include "SKVector2.h"
#include "SKCuller.h"
namespace SKEngine2
{
	class SKShadowMapSceneRender;
	class SKStream;
	class VS2DTexture;
	class SKCamera;
	DECLARE_Ptr(SKTexAllState);
	DECLARE_Ptr(SKMaterialInstance);
	DECLARE_Ptr(SKMaterial);
	DECLARE_Proxy(SKMaterial);
	DECLARE_Ptr(SKRenderTarget);
	class SKGRAPHIC_API SKLight : public SKNodeComponent
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		friend class SKSpatial;
		friend class SKScene;
		enum	//LightType
		{
			LT_POINT,
			LT_SPOT,
			LT_DIRECTION,
			LT_SKY,
			LT_MAX
		};
		SKLight();
		virtual ~SKLight() = 0;
	public:
		virtual void UpdateAll(double dAppTime);
		virtual unsigned int GetLightType()const = 0;
		virtual bool IsRelative(SKGeometry * pGeometry);
		virtual bool Cullby(SKCuller & Culler);
		virtual void UpdateLightState(double dAppTime);
		virtual void GetLightRange() = 0;
		virtual void UpdateTransform(double dAppTime);
		virtual void BuildSceneInfo(SKScene * pScene);
	};
	DECLARE_Ptr(SKLight);
	SKTYPE_MARCO(SKLight);
	
	DECLARE_Ptr(SKLightMaterialSceneRender);
	class SKGRAPHIC_API SKLocalLight : public SKLight
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		SKLocalLight();
		virtual ~SKLocalLight() = 0;
		SKColorRGBA m_Diffuse;
		SKColorRGBA m_Specular;
		virtual void DrawNoDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DisableNoDepandenceShadowMap( double dAppTime);
		virtual void DrawDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DisableDepandenceShadowMap(SKCuller & CurCuller, double dAppTime);
		virtual void DrawPorjectShadow(SKCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
		virtual void DrawVolumeShadow(SKCuller & CurCuller, double dAppTime);
		virtual void UpdateAll(double dAppTime);
		virtual void DrawLightMaterial(double dAppTime);
		void SetCastShadow(bool bIsCastShadow);
		FORCEINLINE bool GetCastShadow()
		{
			return m_bIsCastShadow;
		}
		FORCEINLINE void AddShadowScene(SKScene * pSecen)
		{
			if(pSecen)
			{
				for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
				{
					if (m_pScene[i] == pSecen)
					{
						return ;
					}
				}
				m_pScene.AddElement(pSecen);
			}
		}
		
		FORCEINLINE void ClearAllShadowScene()
		{
			m_pScene.Clear();
		}
		virtual SKTexAllState * GetShadowTexture(unsigned int ID = 0)
		{
			return m_pShadowTexture[ID];	
		}
		unsigned int GetShadowTextureNum()
		{
			return m_pShadowTexture.GetNum();
		}
		unsigned int GetShadowResolution()const
		{
			return m_uiRTWidth;
		}
		FORCEINLINE SKTexAllState * GetLightFunDiffuseTexture()const
		{
			return m_pLightFunDiffuseTexture;
		}
		virtual bool SetLightMaterial(SKMaterialR * pMaterial);
		SKREAL m_ZBias;
		SKMatrix3X3W m_LightShadowMatrix;
		void SetLightMaterialRtWidth(unsigned int uiRtWidth);

		FORCEINLINE SKMaterialInstance * GetMaterialInstance()const
		{
			return m_pLightMaterial;
		}
		FORCEINLINE bool HaveLightFun()const
		{
			return (m_pLightMaterial != NULL);
		}
		unsigned int GetLightMaterialRtWidth()
		{
			return m_uiLightMaterialRTWidth;
		}
		FORCEINLINE SKMatrix3X3W &GetMVP()
		{
			return m_MVP;
		}
		virtual void BuildSceneInfo(SKScene * pScene);
		SKVector2 m_LightFunScale;
		SKVector2 m_LightFunOffset;
		SKColorRGBA m_ProjectShadowColor;
		SKREAL m_ProjectShadowFallOff;
	protected:  
		bool m_bIsCastShadow;
		unsigned int m_uiRTWidth;
		SKArray<SKScene *> m_pScene;
		SKArray<SKTexAllStatePtr> m_pShadowTexture;
		SKTexAllStatePtr m_pLightFunDiffuseTexture;
		SKMaterialInstancePtr	m_pLightMaterial;
		unsigned int m_uiLightMaterialRTWidth;
		SKLightMaterialSceneRenderPtr m_pLMSceneRender;
		SKRenderTargetPtr m_pLightFunDiffuseRenderTarget;
		SKRenderTargetPtr m_pLightFunSpecularRenderTarget;
		SKMatrix3X3W m_MVP;
		virtual void GetLightRange() = 0;
		void GetCullerAABBArray(SKCuller & Culler, SKArray<SKAABB3> & AABBArray, unsigned int GetFlag = SKCuller::GF_ALL);
		bool m_bShadowMapDrawEnd;
	};
	DECLARE_Ptr(SKLocalLight);
	SKTYPE_MARCO(SKLocalLight);
	class SKGRAPHIC_API SKIndirectLight : public SKLight
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		SKIndirectLight();
		virtual ~SKIndirectLight() = 0;
		virtual bool IsRelative(SKGeometry * pGeometry);
		virtual bool Cullby(SKCuller & Culler);
		virtual void SetLocalScale(const SKVector3 & fScale);
		virtual void SetLocalRotate(const SKMatrix3X3 & Rotate);
		virtual void SetLocalTransform(const SKTransform & LocalTransform);
		virtual void SetLocalMat(const SKMatrix3X3W SKMat);
		FORCEINLINE void SetRange(SKVector3 & Range)
		{
			m_Range.x = Range.x > 1.0f ? Range.x : m_Range.x;
			m_Range.y = Range.y > 1.0f ? Range.y : m_Range.y;
			m_Range.z = Range.z > 1.0f ? Range.z : m_Range.z;
			GetLightRange();
		}
	protected:
		SKVector3 m_Range;
		virtual void GetLightRange();
		SKAABB3 m_WorldRenderBV;
	};
	DECLARE_Ptr(SKIndirectLight);
	SKTYPE_MARCO(SKIndirectLight);
}
#endif