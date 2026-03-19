#include "SKGraph.h"
using namespace SKEngine2;
void SKGraphNode::AddEdge(SKGraphEdge * pEdge)
{
	if (pEdge)
	{
		SKMAC_ASSERT(pEdge->GetFrom() && pEdge->GetTo());
		if (pEdge->GetFrom() && pEdge->GetTo())
		{
			m_Edge.AddElement(pEdge);
		}

	}

}
SKGraph::SKGraph(bool bIsDig)
{
	m_bIsDig = bIsDig;
}

SKGraph::~SKGraph()
{
	SKList<SKGraphNode *>::SKListIterator k = m_NodeList.Begin();
	for (;k.IsValid();++k)
	{
		if((*k) != NULL)
		{
			SKMAC_DELETE((*k));
		}
	}
	m_NodeList.Clear();
	SKList<SKGraphEdge *>::SKListIterator l = m_EdgeList.Begin();
	for (;l.IsValid();++l)
	{
		if((*l) != NULL)
		{
			SKMAC_DELETE((*l));
		}
	}
	m_EdgeList.Clear();
}

SKGraphNode * SKGraph::CreateNode()
{
	SKGraphNode * p = SK_NEW SKGraphNode();
	m_NodeList.AddElement(p);
	return p;
}

void SKGraph::DeleteNode(SKGraphNode * pNode)
{
	if (!pNode)
	{
		return ;
	}
	bool bFind = false;
	SKList<SKGraphNode *>::SKListIterator k = m_NodeList.Begin();
	for (;k.IsValid();++k)
	{
		if((*k) == pNode)
		{
			bFind = true;
		}
	}
	if (bFind)
	{
		SKList<SKGraphEdge *> &s = pNode->GetEdge();
		SKList<SKGraphEdge *>::SKListIterator m = s.Begin();
		for (;m.IsValid();++m)
		{
			SKGraphEdge * pEdge = (SKGraphEdge *)(*m);
			m_EdgeList.Erase(pEdge);
			SKMAC_DELETE(pEdge);
		}
		m_NodeList.Erase(pNode);
		SKMAC_DELETE(pNode);
	}
}

SKGraphEdge *SKGraph::CreateEdge(SKGraphNode * pFrom,SKGraphNode * pTo,SKREAL fCost)
{
	if (!pFrom || !pTo)
	{
		return NULL;
	}
	SKGraphEdge * p = SK_NEW SKGraphEdge();
	m_EdgeList.AddElement(p);
	p->SetFrom(pFrom);
	p->SetTo(pTo);
	p->SetCost(fCost);
	SKList<SKGraphNode *>::SKListIterator k = m_NodeList.Begin();
	bool FromFind =false;
	bool ToFind = false;
	for (;k.IsValid();++k)
	{
		if((*k) == pFrom)
		{
			pFrom->AddEdge(p);
			FromFind = true;
		}
		if((*k) == pTo)
		{
			if (!m_bIsDig)
			{

				pTo->AddEdge(p);

			}
			ToFind = true;
		}
	}
	if (!FromFind || !ToFind)
	{
		SKMAC_DELETE(p);
	}

	return p;
}

void SKGraph::DeleteEdge(SKGraphEdge * pEdge)
{
	if (!pEdge)
	{
		return ;
	}
	bool bFind = false;
	SKList<SKGraphEdge *>::SKListIterator k = m_EdgeList.Begin();
	for (;k.IsValid();++k)
	{
		if((*k) == pEdge)
		{
			bFind = true;
		}
	}	
	if (bFind)
	{
		SKGraphNode * pFrom = (SKGraphNode *)pEdge->GetFrom();
		SKList<SKGraphEdge *> &s = pFrom->GetEdge();
		s.Erase(pEdge);
		if (m_bIsDig)
		{
			SKGraphNode * pTo = (SKGraphNode *)pEdge->GetTo();
			SKList<SKGraphEdge *> &w = pTo->GetEdge();
			w.Erase(pEdge);
		}
		SKMAC_DELETE(pEdge);
	}
}

void SKGraph::DeleteEdge(SKGraphNode * pFrom,SKGraphNode * pTo)
{
	SKList<SKGraphNode *>::SKListIterator k = m_NodeList.Begin();
	bool FromFind =false;
	bool ToFind = false;
	for (;k.IsValid();++k)
	{
		if((*k) == pFrom)
		{
			FromFind = true;
		}
		if((*k) == pTo)
		{
			ToFind = true;

		}
	}
	if (!FromFind || !ToFind)
	{
		return;
	}
	SKList<SKGraphEdge *> &l = pFrom->GetEdge();
	SKList<SKGraphEdge *>::SKListIterator m = l.Begin();
	SKGraphEdge * pEdge = NULL;
	for (;m.IsValid(); ++m)
	{
		if ((*m)->GetTo() == pTo)
		{
			pEdge = (SKGraphEdge *)(*m);
		}
	}
	SKMAC_ASSERT(pEdge);

	pFrom = (SKGraphNode *)pEdge->GetFrom();
	SKList<SKGraphEdge *> &s = pFrom->GetEdge();
	s.Erase(pEdge);
	if (m_bIsDig)
	{
		SKGraphNode * pTo = (SKGraphNode *)pEdge->GetTo();
		SKList<SKGraphEdge *> &w = pTo->GetEdge();
		w.Erase(pEdge);
	}
	SKMAC_DELETE(pEdge);
}

unsigned int SKGraph::GetNodeNum()const
{
	return m_NodeList.GetNum();
}

unsigned int SKGraph::GetEdgeNum()const
{
	return m_EdgeList.GetNum();
}
SKGraph_Search::SKGraph_Search(SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget)
{
	m_pGraph = pGraph;
	m_pTarget = pTarget;
	m_pSource = pSource;
}

SKGraph_Search::~SKGraph_Search()
{

}
SKGraph_SearchCommon::SKGraph_SearchCommon(SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget)
:SKGraph_Search(pGraph,pSource,pTarget)
{
	SKList<SKGraphNode*>& List = pGraph->GetAllNode();
	SKList<SKGraphNode*>::SKListIterator k = List.Begin();
	for(;k.IsValid() ; ++k)
	{
		m_NodePathShortestEdge.AddElement((*k),NULL);
		m_NodeCurShortEdge.AddElement((*k),NULL);
	}
}
SKGraph_SearchCommon::~SKGraph_SearchCommon()
{

}
SKArray<SKGraphNode *> SKGraph_SearchCommon::GetPathToTarget()const
{
	SKArray<SKGraphNode *> Path;
	if (!m_pTarget)
	{
		return Path;
	}
	SKGraphNode * pIndexNode = m_pTarget;
	Path.AddElement(pIndexNode);

	while(pIndexNode != m_pSource)
	{
		unsigned int uiIndex = m_NodePathShortestEdge.Find(pIndexNode);

		if (m_NodePathShortestEdge[uiIndex].Value != NULL)
		{
			pIndexNode = m_NodePathShortestEdge[uiIndex].Value->GetFrom();
			Path.AddElement(pIndexNode);
		}
		else
		{
			break;
		}


	}
	return Path;
}
unsigned int SKGraph_SearchCommon::RemoveEdge(SKGraphNode * pGraphNode)
{
	unsigned int uiIndex1 = m_NodePathShortestEdge.Find(pGraphNode);
	//unsigned int uiIndex2 = m_NodeCurShortEdge.Find(pGraphNode);

	m_NodePathShortestEdge[uiIndex1].Value = m_NodeCurShortEdge[uiIndex1].Value;
	return uiIndex1;
}
void SKGraph_SearchCommon::ReSort(SKGraphNode * pGraphNode,SKREAL fCost)
{
	for (unsigned int i = 0 ; i < m_Priority.GetNum() ; i++)
	{
		if(m_Priority[i].m_pGraphNode == pGraphNode)
		{
			m_Priority[i].m_fCost = fCost;
			m_Priority.ReSort(i);
			return;
		}
	}
}
SKGraph_SearchDijkstra::SKGraph_SearchDijkstra(SKGraph * pGraph,SKGraphNode * pSource,SKGraphNode * pTarget)
:SKGraph_SearchCommon(pGraph,pSource,pTarget)
{
	SKList<SKGraphNode*>& List = pGraph->GetAllNode();
	SKList<SKGraphNode*>::SKListIterator k = List.Begin();
	for(;k.IsValid() ; ++k)
	{
		m_CostToThisNode.AddElement((*k),0.0f);
	}
	Search();
}
SKGraph_SearchDijkstra::~SKGraph_SearchDijkstra()
{

}

void SKGraph_SearchDijkstra::Search()
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
		//unsigned int uiIndex = m_CostToThisNode.Find(pNextNode);
		SKREAL fCurCost = m_CostToThisNode[uiIndex].Value; 
		for(;k.IsValid() ; ++k)
		{
			SKREAL fNewCost = fCurCost + (*k)->GetCost();
			unsigned int uiIndex1 = m_NodeCurShortEdge.Find((*k)->GetTo());
			if (m_NodeCurShortEdge[uiIndex1].Value == NULL)
			{
				//unsigned int uiIndex2 = m_CostToThisNode.Find((*k)->GetTo());
				m_CostToThisNode[uiIndex1].Value = fNewCost;
				SortElement SE;
				SE.m_fCost = fNewCost;
				SE.m_pGraphNode = (*k)->GetTo();
				m_Priority.AddElement(SE);
				m_NodeCurShortEdge[uiIndex1].Value = (*k);
			}
			else 
			{
				//unsigned int uiIndex2 = m_NodePathShortestEdge.Find((*k)->GetTo());
				if (m_NodePathShortestEdge[uiIndex1].Value == NULL)
				{
					//unsigned int uiIndex3 = m_CostToThisNode.Find((*k)->GetTo());
					if(m_CostToThisNode[uiIndex1].Value > fNewCost)
					{
						m_CostToThisNode[uiIndex1].Value  = fNewCost;
						ReSort((*k)->GetTo(),fNewCost);
						m_NodeCurShortEdge[uiIndex1].Value = (*k);
					}
				}
			}
		}

	}
}
SKREAL SKGraph_SearchDijkstra::GetCostToTarget()const
{
	if (!m_pTarget)
	{
		return 0.0f;
	}
	unsigned int uiIndex = m_CostToThisNode.Find(m_pTarget);
	return m_CostToThisNode[uiIndex].Value;
}
SKREAL SKGraph_SearchDijkstra::GetCostToNode(SKGraphNode * pGraphNode)const
{
	if (!pGraphNode)
	{
		return 0.0f;
	}
	unsigned int uiIndex = m_CostToThisNode.Find(pGraphNode);
	return m_CostToThisNode[uiIndex].Value;
}

void main()
{
	//SKGraph *K = CreateFrom2DMatrix<SKGraph>(NULL,1,1);
}