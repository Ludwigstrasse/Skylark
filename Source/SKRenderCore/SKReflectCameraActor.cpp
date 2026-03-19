#include "SKReflectCameraActor.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKWorld.h"
#include "SKViewFamily.h"
#include "SKLine3.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKReflectCameraActor, SKActor)
BEGIN_ADD_PROPERTY(SKReflectCameraActor, SKActor)
REGISTER_PROPERTY(m_pTargetCameraActor, TargetCameraActor, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKReflectCameraActor)
IMPLEMENT_INITIAL_END
SKReflectCameraActor::SKReflectCameraActor()
{

}
SKReflectCameraActor::~SKReflectCameraActor()
{

}
void SKReflectCameraActor::Update(double dAppTime)
{
	SKCameraActor::Update(dAppTime);

	if (m_pTargetCameraActor)
	{
		SKCamera * pTargetCamera = m_pTargetCameraActor->GetTypeNode();
		SKCamera * pCamera = GetTypeNode();
		if (pCamera && pTargetCamera)
		{
			pCamera->SetPerspectiveFov(pTargetCamera->GetFov(), pTargetCamera->GetAspect(), pTargetCamera->GetZNear(), pTargetCamera->GetZFar());

			SKVector3 TargetCameraWorldPos =  pTargetCamera->GetWorldTranslate();

			SKVector3 N = m_Plane.GetN();

			SKLine3 Line1(TargetCameraWorldPos, N);

			float t1 = 0;
			m_Plane.RelationWith(Line1, false, t1);

			SKVector3 k1 = Line1.GetParameterPoint(t1);

			SKVector3 CameraWorldPos = k1 + k1 - TargetCameraWorldPos;

			SKVector3 Dir, Up, Right;
			pTargetCamera->GetWorldDir(Dir, Up, Right);


			SKVector3 CameraLookDir = m_Plane.ReflectDir(Dir);
			CameraLookDir.Normalize();

			SKVector3 CameraUp = m_Plane.ReflectDir(Up);
			CameraUp.Normalize();
			
			pCamera->CreateFromLookDirWorld(CameraWorldPos, CameraLookDir, CameraUp);
		}
	}
	
}
void SKReflectCameraActor::SetTargetCameraActor(SKCameraActor * pCameraActor)
{
	m_pTargetCameraActor = DynamicCast<SKReflectCameraActor>(pCameraActor);
	if (m_pTargetCameraActor == NULL)
	{
		m_pTargetCameraActor = pCameraActor;
	}
}
void SKReflectCameraActor::SetPlane(SKPlane3 Plane)
{
	m_Plane = Plane;
	AddCustomCullPlane(m_Plane);
}