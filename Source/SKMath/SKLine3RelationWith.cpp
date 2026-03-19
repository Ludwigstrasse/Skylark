#include "SKLine3.h"
#include "SKRay3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
int SKLine3::RelationWith(const SKTriangle3 & Triangle, bool bCull,SKREAL &fLineParameter,SKREAL fTriangleParameter[3])const
{
	SKVector3 pvec, tvec, qvec;
	SKVector3 TrianglePoint[3];
	Triangle.GetPoint(TrianglePoint);
	SKVector3 edge1 = TrianglePoint[1]- TrianglePoint[0];
	SKVector3 edge2 = TrianglePoint[2] - TrianglePoint[0];

	pvec.Cross(m_Dir, edge2);

	SKREAL det = edge1.Dot(pvec);
	if ( (bCull) && (det < EPSILON_E4) )
		return SKNOINTERSECT;
	else if ( (det < EPSILON_E4) && (det > -EPSILON_E4) )
		return SKNOINTERSECT;

	SKREAL f_det = 1.0f / det;
	tvec = m_Orig - TrianglePoint[0];
	SKREAL u = (tvec.Dot(pvec)) * f_det;
	if (u < 0.0f || u > 1)
		return SKNOINTERSECT;

	qvec.Cross(tvec, edge1);
	SKREAL v = (m_Dir.Dot(qvec)) * f_det;
	if (v < 0.0f || u+v > 1)
		return SKNOINTERSECT;

	SKREAL f = (edge2.Dot(qvec)) * f_det;

	fTriangleParameter[1] = u;
	fTriangleParameter[2] = v;
	fTriangleParameter[0] = 1.0f - u - v;
	fLineParameter = f;


	return SKINTERSECT;

}
/*----------------------------------------------------------------*/
int SKLine3::RelationWith(const SKPlane3 &Plane, bool bCull,SKREAL &fLineParameter)const
{
	SKREAL Vd = Plane.GetN().Dot(m_Dir);


	if (ABS(Vd) < EPSILON_E4)
	{
		
		return m_Orig.RelationWith(Plane);
	}


	if (bCull && (Vd > 0.0f))
		return SKNOINTERSECT;

	SKREAL Vo = -((Plane.GetN().Dot(m_Orig)) + Plane.GetfD());

	SKREAL _t = Vo / Vd;


	fLineParameter = _t;

	return SKINTERSECT;


}
/*----------------------------------------------------------------*/
int SKLine3::RelationWith(const SKRectangle3 &Rectangle,bool bCull,SKREAL &fLineParameter,SKREAL fRectangleParameter[2])const
{
	SKPlane3 Plane = Rectangle.GetPlane();
	if(bCull)
	{
		if(m_Orig.RelationWith(Plane) == SKBACK)
			return SKNOINTERSECT;
	}
	if(RelationWith(Plane,bCull,fLineParameter) == SKINTERSECT)
	{
		SKVector3 Point = GetParameterPoint(fLineParameter);
		if(Rectangle.GetParameter(Point,fRectangleParameter))
		{
			return SKINTERSECT;
		}
			
	}
	return SKNOINTERSECT;

}
/*----------------------------------------------------------------*/
int SKLine3::RelationWith(const SKOBB3 &OBB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	SKREAL e, f, t1, t2, temp;
	SKREAL	tmin = -SKMAX_REAL, 
			tmax =  SKMAX_REAL;
	SKVector3 A[3];
	SKREAL fA[3];
	SKVector3 vcP = OBB.GetCenter() - m_Orig;
	OBB.GetA(A);
	OBB.GetfA(fA);
	for(unsigned int i = 0; i < 3 ; i++)
	{
		e = A[i].Dot(vcP);
		f = A[i].Dot(m_Dir);
		if (ABS(f) > EPSILON_E4) 
		{

			t1 = (e + fA[i]) / f;
			t2 = (e - fA[i]) / f;

			if (t1 > t2) { temp=t1; t1=t2; t2=temp; }
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;
			if (tmin > tmax) return SKNOINTERSECT;
			if (tmax < 0.0f) return SKNOINTERSECT;
		}
		else if ( ((-e - fA[i]) > 0.0f) || ((-e + fA[i]) < 0.0f) )
			return SKNOINTERSECT;

	}

	return SKINTERSECT;


}
/*----------------------------------------------------------------*/
int SKLine3::RelationWith(const SKSphere3 &sphere, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	SKVector3 Delta = m_Orig - sphere.m_Center;
	SKREAL a0 = Delta.Dot(Delta) - sphere.m_fRadius * sphere.m_fRadius;
	SKREAL a1 = Delta.Dot(m_Dir);
	SKREAL Disc = a1 * a1 - a0; 
	if(Disc < 0)
	{
		Quantity = 0;
	}
	else if(Disc >= EPSILON_E4)
	{
		SKREAL root = SQRT(Disc);

		tFar = -a1 + root;
		tNear = -a1 - root;
		Quantity = 2;

	}
	else
	{
		tNear = -a1;
		tFar = -a1;
		Quantity = 1;

	}

	if(Quantity == 0)
		return SKNOINTERSECT;
	else
		return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKLine3::RelationWith(const SKAABB3 &AABB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	SKREAL t0, t1, tmp;
	tNear = -SKMAX_REAL;
	tFar  =  SKMAX_REAL;
	SKVector3 MaxT;
	SKVector3 Min = AABB.GetMinPoint();
	SKVector3 Max = AABB.GetMaxPoint();

	for(int i = 0 ; i < 3; i++)
	{
		if (ABS(m_Dir.m[i]) < EPSILON_E4) 
		{
			if ( (m_Orig.m[i] < Min.m[i]) ||
				(m_Orig.m[i] > Max.m[i]) )
				return SKNOINTERSECT;
		}
		t0 = (Min.m[i] - m_Orig.m[i]) / m_Dir.m[i];
		t1 = (Max.m[i] - m_Orig.m[i]) / m_Dir.m[i];
		if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
		if (t0 > tNear) tNear = t0;
		if (t1 < tFar)  tFar = t1;
		if (tNear > tFar) return SKNOINTERSECT;
		if (tFar < 0) return SKNOINTERSECT;
	}
	return SKINTERSECT;
	
}
/*----------------------------------------------------------------*/
int SKLine3::RelationWith(const SKPolygon3 &Polygon,SKREAL &fLineParameter, 
				 bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3])const
{
	return Polygon.RelationWith(*this,bCull,iIndexTriangle,fTriangleParameter,fLineParameter);
	
}