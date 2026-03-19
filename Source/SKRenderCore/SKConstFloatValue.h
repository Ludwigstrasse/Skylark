#ifndef SKCONSTFLOATVALUE_H
#define SKCONSTFLOATVALUE_H
#include "SKConstValue.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKConstFloatValue : public SKConstValue
	{
		//RTTI
		DECLARE_RTTI;
		//PRIORITY
		DECLARE_INITIAL
	public:
		SKConstFloatValue(const SKUsedName & ShowName,SKMaterial * pMaterial,unsigned int uiValueNum,bool bIsCustom);
		virtual ~SKConstFloatValue();
		virtual unsigned int GetType()
		{
			return SKUserConstant::VT_FLOAT;
		}
		void SetValue(unsigned int uiIndex,SKREAL Value);

		virtual bool GetOutPutValueString(SKString &OutString)const;
		virtual bool GetFuntionString(SKString &OutString)const;
		virtual unsigned int GetValueNum()const
		{
			return m_Value.GetNum();
		}
		virtual unsigned int GetSize()const
		{
			return m_Value.GetNum() * sizeof(SKREAL);
		}
		bool GetDeclareString(SKString &OutString,unsigned int uiRegisterID)const;
		virtual void ResetInShaderName();
		SKArray<SKREAL>& GetValue() { return m_Value;}
	protected:
		SKConstFloatValue();
		SKArray<SKREAL> m_Value;

	public:
		enum
		{
			OUT_VALUE,
			OUT_VALUE_X,
			OUT_VALUE_Y,
			OUT_VALUE_Z,
			OUT_VALUE_W,
			OUT_MAX
		};
	};
	DECLARE_Ptr(SKConstFloatValue);
	SKTYPE_MARCO(SKConstFloatValue);

}
#endif