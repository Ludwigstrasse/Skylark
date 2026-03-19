#ifndef SKBEZIERSURFACE3_H
#define SKBEZIERSURFACE3_H
#include "SKControlSurface3.h"
/*
Beizer棬2άƵ2ʽƣU,V0-1֮



*/
namespace SKEngine2
{
class SKMATH_API SKBezierSurface3 : SKControlSurface3
{
public:
	SKBezierSurface3();
	virtual ~SKBezierSurface3();
	//ÿƵ
	bool Set(const SKVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn);
	//
	bool Copy(const SKBezierSurface3 & BezierSurface3);
	//
	virtual SKVector3 GetPoint(SKREAL U,SKREAL V);
	//ƫ
	virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V);
	virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V);
private:
	/***************************************FORCEINLINE************************************/
	FORCEINLINE SKREAL GetUC(int i,int j)const;
	FORCEINLINE SKREAL SetUC(int i,int j,SKREAL Value);
	FORCEINLINE SKREAL GetVC(int i,int j)const;
	FORCEINLINE SKREAL SetVC(int i,int j,SKREAL Value);
	FORCEINLINE void SetUPD(int i, int j,const SKVector3 & Value);
	FORCEINLINE SKVector3 GetUPD(int i, int j);
	FORCEINLINE void SetVPD(int i, int j,const SKVector3 & Value);
	FORCEINLINE SKVector3 GetVPD(int i, int j);
	SKREAL * m_UC;
	SKREAL * m_VC;
	SKVector3 * m_UPDValue;
	SKVector3 * m_VPDValue;
};

#include "SKBezierSurface3.inl"
}
#endif
