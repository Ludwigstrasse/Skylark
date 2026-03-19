#ifndef SKREVOLUTIONSURFACE3
#define SKREVOLUTIONSURFACE3
#include "SKCurve3.h"
#include "SKSurface3.h"
/*
ת

*/
namespace SKEngine2
{
	class SKMATH_API SKRevolutionSurface3 :public SKSurface3
	{
	public:
		enum{XAXIS,YAXIS,ZAXIS};
		enum{XYPLANE,XZPLANE,YZPLANE};
		SKRevolutionSurface3();
		~SKRevolutionSurface3();
		//ߣתᣬƽ
		bool SetCurve(SKCurve3 * pCurve,unsigned int Axis,unsigned int uiPlane);	
		virtual SKVector3 GetPoint(SKREAL U,SKREAL V);
		virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V);
		virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V);
	private:
		SKCurve3 *m_pCurve;
		unsigned int m_uiAxis;
		unsigned int m_uiPlane;
	
	
	};

}
#endif