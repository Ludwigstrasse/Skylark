#ifndef SKPOINTSET_H
#define SKPOINTSET_H
#include "SKMeshDate.h"
#include "SKVector3.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKPointSet : public SKMeshDate
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKPointSet();
		SKPointSet(const SKVector3 & Point,SKREAL fSize);
		virtual ~SKPointSet();
		bool CreateIndex();
		virtual unsigned int GetTotleNum()const;
		virtual unsigned int GetMeshDateType(){return MDT_POINT;}
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const;


	
	
	};
	DECLARE_Ptr(SKPointSet);
	SKTYPE_MARCO(SKPointSet);

}
#endif