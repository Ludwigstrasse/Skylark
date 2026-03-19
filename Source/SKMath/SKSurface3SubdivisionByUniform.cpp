#include "SKSurface3SubdivisionByUniform.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKSurface3SubdivisionByUniform::SKSurface3SubdivisionByUniform(unsigned int uiRow,unsigned int uiColumn,SKREAL U1,SKREAL U2,SKREAL V1 ,SKREAL V2)
{

}
/*----------------------------------------------------------------*/
SKSurface3SubdivisionByUniform::~SKSurface3SubdivisionByUniform()
{

}
/*----------------------------------------------------------------*/
bool SKSurface3SubdivisionByUniform::Subdivision()
{
	if(!m_uiRow || !m_uiColumn || !m_pSurface)
		return 0;
	if(m_fU2 > m_fU1 || m_fV2 > m_fV1)
		return 0;
	SKMAC_DELETEA(m_pPoint);

	m_pPoint = SK_NEW SKVector3[m_uiRow * m_uiColumn];
	if(!m_pPoint)
		return 0;

	SKMAC_DELETEA(m_pUV);
	m_pUV = SK_NEW SKVector2[m_uiRow * m_uiColumn];
	if(!m_pUV)
		return 0;
	SKREAL fi = (SKREAL)(m_fU2 - m_fU1)/ (m_uiRow - 1);
	SKREAL fj = (SKREAL)(m_fV2 - m_fV1) / (m_uiColumn - 1);
	SKREAL fiTemp = m_fU1;
	for(unsigned int i = 0 ; i < m_uiRow ; i++)
	{
		SKREAL fjTemp = m_fV1;
		unsigned int Temp = i * m_uiColumn;
		for(unsigned int j = 0 ; j < m_uiColumn ; j++)
		{
			m_pUV[Temp + j] = SKVector2(fiTemp,fjTemp);
			m_pPoint[ Temp + j] = m_pSurface->GetPoint(fiTemp,fjTemp);
			fjTemp += fj;
		}
		fiTemp += fi;
	}
	return 1;
}
/*----------------------------------------------------------------*/