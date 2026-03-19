#include "SKCurve3.h"
#include "SKMatrix3X3W.h"
using namespace SKEngine2;
SKCurve3::SKCurve3()
{

}
/*----------------------------------------------------------------*/
SKCurve3::~SKCurve3()
{


}
/*----------------------------------------------------------------*/
void SKCurve3::GetFixedUpBNT(SKREAL t ,SKVector3 & B,SKVector3 & N,SKVector3& T)
{
	SKMatrix3X3W Mat;
	SKVector3 Point(0,0,0);
	SKVector3 FD = GetFirstDerivative(t);
	FD.Normalize();
	Mat.CreateFromLookAt(Point,FD);
	B.x = Mat._00;
	B.y = Mat._10;
	B.z = Mat._20;

	N.x = Mat._01;
	N.y = Mat._11;
	N.z = Mat._21;

	T.x = Mat._02;
	T.y = Mat._12;
	T.z = Mat._22;
}
/*----------------------------------------------------------------*/