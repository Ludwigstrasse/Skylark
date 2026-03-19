#include "SKSphere3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKSphere3::SKSphere3()
{


}
/*----------------------------------------------------------------*/
SKSphere3::~SKSphere3()
{
	m_fRadius = 0.0f;

}
/*----------------------------------------------------------------*/
SKSphere3::SKSphere3(const SKVector3 & Center, SKREAL fRadius)
{
	Set(Center,fRadius);

}
/*----------------------------------------------------------------*/
void SKSphere3::CreateSphere(const SKVector3 *pPointArray,unsigned int uiPointNum)
{
	if(!pPointArray || !uiPointNum)
		return ;
	SKVector3 Sum;
	for(unsigned int i = 0 ; i < uiPointNum ; i++)
		Sum += pPointArray[i];
	m_Center = Sum / (uiPointNum * 1.0f);
	SKREAL radiusSqr = 0.0f;
	for(unsigned int i = 0 ; i < uiPointNum ; i++)
	{
		SKVector3 diff = pPointArray[i] - m_Center;
		SKREAL Temp = diff.GetSqrLength();
		if(Temp > radiusSqr)
			radiusSqr = Temp;
	
	}
	m_fRadius = SQRT(radiusSqr);
}
/*----------------------------------------------------------------*/
SKSphere3 SKSphere3::MergSpheres(const SKSphere3 &Sphere)const
{
	SKSphere3 Temp;
	SKVector3 CenterDiff = m_Center - Sphere.m_Center;
	SKREAL fRadiusDiff = m_fRadius - Sphere.m_fRadius;
	SKREAL fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
	SKREAL Lsqr = CenterDiff.GetSqrLength();
	if(fRadiusDiffSqr >= Lsqr)
	{
		if(fRadiusDiff >= 0.0f)
		{
			 Temp.m_Center = m_Center;
			Temp.m_fRadius = m_fRadius;
		}
		else
		{
			Temp.m_Center = Sphere.m_Center;
			Temp.m_fRadius = Sphere.m_fRadius;
		}
	
	
	}
	else
	{
		SKREAL L = SQRT(Lsqr);
		SKREAL t = (L + m_fRadius - Sphere.m_fRadius)/(2.0f * L);
		Temp.m_Center = Sphere.m_Center + CenterDiff * t;
		Temp.m_fRadius = (L + m_fRadius + Sphere.m_fRadius)/2.0f;
	}

	return Temp;

}
/*----------------------------------------------------------------*/
void SKSphere3::Transform(const SKSphere3 & Sphere,const SKMatrix3X3W &Mat)
{
	m_Center = Sphere.m_Center * Mat;
	/*SKMatrix3X3 M;
	Mat.Get3X3(M);
	SKVector3 V[3];
	SKREAL E[3];
	M.GetEigenSystem(E,V);
	SKREAL Max = E[0];
	for(unsigned int i = 1 ; i < 3 ; i++)
	{
	if(Max < E[i])
	Max = E[i];
	}
	m_fRadius = Sphere.m_fRadius * Max;*/
	SKVector3 V0(1.0f,0.0f,0.0f),V1(0.0f,1.0f,0.0f),V2(0.0f,0.0f,1.0f);
	SKVector3 Temp0 = Sphere.m_Center +  V0 * Sphere.m_fRadius;
	SKVector3 Temp1 = Sphere.m_Center +  V1 * Sphere.m_fRadius;
	SKVector3 Temp2 = Sphere.m_Center +  V2 * Sphere.m_fRadius;

	Temp0 = Temp0 * Mat;
	Temp1 = Temp1 * Mat;
	Temp2 = Temp2 * Mat;

	SKREAL fRadiusTemp;

	m_fRadius = (Temp0 - m_Center).GetLength();

	fRadiusTemp = (Temp1 - m_Center).GetLength();

	if(m_fRadius < fRadiusTemp)
		m_fRadius = fRadiusTemp;

	fRadiusTemp = (Temp2 - m_Center).GetLength();

	if(m_fRadius < fRadiusTemp)
		m_fRadius = fRadiusTemp;
}
SKAABB3 SKSphere3::GetAABB()const
{
	SKVector3 Max = m_Center + SKVector3(m_fRadius,m_fRadius,m_fRadius);
	SKVector3 Min = m_Center + SKVector3(-m_fRadius,-m_fRadius,-m_fRadius);
	return SKAABB3(Max,Min);
}