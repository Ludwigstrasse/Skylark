#ifndef SKANIMBASEFUNCTION_H
#define SKANIMBASEFUNCTION_H
#include "SKController.h"
#include "SKAnimTree.h"
#include "SKPutNode.h"
namespace SKEngine2
{	
	class SKStream;
	class SKGRAPHIC_API SKAnimBaseFunction : public SKController
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		friend class SKAnimTree;
		virtual ~SKAnimBaseFunction() = 0;
		SKAnimBaseFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree);

		virtual bool Update(double dAppTime);
		virtual void ClearFlag();

	protected:
		SKAnimBaseFunction();
	protected:
		SKArray<SKInputNode *> m_pInput;
		SKArray<SKOutputNode *> m_pOutput;
		SKAnimTree * m_pOwner;
		SKUsedName m_ShowName;

		bool m_bIsVisited;
		bool m_bNoLeafStart;
		FORCEINLINE void SetOwner(SKAnimTree * pOwner)
		{
			SKMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}

		
	public:
		SKSkelectonMeshNode * GetSkelectonMeshNode()const;
		SKInputNode * GetInputNode(unsigned int uiNodeID)const;
		SKInputNode * GetInputNode(const SKString & NodeName)const;

		SKOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		SKOutputNode * GetOutputNode(const SKString & NodeName)const;

		FORCEINLINE const SKUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		virtual void SetPara(void * pPara){};

		virtual bool IsLeafNode();
		void GetLeafArray(SKArray<SKAnimBaseFunction *> & LeafNode);

		void EnableLeafStart();
		void NoLeafStart();
	};
	DECLARE_Ptr(SKAnimBaseFunction);
	SKTYPE_MARCO(SKAnimBaseFunction);
}
#endif