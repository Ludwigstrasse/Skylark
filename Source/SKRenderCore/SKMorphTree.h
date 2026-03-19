#ifndef SKMORPHTREE_H
#define SKMORPHTREE_H
#include "SKController.h"
#include "SKModelMeshNode.h"
namespace SKEngine2
{
	class SKMorphBaseFunction;
	class SKMorphFunction;
	class SKMorphMainFunction;
	class SKStream;
	class SKGRAPHIC_API SKMorphTree : public SKController,public SKResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~SKMorphTree();
		virtual bool Update(double dAppTime);

		SKMorphTree(const SKUsedName &ShowName);
	protected:
		SKMorphTree();
	protected:

		SKArray<SKMorphBaseFunction *> m_pMorphFunctionArray;

		SKMorphMainFunction*  m_pMorphMainFunction;
		static SKPointer<SKMorphTree> Default;
		SKUsedName m_ShowName;
		bool m_bCompressData;
	public:
		FORCEINLINE bool IsCompressData()const
		{
			return m_bCompressData;
		}
		FORCEINLINE const SKUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		static const SKMorphTree *GetDefalut()
		{
			return Default;
		}
		FORCEINLINE SKModelMeshNode * GetMeshNode()const
		{
			return DynamicCast<SKModelMeshNode>(m_pObject);
		}
		void ClearChangeFlag();
		void AddMorphFunction(SKMorphBaseFunction * pMorphFunction);
		void DeleteMorphFunction(SKMorphBaseFunction * pMorphFunction);
		FORCEINLINE SKMorphMainFunction * GetMorphMainFunction()const
		{
			return m_pMorphMainFunction;
		}
		virtual bool SetObject(SKObject * pObject);
		virtual unsigned int GetResourceType()const
		{
			return RT_MORPHTREE;
		}
		SKMorphBaseFunction *  GetMorphFunctionFromShowName(const SKUsedName & ShowName);
		void SetNodePara(const SKUsedName & ShowName, void * pPara);
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	};
	DECLARE_Ptr(SKMorphTree);
	SKTYPE_MARCO(SKMorphTree);
	DECLARE_Proxy(SKMorphTree);

	class SKGRAPHIC_API SKMorphTreeTwoSmoothBlendAnim : public SKMorphTree
	{
	public:
		DECLARE_RTTI;
		DECLARE_INITIAL
		SKMorphTreeTwoSmoothBlendAnim();
		SKMorphTreeTwoSmoothBlendAnim(const SKUsedName &ShowName, const SKString &MorphName);
		~SKMorphTreeTwoSmoothBlendAnim();
	};
	DECLARE_Ptr(SKMorphTreeTwoSmoothBlendAnim);
	SKTYPE_MARCO(SKMorphTreeTwoSmoothBlendAnim);
}
#endif