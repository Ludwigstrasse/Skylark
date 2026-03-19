#include "SKFreeCameraController.h"
#include "SKCamera.h"
#include "SKQuat.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKFreeCameraController,SKController)
BEGIN_ADD_PROPERTY(SKFreeCameraController,SKController)
REGISTER_PROPERTY(m_RotXDelta,RotXDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotYDelta,RotYDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotZDelta,RotZDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveDelta,MoveDelta,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveDirection,MoveDirection,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKFreeCameraController)
IMPLEMENT_INITIAL_END
SKFreeCameraController::SKFreeCameraController()
{
	m_RotXDelta = 1.0;
	m_RotYDelta = 1.0;
	m_RotZDelta = 1.0;
	m_MoveDelta = 1.0;
}
SKFreeCameraController:: ~SKFreeCameraController()
{

}
bool SKFreeCameraController::UpdateEx(double dAppTime)
{
	if(!SKController::UpdateEx(dAppTime))
		return 0;


	//SKFreeCamera * Temp = (SKFreeCamera *)m_pObject;

	SKCamera* Temp = DynamicCast<SKCamera>(m_pObject);
	if(!Temp)
		return 0;
	Temp->m_RotX += m_RotXDelta * (SKREAL)m_dIntervalTime;
	Temp->m_RotY += m_RotYDelta * (SKREAL)m_dIntervalTime;
	Temp->m_RotZ += m_RotZDelta * (SKREAL)m_dIntervalTime;
	
	
	if (Temp->m_RotX > VS2PI) Temp->m_RotX -= VS2PI;
	else if (Temp->m_RotX < -VS2PI) Temp->m_RotX += VS2PI;

	if (Temp->m_RotY > VS2PI) Temp->m_RotY -= VS2PI;
	else if (Temp->m_RotY < -VS2PI) Temp->m_RotY += VS2PI;

	if (Temp->m_RotZ > VS2PI) Temp->m_RotZ -= VS2PI;
	else if (Temp->m_RotZ < -VS2PI) Temp->m_RotZ += VS2PI;
	
	SKVector3 LocalPos = Temp->GetLocalTranslate();
 	LocalPos += m_MoveDirection * m_MoveDelta * (SKREAL)m_dIntervalTime;
			
	SKQuat    qFrame(0,0,0,1);	
	qFrame.CreateEuler(Temp->m_RotZ, Temp->m_RotX, Temp->m_RotY);

	SKMatrix3X3 Mat;
	Mat.Identity();
	qFrame.GetMatrix(Mat);

	Temp->SetLocalRotate(Mat);
	Temp->SetLocalTranslate(LocalPos);
	return 1;
}



