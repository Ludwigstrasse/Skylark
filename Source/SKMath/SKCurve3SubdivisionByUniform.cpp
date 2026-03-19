#include "SKCurve3SubdivisionByUniform.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKCurve3SubdivisionByUniform::SKCurve3SubdivisionByUniform(unsigned int PointNum,SKREAL t1,SKREAL t2)
{
	m_uiPointNum = PointNum;
	m_ft1 = t1;
	m_ft2 = t2;
}
/*----------------------------------------------------------------*/
SKCurve3SubdivisionByUniform::~SKCurve3SubdivisionByUniform()
{

}
/*----------------------------------------------------------------*/
bool SKCurve3SubdivisionByUniform::Subdivision()
{
	if(!m_uiPointNum || !m_pCurve)
		return 0;
	if(m_ft1 > m_ft2)
		return 0;
	SKMAC_DELETEA(m_pPoint);
	m_pPoint = SK_NEW SKVector3[m_uiPointNum];

	if(!m_pPoint)
		return 0;
	SKMAC_DELETEA(m_pT);
	m_pT = SK_NEW SKREAL[m_uiPointNum]; 

	if(!m_pT)
		return 0;

	SKREAL det = (m_ft2 - m_ft1) / SKREAL(m_uiPointNum - 1);
	SKREAL temp =m_ft1;
	for(unsigned int i = 0 ; i < m_uiPointNum ; i++)
	{
		m_pT[i] = temp;
		m_pPoint[i] = m_pCurve->GetPoint(temp);
		temp += det;
	
	}
	return 1;
}
/*----------------------------------------------------------------*/
void SKCurve3SubdivisionByUniform::SetPointNum(unsigned int uiPointNum)
{
	SKMAC_ASSERT(uiPointNum);
	m_uiPointNum = uiPointNum;
}