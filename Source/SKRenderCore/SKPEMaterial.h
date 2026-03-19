#ifndef SKPEMATERIAL_H
#define SKPEMATERIAL_H
#include "SKPostEffectFunction.h"
namespace SKEngine2
{
	DECLARE_Ptr(SKMaterialInstance);
	DECLARE_Ptr(SKMaterial);
	DECLARE_Proxy(SKMaterial);
	class SKPEMaterial : public SKPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		SKPEMaterial(const SKUsedName & ShowName, SKPostEffectSet * pPostEffectSet);
		~SKPEMaterial();
		virtual SKPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(SKCuller & Culler, double dAppTime);
		void SetMaterial(SKMaterialR * pMaterial);
		enum
		{
			INPUT_COLOR
		};
		enum
		{
			OUT_COLOR
		};

	protected:
		SKPEMaterial();
		SKMaterialInstancePtr	m_pMaterialInstance;
	};
	DECLARE_Ptr(SKPEMaterial);
	SKTYPE_MARCO(SKPEMaterial);
	
}
#endif