#include "SKPolygon3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
int SKPolygon3::RelationWith(const SKLine3 &Line, bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3],
							SKREAL &fLineParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		int iFlag = TriangleTemp.RelationWith(Line,bCull,fTriangleParameter,fLineParameter);
		if(iFlag == SKINTERSECT)
			return SKINTERSECT;

	}
	
	return SKNOINTERSECT;


}

int SKPolygon3::RelationWith(const SKRay3 &Ray, bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3],
								SKREAL &fRayParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		int iFlag = TriangleTemp.RelationWith(Ray,bCull,fTriangleParameter,fRayParameter);
		iIndexTriangle = i;
		if(iFlag == SKINTERSECT)
			return SKINTERSECT;

	}

	return SKNOINTERSECT;
}

int SKPolygon3::RelationWith(const SKSegment3 &Segment, bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3],
								SKREAL &fSegmentParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		int iFlag = TriangleTemp.RelationWith(Segment,bCull,fTriangleParameter,fSegmentParameter);
		iIndexTriangle = i;
		if(iFlag == SKINTERSECT)
			return SKINTERSECT;

	}

	return SKNOINTERSECT;

}
int SKPolygon3::RelationWith(const SKPlane3 & Plane)const
{
	SKSegment3 Segment;
	int iBackNum = 0 , iFrontNum = 0 ;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{

		int iFlag = m_pPoint[i].RelationWith(Plane);
		if(iFlag == SKBACK)
			iBackNum++;
		else if(iFlag == SKFRONT)
			iFrontNum++;

		if(iBackNum > 0 && iFrontNum >0)
			return SKINTERSECT;
	
	}
	if(iBackNum == 0 && iFrontNum == 0)
		return SKON;
	else if(iBackNum == 0)
		return SKFRONT;
	else if(iFrontNum == 0)
		return SKBACK;
	else
		return SKINTERSECT;

}
int SKPolygon3::RelationWith(const SKPlane3 & Plane,SKSegment3 & Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	int OnNum = 0, BackNum = 0,FrontNum  = 0;
	SKVector3 Orig,End;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		SegmentTemp.Set(m_pPoint[i],m_pPoint[j]);
		SKREAL t;
		int iFlag = SegmentTemp.RelationWith(Plane,0,t);
		if( iFlag == SKINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(t);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(t);
				Segment.Set(Orig,End);	
				return SKINTERSECT;

			}
		}
		else if(iFlag == SKON)
		{
			OnNum++;

		}
		else if(iFlag == SKBACK)
		{
			BackNum++;

		}
		else if(iFlag == SKFRONT)
		{
			FrontNum++;
		}

	}
	if(OnNum == 3)
		return SKON;
	if(BackNum == 3)
		return SKBACK;
	if(FrontNum == 3)
		return SKFRONT;
	//not control all path
	return SKINTERSECT;
}
int SKPolygon3::RelationWith(const SKTriangle3 & Triangle)const
{
	SKSegment3 Segment;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
		SKREAL fSegmentParameter;
		SKREAL TriangleParameter[3];
		if(Segment.RelationWith(Triangle,0,fSegmentParameter,TriangleParameter) == SKINTERSECT)
			return SKINTERSECT;

	}
	SKVector3 Point[3];
	Triangle.GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		SKREAL fSegmentParameter;
		SKREAL TriangleParameter[3];
		int Index;
		if(Segment.RelationWith(*this,fSegmentParameter,0,Index,TriangleParameter) == SKINTERSECT)
			return SKINTERSECT;

	}
	return SKNOINTERSECT;

}
int SKPolygon3::RelationWith(const SKTriangle3 & Triangle,SKSegment3 & Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	SKVector3 Orig,End;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
		SKREAL fTriangleParameter[3];
		SKREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(Triangle,0,fSegmentParameter,fTriangleParameter);
		if( iFlag == SKINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return SKINTERSECT;
			}
		}


	}
	SKVector3 Point[3];
	Triangle.GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(Point[i],Point[j]);
		SKREAL fSegmentParameter;
		SKREAL fTriangleParameter[3];
		int Index;
		int iFlag = SegmentTemp.RelationWith(*this,fSegmentParameter,0,Index,fTriangleParameter);
		if( iFlag == SKINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return SKINTERSECT;
			}
		}


	}
	return SKNOINTERSECT;

}
int SKPolygon3::RelationWith(const SKRectangle3 & Rectangle)const
{
	SKSegment3 Segment;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
		SKREAL fSegmentParameter;
		SKREAL TriangleParameter[2];
		if(Segment.RelationWith(Rectangle,0,fSegmentParameter,TriangleParameter) == SKINTERSECT)
			return SKINTERSECT;

	}
	SKVector3 Point[4];
	Rectangle.GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		SKREAL fSegmentParameter;
		SKREAL TriangleParameter[3];
		int Index;
		if(Segment.RelationWith(*this,fSegmentParameter,0,Index,TriangleParameter) == SKINTERSECT)
			return SKINTERSECT;

	}
	return SKNOINTERSECT;

}
int SKPolygon3::RelationWith(const SKRectangle3 & Rectangle,SKSegment3 & Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	SKVector3 Orig,End;
	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
			j = 0;
		Segment.Set(m_pPoint[i],m_pPoint[j]);
		SKREAL fTriangleParameter[2];
		SKREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(Rectangle,0,fSegmentParameter,fTriangleParameter);
		if( iFlag == SKINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return SKINTERSECT;
			}
		}


	}
	SKVector3 Point[4];
	Rectangle.GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		SegmentTemp.Set(Point[i],Point[j]);
		SKREAL fSegmentParameter;
		SKREAL fTriangleParameter[3];
		int Index;
		int iFlag = SegmentTemp.RelationWith(*this,fSegmentParameter,0,Index,fTriangleParameter);
		if( iFlag == SKINTERSECT)
		{
			InNum ++;
			if(InNum == 1)
			{
				Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

			}
			else if(InNum == 2)
			{
				End = SegmentTemp.GetParameterPoint(fSegmentParameter);
				Segment.Set(Orig,End);	
				return SKINTERSECT;
			}
		}


	}
	return SKNOINTERSECT;
}
int SKPolygon3::RelationWith(const SKAABB3 &AABB)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(AABB) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int iFlag = m_pPoint[i].RelationWith(AABB);
		if(iFlag == SKIN || iFlag == SKON)
		{
			InNum++;

		}
		else if(iFlag == SKOUT)
		{

			OutNum++;
		}
		if(InNum > 0 && OutNum > 0)
			return SKINTERSECT;
	}
	if(InNum == 0)
		return SKOUT;
	if(OutNum == 0)
		return SKIN;
	else 
		return SKINTERSECT;

}


int SKPolygon3::RelationWith(const SKOBB3 &OBB)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(OBB) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int iFlag = m_pPoint[i].RelationWith(OBB);
		if(iFlag == SKIN || iFlag == SKON)
		{
			InNum++;

		}
		else if(iFlag == SKOUT)
		{

			OutNum++;
		}
		if(InNum > 0 && OutNum > 0)
			return SKINTERSECT;
	}
	if(InNum == 0)
		return SKOUT;
	if(OutNum == 0)
		return SKIN;
	else 
		return SKINTERSECT;
}


int SKPolygon3::RelationWith(const SKSphere3 &Sphere)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(Sphere) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int iFlag = m_pPoint[i].RelationWith(Sphere);
		if(iFlag == SKIN || iFlag == SKON)
		{
			InNum++;

		}
		else if(iFlag == SKOUT)
		{

			OutNum++;
		}
		if(InNum > 0 && OutNum > 0)
			return SKINTERSECT;
	}
	if(InNum == 0)
		return SKOUT;
	if(OutNum == 0)
		return SKIN;
	else 
		return SKINTERSECT;

}