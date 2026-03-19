#ifndef  SKCAMERA_H
#define  SKCAMERA_H
#include "SKNodeComponent.h"
#include "SKVector3.h"
#include "SKMatrix3X3.h"
#include "SKMatrix3X3W.h"
#include "SKPlane3.h"

namespace SKEngine2
{
class SKRenderer;
typedef struct SKVIEWPORT_TYPE
{
	SKREAL       XMin;
	SKREAL       YMin;            
	SKREAL       XMax;
	SKREAL       YMax;
	SKREAL		ZMin;
	SKREAL		ZMax;
	bool		bChangeAspect;
	void Reset()
	{
		XMin = 0.0f;
		YMin = 0.0f;
		XMax = 1.0f;
		YMax = 1.0f;
		ZMin = 0.0f;
		ZMax = 1.0f;
		bChangeAspect = false;
	}
	SKVIEWPORT_TYPE()
	{
		Reset();
	}
}SKViewPort;
class SKStream;
class SKGRAPHIC_API SKCamera : public SKNodeComponent
{
	//PRIORITY
	
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	friend class SKViewFamily;
	SKCamera();
	virtual ~SKCamera();
	enum //Camera Plane
	{	
		CP_RIGHT,
		CP_LEFT,
		CP_TOP,
		CP_BOTTOM,
		CP_FAR,
		CP_NEAR,
		CP_MAX = 6
	};
	FORCEINLINE const SKMatrix3X3W & GetViewMatrix()const;
	void CreateFromEuler(const SKVector3 &Pos,SKREAL RotX,SKREAL RotY , SKREAL RotZ);
	void CreateFromLookDir(const SKVector3 &Pos,
							const SKVector3 &vcDir,
							const SKVector3 &vcUp = SKVector3(0,1,0));
	void CreateFromLookAt(const SKVector3 &vcPos,									
							const SKVector3 &vcLookAt,							
							const SKVector3 &vcUp = SKVector3(0,1,0));	
	void CreateFromLookDirWorld(const SKVector3 &Pos,
		const SKVector3 &vcDir,
		const SKVector3 &vcUp = SKVector3(0, 1, 0));
	void CreateFromLookAtWorld(const SKVector3 &vcPos,
		const SKVector3 &vcLookAt,
		const SKVector3 &vcUp = SKVector3(0, 1, 0));
	bool SetPerspectiveFov(SKREAL fFov ,		//XŽ
						SKREAL Aspect,	//߱
						SKREAL fZN ,		//
						SKREAL fZF);		//Զ
	bool SetAspect(SKREAL Aspect);
	
	bool SetOrthogonal(SKREAL fW ,				//
						SKREAL fH,					//
						SKREAL fZN ,				//
						SKREAL fZF)	;				//Զ
	FORCEINLINE const SKMatrix3X3W &GetProjMatrix()const;
	FORCEINLINE bool AddViewPort(const SKViewPort &ViewPort);//ӿ				
	FORCEINLINE SKViewPort* GetViewPort(unsigned int i)const;
	FORCEINLINE unsigned int GetViewPortNum()const
	{
		return m_ViewPort.GetNum();
	}
	void GetFrustumPoint(SKVector3 Point[8]);
	SKAABB3 GetFrustumAABB();
	void GetPlane(SKPlane3 Plane[SKCamera::CP_MAX])const;
	FORCEINLINE SKREAL GetZFar()const
	{
		return m_ZFar;
	}
	FORCEINLINE SKREAL GetZNear()const
	{
		return m_ZNear;
	}
	FORCEINLINE SKREAL GetAspect()const
	{
		return m_Aspect;
	}
	FORCEINLINE SKREAL GetFov()const
	{
		return m_Fov;
	}
	FORCEINLINE SKViewFamily * GetViewFamily(unsigned int uiID)
	{
		if (uiID < m_ViewFamilyArray.GetNum())
		{
			return m_ViewFamilyArray[uiID];
		}
		return NULL;
	}
	FORCEINLINE unsigned int GetViewFamilyNum()
	{
		return m_ViewFamilyArray.GetNum();
	}
	FORCEINLINE void ClearProject()
	{
		m_ProjMat.Identity();
	}
	void AddCustomCullPlane(const SKPlane3& P);
	void ClearCustomCullPlane();
	SKArray<SKPlane3> m_CustomCullPlane;
	virtual void UpdateCameraState(double dAppTime);
public:
	SKREAL		m_RotX;							//ھֲϵµEULER(ת)
	SKREAL		m_RotY;
	SKREAL		m_RotZ;

//	SKVector3	m_LocalPos;						//λ

protected :
	friend class SKFreeCameraController;
	friend class VS1stCameraController;
	friend class SKSpatial;
	friend class SKScene;
	friend class SKSceneManager;
	friend class SKViewFamily;
	friend class SKLight;
	SKArray<SKViewPort>		m_ViewPort;
	SKMatrix3X3W	m_ViewMat;					//
	SKMatrix3X3W	m_ProjMat;
	

	SKREAL		m_Fov;
	SKREAL		m_Aspect;
	SKREAL		m_ZFar;
	SKREAL		m_ZNear;
	SKArrayOrder<SKViewFamily *> m_ViewFamilyArray;
	
	void AddViewFamily(SKViewFamily * pViewFamily);

	
	void DeleteViewFamily(SKViewFamily * pViewFamily);

	
public:
	virtual void UpdateTransform(double dAppTime);//±任Ϣ 
public:

	static bool InitialDefaultState();
	static bool TerminalDefaultState();

protected:
	static SKPointer<SKCamera> Default;
	
public:
	friend class SKProjLight;
	static const SKCamera * GetDefault()
	{
		return Default;
	}
};

#include "SKCamera.inl"
DECLARE_Ptr(SKCamera);
SKTYPE_MARCO(SKCamera);
};
#endif