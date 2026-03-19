#include "SKMath.h"
#include "SKOBB3.h"
#include "SKMatrix3X3.h"
#include "SKLine3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKOBB3::SKOBB3()
{


}
/*----------------------------------------------------------------*/
SKOBB3::~SKOBB3()
{


}
/*----------------------------------------------------------------*/
SKOBB3::SKOBB3(const SKVector3 A[3],SKREAL fA[3],const SKVector3 & Center)
{
	Set(A,fA,Center);

}
/*----------------------------------------------------------------*/
SKOBB3::SKOBB3(const SKVector3 &A0,const SKVector3 &A1,const SKVector3 &A2,
			SKREAL fA0,SKREAL fA1,SKREAL fA2,
			 const SKVector3 & Center)
{
	Set(A0,A1,A2,fA0,fA1,fA2,Center);

}
/*----------------------------------------------------------------*/
void SKOBB3::Transform(const SKOBB3 &OBB, 
						const SKMatrix3X3W &Mat)
{

	m_Center = OBB.m_Center * Mat;
	m_A[0]     = Mat.Apply3X3(OBB.m_A[0]);
	m_A[1]     = Mat.Apply3X3(OBB.m_A[1]);
	m_A[2]     = Mat.Apply3X3(OBB.m_A[2]);

	m_fA[0] = OBB.m_fA[0] * m_A[0].GetLength();
	m_fA[1] = OBB.m_fA[1] * m_A[1].GetLength();
	m_fA[2] = OBB.m_fA[2] * m_A[2].GetLength();

	m_A[0].Normalize();
	m_A[1].Normalize();
	m_A[2].Normalize();

}
/*----------------------------------------------------------------*/ 
void SKOBB3::GetTransform(SKMatrix3X3W &m)const
{
	m.CreateInWorldObject(m_A[0],m_A[1],m_A[2],m_Center);

}
/*----------------------------------------------------------------*/
void SKOBB3::GetRectangle(SKRectangle3 Rectangle[6])const
{
	SKVector3 Center[6];
	Center[0] = m_Center + m_A[0] * m_fA[0];
	Center[1] = m_Center + m_A[0] * (-m_fA[0]);
	Center[2] = m_Center + m_A[1] * m_fA[1];
	Center[3] = m_Center + m_A[1] * (-m_fA[1]);
	Center[4] = m_Center + m_A[2] * m_fA[2];
	Center[5] = m_Center + m_A[2] * (-m_fA[2]);
	//right
	Rectangle[0].Set(Center[0],m_A[1],m_A[2],m_fA[1],m_fA[2]);
	//left
	Rectangle[1].Set(Center[1],m_A[1],m_A[2] * (-1),m_fA[1],m_fA[2]);

	//up
	Rectangle[2].Set(Center[4],m_A[2],m_A[0],m_fA[2],m_fA[1]);
	//down
	Rectangle[3].Set(Center[5],m_A[2],m_A[0] * (-1),m_fA[2],m_fA[0]);

	//front
	Rectangle[4].Set(Center[2],m_A[1],m_A[0] * (-1),m_fA[1],m_fA[0]);
	//back    
	Rectangle[5].Set(Center[3],m_A[1],m_A[0],m_fA[1],m_fA[0]);
	/**/
}
/*----------------------------------------------------------------*/
void SKOBB3::GetPlane(SKPlane3 pPlanes[6])const
{
	SKVector3 Max,Min;
	Max = m_Center + m_A[0] * m_fA[0] + m_A[1] * m_fA[1] + m_A[2] * m_fA[2];
	Min = m_Center + m_A[0] * (-m_fA[0]) + m_A[1] * (-m_fA[1]) + m_A[2] * (-m_fA[2]);

	SKVector3 vcN;

	// 
	vcN = m_A[0];
	pPlanes[0].Set(vcN, Max);

	// 
	vcN = m_A[0] * (-1.0f);
	pPlanes[1].Set(vcN, Min);

	// ǰ
	vcN = m_A[2] * (-1.0f);
	pPlanes[2].Set(vcN, Min);

	// 
	vcN = m_A[2];
	pPlanes[3].Set(vcN, Max);

	// 
	vcN = m_A[1];
	pPlanes[4].Set(vcN, Max);

	// 
	vcN = m_A[1] * (-1.0f);
	pPlanes[5].Set(vcN, Min);
}
/*----------------------------------------------------------------*/
void SKOBB3::GetPoint(SKVector3 Point[8])const
{
	SKVector3 Temp[6];
	Temp[0] =  m_A[0] * m_fA[0];
	Temp[1] =  m_A[0] * (-m_fA[0]);

	Temp[2] =  m_A[1] * m_fA[1];
	Temp[3] =  m_A[1] * (-m_fA[1]);

	Temp[4] =  m_A[2] * m_fA[2];
	Temp[5] =  m_A[2] * (-m_fA[2]);

	SKVector3 Temp1[6];
	Temp1[0] = m_Center + Temp[0];
	Temp1[1] = m_Center + Temp[1];

	Temp1[2] = Temp1[0] + Temp[2];
	Temp1[3] = Temp1[1] + Temp[2];
	Temp1[4] = Temp1[1] + Temp[3];
	Temp1[5] = Temp1[0] + Temp[3];

	Point[0] = Temp1[2] + Temp[4];
	Point[1] = Temp1[3] + Temp[4];
	Point[2] = Temp1[4] + Temp[4];
	Point[3] = Temp1[5] + Temp[4];

	Point[4] = Temp1[2] + Temp[5];
	Point[5] = Temp1[3] + Temp[5];
	Point[6] = Temp1[4] + Temp[5];
	Point[7] = Temp1[5] + Temp[5];
}
/*----------------------------------------------------------------*/
bool SKOBB3::GetParameter(const SKVector3 &Point,SKREAL fOBBParameter[3])const
{
	SKMatrix3X3W m;
	GetTransform(m);
	SKVector3 Temp = Point * m;
	fOBBParameter[0] = Temp.x;
	fOBBParameter[1] = Temp.y;
	fOBBParameter[2] = Temp.z;
	if(ABS(Temp.x) > m_fA[0] || ABS(Temp.y) > m_fA[1] || ABS(Temp.z) > m_fA[2])
		return 0;
	else
		return 1;	
}
/*----------------------------------------------------------------*/
void SKOBB3::CreateOBB(const SKVector3 * const pPointArray,unsigned int uiPointNum)
{
	if(!pPointArray || !uiPointNum)
		return;
	SKVector3 Sum = pPointArray[0];
	for(unsigned int i = 1 ; i < uiPointNum ; i++)
	{
		Sum += pPointArray[i];
	}
	m_Center = Sum / (uiPointNum * 1.0f);
	SKMatrix3X3 MatSum,MatTemp;
	MatSum.SetZero();

	
	for(unsigned int i = 0 ; i < uiPointNum ; i++)
	{
		SKVector3 Diff = pPointArray[i] - m_Center;
		MatTemp.CreateFromTwoVector(Diff,Diff);
		MatSum +=MatTemp;

			

	}
	SKREAL InvNun = 1.0f / uiPointNum ;
	MatSum *= InvNun;
	SKREAL EigenValue[3];
	MatSum.GetEigenSystem(EigenValue,m_A);

	SKVector3 Min, Max;
	SKVector3 Diff = pPointArray[0] - m_Center;
	for(int j = 0 ; j < 3 ; j++)
	{
		Min.m[j] = m_A[j].Dot(Diff);
		Max.m[j] = Min.m[j];
	}
	for(unsigned int i = 1 ; i < uiPointNum ; i++)
	{
		Diff = pPointArray[i] - m_Center;
		for(int j = 0 ; j < 3 ; j++)
		{
			SKREAL temp = m_A[j].Dot(Diff);
			if(temp < Min.m[j])
			{
				Min.m[j] = temp;
			}
			else if(temp > Max.m[j])
			{
				Max.m[j] = temp;
			}
		}
	}

	for(int j = 0 ; j < 3 ; j++)
	{
		m_Center += m_A[j] * (0.5f * ( Min.m[j] + Max.m[j]));
		m_fA[j] = 0.5f * (Max.m[j] - Min.m[j]);
	
	}
} 
/*----------------------------------------------------------------*/
SKOBB3 SKOBB3::MergOBB(const SKOBB3 &OBB)const
{
	SKOBB3 Temp;
	Temp.m_Center = (OBB.m_Center + m_Center) * 0.5f;
	SKMatrix3X3 m1,m2;
	m1.CreateRot(m_A[0],m_A[1],m_A[2]);
	m2.CreateRot(OBB.m_A[0],OBB.m_A[1],OBB.m_A[2]);

	SKQuat q,q1,q2;
	q1 = m1.GetQuat();
	q2 = m2.GetQuat();
	if(q1.Dot(q2) < 0)
	{
		q2 *= -1.0f;
	}
	q = q1 + q2;
	q.Normalize();

	SKMatrix3X3 Mat;

	q.GetMatrix(Mat);
	Mat.GetUVN(Temp.m_A);

	//m_Center = (OBB1.m_Center + OBB2.m_Center) / 2.0f;

	SKVector3 Point[8];

	GetPoint(Point);
	SKVector3 Diff;
	Diff = Point[0] - Temp.m_Center;
	SKVector3 Min = SKVector3(Diff.Dot(Temp.m_A[0]), Diff.Dot(Temp.m_A[1]), Diff.Dot(Temp.m_A[2]));//= Point[0];
	SKVector3 Max = Min;
	
	for(int i = 1 ; i < 8 ; i++)
	{
		Diff = Point[i] - Temp.m_Center;
		for(int j = 0 ; j < 3 ; j++)
		{
			SKREAL fDot = Diff.Dot(Temp.m_A[j]);
			if (fDot > Max.m[j])
			{
				Max.m[j] = fDot;
			}
			else if (fDot < Min.m[j])
			{
				Min.m[j] = fDot;
			}
		}

	}
	OBB.GetPoint(Point);
	for(int i = 1 ; i < 8 ; i++)
	{
		Diff = Point[i] - Temp.m_Center;
		for(int j = 0 ; j < 3 ; j++)
		{
			SKREAL fDot = Diff.Dot(Temp.m_A[j]);
			if (fDot > Max.m[j])
			{
				Max.m[j] = fDot;
			}
			else if (fDot < Min.m[j])
			{
				Min.m[j] = fDot;
			}
		}

	}
	for(int i = 0 ; i < 3 ; i++)
	{
		Temp.m_Center += Temp.m_A[i] * ((Min.m[i] + Max.m[i]) * 0.5f);
		Temp.m_fA[i] = 0.5f * (Max.m[i] - Min.m[i]);
	}
	return Temp;
}
SKAABB3 SKOBB3::GetAABB()const
{
	SKVector3 Point[8];
	GetPoint(Point);
	SKAABB3 Temp;
	Temp.CreateAABB(Point,8);
	return Temp;
}