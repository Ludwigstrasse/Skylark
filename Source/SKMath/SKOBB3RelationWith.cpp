#include "SKOBB3.h"
#include "SKMath.h"
#include "SKMatrix3X3.h"
#include "SKLine3.h"
#include "SKRay3.h"
#include "SKRectangle3.h"
#include "SKSegment3.h"
using namespace SKEngine2;
int SKOBB3::RelationWith(const SKVector3 &Point)const
{
	return Point.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKLine3 &Line, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{

	return Line.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKRay3 &Ray, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{

	return Ray.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKSegment3 &Segment, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	return Segment.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKPlane3 &Plane)const
{
	SKVector3 N = Plane.GetN();
	SKREAL fD = Plane.GetfD();
	SKREAL fRadius = ABS(m_fA[0] * (N.Dot(m_A[0])))
					+ ABS(m_fA[1] * (N.Dot(m_A[1])))
					+ ABS(m_fA[2] * (N.Dot(m_A[2])));


	SKREAL fTest = N.Dot(m_Center) + fD;

	
	if (fTest > fRadius)
		return SKFRONT;
	else if (fTest < -fRadius)
		return SKBACK;
	else
		return SKINTERSECT;

}
/*----------------------------------------------------------------*/
/*void SKOBB3::OBBProj(const SKOBB3 &OBB, const SKVector3 &vcV,
					SKREAL *pfMin, SKREAL *pfMax)const 
{
	SKREAL fDP = vcV * OBB.m_Center;
	SKREAL fR = OBB.m_fA[0] * ABS(vcV * OBB.m_A[0]) +
		OBB.m_fA[0] * ABS(vcV * OBB.m_A[1]) +
		OBB.m_fA[1] * ABS(vcV * OBB.m_A[2]);
	*pfMin = fDP - fR;
	*pfMax = fDP + fR;
} */
/*----------------------------------------------------------------*/
/*void SKOBB3::TriProj(const SKVector3 &v0, const SKVector3 &v1, 
					const SKVector3 &v2, const SKVector3 &vcV,
					SKREAL *pfMin, SKREAL *pfMax)const 
{
	*pfMin = vcV * v0;
	*pfMax = *pfMin;

	SKREAL fDP = vcV * v1;
	if (fDP < *pfMin) *pfMin = fDP;
	else if (fDP > *pfMax) *pfMax = fDP;

	fDP = vcV * v2;
	if (fDP < *pfMin) *pfMin = fDP;
	else if (fDP > *pfMax) *pfMax = fDP;
} */
/*----------------------------------------------------------------*/
/*
int SKOBB3::RelationWith(const SKTriangle3 & Triangle)const
{
	SKREAL     fMin0, fMax0, fMin1, fMax1;
	SKREAL     fD_C;
	SKVector3 vcV, vcTriEdge[3], vcA[3];
	SKVector3 TrianglePoint[3];
	Triangle.GetPoint(TrianglePoint);


	vcA[0] = m_A[0];
	vcA[1] = m_A[1];
	vcA[2] = m_A[2];


	vcTriEdge[0] = TrianglePoint[1] - TrianglePoint[0];
	vcTriEdge[1] = TrianglePoint[2] - TrianglePoint[0];

	vcV.Cross(vcTriEdge[0], vcTriEdge[1]);

	fMin0 = vcV * TrianglePoint[0];
	fMax0 = fMin0;

	OBBProj((*this), vcV, &fMin1, &fMax1);
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return SKNOINTERSECT;


	vcV = m_A[0];
	TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
	fD_C = vcV * m_Center;
	fMin1 = fD_C - m_fA[0];
	fMax1 = fD_C + m_fA[0];
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return SKNOINTERSECT;

	vcV = m_A[1];
	TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
	fD_C = vcV * m_Center;
	fMin1 = fD_C - m_fA[1];
	fMax1 = fD_C + m_fA[1];
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return SKNOINTERSECT;


	vcV = m_A[2];
	TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
	fD_C = vcV * m_Center;
	fMin1 = fD_C - m_fA[2];
	fMax1 = fD_C + m_fA[2];
	if ( fMax1 < fMin0 || fMax0 < fMin1 )
		return SKNOINTERSECT;


	vcTriEdge[2] = vcTriEdge[1] - vcTriEdge[0];
	for (int j=0; j<3; j++) 
	{
		for (int k=0; k<3; k++) 
		{
			vcV.Cross(vcTriEdge[j], vcA[k]);

			TriProj(TrianglePoint[0], TrianglePoint[1], TrianglePoint[2], vcV, &fMin0, &fMax0);
			OBBProj((*this), vcV, &fMin1, &fMax1);

			if ( (fMax1 < fMin0) || (fMax0 < fMin1) )
				return SKNOINTERSECT;
		}
	}

	return SKINTERSECT;
} // Intersects(Tri)
*/
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKTriangle3 & Triangle)const
{
	return Triangle.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKRectangle3 &Rectangle)const
{
	return Rectangle.RelationWith(*this);
}	
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKOBB3 &OBB)const
{
	SKREAL T[3];


	SKVector3 vcD = OBB.m_Center - m_Center;

	SKREAL matM[3][3];   
	SKREAL ra,           
		rb,           
		t;           


	matM[0][0] = m_A[0].Dot(OBB.m_A[0]);
	matM[0][1] = m_A[0].Dot(OBB.m_A[1]);
	matM[0][2] = m_A[0].Dot(OBB.m_A[2]);
	ra   = m_fA[0];
	rb   = OBB.m_fA[0] * ABS(matM[0][0]) + 
		OBB.m_fA[1] * ABS(matM[0][1]) + 
		OBB.m_fA[2] * ABS(matM[0][2]);

	T[0] = vcD.Dot(m_A[0]);
	t    = ABS(T[0]);
	if(t > (ra + rb) ) 
		return SKNOINTERSECT;


	matM[1][0] = m_A[1].Dot(OBB.m_A[0]);
	matM[1][1] = m_A[1].Dot(OBB.m_A[1]);
	matM[1][2] = m_A[1].Dot(OBB.m_A[2]);
	ra   = m_fA[1];
	rb   = OBB.m_fA[0] * ABS(matM[1][0]) + 
		OBB.m_fA[1] * ABS(matM[1][1]) + 
		OBB.m_fA[2] * ABS(matM[1][2]);
	T[1] = vcD.Dot(m_A[1]);
	t    = ABS(T[1]);
	if(t > (ra + rb) ) 
		return SKNOINTERSECT;


	matM[2][0] = m_A[2].Dot(OBB.m_A[0]);
	matM[2][1] = m_A[2].Dot(OBB.m_A[1]);
	matM[2][2] = m_A[2].Dot(OBB.m_A[2]);
	ra   = m_fA[2];
	rb   = OBB.m_fA[0] * ABS(matM[2][0]) + 
		OBB.m_fA[1] * ABS(matM[2][1]) + 
		OBB.m_fA[2] * ABS(matM[2][2]);
	T[2] = vcD.Dot(m_A[2]);
	t    = ABS(T[2]);
	if(t > (ra + rb) ) 
		return SKNOINTERSECT;


	ra = m_fA[0] * ABS(matM[0][0]) + 
		m_fA[1] * ABS(matM[1][0]) + 
		m_fA[2] * ABS(matM[2][0]);
	rb = OBB.m_fA[0];
	t = ABS( T[0]*matM[0][0] + T[1]*matM[1][0] + T[2]*matM[2][0] );
	if(t > (ra + rb) )
		return SKNOINTERSECT;


	ra = m_fA[0] * ABS(matM[0][1]) + 
		m_fA[1] * ABS(matM[1][1]) + 
		m_fA[2] * ABS(matM[2][1]);
	rb = OBB.m_fA[1];
	t = ABS( T[0]*matM[0][1] + T[1]*matM[1][1] + T[2]*matM[2][1] );
	if(t > (ra + rb) )
		return SKNOINTERSECT;


	ra = m_fA[0] * ABS(matM[0][2]) + 
		m_fA[1] * ABS(matM[1][2]) + 
		m_fA[2] * ABS(matM[2][2]);
	rb = OBB.m_fA[2];
	t = ABS( T[0]*matM[0][2] + T[1]*matM[1][2] + T[2]*matM[2][2] );
	if(t > (ra + rb) )
		return SKNOINTERSECT;


	ra = m_fA[1]*ABS(matM[2][0]) + m_fA[2]*ABS(matM[1][0]);
	rb = OBB.m_fA[1]*ABS(matM[0][2]) + OBB.m_fA[2]*ABS(matM[0][1]);
	t = ABS( T[2]*matM[1][0] - T[1]*matM[2][0] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[1]*ABS(matM[2][1]) + m_fA[2]*ABS(matM[1][1]);
	rb = OBB.m_fA[0]*ABS(matM[0][2]) + OBB.m_fA[2]*ABS(matM[0][0]);
	t = ABS( T[2]*matM[1][1] - T[1]*matM[2][1] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[1]*ABS(matM[2][2]) + m_fA[2]*ABS(matM[1][2]);
	rb = OBB.m_fA[0]*ABS(matM[0][1]) + OBB.m_fA[1]*ABS(matM[0][0]);
	t = ABS( T[2]*matM[1][2] - T[1]*matM[2][2] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[0]*ABS(matM[2][0]) + m_fA[2]*ABS(matM[0][0]);
	rb = OBB.m_fA[1]*ABS(matM[1][2]) + OBB.m_fA[2]*ABS(matM[1][1]);
	t = ABS( T[0]*matM[2][0] - T[2]*matM[0][0] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[0]*ABS(matM[2][1]) + m_fA[2]*ABS(matM[0][1]);
	rb = OBB.m_fA[0]*ABS(matM[1][2]) + OBB.m_fA[2]*ABS(matM[1][0]);
	t = ABS( T[0]*matM[2][1] - T[2]*matM[0][1] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[0]*ABS(matM[2][2]) + m_fA[2]*ABS(matM[0][2]);
	rb = OBB.m_fA[0]*ABS(matM[1][1]) + OBB.m_fA[1]*ABS(matM[1][0]);
	t = ABS( T[0]*matM[2][2] - T[2]*matM[0][2] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[0]*ABS(matM[1][0]) + m_fA[1]*ABS(matM[0][0]);
	rb = OBB.m_fA[1]*ABS(matM[2][2]) + OBB.m_fA[2]*ABS(matM[2][1]);
	t = ABS( T[1]*matM[0][0] - T[0]*matM[1][0] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[0]*ABS(matM[1][1]) + m_fA[1]*ABS(matM[0][1]);
	rb = OBB.m_fA[0] *ABS(matM[2][2]) + OBB.m_fA[2]*ABS(matM[2][0]);
	t = ABS( T[1]*matM[0][1] - T[0]*matM[1][1] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	ra = m_fA[0]*ABS(matM[1][2]) + m_fA[1]*ABS(matM[0][2]);
	rb = OBB.m_fA[0]*ABS(matM[2][1]) + OBB.m_fA[1]*ABS(matM[2][0]);
	t = ABS( T[1]*matM[0][2] - T[0]*matM[1][2] );
	if( t > ra + rb )
		return SKNOINTERSECT;


	return SKINTERSECT;
} 
/*----------------------------------------------------------------*/
int SKOBB3::RelationWith(const SKSphere3 &Sphere)const
{
	SKREAL fOBBParameter[3];
	SKREAL Dist = Sphere.m_Center.SquaredDistance(*this,fOBBParameter);
	Dist = SQRT(Dist);
	if(Dist - Sphere.m_fRadius < 0.0f)
		return SKINTERSECT;

	return SKNOINTERSECT;

}
