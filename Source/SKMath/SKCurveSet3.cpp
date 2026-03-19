#include "SKCurveSet3.h"
#include "SKMemManager.h"
#include "SKMatrix3X3W.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKCurveSet3::SKCurveSet3()
{
	m_uiNum = 0;
	m_pHead = NULL;
	m_pNext = NULL;
}
/*----------------------------------------------------------------*/
SKCurveSet3::~SKCurveSet3()
{
	Clear();	

}
/*----------------------------------------------------------------*/
void SKCurveSet3::Clear()
{
	m_pNext = m_pHead;
	while(m_pNext)
	{
		ELEMENT * Temp = m_pNext;
		m_pNext = m_pNext->next;
		SKMAC_DELETE(Temp);
	}
	m_uiNum = 0;
}
/*----------------------------------------------------------------*/
bool SKCurveSet3::Set(SKCurve3 * pCurve)
{
	if(!pCurve)
		return 0;

	ELEMENT *Temp = NULL;
	Temp = SK_NEW ELEMENT;
	if(!Temp)
		return 0;

	Temp->Ptr = pCurve;
	Temp->next = NULL;

	if(!m_pHead)
	{
		m_pHead = Temp;
		m_pNext = m_pHead;

	
	}
	else
	{
		m_pNext->next = Temp;
		m_pNext = m_pNext->next;
	
	}
	m_uiNum++;
	return 1;
}
/*----------------------------------------------------------------*/
unsigned int SKCurveSet3::GetIndex(SKREAL &t)
{
	unsigned int i = 0;
	while(t > (SKREAL)1.0)
	{
		t = t - (SKREAL)1.0;
		i++;
	}
	return i;
}
/*----------------------------------------------------------------*/
bool SKCurveSet3::GetElement(unsigned int Index,SKCurve3 * &Element)
{
	if(Index > m_uiNum)
		return 0;
	else
	{
		m_pNext = m_pHead;
		unsigned int temp = 0;
		while(temp != Index)
		{
			m_pNext = m_pNext->next;
			temp++;
		}
	
	}
	Element = m_pNext->Ptr;
	return 1;
}
/*----------------------------------------------------------------*/
bool SKCurveSet3::GetElement(SKREAL& t,SKCurve3 * &Element)
{
	if(t > m_uiNum * 1.0)
		return 0;
	else
	{
		m_pNext = m_pHead;
	
		while(t > (SKREAL)1.0)
		{
			m_pNext = m_pNext->next;
			t = t - (SKREAL)1.0	;
		}

	}
	Element = m_pNext->Ptr;
	return 1;
}
/*----------------------------------------------------------------*/
bool	SKCurveSet3::GetFixedUpBNT(SKREAL t ,SKVector3 & B,SKVector3 & N,SKVector3 &T)
{
	SKCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return 0;
	else
	{
		if(!Element)
			return 0;
		else
		{
			
			Element->GetFixedUpBNT(t,B,N,T);
			
			
			return 1;
		}
	
	}

}
/*----------------------------------------------------------------*/
bool SKCurveSet3::GetFrenetFrameBNT(SKREAL t,SKVector3 & B,SKVector3 & N,SKVector3 &T)
{
	SKCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return 0;
	else
	{
		if(!Element)
			return 0;
		else
		{

			Element->GetFrenetFrameBNT(t,B,N,T);


			return 1;
		}

	}
}
/*----------------------------------------------------------------*/
SKVector3	SKCurveSet3::GetFirstDerivative(SKREAL t)
{
	SKCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	else
	{
		if(!Element)
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
		else
		{

			return Element->GetFirstDerivative(t);

		
		}

	}

}
/*----------------------------------------------------------------*/
SKVector3	SKCurveSet3::GetSecondDerivative(SKREAL t)
{
	SKCurve3 *Element = NULL;

	if(!GetElement(t,Element))
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	else
	{
		if(!Element)
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
		else
		{

			return Element->GetSecondDerivative(t);

			
			
		}

	}
}
/*----------------------------------------------------------------*/
SKVector3	SKCurveSet3::GetThirdDerivative(SKREAL t)
{
	SKCurve3 *Element = NULL;
	if(!GetElement(t,Element))
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	else
	{
		if(!Element)
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
		else
		{

			return Element->GetThirdDerivative(t);

			
		}

	}

}
/*----------------------------------------------------------------*/
SKREAL		SKCurveSet3::GetLength(SKREAL t1 ,SKREAL t2,unsigned int uiIterations)
{
	if (t1 > t2)
		return SKMAX_REAL;
	if(t2 > m_uiNum * 1.0)
		return SKMAX_REAL;

	unsigned int index1 = GetIndex(t1);
	unsigned int index2 = GetIndex(t2);

	t1 = t1 - index1 * (SKREAL)1.0;
	t2 = t2 - index2 * (SKREAL)1.0;
	if(index1 == index2)
	{
		SKCurve3 *Element = NULL;

		GetElement(index1,Element);
	
		return Element->GetLength(t1,t2,uiIterations);
		
		
	}
	else
	{
		SKCurve3 *Element = NULL;
		SKREAL Len = 0.0;
		GetElement(index1,Element);

		Len +=Element->GetLength(t1,1.0,uiIterations);

		index1++;
		while(index1 != index2)
		{
			
			GetElement(index1,Element);
		
			Len +=Element->GetTotalLength(uiIterations);
			
			index1++;
		}
		GetElement(index1,Element);
	
		Len +=Element->GetLength(0.0,t2,uiIterations);
	
		return Len;
	}


	
	
	
}
/*----------------------------------------------------------------*/
SKVector3	SKCurveSet3::GetPoint(SKREAL t)
{
	SKCurve3 *Element = NULL;
	unsigned int Type = 0;
	if(!GetElement(t,Element))
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	else
	{
		if(!Element)
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
		else
		{
			
			return Element->GetPoint(t);

			
		}

	}

}
/*----------------------------------------------------------------*/
SKREAL		SKCurveSet3::GetTotalLength(unsigned int uiIterations)
{
	SKREAL Len = 0.0;
	m_pNext = m_pHead;
	while(m_pNext)
	{
		

		Len += m_pNext->Ptr->GetTotalLength(uiIterations);

		
		m_pNext = m_pNext->next;
		
	}
	return Len;

}
/*----------------------------------------------------------------*/