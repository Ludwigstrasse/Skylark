#ifndef SKSPATIAL_H
#define SKSPATIAL_H
#include "SKObject.h"
#include "SKTransform.h"
#include "SKController.h"
#include "SKAABB3.h"
#include "SKResource.h"
namespace SKEngine2
{
class SKStream;
class SKCuller;
DECLARE_Ptr(SKMaterialInstance)
DECLARE_Ptr(SKLight);
DECLARE_Ptr(SKCamera);
DECLARE_Proxy(SKMaterial);
class SKGRAPHIC_API SKSpatial :public SKObject
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY;
public:
	virtual ~SKSpatial() = 0;

protected:

	SKAABB3 m_WorldBV;
	
public:
	virtual void ComputeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);

	enum //Cull Mode
	{
		CM_DYNAMIC,
		CM_ALAWAYS,
		CM_NEVER
	};


	virtual void UpDateView(SKCuller & Culler,double dAppTime);
	FORCEINLINE const SKAABB3 & GetWorldAABB()const;

	virtual void CreateLocalAABB(){}
	
protected:
	friend class SKNode;
	friend class SKSwitchNode;
	friend class SKScene;
	friend class SKGeometry;
	friend class SKMeshComponent;
	friend class SKSkelecton;
	friend class SKNodeComponent;
	friend class SKSkelectonMeshNode;
	bool		m_bIsVisibleUpdate;

	//bool m_bForceNoCull;
public:
	virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);

public:
	unsigned int m_uiCullMode;
protected:
	SKSpatial();

	virtual void UpdateWorldBound(double dAppTime) = 0;//±߽ 
	virtual void UpdateNodeAll(double dAppTime) = 0;
	virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)= 0;
protected:
	SKTransform m_World;
	SKTransform m_Local;
	SKSpatial* m_pParent;
	SKArray<SKLight *> m_pAllLight;
	SKArray<SKCamera *> m_pAllCamera;
public:
	bool m_bInheritScale;
	bool m_bInheritRotate;
	bool m_bInheritTranlate;
	virtual void UpdateTransform(double dAppTime);//±任Ϣ 
	virtual void UpdateAll(double dAppTime);
public:
	FORCEINLINE void SetParent(SKSpatial * pParent);
	FORCEINLINE SKSpatial *GetParent()const;
	FORCEINLINE const SKVector3 &GetLocalScale()const;
	FORCEINLINE const SKVector3 & GetLocalTranslate()const;
	FORCEINLINE const SKMatrix3X3 & GetLocalRotate()const;
	FORCEINLINE void GetLocalDir(SKVector3 &Dir, SKVector3 &Up, SKVector3 & Right)const;
	FORCEINLINE const SKTransform & GetLocalTransform();

	virtual const SKVector3 &GetWorldScale();
	virtual const SKVector3 & GetWorldTranslate();
	virtual const SKMatrix3X3 & GetWorldRotate();
	virtual void GetWorldDir(SKVector3 &Dir, SKVector3 &Up, SKVector3 & Right);
	virtual const SKTransform & GetWorldTransform();

	virtual void SetWorldScale(const SKVector3 & fScale);
	virtual void SetWorldTranslate(const SKVector3& Translate);
	virtual void SetWorldRotate(const SKMatrix3X3 & Rotate);
	virtual void SetWorldTransform(const SKTransform & LocalTransform);
	virtual void SetWorldMat(const SKMatrix3X3W SKMat);

	virtual void SetLocalScale(const SKVector3 & fScale);
	virtual void SetLocalTranslate(const SKVector3& Translate);
	virtual void SetLocalRotate(const SKMatrix3X3 & Rotate);
	virtual void SetLocalTransform(const SKTransform & LocalTransform);
	virtual void SetLocalMat(const SKMatrix3X3W SKMat);
public:
	SKArray<SKControllerPtr>	m_ControllerArray;
	FORCEINLINE unsigned int GetControllerNum()const { return m_ControllerArray.GetNum(); }
	virtual void UpdateController(double dAppTime);
	bool AddController(SKController * pController);
	SKController * DeleteController(unsigned int ID);
	unsigned int DeleteController(SKController *pController);
	SKController *GetController(unsigned int ID)const;
	void DeleteAllController();
	FORCEINLINE SKArray<SKPointer<SKController>> * GetControllerList();
public:
	FORCEINLINE void SetDynamic(bool bIsDynamic);
	FORCEINLINE bool IsDynamic()const { return !m_bIsStatic; }
	void ClearAllTime();
	protected:
		bool m_bIsStatic;
		bool m_bIsChanged;
	public:
		bool m_bEnable;

};
DECLARE_Ptr(SKSpatial);
SKTYPE_MARCO(SKSpatial);
}
#endif