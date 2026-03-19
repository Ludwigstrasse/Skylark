#ifndef SKFREECAMERACONTROLLER_H
#define SKFREECAMERACONTROLLER_H
#include "SKMath.h"
#include "SKVector3.h"
#include "SKController.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKFreeCameraController : public SKController
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKFreeCameraController();
		virtual ~SKFreeCameraController();
		SKREAL m_RotXDelta;
		SKREAL m_RotYDelta;
		SKREAL m_RotZDelta;
		SKREAL m_MoveDelta;
		SKVector3 m_MoveDirection;
		virtual bool UpdateEx(double dAppTime);

	};
	DECLARE_Ptr(SKFreeCameraController);
	SKTYPE_MARCO(SKFreeCameraController);

}
#endif