#include "SKGeometry.h"
#include "SKBoneNode.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKTriangleSet.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKGeometry,SKSpatial)
BEGIN_ADD_PROPERTY(SKGeometry,SKSpatial)
REGISTER_PROPERTY(m_LocalBV, LocalBV, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMeshDate,MeshData,SKProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pMorphMeshDate, MorphMeshData, SKProperty::F_CLONE)
REGISTER_PROPERTY(m_BoneName, BoneName, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_GeometryName, GeometryName, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMaterialInstance, MaterialInstance, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME );
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKGeometry)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
SKPointer<SKGeometry> SKGeometry::ms_Quad = NULL;
SKPointer<SKGeometry> SKGeometry::ms_DefaultCub = NULL;
SKPointer<SKGeometry> SKGeometry::ms_DefaultCubCone = NULL;
SKPointer<SKGeometry> SKGeometry::ms_DefaultCone = NULL;
SKPointer<SKGeometry> SKGeometry::ms_DefaultRenderCube = NULL;
bool SKGeometry::InitialDefaultState()
{
	ms_Quad = SK_NEW SKGeometry();
	if (!ms_Quad)
	{
		return false;
	}
	
	ms_DefaultCub = SK_NEW SKGeometry();
	if (!ms_DefaultCub)
	{
		return false;
	}

	ms_DefaultCubCone = SK_NEW SKGeometry();
	if (!ms_DefaultCubCone)
	{
		return false;
	}

	ms_DefaultCone = SK_NEW SKGeometry();
	if (!ms_DefaultCone)
	{
		return false;
	}
	ms_DefaultRenderCube = SK_NEW SKGeometry();
	if (!ms_DefaultRenderCube)
	{
		return false;
	}
	LoadDefault();
	return 1;
}
bool SKGeometry::TerminalDefaultState()
{
	ms_Quad = NULL;
	ms_DefaultCub = NULL;
	ms_DefaultCubCone = NULL;
	ms_DefaultCone = NULL;
	ms_DefaultRenderCube = NULL;
	return 1;
}
void SKGeometry::LoadDefault()
{
	//Quad
	{
		SKArray<SKVector3> VertexArray;
		SKArray<SKVector2> m_TexCoordArray;
		SKArray<SKUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(SKVector3(-1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(SKVector3( 1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(SKVector3( 1.0f, -1.0f, 0.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, 0.0f));

		m_TexCoordArray.AddElement(SKVector2(0.0f, 0.0f));
		m_TexCoordArray.AddElement(SKVector2(1.0f, 0.0f));
		m_TexCoordArray.AddElement(SKVector2(1.0f, 1.0f));
		m_TexCoordArray.AddElement(SKVector2(0.0f, 1.0f));

		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		SKDataBufferPtr  pVertexDate = SK_NEW SKDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),SKDataBuffer::DT_FLOAT32_3);

		SKDataBufferPtr pTexcoord = SK_NEW SKDataBuffer;
		pTexcoord->SetDate(&m_TexCoordArray[0],(unsigned int)m_TexCoordArray.GetNum(),SKDataBuffer::DT_FLOAT32_2);

		SKDataBufferPtr pIndex = SK_NEW SKDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),SKDataBuffer::DT_USHORT);

		//BUFFER
		SKVertexBufferPtr pVertexBuffer = SK_NEW SKVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,SKVertexFormat::VF_POSITION);
		pVertexBuffer->SetDate(pTexcoord,SKVertexFormat::VF_TEXCOORD);

		SKIndexBufferPtr pIndexBuffer = SK_NEW SKIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		SKTriangleSetPtr pTriangleSetDate = SK_NEW SKTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_Quad->SetMeshDate(pTriangleSetDate);
		ms_Quad->m_GeometryName = _T("DefaultQuad");
	}
	

	//Cub
	{
		SKArray<SKVector3> VertexArray;

		SKArray<SKUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(SKVector3(-1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(SKVector3( 1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(SKVector3( 1.0f, -1.0f, 0.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, 0.0f));

		VertexArray.AddElement(SKVector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(SKVector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(SKVector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(5);
		IndexArray.AddElement(4);
		IndexArray.AddElement(7);
		IndexArray.AddElement(6);

		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(7);
		IndexArray.AddElement(4);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(7);

		//right
		IndexArray.AddElement(1);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(1);
		IndexArray.AddElement(6);
		IndexArray.AddElement(2);

		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(0);
		IndexArray.AddElement(5);
		IndexArray.AddElement(1);

		//bottom
		IndexArray.AddElement(3);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(6);

		SKDataBufferPtr  pVertexDate = SK_NEW SKDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),SKDataBuffer::DT_FLOAT32_3);

		SKDataBufferPtr pIndex = SK_NEW SKDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),SKDataBuffer::DT_USHORT);

		//BUFFER
		SKVertexBufferPtr pVertexBuffer = SK_NEW SKVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,SKVertexFormat::VF_POSITION);

		SKIndexBufferPtr pIndexBuffer = SK_NEW SKIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		SKTriangleSetPtr pTriangleSetDate = SK_NEW SKTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCub->SetMeshDate(pTriangleSetDate);
		ms_DefaultCub->m_GeometryName = _T("DefaultCub");
	}

	//Cub Cone
	{
		SKArray<SKVector3> VertexArray;

		SKArray<SKUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(SKVector3( 0.0f,  0.0f, 0.0f));
		

		VertexArray.AddElement(SKVector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(SKVector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(SKVector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(1);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(4);
		IndexArray.AddElement(3);


		//right
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(1);


		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);


		//bottom
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(4);


		SKDataBufferPtr  pVertexDate = SK_NEW SKDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),SKDataBuffer::DT_FLOAT32_3);

		SKDataBufferPtr pIndex = SK_NEW SKDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),SKDataBuffer::DT_USHORT);

		//BUFFER
		SKVertexBufferPtr pVertexBuffer = SK_NEW SKVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,SKVertexFormat::VF_POSITION);

		SKIndexBufferPtr pIndexBuffer = SK_NEW SKIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		SKTriangleSetPtr pTriangleSetDate = SK_NEW SKTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCubCone->SetMeshDate(pTriangleSetDate);
		ms_DefaultCubCone->m_GeometryName = _T("DefaultCubCone");
	}

	//Cone
	{
		unsigned int uiLevel = 1;
		unsigned int CircleLevel = 5;


		SKArray<SKVector3> VertexArray;

		SKArray<SKUSHORT_INDEX> IndexArray;

		VertexArray.AddElement(SKVector3( 0.0f,  0.0f, 0.0f));


		SKREAL RangeStep = 1.0f / uiLevel;
		for (unsigned int i = 0 ; i < uiLevel ; i++)
		{
			SKREAL Z = RangeStep * (i + 1);
			for (unsigned int j = 0 ; j < CircleLevel ; j++)
			{
				SKREAL Radio = ( j * 1.0f ) / CircleLevel * VS2PI;
				SKREAL X = Z * COS(Radio);
				SKREAL Y = Z * SIN(Radio);
				VertexArray.AddElement(SKVector3(X,Y,Z));
			}
		}
		VertexArray.AddElement(SKVector3(0.0f,0.0f, 1.0f));
		

		// first level
		for (unsigned int i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(0);

			unsigned int Index1 = i + 2;
			unsigned int Index2 = i + 1;
			if (Index1 = CircleLevel + 1)
			{
				Index1 = 1;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		//
		for (unsigned int i = 1 ; i < uiLevel ; i++)
		{
			for (unsigned int j = 0 ; j < CircleLevel ; j++)
			{
				unsigned int TopIndex1 = (i - 1) * CircleLevel + 1 + j;
				unsigned int TopIndex2 = (i - 1) * CircleLevel + 1 + j + 1;
				
				
				
				unsigned int BottomIndex1 = i * CircleLevel + 1 + j;
				unsigned int BottomIndex2 = i * CircleLevel + 1 + j + 1;

				if (j + 1 == CircleLevel)
				{
					TopIndex2 = (i - 1) * CircleLevel + 1 ;
					BottomIndex2 = i * CircleLevel + 1;
				}

				IndexArray.AddElement(TopIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(BottomIndex2);
			}
		}

		// last level
		for (unsigned int i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(VertexArray.GetNum() - 1);

			unsigned int Index1 = VertexArray.GetNum() - 3 - i;
			unsigned int Index2 = VertexArray.GetNum() - 2 - i;
			
			if (i == CircleLevel - 1)
			{
				Index1 = VertexArray.GetNum() - 2;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		

		SKDataBufferPtr  pVertexDate = SK_NEW SKDataBuffer;
		pVertexDate->SetDate(&VertexArray[0],(unsigned int)VertexArray.GetNum(),SKDataBuffer::DT_FLOAT32_3);

		SKDataBufferPtr pIndex = SK_NEW SKDataBuffer;
		pIndex->SetDate(&IndexArray[0],(unsigned int)IndexArray.GetNum(),SKDataBuffer::DT_USHORT);

		//BUFFER
		SKVertexBufferPtr pVertexBuffer = SK_NEW SKVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate,SKVertexFormat::VF_POSITION);

		SKIndexBufferPtr pIndexBuffer = SK_NEW SKIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		SKTriangleSetPtr pTriangleSetDate = SK_NEW SKTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCone->SetMeshDate(pTriangleSetDate);
		ms_DefaultCone->m_GeometryName = _T("DefaultCone");
	}

	//Quad Cub
	{
		SKArray<SKVector3> VertexArray;
		SKArray<SKVector2> TexcoordArray;
		SKArray<SKVector3> NormalArray;
		SKArray<SKUSHORT_INDEX> IndexArray;

		//pos uv
		VertexArray.AddElement(SKVector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, 1.0f));

		TexcoordArray.AddElement(SKVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(SKVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 0.0f));

		NormalArray.AddElement(SKVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(SKVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(SKVector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(SKVector3(0.0f, 0.0f, 1.0f));

		//pos uv
		VertexArray.AddElement(SKVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, -1.0f));

		TexcoordArray.AddElement(SKVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(SKVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 0.0f));

		NormalArray.AddElement(SKVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(SKVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(SKVector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(SKVector3(0.0f, 0.0f, -1.0f));

		//pos uv
		VertexArray.AddElement(SKVector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, 1.0f));

		TexcoordArray.AddElement(SKVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(SKVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 0.0f));

		NormalArray.AddElement(SKVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(SKVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(SKVector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(SKVector3(-1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(SKVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(1.0f, 1.0f, -1.0f));

		TexcoordArray.AddElement(SKVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(SKVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 0.0f));

		NormalArray.AddElement(SKVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(SKVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(SKVector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(SKVector3(1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(SKVector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, 1.0f, 1.0f));

		TexcoordArray.AddElement(SKVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(SKVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 0.0f));

		NormalArray.AddElement(SKVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(SKVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(SKVector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(SKVector3(0.0f, 1.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(SKVector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(SKVector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(SKVector3(1.0f, -1.0f, -1.0f));

		TexcoordArray.AddElement(SKVector2(0.0f, 0.0f));
		TexcoordArray.AddElement(SKVector2(0.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 1.0f));
		TexcoordArray.AddElement(SKVector2(1.0f, 0.0f));

		NormalArray.AddElement(SKVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(SKVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(SKVector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(SKVector3(0.0f, -1.0f, 0.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);

		//left
		IndexArray.AddElement(8);
		IndexArray.AddElement(9);
		IndexArray.AddElement(10);
		IndexArray.AddElement(8);
		IndexArray.AddElement(10);
		IndexArray.AddElement(11);

		//right
		IndexArray.AddElement(12);
		IndexArray.AddElement(13);
		IndexArray.AddElement(14);
		IndexArray.AddElement(12);
		IndexArray.AddElement(14);
		IndexArray.AddElement(15);

		//up
		IndexArray.AddElement(16);
		IndexArray.AddElement(17);
		IndexArray.AddElement(18);
		IndexArray.AddElement(16);
		IndexArray.AddElement(18);
		IndexArray.AddElement(19);

		//bottom
		IndexArray.AddElement(20);
		IndexArray.AddElement(21);
		IndexArray.AddElement(22);
		IndexArray.AddElement(20);
		IndexArray.AddElement(22);
		IndexArray.AddElement(23);

		SKDataBufferPtr  pVertexDate = SK_NEW SKDataBuffer;
		pVertexDate->SetDate(&VertexArray[0], (unsigned int)VertexArray.GetNum(), SKDataBuffer::DT_FLOAT32_3);

		SKDataBufferPtr pTexcoord = SK_NEW SKDataBuffer;
		pTexcoord->SetDate(&TexcoordArray[0], TexcoordArray.GetNum(), SKDataBuffer::DT_FLOAT32_2);

		SKDataBufferPtr  pNormalDate = SK_NEW SKDataBuffer;
		pNormalDate->SetDate(&NormalArray[0], (unsigned int)NormalArray.GetNum(), SKDataBuffer::DT_FLOAT32_3);

		SKDataBufferPtr pIndex = SK_NEW SKDataBuffer;
		pIndex->SetDate(&IndexArray[0], (unsigned int)IndexArray.GetNum(), SKDataBuffer::DT_USHORT);

		//BUFFER
		SKVertexBufferPtr pVertexBuffer = SK_NEW SKVertexBuffer(true);
		pVertexBuffer->SetDate(pVertexDate, SKVertexFormat::VF_POSITION);
		pVertexBuffer->SetDate(pTexcoord, SKVertexFormat::VF_TEXCOORD);
		pVertexBuffer->SetDate(pNormalDate, SKVertexFormat::VF_NORMAL);

		SKIndexBufferPtr pIndexBuffer = SK_NEW SKIndexBuffer();

		pIndexBuffer->SetDate(pIndex);

		SKTriangleSetPtr pTriangleSetDate = SK_NEW SKTriangleSet();
		pTriangleSetDate->SetVertexBuffer(pVertexBuffer);
		pTriangleSetDate->SetIndexBuffer(pIndexBuffer);


		ms_DefaultRenderCube->SetMeshDate(pTriangleSetDate);
		ms_DefaultRenderCube->m_GeometryName = _T("DefaultRenderCube");
	}
}
unsigned int SKGeometry::UpdateGeometry()
{
	//¶꣬·
	if (m_pMeshDate && SKEngineFlag::EnableCLODMesh)
	{
		m_pMeshDate->SetLodDesirePercent(m_fCLodPercent);
		m_pMeshDate->UpDateClodMesh();
	}
	return UGRI_END;
	
}
void SKGeometry::CreateMorphMeshData()
{
	m_pMorphMeshDate = NULL;
	m_pMorphMeshDate = (SKMeshDate *)SKObject::CloneCreateObject(m_pMeshDate);
	m_pMorphMeshDate->GetVertexBuffer()->SetStatic(false);
}
void SKGeometry::AddMorphAABB(SKVertexBuffer * pMorphVertexBuffer)
{
	if (!pMorphVertexBuffer)
	{
		return;
	}
	LinkBoneNode();
	if (m_pMeshDate && m_pMeshDate->GetVertexBuffer())
	{
		SKAABB3 NewAABB;

		SKVertexBuffer * pVerBuffer = m_pMeshDate->GetVertexBuffer();
		if (!pMorphVertexBuffer->GetPositionDate(0))
		{
			return;
		}

		SKVector3 * pVer = (SKVector3*)pMorphVertexBuffer->GetPositionDate(0)->GetDate();
		if (!pVer)
		{
			return;
		}
		unsigned int uiVextexNum = pVerBuffer->GetPositionDate(0)->GetNum();
		SKTransform World = m_pParent->GetWorldTransform();
		if (GetAffectSkelecton())
		{
			SKDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesDate();
			SKDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightDate();

			if (!pBlendIndex || !pBoneWeight)
			{
				return;
			}
			SKArray<SKVector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVextexNum);

			if (pBlendIndex->GetDT() == SKDataBuffer::DT_UBYTE4)
			{
				DWORD * pBlendIndexData = (DWORD *)pBlendIndex->GetDate();
				DWORD * pBoneWeightData = (DWORD *)pBoneWeight->GetDate();
				for (unsigned int i = 0; i < uiVextexNum; i++)
				{
					SKVector3W BoneWeight;
					BoneWeight.CreateFormABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					SKDWCOLORGetABGR(pBlendIndexData[i], BlendIndex[0], BlendIndex[1], BlendIndex[2], BlendIndex[3]);
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (unsigned int k = 0; k < 4; k++)
					{

						SKBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if (pBone)
						{
							SKTransform BoneWorld = pBone->GetWorldTransform();

							SKMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}

				}
			}
			else
			{
				SKVector3W * pBlendIndexData = (SKVector3W *)pBlendIndex->GetDate();
				SKVector3W * pBoneWeightData = (SKVector3W *)pBoneWeight->GetDate();
				for (unsigned int i = 0; i < uiVextexNum; i++)
				{
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (unsigned int k = 0; k < 4; k++)
					{
						unsigned int BlendIndex = (unsigned int)pBlendIndexData[i].m[k];
						SKBoneNode * pBone = GetAffectBone(BlendIndex);
						if (pBone)
						{
							SKTransform BoneWorld = pBone->GetWorldTransform();

							SKMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}

			NewAABB.CreateAABB(TempBuffer.GetBuffer(), uiVextexNum);
		}
		else
		{
			NewAABB.CreateAABB(pVer, uiVextexNum);
		}
		m_LocalBV = m_LocalBV.MergAABB(NewAABB);

	}
}
void SKGeometry::CreateLocalAABB()
{
	LinkBoneNode();
	if (m_pMeshDate && m_pMeshDate->GetVertexBuffer())
	{
		SKAABB3 NewAABB;

		SKVertexBuffer * pVerBuffer = m_pMeshDate->GetVertexBuffer();
		if (!pVerBuffer->GetPositionDate(0))
		{
			return;
		}

		SKVector3 * pVer = (SKVector3*)pVerBuffer->GetPositionDate(0)->GetDate();
		if (!pVer)
		{
			return;
		}
		unsigned int uiVextexNum = pVerBuffer->GetPositionDate(0)->GetNum();
		SKTransform World  = m_pParent->GetWorldTransform();
		if (GetAffectSkelecton())
		{
			SKDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesDate();
			SKDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightDate();

			if (!pBlendIndex || !pBoneWeight)
			{
				return ;
			}
			SKArray<SKVector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVextexNum);

			if (pBlendIndex->GetDT() == SKDataBuffer::DT_UBYTE4)
			{
				DWORD * pBlendIndexData = (DWORD *)pBlendIndex->GetDate();
				DWORD * pBoneWeightData = (DWORD *)pBoneWeight->GetDate();	
				for (unsigned int i = 0; i < uiVextexNum ;i++)
				{
					SKVector3W BoneWeight;
					BoneWeight.CreateFormABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					SKDWCOLORGetABGR(pBlendIndexData[i],BlendIndex[0],BlendIndex[1],BlendIndex[2],BlendIndex[3]);
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (unsigned int k = 0 ; k < 4 ; k++)
					{

						SKBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if(pBone)
						{
							SKTransform BoneWorld = pBone->GetWorldTransform();

							SKMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}
					
				}
			}
			else
			{
				SKVector3W * pBlendIndexData = (SKVector3W *)pBlendIndex->GetDate();
				SKVector3W * pBoneWeightData = (SKVector3W *)pBoneWeight->GetDate();	
				for (unsigned int i = 0; i < uiVextexNum ;i++)
				{
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (unsigned int k = 0 ; k < 4 ; k++)
					{
						unsigned int BlendIndex = (unsigned int) pBlendIndexData[i].m[k];
						SKBoneNode * pBone = GetAffectBone(BlendIndex);
						if(pBone)
						{
							SKTransform BoneWorld = pBone->GetWorldTransform();

							SKMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}
			
			NewAABB.CreateAABB(TempBuffer.GetBuffer(),uiVextexNum);
		}
		else
		{		
			NewAABB.CreateAABB(pVer,uiVextexNum);
		}
		m_LocalBV = NewAABB;
		
	}
	
}
void SKGeometry::UpdateWorldBound(double dAppTime)
{
	m_WorldBV.Transform(m_LocalBV,m_World.GetCombine());
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;;
	}
	
}
void SKGeometry::UpdateNodeAll(double dAppTime)
{
	
	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}

	UpdateTransform(dAppTime);
	if (dAppTime > 0.0f)
	{
		UpdateOther(dAppTime);
	}
	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;

	
}
void SKGeometry::UpDateView(SKCuller & Culler, double dAppTime)
{
	SKSpatial::UpDateView(Culler,dAppTime);
	SKCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == SKCuller::CUT_MAIN)
	{
		if (m_pMeshDate->IsClodMesh())
		{
			SKREAL ZFar = pCamera->GetZFar();
			SKVector3 DistVector = pCamera->GetWorldTranslate() - GetWorldTranslate();
			SKREAL Dist = DistVector.GetLength();

			SKREAL f = Dist / ZFar;
			f = 1.0f - Clamp(f, 1.0f, 0.0f);
			m_fCLodPercent = Clamp(f, 1.0f, 0.3f);
		}
		
	}
}
void SKGeometry::ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{
	if (Culler.CullGeometry(this))
	{
		return;
	}
	UpDateView(Culler,dAppTime);
	SKMeshNode * pMeshNode = GetMeshNode();
	if (!pMeshNode)
	{
		return;
	}
	unsigned int uiRenderGroup = pMeshNode->GetRenderGroup();
	if (uiRenderGroup >= SKCuller::RG_MAX)
	{
		return ;
	}
	SKMaterialInstance * pMaterialInstance = NULL;
	if (Culler.GetUseMaterialIndex() == -1 )
	{
		pMaterialInstance = GetUseMaterialInstance();
		if (!pMaterialInstance)
		{
			AddMaterialInstance(SKResourceManager::ms_DefaultMaterialResource);
			pMaterialInstance = GetUseMaterialInstance();
		}
	}
	else
	{
		pMaterialInstance = GetMaterialInstance(Culler.GetUseMaterialIndex());
	}
	if (!pMaterialInstance)
	{
		return;
	}

	SKMaterial * pMaterial = pMaterialInstance->GetMaterial();
	for (unsigned int i = 0 ; i < pMaterial->GetShaderMainFunctionNum() ;i++)
	{
		
		SKRenderContext VisibleContext;
		VisibleContext.m_pGeometry = this;
		VisibleContext.m_pMaterialInstance = pMaterialInstance;
		VisibleContext.m_uiPassId = i;
		VisibleContext.m_pMaterial = pMaterial;
		VisibleContext.m_pMeshNode = pMeshNode;
		const SKBlendDesc & BlendDest = pMaterial->GetRenderState(i).GetBlendState()->GetBlendDesc();
		if (Culler.GetCullerType() == SKCuller::CUT_SHADOW)
		{

			if (BlendDest.IsBlendUsed())
			{
				return;
			}
			else
			{
				Culler.InsertObject(VisibleContext, SKCuller::SKT_BASE, uiRenderGroup);
				return;
			}
		}
		else
		{
			if (pMaterial->GetCombine())
			{
				Culler.InsertObject(VisibleContext, SKCuller::SKT_COMBINE, uiRenderGroup);
			}
			else
			{
				if (BlendDest.IsBlendUsed())
				{
					Culler.InsertObject(VisibleContext, SKCuller::SKT_ALPHABLEND, uiRenderGroup);
				}
				else
					Culler.InsertObject(VisibleContext, SKCuller::SKT_BASE, uiRenderGroup);
			}
		}	
	}
	
}
bool SKGeometry::IsSwapCull()
{
	SKMeshNode * pMeshNode = GetMeshNode();
	if (pMeshNode)
	{
		SKREAL fDet = pMeshNode->GetWorldTransform().GetRotate().Det();
		return (fDet < 0.0f);
	}
	else
	{
		return false;
	}
	
}
SKGeometry::SKGeometry()
{
	m_pMorphMeshDate = NULL;
	m_pMeshDate = NULL;
	m_uiActiveNum = SKMAX_INTEGER;
	m_uiCurUseMaterial = 0;
	m_fCLodPercent = 1.0f;
}
SKGeometry::~SKGeometry()
{
	m_pMeshDate = NULL;
	m_pMorphMeshDate = NULL;
}
void SKGeometry::SetAffectBoneArray(const SKArray<SKBoneNode *> & pBoneNodeArray)
{
	if(pBoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_pBoneNode = pBoneNodeArray;
		for (unsigned int i = 0 ;i  < m_pBoneNode.GetNum(); i++)
		{
			m_BoneName.AddElement(m_pBoneNode[i]->m_cName);
		}
	}

}
void SKGeometry::SetAffectBoneArray(const SKArray<SKUsedName> & BoneNodeArray)
{
	if(BoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_BoneName = BoneNodeArray;
		LinkBoneNode();
	}
}
void SKGeometry::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	return;
}
bool SKGeometry::PostLoad( void * pDate)
{
	SKSpatial::PostLoad(pDate);
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance(SKResourceManager::ms_DefaultMaterialResource);
	}
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		m_pMaterialInstance[i]->GetMaterialR()->AddLoadEventObject(m_pMaterialInstance[i]);
	}
	return 1;
}
bool SKGeometry::PostClone(SKObject * pObjectSrc)
{
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance(SKResourceManager::ms_DefaultMaterialResource);
	}
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		m_pMaterialInstance[i]->GetMaterialR()->AddLoadEventObject(m_pMaterialInstance[i]);
	}
	return true;
}
void SKGeometry::LinkBoneNode()
{
	m_pBoneNode.Clear();
	if (m_BoneName.GetNum())
	{
		SKSkelecton * pSke = GetAffectSkelecton();
		if (pSke)
		{
			m_pBoneNode.Clear();
			for (unsigned int i = 0 ; i < m_BoneName.GetNum() ; i++)
			{
				SKBoneNode * pBoneNode = pSke->GetBoneNode(m_BoneName[i]);
				if (!pBoneNode)
				{
					return ;
				}
				m_pBoneNode.AddElement(pBoneNode);
			}
		}
	}
	m_SkinWeightBuffer.SetBufferNum(GetAffectBoneNum() * 3);
}
SKMeshNode * SKGeometry::GetMeshNode()const
{
	SKMeshNode * pMeshNode = NULL;
	SKSpatial* pParent = m_pParent;
	while(!pMeshNode && pParent)
	{
		pMeshNode = DynamicCast<SKMeshNode>(pParent);
		if (!pMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	return pMeshNode;

}
SKSkelecton * SKGeometry::GetAffectSkelecton()const
{
	SKSkelectonMeshNode * pSkeMeshNode = NULL;
	SKSpatial* pParent = m_pParent;
	while(!pSkeMeshNode && pParent)
	{
		pSkeMeshNode = DynamicCast<SKSkelectonMeshNode>(pParent);
		if (!pSkeMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	if (pSkeMeshNode)
	{
		return pSkeMeshNode->GetSkelecton();
	}
	else
	{
		return NULL;
	}
}
void SKGeometry::CreateClodMesh()
{
	
	if (m_pMeshDate && !m_pMeshDate->IsClodMesh())
	{
		if(m_pMeshDate->CreateCollapseRecord())
		{
			m_bIsStatic = false;
		}
	}

	
}
void SKGeometry::RemoveClodMesh()
{
	if (m_pMeshDate)
	{
		m_pMeshDate->RemoveClodMesh();
	}
}
unsigned int SKGeometry::GetVertexNum()const
{
	if (m_pMeshDate)
	{
		if(m_pMeshDate->GetVertexBuffer())
		{
			return m_pMeshDate->GetVertexBuffer()->GetVertexNum();
		}
	}
	return 0;
}
void SKGeometry::SetMeshDate(SKMeshDate * pMeshDate)
{
	m_pMeshDate = pMeshDate;
}
SKMeshDate * SKGeometry::GetMeshDate()const
{
	if (m_pMorphMeshDate == NULL)
	{
		return m_pMeshDate;
	}
	else
	{
		return m_pMorphMeshDate;
	}
}
SKMeshDate * SKGeometry::GetOriginMeshDate()const
{
	return m_pMeshDate;
}
unsigned int SKGeometry::GetActiveNum()
{
	if (m_pMeshDate)
	{

		if (m_pMeshDate->GetIndexBuffer() && m_pMeshDate->IsClodMesh())
		{
			m_uiActiveNum = m_pMeshDate->GetActiveNum();
		}
		else
		{
			if (m_pMeshDate->GetIndexBuffer() && (m_uiActiveNum == SKMAX_INTEGER))
				m_uiActiveNum = m_pMeshDate->GetActiveNum();
			else if (m_uiActiveNum == SKMAX_INTEGER)
				return 0;


		}
		return m_uiActiveNum;

	}
	return 0;
}
bool SKGeometry::SetActiveNum(unsigned int uiActiveNum)
{
	if (uiActiveNum > GetTotleNum())
		return 0;
	m_uiActiveNum = uiActiveNum;
	return 1;

}
unsigned int SKGeometry::GetTotleNum()const
{
	if (m_pMeshDate)
	{
		return m_pMeshDate->GetTotleNum();
	}
	return 0;

}
unsigned int SKGeometry::GetMeshDateType()
{
	if (m_pMeshDate)
	{
		return m_pMeshDate->GetMeshDateType();
	}
	return  SKMeshDate::MDT_MAX;
}
unsigned int SKGeometry::AddMaterialInstance(SKMaterialR * pMaterial)
{
	if (!pMaterial)
		return 0;
	SKMaterialInstance * pMaterialInstance = SK_NEW SKMaterialInstance(pMaterial);
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
unsigned int SKGeometry::AddMaterialInstance(SKMaterialInstance * pMaterial)
{
	if (!pMaterial || !pMaterial->GetMaterial())
		return 0;
	SKMaterialInstance * pMaterialInstance = SK_NEW SKMaterialInstance(pMaterial->GetMaterialR());
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
bool SKGeometry::SetMaterialInstance(SKMaterialR * pMaterial, unsigned int uiIndex)
{
	if (uiIndex >= m_pMaterialInstance.GetNum() || !pMaterial)
	{
		return false;
	}

	SKMaterialInstance * pMaterialInstance = SK_NEW SKMaterialInstance(pMaterial);
	m_pMaterialInstance[uiIndex] = pMaterialInstance;
	return true;

}
bool SKGeometry::SetMaterialInstance(SKMaterialInstance * pMaterial, unsigned int uiIndex)
{
	if (uiIndex >= m_pMaterialInstance.GetNum() || !pMaterial)
	{
		return false;
	}

	m_pMaterialInstance[uiIndex] = (SKMaterialInstance *)SKObject::CloneCreateObject(pMaterial);

	return true;
}
bool SKGeometry::SetUseMaterialInstance(unsigned int uiIndex)
{
	if (uiIndex >= m_pMaterialInstance.GetNum())
	{
		return false;
	}
	m_uiCurUseMaterial = uiIndex;
	return true;
}
void SKGeometry::ClearAllMaterialInstance()
{
	m_pMaterialInstance.Clear();
}
void SKGeometry::DeleteMaterialInstance(unsigned int i)
{
	if (i >= m_pMaterialInstance.GetNum())
		return;
	m_pMaterialInstance[i] = NULL;
}
unsigned int SKGeometry::DeleteMaterialInstance(SKMaterialInstance * pMaterial)
{
	if (!pMaterial)
		return SKMAX_INTEGER;
	for (unsigned int i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		if (pMaterial == m_pMaterialInstance[i])
		{
			m_pMaterialInstance[i] = NULL;
			return i;
		}
	}
	return SKMAX_INTEGER;
}
SKMaterialInstance * SKGeometry::GetUseMaterialInstance()const
{
	SKMaterialInstance * pMaterialInstance = NULL;
	if (m_uiCurUseMaterial < m_pMaterialInstance.GetNum())
	{
		pMaterialInstance = m_pMaterialInstance[m_uiCurUseMaterial];
	}
	return pMaterialInstance;
}

SKMaterialInstance * SKGeometry::GetMaterialInstance(unsigned int i)const
{
	if (i >= m_pMaterialInstance.GetNum())
		return NULL;
	return m_pMaterialInstance[i];
}
void SKGeometry::UpdateOther(double dAppTime)
{
	if (!m_pMeshDate)
	{
		return;
	}
	SKVertexBuffer * pVBuffer = GetMeshDate()->GetVertexBuffer();
	if(GetAffectBoneNum() && pVBuffer->GetBlendWeightDate() && pVBuffer->GetBlendIndicesDate())
	{
		SKTransform World  = m_pParent->GetWorldTransform();
		
		for (unsigned int i = 0 ; i < GetAffectBoneNum() ; i++)
		{
			SKBoneNode * pBone = GetAffectBone(i);
			if(pBone)
			{
				SKTransform BoneWorld = pBone->GetWorldTransform();
				SKMatrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
				SKVector3W ColumnVector[4];
				//TempBone.Identity();
				TempBone.GetColumnVector(ColumnVector);
				m_SkinWeightBuffer[i * 3] = ColumnVector[0];
				m_SkinWeightBuffer[i * 3 + 1] = ColumnVector[1];
				m_SkinWeightBuffer[i * 3 + 2] = ColumnVector[2];
			}
			else
			{
				m_SkinWeightBuffer[i * 3].Set(1.0f,0.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 1].Set(0.0f,1.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 2].Set(0.0f,0.0f,1.0f,0.0f);
			}

		}

	}
}
IMPLEMENT_RTTI(SKShadowVolumeGeometry,SKGeometry)
BEGIN_ADD_PROPERTY(SKShadowVolumeGeometry,SKGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKShadowVolumeGeometry)
IMPLEMENT_INITIAL_END
SKShadowVolumeGeometry::SKShadowVolumeGeometry()
{

}
SKShadowVolumeGeometry::~SKShadowVolumeGeometry()
{

}