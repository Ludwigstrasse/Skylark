FORCEINLINE SKVector3 *SKCurve3Subdivision::GetPoints()const
{
	return m_pPoint;
}
FORCEINLINE unsigned int SKCurve3Subdivision::GetPointNum()const
{

	return m_uiPointNum;
}
FORCEINLINE SKControlCurve3 *SKCurve3Subdivision::GetCurve()const
{

	return m_pCurve;
}
FORCEINLINE SKREAL *SKCurve3Subdivision::GetT()const
{
	return m_pT;

}