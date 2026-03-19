#ifndef SKUSERCONSTANT_H
#define SKUSERCONSTANT_H
#include "SKMemManager.h"
#include "SKMath.h"
#include "SKObject.h"
#include "SKName.h"
namespace SKEngine2
{
class SKStream;
class SKGRAPHIC_API SKUserConstant : public SKObject
{
	//PRIORITY
	
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	
	
	enum //Value Tpye
	{			
		VT_FLOAT,
		VT_BOOL,
		VT_INT,	
		VT_STRUCT,
		VT_MAX
	};
public:
	SKUserConstant(const SKUsedName & Name,const void * pDate,unsigned int uiSize,
		unsigned int uiRegisterIndex,unsigned int uiRegisterNum,unsigned int uiValueType = VT_FLOAT);
	SKUserConstant(const SKUsedName & Name, const SKUsedName & NameInShader, const void * pDate, unsigned int uiSize,
		unsigned int uiRegisterIndex, unsigned int uiRegisterNum, unsigned int uiValueType = VT_FLOAT);
	virtual ~SKUserConstant();
	bool SetDate(void *pElementDate);
	FORCEINLINE void* GetDate()const;
	FORCEINLINE unsigned int GetValueType()const;
	FORCEINLINE unsigned int GetSize()const;
	FORCEINLINE unsigned int GetRegisterNum()const;
	FORCEINLINE unsigned int GetRegisterIndex()const;
	FORCEINLINE const SKUsedName & GetShowName()const
	{
		return m_Name;
	}
	FORCEINLINE const SKUsedName & GetNameInShader()const
	{
		return m_NameInShader;
	}
	unsigned int m_uiSize;
	unsigned int m_uiValueType;	
	unsigned int m_uiRegisterIndex;
	unsigned int m_uiRegisterNum;
protected:
	friend class SKRenderer;
	SKUserConstant();

private:
	
	unsigned char * m_pDate;
	SKUsedName m_Name;
	SKUsedName m_NameInShader;
};
#include "SKUserConstant.inl"
SKTYPE_MARCO(SKUserConstant);
DECLARE_Ptr(SKUserConstant);
}
#endif