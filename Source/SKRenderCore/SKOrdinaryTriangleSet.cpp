#include "SKOrdinaryTriangleSet.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOrdinaryTriangleSet,SKTriangleSet)
BEGIN_ADD_PROPERTY(SKOrdinaryTriangleSet,SKTriangleSet)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOrdinaryTriangleSet)
IMPLEMENT_INITIAL_END
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet()
{

}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(const SKAABB3 & AABB)
{
	SKVector3 VertexDate[8];
	AABB.GetPoint(VertexDate);
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	pVertex->SetDate(VertexDate,8,SKDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);
	SKUSHORT_INDEX Index[36];
	//ǰ2
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;
	
	//2
	Index[6]=4;   Index[7]=6;   Index[8]=5;
	Index[9]=4;   Index[10]=7;  Index[11]=6;
	
	//Ҳ2
	Index[12]=0;  Index[13]=3;  Index[14]=7;
	Index[15]=0;  Index[16]=7;  Index[17]=4;
	//2
	Index[18]=1;  Index[19]=5;  Index[20]=6;
	Index[21]=1;  Index[22]=6;  Index[23]=2;
	//2	
	Index[24]=0;  Index[25]=5;  Index[26]=1;
	Index[27]=0;  Index[28]=4;  Index[29]=5;
	//2
	Index[30]=2;  Index[31]=6;  Index[32]=7;
	Index[33]=2;  Index[34]=7;  Index[35]=3;
	
	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->SetDate(Index,36,SKDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();
	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);

}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(const SKOBB3 & OBB)
{
	SKVector3 VertexDate[8];
	OBB.GetPoint(VertexDate);
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	pVertex->SetDate(VertexDate,8,SKDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);
	SKUSHORT_INDEX Index[36];
	//ǰ2
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;

	//2
	Index[6]=4;   Index[7]=6;   Index[8]=5;
	Index[9]=4;   Index[10]=7;  Index[11]=6;

	//Ҳ2
	Index[12]=0;  Index[13]=3;  Index[14]=7;
	Index[15]=0;  Index[16]=7;  Index[17]=4;
	//2
	Index[18]=1;  Index[19]=5;  Index[20]=6;
	Index[21]=1;  Index[22]=6;  Index[23]=2;
	//2	
	Index[24]=0;  Index[25]=5;  Index[26]=1;
	Index[27]=0;  Index[28]=4;  Index[29]=5;
	//2
	Index[30]=2;  Index[31]=6;  Index[32]=7;
	Index[33]=2;  Index[34]=7;  Index[35]=3;

	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->SetDate(Index,36,SKDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(const SKPlane3 &Plane)
{
	SKMatrix3X3W Mat;
	Mat.CreateFromLookDir(Plane.GetPoint(),Plane.GetN());

	SKVector3 A0(Mat.M[0][0],Mat.M[0][1],Mat.M[0][2]);
	SKVector3 A1(Mat.M[1][0],Mat.M[1][1],Mat.M[1][2]);

	SKVector3 VertexDate[4];
	VertexDate[0] = Plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexDate[1] = Plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexDate[2] = Plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexDate[3] = Plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;

	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	pVertex->SetDate(VertexDate,4,SKDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);

	SKUSHORT_INDEX Index[6];

	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;

	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->SetDate(Index,6,SKDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();
	
	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(const SKTriangle3 &Triangle)
{
	SKVector3 VertexDate[3];
	Triangle.GetPoint(VertexDate);
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	pVertex->SetDate(VertexDate,3,SKDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);

	SKUSHORT_INDEX Index[3];
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->SetDate(Index,3,SKDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(const SKRectangle3 &Rectangle)
{
	SKVector3 VertexDate[4];
	Rectangle.GetPoint(VertexDate);
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	pVertex->SetDate(VertexDate,4,SKDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);

	SKUSHORT_INDEX Index[6];

	Index[0]=0;  Index[1]=2;  Index[2]=1;
	Index[3]=0;  Index[4]=3;  Index[5]=2;

	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->SetDate(Index,6,SKDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(const SKPolygon3 &Polyon)
{
	SKMAC_ASSERT(Polyon.GetpPoint() && Polyon.GetPointNum());

	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	pVertex->SetDate(Polyon.GetpPoint(),Polyon.GetPointNum(),SKDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);


	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->SetDate(Polyon.GetpIndex(),Polyon.GetIndexNum(),SKDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(const SKSphere3 & Sphere)
{
	SKREAL LongitudeAngle = 10;//ϵĻֽ
	SKREAL LatitudeAngle = 10;//άϵĻֽ

	SKREAL AnglePI = 180.0f;
	unsigned int LongitudeNum = 0;
	while(AnglePI > 0)
	{
		LongitudeNum++ ;
		AnglePI -= LongitudeAngle;
	}
	LongitudeNum++;
	unsigned int LatitudeNum = unsigned int(360.0f / LatitudeAngle) + 1;

	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	pVertex->CreateEmptyBuffer(LongitudeNum * LatitudeNum,SKDataBuffer::DT_FLOAT32_3);

	SKVector3 * pVertexDate = (SKVector3 *)pVertex->GetDate();
	SKREAL i = 0 ;
	for(unsigned int k = 0 ; k < LongitudeNum ; k++)
	{
		if(i > 180)
			i = 180;
		SKREAL iRadian = AngleToRadian(i);
		for(SKREAL j = 0 ; j <= 360.0f; j = j + LatitudeAngle)
		{


			SKREAL jRadian = AngleToRadian(j);
			pVertexDate->x = Sphere.m_fRadius * SIN(iRadian) * COS(jRadian);
			pVertexDate->z = Sphere.m_fRadius * SIN(iRadian) * SIN(jRadian);
			pVertexDate->y = Sphere.m_fRadius * COS(iRadian);

			*pVertexDate += Sphere.m_Center;
			pVertexDate++;
		}
		i = i + LongitudeAngle;
	}
	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);

	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->CreateEmptyBuffer((LongitudeNum - 1) * LatitudeNum * 2 * 3,SKDataBuffer::DT_USHORT);
	SKUSHORT_INDEX * pIndexData = (SKUSHORT_INDEX *)pIndexDateBuffer->GetDate();
	for (unsigned int i = 0 ; i < LongitudeNum - 1 ; i++)
	{
		unsigned int z_width_poly= i * LatitudeNum ;
		unsigned int z_width = i * LatitudeNum;
		for(unsigned int j = 0 ; j < LatitudeNum ; j++)
		{
			unsigned int z_width_x = z_width + j;
			unsigned int z_width_xX2 = (z_width_poly + j) * 2;
			if(j != LatitudeNum - 1)
			{
				pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
				pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
				pIndexData[z_width_xX2 * 3 + 1] = z_width_x + 1;

				pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1 + LatitudeNum;
				pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
			}
			else
			{
				pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
				pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
				pIndexData[z_width_xX2 * 3 + 1] = z_width_x - LatitudeNum + 1;

				pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x - LatitudeNum + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
			}

		}
	}

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
SKOrdinaryTriangleSet::~SKOrdinaryTriangleSet()
{

}
SKOrdinaryTriangleSet::SKOrdinaryTriangleSet(SKSurface3 * pSurface ,SKSurface3Subdivision * pSubidvision)
{
	SKMAC_ASSERT(pSubidvision);
	SKMAC_ASSERT(pSurface);
	if(!pSubidvision->GetPoints())
	{
		pSubidvision->SetSurface(pSurface);
		bool bResult = pSubidvision->Subdivision();
		SKMAC_ASSERT(bResult);
	}
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;
	unsigned int Row = pSubidvision->GetRow();
	unsigned int Column = pSubidvision->GetColumn();
	pVertex->SetDate(pSubidvision->GetPoints(), Row * Column,SKDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);

	SKDataBuffer *pIndexDateBuffer = SK_NEW SKDataBuffer;
	pIndexDateBuffer->CreateEmptyBuffer((Row - 1) * (Column - 1) * 2 * 3,SKDataBuffer::DT_USHORT);
	SKUSHORT_INDEX * pIndexData = (SKUSHORT_INDEX *)pIndexDateBuffer->GetDate();

	for (unsigned int i = 0 ; i < Row - 1 ; i++)
	{
		unsigned int z_width_poly= i * (Column - 1);
		unsigned int z_width = i * Column;
		for(unsigned int j = 0 ; j < Column - 1 ; j++)
		{
			unsigned int z_width_x = z_width + j;
			unsigned int z_width_xX2 = (z_width_poly + j) * 2;
			pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
			pIndexData[z_width_xX2 * 3 + 2] = z_width_x + Column;
			pIndexData[z_width_xX2 * 3 + 1] = z_width_x + 1;

			pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1 + Column;
			pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x + 1;
			pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + Column;

		}
	}
	
	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = SK_NEW SKIndexBuffer();

	}

	m_pIndexBuffer->SetDate(pIndexDateBuffer);
}
