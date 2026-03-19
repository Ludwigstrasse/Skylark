#ifndef SKPEBLUR_H
#define SKPEBLUR_H
#include "SKPostEffectFunction.h"
namespace SKEngine2
{
	class SKPEBlur : public SKPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKPEBlur(const SKUsedName & ShowName,SKPostEffectSet * pMaterial);
		~SKPEBlur();

	};
	DECLARE_Ptr(SKPEBlur);
	SKTYPE_MARCO(SKPEBlur);
}
#endif