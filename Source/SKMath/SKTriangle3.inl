FORCEINLINE void SKTriangle3::Set(const SKVector3 & P0,const SKVector3 &P1,const SKVector3 &P2)
{
	m_V[0] = P0;
	m_V[2] = P2;
	m_V[1] = P1;
	SKPlane3::Set(m_V);
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKTriangle3::Set(const SKVector3 Point[3])
{
	m_V[0] = Point[0];
	m_V[2] = Point[2];
	m_V[1] = Point[1];
	SKPlane3::Set(m_V);
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKTriangle3::GetPoint(SKVector3  Point[3])const
{
	Point[0] = m_V[0];
	Point[1] = m_V[1];
	Point[2] = m_V[2];


}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKTriangle3::GetParameterPoint(SKREAL fTriangleParameter[3])const
{
	return (m_V[0] * fTriangleParameter[0] + m_V[1] * fTriangleParameter[1] + m_V[2] * fTriangleParameter[2]);

}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKTriangle3::GetParameterPoint(SKREAL fTriangleParameter0,
								  SKREAL fTriangleParameter1,SKREAL fTriangleParameter2)const
{

	return (m_V[0] * fTriangleParameter0 + m_V[1] * fTriangleParameter1 + m_V[2] * fTriangleParameter2);
}
/*----------------------------------------------------------------*/