#include "SKMath.h"
#include "SKPlane3.h"
#include "SKLine3.h"
#include "SKTriangle3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKPlane3::SKPlane3()
{
	m_fD = 0.0f;
}
/*----------------------------------------------------------------*/
SKPlane3::SKPlane3(const SKVector3 &N , SKREAL fD)
{
	Set(N,fD);
}
/*----------------------------------------------------------------*/
SKPlane3::SKPlane3(const SKVector3 &N, const SKVector3 &P)
{
	Set(N,P);

}
/*----------------------------------------------------------------*/
SKPlane3::SKPlane3(const SKVector3 &P0,  const SKVector3 &P1, const SKVector3 &P2)
{
	Set(P0,P1,P2); 

}
/*----------------------------------------------------------------*/
SKPlane3::SKPlane3(const SKVector3 Point[3])
{
	Set(Point[0],Point[1],Point[2]);
}
/*----------------------------------------------------------------*/
void SKPlane3::Transform(const SKPlane3 &Plane,const SKMatrix3X3W &Mat)
{
	SKVector3W Temp(m_N.x,m_N.y,m_N.z,m_fD);


	SKMatrix3X3W Inv;
	Inv.InverseOf(Mat);
	SKMatrix3X3W Transpose;
	Transpose.TransposeOf(Inv);
	SKVector3W Resoult = Temp * Transpose;
	Set(SKVector3(Resoult.x, Resoult.y, Resoult.z), Resoult.w);
}
SKVector3 SKPlane3::ReflectDir(const SKVector3 & Dir)const
{
	SKVector3 TempN = m_N * (-1.0f);

	return Dir - m_N * Dir.Dot(m_N) * 2.0f;
}

