#include "SKFunction1.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKFunction1::SKFunction1()
{
	m_pNext = NULL;
	m_pHead = NULL;
	m_pRoot = NULL;
	m_uiRootNum = 0;
}
/*----------------------------------------------------------------*/
SKFunction1::~SKFunction1()
{
	if(m_pHead)
	{
	
		m_pNext = m_pHead;
		while(m_pNext)
		{
			INTERVAL *Temp = m_pNext;
			m_pNext = m_pNext->next;
			SKMAC_DELETE(Temp);
		
		}
		m_pNext = NULL;
		m_pHead = NULL;	
	}
	SKMAC_DELETEA(m_pRoot);
}
/*----------------------------------------------------------------*/
bool SKFunction1::ISHaveRoot(SKREAL t1 , SKREAL t2,unsigned int uiIterator)
{
	if(m_pHead)
	{

		m_pNext = m_pHead;
		while(m_pNext)
		{
			INTERVAL *Temp = m_pNext;
			m_pNext = m_pNext->next;
			SKMAC_DELETE(Temp);

		}
		m_pNext = NULL;
		m_pHead = NULL;	
	}
	m_uiRootNum = 0;

	SKREAL Temp = ABS(t2 - t1) / uiIterator;
	SKREAL k1 = t1;
	SKREAL k2;
	SKREAL Value1 ,Value2;
	Value1 = Value2 = GetValue(k1);
	for(unsigned int i = 0 ; i < uiIterator ; i++)
	{
		Value1 = Value2;
		k2 = k1 + Temp;
		Value2 = GetValue(k2);
		if(Value1 * Value2 < 0)
		{
			m_uiRootNum++;
			if(m_pHead == NULL)
			{
				m_pHead = SK_NEW INTERVAL;
				if(!m_pHead)
					return 0;
				m_pNext = m_pHead;
				m_pNext->t1 = k1;
				m_pNext->t2 = k2;
				m_pNext->value1 = Value1;
				m_pNext->value2 = Value2;
				m_pNext->next = NULL;
				
			}
			else
			{
				m_pNext->next = SK_NEW INTERVAL;
				if(!m_pNext->next)
					return 0;
				m_pNext = m_pNext->next;
				m_pNext->t1 = k1;
				m_pNext->t2 = k2;
				m_pNext->value1 = Value1;
				m_pNext->value2 = Value2;
				m_pNext->next = NULL;
				
			}
		}
		k1 = k2;
	
	}
	if(m_pHead)
	{
		SKMAC_DELETEA(m_pRoot);
		m_pRoot = SK_NEW SKREAL[m_uiRootNum];
		if(!m_pRoot)
			return 0;
		return 1;
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/
void SKFunction1::Root(SKREAL t1 ,SKREAL t2,SKREAL Value1,SKREAL Value2,unsigned int uiLevel)
{
	
	SKREAL tm = (t2 + t1) / (SKREAL)2.0;

	SKREAL Valuem = GetValue(tm);
	if(uiLevel == 0)
	{
		m_pRoot[m_uiIndex] = tm;
		m_uiIndex++;
		return ;
	}
	if(Value1 * Valuem < 0.0)
	{
		Root(t1 ,tm,Value1,Valuem,uiLevel - 1);
	}
	else
	{
		Root(tm ,t2,Valuem,Value2,uiLevel - 1);
	}

}
/*----------------------------------------------------------------*/
bool SKFunction1::ComputeRoot(SKREAL t1 , SKREAL t2,unsigned int uiLevel)
{
	if(!m_pHead)
		return 0;
	m_uiIndex = 0;

	m_pNext = m_pHead;
	while(m_pNext)
	{
		Root(m_pNext->t1 ,m_pNext->t2,m_pNext->value1,m_pNext->value2,uiLevel);
		m_pNext = m_pNext->next;
		

	}
	return 1;

}
/*----------------------------------------------------------------*/
void SKFunction1::GetExtremumValue(SKREAL t1 , SKREAL t2,SKREAL & Min,SKREAL &Max,unsigned int uiLevel)
{
	SKREAL value1 = GetValue(t1);
	SKREAL value2 = GetValue(t2);
	ExtremumValue(t1 ,t2,value1,value2,Min,Max,uiLevel);

}
/*----------------------------------------------------------------*/
void SKFunction1::ExtremumValue(SKREAL t1 , SKREAL t2,SKREAL Value1,SKREAL Value2,
								SKREAL & Min,SKREAL &Max,unsigned int uiLevel)
{

	SKREAL tm = (t1 + t2) / (SKREAL)2.0;
	SKREAL valuem = GetValue(tm);
	
	
	if(uiLevel == 0)
	{
		SKREAL min,max;
		min = Value1;
		max = Value1;
		if(min > valuem)
		{
			min = valuem;
		}
		if(min > Value2)
		{
			min = Value2;
		}
		if(max < valuem)
		{
			max = valuem;
		}
		if(max < Value2)
		{
			max = Value2;
		}
		Min = min;
		Max = max;
	}
	else
	{
		SKREAL min1,max1;
		SKREAL min2,max2;
		ExtremumValue(t1,tm,Value1,valuem,min1,max1,uiLevel - 1);
		ExtremumValue(tm,t2,valuem,Value2,min2,max2,uiLevel - 1);

		if(min1 < min2)
		{
			Min = min1;
		}
		else
		{
			Min = min2;
		}
		if(max1 > max2)
		{
		
			Max = max1;		
		}
		else
		{
		
			Max = max2;
		}
	}

}
/*----------------------------------------------------------------*/
SKREAL SKFunction1::Integration(SKREAL t1 , SKREAL t2,unsigned int uiIterator)
{
	SKREAL tTemp = (t2 - t1) / uiIterator;
	SKREAL result = 0.0;
	SKREAL t1Temp = t1;
	SKREAL t2Temp = t1 + t1Temp;
	SKREAL t1TempValue = GetValue(t1Temp);
	SKREAL t2TempValue = t1TempValue;
	for(unsigned int i = 0 ; i < uiIterator ; i++)
	{
		t2TempValue = GetValue(t2Temp);
		result += (SKREAL)0.5 * tTemp * (t1TempValue + t2TempValue);
		t1Temp = t2Temp;
		t2Temp += tTemp;
		t1TempValue = t2TempValue;
	
	}
	return result;
}
/*----------------------------------------------------------------*/