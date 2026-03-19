FORCEINLINE SKVector3 SKRay3::GetParameterPoint(SKREAL fRayParameter)const
{
	if(fRayParameter < 0)
		fRayParameter = 0;
	return SKLine3::GetParameterPoint(fRayParameter);
}
/*----------------------------------------------------------------*/