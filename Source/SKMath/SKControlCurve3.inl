FORCEINLINE bool SKControlCurve3::ChangeControlPoint(unsigned int i,const SKVector3 & ControlPoint)
{
	if(!m_pControlPoint)
		return 0;
	else
	{
		if(i >= m_uiControlPointNum)
			return 0;
		else
			m_pControlPoint[i] = ControlPoint;
		return 1;

	}

}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 * SKControlCurve3::GetpControlPoint()const
{

	return m_pControlPoint;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKControlCurve3::GetControlPoint(unsigned int i)const
{
	if(!m_pControlPoint)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	else
	{
		if(i >= m_uiControlPointNum)
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
		else
			return m_pControlPoint[i];

	}
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKControlCurve3::GetControlPointNum()const
{

	return m_uiControlPointNum;
}
/*----------------------------------------------------------------*/
FORCEINLINE unsigned int SKControlCurve3::GetType()const
{
	return m_CurveType;
}