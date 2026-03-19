FORCEINLINE void SKAABB3::Set(const SKVector3 & Max , const SKVector3 & Min)
{
	m_Max = Max;
	m_Min = Min;
	m_Center = (Max + Min) / 2.0f;
	SKVector3 Temp = (Max - Min) / 2.0f;
	for(int i = 0 ; i < 3; i++)
		m_fA[i] = Temp.m[i];

}
/*----------------------------------------------------------------*/
FORCEINLINE void SKAABB3::Set(const SKVector3 & Center,SKREAL fA0,SKREAL fA1,SKREAL fA2)
{
	m_fA[0] = ABS(fA0);
	m_fA[1] = ABS(fA1);
	m_fA[2] = ABS(fA2);
	
	m_Center = Center;
	
	m_Max.Set(m_Center.x + m_fA[0],m_Center.y + m_fA[1],m_Center.z + m_fA[2]);
	m_Min.Set(m_Center.x - m_fA[0],m_Center.y - m_fA[1],m_Center.z - m_fA[2]);
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKAABB3::Set(const SKVector3 & Center,SKREAL fA[3])
{
	m_fA[0] = ABS(fA[0]);
	m_fA[1] = ABS(fA[1]);
	m_fA[2] = ABS(fA[2]);

	m_Center = Center;

	m_Max.Set(m_Center.x + m_fA[0],m_Center.y + m_fA[1],m_Center.z + m_fA[2]);
	m_Min.Set(m_Center.x - m_fA[0],m_Center.y - m_fA[1],m_Center.z - m_fA[2]);

}
/*----------------------------------------------------------------*/
FORCEINLINE void SKAABB3::GetfA(SKREAL fA[3])const
{
	for(int i =0 ;i< 3 ; i++)
	{
		fA[i] = m_fA[i];
	}

}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKAABB3::GetCenter()const
{
	
	return m_Center;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKAABB3::GetParameterPoint(SKREAL fAABBParameter[3])const
{

	return SKVector3(m_Center.x + fAABBParameter[0],m_Center.y + fAABBParameter[1] ,m_Center.z + fAABBParameter[2]);
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKAABB3::GetParameterPoint(SKREAL fAABBParameter0,SKREAL fAABBParameter1,SKREAL fAABBParameter2)const
{

	return SKVector3(m_Center.x + fAABBParameter0,m_Center.y + fAABBParameter1 ,m_Center.z + fAABBParameter2);
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKAABB3::GetMaxPoint()const
{
	return m_Max;
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKAABB3::GetMinPoint()const
{	
	return m_Min;
}