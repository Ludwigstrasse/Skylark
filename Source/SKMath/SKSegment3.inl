FORCEINLINE const SKVector3 & SKSegment3::GetEnd()const 
{
	return m_End;
}
/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKSegment3::GetLen()const 
{
	return m_fLen;
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKSegment3::Set(const SKVector3 &Orig,const SKVector3 &End)
{
	SKVector3 Dir = End - Orig;
	m_fLen = Dir.GetLength();

	SKLine3::Set(Orig,Dir);

}
/*----------------------------------------------------------------*/
FORCEINLINE void SKSegment3::Set(const SKVector3 &Orig,const SKVector3 &Dir,SKREAL fLen)
{


	SKLine3::Set(Orig,Dir);
	m_fLen = fLen;
	m_End = Orig + (m_Dir * m_fLen);

}
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKSegment3::GetParameterPoint(SKREAL fSegmentParameter)const
{
	if(fSegmentParameter > m_fLen)
		fSegmentParameter = m_fLen;

	return SKRay3::GetParameterPoint(fSegmentParameter);
}
/*----------------------------------------------------------------*/