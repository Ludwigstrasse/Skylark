#include "SKSegment3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKSegment3::SKSegment3()
{

}
/*----------------------------------------------------------------*/
SKSegment3::~SKSegment3()
{


}
/*----------------------------------------------------------------*/
SKSegment3::SKSegment3(const SKVector3 &Orig,const SKVector3 &End)
{
	Set(Orig,End);
}
/*----------------------------------------------------------------*/
SKSegment3::SKSegment3(const SKVector3 &Orig,const SKVector3 &Dir,SKREAL fLen)
{
	Set(Orig,Dir,fLen);
}
/*----------------------------------------------------------------*/
void SKSegment3::Transform(const SKSegment3 & Segment,const SKMatrix3X3W &Mat)
{

	m_Orig = Mat * Segment.m_Orig;
	m_End =  Mat * Segment.m_End;
	m_Dir  = Mat.Apply3X3(Segment.m_Dir);
	m_Dir.Normalize();
	m_fLen = Segment.m_fLen;
}
/*----------------------------------------------------------------*/
bool SKSegment3::GetParameter(const SKVector3 &Point,SKREAL &fSegmentParameter )const
{
	if(!SKRay3::GetParameter(Point,fSegmentParameter))
		return 0;
	if(fSegmentParameter > m_fLen)
		return 0;
	return 1;

}