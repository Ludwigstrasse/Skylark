#ifndef SKLINESET_H
#define SKLINESET_H
#include "SKMeshDate.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKLineSet:public SKMeshDate
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKLineSet();
		virtual ~SKLineSet();
		enum	//Line Type
		{
			LT_OPEN,
			LT_CLOSE,
			LT_SEGMENT,
			LT_MAX
		
		};
		bool CreateIndex(unsigned int uiLineType);
		virtual unsigned int GetTotleNum()const;
		virtual unsigned int GetMeshDateType(){return MDT_LINE;}

		virtual bool CreateCollapseRecord();

		virtual void UpDateClodMesh();
		virtual unsigned int GetActiveNum()const;

		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const;

		
	};
	DECLARE_Ptr(SKLineSet);
	SKTYPE_MARCO(SKLineSet);



}
#endif