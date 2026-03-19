#ifndef SKREFLECTCAMERAACTOR_H
#define SKREFLECTCAMERAACTOR_H
#include "SKReflectCameraActor.h"
#include "SKCameraActor.h"
#include "SKPostEffectSet.h"
namespace SKEngine2
{
	class  SKGRAPHIC_API SKReflectCameraActor : public SKCameraActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		SKReflectCameraActor();
		virtual ~SKReflectCameraActor();
		DECLARE_INITIAL

		virtual void Update(double dAppTime);

		void SetTargetCameraActor(SKCameraActor * pCameraActor);
		void SetPlane(SKPlane3 Plane);
	protected:
		SKCameraActor * m_pTargetCameraActor;
		SKPlane3 m_Plane;
	};
	DECLARE_Ptr(SKReflectCameraActor);
	SKTYPE_MARCO(SKReflectCameraActor);
}
#endif