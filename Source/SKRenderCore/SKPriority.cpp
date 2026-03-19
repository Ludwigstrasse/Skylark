#include "SKPriority.h"
using namespace SKEngine2;
bool SKPriority::AddPriorityThan(SKPriority * pPriority)
{
	if(!pPriority)
		return 0;
	if(pPriority->CheckIsCircle(this))
		return 0;
	m_uiPriorityNum = 0;
	
	m_pPriorityThanList.AddElement(pPriority);
	return 1;
}
unsigned int SKPriority::GetPriorityNum()
{
	CheckPriorityNum();
	return m_uiPriorityNum;
}
SKPriority::SKPriority()
{
	m_uiPriorityNum = 0;
}
SKPriority::SKPriority(unsigned int uiPriorityNum)
{
	m_uiPriorityNum = uiPriorityNum;
}
SKPriority::~SKPriority()
{
	m_pPriorityThanList.Clear();
}
SKPriority & SKPriority::operator= (SKPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	m_uiPriorityNum = rPriority.m_uiPriorityNum;
	m_pPriorityThanList.Clear();
	m_pPriorityThanList = rPriority.m_pPriorityThanList;
	return *this;
}
bool SKPriority::operator== (SKPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum == rPriority.m_uiPriorityNum);
}
bool SKPriority::operator!= (SKPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum != rPriority.m_uiPriorityNum);
}


bool SKPriority::operator >(SKPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum > rPriority.m_uiPriorityNum);
}
bool SKPriority::operator <(SKPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum < rPriority.m_uiPriorityNum);
}

bool SKPriority::operator >=(SKPriority& rPriority)
{	
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum >= rPriority.m_uiPriorityNum);
}
bool SKPriority::operator <=(SKPriority& rPriority)
{
	rPriority.CheckPriorityNum();
	CheckPriorityNum();
	return (m_uiPriorityNum <= rPriority.m_uiPriorityNum);

}
unsigned int SKPriority::CheckPriorityNum()
{
	if(m_uiPriorityNum || !m_pPriorityThanList.GetNum())
		return m_uiPriorityNum;
	else
	{
		unsigned int uiMax = 0;
		for(unsigned int i = 0; i < m_pPriorityThanList.GetNum() ; i++)
		{
			if(uiMax < m_pPriorityThanList[i]->CheckPriorityNum())
				uiMax = m_pPriorityThanList[i]->m_uiPriorityNum;
		
		}
		m_uiPriorityNum = uiMax + 1;
		return m_uiPriorityNum;
	}


}
bool SKPriority::CheckIsCircle(const SKPriority *pChecked)const
{
	if(!pChecked)
		return false;
	
	for(unsigned int i = 0 ; i < m_pPriorityThanList.GetNum() ; i++)
	{
		if(pChecked == m_pPriorityThanList[i])
			return true;
		else
			return m_pPriorityThanList[i]->CheckIsCircle(pChecked);
	
	}
	return false;


}