#ifndef SKPUTNODE_H
#define SKPUTNODE_H
#include "SKObject.h"
#include "SKName.h"
namespace SKEngine2
{
	class SKShaderFunction;
	class SKInputNode;
	class SKOutputNode;
	class SKStream;
	class SKGRAPHIC_API SKPutNode : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum // Value Type
		{
			VT_1,
			VT_2,
			VT_3,
			VT_4,
			VT_MAX
		};
		enum		//ANIM VALUE TYPE
		{
			AVT_ANIM,
			AVT_MORPH,
			AVT_IK,
			AVT_MAX
		};
		enum	//Post Effect Type
		{
			PET_OUT
		};
		virtual ~SKPutNode() = 0;
	protected:
		SKPutNode(unsigned int uiValueType,const SKUsedName & NodeName,SKObject * pShaderFunction);
		SKPutNode();
		SKObject * m_pOwner;
		SKUsedName m_NodeName;
		unsigned int m_uiValueType;
		FORCEINLINE void SetOwner(SKObject *pOwner)
		{
			m_pOwner = pOwner;
		}
	public:
		friend class SKShaderFunction;
		friend class SKAnimBaseFunction;
		friend class SKMorphBaseFunction;
		friend class SKPostEffectFunction;
	
		
		

		FORCEINLINE unsigned int GetValueType()const
		{
			return m_uiValueType;
		}
		FORCEINLINE void SetValueType(unsigned int uiValueType)
		{
			m_uiValueType = uiValueType;
		}
		FORCEINLINE const SKUsedName & GetNodeName()const
		{
			return m_NodeName;
		}
		FORCEINLINE void SetNodeName(const SKUsedName & NodeName)
		{
			m_NodeName = NodeName;
		}
		FORCEINLINE SKObject * GetOwner()const
		{
			return m_pOwner;
		}

	};
	DECLARE_Ptr(SKPutNode);
	SKTYPE_MARCO(SKPutNode);
	class SKGRAPHIC_API SKInputNode : public SKPutNode
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		SKInputNode(unsigned int uiValueType,const SKUsedName &NodeName,SKObject * pObject);
		virtual ~SKInputNode();
	protected:
		SKInputNode();
		SKOutputNode * m_pOutputLink;
		

	public:
		friend class SKOutputNode;
		bool Connection(SKOutputNode * pOutputNode,bool bIsJudgeType = false);
		void UnConnection();
		FORCEINLINE const SKOutputNode *GetOutputLink()const
		{
			return m_pOutputLink;
		}
		bool IsConnection()
		{
			return m_pOutputLink != NULL;
		}
	};
	DECLARE_Ptr(SKInputNode);
	SKTYPE_MARCO(SKInputNode);
	class SKGRAPHIC_API SKOutputNode:public SKPutNode
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		SKOutputNode(unsigned int uiValueType,const SKUsedName &NodeName,SKObject * pShaderFunction);
		virtual ~SKOutputNode();
	protected:
		SKOutputNode();
		SKArray<SKInputNode *> m_pInputLink;
		

	public:
		friend class SKInputNode;
		bool Connection(SKInputNode * pInputNode,bool bIsJudgeType = false);
		void UnConnection();
		bool IsConnection()
		{
			return m_pInputLink.GetNum() > 0;
		}
		SKInputNode * GetInputLink(unsigned int i)
		{
			return m_pInputLink[i];
		}
		unsigned int GetInputNum()const
		{
			return m_pInputLink.GetNum();
		}
	protected:
		bool CheckIsConnection(SKInputNode * pInputNode)const;
	
	};
	DECLARE_Ptr(SKOutputNode);
	SKTYPE_MARCO(SKOutputNode);
}
#endif