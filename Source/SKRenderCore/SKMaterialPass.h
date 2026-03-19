#ifndef SKMATERIALPASS_H
#define SKMATERIALPASS_H
#include "SKPass.h"

namespace SKEngine2
{
	class SKMaterialInstance;
	class SKGRAPHIC_API SKMaterialPass : public SKPass
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		SKMaterialPass();
		virtual ~SKMaterialPass();
	public:

		FORCEINLINE void AddLight(SKLight * pLight)
		{
			if (pLight)
			{
				for (unsigned int i = 0 ; i < m_Light.GetNum() ;i++)
				{
					if (m_Light[i] == pLight)
					{
						return ;
					}
				}
				m_Light.AddElement(pLight);
			}
		}
		
		virtual unsigned int GetPassType()
		{
			return PT_MATERIAL;
		}
	protected:
		SKArray<SKLight *> m_Light;
		SKRenderState m_RenderSceondPassUsed;
		
	public:
		

	public:
		virtual bool Draw(SKRenderer * pRenderer);


	};
	DECLARE_Ptr(SKMaterialPass);
	SKTYPE_MARCO(SKMaterialPass);
}
#endif