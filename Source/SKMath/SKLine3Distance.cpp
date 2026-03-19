#include "SKLine3.h"
#include "SKRay3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
SKREAL SKLine3::SquaredDistance(const SKVector3 &Point,SKREAL &fLineParameter)const
{
	SKVector3 Diff = Point - m_Orig;

	fLineParameter = m_Dir.Dot(Diff);

	Diff -= m_Dir * fLineParameter;

	return Diff.GetSqrLength();
}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKLine3 &Line,SKREAL &fLine1Parameter,SKREAL &fLine2Parameter)const
{
	SKREAL SquareDis;
	SKVector3 Diff = m_Orig - Line.m_Orig;
	SKREAL a01 = -(m_Dir.Dot(Line.m_Dir));
	SKREAL b0 = m_Dir.Dot(Diff);
	SKREAL c = Diff.GetSqrLength();
	SKREAL det = 1 - a01 * a01;

	if(ABS(det) >=EPSILON_E4)
	{
		SKREAL b1 = -(Line.m_Dir.Dot(Diff));
		SKREAL invDet = 1 / det;
		fLine1Parameter = (a01 * b1 - b0) * invDet;
		fLine2Parameter = (a01 * b0 - b1) * invDet;
		SquareDis = fLine1Parameter * (fLine1Parameter + a01 * fLine2Parameter + 2 * b0) + fLine2Parameter * (a01 * fLine1Parameter + fLine2Parameter + 2 * b1) + c;

	}
	else
	{
		fLine1Parameter =  - b0;
		fLine2Parameter = 0;

		SquareDis = b0 * fLine1Parameter + c;

	}
	return SquareDis;
}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKRay3 &Ray,SKREAL &fLineParameter,SKREAL &fRayParameter)const
{
	const SKLine3 Line = Ray;
	SKREAL sqrDist = SquaredDistance(Line,fLineParameter,fRayParameter);

	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = SquaredDistance(Ray.m_Orig,fLineParameter);

	}
	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKSegment3 & Segment,SKREAL &fLineParameter,SKREAL &fSegmentParameter)const
{
	const SKRay3 Ray = Segment;
	SKREAL sqrDist = SquaredDistance(Ray,fLineParameter,fSegmentParameter);
	SKREAL fLen = Segment.GetLen();

	if(fSegmentParameter > fLen)
	{
		SKVector3 End = Segment.GetEnd(); 
		fSegmentParameter = fLen;
		sqrDist = SquaredDistance(End,fLineParameter);

	}
	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKTriangle3 &Triangle,SKREAL &fLineParameter,SKREAL fTriangleParameter[3])const
{
	SKVector3 TrianglePoint[3];
	Triangle.GetPoint(TrianglePoint);

	/*SKVector3 Diff = TrianglePoint[0] - m_Orig;
	SKVector3 Edge0 = TrianglePoint[1] - TrianglePoint[0];
	SKVector3 Edge1 = TrianglePoint[2] - TrianglePoint[0];
	SKREAL fA00 = m_Dir * m_Dir;
	SKREAL fA01 = m_Dir * Edge0;
	SKREAL fA02 = m_Dir * Edge1;
	SKREAL fA11 = Edge0.GetSqrLength();
	SKREAL fA12 = Edge0 * Edge1;
	SKREAL fA22 = Edge1 * Edge1;
	SKREAL fB0  = Diff * m_Dir;
	SKREAL fB1  = Diff * Edge0;
	SKREAL fB2  = Diff * Edge1;
	SKVector3 Normal;
	Normal.Cross(Edge0,Edge1);
	SKREAL fDot = Normal * m_Dir;
	if (ABS(fDot) >= EPSILON_E4)
	{

		SKREAL fCof00 = fA11*fA22-fA12*fA12;
		SKREAL fCof01 = fA02*fA12-fA01*fA22;
		SKREAL fCof02 = fA01*fA12-fA02*fA11;
		SKREAL fCof11 = fA00*fA22-fA02*fA02;
		SKREAL fCof12 = fA02*fA01-fA00*fA12;
		SKREAL fCof22 = fA00*fA11-fA01*fA01;
		SKREAL fInvDet = (1.0f)/(fA00*fCof00+fA01*fCof01+fA02*fCof02);
		SKREAL fRhs0 = -fB0*fInvDet;
		SKREAL fRhs1 = -fB1*fInvDet;
		SKREAL fRhs2 = -fB2*fInvDet;


		fLineParameter = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;


		fTriangleParameter[1] = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
		fTriangleParameter[2] = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;
		fTriangleParameter[0] = 1.0f - fTriangleParameter[1] - fTriangleParameter[2];

		if (fTriangleParameter[0] >= 0.0f 
			&&  fTriangleParameter[1]>= 0.0f
			&&  fTriangleParameter[2] >= 0.0f)
		{

			return 0.0f;
		}
	}*/


	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKSegment3 Edge;
	SKREAL Tempt0,Tempt1;
	Edge.Set(TrianglePoint[0],TrianglePoint[1]);
	fSqrDistTmp = SquaredDistance(Edge,Tempt0,Tempt1);

	if (fSqrDistTmp < fSqrDist)
	{


		fLineParameter = Tempt0;

		fTriangleParameter[1] = Tempt1 / Edge.GetLen();
		fTriangleParameter[0] = 1.0f - fTriangleParameter[1];
		fTriangleParameter[2] = 0.0f;

		fSqrDist = fSqrDistTmp;
	}


	Edge.Set(TrianglePoint[0],TrianglePoint[2]);
	fSqrDistTmp = SquaredDistance(Edge,Tempt0,Tempt1);

	if (fSqrDistTmp < fSqrDist)
	{


		fLineParameter = Tempt0;

		fTriangleParameter[1] = 0.0f;
		fTriangleParameter[2] = Tempt1 / Edge.GetLen();
		fTriangleParameter[0] = 1.0f - fTriangleParameter[2];


		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(TrianglePoint[1],TrianglePoint[2]);
	fSqrDistTmp = SquaredDistance(Edge,Tempt0,Tempt1);

	if (fSqrDistTmp < fSqrDist)
	{


		fLineParameter = Tempt0;

		fTriangleParameter[1] = Tempt1 / Edge.GetLen();
		fTriangleParameter[2] = 1.0f - fTriangleParameter[1];
		fTriangleParameter[0] = 0.0f;


		fSqrDist = fSqrDistTmp;
	}
	return fSqrDist;


}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKRectangle3& Rectangle,SKREAL &fLineParameter,SKREAL fRectangleParameter[2])const
{
	//ûвཻ


	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKVector3 r[4];

	SKVector3 A[2];
	Rectangle.GetA(A);
	SKREAL fA[2];
	Rectangle.GetfA(fA);


	SKVector3 Temp0 = Rectangle.GetCenter() + A[0] * fA[0];
	SKVector3 Temp1 = Rectangle.GetCenter() + A[0] * (-fA[0]);
	r[0] = Temp0 + A[1] * fA[1] ;
	r[1] = Temp0 + A[1] * (-fA[1]);
	r[2] = Temp1 + A[1] * (-fA[1]);
	r[3] = Temp1 + A[1] * fA[1] ;

	SKSegment3 Edge;
	Edge.Set(r[0],r[1]);

	SKREAL fT0,fT1;
	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = fA[0];
		fRectangleParameter[1] = fT1 - fA[1];
	}
	Edge.Set(r[2],r[1]);

	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = fA[1];
		fRectangleParameter[1] = fT1 - fA[0];
	}
	Edge.Set(r[3],r[2]);

	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = -fA[0];
		fRectangleParameter[1] = fT1 - fA[1];
	}
	Edge.Set(r[3],r[0]);
	fSqrDistTmp = SquaredDistance(Edge,fT0,fT1);

	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fLineParameter = fT0;
		fRectangleParameter[0] = -fA[1];
		fRectangleParameter[1] = fT1 - fA[0];
	}
	return fSqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKOBB3 &OBB,SKREAL &fLineParameter,SKREAL fOBBParameter[3])const
{


	return OBB.SquaredDistance(*this,fOBBParameter,fLineParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKLine3::Distance(const SKSphere3 &Sphere,SKREAL &fLineParameter,SKVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fLineParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKLine3::Distance(const SKPlane3 &Plane,SKVector3 &LinePoint,SKVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,LinePoint);

}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKAABB3 &AABB,SKREAL &fLineParameter, SKREAL fAABBParameter[3])const
{
	return AABB.SquaredDistance(*this,fAABBParameter,fLineParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKLine3::SquaredDistance(const SKPolygon3 & Polygon,SKREAL &fLineParameter,
					   int& IndexTriangle,
					   SKREAL fTriangleParameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fLineParameter);
}