#include "SKMath.h"
#include "SKQuat.h"
#include "SKVector3.h"
#include "SKMatrix3X3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKQuat::SKQuat(void) 
{ 
	x=0.0f, y=0.0f, z=0.0f, w=1.0f; 
}
/*----------------------------------------------------------------*/
SKQuat::SKQuat(SKREAL _x, SKREAL _y, SKREAL _z, SKREAL _w)
{ 
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
void SKQuat::Set(SKREAL _x, SKREAL _y, SKREAL _z, SKREAL _w)
{
	x=_x; y=_y; z=_z; w=_w; 
}
/*----------------------------------------------------------------*/
void SKQuat::CreateEuler(SKREAL fRoll, SKREAL fPitch, SKREAL fYaw)
{
	SKREAL cX, cY, cZ, sX, sY, sZ, cXcZ, sXsZ, cXsZ, sXcZ;

	fPitch *= 0.5f;
	fYaw   *= 0.5f;
	fRoll  *= 0.5f;

	cX = COS(fPitch);
	cY = COS(fYaw);
	cZ = COS(fRoll);

	sX = SIN(fPitch);
	sY = SIN(fYaw);
	sZ = SIN(fRoll);

	cXcZ = cX * cZ;
	sXsZ = sX * sZ;
	cXsZ = cX * sZ;
	sXcZ = sX * cZ;
	
	w = cXcZ * cY + sXsZ * sY;
	x = sXcZ * cY + cXsZ * sY;
	y = cXcZ * sY - sXsZ * cY;
	z = cXsZ * cY - sXcZ * sY;

} // MakeFromEuler
/*----------------------------------------------------------------*/
void SKQuat::Normalize(void) 
{
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _q0q0 = _mm_mul_ps(_q0,_q0);
		SKREAL Invm = 1.0f/ SQRT(_q0q0.m128_f32[3] + _q0q0.m128_f32[2] + _q0q0.m128_f32[1] + _q0q0.m128_f32[0]);
		__m128 _Invm = _mm_set_ps(Invm,Invm,Invm,Invm);
		__m128 _r = _mm_mul_ps(_Invm,_q0);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		SKREAL m=SQRT(x*x + y*y + z*z + w*w);
		SKREAL Invm = 1.0f/ m;
		x *= Invm; y *= Invm; z *= Invm; w *= Invm;
	}
#endif
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::GetConjugate()const
{
	
	return SKQuat(-x,-y,-z,w);
}
/*----------------------------------------------------------------*/
void SKQuat::GetEulers(SKREAL &fRoll, SKREAL &fPitch, SKREAL &fYaw)const
{

	SKREAL wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;


	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;
	xy = x * y2;
	xz = x * z2;

	yy = y * y2;
	yz = y * z2;
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	SKREAL _00 = 1.0f - (yy + zz);//
	SKREAL _01 = xy + wz;//


	SKREAL _10 = xy - wz;//
	SKREAL _11 = 1.0f - (xx + zz);//


	SKREAL _20 = xz + wy;//
	SKREAL _21 = yz - wx;//
	SKREAL _22 = 1.0f - (xx + yy);//

	if (_21 > 1.0f)
		fPitch = -SKPI2;
	else if (_21 < -1.0f)
		fPitch = SKPI2;
	else
		fPitch = ASIN(-_21);

	if (fabs(_21) > SKREAL_1)
	{
		fYaw = 0;
		fRoll = ATAN2(-_10, _00);

	}
	else
	{
		fRoll = ATAN2(_01, _11);
		fYaw = ATAN2(_20, _22);

	}


} // EulerFromQuat
/*----------------------------------------------------------------*/
void SKQuat::GetMatrix(SKMatrix3X3 &Mat)const
{
	SKREAL wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;


	x2 = x + x; 
	y2 = y + y; 
	z2 = z + z;

	xx = x * x2;   
	xy = x * y2;  
	xz = x * z2;

	yy = y * y2;   
	yz = y * z2;   
	zz = z * z2;

	wx = w * x2;   
	wy = w * y2;   
	wz = w * z2;

	Mat._00 = 1.0f - (yy + zz);
	Mat._01 = xy + wz;
	Mat._02 = xz - wy;

	Mat._10 = xy - wz;
	Mat._11 = 1.0f - (xx + zz);
	Mat._12 = yz + wx;

	Mat._20 = xz + wy;
	Mat._21 = yz - wx;
	Mat._22 = 1.0f - (xx + yy);
} // GetMatrix
/*----------------------------------------------------------------*/
SKREAL SKQuat::GetLength(void)const 
{
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _q0q0 = _mm_mul_ps(_q0,_q0);
		return SQRT(_q0q0.m128_f32[3] + _q0q0.m128_f32[2] + _q0q0.m128_f32[1] + _q0q0.m128_f32[0]);
	}
#else
	{
		return SQRT(x*x + y*y + z*z + w*w);
	}
#endif
}
/*----------------------------------------------------------------*/
void SKQuat::operator /= (SKREAL f) 
{ 
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_div_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w /= f;  x /= f;  y /= f;  z /= f; 
	}
#endif
}

/*----------------------------------------------------------------*/
SKQuat SKQuat::operator / (SKREAL f) const
{
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_div_ps(_q0,_f);
		return SKQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{
		return SKQuat(x/f, y/f, z/f, w/f);
	}
#endif
}
/*----------------------------------------------------------------*/

void SKQuat::operator *= (SKREAL f) 
{ 
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_mul_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w *= f;  x *= f;  y *= f;  z *= f; 
	}
#endif
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::operator * (SKREAL f) const
{
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(f,f,f,f);
		__m128 _r = _mm_mul_ps(_q0,_f);
		return SKQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{
		return SKQuat(x*f, y*f, z*f, w*f);
	}
#endif
}
/*----------------------------------------------------------------*/


void SKQuat::operator += (const SKQuat &q) 
{ 
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_add_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w += q.w;  x += q.x;  y += q.y;  z += q.z; 
	}
#endif
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::operator + (const SKQuat &q) const
{
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_add_ps(_q0,_f);
		return SKQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{
		return SKQuat(x+q.x, y+q.y, z+q.z, w+q.w);
	}
#endif
}
/*----------------------------------------------------------------*/
void SKQuat::operator -= (const SKQuat &q) 
{ 
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_sub_ps(_q0,_f);
		x = _r.m128_f32[3];
		y = _r.m128_f32[2];
		z = _r.m128_f32[1];
		w = _r.m128_f32[0];
	}
#else
	{
		w -= q.w;  x -= q.x;  y -= q.y;  z -= q.z; 
	}
#endif
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::operator - (const SKQuat &q) const
{
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(x,y,z,w);
		__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
		__m128 _r = _mm_sub_ps(_q0,_f);
		return SKQuat(_r.m128_f32[3],_r.m128_f32[2],_r.m128_f32[1],_r.m128_f32[0]);
	}
#else
	{	
		return SKQuat(x-q.x, y-q.y, z-q.z, w-q.w);
	}
#endif
}
/*----------------------------------------------------------------*/
void SKQuat::operator *= (const SKQuat &q) 
{
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(w, w, w, w);
		__m128 _s0 = _mm_set_ps(q.w, q.x, q.y, q.z);

		__m128 _q1 = _mm_set_ps(-x, x, y, z);
		__m128 _s1 = _mm_set_ps(q.x, q.w, q.w, q.w);

		__m128 _q2 = _mm_set_ps(-y, z, x, y);
		__m128 _s2 = _mm_set_ps(q.y, q.y, q.z, q.x);

		__m128 _q3 = _mm_set_ps(-z, -y, -z, -x);
		__m128 _s3 = _mm_set_ps(q.z, q.z, q.x, q.y);

		__m128 _q0s0 = _mm_mul_ps(_q0,_s0);
		__m128 _q1s1 = _mm_mul_ps(_q1,_s1);
		__m128 _q2s2 = _mm_mul_ps(_q2,_s2);
		__m128 _q3s3 = _mm_mul_ps(_q3,_s3);

		__m128 _r = _mm_add_ps(_q0s0,_mm_add_ps(_q1s1,_mm_add_ps(_q2s2,_q3s3)));

		w = _r.m128_f32[3];
		x = _r.m128_f32[2];
		y = _r.m128_f32[1];
		z = _r.m128_f32[0];
		

	}
#else
	{

		w = w*q.w - x*q.x - y*q.y - z*q.z;
		x = w*q.x + x*q.w + z*q.y - y*q.z;
		y = w*q.y + y*q.w + x*q.z - z*q.x;
		z = w*q.z + z*q.w + y*q.x - x*q.y;
	}
#endif
}
/*----------------------------------------------------------------*/


SKQuat SKQuat::operator * (const SKQuat &q) const 
{
	SKQuat qResult;
#ifdef SK_SSE
	{
		__m128 _q0 = _mm_set_ps(w,w,w,w);
		__m128 _s0 = _mm_set_ps(q.w,q.x,q.y,q.z);

		__m128 _q1 = _mm_set_ps(-x,x,y,z);
		__m128 _s1 = _mm_set_ps(q.x,q.w,q.w,q.w);

		__m128 _q2 = _mm_set_ps(-y,z,x,y);
		__m128 _s2 = _mm_set_ps(q.y,q.y,q.z,q.x);

		__m128 _q3 = _mm_set_ps(-z,-y,-z,-x);
		__m128 _s3 = _mm_set_ps(q.z,q.z,q.x,q.y);

		__m128 _q0s0 = _mm_mul_ps(_q0,_s0);
		__m128 _q1s1 = _mm_mul_ps(_q1,_s1);
		__m128 _q2s2 = _mm_mul_ps(_q2,_s2);
		__m128 _q3s3 = _mm_mul_ps(_q3,_s3);

		__m128 _r = _mm_add_ps(_q0s0,_mm_add_ps(_q1s1,_mm_add_ps(_q2s2,_q3s3)));

		qResult.w = _r.m128_f32[3];
		qResult.x = _r.m128_f32[2];
		qResult.y = _r.m128_f32[1];
		qResult.z = _r.m128_f32[0];
		

	}
#else
	{
		qResult.w = w*q.w - x*q.x - y*q.y - z*q.z;
		qResult.x = w*q.x + x*q.w + z*q.y - y*q.z;
		qResult.y = w*q.y + y*q.w + x*q.z - z*q.x;
		qResult.z = w*q.z + z*q.w + y*q.x - x*q.y;
	}
#endif
	return qResult;
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::operator * (const SKVector3 &v) const 
{
	SKQuat t(v.x, v.y, v.z, 0);
	return (*this) * t;
}
/*----------------------------------------------------------------*/
bool SKQuat::operator ==(const SKQuat &q)const
{
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		if (ABS(m[i] - q.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
}
/*----------------------------------------------------------------*/
void SKQuat::Rotate(const SKQuat &q1, const SKQuat &q2) 
{ 
	SKQuat m1 = q1;
	SKQuat m2 = q2;
	m1.Normalize();
	m2.Normalize();
	SKQuat t = (~m1)*m2*m1;
	x=t.x; y=t.y; z=t.z; w=t.w; 
}
/*----------------------------------------------------------------*/
SKVector3 SKQuat::Rotate(const SKVector3 &v)const 
{ 
	SKQuat t(x, y, z, w);
	t.Normalize();
	SKQuat r = (~t)*v*t;
	return SKVector3(r.x, r.y, r.z); 
}
/*----------------------------------------------------------------*/
SKREAL SKQuat::Dot(const SKQuat& q)const
{
#ifdef SK_SSE
	{
	__m128 _q0 = _mm_set_ps(x,y,z,w);
	__m128 _f = _mm_set_ps(q.x,q.y,q.z,q.w);
	__m128 _r = _mm_mul_ps(_q0,_f);
	return _r.m128_f32[3] + _r.m128_f32[1] + _r.m128_f32[2] +_r.m128_f32[0];
	}
#else
	{
	return (w * q.w + x * q.x + y * q.y + z * q.z);
	}
#endif

}
/*----------------------------------------------------------------*/
SKQuat SKQuat::operator ~(void) const
{ 
	return SKQuat(-x, -y, -z, w); 
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::GetInverse()const
{
	return (GetConjugate()/GetLength());

}
/*----------------------------------------------------------------*/
void SKQuat::CreateAxisAngle(const SKVector3& Axis,SKREAL fAngle)
{
	SKREAL fTemp = fAngle/2.0f;
	SKREAL Sin = SIN(fTemp);
	SKREAL Cos = COS(fTemp);
	
	SKVector3 N = Axis;
	N.Normalize();
	
	w = Cos;
	x = Sin * N.x;
	y = Sin * N.y;
	z = Sin * N.z;
}
/*----------------------------------------------------------------*/
void SKQuat::GetAxisAngle(SKVector3 & Axis , SKREAL & fAngle)const
{

	SKQuat QTemp = *this;
	SKREAL fHalfAngle = ACOS(QTemp.w);

	fAngle = fHalfAngle * 2.0f;

	SKREAL SinHalfAngle = 1.0f - QTemp.w * QTemp.w;

	SKREAL InvSinHalfAngle = 1.0f / SinHalfAngle;
	Axis.x = QTemp.x * InvSinHalfAngle;
	Axis.y = QTemp.y * InvSinHalfAngle;
	Axis.z = QTemp.z * InvSinHalfAngle;


}
/*----------------------------------------------------------------*/
void SKQuat::Slerp(SKREAL t,const SKQuat & q1,const SKQuat & q2)
{
	if(t <= 0)  
	{
		(*this) = q1;
		return ;
	}
	if(t >= 1)  
	{
		(*this) = q2;
		return;
	}

	

	SKQuat q1Temp,q2Temp;
	q1Temp = q1;
	q2Temp = q2;
	q1Temp.Normalize();
	q2Temp.Normalize();
	SKREAL fCos = q1Temp.Dot(q2Temp);
	if(fCos < 0.0f)
	{
		q1Temp *= -1.0f;
		fCos = -fCos;
	}
	SKREAL k1 ,k2;
	if(fCos > SKREAL_1)
	{
		k1 = 1.0f - t;
		k2 = t;
	}
	else
	{
		SKREAL fSin = SQRT(1.0f - fCos * fCos);
		SKREAL fAngle = ATAN2(fSin, fCos);
		SKREAL fInvSin = 1.0f / fSin;
		k1 = SIN((1.0f - t) * fAngle) * fInvSin;
		k2 = SIN(t * fAngle) * fInvSin;

	
	}
	x = k1 * q1Temp.x + k2 * q2Temp.x;
	y = k1 * q1Temp.y + k2 * q2Temp.y;
	z = k1 * q1Temp.z + k2 * q2Temp.z;
	w = k1 * q1Temp.w + k2 * q2Temp.w;


}
/*----------------------------------------------------------------*/
SKQuat SKQuat::Pow(SKREAL exp)const
{
	if(ABS(w) > SKREAL_1)
		return *this;

	SKREAL alpha = ACOS(w);
	SKREAL newAlpha = alpha * exp;

	SKREAL mult = SIN(newAlpha) / SIN(alpha);
	return SKQuat(x * mult , y * mult , z * mult,COS(newAlpha)); 
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::Ln()const
{
	if(ABS(w) > SKREAL_1)
		return SKQuat(x , y , z , 0.0f);;
	SKREAL alpha = ACOS(w);

	SKREAL Sin = SIN(alpha);
	SKREAL temp = alpha / Sin;
	return SKQuat( temp * x  , temp * y ,temp * z  , 0.0f);
	
}
/*----------------------------------------------------------------*/
SKQuat SKQuat::Exp()const
{
	SKREAL alpha = GetLength();
	SKREAL Sin = SIN(alpha);
	if(ABS(Sin) > EPSILON_E4)
	{
		SKREAL temp = Sin / alpha;
		return SKQuat(x * temp , y * temp , z * temp,COS(alpha));
	
	}
	else
	{
		return SKQuat(x  , y , z ,COS(alpha));
	}
}
/*----------------------------------------------------------------*/
void SKQuat::SlerpSValueOf(const SKQuat & q1,const SKQuat & q2,const SKQuat & q3)
{
	SKQuat q2Inv = q2.GetInverse();
	SKQuat K1 = q1 * q2;
	SKQuat K2 = q3 * q2;

	SKQuat K3 = ( K1.Ln() + K2.Ln() ) * (-0.25f);
	*this = q2 * K3.Exp();
	

}
/*----------------------------------------------------------------*/
void SKQuat::Slerp(SKREAL t,const SKQuat & q1,const SKQuat & q2,const SKQuat & s1,const SKQuat & s2)
{
	SKREAL fSlerpT = 2.0f * t * (1.0f - t);
	SKQuat SlerpP;
	SlerpP.Slerp(t,q1,q2);
	SKQuat SlerpQ;
	SlerpQ.Slerp(t,s1,s2);
	Slerp(fSlerpT,SlerpP,SlerpQ);

}
/*----------------------------------------------------------------*/
void SKQuat::TriangleSlerp(SKREAL t1,SKREAL t2, const SKQuat & q1,const SKQuat & q2,const SKQuat & q3)
{
	*this = q1.Pow(t1) + q2.Pow(t2) + q3.Pow(1.0f - t1 - t2);
}