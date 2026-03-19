#ifndef SKPEGRAY_H
#define SKPEGRAY_H
#include "SKPostEffectFunction.h"
namespace SKEngine2
{
	class SKPEGray : public SKPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKPEGray (const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet);
		~SKPEGray ();
		virtual SKPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(SKCuller & Culler,double dAppTime);
		enum
		{
			INPUT_COLOR
		};
		enum
		{
			OUT_COLOR
		};
	protected:
		SKPEGray ();

	};
	DECLARE_Ptr(SKPEGray);
	SKTYPE_MARCO(SKPEGray);
}
#endif