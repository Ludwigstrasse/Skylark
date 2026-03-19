#ifndef SKCURVESUBDIVISIONBYERROR_H
#define SKCURVESUBDIVISIONBYERROR_H
#include "SKCurve3Subdivision.h"
/*
	ַ



*/
namespace SKEngine2
{
	class SKMATH_API SKCurve3SubdivisionByError :public SKCurve3Subdivision
	{
	public:
		SKCurve3SubdivisionByError(SKREAL t1 = 0.0f,SKREAL t2 = 1.0f,
									unsigned int Level = 5, SKREAL error = EPSILON_E4);
		~SKCurve3SubdivisionByError();
		//ָݹκ t1,t2֮
		virtual bool Subdivision();
		

		virtual unsigned int GetSubdivisionMethod(){ return SM_ERROR;}
	protected:
		typedef struct  POINTLIST
		{
			SKVector3 Point;
			POINTLIST * next;
			SKREAL	  t;
		};
		POINTLIST * m_pPointHead;
		POINTLIST * m_pNext;
		bool SubdivisionBisect(unsigned int Level, SKREAL error,SKREAL t1,SKREAL t2,
							const SKVector3 &Point1,const SKVector3 &Point2);
		void Clear();
		unsigned int m_uiLevel;
		SKREAL m_fError;
	};


}
#endif