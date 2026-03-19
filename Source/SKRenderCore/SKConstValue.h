#ifndef SKCONSTVALUE_H
#define SKCONSTVALUE_H
#include "SKMath.h"
#include "SKShaderFunction.h"
#include "SKUserConstant.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKConstValue : public SKShaderFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:

		SKConstValue(const SKUsedName & ShowName,SKMaterial * pMaterial,unsigned int uiValueNum,bool bIsCustom);
		virtual ~SKConstValue();
		virtual unsigned int GetType() = 0;
		virtual bool GetOutPutValueString(SKString &OutString)const = 0;
		virtual bool GetFuntionString(SKString &OutString)const = 0;
		FORCEINLINE bool IsCustom()const
		{
			return m_bIsCustom;
		}
		virtual bool GetDeclareString(SKString &OutString,unsigned int uiRegisterID)const = 0;

		virtual unsigned int GetValueNum()const = 0;

		virtual unsigned int GetSize()const  = 0;
	protected:
		SKConstValue();
		bool m_bIsCustom;
		
	};
	DECLARE_Ptr(SKConstValue);
	SKTYPE_MARCO(SKConstValue);
	
	

}
#endif