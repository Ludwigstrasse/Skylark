#ifndef SKVERTEXCOLOR_H
#define SKVERTEXCOLOR_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKVertexColor : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKVertexColor(const SKUsedName & ShowName,SKMaterial * pMaterial);
		virtual ~SKVertexColor();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		SKVertexColor();
		unsigned int m_uiColorIndex;

	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};
		enum //Color Index
		{
			CI_1,
			CI_2,
			CI_MAX
		};
		FORCEINLINE void SetColorIndex(unsigned int uiIndex)
		{
			if (uiIndex >= CI_MAX)
			{
				return;
			}
			m_uiColorIndex = uiIndex;
		}
	};
	DECLARE_Ptr(SKVertexColor);
	SKTYPE_MARCO(SKVertexColor);
}
#endif