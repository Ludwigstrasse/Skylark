#ifndef SKCULLER_H
#define	SKCULLER_H
#include "SKPlane3.h"
#include "SKObject.h"
#include "SKSphere3.h"
namespace SKEngine2
{
	/*
		б洢пɼڵ㣬ÿڵ洢ԼҶӶӦҶӼڿԼб±꣬ȥܣ
		бֻ洢Ҷӽڵ㡣
	*/
	class SKSpatial;
	class SKMaterialInstance;
	class SKMaterial;
	class SKMeshNode;
	class SKGeometry;
	class SKCamera;
	class SKLight;
	class SKRenderContext
	{
	public:
		SKRenderContext()
		{
			m_pGeometry = NULL;
			m_pMaterialInstance = NULL;
			m_uiPassId = 0;
			m_pMaterial = NULL;
			m_pMeshNode = NULL;
		}
		~SKRenderContext()
		{

		}
		static SKRenderContext ms_RenderContextNULL;
		SKGeometry * m_pGeometry;
		SKMaterialInstance * m_pMaterialInstance;
		SKMaterial * m_pMaterial;
		unsigned int m_uiPassId;
		SKMeshNode * m_pMeshNode;
		SKArray<SKLight *> m_pInDirectLight;
		SKArray<SKLight *> m_pDirectLight;
	};
	class SKGRAPHIC_API SKCuller : public SKMemObject
	{
	public:
		enum //CULLER TYPE
		{
			CUT_MAIN,
			CUT_SHADOW,
			CUT_MAX
		};
		SKCuller(int iUseMaterialIndex = -1);
		virtual ~SKCuller();
		enum
		{
			SK_MAX_PLANE_NUM = 32
		};
		enum //VisibleSet Type
		{
			SKT_BASE,
			SKT_ALPHATEST,
			SKT_ALPHABLEND,	
			SKT_COMBINE,
			SKT_MAX
		};
		enum	//Render Group
		{
			RG_BACK,
			RG_NORMAL,
			RG_FRONT,		
			RG_MAX
		};
		enum	// VisibleSet Flag
		{
			SKF_NONE,
			SKF_ALL,
			SKF_PARTIAL,
			SKF_MAX
			
		};
		enum //Get Flag
		{
			GF_ALL = 0,
			GF_NO_CASTER_SHADOW = 1,
			GF_MAX

		};
		bool PushPlane(const SKPlane3 & Plane);
		bool PushCameraPlane(SKCamera &Camera);
		FORCEINLINE void SetCamera(SKCamera &Camera);
		bool PopPlane(SKPlane3 &Plane);
		FORCEINLINE void ClearAllPlane();

		bool InsertObject(SKRenderContext &VisibleContext,unsigned int uiVisibleSetType = SKT_BASE,unsigned int uiRenderGroup = RG_NORMAL);
		FORCEINLINE unsigned int GetPlaneNum()const;
		FORCEINLINE void SetPlaneState(unsigned int uiPlaneState);
		FORCEINLINE unsigned int GetPlaneState()const;
		FORCEINLINE unsigned int GetVisibleNum(unsigned int uiVisibleSetType = SKT_BASE,unsigned int uiRenderGroup = RG_NORMAL)const;
		FORCEINLINE unsigned int GetRenderGroupVisibleNum(unsigned int uiRenderGroup = RG_NORMAL)const;
		FORCEINLINE unsigned int GetAllVisibleNum()const;
		FORCEINLINE void ClearVisibleSet();
		FORCEINLINE void ClearPlaneState();

		FORCEINLINE unsigned int GetLightNum()const;
		FORCEINLINE SKLight * GetLight(unsigned int i)const;
		FORCEINLINE void ClearLight();
		void InsertLight(SKLight * pLight);
		bool HasLight(SKLight * pLight);
		virtual unsigned int IsVisible(const SKAABB3 &BV,bool bClearState = false);
		virtual unsigned int IsVisible(const SKSphere3 &S,bool bClearState = false);
		virtual unsigned int IsVisible(const SKVector3 & Point,bool bClearState = false);
		FORCEINLINE void SetUseMaterialIndex(int iUseMaterialIndex)
		{
			m_iUseMaterialIndex = iUseMaterialIndex;
		}

		FORCEINLINE int GetUseMaterialIndex()const
		{
			return m_iUseMaterialIndex;
		}
		virtual unsigned int GetCullerType()const
		{
			return CUT_MAIN;
		}
		virtual bool ForceNoCull(const SKSpatial * pSpatial);
		virtual bool CullConditionNode(const SKMeshNode * pMeshNode);
		FORCEINLINE SKRenderContext& GetVisibleSpatial(unsigned int i,unsigned int uiVisibleSetType = SKT_BASE,unsigned int uiRenderGroup = RG_NORMAL)const;
		FORCEINLINE SKCamera * GetCamera()const;
		class RenderPriority
		{
		public:
			RenderPriority(){}
			~RenderPriority(){}
			bool operator()(SKRenderContext & p1,SKRenderContext & p2);


		};
		class AlphaPriority
		{
		public:
			AlphaPriority(SKCamera * pCamera){m_pCamera = pCamera;}
			~AlphaPriority(){}
			bool operator()(SKRenderContext & p1,SKRenderContext & p2);
		protected:
			SKCamera *m_pCamera;
		};
		friend class SKShadowCuller;
		friend class SKVolumeShadowMapCuller;
		virtual void Sort();


		virtual bool CullGeometry(SKGeometry * pGeometry);

		void GetAllVisibleAABB(unsigned int uiVisibleSetType, unsigned int uiRenderGroup, SKArray<SKAABB3> & AABBArray, unsigned int GetFlag = GF_ALL);

		void GetAndSortLight();
	protected:
		unsigned int m_uiPlaneNum;
		SKPlane3	m_Plane[SK_MAX_PLANE_NUM];
		unsigned int m_uiPlaneState;
		SKArray<SKRenderContext> m_VisibleSet[RG_MAX][SKT_MAX];
		SKArray<SKLight *> m_LightSet;
		SKCamera *m_pCamera;
		int m_iUseMaterialIndex;		
	};
#include "SKCuller.inl"
	class SKLocalLight;
	class SKGRAPHIC_API SKShadowCuller : public SKCuller
	{
	public:
		// use light Camera
		SKShadowCuller(int iUseMaterialIndex = -1);
		virtual ~SKShadowCuller();
		virtual unsigned int GetCullerType()const
		{
			return CUT_SHADOW;
		}
		SKLocalLight * m_pLocalLight;
		virtual bool CullConditionNode(const SKMeshNode * pMeshNode);
		virtual void Sort();
	protected:
		bool TestSweptSphere(SKSphere3 & Sphere, SKVector3 & SweptDir);
		bool SweptSpherePlaneIntersect(float& t0, float& t1, SKPlane3 &Plane, SKSphere3 & Shpere, SKVector3 & SweptDir);
		bool TestSphere(SKSphere3 & Shpere) const;
	};
	class SKGRAPHIC_API SKVolumeShadowMapCuller : public SKShadowCuller
	{
	public://use Culler Camera to Cull
		SKVolumeShadowMapCuller(int iUseMaterialIndex = -1);
		virtual ~SKVolumeShadowMapCuller();
		virtual bool CullConditionNode(const SKMeshNode * pMeshNode);
		virtual bool ForceNoCull(const SKSpatial * pSpatial);	
		virtual bool CullGeometry(SKGeometry * pGeometry);
	};

	class SKGRAPHIC_API SKDirShadowMapCuller : public SKShadowCuller
	{
	public:

		SKDirShadowMapCuller(int iUseMaterialIndex = -1);
		virtual ~SKDirShadowMapCuller();
		virtual bool ForceNoCull(const SKSpatial * pSpatial);
	
	};
}
#endif