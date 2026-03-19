#ifndef SKSHADOWPASS_H
#define SKSHADOWPASS_H
#include "SKPass.h"
namespace SKEngine2
{
	class SKPointLight;
	class SKGRAPHIC_API SKCubeShadowPass : public SKPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~SKCubeShadowPass();
		SKCubeShadowPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		SKRenderState m_RenderState;
	public:
		virtual bool Draw(SKRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_POINT_CUBE_SHADOW;
		}
		SKPointLight * m_pPointLight;

	};

	class SKGRAPHIC_API SKVolumeShadowPass : public SKPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~SKVolumeShadowPass();
		SKVolumeShadowPass();

	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		SKRenderState m_RenderState;
		void SetCustomConstant(MaterialShaderPara &MSPara, SKGShader * pGShader);
	public:
		virtual bool Draw(SKRenderer * pRenderer);
		virtual unsigned int GetPassType()
		{
			if (m_pLocalLight->GetLightType() == SKLight::LT_POINT)
			{
				return PT_POINT_VOLUME_SHADOW;
			} 
			else
			{
				return PT_DIRECT_VOLUME_SHADOW;
			}
			
		}
		SKLocalLight * m_pLocalLight;

	};
	class SKGRAPHIC_API SKShadowPass : public SKPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~SKShadowPass();
		SKShadowPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		SKRenderState m_RenderState;
	public:
		virtual bool Draw(SKRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_SHADOW;
		}
		SKLocalLight * m_pLocalLight;
	}; 

	class SKGRAPHIC_API SKDualParaboloidShadowPass : public SKPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~SKDualParaboloidShadowPass();
		SKDualParaboloidShadowPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		SKRenderState m_RenderState;
	public:
		virtual bool Draw(SKRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_DUAL_PARABOLOID_SHADOW;
		}
		SKPointLight * m_pPointLight;
	}; 

}
#endif