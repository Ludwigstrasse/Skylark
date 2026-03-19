/*----------------------------------------------------------------*/
void SKLine3::Set(const SKVector3 & Orig,const SKVector3 &Dir)
{
	m_Orig = Orig;
	m_Dir = Dir;
	m_Dir.Normalize();
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKLine3::GetOrig()const 
{
	return m_Orig;
}
/*----------------------------------------------------------------*/
FORCEINLINE const SKVector3 & SKLine3::GetDir()const 
{
	return m_Dir;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKLine3::GetParameterPoint(SKREAL fLineParameter)const
{
	return  (m_Orig + m_Dir * fLineParameter); 
 
}
/*----------------------------------------------------------------*/
