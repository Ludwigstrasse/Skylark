#ifndef SKANIMTREE_H
#define SKANIMTREE_H
#include "SKController.h"
#include "SKAnimSet.h"
namespace SKEngine2
{
	class SKAnimFunction;
	class SKAnimMainFunction;
	class SKAnimBaseFunction;
	class SKSkelectonMeshNode;
	class SKStream;
	class SKGRAPHIC_API SKAnimTree : public SKController , public SKResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~SKAnimTree();
		virtual bool Update(double dAppTime);
	
		SKAnimTree(const SKUsedName &ShowName);
	protected:
		SKAnimTree();
	protected:
		SKArray<SKAnimBaseFunction *> m_pAnimFunctionArray;

		SKAnimMainFunction*  m_pAnimMainFunction;
		static SKPointer<SKAnimTree> Default;
		SKUsedName m_ShowName;
	public:
		FORCEINLINE const SKUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		static const SKAnimTree *GetDefalut()
		{
			return Default;
		}
		SKSkelectonMeshNode * GetSkelectonMeshNode()const;


		void AddAnimFunction(SKAnimBaseFunction * pAnimFunction);
		void DeleteAnimFunction(SKAnimBaseFunction * pAnimFunction);
		FORCEINLINE SKAnimMainFunction * GetAnimMainFunction()const
		{
			return m_pAnimMainFunction;
		}
		virtual bool SetObject(SKObject * pObject);
		void ResetAnimFunction();

		SKAnimBaseFunction *  GetAnimFunctionFromShowName(const SKUsedName & ShowName);
		virtual unsigned int GetResourceType()const
		{
			return RT_ANIMTREE;
		}
		void SetNodePara(const SKUsedName & ShowName,void * pPara);
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	};
	DECLARE_Ptr(SKAnimTree);
	SKTYPE_MARCO(SKAnimTree);
	DECLARE_Proxy(SKAnimTree);
	class SKGRAPHIC_API SKAnimTreeOneAnim : public SKAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		SKAnimTreeOneAnim();
		SKAnimTreeOneAnim(const SKUsedName &ShowName,const SKString &AnimName);
		~SKAnimTreeOneAnim();
	};
	DECLARE_Ptr(SKAnimTreeOneAnim);
	SKTYPE_MARCO(SKAnimTreeOneAnim);

	class SKGRAPHIC_API SKAnimTreeTwoSmoothBlendAnim : public SKAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		SKAnimTreeTwoSmoothBlendAnim();
		SKAnimTreeTwoSmoothBlendAnim(const SKUsedName &ShowName, const SKString &AnimName1, const SKString &AnimName2);
		~SKAnimTreeTwoSmoothBlendAnim();
	};
	DECLARE_Ptr(SKAnimTreeTwoSmoothBlendAnim);
	SKTYPE_MARCO(SKAnimTreeTwoSmoothBlendAnim);

	class SKGRAPHIC_API SKAnimTreeTwoImmediateAnim : public SKAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		SKAnimTreeTwoImmediateAnim();
		SKAnimTreeTwoImmediateAnim(const SKUsedName &ShowName, const SKString &AnimName1, const SKString &AnimName2);
		~SKAnimTreeTwoImmediateAnim();
	};
	DECLARE_Ptr(SKAnimTreeTwoImmediateAnim);
	SKTYPE_MARCO(SKAnimTreeTwoImmediateAnim);

	class SKGRAPHIC_API SKAnimTreePartialAnim : public SKAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		SKAnimTreePartialAnim();
		SKAnimTreePartialAnim(const SKUsedName &ShowName, const SKString &AnimName1, const SKString &AnimName2);
		~SKAnimTreePartialAnim();
	};
	DECLARE_Ptr(SKAnimTreePartialAnim);
	SKTYPE_MARCO(SKAnimTreePartialAnim);

	class SKGRAPHIC_API SKAnimTreeRectBlendAnim : public SKAnimTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		SKAnimTreeRectBlendAnim();
		SKAnimTreeRectBlendAnim(const SKUsedName &ShowName,
							const SKString &AnimName1,
							const SKString &AnimName2,
							const SKString &AnimName3,
							const SKString &AnimName4);
		~SKAnimTreeRectBlendAnim();
	};
	DECLARE_Ptr(SKAnimTreeRectBlendAnim);
	SKTYPE_MARCO(SKAnimTreeRectBlendAnim);

}
#endif