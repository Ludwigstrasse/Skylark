#include"SKRtti.h"
#include "SKProperty.h"
using namespace SKEngine2;
/********************************************************************************/
SKRtti::SKRtti(const TCHAR* pcRttiName,SKRtti *pBase,CreateObjectFun COF):m_cRttiName(pcRttiName)
{
	
	m_pBase = pBase;
	m_CreateFun = COF;
}//end construct
/********************************************************************************/
SKRtti::~SKRtti()
{

	m_pBase = 0;
	for(unsigned int i = 0 ; i < m_PropertyArray.GetNum() ;i++) 
	{ 
		if(m_PropertyArray[i]) 
		{ 
			SKMAC_DELETE(m_PropertyArray[i]) 
		} 
	} 
	for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
	{
		if (m_FunctionArray[i])
		{
			SKMAC_DELETE(m_FunctionArray[i])
		}
	}

}//end destruct
/********************************************************************************/
SKProperty * SKRtti::GetProperty(unsigned int uiIndex)const
{
	if (uiIndex >= m_PropertyArray.GetNum())
	{
		return NULL;
	}
	return m_PropertyArray[uiIndex];
}
SKProperty *SKRtti::GetProperty(const SKString & PropertyName)const
{
	for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
	{
		if (m_PropertyArray[i]->GetName() == PropertyName)
		{
			return m_PropertyArray[i];
		}
	}
	SKMAC_ASSERT(0);
	return NULL;
}
unsigned int SKRtti::GetPropertyNum()const
{
	return m_PropertyArray.GetNum();
}
void SKRtti::AddProperty(SKProperty * pProperty)
{	
	if (pProperty)
	{
		for (unsigned int i = 0 ; i < m_PropertyArray.GetNum() ; i++)
		{
			SKMAC_ASSERT(m_PropertyArray[i]->GetName() != pProperty->GetName());
		}
		m_PropertyArray.AddElement(pProperty);
	}

}
void SKRtti::AddProperty(SKRtti & Rtti)
{
	for (unsigned int i = 0 ; i < Rtti.m_PropertyArray.GetNum() ; i++)
	{
		SKProperty * pProperty = Rtti.m_PropertyArray[i];
		SKProperty *pNewProperty = pProperty->GetInstance();
		pNewProperty->Clone(pProperty);
		pNewProperty->SetOwner(*this);
		m_PropertyArray.AddElement(pNewProperty);
	}
}
void SKRtti::ClearProperty()
{
	for(unsigned int i = 0 ; i < m_PropertyArray.GetNum() ;i++) 
	{ 
		if(m_PropertyArray[i]) 
		{ 
			SKMAC_DELETE(m_PropertyArray[i]) 
		} 
	} 
	m_PropertyArray.Clear();	
	for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
	{
		if (m_FunctionArray[i])
		{
			SKMAC_DELETE(m_FunctionArray[i])
		}
	}
	m_FunctionArray.Clear();
}
SKFunction * SKRtti::GetFunction(unsigned int uiIndex)const
{
	if (uiIndex >= m_FunctionArray.GetNum())
	{
		return NULL;
	}
	return m_FunctionArray[uiIndex];
}
unsigned int SKRtti::GetFunctionNum()const
{
	return m_FunctionArray.GetNum();
}
void SKRtti::AddFunction(SKFunction * pFunction)
{
	if (pFunction)
	{
		for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
		{
			if (m_FunctionArray[i]->IsSame(pFunction))
			{
				SKMAC_DELETE(m_FunctionArray[i]);
				m_FunctionArray[i] = pFunction;
				return;
			}
		}
		m_FunctionArray.AddElement(pFunction);
	}

}
void SKRtti::AddFunction(SKRtti & Rtti)
{
	for (unsigned int i = 0; i < Rtti.m_FunctionArray.GetNum(); i++)
	{
		SKFunction * pFunction = Rtti.m_FunctionArray[i];
		SKFunction *pNewFunction = pFunction->GetInstance();
		pNewFunction->SetOwner(*this);
		pNewFunction->Clone(pFunction);
		m_FunctionArray.AddElement(pNewFunction);
	}
}
void SKRtti::ClearFunction()
{
	for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
	{
		if (m_FunctionArray[i])
		{
			SKMAC_DELETE(m_FunctionArray[i])
		}
	}
	m_FunctionArray.Clear();
}