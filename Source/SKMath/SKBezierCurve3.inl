FORCEINLINE SKREAL SKBezierCurve3::GetC(int i,int j)const
{
	return m_C[m_uiControlPointNum * i + j];
}
FORCEINLINE SKREAL SKBezierCurve3::SetC(int i,int j,SKREAL Value)
{
	return m_C[m_uiControlPointNum * i + j] = Value;
}