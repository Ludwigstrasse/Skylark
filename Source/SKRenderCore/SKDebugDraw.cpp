#include "SKDebugDraw.h"
#include "SKAABB3.h"
#include "SKSphere3.h"
#include "SKOBB3.h"
#include "SKMaterialPass.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
SKDebugDraw::SKDebugDraw()
{


	SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> ForamtArray;
	SKVertexFormat::VERTEXFORMAT_TYPE Pos;
	Pos.DateType = SKDataBuffer::DT_FLOAT32_3;
	Pos.OffSet = 0;
	Pos.Semantics = SKVertexFormat::VF_POSITION;
	Pos.SemanticsIndex = 0;
	ForamtArray.AddElement(Pos);

	SKVertexFormat::VERTEXFORMAT_TYPE Color;
	Color.DateType = SKDataBuffer::DT_COLOR;
	Color.OffSet = 12;
	Color.Semantics = SKVertexFormat::VF_COLOR;
	Color.SemanticsIndex = 0;
	ForamtArray.AddElement(Color);


	m_pDrawVertexFormat = SKResourceManager::LoadVertexFormat(NULL, &ForamtArray);

	m_pOnlyVertexColor = SKMaterialR::Create((SKMaterial *)SKMaterial::GetDefaultOnlyVertexColor());
	m_pOnlyVertexColorDisableDepth = SKMaterialR::Create((SKMaterial *)SKMaterial::GetDefaultOnlyVertexColorDisableDepth());

	m_bEnable = true;
}
SKDebugDraw::~SKDebugDraw()
{
	m_pOnlyVertexColor = NULL;
	m_pOnlyVertexColorDisableDepth = NULL;

	m_pDrawVertexFormat = NULL;
}

void SKDebugDraw::AddDebugLine(const SKVector3 & P1,const SKVector3 & P2,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}

	DebugVertexType V[2];
	V[0].Pos = P1;
	V[0].Color = Color;

	V[1].Pos = P2;
	V[1].Color = Color;
	if (bDepth)
	{
		DepthDebugLineArray.AddElement(V[0]);
		DepthDebugLineArray.AddElement(V[1]);

	}
	else
	{
		NoDepthDebugLineArray.AddElement(V[0]);
		NoDepthDebugLineArray.AddElement(V[1]);

	}

}
void SKDebugDraw::AddDebugTriangle(const SKVector3 & P1,const SKVector3 & P2,const SKVector3 &P3,
								  const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
}
void SKDebugDraw::AddDebugLineAABB(const SKAABB3 & AABB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	SKVector3 Point[8];
	AABB.GetPoint(Point);
	AddDebugLine(Point[0],Point[1],Color,bDepth);
	AddDebugLine(Point[1],Point[2],Color,bDepth);
	AddDebugLine(Point[2],Point[3],Color,bDepth);
	AddDebugLine(Point[3],Point[0],Color,bDepth);
	AddDebugLine(Point[4],Point[5],Color,bDepth);
	AddDebugLine(Point[5],Point[6],Color,bDepth);
	AddDebugLine(Point[6],Point[7],Color,bDepth);
	AddDebugLine(Point[7],Point[4],Color,bDepth);
	AddDebugLine(Point[0],Point[4],Color,bDepth);
	AddDebugLine(Point[1],Point[5],Color,bDepth);
	AddDebugLine(Point[2],Point[6],Color,bDepth);
	AddDebugLine(Point[3],Point[7],Color,bDepth);
}
void SKDebugDraw::AddDebugLineSphere(const SKSphere3 & Sphere,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	for (unsigned int i = 0 ; i < 360 ; i++)
	{
		SKVector3 Point1 = Sphere.m_Center + SKVector3(GetFastSin(i),GetFastCos(i),0.0f) * Sphere.m_fRadius;	
		SKVector3 Point2 = Sphere.m_Center + SKVector3(GetFastSin(i + 1),GetFastCos(i + 1),0.0f) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
	for (unsigned int i = 0 ; i < 360 ; i++)
	{
		SKVector3 Point1 = Sphere.m_Center + SKVector3(GetFastSin(i),0.0f,GetFastCos(i)) * Sphere.m_fRadius;	
		SKVector3 Point2 = Sphere.m_Center + SKVector3(GetFastSin(i + 1),0.0f,GetFastCos(i + 1)) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
	for (unsigned int i = 0 ; i < 360 ; i++)
	{
		SKVector3 Point1 = Sphere.m_Center + SKVector3(0.0f,GetFastSin(i),GetFastCos(i)) * Sphere.m_fRadius;	
		SKVector3 Point2 = Sphere.m_Center + SKVector3(0.0f,GetFastSin(i + 1),GetFastCos(i + 1)) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
}
void SKDebugDraw::AddDebugLineOBB(const SKOBB3 & OBB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	SKVector3 Point[8];
	OBB.GetPoint(Point);
	AddDebugLine(Point[0],Point[1],Color,bDepth);
	AddDebugLine(Point[1],Point[2],Color,bDepth);
	AddDebugLine(Point[2],Point[3],Color,bDepth);
	AddDebugLine(Point[3],Point[0],Color,bDepth);
	AddDebugLine(Point[4],Point[5],Color,bDepth);
	AddDebugLine(Point[5],Point[6],Color,bDepth);
	AddDebugLine(Point[6],Point[7],Color,bDepth);
	AddDebugLine(Point[7],Point[4],Color,bDepth);
	AddDebugLine(Point[0],Point[4],Color,bDepth);
	AddDebugLine(Point[1],Point[5],Color,bDepth);
	AddDebugLine(Point[2],Point[6],Color,bDepth);
	AddDebugLine(Point[3],Point[7],Color,bDepth);

}
void SKDebugDraw::AddDebugTriangleAABB(const SKAABB3 & AABB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	SKVector3 Point[8];
	AABB.GetPoint(Point);

}
void SKDebugDraw::AddDebugTriangleSphere(const SKSphere3 & Sphere,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}

}
void SKDebugDraw::AddDebugTriangleOBB(const SKOBB3 & OBB,const DWORD &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	SKVector3 Point[8];
	OBB.GetPoint(Point);

}
void SKDebugDraw::DrawDebugInfo(SKCamera * pCamera)
{
	if (!m_bEnable)
	{
		return ;
	}

	
	SKDVGeometry *  pBuffer = SKResourceManager::GetDVGeometry(m_pDrawVertexFormat, SKMeshDate::MDT_LINE, DepthDebugLineArray.GetNum() + NoDepthDebugLineArray.GetNum());

	if (pBuffer)
	{
		pBuffer->Add(DepthDebugLineArray.GetBuffer(), sizeof(DebugVertexType)* DepthDebugLineArray.GetNum());
		pBuffer->ClearAllMaterialInstance();
		pBuffer->AddMaterialInstance(m_pOnlyVertexColor);
		pBuffer->Draw(pCamera);

		pBuffer->Add(NoDepthDebugLineArray.GetBuffer(), sizeof(DebugVertexType)* NoDepthDebugLineArray.GetNum());
		pBuffer->ClearAllMaterialInstance();
		pBuffer->AddMaterialInstance(m_pOnlyVertexColorDisableDepth);
		pBuffer->Draw(pCamera);
	}
	

	DepthDebugLineArray.Clear();
	NoDepthDebugLineArray.Clear();

}