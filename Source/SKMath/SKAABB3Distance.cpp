#include "SKAABB3.h"
#include "SKMath.h"
#include "SKMatrix3X3.h"
#include "SKLine3.h"
#include "SKRay3.h"
#include "SKRectangle3.h"
#include "SKSegment3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKVector3 & Point,SKREAL fAABBParameter[3])const
{
	SKVector3 Diff = Point - m_Center;


	SKREAL fSqrDistance = 0.0, fDelta;

	fAABBParameter[0] = Diff.x;
	if (fAABBParameter[0] < -m_fA[0])
	{
		fDelta = fAABBParameter[0] + m_fA[0];
		fSqrDistance += fDelta*fDelta;
		fAABBParameter[0] = -m_fA[0];
	}
	else if (fAABBParameter[0] > m_fA[0])
	{
		fDelta = fAABBParameter[0] - m_fA[0];
		fSqrDistance += fDelta*fDelta;
		fAABBParameter[0] = m_fA[0];
	}

	fAABBParameter[1] = Diff.y;
	if (fAABBParameter[1] < -m_fA[1])
	{
		fDelta = fAABBParameter[1] + m_fA[1];
		fSqrDistance += fDelta*fDelta;
		fAABBParameter[1] = -m_fA[1];
	}
	else if (fAABBParameter[1] > m_fA[1])
	{
		fDelta = fAABBParameter[1] - m_fA[1];
		fSqrDistance += fDelta*fDelta;
		fAABBParameter[1] = m_fA[1];
	}

	fAABBParameter[2] = Diff.z;
	if (fAABBParameter[2] < -m_fA[2])
	{
		fDelta = fAABBParameter[2] + m_fA[2];
		fSqrDistance += fDelta*fDelta;
		fAABBParameter[2] = -m_fA[2];
	}
	else if (fAABBParameter[2] > m_fA[2])
	{
		fDelta = fAABBParameter[2] - m_fA[2];
		fSqrDistance += fDelta*fDelta;
		fAABBParameter[2] = m_fA[2];
	}
	return fSqrDistance;
}
/*----------------------------------------------------------------*/
/*
void SKAABB3::Face (int i0, int i1, int i2, SKVector3 Pnt,
				  SKVector3 Dir, SKVector3 PmE,
				  SKREAL& rfSqrDistance,SKREAL &fLParam)const
{
	SKVector3 kPpE;
	SKREAL fLSqr, fInv, fTmp, fParam, fT, fDelta;

	kPpE.m[i1] = Pnt.m[i1] + m_fA[i1];
	kPpE.m[i2] = Pnt.m[i2] + m_fA[i2];
	if (Dir.m[i0] * kPpE.m[i1] >= Dir.m[i1] * PmE.m[i0])
	{
		if (Dir.m[i0] * kPpE.m[i2] >= Dir.m[i2] * PmE.m[i0])
		{
			// v[i1] >= -e[i1], v[i2] >= -e[i2] (distance = 0)
			Pnt.m[i0] = m_fA[i0];
			fInv = 1.0f / Dir.m[i0];
			Pnt.m[i1] -= Dir.m[i1] * PmE.m[i0] * fInv;
			Pnt.m[i2] -= Dir.m[i2] * PmE.m[i0] * fInv;
			fLParam = -PmE.m[i0] * fInv;
		}
		else
		{
			// v[i1] >= -e[i1], v[i2] < -e[i2]
			fLSqr = Dir.m[i0] * Dir.m[i0] + Dir.m[i2] * Dir.m[i2];
			fTmp = fLSqr * kPpE.m[i1] - Dir.m[i1] * ( Dir.m[i0] * PmE.m[i0] +
				Dir.m[i2] * kPpE.m[i2]);
			if (fTmp <= 2.0f * fLSqr * m_fA[i1])
			{
				fT = fTmp / fLSqr;
				fLSqr += Dir.m[i1]*Dir.m[i1];
				fTmp = kPpE.m[i1] - fT;
				fDelta = Dir.m[i0] * PmE.m[i0] + Dir.m[i1] * fTmp +
					Dir.m[i2]*kPpE.m[i2];
				fParam = -fDelta / fLSqr;
				rfSqrDistance += PmE.m[i0] * PmE.m[i0] + fTmp * fTmp +
					kPpE.m[i2]*kPpE.m[i2] + fDelta * fParam;

				fLParam = fParam;
				Pnt.m[i0] = m_fA[i0];
				Pnt.m[i1] = fT - m_fA[i1];
				Pnt.m[i2] = -m_fA[i2];
			}
			else
			{
				fLSqr += Dir.m[i1] * Dir.m[i1];
				fDelta = Dir.m[i0] * PmE.m[i0] + Dir.m[i1] * PmE.m[i1] +
					Dir.m[i2] * kPpE.m[i2];
				fParam = -fDelta / fLSqr;
				rfSqrDistance += PmE.m[i0] * PmE.m[i0] + PmE.m[i1] * PmE.m[i1] +
					kPpE.m[i2] * kPpE.m[i2] + fDelta * fParam;

				fLParam = fParam;
				Pnt.m[i0] = m_fA[i0];
				Pnt.m[i1] = m_fA[i1];
				Pnt.m[i2] = -m_fA[i2];
			}
		}
	}
	else
	{
		if (Dir.m[i0] * kPpE.m[i2] >= Dir.m[i2] * PmE.m[i0])
		{
			// v[i1] < -e[i1], v[i2] >= -e[i2]
			fLSqr = Dir.m[i0] * Dir.m[i0] + Dir.m[i1] * Dir.m[i1];
			fTmp = fLSqr * kPpE.m[i2] - Dir.m[i2] * (Dir.m[i0]*  PmE.m[i0] +
				Dir.m[i1]*kPpE.m[i1] );
			if (fTmp <= 2.0f * fLSqr * m_fA[i2])
			{
				fT = fTmp / fLSqr;
				fLSqr += Dir.m[i2] * Dir.m[i2];
				fTmp = kPpE.m[i2] - fT;
				fDelta = Dir.m[i0] * PmE.m[i0] + Dir.m[i1] * kPpE.m[i1] +
					Dir.m[i2] * fTmp;
				fParam = -fDelta / fLSqr;
				rfSqrDistance += PmE.m[i0] * PmE.m[i0] + kPpE.m[i1] * kPpE.m[i1] +
					fTmp * fTmp + fDelta * fParam;

				fLParam = fParam;
				Pnt.m[i0] = m_fA[i0];
				Pnt.m[i1] = -m_fA[i1];
				Pnt.m[i2] = fT - m_fA[i2];
			}
			else
			{
				fLSqr += Dir.m[i2]*Dir.m[i2];
				fDelta = Dir.m[i0]*PmE.m[i0] + Dir.m[i1]*kPpE.m[i1] +
					Dir.m[i2]*PmE.m[i2];
				fParam = -fDelta/fLSqr;
				rfSqrDistance += PmE.m[i0]*PmE.m[i0] + kPpE.m[i1]*kPpE.m[i1] +
					PmE.m[i2]*PmE.m[i2] + fDelta*fParam;

				fLParam = fParam;
				Pnt.m[i0] = m_fA[i0];
				Pnt.m[i1] = -m_fA[i1];
				Pnt.m[i2] = m_fA[i2];
			}
		}
		else
		{
			// v[i1] < -e[i1], v[i2] < -e[i2]
			fLSqr = Dir.m[i0]*Dir.m[i0]+Dir.m[i2]*Dir.m[i2];
			fTmp = fLSqr*kPpE.m[i1] - Dir.m[i1]*(Dir.m[i0]*PmE.m[i0] +
				Dir.m[i2]*kPpE.m[i2]);
			if (fTmp >= 0.0f)
			{
				// v[i1]-edge is closest
				if (fTmp <= (2.0f)*fLSqr*m_fA[i1])
				{
					fT = fTmp/fLSqr;
					fLSqr += Dir.m[i1]*Dir.m[i1];
					fTmp = kPpE.m[i1] - fT;
					fDelta = Dir.m[i0]*PmE.m[i0] + Dir.m[i1]*fTmp +
						Dir.m[i2]*kPpE.m[i2];
					fParam = -fDelta/fLSqr;
					rfSqrDistance += PmE.m[i0]*PmE.m[i0] + fTmp*fTmp +
						kPpE.m[i2]*kPpE.m[i2] + fDelta*fParam;

					fLParam = fParam;
					Pnt.m[i0] = m_fA[i0];
					Pnt.m[i1] = fT - m_fA[i1];
					Pnt.m[i2] = -m_fA[i2];
				}
				else
				{
					fLSqr += Dir.m[i1]*Dir.m[i1];
					fDelta = Dir.m[i0]*PmE.m[i0] + Dir.m[i1]*PmE.m[i1] +
						Dir.m[i2]*kPpE.m[i2];
					fParam = -fDelta/fLSqr;
					rfSqrDistance += PmE.m[i0]*PmE.m[i0] + PmE.m[i1]*PmE.m[i1]
					+ kPpE.m[i2]*kPpE.m[i2] + fDelta*fParam;

					fLParam = fParam;
					Pnt.m[i0] = m_fA[i0];
					Pnt.m[i1] = m_fA[i1];
					Pnt.m[i2] = -m_fA[i2];
				}
				return;
			}

			fLSqr = Dir.m[i0]*Dir.m[i0] + Dir.m[i1]*Dir.m[i1];
			fTmp = fLSqr*kPpE.m[i2] - Dir.m[i2]*(Dir.m[i0]*PmE.m[i0] +
				Dir.m[i1]*kPpE.m[i1]);
			if (fTmp >= 0.0f)
			{
				// v[i2]-edge is closest
				if (fTmp <= (2.0f)*fLSqr*m_fA[i2])
				{
					fT = fTmp/fLSqr;
					fLSqr += Dir.m[i2]*Dir.m[i2];
					fTmp = kPpE.m[i2] - fT;
					fDelta = Dir.m[i0]*PmE.m[i0] + Dir.m[i1]*kPpE.m[i1] +
						Dir.m[i2]*fTmp;
					fParam = -fDelta/fLSqr;
					rfSqrDistance += PmE.m[i0]*PmE.m[i0] + kPpE.m[i1]*kPpE.m[i1] +
						fTmp*fTmp + fDelta*fParam;

					fLParam = fParam;
					Pnt.m[i0] = m_fA[i0];
					Pnt.m[i1] = -m_fA[i1];
					Pnt.m[i2] = fT - m_fA[i2];
				}
				else
				{
					fLSqr += Dir.m[i2]*Dir.m[i2];
					fDelta = Dir.m[i0]*PmE.m[i0] + Dir.m[i1]*kPpE.m[i1] +
						Dir.m[i2]*PmE.m[i2];
					fParam = -fDelta/fLSqr;
					rfSqrDistance += PmE.m[i0]*PmE.m[i0] + kPpE.m[i1]*kPpE.m[i1] +
						PmE.m[i2]*PmE.m[i2] + fDelta*fParam;

					fLParam = fParam;
					Pnt.m[i0] = m_fA[i0];
					Pnt.m[i1] = -m_fA[i1];
					Pnt.m[i2] = m_fA[i2];
				}
				return;
			}

			// (v[i1],v[i2])-corner is closest
			fLSqr += Dir.m[i2]*Dir.m[i2];
			fDelta = Dir.m[i0]*PmE.m[i0] + Dir.m[i1]*kPpE.m[i1] +
				Dir.m[i2]*kPpE.m[i2];
			fParam = -fDelta/fLSqr;
			rfSqrDistance += PmE.m[i0]*PmE.m[i0] + kPpE.m[i1]*kPpE.m[i1] +
				kPpE.m[i2]*kPpE.m[i2] + fDelta*fParam;

			fLParam = fParam;
			Pnt.m[i0] = m_fA[i0];
			Pnt.m[i1] = -m_fA[i1];
			Pnt.m[i2] = -m_fA[i2];
		}
	}
}*/
/*----------------------------------------------------------------*/
/*void SKAABB3::CaseNoZeros (SKVector3& Pnt,
						 const SKVector3& Dir, SKREAL& rfSqrDistance,SKREAL &fLParam)const
{
	SKVector3 kPmE(
		Pnt.x - m_fA[0],
		Pnt.y - m_fA[1],
		Pnt.z - m_fA[2]);

	SKREAL fProdDxPy = Dir.x * kPmE.y;
	SKREAL fProdDyPx = Dir.y * kPmE.x;
	SKREAL fProdDzPx, fProdDxPz, fProdDzPy, fProdDyPz;

	if (fProdDyPx >= fProdDxPy)
	{
		fProdDzPx = Dir.z*kPmE.x;
		fProdDxPz = Dir.x*kPmE.z;
		if (fProdDzPx >= fProdDxPz)
		{
			// line intersects x = e0
			Face(0,1,2,Pnt,Dir,kPmE,rfSqrDistance,fLParam);
		}
		else
		{
			// line intersects z = e2
			Face(2,0,1,Pnt,Dir,kPmE,rfSqrDistance,fLParam);
		}
	}
	else
	{
		fProdDzPy = Dir.z*kPmE.y;
		fProdDyPz = Dir.y*kPmE.z;
		if (fProdDzPy >= fProdDyPz)
		{
			// line intersects y = e1
			Face(1,2,0,Pnt,Dir,kPmE,rfSqrDistance,fLParam);
		}
		else
		{
			// line intersects z = e2
			Face(2,0,1,Pnt,Dir,kPmE,rfSqrDistance,fLParam);
		}
	}
}*/
/*----------------------------------------------------------------*/
/*void SKAABB3::Case0 (int i0, int i1, int i2, SKVector3& Pnt,
				   const SKVector3& Dir, SKREAL& rfSqrDistance,SKREAL &fLParam)const
{
	SKREAL fPmE0 = Pnt.m[i0] - m_fA[i0];
	SKREAL fPmE1 = Pnt.m[i1] - m_fA[i1];
	SKREAL fProd0 = Dir.m[i1]*fPmE0;
	SKREAL fProd1 = Dir.m[i0]*fPmE1;
	SKREAL fDelta, fInvLSqr, fInv;

	if (fProd0 >= fProd1)
	{
		// line intersects P[i0] = e[i0]
		Pnt.m[i0] = m_fA[i0];

		SKREAL fPpE1 = Pnt.m[i1] + m_fA[i1];
		fDelta = fProd0 - Dir.m[i0]*fPpE1;
		if (fDelta >= 0.0f)
		{
			fInvLSqr = 1.0f/(Dir.m[i0]*Dir.m[i0]+Dir.m[i1]*Dir.m[i1]);
			rfSqrDistance += fDelta*fDelta*fInvLSqr;
			Pnt.m[i1] = -m_fA[i1];
			fLParam = -(Dir.m[i0]*fPmE0+Dir.m[i1]*fPpE1)*fInvLSqr;
		}
		else
		{
			fInv = 1.0f/Dir.m[i0];
			Pnt.m[i1] -= fProd0*fInv;
			fLParam = -fPmE0*fInv;
		}
	}
	else
	{
		// line intersects P[i1] = e[i1]
		Pnt.m[i1] = m_fA[i1];

		SKREAL fPpE0 = Pnt.m[i0] + m_fA[i0];
		fDelta = fProd1 - Dir.m[i1]*fPpE0;
		if (fDelta >= 0.0f)
		{
			fInvLSqr = 1.0f/(Dir.m[i0]*Dir.m[i0]+Dir.m[i1]*Dir.m[i1]);
			rfSqrDistance += fDelta*fDelta*fInvLSqr;
			Pnt.m[i0] = -m_fA[i0];
			fLParam = -(Dir.m[i0]*fPpE0+Dir.m[i1]*fPmE1)*fInvLSqr;
		}
		else
		{
			fInv = 1.0f/Dir.m[i1];
			Pnt.m[i0] -= fProd1*fInv;
			fLParam = -fPmE1*fInv;
		}
	}

	if (Pnt.m[i2] < -m_fA[i2])
	{
		fDelta = Pnt.m[i2] + m_fA[i2];
		rfSqrDistance += fDelta*fDelta;
		Pnt.m[i2] = -m_fA[i2];
	}
	else if (Pnt.m[i2] > m_fA[i2])
	{
		fDelta = Pnt.m[i2] - m_fA[i2];
		rfSqrDistance += fDelta*fDelta;
		Pnt.m[i2] = m_fA[i2];
	}
}*/
/*----------------------------------------------------------------*/
/*void SKAABB3::Case00 (int i0, int i1, int i2,
					SKVector3& Pnt, const SKVector3& Dir,  SKREAL& rfSqrDistance,SKREAL &fLParam)const
{
	SKREAL fDelta;

	fLParam = (m_fA[i0] - Pnt.m[i0])/Dir.m[i0];

	Pnt.m[i0] = m_fA[i0];

	if (Pnt.m[i1] < -m_fA[i1])
	{
		fDelta = Pnt.m[i1] + m_fA[i1];
		rfSqrDistance += fDelta*fDelta;
		Pnt.m[i1] = -m_fA[i1];
	}
	else if (Pnt.m[i1] > m_fA[i1])
	{
		fDelta = Pnt.m[i1] - m_fA[i1];
		rfSqrDistance += fDelta*fDelta;
		Pnt.m[i1] = m_fA[i1];
	}

	if (Pnt.m[i2] < -m_fA[i2])
	{
		fDelta = Pnt.m[i2] + m_fA[i2];
		rfSqrDistance += fDelta*fDelta;
		Pnt.m[i2] = -m_fA[i2];
	}
	else if (Pnt.m[i2] > m_fA[i2])
	{
		fDelta = Pnt.m[i2] - m_fA[i2];
		rfSqrDistance += fDelta*fDelta;
		Pnt.m[i2] = m_fA[i2];
	}
}*/
/*----------------------------------------------------------------*/
/*void SKAABB3::Case000 (SKVector3& Pnt, SKREAL& rfSqrDistance)const
{
	SKREAL fDelta;

	if (Pnt.x < -m_fA[0])
	{
		fDelta = Pnt.x + m_fA[0];
		rfSqrDistance += fDelta*fDelta;
		Pnt.x = -m_fA[0];
	}
	else if (Pnt.x > m_fA[0])
	{
		fDelta = Pnt.x - m_fA[0];
		rfSqrDistance += fDelta*fDelta;
		Pnt.x = m_fA[0];
	}

	if (Pnt.y < -m_fA[1])
	{
		fDelta = Pnt.y + m_fA[1];
		rfSqrDistance += fDelta*fDelta;
		Pnt.y = -m_fA[1];
	}
	else if (Pnt.y > m_fA[1])
	{
		fDelta = Pnt.y - m_fA[1];
		rfSqrDistance += fDelta*fDelta;
		Pnt.y = m_fA[1];
	}

	if (Pnt.z < -m_fA[2])
	{
		fDelta = Pnt.z + m_fA[2];
		rfSqrDistance += fDelta*fDelta;
		Pnt.z = -m_fA[2];
	}
	else if (Pnt.z > m_fA[2])
	{
		fDelta = Pnt.z - m_fA[2];
		rfSqrDistance += fDelta*fDelta;
		Pnt.z = m_fA[2];
	}
}*/
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKLine3 & Line,SKREAL fAABBParameter[3],SKREAL &fLineParameter)const
{
	SKRectangle3 Rectangle[6];
	GetRectangle(Rectangle);
	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKREAL t,fAABB[3];
	for(int i = 0 ; i < 6 ; i++)
	{
		fSqrDistTmp = Line.SquaredDistance(*this,t,fAABB);
		if(fSqrDist < fSqrDistTmp)
		{	
			fSqrDist = fSqrDistTmp;
			fLineParameter = t;
			for(int j = 0 ; j < 3 ; j++)
			{
				fAABBParameter[j] = fAABB[j];
			}
		
		}
	
	}
	return fSqrDist;
	/*SKVector3 Orig = Line.GetOrig() - m_Center;
	SKVector3 Dir = Line.GetDir();

	bool bReflect[3];
	int i;
	for (i = 0; i < 3; i++)
	{
		if (Dir.m[i] < 0.0f)
		{
			Orig.m[i] = -Orig.m[i];
			Dir.m[i] = -Dir.m[i];
			bReflect[i] = true;
		}
		else
		{
			bReflect[i] = false;
		}
	}

	SKREAL fSqrDistance = 0.0f;
	fLineParameter = 0.0f;  

	if (Dir.x > 0.0f)
	{
		if (Dir.y > 0.0f)
		{
			if (Dir.z > 0.0f)  // (+,+,+)
			{
				CaseNoZeros(Orig,Dir,fSqrDistance,fLineParameter);
			}
			else  // (+,+,0)
			{
				Case0(0,1,2,Orig,Dir,fSqrDistance,fLineParameter);
			}
		}
		else
		{
			if (Dir.z > 0.0f)  // (+,0,+)
			{
				Case0(0,2,1,Orig,Dir,fSqrDistance,fLineParameter);
			}
			else  // (+,0,0)
			{
				Case00(0,1,2,Orig,Dir,fSqrDistance,fLineParameter);
			}
		}
	}
	else
	{
		if (Dir.y > 0.0f)
		{
			if (Dir.z > 0.0f)  // (0,+,+)
			{
				Case0(1,2,0,Orig,Dir,fSqrDistance,fLineParameter);
			}
			else  // (0,+,0)
			{
				Case00(1,0,2,Orig,Dir,fSqrDistance,fLineParameter);
			}
		}
		else
		{
			if (Dir.z > 0.0f)  // (0,0,+)
			{
				Case00(2,0,1,Orig,Dir,fSqrDistance,fLineParameter);
			}
			else  // (0,0,0)
			{
				Case000(Orig,fSqrDistance);
			}
		}
	}


	for (i = 0; i < 3; i++)
	{

		if (bReflect[i])
		{
			Orig.m[i] = -Orig.m[i];
		}


	}
	fAABBParameter[0] = Orig.x;
	fAABBParameter[1] = Orig.y;
	fAABBParameter[2] = Orig.z;
	return fSqrDistance;*/
}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKRay3 &Ray,SKREAL fAABBParameter[3],SKREAL &fRayParameter)const
{

	return Ray.SquaredDistance(*this,fRayParameter,fAABBParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKSegment3 & Segment,SKREAL fAABBParameter[3],SKREAL &fSegmentParameter)const
{
	return Segment.SquaredDistance(*this,fSegmentParameter,fAABBParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKTriangle3 & Triangle , SKREAL AABBParameter[3],
							SKREAL TriangleParameter[3])const
{
	SKRectangle3 Rectangle[6];
	GetRectangle(Rectangle);
	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKREAL RectP[2],TriP[3];

	fSqrDistTmp = Triangle.SquaredDistance(Rectangle[0],TriP,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		TriangleParameter[0] = TriP[0];
		TriangleParameter[1] = TriP[1];
		TriangleParameter[2] = TriP[2];
		AABBParameter[0] = m_fA[0];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = RectP[1];
	}

	fSqrDistTmp = Triangle.SquaredDistance(Rectangle[1],TriP,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		TriangleParameter[0] = TriP[0];
		TriangleParameter[1] = TriP[1];
		TriangleParameter[2] = TriP[2];
		AABBParameter[0] = -m_fA[0];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = -RectP[1];
	}

	fSqrDistTmp = Triangle.SquaredDistance(Rectangle[2],TriP,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		TriangleParameter[0] = TriP[0];
		TriangleParameter[1] = TriP[1];
		TriangleParameter[2] = TriP[2];
		AABBParameter[0] = RectP[1];
		AABBParameter[1] = m_fA[1];
		AABBParameter[2] = RectP[0];
	}

	fSqrDistTmp = Triangle.SquaredDistance(Rectangle[3],TriP,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		TriangleParameter[0] = TriP[0];
		TriangleParameter[1] = TriP[1];
		TriangleParameter[2] = TriP[2];
		AABBParameter[0] = -RectP[1];
		AABBParameter[1] = -m_fA[1];
		AABBParameter[2] = RectP[0];
	}

	fSqrDistTmp = Triangle.SquaredDistance(Rectangle[4],TriP,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		TriangleParameter[0] = TriP[0];
		TriangleParameter[1] = TriP[1];
		TriangleParameter[2] = TriP[2];
		AABBParameter[0] = -RectP[1];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = m_fA[2];
	}

	fSqrDistTmp = Triangle.SquaredDistance(Rectangle[5],TriP,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		TriangleParameter[0] = TriP[0];
		TriangleParameter[1] = TriP[1];
		TriangleParameter[2] = TriP[2];
		AABBParameter[0] = RectP[1];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = -m_fA[2];
	}
	return fSqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKRectangle3& Rectangle , SKREAL AABBParameter[3],
							 SKREAL RectangleParameter[2])const
{

	SKRectangle3 RectangleTemp[6];
	GetRectangle(RectangleTemp);
	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKREAL RectP[2],RectP1[2];

	fSqrDistTmp = Rectangle.SquaredDistance(RectangleTemp[0],RectP1,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		RectangleParameter[0] = RectP1[0];
		RectangleParameter[1] = RectP1[1];

		AABBParameter[0] = m_fA[0];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = RectP[1];
	}

	fSqrDistTmp = Rectangle.SquaredDistance(RectangleTemp[1],RectP1,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		RectangleParameter[0] = RectP1[0];
		RectangleParameter[1] = RectP1[1];

		AABBParameter[0] = -m_fA[0];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = -RectP[1];
	}

	fSqrDistTmp = Rectangle.SquaredDistance(RectangleTemp[2],RectP1,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		RectangleParameter[0] = RectP1[0];
		RectangleParameter[1] = RectP1[1];

		AABBParameter[0] = RectP[1];
		AABBParameter[1] = m_fA[1];
		AABBParameter[2] = RectP[0];
	}

	fSqrDistTmp = Rectangle.SquaredDistance(RectangleTemp[3],RectP1,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		RectangleParameter[0] = RectP1[0];
		RectangleParameter[1] = RectP1[1];

		AABBParameter[0] = -RectP[1];
		AABBParameter[1] = -m_fA[1];
		AABBParameter[2] = RectP[0];
	}

	fSqrDistTmp = Rectangle.SquaredDistance(RectangleTemp[4],RectP1,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		RectangleParameter[0] = RectP1[0];
		RectangleParameter[1] = RectP1[1];

		AABBParameter[0] = -RectP[1];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = m_fA[2];
	}

	fSqrDistTmp = Rectangle.SquaredDistance(RectangleTemp[5],RectP1,RectP);
	if(fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		RectangleParameter[0] = RectP1[0];
		RectangleParameter[1] = RectP1[1];

		AABBParameter[0] = RectP[1];
		AABBParameter[1] = RectP[0];
		AABBParameter[2] = -m_fA[2];
	}
	return fSqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKAABB3 &AABB , SKREAL AABB1Parameter[3],
							 SKREAL AABB2Parameter[3])const
{


	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKVector3 Point[8];
	GetPoint(Point);
	SKREAL Temp[3];
	for(int i = 0 ; i < 8 ; i++)
	{
		fSqrDistTmp = AABB.SquaredDistance(Point[i],Temp);
		if(fSqrDistTmp < fSqrDist)
		{
			GetParameter(Point[i],AABB1Parameter);
			for(int j = 0 ; j < 3; j++)
			{
				AABB2Parameter[j] = Temp[j];
			}
		}
	
	}
	
	return fSqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::Distance(const SKPlane3 &Plane,SKVector3 &AABBPoint,SKVector3 &PlanePoint)const
{

	return Plane.Distance(*this,PlanePoint,AABBPoint);
}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::Distance(const SKSphere3 & Sphere,SKREAL fAABBParameter[3],SKVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fAABBParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKAABB3::SquaredDistance(const SKPolygon3 &Polygon,SKREAL AABBParameter[3],int& IndexTriangle,
					   SKREAL TriangleParameter[3])const
{
		return Polygon.SquaredDistance(*this,IndexTriangle,TriangleParameter,AABBParameter);

}
