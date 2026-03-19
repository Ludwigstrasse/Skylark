#ifndef SKSKELECTON_H
#define SKSKELECTON_H
#include "SKNode.h"
#include "SKGeometry.h"
namespace SKEngine2
{
	DECLARE_Ptr(SKBoneNode);
	class SKStream;
	class SKGRAPHIC_API SKSkelecton : public SKNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKSkelecton();
		virtual ~SKSkelecton();

	protected:
		SKArray<SKBoneNode *> m_pBoneArray;

		void LinkBoneArray();
		virtual void UpdateNoChild(double dAppTime);
		virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);
		static SKREAL ms_fBoneAxisLength;
		void Draw(SKCamera * pCamera);
		SKAABB3	m_LocalBV;
		SKAABB3 m_OriginLocalBV;
	public:
		friend class SKSkelectonMeshNode;
		virtual void CreateLocalAABB();
		FORCEINLINE void SetLocalBV(const SKAABB3 & BV)
		{
			m_LocalBV = BV;
		}
		FORCEINLINE SKAABB3 GetLocalBV()const
		{
			return m_LocalBV;
		}
		void CreateBoneArray();
		
		

		unsigned int GetBoneNum()const;
		SKBoneNode *GetBoneNode(const SKUsedName & Name)const;
		SKBoneNode * GetBoneNode(unsigned int i)const;

		int GetBoneIndex(const SKUsedName &Name)const;
		bool m_bIsDrawSkelecton;
	

		virtual void UpDateView(SKCuller & Culler,double dAppTime);
		const SKTransform &GetRootTransform();
	};
	DECLARE_Ptr(SKSkelecton);
	SKTYPE_MARCO(SKSkelecton);
}
#endif