#include "SKTriangle3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKREAL SKTriangle3::SquaredDistance(const SKLine3 &Line,SKREAL fTriangleParameter[3],SKREAL &fLineParameter)const
{

	return Line.SquaredDistance(*this,fLineParameter,fTriangleParameter);


}
/*----------------------------------------------------------------*/
//κOBB
SKREAL SKTriangle3::SquaredDistance(const SKOBB3 &OBB,SKREAL TriangleParameter[3], SKREAL OBBParameter[3])const
{

	return OBB.SquaredDistance(*this, OBBParameter,TriangleParameter);

}
/*----------------------------------------------------------------*/
//ߺξ
SKREAL SKTriangle3::SquaredDistance(const SKRay3 & Ray,SKREAL fTriangleParameter[3],SKREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter,fTriangleParameter);

}
/*----------------------------------------------------------------*/
//߶κξ
SKREAL SKTriangle3::SquaredDistance(const SKSegment3& Segment,SKREAL fTriangleParameter[3],SKREAL &fSegmentParameter)const
{
	return Segment.SquaredDistance(*this,fSegmentParameter,fTriangleParameter);

}
/*----------------------------------------------------------------*/
//ξ
SKREAL SKTriangle3::SquaredDistance(const SKVector3 & Point,SKREAL fTriangleParameter[3])const
{
	return Point.SquaredDistance(*this,fTriangleParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKTriangle3::SquaredDistance(const SKTriangle3 &Triangle,
								  SKREAL fTriangle1Parameter[3],
								  SKREAL fTriangle2Parameter[3])const
{

	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKSegment3 Edge;
	SKREAL Tempt0,Temp[3];
	SKREAL fR0,fS0,fT0,fR1,fS1,fT1;
	Edge.Set(m_V[0],m_V[1]);
	fSqrDistTmp = Edge.SquaredDistance(Triangle,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS0 = Tempt0 / Edge.GetLen();
		fR0 = 1.0f - fS0;
		fT0 = 0.0f;
		fR1 = Temp[0];
		fS1 = Temp[1];
		fT1 = Temp[2];
		fSqrDist = fSqrDistTmp;
	}


	Edge.Set(m_V[0],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Triangle,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{


		fS0 = 0.0f;
		fT0 = Tempt0 / Edge.GetLen();
		fR0 = 1.0f - fT0;

		fR1 = Temp[0];
		fS1 = Temp[1];
		fT1 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(m_V[1],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Triangle,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS0 = Tempt0 / Edge.GetLen();
		fT0 = 1.0f - fS0;
		fR0 = 0.0f;

		fR1 = Temp[0];
		fS1 = Temp[1];
		fT1 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}



	Edge.Set(Triangle.m_V[0],Triangle.m_V[1]);
	fSqrDistTmp = Edge.SquaredDistance(*this,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS1= Tempt0 / Edge.GetLen();
		fR1 = 1.0f - fS0;
		fT1 = 0.0f;
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
		fSqrDist = fSqrDistTmp;
	}


	Edge.Set(Triangle.m_V[0],Triangle.m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(*this,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{


		fS1 = 0.0f;
		fT1 = Tempt0 / Edge.GetLen();
		fR1 = 1.0f - fT0;

		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(Triangle.m_V[1],Triangle.m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(*this,Tempt0,Temp);

	if (fSqrDistTmp < fSqrDist)
	{

		fS1 = Tempt0 / Edge.GetLen();
		fT1 = 1.0f - fS0;
		fR1 = 0.0f;

		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];

		fSqrDist = fSqrDistTmp;
	}

	fTriangle1Parameter[0] = fR0;
	fTriangle1Parameter[1] = fS0;
	fTriangle1Parameter[2] = fT0;

	fTriangle2Parameter[0] = fR1;
	fTriangle2Parameter[1] = fS1;
	fTriangle2Parameter[2] = fT1;
	return fSqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKTriangle3::SquaredDistance(const SKRectangle3 &Rectangle,
								  SKREAL fTriangleParameter[3],
								  SKREAL fRectangleParameter[2])const
{

	SKVector3 RectanglePoint[4];
	Rectangle.GetPoint(RectanglePoint);
	SKSegment3 Edge;
	//SKVector3 A[2];
	//Rectangle.GetA(A);
	SKREAL fA[2];
	Rectangle.GetfA(fA);
	SKREAL t,Temp[3];
	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKREAL fR0,fS0,fT0,fT1,fS1;
	Edge.Set(RectanglePoint[1],RectanglePoint[0]);
	fSqrDistTmp =Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;

		fS1 = t - fA[0];
		fT1 = fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];

	}
	Edge.Set(RectanglePoint[3],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = fA[0];
		fT1 = t - fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
	}
	Edge.Set(RectanglePoint[2],RectanglePoint[3]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = t - fA[0];
		fT1 =  -fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
	}
	Edge.Set(RectanglePoint[2],RectanglePoint[1]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = -fA[0];
		fT1 = t - fA[1];
		fR0 = Temp[0];
		fS0 = Temp[1];
		fT0 = Temp[2];
	}

	Edge.Set(m_V[0],m_V[1]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{


		fS0 = t / Edge.GetLen();
		fR0 = 1.0f - fS0;
		fT0 = 0.0f;
		fS1 = Temp[0];
		fT1 = Temp[1];
		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(m_V[0],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{


		fS0 = 0.0f;
		fT0 = t / Edge.GetLen();
		fR0 = 1.0f - fT0;

		fS1 = Temp[0];
		fT1 = Temp[1];
		fSqrDist = fSqrDistTmp;
	}

	Edge.Set(m_V[1],m_V[2]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,Temp);
	if (fSqrDistTmp < fSqrDist)
	{



		fS0 = t / Edge.GetLen();
		fT0 = 1.0f - fS0;
		fR0 = 0.0f;

		fS1 = Temp[0];
		fT1 = Temp[1];
		fSqrDist = fSqrDistTmp;
	}
	fTriangleParameter[0] = fR0;
	fTriangleParameter[1] = fS0;
	fTriangleParameter[2] = fT0;	
	fRectangleParameter[0] = fS1;
	fRectangleParameter[2] = fT1;
	return fSqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKTriangle3::Distance(const SKSphere3 &Sphere,SKREAL fTriangleParameter[3],SKVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fTriangleParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKTriangle3::Distance(const SKPlane3 &Plane,SKVector3 &TrianglePoint,SKVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,TrianglePoint);

}
/*----------------------------------------------------------------*/
SKREAL SKTriangle3::SquaredDistance(const SKAABB3 &AABB ,SKREAL TriangleParameter[3], SKREAL AABBParameter[3])const
{

	return AABB.SquaredDistance(*this,AABBParameter,TriangleParameter);


}
/*----------------------------------------------------------------*/
SKREAL SKTriangle3::SquaredDistance(const SKPolygon3& Polygon,
								   SKREAL fTriangle1Parameter[3],
								   int& IndexTriangle,
								   SKREAL fTriangle2Parameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangle2Parameter,fTriangle1Parameter);
}