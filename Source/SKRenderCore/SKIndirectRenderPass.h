#ifndef SKINDIRECTRENDERPASS_H
#define SKINDIRECTRENDERPASS_H
#include "SKPass.h"

namespace SKEngine2
{
	class SKGRAPHIC_API SKIndirectRenderPass : public SKPass
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		SKIndirectRenderPass();
		virtual ~SKIndirectRenderPass();
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
			return PT_INDIRECT;
		}
	protected:
		SKArray<SKLight *> m_Light;
	public:
		virtual bool Draw(SKRenderer * pRenderer);


	};
	DECLARE_Ptr(SKIndirectRenderPass);
	SKTYPE_MARCO(SKIndirectRenderPass);
}
#endif