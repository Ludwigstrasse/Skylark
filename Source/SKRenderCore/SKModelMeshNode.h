#ifndef SKMODELMESHNODE_H
#define SKMODELMESHNODE_H
#include "SKMeshNode.h"
#include "SKGeometryNode.h"
namespace SKEngine2
{
	class SKGeometryNode;
	class SKSwitchNode;
	class SKStream;
	DECLARE_Ptr(SKMorphTree);
	DECLARE_Proxy(SKMorphTree);
	class SKGRAPHIC_API SKModelMeshNode : public SKMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	//LOD TYPE
		{
			LT_NONE,
			LT_DLOD,
			LT_CLOD,
			LT_MAX
		};
		SKModelMeshNode();
		virtual ~SKModelMeshNode();

		FORCEINLINE unsigned int GetLodType()const{return m_uiLodType;}
		FORCEINLINE void SetLodType(unsigned int uiLodType)
		{
			if (uiLodType >= LT_MAX)
			{
				return;
			}
			m_uiLodType = uiLodType;
		}


		virtual SKSwitchNode * GetDlodNode()const;
		virtual unsigned int GetResourceType()const
		{
			return RT_MAX;
		}
		virtual SKGeometryNode * GetGeometryNode(unsigned int uiLodLevel);
		void SetMorphTree(SKMorphTreeR * pMorphTree);
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual void UpdateController(double dAppTime);
		void SetMorphTreeNodePara(const SKUsedName & ShowName, void * pPara);
	protected:

		unsigned int m_uiLodType;
		SKMorphTreeRPtr m_pMorphTree;
		SKMorphTreePtr m_pMorphTreeInstance;
	};
	DECLARE_Ptr(SKModelMeshNode);
	SKTYPE_MARCO(SKModelMeshNode);
}
#endif