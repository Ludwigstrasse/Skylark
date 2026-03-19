#ifndef SKMORPHMAINFUNCTION_H
#define SKMORPHMAINFUNCTION_H
#include "SKMorphBaseFunction.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKMorphMainFunction : public SKMorphBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~SKMorphMainFunction();
		virtual bool Update(double dAppTime);

		SKMorphMainFunction(const SKUsedName & ShowName,SKMorphTree * pMorphTree);
		//void CreateNode(const SKString & NodeName,unsigned int uiNodeType);
	protected:
		SKMorphMainFunction();
		void MainThreadUpdate(SKMorphFunction *pMorphFunction);
		void RenderThreadUpdate(SKMorphFunction *pMorphFunction);


	};
	DECLARE_Ptr(SKMorphMainFunction);
	SKTYPE_MARCO(SKMorphMainFunction);

}
#endif