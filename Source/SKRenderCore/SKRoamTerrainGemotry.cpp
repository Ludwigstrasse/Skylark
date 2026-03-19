#include "SKRoamTerrainGemotry.h"
#include "SKCLodTerrainNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKRenderThread.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKRoamTerrainGeometry,SKCLodTerrainGeometry)
BEGIN_ADD_PROPERTY(SKRoamTerrainGeometry,SKCLodTerrainGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKRoamTerrainGeometry)
IMPLEMENT_INITIAL_END
SKRoamTerrainGeometry::SKRoamTerrainGeometry()
{
	m_Variance[0].Clear();
	m_Variance[1].Clear();

	m_TriTreeNode[0].pOwner = this;
	m_TriTreeNode[1].pOwner = this;
	m_TriBuffer.Clear();
	m_uiCurBufferNum =0;
}

SKRoamTerrainGeometry::~SKRoamTerrainGeometry()
{

}
SKREAL SKRoamTerrainGeometry::RecursiveComputeVariance(SKArray<SKREAL>& Variance,unsigned int uiIndex,
													   unsigned int A,unsigned int B,unsigned int C)
{
	
	if (uiIndex >= Variance.GetNum())
	{
		return 0;
	}
	SKVector3 * pVer = (SKVector3 *)m_pMeshDate->GetVertexBuffer()->GetPositionDate(0)->GetDate();
	if (!pVer)
	{
		return 0;
	}
	unsigned int uiMiddle = (B + C) >> 1;
	SKREAL fVariance = ABS(pVer[uiMiddle].y - (pVer[B].y + pVer[C].y) * 0.5f);
	SKREAL fLeftChildVariance = RecursiveComputeVariance(Variance,(uiIndex << 1) + 1,uiMiddle,A,B);
	SKREAL fRightChildVariance = RecursiveComputeVariance(Variance,(uiIndex << 1) + 2,uiMiddle,C,A);
	fVariance = Max(fVariance,fLeftChildVariance);
	fVariance = Max(fVariance,fRightChildVariance);

	Variance[uiIndex] = fVariance;
	return fVariance;
	
}
void SKRoamTerrainGeometry::ComputeVariance(unsigned int uiCurLevel)
{
	SKCLodTerrainNode * pTerrainNode = (SKCLodTerrainNode *)m_pParent;
	unsigned int uiLevel = 1 << pTerrainNode->GetTesselationLevel();
	unsigned int uiVarianceNum = (1 << (pTerrainNode->GetTesselationLevel() * 2 + 1)) - 1;

	m_Variance[0].SetBufferNum(uiVarianceNum);
	m_Variance[1].SetBufferNum(uiVarianceNum);
	m_TriBuffer.SetBufferNum(uiLevel * uiLevel * 2 * 2);
	for (unsigned int i = 0; i < m_TriBuffer.GetNum();i++)
	{
		m_TriBuffer[i].pOwner = this;
	}
	m_uiCurBufferNum = 0;
	unsigned int uiTri1A = uiLevel;
	unsigned int uiTri1B = uiLevel * (uiLevel + 1) + uiLevel;
	unsigned int uiTri1C = 0;

	unsigned int uiTri2A = uiLevel * (uiLevel + 1);
	unsigned int uiTri2B = 0;
	unsigned int uiTri2C = uiLevel * (uiLevel + 1) + uiLevel;
	

	RecursiveComputeVariance(m_Variance[0],0,uiTri1A,uiTri1B,uiTri1C);
	RecursiveComputeVariance(m_Variance[1],0,uiTri2A,uiTri2B,uiTri2C);
}
void SKRoamTerrainGeometry::LinkNeighbor()
{
	if (m_pNeighbor[NT_LEFT])
	{
		m_TriTreeNode[0].pLeftNeighbor = &((SKRoamTerrainGeometry *)m_pNeighbor[NT_LEFT])->m_TriTreeNode[1];
	}
	if (m_pNeighbor[NT_RIGHT])
	{
		m_TriTreeNode[1].pLeftNeighbor = &((SKRoamTerrainGeometry *)m_pNeighbor[NT_RIGHT])->m_TriTreeNode[0];
	}
	if (m_pNeighbor[NT_TOP])
	{
		m_TriTreeNode[0].pRightNeighbor = &((SKRoamTerrainGeometry *)m_pNeighbor[NT_TOP])->m_TriTreeNode[1];
	}
	if (m_pNeighbor[NT_BOTTOM])
	{
		m_TriTreeNode[1].pRightNeighbor = &((SKRoamTerrainGeometry *)m_pNeighbor[NT_BOTTOM])->m_TriTreeNode[0];
	}

	m_TriTreeNode[0].pBaseNeighbor = &m_TriTreeNode[1];
	m_TriTreeNode[1].pBaseNeighbor = &m_TriTreeNode[0];

	m_TriTreeNode[0].pLeftChild = NULL;
	m_TriTreeNode[0].pRightChild = NULL;

	m_TriTreeNode[1].pLeftChild = NULL;
	m_TriTreeNode[1].pRightChild = NULL;

	m_TriTreeNode[0].pOwner = this;
	m_TriTreeNode[1].pOwner = this;
}
void SKRoamTerrainGeometry::RecursiveTessellate(SKTriTreeNode * pTri,const SKVector3 & CameraPos,SKArray<SKREAL>& Variance,
												unsigned int uiIndex,unsigned int A,unsigned int B,unsigned int C)
{
	
	
	//㹻Ŀռ䱣ּ֣ΪռǸ޶ȷġ
	if (((uiIndex << 1) + 2) < Variance.GetNum())
	{
		SKVector3 * pVer = (SKVector3 *)m_pMeshDate->GetVertexBuffer()->GetPositionDate(0)->GetDate();
		if (!pVer)
		{
			return ;
		}

		SKCLodTerrainNode * pTerrainNode = (SKCLodTerrainNode *)m_pParent;
		

		unsigned int uiMiddle = (B + C) >> 1;
		SKVector3 Dist = CameraPos - pVer[uiMiddle];

		if (!pTri->pLeftChild)
		{
			SKREAL fRatio = ( Dist.GetSqrLength() ) / ( Max(Variance[uiIndex],1.0f) * pTerrainNode->GetCLODScale() );
			if (fRatio < 1.0f)
			{
				Split(pTri);
			}
		}
		if (pTri->pLeftChild)
		{


			RecursiveTessellate(pTri->pLeftChild,CameraPos,Variance,
				(uiIndex << 1) + 1,uiMiddle,C,A);

			RecursiveTessellate(pTri->pRightChild,CameraPos,Variance,
				(uiIndex << 1) + 2,uiMiddle,A,B);

		}
	}
}
void SKRoamTerrainGeometry::Split(SKTriTreeNode * pTri)
{
	if (pTri->pRightChild)
		return;

	if ( pTri->pBaseNeighbor && (pTri->pBaseNeighbor->pBaseNeighbor != pTri) )
		pTri->pBaseNeighbor->pOwner->Split(pTri->pBaseNeighbor);


	pTri->pLeftChild = GetBuffer();
	pTri->pRightChild = GetBuffer();

	if ( !pTri->pLeftChild || !pTri->pRightChild)
	{
		pTri->pLeftChild = NULL;
		pTri->pRightChild = NULL;
		return;
	}


	pTri->pLeftChild->pBaseNeighbor  = pTri->pLeftNeighbor;
	pTri->pLeftChild->pLeftNeighbor  = pTri->pRightChild;

	pTri->pRightChild->pBaseNeighbor  = pTri->pRightNeighbor;
	pTri->pRightChild->pRightNeighbor = pTri->pLeftChild;

	if (pTri->pLeftNeighbor != NULL)
	{
		if (pTri->pLeftNeighbor->pBaseNeighbor == pTri)
			pTri->pLeftNeighbor->pBaseNeighbor = pTri->pLeftChild;
		else if (pTri->pLeftNeighbor->pLeftNeighbor == pTri)
			pTri->pLeftNeighbor->pLeftNeighbor = pTri->pLeftChild;
		else if (pTri->pLeftNeighbor->pRightNeighbor == pTri)
			pTri->pLeftNeighbor->pRightNeighbor = pTri->pLeftChild;
		else
		{
			SKMAC_ASSERT(0);
		}
	}

	if (pTri->pRightNeighbor != NULL)
	{
		if (pTri->pRightNeighbor->pBaseNeighbor == pTri)
			pTri->pRightNeighbor->pBaseNeighbor = pTri->pRightChild;
		else if (pTri->pRightNeighbor->pRightNeighbor == pTri)
			pTri->pRightNeighbor->pRightNeighbor = pTri->pRightChild;
		else if (pTri->pRightNeighbor->pLeftNeighbor == pTri)
			pTri->pRightNeighbor->pLeftNeighbor = pTri->pRightChild;
		else
		{
			SKMAC_ASSERT(0);
		}
	}

	if (pTri->pBaseNeighbor != NULL)
	{
		if ( pTri->pBaseNeighbor->pLeftChild )
		{
			pTri->pBaseNeighbor->pLeftChild->pRightNeighbor = pTri->pRightChild;
			pTri->pBaseNeighbor->pRightChild->pLeftNeighbor = pTri->pLeftChild;
			pTri->pLeftChild->pRightNeighbor = pTri->pBaseNeighbor->pRightChild;
			pTri->pRightChild->pLeftNeighbor = pTri->pBaseNeighbor->pLeftChild;
		}
		else
			pTri->pBaseNeighbor->pOwner->Split(pTri->pBaseNeighbor);
	}
	else
	{
		// An edge triangle, trivial case.
		pTri->pLeftChild->pRightNeighbor = NULL;
		pTri->pRightChild->pLeftNeighbor = NULL;
	}
}
SKRoamTerrainGeometry::SKTriTreeNode * SKRoamTerrainGeometry::GetBuffer()
{
// 	if (m_uiCurBufferNum >= m_TriBuffer.GetNum())
// 	{
// 		return NULL;
// 	}
// 	else
	{
		SKTriTreeNode * pTriTree = &m_TriBuffer[m_uiCurBufferNum];
		m_uiCurBufferNum++;
		
		pTriTree->Clear();
		
		return pTriTree;
	}
}
void SKRoamTerrainGeometry::TessellateEx(const SKVector3 & CameraPos, unsigned int uiLevel)
{
	unsigned int uiTri1A = uiLevel;
	unsigned int uiTri1B = uiLevel * (uiLevel + 1) + uiLevel;
	unsigned int uiTri1C = 0;

	unsigned int uiTri2A = uiLevel * (uiLevel + 1);
	unsigned int uiTri2B = 0;
	unsigned int uiTri2C = uiLevel * (uiLevel + 1) + uiLevel;


	RecursiveTessellate(&m_TriTreeNode[0], CameraPos, m_Variance[0],
		0, uiTri1A, uiTri1B, uiTri1C);
	RecursiveTessellate(&m_TriTreeNode[1], CameraPos, m_Variance[1],
		0, uiTri2A, uiTri2B, uiTri2C);
}
void SKRoamTerrainGeometry::Tessellate(const SKVector3 & CameraPos)
{
	SKCLodTerrainNode * pTerrainNode = (SKCLodTerrainNode *)m_pParent;
	unsigned int uiLevel = 1 << pTerrainNode->GetTesselationLevel();
	if (SKRenderer::ms_pRenderer->IsSupportMulBufferSwtich())
	{
		TessellateEx(CameraPos, uiLevel);
	}
	else
	{
		struct SKTessellatePara
		{
			SKVector3  CameraPos;
			unsigned int uiLevel;
		};
		SKTessellatePara TessellatePara;
		TessellatePara.CameraPos = CameraPos;
		TessellatePara.uiLevel = uiLevel;
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKTessellateCommand,
			SKTessellatePara, TessellatePara, TessellatePara, SKRoamTerrainGeometry *,pRoamTerrainGeometry,this,
		{
			pRoamTerrainGeometry->TessellateEx(TessellatePara.CameraPos, TessellatePara.uiLevel);
		})
		TessellateEx(CameraPos, uiLevel);
		ENQUEUE_UNIQUE_RENDER_COMMAND_END
	}
	
}
void SKRoamTerrainGeometry::ClearInfoEx()
{
	LinkNeighbor();
	m_uiCurBufferNum = 0;
}
void SKRoamTerrainGeometry::ClearInfo()
{
	if (SKRenderer::ms_pRenderer->IsSupportMulBufferSwtich())
	{
		ClearInfoEx();
	}
	else
	{

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(SKClearInfoCommand,
			SKRoamTerrainGeometry *, pRoamTerrainGeometry, this,
		{
			pRoamTerrainGeometry->ClearInfoEx();
		})
			ClearInfoEx();
		ENQUEUE_UNIQUE_RENDER_COMMAND_END
	}
}
void SKRoamTerrainGeometry::UpdateOtherEx(unsigned int uiLevel)
{
	unsigned int uiTri1A = uiLevel;
	unsigned int uiTri1B = uiLevel * (uiLevel + 1) + uiLevel;
	unsigned int uiTri1C = 0;

	unsigned int uiTri2A = uiLevel * (uiLevel + 1);
	unsigned int uiTri2B = 0;
	unsigned int uiTri2C = uiLevel * (uiLevel + 1) + uiLevel;
	SKUSHORT_INDEX * pIndexData = (SKUSHORT_INDEX *)m_pMeshDate->GetIndexBuffer()->Lock();

	if (!pIndexData)
	{
		return;
	}
	unsigned int uiCurRenderTriNum = 0;
	RecursiveBuildRenderTriange(&m_TriTreeNode[0], uiTri1A, uiTri1B, uiTri1C, pIndexData, uiCurRenderTriNum);
	RecursiveBuildRenderTriange(&m_TriTreeNode[1], uiTri2A, uiTri2B, uiTri2C, pIndexData, uiCurRenderTriNum);
	m_pMeshDate->GetIndexBuffer()->UnLock();
	SetActiveNum(uiCurRenderTriNum);
}
void SKRoamTerrainGeometry::UpdateOther(double dAppTime)
{
	if (!m_pMeshDate)
	{
		return;
	}
	SKCLodTerrainNode * pTerrainNode = (SKCLodTerrainNode *)m_pParent;
	unsigned int uiLevel = 1 << pTerrainNode->GetTesselationLevel();
	if (SKRenderer::ms_pRenderer->IsSupportMulBufferSwtich())
	{
		UpdateOtherEx(uiLevel);
	}
	else
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKTessellateCommand,
			unsigned int ,uiLevel, uiLevel, SKRoamTerrainGeometry *, pRoamTerrainGeometry, this,
		{
			pRoamTerrainGeometry->UpdateOtherEx(uiLevel);;
		})
			UpdateOtherEx(uiLevel);
		ENQUEUE_UNIQUE_RENDER_COMMAND_END
	}
	
}
unsigned int SKRoamTerrainGeometry::UpdateGeometry()
{
	

	return UGRI_END;
}
void SKRoamTerrainGeometry::RecursiveBuildRenderTriange(SKTriTreeNode * pTri,
								 unsigned int A,unsigned int B,unsigned int C,SKUSHORT_INDEX * &pIndexData,unsigned int &uiCurRenderTriNum)
{
	if (pTri->pLeftChild)
	{
		unsigned int uiMiddle = (B + C) >> 1;
		RecursiveBuildRenderTriange(pTri->pLeftChild,uiMiddle,C,A,pIndexData,uiCurRenderTriNum);

		RecursiveBuildRenderTriange(pTri->pRightChild,uiMiddle,A,B,pIndexData,uiCurRenderTriNum);
	}
	else
	{
		*pIndexData = A;
		pIndexData++;

		*pIndexData = B;
		pIndexData++;

		*pIndexData = C;
		pIndexData++;
		uiCurRenderTriNum++;
	}

}
