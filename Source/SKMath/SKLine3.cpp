#include "SKLine3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKLine3::SKLine3()
{


}
/*----------------------------------------------------------------*/
SKLine3::SKLine3(const SKVector3 & Orig,const SKVector3 &Dir)
{
	Set(Orig,Dir);

}
/*----------------------------------------------------------------*/
SKLine3::~SKLine3()
{


}
/*----------------------------------------------------------------*/
bool SKLine3::GetParameter(const SKVector3 &Point,SKREAL &fLineParameter )const
{
	SKVector3 Temp = Point - m_Orig;
	if(Temp.IsParallel(m_Dir))
	{
		fLineParameter = Temp.GetLength();
		return 1;
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/
void SKLine3::Transform(const SKLine3 &Line,const SKMatrix3X3W &Mat)
{
	m_Dir = Mat.Apply3X3(Line.m_Dir);
	m_Dir.Normalize();
	m_Orig = Line.m_Orig * Mat;

}