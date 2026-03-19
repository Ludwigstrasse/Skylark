#ifndef SKANIMMAINFUNCTION_H
#define SKANIMMAINFUNCTION_H
#include "SKAnimBaseFunction.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKAnimMainFunction : public SKAnimBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~SKAnimMainFunction();
		virtual bool Update(double dAppTime);
		SKAnimMainFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree);
	protected:
		SKAnimMainFunction();


	};
	DECLARE_Ptr(SKAnimMainFunction);
	SKTYPE_MARCO(SKAnimMainFunction);
}
#endif