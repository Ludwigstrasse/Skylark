#ifndef SKSKELECTONMESHNODE_H
#define SKSKELECTONMESHNODE_H
#include "SKModelMeshNode.h"
#include "SKSkelecton.h"
#include "SKAnimSet.h"
#include "SKAnimSequenceFunc.h"
#include "SKSocketNode.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKSkelectonMeshNode : public SKModelMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKSkelectonMeshNode();
		virtual ~SKSkelectonMeshNode();
		
		virtual unsigned int GetResourceType()const
		{
			return RT_SKELECTON_MODEL;
		}
	protected:
		SKSkelectonPtr m_pSkelecton;
		SKAnimSetPtr   m_pAnimSet;
		SKAnimSequenceFuncPtr m_pAnimSequence;

	public:
		void SetSkelecton(SKSkelecton * pSkelecton);
		void SetAnimSet(SKAnimSet * pAnimSet);
		void SetAnimTree(SKAnimTreeR * pAnimTree);
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);
		FORCEINLINE SKSkelecton * GetSkelecton()const
		{
			return m_pSkelecton;
		}
		FORCEINLINE SKAnimSet * GetAnimSet()const
		{
			return m_pAnimSet;
		}
		FORCEINLINE void SetIsDrawSkelecton(bool bIsDrawSkelecton)
		{
			if (m_pSkelecton)
			{
				m_pSkelecton->m_bIsDrawSkelecton = bIsDrawSkelecton;
			}
		}

		FORCEINLINE bool IsDrawSkelecton()const
		{
			if (m_pSkelecton)
			{
				return m_pSkelecton->m_bIsDrawSkelecton;
			}
			return 0;
		}
		virtual void UpdateController(double dAppTime);
		//can not use with anim tree
		bool PlayAnim(const SKString & AnimName,SKREAL fRatio = 1.0f,unsigned int uiRepeatType = SKController::RT_NONE);
		void StopAnim();

		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual void UpdateAll(double dAppTime);
		SKSocketNode * CreateSocket(const SKUsedName & BoneName,const SKUsedName &SocketName);
		void DeleteSocket(const SKUsedName &SocketName);
		SKSocketNode * GetSocket(const SKUsedName &SocketName);
		void SetAnimTreeNodePara(const SKUsedName & ShowName, void * pPara);
	protected:
		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);
	protected:
		static SKPointer<SKSkelectonMeshNode> Default;
		SKAnimTreeRPtr m_pAnimTree;
		
		SKAnimTreePtr m_pAnimTreeInstance;
		
		void UpdateLocalAABB();
		SKArray<SKSocketNodePtr> m_pSocketArray;
	public:
		static const SKSkelectonMeshNode *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	};
	DECLARE_Ptr(SKSkelectonMeshNode);
	SKTYPE_MARCO(SKSkelectonMeshNode);
}
#endif