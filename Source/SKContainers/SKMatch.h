#ifndef SKMATCH_H
#define SKMATCH_H
#include "SKFile.h"
#include "SKContainer.h"
#include "SKArray.h"
#include "SKMath.h"
namespace SKEngine2
{
	/*
	ģʽƥƥ['name'][s>0][s=5][s<10][f][i]Կոַֿ 
	㷨ʵ:ڡ3DϷ̴ʦɡʵ㷨ûиʵ߼ӣ
	Զʵ(ʵкƥйصԶͲʽɽ)
	㷨ˣԶԼԺɵдģ
	ģʽƥ̶ģʽƥ䲻ͬģʽǹ̶ģҪģʽΪԶ
	,ԷΪ4̣['name']̶ֹƥ,[s>0]ֲƥ,[f]ƥ
	[i]ƥ,ģʽƥ(Ҳģʽƥ),жƥ䴮ǷҪ
	*/
	class SKDATESTRUCT_API SKMatch
	{
	private:
		void Clear();
		//ģʽƥԶ
		//ʼ״̬
		bool Begin();
		// ״̬1 
		bool Fun1();
		//״̬ 2 
		bool Fun2();
		// ̶ַƥ״̬
		bool FunStrMatch();
		// ַƥƥ 
		bool FunStr_I_Match();
		// ģʽַжƥ
		int FunIntMatch_D();

		// ƥ䴮ƥ
		bool FunIntMatch();
		// ƥ䴮㴮ƥ
		bool FunFloMatch();
		SKFile File;
	public:
		enum{BUFFER_SIZE=100,MAX_ARGS=16,MAX_COMMENT=80};
		SKMatch();
		~SKMatch();
		bool Open(TCHAR *filename);
		bool Getline();
		//ƥ亯
		bool PatternMatch(TCHAR *_pattern);
		TCHAR buffer[BUFFER_SIZE];			//л
		TCHAR *pattern;
		int  b_index;
		int	 p_index;
		int  length;						//ǰг
		int  num_lines;						//
		//ģʽֵ
		char  pstrings[MAX_ARGS][BUFFER_SIZE]; // any strings
		int   num_pstrings;
		SKArray<SKREAL> pfloats;               // any floats
		SKArray<int>pints;						// any ints

	}; // end CLASS CPARS
}

#endif