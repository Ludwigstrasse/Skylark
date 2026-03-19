#ifndef SKGRAPH_H
#define SKGRAPH_H
#include "SKList.h"
#include "SKMap.h"
#include "SKStack.h"
#include "SKMath.h"
namespace SKEngine2
{
	class SKGraphEdge;
	class SKDATESTRUCT_API SKGraphNode 
	{
	public:
		SKGraphNode()
		{
			m_Edge.Clear();
		}
		~SKGraphNode()
		{

		}
		void AddEdge(SKGraphEdge * pEdge);

		SKList<SKGraphEdge *> &GetEdge()
		{
			return m_Edge;
		}
	protected:
		//ͨı
		SKList<SKGraphEdge *> m_Edge;
	};
	class SKDATESTRUCT_API SKGraphEdge
	{
	public:
		SKGraphEdge()
		{
			m_pFrom = NULL;
			m_pTo = NULL;
		}
		~SKGraphEdge()
		{
			m_pFrom = NULL;
			m_pTo = NULL;
		}
		FORCEINLINE void SetFrom(SKGraphNode * pGraphNode)
		{
			SKMAC_ASSERT(pGraphNode);
			m_pFrom = pGraphNode;	
		}
		FORCEINLINE void SetTo(SKGraphNode *pGraphNode)
		{
			SKMAC_ASSERT(pGraphNode);
			m_pTo = pGraphNode;
		}
		FORCEINLINE void SetCost(SKREAL fCost)
		{
			m_fCost = fCost;
		}

		FORCEINLINE SKGraphNode * GetFrom()
		{
			return m_pFrom;
		}
		FORCEINLINE SKGraphNode * GetTo()
		{
			return m_pTo;
		}
		FORCEINLINE SKREAL GetCost()
		{
			return m_fCost;
		}
	protected:
		SKGraphNode * m_pFrom;
		SKGraphNode * m_pTo;
		SKREAL m_fCost;
	};

	class SKDATESTRUCT_API SKGraph
	{
	public:
		SKGraph(bool bIsDig = false);
		~SKGraph();
		
		virtual SKGraphNode * CreateNode();
		void DeleteNode(SKGraphNode * pNode);
		virtual SKGraphEdge *CreateEdge(SKGraphNode * pFrom,SKGraphNode * pTo,SKREAL fCost = 0.0f);
		void DeleteEdge(SKGraphEdge * pEdge);
		void DeleteEdge(SKGraphNode * pFrom,SKGraphNode * pTo);
		unsigned int GetNodeNum()const;
		unsigned int GetEdgeNum()const;
		SKList<SKGraphNode *> & GetAllNode()
		{
			return m_NodeList;
		}
		SKList<SKGraphEdge *> &GetAllEdge()
		{
			return m_EdgeList;
		}
	protected:
		bool m_bIsDig;
		SKList<SKGraphNode *> m_NodeList;
		SKList<SKGraphEdge *> m_EdgeList;
	};
	
	
	
	template<class T,unsigned int uiNodeNum>
	T *CreateFromAdjacencyMatrix(SKREAL pMatrixArray[uiNodeNum][uiNodeNum])
	{
		if (!pMatrixArray || !uiNodeNum)
		{
			return NULL;
		}
		SKGraph * pGraph = SK_NEW T(true);
		SKGraphNode ** pGraphNode = SK_NEW SKGraphNode *[uiNodeNum];

		for (unsigned int i = 0 ; i < uiNodeNum ; i++)
		{
			pGraphNode[i] = pGraph->CreateNode();

		}
		for (unsigned int i = 0 ; i < uiNodeNum ; i++)
		{
			for (unsigned int j = 0 ; j < uiNodeNum ; j++)
			{
				if (i == j)
				{
					continue;
				}
				if(pMatrixArray[i][j] > 0.0f)
				{
					pGraph->CreateEdge(pGraphNode[i],pGraphNode[j],pMatrixArray[i][j]);
				}
			}
		}
		SKMAC_DELETEA(pGraphNode);
		return pGraph;
	}
	//0 ͨ 0ʾͨ
	template<class T>
	T *CreateFrom2DMatrix(unsigned char ** pMatrixArray,int uiWidth,int uiHeight)
	{
		if (!pMatrixArray || !uiWidth ||!uiHeight)
		{
			return NULL;
		}
		SKGraph * pGraph = SK_NEW T(false);
		SKGraphNode ** pGraphNode = SK_NEW SKGraphNode *[uiWidth * uiHeight];
		for (int i = 0 ; i < uiHeight ; i++)
		{
			for (int j = 0 ; j < uiWidth ;j++)
			{
				pGraphNode[i * uiWidth + j] = pGraph->CreateNode();
			}
			
		}
		struct TempElement
		{
			TempElement()
			{

			}
			TempElement(int i,int j)
			{
				di = i;
				dj = j;
			}
			~TempElement()
			{
			}
			int di;
			int dj;
		};
		SKArray<TempElement> EArray;
		EArray.AddElement(TempElement(-1,-1));
		EArray.AddElement(TempElement(-1,0));
		EArray.AddElement(TempElement(-1,1));
		EArray.AddElement(TempElement(0,-1));
		EArray.AddElement(TempElement(0,1));
		EArray.AddElement(TempElement(1,-1));
		EArray.AddElement(TempElement(1,0));
		EArray.AddElement(TempElement(1,1));
		for (int i = 0 ; i < uiHeight ; i++)
		{
			for (int j = 0 ; j < uiWidth ; j++)
			{
				if (pGraphNode[i][j] > 0 && i != j)
				{
					for (unsigned int k = 0 ; k < EArray.GetNum() ; k++)
					{
						int Usei = EArray[k].di + i;
						if ( Usei < 0 || Usei >= uiHeight) 
						{
							continue;
						}
						int Usej = EArray[k].dj + j;
						if (Usej < 0 || Usej >= uiWidth) 
						{
							continue;
						}
						if (pGraphNode[Usei][Usej] > 0)
						{
							pGraph->CreateEdge(pGraphNode[i][j],pGraphNode[Usei][Usej],1.0f);
						}

					}
				}
				
				
					
				
			}
		}
		SKMAC_DELETEA(pGraphNode);
		return pGraph;

	}

	class SKDATESTRUCT_API SKGraph_Search
	{
	protected:
		SKGraphNode * m_pSource;
		SKGraphNode * m_pTarget;
		virtual void Search() = 0;
		SKGraph * m_pGraph;
	public:
		SKGraph_Search( SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget);
		virtual ~SKGraph_Search();
		virtual SKArray<SKGraphNode *> GetPathToTarget()const = 0;
		virtual SKREAL GetCostToTarget()const = 0;
	};
	
	
	class SKDATESTRUCT_API SKGraph_SearchCommon : public SKGraph_Search
	{
	public:
		SKGraph_SearchCommon( SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget);
		virtual ~SKGraph_SearchCommon();
		virtual SKArray<SKGraphNode *> GetPathToTarget()const;
	protected:
		unsigned int RemoveEdge(SKGraphNode * pGraphNode);
		void ReSort(SKGraphNode * pGraphNode,SKREAL fCost);
		SKMapOrder<SKGraphNode * , SKGraphEdge *> m_NodePathShortestEdge;
		SKMapOrder<SKGraphNode * ,SKGraphEdge *> m_NodeCurShortEdge;
		struct SortElement
		{
		public:
			SortElement()
			{
				m_fCost = 0.0f;
				m_pGraphNode = NULL;
			}
			~SortElement()
			{	

			}
			SKREAL m_fCost;
			SKGraphNode * m_pGraphNode;
			bool operator > (const SortElement& e)const
			{
				return m_fCost < e.m_fCost;
			}
			bool operator < (const SortElement& e)const
			{
				return m_fCost > e.m_fCost;
			}
			bool operator == (const SortElement& e)const
			{
				return m_fCost == e.m_fCost;
			}
		};

		SKArrayOrder<SortElement> m_Priority;
	};
	class SKDATESTRUCT_API SKGraph_SearchDijkstra : public SKGraph_SearchCommon
	{
	public:
		SKGraph_SearchDijkstra(SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget);
		virtual ~SKGraph_SearchDijkstra();
		
		virtual SKREAL GetCostToTarget()const;
		virtual SKREAL GetCostToNode(SKGraphNode * pGraphNode)const;
	protected:
		virtual void Search();
		
		SKMapOrder<SKGraphNode * , SKREAL> m_CostToThisNode;
		
		
	};
	class SKDATESTRUCT_API SKHeuristic
	{
	public:
		static SKREAL Calculate(SKGraphNode * pN1,SKGraphNode * pN2)
		{
			return 0.0f;
		}
	};
	template<class SKHeuristic>
	class SKGraph_SearchAStar : public SKGraph_SearchCommon
	{
	public:
		SKGraph_SearchAStar(SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget);
		virtual ~SKGraph_SearchAStar();
		virtual SKREAL GetCostToTarget()const;
	protected:
		virtual void Search();
		SKMapOrder<SKGraphNode * , SKREAL> m_CostFToThisNode;
		SKMapOrder<SKGraphNode * , SKREAL> m_CostGToThisNode;
	};
	template<class SKHeuristic>
	SKGraph_SearchAStar<SKHeuristic>::SKGraph_SearchAStar(SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget)
		:SKGraph_SearchCommon(pGraph,pSource,pTarget)
	{
		SKList<SKGraphNode*>& List = pGraph->GetAllNode();
		SKList<SKGraphNode*>::SKListIterator k = List.Begin();
		for(;k.IsValid() ; ++k)
		{
			m_CostFToThisNode.AddElement((*k),0.0f);
			m_CostGToThisNode.AddElement((*k),0.0f);
		}
		Search();
	}
	template<class SKHeuristic>
	SKGraph_SearchAStar<SKHeuristic>::~SKGraph_SearchAStar()
	{

	}
	template<class SKHeuristic>
	void SKGraph_SearchAStar<SKHeuristic>::Search()
	{
		SortElement SE;
		SE.m_fCost = 0.0f;
		SE.m_pGraphNode = m_pSource;
		m_Priority.AddElement(SE);
		while(m_Priority.GetNum())
		{
			SKGraphNode * pNextNode = m_Priority[m_Priority.GetNum() - 1].m_pGraphNode;
			m_Priority.Erase(m_Priority.GetNum() - 1);
			unsigned int uiIndex = RemoveEdge(pNextNode);
			if (pNextNode == m_pTarget)
			{
				return ;
			}
			SKList<SKGraphEdge*>& List  = pNextNode->GetEdge();
			SKList<SKGraphEdge*>::SKListIterator k = List.Begin();
			//unsigned int uiIndex = m_CostGToThisNode.Find(pNextNode);
			SKREAL fGCost = m_CostGToThisNode[uiIndex].Value; 
			for(;k.IsValid() ; ++k)
			{
				SKREAL fNewCost = fGCost + (*k)->GetCost();
				SKREAL fHCost = SKHeuristic::Calculate(m_pTarget,(*k)->GetTo());
				unsigned int uiIndex1 = m_NodeCurShortEdge.Find((*k)->GetTo());
				if (m_NodeCurShortEdge[uiIndex1].Value == NULL)
				{
					//unsigned int uiIndex2 = m_CostFToThisNode.Find((*k)->GetTo());
					m_CostFToThisNode[uiIndex1].Value = fNewCost + fHCost;

					//unsigned int uiIndex3 = m_CostGToThisNode.Find((*k)->GetTo());
					m_CostGToThisNode[uiIndex1].Value = fNewCost;

					SortElement SE;
					SE.m_fCost = fNewCost + fHCost;
					SE.m_pGraphNode = (*k)->GetTo();
					m_Priority.AddElement(SE);
					m_NodeCurShortEdge[uiIndex1].Value = (*k);
				}
				else 
				{
					//unsigned int uiIndex2 = m_NodePathShortestEdge.Find((*k)->GetTo());
					if (m_NodePathShortestEdge[uiIndex1].Value == NULL)
					{
						//unsigned int uiIndex3 = m_CostGToThisNode.Find((*k)->GetTo());
						if(m_CostGToThisNode[uiIndex1].Value > fNewCost)
						{
							m_CostGToThisNode[uiIndex1].Value  = fNewCost;
							m_CostGToThisNode[uiIndex1].Value  = fNewCost + fHCost;
							ReSort((*k)->GetTo(),fNewCost + fHCost);
							m_NodeCurShortEdge[uiIndex1].Value = (*k);
						}
					}
				}
			}

		}
	}
	template<class SKHeuristic>
	SKREAL SKGraph_SearchAStar<SKHeuristic>::GetCostToTarget()const
	{
		if (!m_pTarget)
		{
			return 0.0f;
		}
		unsigned int uiIndex = m_CostGToThisNode.Find(m_pTarget);
		return m_CostGToThisNode[uiIndex].Value;
	}
}
#endif