/*----------------------------------------------------------------*/
FORCEINLINE void SKVector2::Set(SKREAL _x, SKREAL _y) 
{
	x=_x; y=_y; 
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKVector2::Normalize(void) 
{

#ifdef SK_SSE
	{
		SKFastNormalize(*this,*this);
	}
#else
	{
		SKREAL f = (SKREAL)SQRT(x*x + y*y);

		if (f != 0.0f) 
		{
			x/=f; y/=f;
		}

	}
#endif
}