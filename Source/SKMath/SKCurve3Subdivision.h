#ifndef SKCURVE3SUBDIVISION_H
#define SKCURVE3SUBDIVISION_H
#include "SKControlCurve3.h"
/*
߻ַ

*/
namespace SKEngine2
{
class SKMATH_API SKCurve3Subdivision : public SKMemObject
{
public:
	enum	//Subdivision Method
	{
		SM_ERROR,
		SM_LENGTH,
		SM_UNIFORM,
		SM_MAX
	};
	SKCurve3Subdivision();
	virtual ~SKCurve3Subdivision() = 0;
	//
	bool SetSubCurve(SKControlCurve3 * pCurve);
    //õֺĵ
	FORCEINLINE SKVector3 *GetPoints()const;
	////õֺĵ
	FORCEINLINE unsigned int GetPointNum()const;
	//õ
	FORCEINLINE SKControlCurve3 *GetCurve()const;
	//õֺߵʱ
	FORCEINLINE SKREAL *GetT()const;
	//ָߵĳƵ仯лֺĵ
	bool ControlPointChange(unsigned int i);
	//
	virtual bool Subdivision() = 0;

	virtual unsigned int GetSubdivisionMethod() = 0;
protected:
	SKVector3 *m_pPoint;
	unsigned int m_uiPointNum;
	SKControlCurve3 *m_pCurve;
	SKREAL			*m_pT;
	SKREAL m_ft1;
	SKREAL m_ft2;
};

#include "SKCurve3Subdivision.inl"
}
#endif