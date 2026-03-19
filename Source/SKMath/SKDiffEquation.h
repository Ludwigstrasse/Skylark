#ifndef SKDIFFEQUATION_H
#define SKDIFFEQUATION_H
#include "SKFunction1.h"
/*
	һԪ΢ַ


*/
namespace SKEngine2
{
class SKMATH_API SKDiffEquation
{
public:
	SKDiffEquation();
	~SKDiffEquation();
	//ԭ
	FORCEINLINE void SetFunction1(SKFunction1 * Func){if(!Func) return ; m_pFunc = Func;}
	FORCEINLINE SKFunction1 *GetFunc(){return m_pFunc;}
    //ò
	FORCEINLINE void SetStep(SKREAL Step){if(ABS(Step) < EPSILON_E4) return; m_Step = Step;}
	FORCEINLINE SKREAL GetStep(){return m_Step;}
	//ÿʼԱ
	FORCEINLINE void SetBeginT(SKREAL BeginT){m_BeginT = BeginT;}
	FORCEINLINE SKREAL GetBeginT(){return m_BeginT;}
	//ÿʼֵ
	FORCEINLINE void SetBeginX(SKREAL BeginX){m_BeginX = BeginX;}
	FORCEINLINE SKREAL GetBeginX(){return m_BeginX;}
	//
	void Update();
protected:
	SKFunction1 * m_pFunc;
	SKREAL m_Step;
	SKREAL m_BeginT;
	SKREAL m_BeginX;

};



}
#endif