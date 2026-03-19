/*----------------------------------------------------------------*/
FORCEINLINE void SKOBB3::Set(const SKVector3 A[3],SKREAL fA[3],const SKVector3 & Center)
{
	m_A[0] = A[0];
	m_A[1] = A[1];
	m_A[2] = A[2];
	m_A[0].Normalize();
	m_A[1].Normalize();
	m_A[2].Normalize();
	m_fA[0] = ABS(fA[0]);
	m_fA[1] = ABS(fA[1]);
	m_fA[2] = ABS(fA[2]);
	m_Center = Center;

}
/*----------------------------------------------------------------*/
FORCEINLINE void SKOBB3::Set(const SKVector3 &A0,const SKVector3 &A1,const SKVector3 &A2,
						SKREAL fA0,SKREAL fA1,SKREAL fA2,
						const SKVector3 & Center)
{

	m_A[0] = A0;
	m_A[1] = A1;
	m_A[2] = A2;

	m_A[0].Normalize();
	m_A[1].Normalize();
	m_A[2].Normalize();

	m_fA[0] = ABS(fA0);
	m_fA[1] = ABS(fA1);
	m_fA[2] = ABS(fA2);
	m_Center = Center;
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKOBB3::GetA(SKVector3 A[3])const
{
	for(int i =0 ;i< 3 ; i++)
	{
		A[i] = m_A[i];
	}
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKOBB3::GetfA(SKREAL fA[3])const
{
	for(int i =0 ;i< 3 ; i++)
	{
		fA[i] = m_fA[i];
	}
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKOBB3::GetCenter()const
{

	return m_Center;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKOBB3::GetParameterPoint(SKREAL fOBBParameter[3])const
{
	return (m_Center + m_A[0] * fOBBParameter[0] + m_A[1] * fOBBParameter[1] + m_A[2] * fOBBParameter[2]);

}
FORCEINLINE SKVector3 SKOBB3::GetParameterPoint(SKREAL fOBBParameter0,SKREAL fOBBParameter1,SKREAL fOBBParameter2)const
{

	return (m_Center + m_A[0] * fOBBParameter0 + m_A[1] * fOBBParameter1 + m_A[2] * fOBBParameter2);

}