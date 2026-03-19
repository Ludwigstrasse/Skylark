#ifndef SKMAIN_H
#define SKMAIN_H
#include "SKSystem.h"
#include "SKPriority.h"
#include "SKArray.h"
namespace SKEngine2
{
	typedef bool (*Function)();
	class SKRtti ;
	typedef bool (*FunctionProperty)(SKRtti *);
	class SKGRAPHIC_API SKMain
	{
	public:
		
		static void AddInitialPropertyFuntion(FunctionProperty Func);
		static void AddTerminalPropertyFuntion(Function Func);
		static void AddInitialFuntion(Function Func);
		static void AddInitialFuntion(Function Func,SKPriority *pPriority);
		static bool Initialize();
		static void AddTerminalFuntion(Function Func);
		static void AddTerminalFuntion(Function Func,SKPriority *pPriority);
		static bool Terminate();
	private:
		SKMain();
		~SKMain();
		struct Element
		{
		public:
			Element()
			{
				Func = NULL;
				pPriority = NULL;
			}
			~Element()
			{
				Func = NULL;
				pPriority = NULL;
			}
			Function		Func;
			SKPriority *	pPriority;
			bool operator > (const Element& e)const
			{
				static SKPriority Pr1;
				static SKPriority Pr2;

				SKPriority *p1 = NULL;
				SKPriority *p2 = NULL;
				if(pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if(e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) > (*p2);
			}
			bool operator < (const Element& e)const
			{
				static SKPriority Pr1;
				static SKPriority Pr2;

				SKPriority *p1 = NULL;
				SKPriority *p2 = NULL;
				if(pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if(e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) > (*p2);
			}
			bool operator == (const Element& e)const
			{
				static SKPriority Pr1;
				static SKPriority Pr2;

				SKPriority *p1 = NULL;
				SKPriority *p2 = NULL;
				if(pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if(e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) == (*p2);
			}
		};
		static SKArray<Element> * ms_pInitialArray;
		static SKArray<FunctionProperty> * ms_pInitialPropertyArray;
		static SKArray<Function> * ms_pTerminalPropertyArray;
		static SKArray<Element> * ms_pTerminalArray;
		static unsigned int ms_uiInitialObject;
		static unsigned int ms_uiTerminalObject;
		
		class PriorityCompare
		{
		public:
			FORCEINLINE bool operator()(Element & e1,Element& e2)
			{
				static SKPriority Pr1;
				static SKPriority Pr2;

				SKPriority *p1 = NULL;
				SKPriority *p2 = NULL;
				if(e1.pPriority)
					p1 = e1.pPriority;
				else
					p1 = &Pr1;
				if(e2.pPriority)
					p2 = e2.pPriority;
				else
					p2 = &Pr2;
				return (*p1) <= (*p2);
			
			}
		
		};
	};
}
#endif