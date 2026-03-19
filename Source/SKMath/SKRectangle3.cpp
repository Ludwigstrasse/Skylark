#include "SKRectangle3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKRectangle3::SKRectangle3()
{

}
/*----------------------------------------------------------------*/
SKRectangle3::~SKRectangle3()
{

}
/*----------------------------------------------------------------*/
SKRectangle3::SKRectangle3(const SKVector3 & Center,const SKVector3 &A0,const SKVector3 & A1,SKREAL fA0, SKREAL fA1)
{
	Set(Center,A0,A1,fA0, fA1);
}
/*----------------------------------------------------------------*/
SKRectangle3::SKRectangle3(const SKVector3 & Center,const SKVector3 A[2],const SKREAL fA[2])
{
	Set(Center,A,fA);
}
/*----------------------------------------------------------------*/
bool SKRectangle3::GetParameter(const SKVector3 &Point,SKREAL fRectangleParameter[2])const
{
	SKVector3 A2;
	A2.Cross(m_A[0],m_A[1]);
	SKMatrix3X3W m;
	m.CreateInWorldObject(m_A[0],m_A[1],A2,m_Center);
	SKVector3 Temp;
	Temp = Point * m;
	fRectangleParameter[0] = Temp.x;
	fRectangleParameter[1] = Temp.y;
	if(ABS(Temp.z) > EPSILON_E4 || ABS(Temp.x) > m_fA[0] || ABS(Temp.y) > m_fA[1])
		return 0;
	return 1;
	

}
/*----------------------------------------------------------------*/
void SKRectangle3::Transform(const SKRectangle3 &Rectangle, 
							 const SKMatrix3X3W &Mat)
{
	m_A[0] = Mat.Apply3X3(Rectangle.m_A[0]);
	m_A[1] = Mat.Apply3X3(Rectangle.m_A[1]);
	m_A[0].Normalize();
	m_A[1].Normalize();
	m_Center = Rectangle.m_Center * Mat;
	m_fA[0] = Rectangle.m_fA[0];
	m_fA[1] = Rectangle.m_fA[1];

	SKVector3 N;
	N.Cross(m_A[0],m_A[1]);
	SKPlane3(N,m_Center);
}
