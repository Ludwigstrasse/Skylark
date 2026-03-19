/*----------------------------------------------------------------*/
FORCEINLINE void SKRectangle3::Set(const SKVector3 & Center,const SKVector3 &A0,const SKVector3 & A1,SKREAL fA0, SKREAL fA1)
{
	m_Center = Center;
	m_A[0] = A0;
	m_A[1] = A1;
	m_A[0].Normalize();
	m_A[1].Normalize();
	m_fA[0] = ABS(fA0);
	m_fA[1] = ABS(fA1);
	SKVector3 N;
	N.Cross(A0,A1);
	SKPlane3::Set(N,Center);

}
/*----------------------------------------------------------------*/
FORCEINLINE void SKRectangle3::Set(const SKVector3 & Center,const SKVector3 A[2],const SKREAL fA[2])
{
	Set(Center,A[0],A[1],fA[0],fA[1]);

}
/*----------------------------------------------------------------*/
FORCEINLINE void SKRectangle3::GetA(SKVector3 A[2])const
{
	A[0] = m_A[0];
	A[1] = m_A[1];
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKRectangle3::GetfA(SKREAL fA[2])const
{
	fA[0] = m_fA[0];
	fA[1] = m_fA[1];

}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKRectangle3::GetCenter()const
{
	return m_Center;

}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKRectangle3::GetParameterPoint(SKREAL fRectangleParameter[2])const
{
	return (m_Center + m_A[0] * fRectangleParameter[0] + m_A[1] * fRectangleParameter[1]);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKRectangle3::GetParameterPoint(SKREAL fRectangleParameter0,SKREAL fRectangleParameter1)const
{
	return (m_Center + m_A[0] * fRectangleParameter0 + m_A[1] * fRectangleParameter1);
}
