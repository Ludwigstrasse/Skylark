#include "SKCamera.h"
#include "SKRenderer.h"
#include "SKGraphicInclude.h"
#include "SK2DTexture.h"
#include "SKDepthStencil.h"
#include "SKStream.h"
#include "SKSceneManager.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKCamera, SKNodeComponent)
BEGIN_ADD_PROPERTY(SKCamera, SKNodeComponent)
REGISTER_PROPERTY(m_RotX,RotX,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotY,RotY,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotZ,RotZ,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Fov, Fov, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Aspect,Aspect,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ZFar, ZFar, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ZNear, ZNear, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ViewPort, ViewPort, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ViewMat,ViewMat,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ProjMat,ProjMat,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_CustomCullPlane, CustomCullPlane, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCamera)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
SKPointer<SKCamera> SKCamera::Default;
bool SKCamera::InitialDefaultState()
{
	SKCamera *p = NULL;
	p = SK_NEW SKCamera();

	if(p)
	{
		Default = p;
		
	}
	else
		return 0;

	return 1;
}
bool SKCamera::TerminalDefaultState()
{
	Default = NULL;
	return 1;
}

SKCamera::SKCamera()
{
	m_ViewMat.Identity();
	m_ProjMat.Identity();
	m_RotX = 0.0;						
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Fov = (SKREAL)0.8;
	m_Aspect =(SKREAL)(4.0 / 3.0);
	m_ZNear= (SKREAL)1.0;
	m_ZFar = (SKREAL)8000.0;
	m_ProjMat.CreatePerspective(m_Fov,m_Aspect,m_ZFar,m_ZNear);
	m_bEnable = true;

}
SKCamera::~SKCamera()
{
	for (unsigned int i = 0 ; i < m_ViewFamilyArray.GetNum() ; i++)
	{
		SKSceneManager::ms_pSceneManager->DeleteViewFamily(m_ViewFamilyArray[i]);
		SKMAC_DELETE(m_ViewFamilyArray[i]);
	}

}
void SKCamera::CreateFromEuler(const SKVector3 &Pos,SKREAL RotX,SKREAL RotY , SKREAL RotZ)
{
	SetLocalTranslate(Pos);
	m_RotX = RotX;
	m_RotY = RotY;
	m_RotZ = RotZ;
	SKQuat    qFrame(0,0,0,1);			

	qFrame.CreateEuler(m_RotY,m_RotX,  m_RotZ);

	SKMatrix3X3 Mat;
	Mat.Identity();
	qFrame.GetMatrix(Mat);
	SetLocalRotate(Mat);

}
void SKCamera::CreateFromLookDir(const SKVector3 &Pos,
					   const SKVector3 &vcDir,
					   const SKVector3 &vcUp)
{

	SKMatrix3X3W MatTemp;
	MatTemp.CreateFromLookDir(Pos,vcDir,vcUp);

	SKMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);
	SKMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);

	MatInv.GetEluer(m_RotZ,m_RotX,m_RotY);
	SetLocalRotate(MatInv);
	SetLocalTranslate(Pos);
}
void SKCamera::CreateFromLookAt(const SKVector3 &vcPos,									
					  const SKVector3 &vcLookAt,							
					  const SKVector3 &vcUp)
{
	SKMatrix3X3W MatTemp;
	MatTemp.CreateFromLookAt(vcPos,vcLookAt,vcUp);

	SKMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);

	SKMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);

	MatInv.GetEluer(m_RotZ,m_RotX,m_RotY); 
	SetLocalRotate(MatInv);
	SetLocalTranslate(vcPos);
}
void SKCamera::CreateFromLookDirWorld(const SKVector3 &Pos,
	const SKVector3 &vcDir,
	const SKVector3 &vcUp)
{

	SKMatrix3X3W MatTemp;
	MatTemp.CreateFromLookDir(Pos, vcDir, vcUp);

	SKMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);
	SKMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);

	SetWorldRotate(MatInv);
	SetWorldTranslate(Pos);
	m_Local.GetRotate().GetEluer(m_RotZ,m_RotX,  m_RotY);
}
void SKCamera::CreateFromLookAtWorld(const SKVector3 &vcPos,
	const SKVector3 &vcLookAt,
	const SKVector3 &vcUp)
{
	SKMatrix3X3W MatTemp;
	MatTemp.CreateFromLookAt(vcPos, vcLookAt, vcUp);

	SKMatrix3X3 Mat;
	MatTemp.Get3X3(Mat);

	SKMatrix3X3 MatInv;
	MatInv.InverseOf(Mat);
	SetWorldRotate(MatInv);
	SetWorldTranslate(vcPos);
	m_Local.GetRotate().GetEluer(m_RotZ,m_RotX,  m_RotY);
}
bool SKCamera::SetAspect(SKREAL Aspect)
{
	m_Aspect = Aspect;
	return m_ProjMat.CreatePerspective(m_Fov,m_Aspect,m_ZNear,m_ZFar);
}
bool SKCamera::SetPerspectiveFov(SKREAL fFov ,				//XŽ
					   SKREAL Aspect,			//߱
					   SKREAL fZN ,		//
					   SKREAL fZF)		//Զ
{

	m_Fov = fFov;
	m_Aspect = Aspect;
	m_ZFar = fZF;
	m_ZNear = fZN;
	return m_ProjMat.CreatePerspective(m_Fov,m_Aspect,m_ZNear,m_ZFar);
}
bool SKCamera::SetOrthogonal(SKREAL fW ,				//
				   SKREAL fH,					//
				   SKREAL fZN ,				//
				   SKREAL fZF)				//Զ
{
	m_ZFar = fZF;
	m_ZNear = fZN;
	return m_ProjMat.CreateOrthogonal(fW,fH,m_ZNear,m_ZFar);
}
/*----------------------------------------------------------------*/
/*
ƽ

ViewProjΪͶӰĳ˻
(x,y,z,1)һ㾭任Ϊ(x',y',z',1)

(x,y,z,1)ViewProj = (x',y',z',1)''''''''''''''''''''''''''''''''''(1)

任ĵķΧ-1 <= x <= 1 , -1 <= y <= 1 ,0 <= z <= 1
Ҳ̨˳,6,÷
leftΪ-x-1 = 0( x + 1 = 0 )
nearΪ -z = 0
ͬ

ƽλµķΪax + by + cz + d = 0

				a	
				b
(x,y,z,1)(	c	) = 0				'''''''''''''''''''''''''(2)
				d

ƽ任ķΪa'x + b'y + c'z + d' = 0

					a'	
					b'
(x',y',z',1)(	c'	) = 0			''''''''''''''''''''''''''(3)
					d'
(3)(1):

						a'
						b'
(x,y,z,1)ViewProj(	c'	) = 0		'''''''''''''''''''''''''''''(4)
						d'
(4)(2)

	a					a'
	b					b'
(	c	) = ViewProj(	c'	)
	d					d'

ƽͶӰ̶֪a',b',c',d'֪),a,b,c,d
*/
void SKCamera::GetPlane(SKPlane3 Plane[SKCamera::CP_MAX])const
{
	SKMatrix3X3W ViewProj;
	ViewProj = m_ViewMat * m_ProjMat;

	SKVector3 N;
	SKREAL  fD;

	// right plane
	N.x = -(ViewProj._03 - ViewProj._00);
	N.y = -(ViewProj._13 - ViewProj._10);
	N.z = -(ViewProj._23 - ViewProj._20);
	fD    = -(ViewProj._33 - ViewProj._30);
	Plane[0].Set(N,fD);

	// left plane

	N.x = -(ViewProj._03 + ViewProj._00);
	N.y = -(ViewProj._13 + ViewProj._10);
	N.z = -(ViewProj._23 + ViewProj._20);
	fD    = -(ViewProj._33 + ViewProj._30);
	Plane[1].Set(N,fD);
	
	// top plane
	N.x = -(ViewProj._03 - ViewProj._01);
	N.y = -(ViewProj._13 - ViewProj._11);
	N.z = -(ViewProj._23 - ViewProj._21);
	fD    = -(ViewProj._33 - ViewProj._31);
	Plane[2].Set(N,fD);
	// bottom plane
	N.x = -(ViewProj._03 + ViewProj._01);
	N.y = -(ViewProj._13 + ViewProj._11);
	N.z = -(ViewProj._23 + ViewProj._21);
	fD    = -(ViewProj._33 + ViewProj._31);
	Plane[3].Set(N,fD);

	// far plane
	N.x = -(ViewProj._03 - ViewProj._02);
	N.y = -(ViewProj._13 - ViewProj._12);
	N.z = -(ViewProj._23 - ViewProj._22);
	fD    = -(ViewProj._33 - ViewProj._32);
	Plane[4].Set(N,fD);

	// near plane
	N.x = -ViewProj._02;
	N.y = -ViewProj._12;
	N.z = -ViewProj._22;
	fD    = -ViewProj._32;
	Plane[5].Set(N,fD);
	


}
SKAABB3 SKCamera::GetFrustumAABB()
{
	SKVector3 Point[8];
	GetFrustumPoint(Point);

	SKAABB3 Aabb;
	Aabb.CreateAABB(Point,8);
	
	return Aabb;
}
void SKCamera::GetFrustumPoint(SKVector3 Point[8])
{
	SKMatrix3X3W ViewProj = m_ViewMat * m_ProjMat;

	SKMatrix3X3W ViewProjInv = ViewProj.GetInverse();

	Point[0] = SKVector3(1,1,0);
	Point[1] = SKVector3(1,-1,0);
	Point[2] = SKVector3(-1,1,0);
	Point[3] = SKVector3(-1,-1,0);

	Point[4] = SKVector3(1,1,1);
	Point[5] = SKVector3(1,-1,1);
	Point[6] = SKVector3(-1,1,1);
	Point[7] = SKVector3(-1,-1,1);

	for(unsigned int i = 0 ; i < 8 ;i++)
	{
		Point[i] = Point[i] * ViewProjInv;
	}
}
void SKCamera::UpdateCameraState(double dAppTime)
{
	SKNodeComponent::UpdateCameraState(dAppTime);
	m_pAllCamera.AddElement(this);
}
void SKCamera::UpdateTransform(double dAppTime) 
{ 

	SKNodeComponent::UpdateTransform(dAppTime);
	if(m_bIsChanged)
	{
 		SKTransform Trans = GetWorldTransform();
 		m_ViewMat = Trans.GetCombineInverse();
	}
}
void SKCamera::AddViewFamily(SKViewFamily * pViewFamily)
{
	if (pViewFamily)
	{
		m_ViewFamilyArray.AddElement(pViewFamily);
		SKSceneManager::ms_pSceneManager->AddViewFamily(pViewFamily);
	}
}

void SKCamera::DeleteViewFamily(SKViewFamily * pViewFamily)
{
	if (pViewFamily)
	{
		unsigned int uiID = m_ViewFamilyArray.FindElement(pViewFamily);
		if (uiID >= m_ViewFamilyArray.GetNum())
		{
			return ;
		}
		m_ViewFamilyArray.Erase(uiID);
		SKSceneManager::ms_pSceneManager->DeleteViewFamily(pViewFamily);
		SKMAC_DELETE(pViewFamily);
	}
}
void SKCamera::AddCustomCullPlane(const SKPlane3& P)
{
	m_CustomCullPlane.AddElement(P);
}
void SKCamera::ClearCustomCullPlane()
{
	m_CustomCullPlane.Clear();
}