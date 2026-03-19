#ifndef SKRECTANIMBLEND_H
#define SKRECTANIMBLEND_H
#include "SKTwoParamAnimBlend.h"
namespace SKEngine2
{
	/*
						
			LeftTop	 __________  RightTop
					|			|
					|			| 
					|			|
		LeftBottom	|_________	| RightBottom
					  


	   CrossFading IMMEDIATE 
					

				
					0 0,0____________1 0,1
						|				|
						|				|
						|				|
						|				|
						|				|
					2 1,0------------3 1,1

	 

	*/
	class SKStream;
	class SKGRAPHIC_API SKRectAnimBlend : public SKTwoParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

		enum	//Child Location
		{
			CL_LEFT_TOP,
			CL_RIGTH_TOP,
			CL_LEFT_BOTTOM,
			CL_RIGHT_BOTTOM,
			CL_MAX
		};

	public:
		virtual ~SKRectAnimBlend();

		SKRectAnimBlend(const SKUsedName & ShowName,SKAnimTree * pAnimTree);

	protected:

		SKArray<SKAnimAtom> m_BlendBoneMatrix[2];
		SKAnimAtom m_BlendRootMatrix[2];
		SKRectAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
	public:
		virtual bool SetObject(SKObject * pObject);

	};
	DECLARE_Ptr(SKRectAnimBlend);
	SKTYPE_MARCO(SKRectAnimBlend);
}
#endif