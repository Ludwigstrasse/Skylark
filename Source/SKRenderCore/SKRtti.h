#ifndef SKRTTI_H
#define SKRTTI_H
#include"SKRtti.marc"
#include "SKString.h"
#include "SKGraphic.h"
#include "SKMemManager.h"
#include "SKArray.h"

namespace SKEngine2
{
class SKObject;
typedef SKObject *(*CreateObjectFun)();
class SKProperty;
class SKFunction;
class SKGRAPHIC_API SKRtti : public SKMemObject
{
public:
	SKRtti(const TCHAR * pcRttiName,SKRtti *pBase,CreateObjectFun COF);
	~SKRtti();


	FORCEINLINE const SKString &GetName()const;
	FORCEINLINE bool IsSameType(const SKRtti &Tpye) const;
	FORCEINLINE bool IsDerived(const SKRtti &Tpye) const;
	FORCEINLINE SKRtti* GetBase()const
	{
		return m_pBase;
	}
	SKProperty * GetProperty(unsigned int uiIndex)const;
	SKProperty *GetProperty(const SKString & PropertyName)const;
	unsigned int GetPropertyNum()const;
	void AddProperty(SKProperty * pProperty);
	void AddProperty(SKRtti & Rtti);
	friend class SKObject;
	typedef SKObject *(*CreateObjectFun)();
	void ClearProperty();

	SKFunction * GetFunction(unsigned int uiIndex)const;
	unsigned int GetFunctionNum()const;
	void AddFunction(SKFunction * pProperty);
	void AddFunction(SKRtti & Rtti);
	void ClearFunction();
private:
	SKString		m_cRttiName;
	SKRtti*	m_pBase;
	SKArray<SKProperty *> m_PropertyArray;
	SKArray<SKFunction *> m_FunctionArray;
	CreateObjectFun m_CreateFun;
};

#include"SKRtti.inl"
}
#endif