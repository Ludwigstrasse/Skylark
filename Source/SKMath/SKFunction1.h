#ifndef SKFUNCTION1_H
#define SKFUNCTION2_H
#include "SKMath.h"
/*
	һԪ

*/
namespace SKEngine2
{
	class SKMATH_API SKFunction1 : public SKMemObject
	{
	public:
		SKFunction1();
		virtual ~SKFunction1() = 0;
		//ֵ
		virtual SKREAL GetValue(SKREAL fParameter) = 0 ;
		//
		virtual SKREAL GetFirstDerivative(SKREAL fParameter) = 0;
		virtual SKREAL GetSecondDerivative(SKREAL fParameter) = 0;
		virtual SKREAL GetThirdDerivative(SKREAL fParameter) = 0;
		//
		SKREAL Integration(SKREAL t1 , SKREAL t2,unsigned int uiIterator);
		//ֵ
		void GetExtremumValue(SKREAL t1 , SKREAL t2,SKREAL & Min,SKREAL &Max,unsigned int uiLevel);
		//Ƿи
		bool ISHaveRoot(SKREAL t1 , SKREAL t2,unsigned int uiIterator);
		//
		bool ComputeRoot(SKREAL t1 , SKREAL t2,unsigned int uiLevel);
		//ظָ
		FORCEINLINE SKREAL* GetRoot()const;
		//ظ
		FORCEINLINE unsigned int GetRootNum()const;
	protected:
		typedef struct INTERVAL
		{
			SKREAL t1;
			SKREAL t2;
			SKREAL value1;
			SKREAL value2;
			INTERVAL * next;
		};

		INTERVAL  *m_pHead;
		INTERVAL  *m_pNext;
		SKREAL	*m_pRoot;
		unsigned int m_uiRootNum;
		unsigned int m_uiIndex;
		void Root(SKREAL t1 ,SKREAL t2,SKREAL Value1,SKREAL Value2,unsigned int uiLevel);
		void ExtremumValue(SKREAL t1 , SKREAL t2,SKREAL Value1,SKREAL Value2,SKREAL & Min,SKREAL &Max,unsigned int uiLevel);
	};


}
#endif