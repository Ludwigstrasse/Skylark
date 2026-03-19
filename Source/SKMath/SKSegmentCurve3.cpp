#include "SKSegmentCurve3.h"
using namespace SKEngine2;
SKSegmentCurve3::SKSegmentCurve3()
{

}
/*----------------------------------------------------------------*/
SKSegmentCurve3::~SKSegmentCurve3()
{

	
}
/*----------------------------------------------------------------*/
void SKSegmentCurve3::Set(const SKSegment3 &Segment)
{

	m_Segment = Segment;
}
/*----------------------------------------------------------------*/
SKSegment3 & SKSegmentCurve3::GetSegment()
{
	return m_Segment;
}
/*----------------------------------------------------------------*/