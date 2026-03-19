#ifndef SKPASS_H
#define SKPASS_H
#include "SKObject.h"
#include "SKShaderMap.h"
#include "SKShaderKey.h"
#include "SKPShader.h"
#include "SKVShader.h"
#include "SKGShader.h"
#include "SKDShader.h"
#include "SKHShader.h"
#include "SKMatrix3X3W.h"
namespace SKEngine2
{
	class SKSpatial;
	class SKCamera;
	class SKGRAPHIC_API SKPass : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
		enum //pass type
		{
			PT_MATERIAL,
			PT_NORMALDEPTH,
			PT_PREZ,
			PT_POINT_CUBE_SHADOW,
			PT_POINT_VOLUME_SHADOW,
			PT_DIRECT_VOLUME_SHADOW,
			PT_SHADOW,
			PT_DUAL_PARABOLOID_SHADOW,
			PT_LIGHT_FUNCTION,
			PT_INDIRECT,
			PT_POSTEFFECT,
			PT_MAX
		};
	public:
		virtual ~SKPass() = 0;
	protected:
		SKPass();
		SKSpatial *						m_pSpatial;
		SKCamera * 						m_pCamera;
		unsigned int					m_uiPassId;
		SKShaderSet * m_pVShaderSet;
		SKShaderKey m_VShaderkey;

		SKShaderSet * m_pPShaderSet;
		SKShaderKey m_PShaderkey;

		SKShaderSet * m_pGShaderSet;
		SKShaderKey m_GShaderkey;

		SKMaterialInstance * m_pMaterialInstance;
		MaterialShaderPara MSPara;
	public:
		FORCEINLINE void SetMaterialInstance(SKMaterialInstance * pMaterialInstance)
		{
			m_pMaterialInstance = pMaterialInstance;
		}
		FORCEINLINE void SetSpatial(SKSpatial * pSpatial)
		{
			//SKMAC_ASSERT(pSpatial);
			m_pSpatial = pSpatial;
		}
		FORCEINLINE void SetCamera(SKCamera * pCamera)
		{
			//SKMAC_ASSERT(pCamera);
			m_pCamera = pCamera;
		}
		FORCEINLINE void SetPassId(unsigned int uiPassId)
		{
			m_uiPassId = uiPassId;
		}
		virtual bool Draw(SKRenderer * pRenderer) = 0;
		virtual unsigned int GetPassType() = 0;

		bool GetPShader(MaterialShaderPara& MSPara,SKShaderMap & ShaderMap,const SKUsedName &Name);
		bool GetVShader(MaterialShaderPara& MSPara,SKShaderMap & ShaderMap,const SKUsedName &Name);
		bool GetGShader(MaterialShaderPara& MSPara, SKShaderMap & ShaderMap, const SKUsedName &Name);
	};
	DECLARE_Ptr(SKPass);
	SKTYPE_MARCO(SKPass);
}
#endif