#ifndef SKNORMALDEPTHPASS_H
#define SKNORMALDEPTHPASS_H
#include "SKPass.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKNormalDepthPass : public SKPass
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~SKNormalDepthPass();
		SKNormalDepthPass();
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
			return PT_NORMALDEPTH;
		}

	};
	DECLARE_Ptr(SKNormalDepthPass);
	SKTYPE_MARCO(SKNormalDepthPass);
}
#endif