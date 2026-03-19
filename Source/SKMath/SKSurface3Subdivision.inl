FORCEINLINE bool SKSurface3Subdivision::SetSurface(SKSurface3 * pSurface)
{

	if(!pSurface)
		return 0;
	m_pSurface = pSurface;
	return 1;
}
FORCEINLINE SKVector3 * SKSurface3Subdivision::GetPoints()const
{
	return m_pPoint;
}
FORCEINLINE unsigned int SKSurface3Subdivision::GetRow()const
{
	return m_uiRow;
}
FORCEINLINE unsigned int SKSurface3Subdivision::GetColumn()const
{

	return m_uiColumn;
}
FORCEINLINE SKVector2 * SKSurface3Subdivision::GetUV()const
{

	return m_pUV;
}