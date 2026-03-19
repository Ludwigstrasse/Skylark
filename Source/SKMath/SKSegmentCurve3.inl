FORCEINLINE void SKSegmentCurve3::GetFrenetFrameBNT(SKREAL t,SKVector3 & B,SKVector3 & N,SKVector3 &T)
{
	SKCurve3::GetFixedUpBNT(t,B,N,T);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3	SKSegmentCurve3::GetFirstDerivative(SKREAL t)
{

	return m_Segment.GetDir() * m_Segment.GetLen();
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3	SKSegmentCurve3::GetSecondDerivative(SKREAL t)
{
	return SKVector3(0,0,0);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3	SKSegmentCurve3::GetThirdDerivative(SKREAL t)
{

	return SKVector3(0,0,0);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL		SKSegmentCurve3::GetLength(SKREAL t1 ,SKREAL t2,unsigned int uiIterations)
{

	return m_Segment.GetLen() * (t2 - t1);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3	SKSegmentCurve3::GetPoint(SKREAL t)
{
	return m_Segment.GetParameterPoint(t * m_Segment.GetLen());
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL		SKSegmentCurve3::GetTotalLength(unsigned int uiIterations)
{
	return m_Segment.GetLen();

}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKSegmentCurve3::GetTime (SKREAL fLength, unsigned int uiIterations,
					   SKREAL fTolerance)
{
	return fLength / m_Segment.GetLen();

}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKSegmentCurve3::GetCurvature(SKREAL fTime)
{
	return 0;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKSegmentCurve3::GetTorsion(SKREAL fTime)
{
	return 0;
}
/*----------------------------------------------------------------*/