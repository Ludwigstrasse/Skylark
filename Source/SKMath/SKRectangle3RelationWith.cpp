#include "SKRectangle3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
int SKRectangle3::RelationWith(const SKLine3 &Line,bool bCull,SKREAL fRectangleParameter[2],SKREAL &fLineParameter)const
{
	return Line.RelationWith(*this,1,fLineParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKRay3 &Ray,bool bCull,SKREAL fRectangleParameter[2],SKREAL &fRayParameter)const
{
	return Ray.RelationWith(*this,1,fRayParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKSegment3 &Segment,bool bCull,SKREAL fRectangleParameter[2],SKREAL &fSegmentParameter)const
{
	return Segment.RelationWith(*this,1,fSegmentParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKPlane3 &Plane)const
{
	int iFrontNum = 0;
	int iBackNum = 0;
	SKVector3 Point[4];
	GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int iFlag = Point[i].RelationWith(Plane);
		if( iFlag == SKFRONT)
		{
			iFrontNum++;
		}
		else if(iFlag == SKBACK)
		{
			iBackNum++;
		}


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
int SKRectangle3::RelationWith(const SKTriangle3 &Triangle)const
{
	return Triangle.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKRectangle3 &Rectangle)const
{
	SKVector3 Point[4];
	GetPoint(Point);

	SKSegment3 Segment;
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		SKREAL fSegmentParameter;
		SKREAL RectangleParameter[2];
		if(Segment.RelationWith(Rectangle,0,fSegmentParameter,RectangleParameter) == SKINTERSECT)
			return SKINTERSECT;

	}
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
int SKRectangle3::RelationWith(const SKAABB3 &AABB)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(AABB) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;
	SKVector3 Point[4];
	GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
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
	/*SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(AABB) != SKINTERSECT)
		return SKNOINTERSECT;
	SKVector3 Point[4];
	GetPoint(Point);
	SKSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		SKREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(AABB,uiQuantity,fNear,fFar);
		if( iFlag == SKINTERSECT)
			return SKINTERSECT;
		else if(iFlag == SKIN)
			InNum ++;


	}
	if(InNum == 4)
		return SKIN;
	return SKNOINTERSECT;*/
}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKOBB3 &OBB)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(OBB) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;
	SKVector3 Point[4];
	GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
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
	SKVector3 Point[4];
	GetPoint(Point);
	SKSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		SKREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(OBB,uiQuantity,fNear,fFar);
		if( iFlag == SKINTERSECT)
			return SKINTERSECT;
		else if(iFlag == SKIN)
			InNum ++;


	}
	if(InNum == 4)
		return SKIN;
	return SKNOINTERSECT;*/
}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKSphere3 & Sphere)const
{
	SKPlane3 Plane = GetPlane();
	if(Plane.RelationWith(Sphere) != SKINTERSECT)
		return SKNOINTERSECT;
	int InNum = 0,OutNum = 0;
	SKVector3 Point[4];
	GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
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
	SKVector3 Point[4];
	GetPoint(Point);
	SKSegment3 Segment;
	int InNum = 0;
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		Segment.Set(Point[i],Point[j]);
		SKREAL fNear,fFar;
		unsigned int uiQuantity;
		int iFlag = Segment.RelationWith(Sphere,uiQuantity,fNear,fFar);
		if( iFlag == SKINTERSECT)
			return SKINTERSECT;
		else if(iFlag == SKIN)
			InNum ++;


	}
	if(InNum == 4)
		return SKIN;
	return SKNOINTERSECT;*/

}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKPlane3 &Plane,SKSegment3 &Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	int OnNum = 0, BackNum = 0,FrontNum  = 0;
	SKVector3 Orig,End;
	SKVector3 Point[4];
	GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		SegmentTemp.Set(Point[i],Point[j]);
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
	if(OnNum == 4)
		return SKON;
	if(BackNum == 4)
		return SKBACK;
	if(FrontNum == 4)
		return SKFRONT;
	//not control all path
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKTriangle3 &Triangle,SKSegment3 & Segment)const
{

	return Triangle.RelationWith(*this,Segment);
}
/*----------------------------------------------------------------*/
int SKRectangle3::RelationWith(const SKRectangle3 &Rectangle,SKSegment3 & Segment)const
{
	SKSegment3 SegmentTemp;
	int InNum = 0;
	SKVector3 Orig,End;
	SKVector3 Point[4];
	GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		SegmentTemp.Set(Point[i],Point[j]);
		SKREAL fSegmentParameter;
		SKREAL fRectangleParameter[2];
		int iFlag = SegmentTemp.RelationWith(Rectangle,0,fSegmentParameter,fRectangleParameter);
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
	
	
	Rectangle.GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		int j = i + 1;
		if(j == 4)
			j = 0;
		SegmentTemp.Set(Point[i],Point[j]);
		SKREAL fSegmentParameter;
		SKREAL fRectangleParameter[2];
		int iFlag = SegmentTemp.RelationWith(*this,0,fSegmentParameter,fRectangleParameter);
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
	if(!InNum)
		return SKNOINTERSECT;
	//not control all path
	return SKINTERSECT;
}