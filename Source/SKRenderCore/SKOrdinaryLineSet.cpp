#include "SKOrdinaryLineSet.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOrdinaryLineSet,SKLineSet)
BEGIN_ADD_PROPERTY(SKOrdinaryLineSet,SKLineSet)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOrdinaryLineSet)
IMPLEMENT_INITIAL_END
SKOrdinaryLineSet::SKOrdinaryLineSet()
{

}
SKOrdinaryLineSet::~SKOrdinaryLineSet()
{

}
SKOrdinaryLineSet::SKOrdinaryLineSet(SKControlCurve3 * pControlCurve,SKCurve3Subdivision * pSubidvision)
{
	SKMAC_ASSERT(pSubidvision && pControlCurve);
	if(!pSubidvision->GetPoints())
	{
		pSubidvision->SetSubCurve(pControlCurve);
		pSubidvision->Subdivision();
	}
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;

	pVertex->SetDate(pSubidvision->GetPoints(), pSubidvision->GetPointNum(),SKDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);

	bool bResult = CreateIndex(LT_OPEN);
	SKMAC_ASSERT(bResult);
}
SKOrdinaryLineSet::SKOrdinaryLineSet(const SKLine3 & Line)
{
	SKVector3 Verts[2];
	Verts[0] = Line.GetOrig() - Line.GetDir() * DRAW_MATH_ELEMENT_LENGTH;
	Verts[1] = Line.GetOrig() + Line.GetDir() * DRAW_MATH_ELEMENT_LENGTH;

	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;

	pVertex->SetDate(Verts, 2,SKDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	SKMAC_ASSERT(bResult);
}
SKOrdinaryLineSet::SKOrdinaryLineSet(const SKRay3 & Ray)
{
	SKVector3 Verts[2];
	Verts[0] = Ray.GetOrig();
	Verts[1] = Verts[0] + Ray.GetDir() * DRAW_MATH_ELEMENT_LENGTH;
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;

	pVertex->SetDate(Verts, 2,SKDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	SKMAC_ASSERT(bResult);
}
SKOrdinaryLineSet::SKOrdinaryLineSet(const SKSegment3 & Segment)
{
	SKVector3 Verts[2];
	Verts[0] = Segment.GetOrig();
	Verts[1] = Segment.GetEnd();
	SKDataBuffer * pVertex = SK_NEW SKDataBuffer;

	pVertex->SetDate(Verts, 2,SKDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = SK_NEW SKVertexBuffer(true);
	m_pVertexBuffer->SetDate(pVertex,SKVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	SKMAC_ASSERT(bResult);

}
