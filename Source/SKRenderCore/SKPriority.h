#ifndef SKPRIORITY_H
#define SKPRIORITY_H
#include "SKArray.h"
#include "SKGraphic.h"
#include "SKPriority.marc"
namespace SKEngine2
{
	//template<class T>
	class SKGRAPHIC_API SKPriority
	{
	public:
		SKPriority();
		SKPriority(unsigned int uiPriorityNum);
		~SKPriority();
		bool AddPriorityThan(SKPriority * pPriority);
		unsigned int GetPriorityNum();

		SKPriority & operator= (SKPriority& rPriority);
		bool operator== (SKPriority& rPriority);
		bool operator!= (SKPriority& rPriority);

	
		bool operator >(SKPriority& rPriority);
		bool operator <(SKPriority& rPriority);
		
		bool operator >=(SKPriority& rPriority);
		bool operator <=(SKPriority& rPriority);

	private:
		bool CheckIsCircle(const SKPriority *pChecked)const;
		unsigned int CheckPriorityNum();
		//0ȱʾȼҲûȼm_pPriorityThanList޳Աʾ޼
		//Ҫȼ
		unsigned int m_uiPriorityNum;
		SKArray<SKPriority *> m_pPriorityThanList;
	
	
	};


}
#endif