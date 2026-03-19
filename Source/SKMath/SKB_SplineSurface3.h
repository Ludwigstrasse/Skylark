#ifndef SKB_SPLINESURFACE3_H
#define SKB_SPLINESURFACE3_H
#include "SKControlSurface3.h"
namespace SKEngine2
{
class SKMATH_API SKB_SplineSurface3 : public SKControlSurface3
{
public:
	SKB_SplineSurface3();
	virtual ~SKB_SplineSurface3();
	bool Copy(const SKB_SplineSurface3 & B_SplineSurface3);
	bool Set(const SKVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn);

	bool SetUKnotVector(const SKREAL * pUKnotVector,unsigned int uiUDegree,unsigned int uiRow);
	bool SetVKnotVector(const SKREAL * pVKnotVector,unsigned int uiVDegree,unsigned int uiColumn);

	bool CreateOpenUniformUKnotVector(unsigned int uiDegree,unsigned int uiRow);
	bool CreatePeriodicUKnotVector(unsigned int uiDegree,unsigned int uiRow);

	bool CreateOpenUniformVKnotVector(unsigned int uiDegree,unsigned int uiColumn);
	bool CreatePeriodicVKnotVector(unsigned int uiDegree,unsigned int uiColumn);
	int GetUKey(SKREAL t)const;
	int GetVKey(SKREAL t)const;

	virtual SKVector3 GetPoint(SKREAL U,SKREAL V) = 0;
	virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V) = 0;
	virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V) = 0;
public:
	/***************************************FORCEINLINE************************************/
	FORCEINLINE bool SetUKnotVector(unsigned int i,SKREAL Value);
	FORCEINLINE const SKREAL * GetpUKnotVector()const;
	FORCEINLINE SKREAL GetUKnotVectorValue(unsigned int i)const;

	FORCEINLINE bool SetVKnotVector(unsigned int i,SKREAL Value);
	FORCEINLINE const SKREAL * GetpVKnotVector()const;
	FORCEINLINE SKREAL GetVKnotVectorValue(unsigned int i)const;
protected:
	FORCEINLINE void SetUBasisValue(int i, int j, SKREAL Value);
	FORCEINLINE SKREAL GetUBasisValue(int i ,int j)const;

	FORCEINLINE void SetUBasisFDValue(int i, int j, SKREAL Value);
	FORCEINLINE SKREAL GetUBasisFDValue(int i ,int j)const;


	FORCEINLINE unsigned int GetUDegree()const;
	FORCEINLINE unsigned int GetUKnotNum()const;


	FORCEINLINE void SetVBasisValue(int i, int j, SKREAL Value);
	FORCEINLINE SKREAL GetVBasisValue(int i ,int j)const;

	FORCEINLINE void SetVBasisFDValue(int i, int j, SKREAL Value);
	FORCEINLINE SKREAL GetVBasisFDValue(int i ,int j)const;


	FORCEINLINE unsigned int GetVDegree()const;
	FORCEINLINE unsigned int GetVKnotNum()const;

protected:
	bool GetUFuncValue(SKREAL t,int &i,unsigned int uiOrder);
	bool GetVFuncValue(SKREAL t,int &i,unsigned int uiOrder);

	SKREAL		* m_pUKnotVector;
	unsigned int m_uiUDegree;
	unsigned int m_uiUKnotNum;
	SKREAL		*m_pUBasis;
	SKREAL		*m_pUBasisFD;
	unsigned int m_uiUBasisNum;


	SKREAL		* m_pVKnotVector;
	unsigned int m_uiVDegree;
	unsigned int m_uiVKnotNum;
	SKREAL		*m_pVBasis;
	SKREAL		*m_pVBasisFD;
	unsigned int m_uiVBasisNum;
};
#include "SKB_SplineSurface3.inl"
}
#endif
