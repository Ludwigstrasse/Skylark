#ifndef SKQUAT_H
#define SKQUAT_H
namespace SKEngine2
{
class SKMatrix3X3;
class SKVector3;
class  SKMATH_API SKQuat 
{
public:
	union
	{
		SKREAL m[4];
		struct  
		{
			SKREAL x, y, z, w;
		};
	};

	SKQuat(void);
	SKQuat(SKREAL _x, SKREAL _y, SKREAL _z, SKREAL _w);
	void Set(SKREAL _x, SKREAL _y, SKREAL _z, SKREAL _w);
	//ͨתתǹԪ
	void CreateAxisAngle(const SKVector3& Axis,SKREAL fAngle);
	//ŷǹԪ
	void  CreateEuler(SKREAL fRoll, SKREAL fPitch, SKREAL fYaw);
	//λ
	void  Normalize();
	//
	SKQuat  GetConjugate()const;
	//õŷ
	void  GetEulers(SKREAL &fRoll, SKREAL &fPitch, SKREAL &fYaw)const;
	//Ԫõ任
	void  GetMatrix(SKMatrix3X3 &Matrix)const;
	//õ
	SKREAL GetLength(void)const;
	//ȡ
	SKQuat GetInverse()const;
	//ȡתת
	void GetAxisAngle(SKVector3 & Axis , SKREAL & fAngle)const;
	void    operator /= (SKREAL f);
	SKQuat operator /  (SKREAL f)const;

	void    operator *= (SKREAL f);
	SKQuat operator *  (SKREAL f)const;

	SKQuat operator *  (const SKVector3 &v) const;

	SKQuat operator *  (const SKQuat &q) const;
	void    operator *= (const SKQuat &q);

	void    operator += (const SKQuat &q);
	SKQuat operator +  (const SKQuat &q) const;

	void    operator -= (const SKQuat &q);
	SKQuat operator -  (const SKQuat &q) const;

	bool operator ==(const SKQuat &q)const;
	//
	SKREAL Dot(const SKQuat& q)const;
	//
	SKQuat operator ~(void) const;

	//q2q1תԪ
	void Rotate(const SKQuat &q1, const SKQuat &q2);

	//תһ
	SKVector3 Rotate(const SKVector3 &v)const;
	//ֵ
	void Slerp(SKREAL t,const SKQuat & q1,const SKQuat & q2);
	//2άͲֵ
	void TriangleSlerp(SKREAL t1,SKREAL t2, const SKQuat & q1,const SKQuat & q2,const SKQuat & q3);
	//Ԫֵ
	void Slerp(SKREAL t,const SKQuat & q1,const SKQuat & q2,const SKQuat & s1,const SKQuat & s2);
	void SlerpSValueOf(const SKQuat & q1,const SKQuat & q2,const SKQuat & q3);
	//
	SKQuat Pow(SKREAL exp)const;
	//eΪ׵Ķ
	SKQuat Ln()const;
	//eΪָ
	SKQuat Exp()const;



}; // class
/*----------------------------------------------------------------*/
}
#endif