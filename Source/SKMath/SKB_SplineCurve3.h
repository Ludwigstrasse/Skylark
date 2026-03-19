#ifndef SKB_SPLINECURVE3_H
#define SKB_SPLINECURVE3_H
#include "SKControlCurve3.h"
/*
	B_SplineߣڿƵһ֣ɿƵ㣬ߵĶȣʽ,t0-1֮

*/
namespace SKEngine2
{

	class SKMATH_API SKB_SplineCurve3:public SKControlCurve3
	{
	public:
		SKB_SplineCurve3();
		virtual ~SKB_SplineCurve3();
		//
		bool Copy(const SKB_SplineCurve3 & B_SplineCurve3);
		//ÿƵ
		bool Set(const SKVector3 * pControlPoint,unsigned int uiControlPointNum);
		//ѿƵ任ɱպϿʽƵ
		bool ChangeToOpenClosePoints();
		//ѿƵ任ڱպϿƵ
		bool ChangeToPeriodicClosePoints(unsigned int uiDegree);
		//ý
		bool SetKnotVector(const SKREAL * pKnotVector,unsigned int uiDegree,unsigned int uiControlPointNum);
		//Ⱦ
		bool CreateOpenUniformKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum);
		//ڽ
		bool CreatePeriodicKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum);
		//
		virtual SKVector3	GetFirstDerivative(SKREAL t);
		virtual SKVector3	GetSecondDerivative(SKREAL t);
		virtual SKVector3	GetThirdDerivative(SKREAL t);
		//virtual SKREAL		GetLength(SKREAL t1 ,SKREAL t2);
		//õĵ
		virtual SKVector3	GetPoint(SKREAL t);

	/***************************************FORCEINLINE************************************/

	public:
		//ýһֵ
		FORCEINLINE bool SetKnotVector(unsigned int i,SKREAL Value);
		//õָ
		FORCEINLINE const SKREAL * GetpKnotVector()const;
		//õһֵ
		FORCEINLINE SKREAL GetKnotVectorValue(unsigned int i)const;
	protected:
		//ʽֵ
		FORCEINLINE void SetBasisValue(int i, int j, SKREAL Value);
		FORCEINLINE SKREAL GetBasisValue(int i ,int j)const;

		FORCEINLINE void SetBasisFDValue(int i, int j, SKREAL Value);
		FORCEINLINE SKREAL GetBasisFDValue(int i ,int j)const;

		FORCEINLINE void SetBasisSDValue(int i, int j, SKREAL Value);
		FORCEINLINE SKREAL GetBasisSDValue(int i ,int j)const;

		FORCEINLINE void SetBasisTDValue(int i, int j, SKREAL Value);
		FORCEINLINE SKREAL GetBasisTDValue(int i ,int j)const;
	public:
		//ض
		FORCEINLINE unsigned int GetDegree()const;
		//ؽ
		FORCEINLINE unsigned int GetKnotNum()const;
	protected:
		int GetKey(SKREAL t)const;
		bool GetFuncValue(SKREAL t,int &i,unsigned int uiOrder);
	protected:
		SKREAL		* m_pKnotVector;
		unsigned int m_uiDegree;
		unsigned int m_uiKnotNum;
		SKREAL		*m_pBasis;
		SKREAL		*m_pBasisFD;
		SKREAL		*m_pBasisSD;
		SKREAL		*m_pBasisTD;
		unsigned int m_uiBasisNum;
		

	};

#include "SKB_SplineCurve3.inl"
}
#endif