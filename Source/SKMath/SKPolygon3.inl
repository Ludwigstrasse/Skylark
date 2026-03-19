FORCEINLINE void SKPolygon3::Set(const SKVector3 *pPoint,unsigned int PointNum)
{
	if(!pPoint || !PointNum)
		return ;

	m_PointNum = PointNum;
	m_IndexNum = 3 * (m_PointNum - 2);

	SKMAC_DELETEA(m_pPoint);
	SKMAC_DELETEA(m_pIndex);
	m_pPoint = SK_NEW SKVector3[m_PointNum];
	m_pIndex = SK_NEW SKUSHORT_INDEX[m_IndexNum];
	SKMemcpy(m_pPoint,pPoint,sizeof(SKVector3) * PointNum);

	m_Flag = 1;

	unsigned int I0, I1, I2;
	for (unsigned int nLoop = 0 ; nLoop < (m_PointNum - 2); nLoop++) 
	{
		if (nLoop == 0) { I0=0; I1=1; I2=2; }
		else { I1=I2; I2++; }

		m_pIndex[(nLoop*3)   ] = I0;
		m_pIndex[(nLoop*3) +1] = I1;
		m_pIndex[(nLoop*3) +2] = I2;
	}
	
	
	SKVector3 vcEdge0 = m_pPoint[1] - m_pPoint[0];
	SKVector3 vcEdge1;
	//ƽ,ֱҵƽе߶Ϊֹ
	for (unsigned int i = 2; i < PointNum ; i++) 
	{
		vcEdge1 = m_pPoint[i] - m_pPoint[0];

		vcEdge0.Normalize();
		vcEdge1.Normalize();

		//Ƿƽ
		SKREAL dot = vcEdge0.Dot(vcEdge1);
		if(ABS(dot) < SKREAL_1)      
			break;
	} // for

	SKVector3 N;
	N.Cross(vcEdge0, vcEdge1);
	SKPlane3::Set(N,m_pPoint[0]);
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKPolygon3::GetPointNum()const
{
	return m_PointNum;
	
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKPolygon3::GetIndexNum()const
{
	return m_IndexNum;
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 * const SKPolygon3::GetpPoint()const
{

	return m_pPoint;
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKUSHORT_INDEX * const SKPolygon3::GetpIndex()const
{
	return m_pIndex;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKPolygon3::GetFlag()const
{
	return m_Flag;
}
/*----------------------------------------------------------------*/