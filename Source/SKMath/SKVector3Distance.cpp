#include "SKVector3.h"
#include "SKMath.h"
#include "SKMatrix3X3.h"
#include "SKQuat.h"
#include "SKLine3.h"
#include "SKRay3.h"
#include "SKSegment3.h"
#include "SKTriangle3.h"
#include "SKRectangle3.h"
#include "SKOBB3.h"
#include "SKSphere3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKVector3 &Point)const
{
	return (x * Point.x + y * Point.y + z * Point.z);

}
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKLine3 & Line,SKREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKRay3 & Ray,SKREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKSegment3 & Segment,SKREAL &fSegmentParameter)const
{
	return  Segment.SquaredDistance(*this,fSegmentParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKTriangle3 &Triangle,SKREAL fTriangleParameter[3])const
{
	SKVector3 TrianglePoint[3];
	Triangle.GetPoint(TrianglePoint);

	SKVector3 Diff = TrianglePoint[0] - *this;
	SKVector3 Edge0 = TrianglePoint[1]  - TrianglePoint[0] ;
	SKVector3 Edge1 = TrianglePoint[2]  - TrianglePoint[0] ;

	SKREAL fA00 = Edge0.GetSqrLength();
	SKREAL fA01 = Edge0.Dot(Edge1);
	SKREAL fA11 = Edge1.GetSqrLength();
	SKREAL fB0 = Diff.Dot(Edge0);
	SKREAL fB1 = Diff.Dot(Edge1);
	SKREAL fC = Diff.GetSqrLength();
	SKREAL fDet = ABS(fA00*fA11-fA01*fA01);
	SKREAL fS = fA01*fB1-fA11*fB0;
	SKREAL fT = fA01*fB0-fA00*fB1;
	SKREAL fSqrDistance;

	if (fS + fT <= fDet)
	{
		if (fS < 0.0f)
		{
			if (fT < 0.0f)  // region 4
			{
				if (fB0 < 0.0f)
				{
					fT = 0.0f;
					if (-fB0 >= fA00)
					{
						fS = 1.0f;
						fSqrDistance = fA00+(2.0f) * fB0 + fC;
					}
					else
					{
						fS = -fB0 / fA00;
						fSqrDistance = fB0 * fS + fC;
					}
				}
				else
				{
					fS = 0.0f;
					if (fB1 >= 0.0f)
					{
						fT = 0.0f;
						fSqrDistance = fC;
					}
					else if (-fB1 >= fA11)
					{
						fT = 1.0f;
						fSqrDistance = fA11 + 2.0f * fB1 + fC;
					}
					else
					{
						fT = -fB1 / fA11;
						fSqrDistance = fB1 * fT + fC;
					}
				}
			}
			else  // region 3
			{
				fS = 0.0f;
				if (fB1 >= 0.0f)
				{
					fT = 0.0f;
					fSqrDistance = fC;
				}
				else if (-fB1 >= fA11)
				{
					fT = 1.0f;
					fSqrDistance = fA11 + 2.0f * fB1 + fC;
				}
				else
				{
					fT = -fB1 / fA11;
					fSqrDistance = fB1 * fT + fC;
				}
			}
		}
		else if (fT < 0.0f)  // region 5
		{
			fT = 0.0f;
			if (fB0 >= 0.0f)
			{
				fS = 0.0f;
				fSqrDistance = fC;
			}
			else if (-fB0 >= fA00)
			{
				fS = 1.0f;
				fSqrDistance = fA00 + 2.0f * fB0 + fC;
			}
			else
			{
				fS = -fB0 / fA00;
				fSqrDistance = fB0 * fS + fC;
			}
		}
		else  // region 0
		{

			SKREAL fInvDet = 1.0f / fDet;
			fS *= fInvDet;
			fT *= fInvDet;
			fSqrDistance = fS * ( fA00 * fS + fA01 * fT + 2.0f * fB0) +
				fT * ( fA01 * fS + fA11 * fT + 2.0f * fB1 ) + fC;
		}
	}
	else
	{
		SKREAL fTmp0, fTmp1, fNumer, fDenom;

		if (fS < 0.0f)  // region 2
		{
			fTmp0 = fA01 + fB0;
			fTmp1 = fA11 + fB1;
			if (fTmp1 > fTmp0)
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00 - 2.0f * fA01 + fA11;
				if (fNumer >= fDenom)
				{
					fS = 1.0f;
					fT = 0.0f;
					fSqrDistance = fA00 + 2.0f * fB0 + fC;
				}
				else
				{
					fS = fNumer/fDenom;
					fT = 1.0f - fS;
					fSqrDistance = fS * ( fA00 * fS + fA01 * fT + 2.0f * fB0) +
						fT * ( fA01 * fS + fA11 * fT + 2.0f * fB1 )+fC;
				}
			}
			else
			{
				fS = 0.0f;
				if (fTmp1 <= 0.0f)
				{
					fT = 1.0f;
					fSqrDistance = fA11 + 2.0f * fB1 + fC;
				}
				else if (fB1 >= 0.0f)
				{
					fT = 0.0f;
					fSqrDistance = fC;
				}
				else
				{
					fT = -fB1 / fA11;
					fSqrDistance = fB1 * fT + fC;
				}
			}
		}
		else if (fT < 0.0f)  // region 6
		{
			fTmp0 = fA01 + fB1;
			fTmp1 = fA00 + fB0;
			if (fTmp1 > fTmp0)
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00 - 2.0f * fA01 + fA11;
				if (fNumer >= fDenom)
				{
					fT = 1.0f;
					fS = 0.0f;
					fSqrDistance = fA11 + 2.0f * fB1 + fC;
				}
				else
				{
					fT = fNumer / fDenom;
					fS = 1.0f - fT;
					fSqrDistance = fS * ( fA00 * fS + fA01 * fT + 2.0f * fB0) +
						fT*( fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
				}
			}
			else
			{
				fT = 0.0f;
				if (fTmp1 <= 0.0f)
				{
					fS = 1.0f;
					fSqrDistance = fA00 + 2.0f * fB0 + fC;
				}
				else if (fB0 >= 0.0f)
				{
					fS = 0.0f;
					fSqrDistance = fC;
				}
				else
				{
					fS = -fB0/fA00;
					fSqrDistance = fB0*fS+fC;
				}
			}
		}
		else  // region 1
		{
			fNumer = fA11 + fB1 - fA01 - fB0;
			if (fNumer <= 0.0f)
			{
				fS = 0.0f;
				fT = 1.0f;
				fSqrDistance = fA11 + 2.0f * fB1 + fC;
			}
			else
			{
				fDenom = fA00 - 2.0f * fA01 + fA11;
				if (fNumer >= fDenom)
				{
					fS = 1.0f;
					fT = 0.0f;
					fSqrDistance = fA00 + 2.0f * fB0 + fC;
				}
				else
				{
					fS = fNumer/fDenom;
					fT = 1.0f - fS;
					fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
						fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
				}
			}
		}
	}


	if (fSqrDistance < 0.0f)
	{
		fSqrDistance = 0.0f;
	}
	fTriangleParameter[1] = fS;
	fTriangleParameter[2] = fT;
	fTriangleParameter[0] = 1 - fTriangleParameter[1] -fTriangleParameter[2];

	return fSqrDistance;
}
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKRectangle3 &Rectangle,SKREAL fRectangleParameter[2])const
{
	SKVector3 Diff = Rectangle.GetCenter() - *this;

	SKVector3 A[2];
	Rectangle.GetA(A);
	SKREAL fA[2];
	Rectangle.GetfA(fA);

	SKREAL fB0 = Diff.Dot(A[0]);
	SKREAL fB1 = Diff.Dot(A[1]);
	SKREAL fS = -fB0;
	SKREAL fT = -fB1;
	SKREAL fSqrDistance = Diff.GetSqrLength();

	if (fS < -fA[0])
	{
		fS = -fA[0];
	}
	else if (fS > fA[0])
	{
		fS = fA[0];
	}
	fSqrDistance += fS * (fS + 2.0f * fB0);

	if (fT < -fA[1])
	{
		fT = -fA[1];
	}
	else if (fT > fA[1])
	{
		fT = fA[1];
	}
	fSqrDistance += fT * ( fT + 2.0f * fB1 );


	if (fSqrDistance <0.0f)
	{
		fSqrDistance = 0.0f;
	}
	fRectangleParameter[0] = fS;
	fRectangleParameter[1] = fT;
	return fSqrDistance;

}
/*----------------------------------------------------------------*/
//OBB
SKREAL SKVector3::SquaredDistance(const SKOBB3 &OBB,SKREAL fOBBParameter[3])const
{
	return OBB.SquaredDistance(*this,fOBBParameter);



}
/*----------------------------------------------------------------*/
SKREAL SKVector3::Distance(const SKSphere3& Sphere,SKVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint);

}
/*----------------------------------------------------------------*/
SKREAL SKVector3::Distance(const SKPlane3 & Plane,SKVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint);

}
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKAABB3 &AABB,SKREAL fAABBParameter[3])const
{
	return AABB.SquaredDistance(*this,fAABBParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKVector3::SquaredDistance(const SKPolygon3 & Polygon , int& IndexTriangle,SKREAL fTriangleParameter[3])const
{
	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter);

}