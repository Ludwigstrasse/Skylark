/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKSurface3::GetN(SKREAL U,SKREAL V)
{
	SKVector3 N;
	N.Cross(GetUPartialDerivative(U,V),GetVPartialDerivative(U,V));
	return N;

}
/*----------------------------------------------------------------*/