#ifndef SKLIGHTFUNPASS_H
#define SKLIGHTFUNPASS_H
#include "SKPass.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKLightFunPass : public SKPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~SKLightFunPass();
		SKLightFunPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		virtual bool Draw(SKRenderer * pRenderer);

		virtual unsigned int GetPassType()
		{
			return PT_LIGHT_FUNCTION;
		}
		SKLight * m_pLight;
		SKRenderState m_RenderState;
	};
	DECLARE_Ptr(SKLightFunPass);
	SKTYPE_MARCO(SKLightFunPass);
}
#endif