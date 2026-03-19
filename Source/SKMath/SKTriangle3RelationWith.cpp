#include "SKTriangle3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKLine3 &Line, bool bCull,SKREAL fTriangleParameter[3],
				 SKREAL &fLineParameter)const
{

	return Line.RelationWith(*this,bCull,fLineParameter,fTriangleParameter);
}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKRay3 & Ray, bool bCull,SKREAL fTriangleParameter[3],
							 SKREAL &fRayParameter)const
{

	return Ray.RelationWith(*this,bCull,fRayParameter,fTriangleParameter);
}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKSegment3 & Segment, bool bCull,SKREAL fTriangleParameter[3],
							 SKREAL &fSegmentParameter)const
{

	return Segment.RelationWith(*this,bCull,fSegmentParameter,fTriangleParameter);
}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKPlane3 & Plane)const
{
	int iFrontNum = 0;
	int iBackNum = 0;

	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = m_V[i].RelationWith(Plane);
		if( iFlag == SKFRONT)
		{
			iFrontNum++;
		}
		else if(iFlag == SKBACK)
		{
			iBackNum++;
		}
		if(iFrontNum > 0 && iBackNum >0)
			return SKINTERSECT;
			
			
	}
	if(iFrontNum == 0 && iBackNum == 0)
	{
		return SKON;
	}
	else if(iFrontNum == 0)
		return SKBACK;
	else if(iBackNum  == 0)
		return SKFRONT;
	else
		return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKTriangle3 & Triangle)const
{
	SKSegment3 Segment;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		SKREAL fSegmentParameter;
		SKREAL TriangleParameter[3];
		if(Segment.RelationWith(Triangle,0,fSegmentParameter,TriangleParameter) == SKINTERSECT)
			return SKINTERSECT;
	
	}
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(Triangle.m_V[i],Triangle.m_V[j]);
		SKREAL fSegmentParameter;
		SKREAL TriangleParameter[3];
		if(Segment.RelationWith(*this,0,fSegmentParameter,TriangleParameter) == SKINTERSECT)
			return SKINTERSECT;

	}
	return SKNOINTERSECT;

}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKRectangle3 & Rectangle)const
{
	SKSegment3 Segment;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		SKREAL fSegmentParameter;
		SKREAL RectangleParameter[2];
		if(Segment.RelationWith(Rectangle,0,fSegmentParameter,RectangleParameter) == SKINTERSECT)
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
		SKREAL RectangleParameter[2];
		if(Segment.RelationWith(*this,0,fSegmentParameter,RectangleParameter) == SKINTERSECT)
			return SKINTERSECT;

	}
	return SKNOINTERSECT;

}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKAABB3 &AABB)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(AABB) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;
	SKVector3 Point[3];
	GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = Point[i].RelationWith(AABB);
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
	/*SKSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		SKREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(AABB,uiQuantity,fNear,fFar);
		if( iFlag == SKINTERSECT)
			return SKINTERSECT;
		else if(iFlag == SKIN)
			InNum ++;


	}
	if(InNum == 3)
		return SKIN;

	return SKNOINTERSECT;*/

}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKOBB3 &OBB)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(OBB) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;
	SKVector3 Point[3];
	GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = Point[i].RelationWith(OBB);
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
	/*SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(OBB) != SKINTERSECT)
		return SKNOINTERSECT;
	SKSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		SKREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(OBB,uiQuantity,fNear,fFar);
		if( iFlag == SKINTERSECT)
			return SKINTERSECT;
		else if(iFlag == SKIN)
			InNum ++;


	}
	if(InNum == 3)
		return SKIN;
	return SKNOINTERSECT;*/

}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKSphere3 &Sphere)const
{

	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(Sphere) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;
	SKVector3 Point[3];
	GetPoint(Point);
	for(int i = 0 ; i < 3 ; i++)
	{
		int iFlag = Point[i].RelationWith(Sphere);
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
	/*SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(Sphere) != SKINTERSECT)
		return SKNOINTERSECT;
	SKSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		Segment.Set(m_V[i],m_V[j]);
		SKREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(Sphere,uiQuantity,fNear,fFar);
		if( iFlag == SKINTERSECT)
			return SKINTERSECT;
		else if(iFlag == SKIN)
			InNum ++;


	}
	if(InNum == 3)
		return SKIN;
	return SKNOINTERSECT;*/
	
}
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKPlane3 & Plane,SKSegment3 & Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	int OnNum = 0, BackNum = 0,FrontNum  = 0;
	SKVector3 Orig,End;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(m_V[i],m_V[j]);
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
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKTriangle3 & Triangle,SKSegment3 & Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	SKVector3 Orig,End;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(m_V[i],m_V[j]);
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

	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(Triangle.m_V[i],Triangle.m_V[j]);
		SKREAL fTriangleParameter[3];
		SKREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(*this,0,fSegmentParameter,fTriangleParameter);
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
/*----------------------------------------------------------------*/
int SKTriangle3::RelationWith(const SKRectangle3 & Rectangle,SKSegment3 & Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	SKVector3 Orig,End;
	for(int i = 0 ; i < 3 ; i++)
	{
		int j = i + 1;
		if(j == 3)
			j = 0;
		SegmentTemp.Set(m_V[i],m_V[j]);
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
		SKREAL fTriangleParameter[3];
		SKREAL fSegmentParameter;
		int iFlag = SegmentTemp.RelationWith(*this,0,fSegmentParameter,fTriangleParameter);
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