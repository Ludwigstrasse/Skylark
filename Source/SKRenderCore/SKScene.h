#ifndef SKSCENE_H
#define SKSCENE_H
#include "SKNode.h"
namespace SKEngine2
{
	class SKRenderer;
	class SKStream;
	class SKQuadNode : public SKNode
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKQuadNode();
		~SKQuadNode();
		virtual void ComputeNodeVisibleSet(SKCuller & Culler, bool bNoCull, double dAppTime);
		bool RecursiveBuild(const SKArray<SKSpatial *> &pObjectArray);
		virtual unsigned int AddChild(SKSpatial * pChild);
		virtual unsigned int DeleteChild(SKSpatial *pChild);
		virtual bool DeleteChild(unsigned int i);
		virtual void		DeleteAllChild();
		SKNodeComponent *GetNeedDrawNode(unsigned int uiIndex)const;
		unsigned int GetNeedDrawNodeNum()const;
	protected:
		void DeleteNeedDrawNode(SKNodeComponent *pNeedDrawNode);
		void AddNeedDrawNode(SKNodeComponent *pNeedDrawNode);
		SKArray<SKNodeComponent *> m_pNeedDrawNode;

	};
	DECLARE_Ptr(SKQuadNode);
	SKTYPE_MARCO(SKQuadNode);
	
	class SKGRAPHIC_API SKScene:public SKObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class SKSceneManager;
		void Update(double dAppTime);
		void CollectUpdateInfo();
		SKScene();
		~SKScene();
		

		FORCEINLINE SKLight * GetAllLight(unsigned int i)const;
		FORCEINLINE unsigned int GetAllLightNum()const;


		virtual void AddObject(SKNode * pObject);
		virtual void DeleteObject(SKNode *pObject);
		virtual void DeleteAllObject();	

		virtual void ComputeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);
		bool Build();
		FORCEINLINE bool IsBuild()
		{
			return m_bIsBuild;
		}
	protected:
		SKArray<SKLight *> m_pAllLight;
		SKArray<SKCamera *> m_pAllCamera;

		SKArray<SKNode *> m_pDynamic;
		SKQuadNodePtr m_pStaticRoot;
		SKArray<SKNode *> m_ObjectNodes;
		bool m_bIsBuild;
	};
	DECLARE_Ptr(SKScene);
	SKTYPE_MARCO(SKScene);
}
#endif