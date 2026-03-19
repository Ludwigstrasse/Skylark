#ifndef SKBONENODE_H
#define SKBONENODE_H
#include "SKNode.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKBoneNode : public SKNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKBoneNode();
		virtual ~SKBoneNode();
	public:
		
	
		virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);

		//Use for IK
		bool m_bIsEffector;
		SKVector3 m_TargetPosInWorld;

		SKArray<bool> m_bAllowTranslation;
		SKArray<SKREAL> m_fMinTranslation;
		SKArray<SKREAL> m_fMaxTranslation;

		SKArray<bool> m_bAllowRotation;
		SKArray<SKREAL> m_fMinRotation;
		SKArray<SKREAL> m_fMaxRotation;
		
		SKREAL m_fIKWeight;

		SKUsedName m_cName;

		void GetIKMoveAxis(SKVector3 Axis[3])const;
		void ComputeIKLocalRotDelta(const SKMatrix3X3 & WorldRot,SKMatrix3X3 & LocalRotDelta)const;

		SKMatrix3X3W m_OffSetMatrix;


		virtual void SetLocalMat(const SKMatrix3X3W SKMat);
		
	public:

		FORCEINLINE const SKMatrix3X3W & GetBoneOffsetMatrix()const
		{
			return m_OffSetMatrix;
		}


		SKBoneNode * GetBoneNodeFromLevel(const SKUsedName & BoneName);
		unsigned int GetAllBoneNum()const;
		void GetAllBoneArray(SKArray<SKBoneNode *> & AllNodeArray);
	};
	DECLARE_Ptr(SKBoneNode);
	SKTYPE_MARCO(SKBoneNode);
}
#endif