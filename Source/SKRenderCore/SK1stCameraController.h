#ifndef VS1STCAMERACONTROLLER_H
#define VS1STCAMERACONTROLLER_H
#include "SKMath.h"
#include "SKVector3.h"
#include "SKController.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API VS1stCameraController : public SKController
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS1stCameraController();
		virtual ~VS1stCameraController();
		SKREAL m_RotXDelta;
		SKREAL m_RotYDelta;
		SKREAL m_MoveXDelta;
		SKREAL m_MoveYDelta;
		SKREAL m_MoveZDelta;
		virtual bool UpdateEx(double dAppTime);

	};
	DECLARE_Ptr(VS1stCameraController);
	SKTYPE_MARCO(VS1stCameraController);

}
#endif