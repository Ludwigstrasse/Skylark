#ifndef SKORDINARYGEOMETRY_H
#define SKORDINARYGEOMETRY_H
#include "SKTriangleSet.h"
#include "SKAABB3.h"
#include "SKOBB3.h"
#include "SKSurface3.h"
#include "SKSurface3Subdivision.h"
#include "SKPolygon3.h"
#include "SKSphere3.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKOrdinaryTriangleSet : public SKTriangleSet
	{
	public:
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKOrdinaryTriangleSet(const SKAABB3 & AABB);
		SKOrdinaryTriangleSet(const SKOBB3 & OBB);
		SKOrdinaryTriangleSet(const SKSphere3 & Sphere);
		SKOrdinaryTriangleSet(SKSurface3 * pSurface ,SKSurface3Subdivision * pSubidvision);
		SKOrdinaryTriangleSet(const SKPlane3 &Plane);
		SKOrdinaryTriangleSet(const SKTriangle3 &Triangle);
		SKOrdinaryTriangleSet(const SKRectangle3 &Rectangle);
		SKOrdinaryTriangleSet(const SKPolygon3 &Polyon);
		virtual ~SKOrdinaryTriangleSet();

	protected:
		SKOrdinaryTriangleSet();

	};
	DECLARE_Ptr(SKOrdinaryTriangleSet);
	SKTYPE_MARCO(SKOrdinaryTriangleSet);
};
#endif