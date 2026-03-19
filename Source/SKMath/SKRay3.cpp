#include "SKRay3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKRay3::SKRay3()
{


}
/*----------------------------------------------------------------*/
SKRay3::SKRay3(const SKVector3 & Orig,const SKVector3 &Dir)
{
	Set(Orig,Dir);
}
/*----------------------------------------------------------------*/
SKRay3::~SKRay3()
{


}
/*----------------------------------------------------------------*/
bool SKRay3::GetParameter(const SKVector3 &Point,SKREAL &fRayParameter )const
{
	if(!SKLine3::GetParameter(Point,fRayParameter))
		return 0;

	if(fRayParameter < 0)
		return 0;

	return 1;

}
/*----------------------------------------------------------------*/
void SKRay3::Transform(const SKRay3 &Ray,const SKMatrix3X3W &Mat)
{

	SKLine3::Transform(Ray,Mat);
}