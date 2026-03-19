/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKPlane3::GetN()const 
{
	return m_N;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKPlane3::GetPoint()const 
{
	if (ABS(m_N.z) < EPSILON_E4)
	{
		return SKVector3(0.0f,0.0f,0.0f);
	}
	return SKVector3(0.0f,0.0f,- m_fD / m_N.z);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKPlane3::GetfD()const 
{
	return m_fD;
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKPlane3::Set(const SKVector3 &N, const SKVector3 &P) 
{
	m_N = N ;
	m_N.Normalize(); 
	m_fD = -( m_N.Dot(P));
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKPlane3::Set(const SKVector3 &P0,  const SKVector3 &P1, const SKVector3 &P2) 
{
	SKVector3 vcEdge1 = P1 - P0;
	SKVector3 vcEdge2 = P2 - P0;

	m_N.Cross(vcEdge1, vcEdge2);
	m_N.Normalize();
	m_fD = -(m_N.Dot(P0));
}
/*----------------------------------------------------------------*/
FORCEINLINE void  SKPlane3::Set(const SKVector3 Point[3])
{

	Set(Point[0],Point[1],Point[2]);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKPlane3 SKPlane3::GetPlane()const
{
	return *this;

}
/*----------------------------------------------------------------*/
FORCEINLINE void  SKPlane3::Set(const SKVector3 &N , SKREAL fD)
{
	SKREAL Len = N.GetLength();
	m_N = N / Len;

	m_fD = fD / Len;
}