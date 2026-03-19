#ifndef SKMORPHFUNCTION_H
#define SKMORPHFUNCTION_H
#include "SKVector2.h"
#include "SKMorphTree.h"
#include "SKMorphBaseFunction.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKMorphFunction : public SKMorphBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~SKMorphFunction() = 0;
		SKMorphFunction(const SKUsedName & ShowName,SKMorphTree * pMorphTree);
		virtual void UpdateGeometryDate(unsigned int GeometryIndex);
		virtual void UpdateVertexDate(unsigned int uiVertexIndex);
		virtual void ClearChangeFlag();

		enum
		{
			MAX_NUM_POS3		= 2,
			MAX_NUM_NORMAL3		= 2,
			MAX_NUM_COLOR		= 2
		};
	protected:
		SKMorphFunction();
		/*
			ʽÿֶöchannelMorph̶ֹֻ֧ĳchannelַ棺
			VSMorphSequenceϽжϣҲ˵morphsetmorphӦĲchannelmorph
			򲻻ӦsetflagΪfalseMorphMainfuntionͲϡ
		*/
		SKVector3	m_Pos[MAX_NUM_POS3];
		bool		m_bPosChange[MAX_NUM_POS3];

		SKVector3	m_Normal[MAX_NUM_NORMAL3];
		bool		m_bNormalChange[MAX_NUM_NORMAL3];
		SKVector3W	m_Tangent;
		bool		m_bTangentChange;
		SKVector3	m_Binormal;
		bool		m_bBinormalChange;

		DWORD		m_Color[MAX_NUM_COLOR];
		bool		m_bColorChange[MAX_NUM_COLOR];

	public:
		FORCEINLINE void SetPos(const SKVector3 & Pos,unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_POS3)
			{
				m_Pos[uiLevel] = Pos;
				m_bPosChange[uiLevel] = true;
			}
		}

		FORCEINLINE void SetNormal(const SKVector3 & Normal,unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_NORMAL3)
			{
				m_Normal[uiLevel] = Normal;
				m_bNormalChange[uiLevel] = true;
			}
		}
		FORCEINLINE void SetTangent(const SKVector3W &Tangent)
		{
			m_Tangent = Tangent;
			m_bTangentChange = true;
		}
		FORCEINLINE void SetBinormal(const SKVector3 &Binormal)
		{
			m_Binormal= Binormal;
			m_bBinormalChange = true;
		}

		FORCEINLINE void SetColor(const DWORD Color, unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_NORMAL3)
			{
				m_Color[uiLevel] = Color;
				m_bColorChange[uiLevel] = true;
			}
		}

		FORCEINLINE SKVector3 * GetPos(unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_POS3 && m_bPosChange[uiLevel])
			{
				return &m_Pos[uiLevel];
			}
			return NULL;
		}

		FORCEINLINE SKVector3 * GetNormal(unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_NORMAL3 && m_bNormalChange[uiLevel])
			{
				return &m_Normal[uiLevel];
			}
			return NULL;
		}
		FORCEINLINE SKVector3W * GetTangent()
		{
			if (m_bTangentChange)
			{
				return &m_Tangent;
			}
			return NULL;
		}
		FORCEINLINE SKVector3 * GetBinormal()
		{
			if (m_bBinormalChange)
			{
				return &m_Binormal;
			}
			return NULL;	
		}

		FORCEINLINE DWORD * GetColor(unsigned int uiLevel)
		{
			if (uiLevel < MAX_NUM_COLOR && m_bColorChange[uiLevel])
			{
				return &m_Color[uiLevel];
			}
			return NULL;
		}

	};
	DECLARE_Ptr(SKMorphFunction);
	SKTYPE_MARCO(SKMorphFunction);
}
#endif
