/*----------------------------------------------------------------*/
FORCEINLINE bool SKNURBSSurface3::ChangeWeight(unsigned int U,unsigned int V,SKREAL Weight)
{
	if(U > m_uiRow || V > m_uiColumn)
		return 0;
	else
	{
		m_pWeight[U * m_uiColumn + V] = Weight;
		return 1;
	}
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKREAL * SKNURBSSurface3::GetWeight()const
{
	return m_pWeight;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKNURBSSurface3::GetWeight(unsigned int U,unsigned int V)const
{
	if(U > m_uiRow || V > m_uiColumn)
		return SKMAX_REAL;
	else
		return m_pWeight[U * m_uiColumn + V];

}
/*----------------------------------------------------------------*/