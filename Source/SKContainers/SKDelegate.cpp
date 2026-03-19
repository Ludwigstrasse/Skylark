#include "SKDelegateList.h"
using namespace SKEngine2;
// DECLARE_DELEGATE(SKDelegate)
// DECLARE_DELEGATE_ONE(SKDelegate1)
// DECLARE_DELEGATE_TWO(SKDelegate2)
// DECLARE_DELEGATE_THREE(SKDelegate3)
// SKDelegate<int> a;
// SKDelegate1<int,int> f;
// SKDelegate2<int,int,int> c;
// int ss(int k)
// {
// 	return 1;
// }
// int sss(int k,int m)
// {
// 	return 1;
// }
// void m()
// {
// 	f = SKDelegate1<int,int>::FromFun<&ss>();
// 	c = SKDelegate2<int,int,int>::FromFun<&sss>();
// }

// using namespace SKEngine2;
// SKDelegate0<int> a;
// SKDelegate1<int,int> b;
// SKDelegate2<int,int,int> c;
// SKDelegate3<int,int,int,int> d;
using namespace SKEngine2;

#ifdef DELEGATE_PREFERRED_SYNTAX
typedef SKDelegateEvent<int(int, int)> MouseMoveEvent;
#else
typedef SKDelegateEvent2<int, int, int> MouseMoveEvent;
#endif
int sss(int k, int m)
{
	return 1;
}
class A
{
public:
	int LL(int a, int b)const
	{
		return 1;
	}
};
void m()
{
	A a;
	MouseMoveEvent MouseMove;
	MouseMove += MouseMoveEvent::Handler::FromFun<&sss>();
	MouseMove += MouseMoveEvent::Handler::FromMethod<A,&A::LL>(&a);

	MouseMove(1, 2);
}