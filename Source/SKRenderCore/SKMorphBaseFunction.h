#ifndef SKMORPHBASEFUNCTION_H
#define SKMORPHBASEFUNCTION_H
#include "SKController.h"
#include "SKMorphTree.h"
#include "SKPutNode.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKMorphBaseFunction : public SKController
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		friend class SKMorphTree;
		virtual ~SKMorphBaseFunction() = 0;
		SKMorphBaseFunction(const SKUsedName & ShowName,SKMorphTree * pMorphTree);
		virtual bool Update(double dAppTime);

	protected:
		SKMorphBaseFunction();

		SKArray<SKInputNode *> m_pInput;
		SKArray<SKOutputNode *> m_pOutput;
		SKMorphTree * m_pOwner;
		SKUsedName m_ShowName;
		SKGeometryNode * m_pGeomeNode;
		bool m_bIsVisited;

		FORCEINLINE void SetOwner(SKMorphTree * pOwner)
		{
			SKMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}

		virtual void ClearFlag();
		virtual void ClearChangeFlag(){};
	public:
		

		SKInputNode * GetInputNode(unsigned int uiNodeID)const;
		SKInputNode * GetInputNode(const SKString & NodeName)const;

		SKOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		SKOutputNode * GetOutputNode(const SKString & NodeName)const;

		FORCEINLINE SKModelMeshNode * GetMeshNode()const
		{
			return DynamicCast<SKModelMeshNode>(m_pObject);
		}

		FORCEINLINE const SKUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		virtual void SetPara(void * pPara){};
		FORCEINLINE void SetCurGeometryNodeNode(SKGeometryNode * pGeomeNode)
		{
			m_pGeomeNode = pGeomeNode;
		}

	};
	DECLARE_Ptr(SKMorphBaseFunction);
	SKTYPE_MARCO(SKMorphBaseFunction);
}
#endif