#ifndef SKTUBESURFACE3_H
#define SKTUBESURFACE3_H
#include "SKCurve3.h"
#include "SKSurface3.h"
/*


*/
namespace SKEngine2
{
	class SKMATH_API SKTubeSurface3 :public SKSurface3
	{
	public:
		SKTubeSurface3();
		~SKTubeSurface3();
		//ߣU,Vĳ
		bool SetCurve(SKCurve3 * pCurve,SKREAL fN,SKREAL fB);	
		virtual SKVector3 GetPoint(SKREAL U,SKREAL V);
		virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V);
		virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V);
	private:
		SKCurve3 *m_pCurve;
		SKREAL m_fN;
		SKREAL m_fB;

	};


}
#endif