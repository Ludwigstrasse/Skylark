
/*----------------------------------------------------------------*/
FORCEINLINE void SKVector3W::Set(SKREAL _x, SKREAL _y, SKREAL _z, SKREAL _w) 
{
	x=_x; y=_y; z=_z; w=_w;
}
SKVector3 SKVector3W::GetV3()const
{
	return SKVector3(x,y,z);
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKVector3W::GetV3(SKVector3 & V)const
{

	V.x = x;
	V.y = y;
	V.z = z;
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKVector3W::Set(const SKVector3 & V)
{
	x = V.x; 
	y = V.y; 
	z = V.z; 
	w = 1.0f;
}
