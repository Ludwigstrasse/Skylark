#include "SKMorphSequenceFunc.h"
#include "SKMorphSet.h"
#include "SKGeometryNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKMorphSequenceFunc,SKMorphFunction)
BEGIN_ADD_PROPERTY(SKMorphSequenceFunc,SKMorphFunction)
REGISTER_PROPERTY(m_MorphName,MorphName,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMorphSequenceFunc)
IMPLEMENT_INITIAL_END
SKMorphSequenceFunc::SKMorphSequenceFunc(const SKUsedName & ShowName,SKMorphTree * pMorphTree)
:SKMorphFunction(ShowName,pMorphTree)
{
	

	SKString OutputName = _T("Output");
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::AVT_MORPH,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	m_uiRepeatType = SKController::RT_CYCLE;

	m_pVertexBuffer = NULL;
	m_pGeomeNode = NULL;
}
SKMorphSequenceFunc::SKMorphSequenceFunc()
{

	m_uiRepeatType = SKController::RT_CYCLE;
	m_pVertexBuffer = NULL;
	m_pGeomeNode = NULL;
}
SKMorphSequenceFunc::~SKMorphSequenceFunc()
{

}
void SKMorphSequenceFunc::SetMorph(const SKUsedName & MorphName)
{
	m_MorphName = MorphName;
}
void SKMorphSequenceFunc::ClearChangeFlag()
{
	SKMorphFunction::ClearChangeFlag();
	m_pVertexBuffer = NULL;
	for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_POS3; uiLevel++)
	{
		pPosDate[uiLevel] = NULL;
	}
	for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
	{
		pNormalDate[uiLevel] = NULL;
	}
	pTangentDate = NULL;
	pBinormalDate = NULL;
	for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_COLOR; uiLevel++)
	{
		pColorDate[uiLevel] = NULL;
	}
}
void SKMorphSequenceFunc::UpdateGeometryDate(unsigned int GeometryIndex)
{
	if (!m_pGeomeNode)
	{
		return;
	}
	if (m_MorphName.GetBuffer())
	{
		const SKMorphSet * pMorphSet = m_pGeomeNode->GetMorphSet();
		if (!pMorphSet)
		{
			return;
		}

		SKMorph * pMorph = pMorphSet->GetMorph(m_MorphName);
		if (!pMorph)
		{
			return;
		}

		if (!pMorph->GetVertexNum(GeometryIndex))
		{
			return;
		}

		m_pVertexBuffer = pMorph->GetBuffer(GeometryIndex);
	}
	else
	{
		SKGeometry  * pGeometry = m_pGeomeNode->GetNormalGeometry(GeometryIndex);
		SKMeshDate * pMeshData = pGeometry->GetOriginMeshDate();
		m_pVertexBuffer = pMeshData->GetVertexBuffer();
	}

	if (!m_pVertexBuffer)
	{
		return;
	}

	for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_POS3; uiLevel++)
	{
		pPosDate[uiLevel] = m_pVertexBuffer->GetPositionDate(uiLevel);
	}
	for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
	{
		pNormalDate[uiLevel] = m_pVertexBuffer->GetNormalDate(uiLevel);
	}
	pTangentDate = m_pVertexBuffer->GetTangentDate();
	pBinormalDate = m_pVertexBuffer->GetBinormalDate();
	for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_COLOR; uiLevel++)
	{
		pColorDate[uiLevel] = m_pVertexBuffer->GetColorDate(uiLevel);
	}
}
void SKMorphSequenceFunc::UpdateVertexDate(unsigned int uiVertexIndex)
{
	
	if (!m_pVertexBuffer)
	{
		return;
	}

	
	for (unsigned int uiLevel = 0 ; uiLevel < SKMorphFunction::MAX_NUM_POS3 ; uiLevel++)
	{
		SKDataBuffer * pDate = pPosDate[uiLevel];
		if (pDate)
		{
			SKVector3 * pPos = (SKVector3 *)pDate->GetDate();
			if (pPos)
			{
				pPos +=uiVertexIndex;
				SetPos(*pPos,uiLevel);
			}
			
		}
	}

	for (unsigned int uiLevel = 0 ; uiLevel < SKMorphFunction::MAX_NUM_NORMAL3 ; uiLevel++)
	{
		SKDataBuffer * pDate = pNormalDate[uiLevel];
		if (pDate)
		{
			if (pDate->GetDT() == SKDataBuffer::DT_UBYTE4N)
			{
				DWORD * pNormal = (DWORD *)pDate->GetDate();
				if (pNormal)
				{
					pNormal += uiVertexIndex;
					
					SKVector3W Temp;
					Temp.CreateFormABGR(*pNormal);
					//еĻ϶Եģ * 2 - 1ʡ
					SetNormal(Temp.GetV3(), uiLevel);
				}
			}
			else
			{
				SKVector3 * pNormal = (SKVector3 *)pDate->GetDate();
				if (pNormal)
				{
					pNormal += uiVertexIndex;
					SetNormal(*pNormal, uiLevel);
				}
			}
			

		}
	}
	
	{
		SKDataBuffer * pDate = pTangentDate;
		if (pDate)
		{
			if (pDate->GetDT() == SKDataBuffer::DT_UBYTE4N)
			{
				DWORD * pTangent = (DWORD *)pDate->GetDate();
				if (pTangent)
				{
					pTangent += uiVertexIndex;

					SKVector3W Temp;
					Temp.CreateFormABGR(*pTangent);
					//еĻ϶Եģ * 2 - 1ʡ
					SetTangent(Temp);
				}
			}
			else
			{
				SKVector3 * pTangent = (SKVector3 *)pDate->GetDate();
				if (pTangent)
				{
					pTangent += uiVertexIndex;
					SKVector3W Temp(*pTangent);
					SetTangent(Temp);
				}
			}
			

		}
	
	}

	{
		SKDataBuffer * pDate = pBinormalDate;
		if (pDate)
		{
			SKVector3 * pBinormal = (SKVector3 *)pDate->GetDate();
			if (pBinormal)
			{
				pBinormal +=uiVertexIndex;
				SetBinormal(*pBinormal);
			}

		}

	}


	for (unsigned int uiLevel = 0 ; uiLevel < SKMorphFunction::MAX_NUM_COLOR ; uiLevel++)
	{
		SKDataBuffer * pDate = pColorDate[uiLevel];
		if (pDate)
		{
			DWORD * pColor = (DWORD *)pDate->GetDate();
			if (pColor)
			{
				pColor +=uiVertexIndex;
				SetColor(*pColor,uiLevel);
			}

		}
	}

}