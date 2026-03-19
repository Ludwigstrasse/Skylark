#ifndef SKPOSTEFFECTPASS_H
#define SKPOSTEFFECTPASS_H
#include "SKPass.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKPostEffectPass : public SKPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~SKPostEffectPass();
		SKPostEffectPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		void SetCustomConstant(MaterialShaderPara &MSPara, SKVShader * pVShader, SKPShader * pPShader);
	public:
		virtual bool Draw(SKRenderer * pRenderer);

		virtual unsigned int GetPassType()	
		{
			return PT_POSTEFFECT;
		}
		unsigned int m_uiRTWidth;
		unsigned int m_uiRTHeight;
		SKTexAllState * m_PColorBuffer;
		SKRenderState m_RenderState;
	};
	DECLARE_Ptr(SKPostEffectPass);
	SKTYPE_MARCO(SKPostEffectPass);
}
#endif