#ifndef SKORDINARYLINESET_H
#define SKORDINARYLINESET_H
#include "SKLineSet.h"
#include "SKCurve3.h"
#include "SKCurve3Subdivision.h"
#include "SKLine3.h"
#include "SKRay3.h"
#include "SKSegment3.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKOrdinaryLineSet : public SKLineSet
	{
	public:
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		SKOrdinaryLineSet(SKControlCurve3 * pControlCurve,SKCurve3Subdivision * pSubidvision);
		SKOrdinaryLineSet(const SKLine3 & Line);
		SKOrdinaryLineSet(const SKRay3 & Ray);
		SKOrdinaryLineSet(const SKSegment3 & Segment);
		virtual ~SKOrdinaryLineSet();
	
	protected:
		SKOrdinaryLineSet();

	};
	DECLARE_Ptr(SKOrdinaryLineSet);
	SKTYPE_MARCO(SKOrdinaryLineSet);

};
#endif