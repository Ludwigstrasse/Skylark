#ifndef SKANIMSEQUENCEFUNC_H
#define SKANIMSEQUENCEFUNC_H
#include "SKAnimFunction.h"
namespace SKEngine2
{
	class SKStream;


	class SKGRAPHIC_API SKAnimSequenceFunc : public SKAnimFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~SKAnimSequenceFunc();
		virtual bool Update(double dAppTime);
		friend class SKSkelectonMeshNode;
		friend class SKOneParamSmoothAnimBlendSequence;
		friend class SKTwoParamAnimBlendSequence;
		SKAnimSequenceFunc(const SKUsedName & ShowName,SKAnimTree * pAnimTree);
		
		void SetAnim(const SKUsedName& AnimName);
		virtual bool SetObject(SKObject * pObject);
		FORCEINLINE const SKAnimR * GetAnim()const
		{
			return m_pAnimR;
		}
		FORCEINLINE const SKUsedName & GetAnimName()const
		{
			return m_AnimName;
		}
		SKREAL GetAnimTime();
		friend class SKPlayRootAnimEvent;
		void BeginStart();
		bool m_bOnlyAddtiveOutput;
		virtual bool IsLeafNode(){ return true; }
	protected:
		SKAnimSequenceFunc();
	protected:
		SKAnimRPtr m_pAnimR;
		SKArray<unsigned int> m_UsedBoneIndexInAnim;
		SKUsedName m_AnimName;
		struct LAST_KEY_TYPE 
		{
		public:
			LAST_KEY_TYPE()
			{
				uiLKTranslation = 0;
				uiLKScale = 0;
				uiLKRotator = 0;
			}
			~LAST_KEY_TYPE()
			{

			}
			unsigned int uiLKTranslation;
			unsigned int uiLKScale;
			unsigned int uiLKRotator;
		};
		SKArray<LAST_KEY_TYPE> m_LastKey;
		SKArray<LAST_KEY_TYPE> m_AdditiveLastKey;
		// used only in SkelectonMeshNode
		void UpDateBone();
		SKAnimSequenceFunc(SKSkelectonMeshNode * pSkelectonMeshNode);
		void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);

		SKTransform m_SaveStartMeshComponetTransform;
		SKTransform m_SaveStartRootBoneTransform;
		
		void ComputeAnim(SKAnim * pAnim, SKArray<LAST_KEY_TYPE> & LastKey, SKArray<SKAnimAtom> &BoneOutPut);
		SKArray<SKAnimAtom> m_AdditiveBoneOutPut;
	};
	DECLARE_Ptr(SKAnimSequenceFunc);
	SKTYPE_MARCO(SKAnimSequenceFunc);

}
#endif