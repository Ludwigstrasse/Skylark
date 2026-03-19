FORCEINLINE SKVector3 SKControlSurface3::GetControlPoint(unsigned int U,unsigned int V)const
{
	if(U > m_uiRow || V > m_uiColumn)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	else
		return m_pControlPoint[U * m_uiColumn + V];

}
/*----------------------------------------------------------------*/
FORCEINLINE bool SKControlSurface3::ChangeControlPoint(unsigned int U,unsigned int V,const SKVector3 &Value)
{
	if(U > m_uiRow || V > m_uiColumn)
		return 0;
	else
	{
		m_pControlPoint[U * m_uiColumn + V] = Value;
		return 1;
	}


}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 *SKControlSurface3::GetControlPoint()const
{
	return m_pControlPoint;

}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKControlSurface3::GetControlPointNum()const
{
	return m_uiControlPointNum;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKControlSurface3::GetRow()const
{
	return m_uiRow;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKControlSurface3::GetColumn()const
{
	return m_uiColumn;
}
/*----------------------------------------------------------------*/