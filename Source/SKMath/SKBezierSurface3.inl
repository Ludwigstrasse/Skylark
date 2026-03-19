FORCEINLINE SKREAL SKBezierSurface3::GetUC(int i,int j)const
{
	return m_UC[i * m_uiRow + j];
	
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKBezierSurface3::SetUC(int i,int j,SKREAL Value)
{
	return m_UC[i * m_uiRow + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKBezierSurface3::GetVC(int i,int j)const
{
	return m_VC[i * m_uiColumn + j];

}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKBezierSurface3::SetVC(int i,int j,SKREAL Value)
{
	return m_VC[i * m_uiColumn + j] = Value;
}
FORCEINLINE void SKBezierSurface3::SetUPD(int i, int j,const SKVector3 & Value)
{
	m_UPDValue[i * m_uiColumn + j] = Value;

}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKBezierSurface3::GetUPD(int i, int j)
{

	return m_UPDValue[i * m_uiColumn + j];
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKBezierSurface3::SetVPD(int i, int j,const SKVector3 & Value)
{

	m_VPDValue[i * (m_uiColumn- 1) + j] = Value;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKBezierSurface3::GetVPD(int i, int j)
{

	return m_VPDValue[i * (m_uiColumn- 1) + j];
}
/*----------------------------------------------------------------*/