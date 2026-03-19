#ifndef SKROAMTERRAINGEOMETRY_H
#define SKROAMTERRAINGEOMETRY_H
#include "SKCLodTerrainGeometry.h"
namespace SKEngine2
{

	/*
			    A
				 /\
				/  \
			  C/____\ B
	˳ʱУɼ

	 |Z
	 |
	A|____B
	 |   /|
	 |  / |
	 | /  |
	C|/___|_______X

	*/
	class SKStream;
	class SKGRAPHIC_API SKRoamTerrainGeometry : public SKCLodTerrainGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKRoamTerrainGeometry();
		virtual ~SKRoamTerrainGeometry();


	public:
		virtual unsigned int GetTerrainGeometryType() {return TGT_ROAM;}
		class SKTriTreeNode
		{
		public:
			SKTriTreeNode *pBaseNeighbor;
			SKTriTreeNode *pLeftNeighbor;
			SKTriTreeNode *pRightNeighbor;
			SKTriTreeNode *pLeftChild;
			SKTriTreeNode *pRightChild;

			SKRoamTerrainGeometry * pOwner;
			SKTriTreeNode()
			{
				pOwner = NULL;
				pBaseNeighbor = NULL;
				pLeftNeighbor = NULL;
				pRightNeighbor = NULL;
				pLeftChild = NULL;
				pRightChild = NULL;
			}
			FORCEINLINE void Clear()
			{
				//pOwner = NULL;
				pBaseNeighbor = NULL;
				pLeftNeighbor = NULL;
				pRightNeighbor = NULL;
				pLeftChild = NULL;
				pRightChild = NULL;
			}
			
		};
		virtual void ComputeVariance(unsigned int uiCurLevel = 0);
		virtual void LinkNeighbor();	
		virtual void ClearInfo();
		virtual unsigned int UpdateGeometry();
	protected:
		
		virtual void Tessellate(const SKVector3 & CameraPos);
		SKREAL RecursiveComputeVariance(SKArray<SKREAL>& Variance,unsigned int uiIndex,
			unsigned int A,unsigned int B,unsigned int C);	
		void RecursiveTessellate(SKTriTreeNode * pTri,const SKVector3 & CameraPos,SKArray<SKREAL>& Variance,
			unsigned int uiIndex,unsigned int A,unsigned int B,unsigned int C);
		void Split(SKTriTreeNode * pTri);
		SKTriTreeNode * GetBuffer();
		
		void RecursiveBuildRenderTriange(SKTriTreeNode * pTri,
			unsigned int A,unsigned int B,unsigned int C,SKUSHORT_INDEX * &pIndexData,unsigned int &uiCurRenderTriNum);
		virtual void UpdateOther(double dAppTime);

		void TessellateEx(const SKVector3 & CameraPos, unsigned int uiLevel);
		void ClearInfoEx();
		void UpdateOtherEx(unsigned int uiLevel);
	protected:
		SKArray<SKREAL> m_Variance[2];
		SKTriTreeNode	m_TriTreeNode[2];
		SKArray<SKTriTreeNode>	m_TriBuffer;
		unsigned int m_uiCurBufferNum;
	};
	DECLARE_Ptr(SKRoamTerrainGeometry);
	SKTYPE_MARCO(SKRoamTerrainGeometry);
}
#endif