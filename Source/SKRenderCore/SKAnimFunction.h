#ifndef SKANIMFUNCTION_H
#define SKANIMFUNCTION_H

#include "SKController.h"
#include "SKAnimTree.h"
#include "SKAnimBaseFunction.h"
#include "SKAnimAtom.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKAnimFunction : public SKAnimBaseFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~SKAnimFunction() = 0;
		//жǷSkelectonMeshNode,skelecton
		SKAnimFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree);
		virtual bool Update(double dAppTime);
	protected:
		SKAnimFunction();
	public:
		SKArray<SKAnimAtom> m_BoneOutPut;
		SKAnimAtom m_RootAtom;
	public:
		virtual bool SetObject(SKObject * pObject);
		virtual SKAnimAtom & GetRootTranfform();
	};
	DECLARE_Ptr(SKAnimFunction);
	SKTYPE_MARCO(SKAnimFunction);

}
#endif