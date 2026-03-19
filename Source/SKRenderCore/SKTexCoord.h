#ifndef SKTEXCOORD_H
#define SKTEXCOORD_H
#include "SKShaderFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKTexCoord : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKTexCoord(const SKUsedName & ShowName, SKMaterial * pMaterial);
		virtual ~SKTexCoord();
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual void ResetInShaderName();
	protected:
		SKTexCoord();
		unsigned int m_uiTexCoordIndex;

	public:
		enum
		{
			OUT_COLOR,
			OUT_MAX
		};
		enum //TexCoord Index
		{
			TI_1,
			TI_2,
			TI_MAX
		};
		FORCEINLINE void SetTexCoordIndex(unsigned int uiIndex)
		{
			if (uiIndex >= TI_MAX)
			{
				return;
			}
			m_uiTexCoordIndex = uiIndex;
		}
	};
	DECLARE_Ptr(SKTexCoord);
	SKTYPE_MARCO(SKTexCoord);
}
#endif