FORCEINLINE bool SKNURBSCurve3::ChangeWeight(unsigned int i,SKREAL Weight)
{
	if(!m_pWeight)
		return 0;
	else
	{
		if(i >= m_uiControlPointNum)
			return 0;
		else
			m_pWeight[i] = Weight;
		return 1;

	}

}
/*----------------------------------------------------------------*/
FORCEINLINE const SKREAL * SKNURBSCurve3::GetWeight()const
{
	return m_pWeight;

}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKNURBSCurve3::GetWeight(unsigned int i)const
{
	if(!m_pWeight)
		return SKMAX_REAL;
	else
	{
		if(i >= m_uiControlPointNum)
			return SKMAX_REAL;
		else
			return m_pWeight[i];

	}

}
/*----------------------------------------------------------------*/