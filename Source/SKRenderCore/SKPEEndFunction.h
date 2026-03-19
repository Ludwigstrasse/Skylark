#ifndef SKPEENDFUNCTION_H
#define SKPEENDFUNCTION_H
#include "SKPostEffectFunction.h"
namespace SKEngine2
{
	class SKPEEndFunction : public SKPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		enum
		{
			INPUT_COLOR
		};
		SKPEEndFunction(const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet);
		
		~SKPEEndFunction();

	protected:
		SKPEEndFunction();
		virtual SKPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(SKCuller & Culler,double dAppTime);
	};
	DECLARE_Ptr(SKPEEndFunction);
	SKTYPE_MARCO(SKPEEndFunction);

}
#endif