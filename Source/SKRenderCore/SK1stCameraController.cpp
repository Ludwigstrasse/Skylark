#include "SK1stCameraController.h"
#include "SKCamera.h"
#include "SKQuat.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(VS1stCameraController,SKController)
BEGIN_ADD_PROPERTY(VS1stCameraController,SKController)
REGISTER_PROPERTY(m_RotXDelta,RotXDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotYDelta,RotYDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveXDelta,MoveXDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveYDelta,MoveYDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveZDelta,MoveZDelta,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS1stCameraController)
IMPLEMENT_INITIAL_END
VS1stCameraController::VS1stCameraController()
{
	m_RotXDelta = 0.0f;
	m_RotYDelta = 0.0f;
	m_MoveXDelta = 0.0f;
	m_MoveYDelta = 0.0f;
	m_MoveZDelta = 0.0f;
}
VS1stCameraController:: ~VS1stCameraController()
{

}
bool VS1stCameraController::UpdateEx(double dAppTime)
{
	if(!SKController::UpdateEx(dAppTime))
		return 0;


	//SKFreeCamera * Temp = (SKFreeCamera *)m_pObject;

	SKCamera* Temp = DynamicCast<SKCamera>(m_pObject);
	if(!Temp)
		return 0;
	Temp->m_RotX += m_RotXDelta * (SKREAL)m_dIntervalTime;
	Temp->m_RotY += m_RotYDelta * (SKREAL)m_dIntervalTime;
	Temp->m_RotZ  = 0;

	if (Temp->m_RotX > 1.4707963f) Temp->m_RotX = 1.4707963f;
	else if (Temp->m_RotX < -1.4707963f) Temp->m_RotX = -1.4707963f;

	if (Temp->m_RotY > VS2PI) Temp->m_RotY -= VS2PI;
	else if (Temp->m_RotY < -VS2PI) Temp->m_RotY += VS2PI;


	SKVector3 LocalPos = Temp->GetLocalTranslate();
	SKMatrix3X3 Rot = Temp->GetLocalRotate();
	SKVector3 Velocity(m_MoveXDelta,m_MoveYDelta,m_MoveZDelta);
	LocalPos += Velocity * Rot * (SKREAL)m_dIntervalTime;

	SKQuat    qFrame(0,0,0,1);			

	qFrame.CreateEuler(Temp->m_RotZ,Temp->m_RotX,Temp->m_RotY );

	SKMatrix3X3 Mat;
	Mat.Identity();
	qFrame.GetMatrix(Mat);

	Temp->SetLocalRotate(Mat);
	Temp->SetLocalTranslate(LocalPos);

	return 1;
}
