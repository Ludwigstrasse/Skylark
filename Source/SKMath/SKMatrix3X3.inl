#include "SKMath.h"
/*----------------------------------------------------------------*/
FORCEINLINE void SKMatrix3X3::Identity(void) 
{
	SKMemset(m, 0, sizeof(SKMatrix3X3));
	_00 = _11 = _22 =1.0f;
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKMatrix3X3::TransposeOf(const SKMatrix3X3 &Matrix) 
{
	_00 = Matrix._00;
	_10 = Matrix._01;
	_20 = Matrix._02;


	_01 = Matrix._10;
	_11 = Matrix._11;
	_21 = Matrix._12;


	_02 = Matrix._20;
	_12 = Matrix._21;
	_22 = Matrix._22;

}

/*----------------------------------------------------------------*/
FORCEINLINE SKREAL SKMatrix3X3::Det()const
{
#ifdef SK_SSE
	{
		__m128 _v1 = _mm_set_ps(M[1][1],M[1][0],M[1][0],0.0f);
		__m128 _v2 = _mm_set_ps(M[2][2],M[2][2],M[2][1],0.0f);
		__m128 _v12 = _mm_mul_ps(_v1,_v2);
		__m128 _v3 = _mm_set_ps(M[2][1],M[2][0],M[2][0],0.0f);
		__m128 _v4 = _mm_set_ps(M[1][2],M[1][2],M[1][1],0.0f);
		__m128 _v34 = _mm_mul_ps(_v3,_v4);

		__m128 _v = _mm_sub_ps(_v12,_v34);

		__m128 _vv = _mm_set_ps(M[0][0],M[0][1],M[0][2],0.0f);

		__m128 _r = _mm_mul_ps(_v,_vv);

		return _r.m128_f32[3] - _r.m128_f32[2] + _r.m128_f32[1];
	}
#else
	{
		return (M[0][0]*(M[1][1]*M[2][2] - M[2][1]*M[1][2]) - 
			M[0][1]*(M[1][0]*M[2][2] - M[2][0]*M[1][2]) + 
			M[0][2]*(M[1][0]*M[2][1] - M[2][0]*M[1][1]));
	}
#endif

}
FORCEINLINE void SKMatrix3X3::InverseOf(const SKMatrix3X3 &Matirx) 
{

	SKREAL det = Matirx.Det();

	if (ABS(det) < EPSILON_E4)
		return;


	SKREAL det_inv = 1.0f / det;

#ifdef SK_SSE
 	{
		__m128 _v0 = _mm_set_ps(Matirx.M[0][1],Matirx.M[0][0],Matirx.M[0][0],0.0f);
		__m128 _v1 = _mm_set_ps(Matirx.M[1][1],Matirx.M[1][0],Matirx.M[1][0],0.0f);
		__m128 _v2 = _mm_set_ps(Matirx.M[2][1],Matirx.M[2][0],Matirx.M[2][0],0.0f);
	
		__m128 _s0 = _mm_set_ps(Matirx.M[0][2],Matirx.M[0][2],Matirx.M[0][1],0.0f);
		__m128 _s1 = _mm_set_ps(Matirx.M[1][2],Matirx.M[1][2],Matirx.M[1][1],0.0f);
		__m128 _s2 = _mm_set_ps(Matirx.M[2][2],Matirx.M[2][2],Matirx.M[2][1],0.0f);

		__m128 _v1_s2 = _mm_mul_ps(_v1,_s2);
		__m128 _v2_s1 = _mm_mul_ps(_v2,_s1);

		__m128 _v0_s2 = _mm_mul_ps(_v0,_s2);
		__m128 _v2_s0 = _mm_mul_ps(_v2,_s0);

		__m128 _v0_s1 = _mm_mul_ps(_v0,_s1);
		__m128 _v1_s0 = _mm_mul_ps(_v1,_s0);

		__m128 k0 = _mm_sub_ps(_v1_s2,_v2_s1);
		__m128 k1 = _mm_sub_ps(_v0_s2,_v2_s0);
		__m128 k2 = _mm_sub_ps(_v0_s1,_v1_s0);

		__m128 f = _mm_set_ps(det_inv,det_inv,det_inv,0.0f);

		__m128 k0f = _mm_mul_ps(k0,f);
		__m128 k1f = _mm_mul_ps(k1,f);
		__m128 k2f = _mm_mul_ps(k2,f);

		M[0][0] =  k0f.m128_f32[3];
		M[1][0] = -k0f.m128_f32[2];
		M[2][0] =  k0f.m128_f32[1];

		M[0][1] = -k1f.m128_f32[3];
		M[1][1] =  k1f.m128_f32[2];
		M[2][1] = -k1f.m128_f32[1];

		M[0][2] =  k2f.m128_f32[3];
		M[1][2] = -k2f.m128_f32[2];
		M[2][2] =  k2f.m128_f32[1];
	}
#else
	{
		M[0][0] =  det_inv * (Matirx.M[1][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[1][2]);
		M[1][0] = -det_inv * (Matirx.M[1][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[1][2]);
		M[2][0] =  det_inv * (Matirx.M[1][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[1][1]);

		M[0][1] = -det_inv * (Matirx.M[0][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[0][2]);
		M[1][1] =  det_inv * (Matirx.M[0][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[0][2]);
		M[2][1] = -det_inv * (Matirx.M[0][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[0][1]);

		M[0][2] =  det_inv * (Matirx.M[0][1] * Matirx.M[1][2] - Matirx.M[1][1] * Matirx.M[0][2]);
		M[1][2] = -det_inv * (Matirx.M[0][0] * Matirx.M[1][2] - Matirx.M[1][0] * Matirx.M[0][2]);
		M[2][2] =  det_inv * (Matirx.M[0][0] * Matirx.M[1][1] - Matirx.M[1][0] * Matirx.M[0][1]);
	}
#endif

} // func
/*----------------------------------------------------------------*/

FORCEINLINE SKMatrix3X3 SKMatrix3X3::operator * (const SKMatrix3X3 &Matirx) const 
{
	SKMatrix3X3 mResult;
	mResult.M[0][0] = 0;
	mResult.M[1][1] = 0;
	mResult.M[2][2] = 0;
#ifdef SK_SSE
 	{
		 __m128 _v1 = _mm_set_ps(M[0][0],M[0][1],M[0][2],0.0f);
		 __m128 _v2 = _mm_set_ps(M[1][0],M[1][1],M[1][2],0.0f);
		 __m128 _v3 = _mm_set_ps(M[2][0],M[2][1],M[2][2],0.0f);

		 __m128 _s1 = _mm_set_ps(Matirx.M[0][0],Matirx.M[1][0],Matirx.M[2][0],0.0f);
		 __m128 _s2 = _mm_set_ps(Matirx.M[0][1],Matirx.M[1][1],Matirx.M[2][1],0.0f);
		 __m128 _s3 = _mm_set_ps(Matirx.M[0][2],Matirx.M[1][2],Matirx.M[2][2],0.0f);
		
		 __m128 _v = _mm_mul_ps(_v1,_s1);
		 mResult.M[0][0]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];
		 _v = _mm_mul_ps(_v1,_s2);
		 mResult.M[0][1]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];
		 _v = _mm_mul_ps(_v1,_s3);
		 mResult.M[0][2]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];

		 _v = _mm_mul_ps(_v2,_s1);
		 mResult.M[1][0]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];
		 _v = _mm_mul_ps(_v2,_s2);
		 mResult.M[1][1]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];
		 _v = _mm_mul_ps(_v2,_s3);
		 mResult.M[1][2]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];

		 _v = _mm_mul_ps(_v3,_s1);
		 mResult.M[2][0]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];
		 _v = _mm_mul_ps(_v3,_s2);
		 mResult.M[2][1]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];
		 _v = _mm_mul_ps(_v3,_s3);
		 mResult.M[2][2]	= _v.m128_f32[3] + _v.m128_f32[2] + _v.m128_f32[1];

	}
#else
	{		
		for(unsigned char i = 0; i < 3; i++) 
			for(unsigned char j = 0; j < 3; j++) 
				for(unsigned int k = 0 ; k < 3 ; k++)
					mResult.M[i][j] += M[i][k] * Matirx.M[k][j];
	}
#endif
	return mResult;
} 
/*----------------------------------------------------------------*/
FORCEINLINE SKVector3 SKMatrix3X3::operator * (const SKVector3 &vc) const
{
	return vc * (*this);
}

/*----------------------------------------------------------------*/
FORCEINLINE void SKMatrix3X3::CreateFromTwoVector(const SKVector3 & v1,const SKVector3 & v2)
{
#ifdef SK_SSE
	{
		__m128 _v1 = _mm_set_ps(v1.x,v1.x,v1.x,0.0f);
		__m128 _v2 = _mm_set_ps(v1.y,v1.y,v1.y,0.0f);
		__m128 _v3 = _mm_set_ps(v1.z,v1.z,v1.z,0.0f);

		__m128 _v = _mm_set_ps(v2.x,v2.y,v2.z,0.0f);

		__m128 _r1 = _mm_mul_ps(_v1,_v);
		__m128 _r2 = _mm_mul_ps(_v2,_v);
		__m128 _r3 = _mm_mul_ps(_v3,_v);
		M[0][0] = _r1.m128_f32[3]; M[0][1] = _r1.m128_f32[2]; M[0][2] = _r1.m128_f32[1];
		M[1][0] = _r2.m128_f32[3]; M[1][1] = _r2.m128_f32[2]; M[1][2] = _r2.m128_f32[1];
		M[2][0] = _r3.m128_f32[3]; M[2][1] = _r3.m128_f32[2]; M[2][2] = _r3.m128_f32[1];
	}
#else
	{
		for(int i = 0 ; i < 3 ; i++)
		{
			for(int j = 0 ; j < 3 ;j++)
			{
				M[i][j] = v1.m[i] * v2.m[j];
			
			}
		}
	}
#endif
}
/*----------------------------------------------------------------*/
FORCEINLINE void SKMatrix3X3::SetZero()
{
	SKMemset(m, 0, sizeof(SKMatrix3X3));
}
