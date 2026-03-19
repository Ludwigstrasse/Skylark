#include "SKPolygon3.h"
using namespace SKEngine2;
SKPolygon3::SKPolygon3()
{
	m_PointNum = 0;		//
	m_IndexNum = 0;		//
	m_Flag = 0;			// ־λ
	m_pPoint = NULL;		// б
	m_pIndex = NULL;		// б
}
/*----------------------------------------------------------------*/
SKPolygon3::~SKPolygon3()
{
	SKMAC_DELETEA(m_pPoint);
	SKMAC_DELETEA(m_pIndex);
}
/*----------------------------------------------------------------*/
SKPolygon3::SKPolygon3(const SKVector3 *pPoint,unsigned int PointNum)
{
	Set(pPoint,PointNum);
}
/*----------------------------------------------------------------*/
bool SKPolygon3::CopyFrom(const SKPolygon3 & Polygon)
{
	if(!Polygon.m_pPoint || !Polygon.m_PointNum || !Polygon.m_pIndex || !Polygon.m_IndexNum)
		return 0;
	m_PointNum = Polygon.m_PointNum;	
	m_IndexNum = Polygon.m_IndexNum;		
	m_Flag = Polygon.m_Flag;
	SKMAC_DELETEA(m_pPoint);
	SKMAC_DELETEA(m_pIndex);
	m_pPoint = SK_NEW SKVector3[m_PointNum];
	if(!m_pPoint)
		return 0;
	m_pIndex = SK_NEW SKUSHORT_INDEX[m_IndexNum];
	if(!m_pIndex)
		return 0;
	SKMemcpy(m_pPoint,Polygon.m_pPoint,sizeof(SKVector3) * m_PointNum);
	SKMemcpy(m_pIndex,Polygon.m_pIndex,sizeof(SKUSHORT_INDEX) * m_IndexNum);
	return 1;

}
/*----------------------------------------------------------------*/
void SKPolygon3::SwapFaces()
{

	SKUSHORT_INDEX *pIndis = new SKUSHORT_INDEX[m_IndexNum];


	for (unsigned int i = 0; i < m_IndexNum ; i++)
		pIndis[m_IndexNum - i - 1] = m_pIndex[i];

	m_Flag = !m_Flag;

	SKPlane3::Set(m_N * (-1.0f),m_fD);

	SKMAC_DELETEA(m_pIndex);
	m_pIndex = pIndis;
}