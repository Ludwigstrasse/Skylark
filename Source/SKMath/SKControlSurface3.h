#ifndef SKCONTROLSURFACE3_H
#define SKCONTROLSURFACE3_H
#include "SKSurface3.h"
/*
࣬еĿƵ涼̳࣬ûʵ
*/
namespace SKEngine2
{
	class SKMATH_API SKControlSurface3 : public SKSurface3
	{
	public:
		SKControlSurface3();
		virtual ~SKControlSurface3() = 0;
		//
		bool Copy(const SKControlSurface3& ControlSurface3);
		//õ
		virtual SKVector3 GetPoint(SKREAL U,SKREAL V) = 0;
		//ƫ
		virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V) = 0;
		virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V) = 0;
		/****************************FORCEINLINE********************************************/
		//õƵ
		FORCEINLINE SKVector3 GetControlPoint(unsigned int U,unsigned int V)const;
		//ıƵ
		FORCEINLINE bool ChangeControlPoint(unsigned int U,unsigned int V,const SKVector3& Value);
		//õƵָ
		FORCEINLINE const SKVector3 *GetControlPoint()const;
		//ÿƵ
		FORCEINLINE unsigned int GetControlPointNum()const;
		//
		FORCEINLINE unsigned int GetRow()const;
		//
		FORCEINLINE unsigned int GetColumn()const;

		SKREAL GetULength(SKREAL V,SKREAL U1, SKREAL U2,unsigned int uiIterations = 32);
		SKREAL GetVLength(SKREAL U,SKREAL V1, SKREAL V2,unsigned int uiIterations = 32);
		SKREAL GetUTime (SKREAL V,SKREAL fLength, unsigned int uiIterations = 32,SKREAL fTolerance = EPSILON_E4);
		SKREAL GetVTime (SKREAL U,SKREAL fLength, unsigned int uiIterations = 32,SKREAL fTolerance = EPSILON_E4);
	protected:
		SKVector3 * m_pControlPoint;
		unsigned int m_uiControlPointNum;
		unsigned int m_uiRow;
		unsigned int m_uiColumn;
	};

#include "SKControlSurface3.inl"

}
#endif