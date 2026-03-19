#ifndef SKGENERALIZEDCYLINDERSURFACE3_H
#define SKGENERALIZEDCYLINDERSURFACE3_H
#include "SKCurve3.h"
#include "SKSurface3.h"
/*
	һ

*/
namespace SKEngine2
{
	class SKMATH_API SKGeneralizedCylinderSurface3 :public SKSurface3
	{
	public:

		SKGeneralizedCylinderSurface3();
		~SKGeneralizedCylinderSurface3();
		bool SetCurve(SKCurve3 * pCurve1,SKCurve3 *pCurve2);
		virtual SKVector3 GetPoint(SKREAL U,SKREAL V);
		virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V);
		virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V);
	private:
		SKCurve3 *m_pCurve1;
		SKCurve3 *m_pCurve2;
	
	};


}
#endif