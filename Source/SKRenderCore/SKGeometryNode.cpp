#include "SKGeometryNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKGeometryNode,SKNode)
BEGIN_ADD_PROPERTY(SKGeometryNode,SKNode)
REGISTER_PROPERTY(m_pMorphSet, MorphSet, SKProperty::F_SAVE_LOAD_COPY | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKGeometryNode)
IMPLEMENT_INITIAL_END
SKGeometryNode::SKGeometryNode()
{
}
SKGeometryNode::~SKGeometryNode()
{
	if (m_pMorphSet)
	{
		m_pMorphSet->m_AddMorphEvent.RemoveMethod<SKGeometryNode, &SKGeometryNode::UpdateLocalAABB>(&(*this));
	}
}

SKGeometry * SKGeometryNode::GetGeometry(unsigned int i)
{
	if (i >= m_pChild.GetNum())
	{
		return NULL;
	}
	else
	{
		SKGeometry * pGeometry= DynamicCast<SKGeometry>(m_pChild[i]);
		return pGeometry;
	}
}
unsigned int SKGeometryNode::GetNormalGeometryNum()
{
	unsigned int k = 0;

	for (unsigned int i =0 ; i < m_pChild.GetNum() ;i++)
	{
		SKGeometry * pGeometry= DynamicCast<SKGeometry>(m_pChild[i]);
		if (pGeometry && pGeometry->GetGeometryUseType() == SKGeometry::GUT_NORMAL)
		{
			
			k++;
			

		}

	}
	return k ;
}
SKGeometry * SKGeometryNode::GetNormalGeometry(unsigned int index)
{
	unsigned int k = 0;
	for (unsigned int i =0 ; i < m_pChild.GetNum() ;i++)
	{
		SKGeometry * pGeometry= DynamicCast<SKGeometry>(m_pChild[i]);
		if (pGeometry && pGeometry->GetGeometryUseType() == SKGeometry::GUT_NORMAL)
		{
			if (k == index)
			{
				return pGeometry;
			}
			else
			{
				k++;
			}
			
		}
		
	}
	return NULL;
}
void SKGeometryNode::SetMorphSet(SKMorphSet * pMorphSet)
{
	if (!pMorphSet)
	{
		m_pMorphSet = NULL;
		return;
	}
	else
	{
		if (GetNormalGeometryNum() != pMorphSet->GetBufferNum())
		{
			return;
		}
		for (unsigned int i = 0; i < pMorphSet->GetMorphNum(); i++)
		{
			for (unsigned int j = 0; j < pMorphSet->GetBufferNum(); j++)
			{
				unsigned int VertexNum = pMorphSet->GetMorph(i)->GetVertexNum(j);
				SKGeometry * pGeometry = (SKGeometry *)GetNormalGeometry(j);
				if (VertexNum && pGeometry)
				{
					if (VertexNum != pGeometry->GetVertexNum())
					{
						return;
					}
				}
			}

		}

	}

	if (m_pMorphSet)
	{
		m_pMorphSet->m_AddMorphEvent.RemoveMethod<SKGeometryNode, &SKGeometryNode::UpdateLocalAABB>(&(*this));
	}
	m_pMorphSet = pMorphSet;
	m_pMorphSet->m_AddMorphEvent.AddMethod<SKGeometryNode, &SKGeometryNode::UpdateLocalAABB>(&(*this));
	UpdateLocalAABB();
}
void SKGeometryNode::UpdateLocalAABB()
{
	if (!m_pMorphSet)
	{
		return;
	}
	for (unsigned int i = 0; i < m_pMorphSet->GetMorphNum();i++)
	{
		SKMorph *  pMorph = m_pMorphSet->GetMorph(i);
		for (unsigned int j = 0; j < GetNormalGeometryNum(); j++)
		{
			SKGeometry * NormalGeometry = GetNormalGeometry(j);
			NormalGeometry->AddMorphAABB(pMorph->GetBuffer(j));
		}
	}
}
bool SKGeometryNode::PostLoad(void * pDate)
{
	if (SKNode::PostLoad(pDate) == false)
	{
		return false;
	}
	if (!m_pMorphSet)
	{
		return true;
	}
	else
	{
		for (unsigned int j = 0; j < GetNormalGeometryNum(); j++)
		{
			SKGeometry * NormalGeometry = GetNormalGeometry(j);
			NormalGeometry->CreateMorphMeshData();
		}
		return true;
	}
}
bool SKGeometryNode::PostClone(SKObject * pObjectSrc)
{
	if (SKNode::PostClone(pObjectSrc) == false)
	{
		return false;
	}
	return true;
}