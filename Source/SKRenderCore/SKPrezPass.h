#ifndef SKPREZPASS_H
#define SKPREZPASS_H
#include "SKPass.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKPrezPass : public SKPass
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKPrezPass();
		virtual ~SKPrezPass();
		

	protected:
		static SKPointer<SKPrezPass>	Default;
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		SKRenderState m_RenderState;
	public:
		virtual bool Draw(SKRenderer * pRenderer);
		
		static SKPrezPass * GetDefault()
		{
			return Default;
		}
		virtual unsigned int GetPassType()
		{
			return PT_PREZ;
		}

	};
	DECLARE_Ptr(SKPrezPass);
	SKTYPE_MARCO(SKPrezPass);
}
#endif