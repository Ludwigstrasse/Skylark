#include "SKMorphSet.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKMorph,SKObject)
BEGIN_ADD_PROPERTY(SKMorph,SKObject)
REGISTER_PROPERTY(m_pVertexBufferArray,pVertexBufferArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_cName,MorphName,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMorph)
IMPLEMENT_INITIAL_END
SKMorph::SKMorph()
{
	m_pVertexBufferArray.Clear();
}
SKMorph:: ~SKMorph()
{
	m_pVertexBufferArray.Clear();
}

IMPLEMENT_RTTI(SKMorphSet,SKObject)
BEGIN_ADD_PROPERTY(SKMorphSet,SKObject)
REGISTER_PROPERTY(m_pMorphArray,MorphArray,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMorphSet)
IMPLEMENT_INITIAL_END
SKMorphSet::SKMorphSet()
{
	m_pMorphArray.Clear();
}
SKMorphSet:: ~SKMorphSet()
{
	m_pMorphArray.Clear();
}
bool SKMorphSet::SetMorph(SKMorph * pMorph)
{
	if (!pMorph)
	{
		return 0;
	}
	if (m_pMorphArray.GetNum() != 0)
	{
		//Ƿ
		for (unsigned int i = 0 ; i < m_pMorphArray.GetNum() ; i++)
		{
			if(m_pMorphArray[i] == pMorph || m_pMorphArray[i]->m_cName == pMorph->m_cName)
				return 1;
		}
		//bufferNum ͳһ
		if(pMorph->GetBufferNum() != m_pMorphArray[0]->GetBufferNum())
		{
			return 0;
		}
		//ֱÿmorph ÿbufferУ
		for (unsigned int i = 0 ;i < pMorph->GetBufferNum() ; i++)
		{
			if (pMorph->GetVertexNum(i))
			{
				for (unsigned int j = 0 ; j < m_pMorphArray.GetNum() ; j++)
				{
					if (m_pMorphArray[j]->GetVertexNum(i))
					{
						if (pMorph->GetVertexNum(i) == m_pMorphArray[j]->GetVertexNum(i))
						{
							break;
						}
						else
						{
							return 0;
						}
					}					
				}
			}
		}
	}
	m_pMorphArray.AddElement(pMorph);
	return 1;
}
SKMorph * SKMorphSet::GetMorph(const SKUsedName& MorphName)const
{
	for(unsigned int i = 0 ; i < m_pMorphArray.GetNum() ; i++)
	{
		if(m_pMorphArray[i])
		{
			if(m_pMorphArray[i]->m_cName == MorphName)
				return m_pMorphArray[i];
		}
	}

	return NULL;
}
SKMorph * SKMorphSet::GetMorph(unsigned int i)const
{
	if(i >= m_pMorphArray.GetNum())
		return NULL;
	return m_pMorphArray[i];
}