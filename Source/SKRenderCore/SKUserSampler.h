#ifndef SKUSERSAMPLER_H
#define SKUSERSAMPLER_H
#include "SKMemManager.h"
#include "SKMath.h"
#include "SKObject.h"
#include "SKTexAllState.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKUserSampler : public SKObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		SKUserSampler(const SKUsedName & Name,unsigned int uiTexType,unsigned int uiRegisterIndex,unsigned int uiRegisterNum);
		virtual ~SKUserSampler();

		FORCEINLINE unsigned int GetTexType()const
		{
			return m_uiTexType;
		}
		
		FORCEINLINE SKTexAllState * GetTex(unsigned int uiIndex)const
		{
			return m_pTextureArray[uiIndex];
		}
		FORCEINLINE const SKUsedName & GetName()const
		{
			return m_Name;
		}
		FORCEINLINE void SetTex(SKTexAllState * pTex,unsigned int uiIndex)
		{
			if (uiIndex >= m_uiRegisterNum)
			{
				return ;
			}
			m_pTextureArray[uiIndex] = pTex;
		}
		FORCEINLINE unsigned int GetRegisterIndex()const
		{
			return m_uiRegisterIndex;
		}
		FORCEINLINE unsigned int GetRegisterNum()const
		{
			return m_uiRegisterNum;
		}
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
	protected:
		friend class SKRenderer;
		SKUserSampler();
	private:

		unsigned int m_uiTexType;
		SKArray<SKTexAllStatePtr> m_pTextureArray;
		SKUsedName		m_Name;
		unsigned int m_uiRegisterIndex;
		unsigned int m_uiRegisterNum;
	};
	SKTYPE_MARCO(SKUserSampler);
	DECLARE_Ptr(SKUserSampler);
}
#endif