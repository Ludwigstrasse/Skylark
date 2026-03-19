#ifndef SKTRIANGLESET_H
#define SKTRIANGLESET_H
#include "SKMeshDate.h"

namespace SKEngine2
{

	

	class SKGRAPHIC_API SKTriangleSet : public SKMeshDate
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKTriangleSet();
		virtual ~SKTriangleSet();
		bool CreateFanIndex();
		bool CreateNormal(unsigned int uiVertexLevel = 0);
		bool CreateTangent(unsigned int uiTexCoordLevel = 0);
		virtual unsigned int GetTotleNum()const;
		virtual unsigned int GetMeshDateType(){return MDT_TRIANGLE;}
		virtual bool CreateCollapseRecord();
		virtual void UpDateClodMesh();
		virtual unsigned int GetActiveNum()const;
		virtual unsigned int GetGirdNum(unsigned int uiInputNum)const;
		
	
	};
	DECLARE_Ptr(SKTriangleSet);
	SKTYPE_MARCO(SKTriangleSet);
	
}
#endif