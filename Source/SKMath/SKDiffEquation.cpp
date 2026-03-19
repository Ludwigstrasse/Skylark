#include "SKDiffEquation.h"
using namespace SKEngine2;
SKDiffEquation::SKDiffEquation()
{
	m_pFunc = NULL;
	m_Step = 0.0;
	m_BeginT = 0.0;
	m_BeginX = 0.0;
}
/*----------------------------------------------------------------*/
SKDiffEquation::~SKDiffEquation()
{
	m_pFunc = NULL;
	m_Step = 0.0;
	m_BeginT = 0.0;
	m_BeginX = 0.0;
}
/*----------------------------------------------------------------*/
void SKDiffEquation::Update()
{
	if(!m_pFunc)
		return ;
	SKREAL HalfStep = m_Step / (SKREAL)2.0;
	SKREAL T = m_BeginT + HalfStep;
	
	SKREAL X = m_BeginX + m_pFunc->GetFirstDerivative(m_BeginT) * HalfStep;

	m_BeginX = X + m_pFunc->GetFirstDerivative(T) * HalfStep;

	m_BeginT +=m_Step;
}
/*----------------------------------------------------------------*/