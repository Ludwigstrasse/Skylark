#ifndef SKGEOMETRY_H
#define SKGEOMETRY_H
#include "SKSpatial.h"
#include "SKArray.h"
#include "SKVertexBuffer.h"
namespace SKEngine2
{

	class SKSkelecton;
	class SKMeshNode;
	class SKStream;
	DECLARE_Ptr(SKBoneNode);
	DECLARE_Ptr(SKMorphSet);
	DECLARE_Ptr(SKMeshDate);
	class SKGRAPHIC_API SKGeometry : public SKSpatial
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	// Update Geometry Renturn Information
		{
			 UGRI_FAIL,
			 UGRI_CONTINUME,
			 UGRI_END,			 
			 UGRI_MAX
		};
		enum // Geometry Use Type
		{
			GUT_NORMAL,
			GUT_SHADOW_VOLUME,
			GUT_MAX
		};
		SKGeometry();
		virtual ~SKGeometry();
		
		void SetMeshDate(SKMeshDate * pMeshDate);	
		SKMeshDate * GetMeshDate()const;
		SKMeshDate * GetOriginMeshDate()const;

		unsigned int GetActiveNum();
		bool SetActiveNum(unsigned int uiActiveNum);
		unsigned int GetTotleNum()const;
		unsigned int GetMeshDateType();



		virtual void CreateLocalAABB();

		void SetAffectBoneArray(const SKArray<SKBoneNode *> & pBoneNodeArray);
		void SetAffectBoneArray(const SKArray<SKUsedName> & BoneNodeArray);
		FORCEINLINE unsigned int GetAffectBoneNum()const
		{
			return m_pBoneNode.GetNum();
		}
		FORCEINLINE SKArray<SKVector3W>& GetSkinWeightBuffer()
		{
			return m_SkinWeightBuffer;
		}
		FORCEINLINE SKBoneNode * GetAffectBone(unsigned int i)const
		{
			if(i < m_pBoneNode.GetNum())
				return m_pBoneNode[i];
			else
				return NULL;
		}
		SKSkelecton * GetAffectSkelecton()const;

		SKMeshNode * GetMeshNode()const;

		virtual void CreateClodMesh();
		virtual void RemoveClodMesh();
		unsigned int GetVertexNum()const;


		//ºӵ޹صMesh Ϣ
		virtual unsigned int UpdateGeometry();

		virtual bool PostLoad( void * pDate);
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);
		FORCEINLINE void SetLocalBV(const SKAABB3 & BV)
		{
			m_LocalBV = BV;
		}
		FORCEINLINE SKAABB3 GetLocalBV()const
		{
			return m_LocalBV;
		}
		SKUsedName m_GeometryName;
		virtual bool IsSwapCull();
		virtual unsigned int GetGeometryUseType()
		{
			return GUT_NORMAL;
		}
		static void LoadDefault();
		static SKGeometry * GetDefaultQuad()
		{
			return ms_Quad;
		}
		static SKGeometry * GetDefaultCub()
		{
			return ms_DefaultCub;
		}
		static SKGeometry * GetDefaultCubCone()
		{
			return ms_DefaultCubCone;
		}
		static SKGeometry * GetDefaultCone()
		{
			return ms_DefaultCone;
		}
		static SKGeometry *GetDefaultRenderCube()
		{
			return ms_DefaultRenderCube;
		}
		bool SetMaterialInstance(SKMaterialInstance * pMaterial, unsigned int uiIndex);
		bool SetMaterialInstance(SKMaterialR * pMaterial, unsigned int uiIndex);

		bool SetUseMaterialInstance(unsigned int uiIndex);
		unsigned int AddMaterialInstance(SKMaterialR * pMaterial);
		unsigned int AddMaterialInstance(SKMaterialInstance * pMaterial);


		void ClearAllMaterialInstance();
		void DeleteMaterialInstance(unsigned int i);
		unsigned int DeleteMaterialInstance(SKMaterialInstance * pMaterial);

		SKMaterialInstance * GetUseMaterialInstance()const;

		SKMaterialInstance * GetMaterialInstance(unsigned int i)const;
		void AddMorphAABB(SKVertexBuffer * pMorphVertexBuffer);
		void CreateMorphMeshData();
	protected:
		//friend class SKRenderer;
		SKArray<SKMaterialInstancePtr> m_pMaterialInstance;
		unsigned int m_uiCurUseMaterial;

		virtual void UpDateView(SKCuller & Culler, double dAppTime);
		virtual void UpdateWorldBound(double dAppTime);//߽ 
		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);
		virtual void UpdateOther(double dAppTime);
		SKMeshDatePtr m_pMeshDate;
		SKMeshDatePtr m_pMorphMeshDate;
		unsigned int m_uiActiveNum;
		SKArray<SKBoneNode *> m_pBoneNode;

		SKArray<SKUsedName> m_BoneName;
		SKAABB3	m_LocalBV;
		void LinkBoneNode();
		SKArray<SKVector3W> m_SkinWeightBuffer;
		
		static SKPointer<SKGeometry> ms_Quad;
		static SKPointer<SKGeometry> ms_DefaultCub;
		static SKPointer<SKGeometry> ms_DefaultCubCone;
		static SKPointer<SKGeometry> ms_DefaultCone;
		static SKPointer<SKGeometry> ms_DefaultRenderCube;
		SKREAL m_fCLodPercent;
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	};
	DECLARE_Ptr(SKGeometry);
	SKTYPE_MARCO(SKGeometry);

	class SKGRAPHIC_API SKShadowVolumeGeometry : public SKGeometry
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKShadowVolumeGeometry();
		virtual ~SKShadowVolumeGeometry();
		virtual unsigned int GetGeometryUseType()
		{
			return GUT_SHADOW_VOLUME;
		}
		virtual void CreateClodMesh(){};
		virtual void RemoveClodMesh(){};
	};
	DECLARE_Ptr(SKShadowVolumeGeometry);
	SKTYPE_MARCO(SKShadowVolumeGeometry);
};
#endif