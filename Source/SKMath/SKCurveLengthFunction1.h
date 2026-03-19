#ifndef SKCURVELENGTHFUNCTION1_H
#define SKCURVELENGTHFUNCTION1_H
#include "SKFunction1.h"
#include "SKCurve3.h"
/*
߳Ⱥ
*/
namespace SKEngine2
{
	//the function is sqrt(dy/dt * dy/dt + dx/dt * dx/dt);
	class SKMATH_API SKCurveLengthFunction1:public SKFunction1
	{
	public:
		SKCurveLengthFunction1();
		~SKCurveLengthFunction1();
		//õֵ
		virtual SKREAL GetValue(SKREAL fParameter);
		// to do 
		virtual SKREAL GetFirstDerivative(SKREAL fParameter);
		virtual SKREAL GetSecondDerivative(SKREAL fParameter);
		virtual SKREAL GetThirdDerivative(SKREAL fParameter);
		//
		FORCEINLINE void SetCurve(SKCurve3 *pCurve){if(!pCurve) return ; m_pCurve = pCurve;}
		FORCEINLINE SKCurve3 *GetCurve(){return m_pCurve;}
	protected:
		SKCurve3 *m_pCurve;
	};


}
#endif