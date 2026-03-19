#include "SKSurface3SubdivisionByLength.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKSurface3SubdivisionByLength::SKSurface3SubdivisionByLength(unsigned int Row,unsigned int Column,SKREAL U1,SKREAL U2,SKREAL V1 ,SKREAL V2,unsigned int uiIterations)
{
	m_uiRow = Row;
	m_uiColumn = Column;
	m_fU1 = U1;
	m_fU2 = U2;
	m_fV1 = V1;
	m_fV2 = V2;
	m_uiIterations = uiIterations;

}
/*----------------------------------------------------------------*/
SKSurface3SubdivisionByLength::~SKSurface3SubdivisionByLength()
{



}
/*----------------------------------------------------------------*/
bool SKSurface3SubdivisionByLength::Subdivision()
{
	if(!m_uiRow || !m_uiColumn || !m_pSurface)
		return 0;
	if(m_fU2 > m_fU1 || m_fV2 > m_fV1)
		return 0;
	return 1;
// 	SKMAC_DELETEA(m_pPoint);
// 
// 	m_pPoint = SK_NEW SKVector3[m_uiRow * m_uiColumn];
// 	if(!m_pPoint)
// 		return 0;
// 
// 	SKMAC_DELETEA(m_pUV);
// 	m_pUV = SK_NEW SKVector2[m_uiRow * m_uiColumn];
// 	if(!m_pUV)
// 		return 0;
// 	SKREAL fi = (SKREAL)GetULength()/ (m_uiRow - 1);
// 	SKREAL fj = (SKREAL)(m_fV2 - m_fV1) / (m_uiColumn - 1);
// 	SKREAL fiTemp = m_fU1;
// 	for(unsigned int i = 0 ; i < m_uiRow ; i++)
// 	{ 
// 		SKREAL fjTemp = m_fV1;
// 		unsigned int Temp = i * m_uiColumn;
// 		for(unsigned int j = 0 ; j < m_uiColumn ; j++)
// 		{
// 			m_pUV[Temp + j] = SKVector2(fiTemp,fjTemp);
// 			m_pPoint[ Temp + j] = m_pSurface->GetPoint(fiTemp,fjTemp);
// 			fjTemp += fj;
// 		}
// 		fiTemp += fi;
// 	}
// 	return 1;
// 
// 
// 
// 	SKMAC_DELETEA(m_pT);
// 	m_pT = SK_NEW SKREAL[m_uiPointNum]; 
// 
// 	if(!m_pT)
// 		return 0;
// 
// 	SKREAL det = m_pCurve->GetLength(m_ft1,m_ft2,m_uiIterations)/SKREAL(m_uiPointNum - 1);
// 	SKREAL temp = m_ft1;
// 	for(unsigned int i = 0 ; i < m_uiPointNum ; i++)
// 	{
// 		m_pT[i] = m_pCurve->GetTime(temp,m_uiIterations);
// 		m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
// 		temp += det;
// 
// 	}
// 	return 1;

}