#ifndef SKNURBSSURFACE3_H
#define SKNURBSSURFACE3_H
#include "SKB_SplineSurface3.h"
/*
NURB

*/
namespace SKEngine2
{
	class SKMATH_API SKNURBSSurface3:public SKB_SplineSurface3
	{
	public:
		SKNURBSSurface3();
		~SKNURBSSurface3();
		//Ȩ
		bool Set(const SKREAL * pWeight,unsigned int uiRow,unsigned int uiColumn);
		//
		bool Copy(const SKNURBSSurface3 & NURBSSurface3);
		/***************************************FORCEINLINE************************************/
		//ıȨ
		FORCEINLINE bool ChangeWeight(unsigned int U,unsigned int V,SKREAL Weight);
		//Ȩָ
		FORCEINLINE const SKREAL * GetWeight()const;
		//õȨ
		FORCEINLINE SKREAL GetWeight(unsigned int U,unsigned int V)const;
		//ȡõ
		virtual SKVector3 GetPoint(SKREAL U,SKREAL V);
		//õƫ
		virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V);
		virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V);

	protected:
		bool Get(unsigned int Order,SKREAL U,SKREAL V ,SKVector3 *D0,SKVector3 *DU,SKVector3 *DV);
	private:
		SKREAL *m_pWeight;
	};
#include "SKNURBSSurface3.inl"
}
#endif