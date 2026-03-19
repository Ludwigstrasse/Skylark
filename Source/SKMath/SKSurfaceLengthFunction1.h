#ifndef SKSURFACELENGTHFUNCTION1_H
#define SKSURFACELENGTHFUNCTION1_H
#include "SKFunction1.h"
#include "SKSurface3.h"
/*
߳Ⱥ
*/
namespace SKEngine2
{
	//the function is sqrt(dy/dt * dy/dt + dx/dt * dx/dt);
	class SKMATH_API SKSurfaceLengthFunction1:public SKFunction1
	{
	public:
		enum // length type
		{
			LT_U_LEN,
			LT_V_LEN
		};
		SKSurfaceLengthFunction1(unsigned int uiLengthType,SKREAL fTypePara);
		~SKSurfaceLengthFunction1();
		//õֵ
		virtual SKREAL GetValue(SKREAL fParameter);
		// to do 
		virtual SKREAL GetFirstDerivative(SKREAL fParameter);
		virtual SKREAL GetSecondDerivative(SKREAL fParameter);
		virtual SKREAL GetThirdDerivative(SKREAL fParameter);
		//
		FORCEINLINE void SetSurface(SKSurface3 *pSurface){if(!pSurface) return ; m_pSurface = pSurface;}
		FORCEINLINE SKSurface3 *GetSurface(){return m_pSurface;}
	protected:
		SKSurface3 *m_pSurface;
		SKREAL m_fTypePara;
		unsigned int m_uiLengthType;
	};


}
#endif