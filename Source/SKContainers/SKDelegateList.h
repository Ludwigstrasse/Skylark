#ifndef SKDELEGATELIST_H
#define SKDELEGATELIST_H
#include "SKSystem.h"
#include "SKArray.h"
namespace SKEngine2
{
	#define DELEGATE_PREFERRED_SYNTAX
	//ע	ݲͬӦÿҪһ࣬ͬ
	//		ͬʱķʹͳģͬ޷ͬ
	//		꣬Ͳͬ벻ʹͳһ
	// һint ͷintĺ˵꣬
	//		SKDelegate<int(int)> k; ַڱԡ
	//		
	//		SKDelegate1<int,int> ; ֱʾ
	#ifdef DELEGATE_PREFERRED_SYNTAX
		template <typename TSignature> class SKDelegate;
		template <typename TSignature> class SKDelegateEvent;
	#endif

	#define DELEGATE_PARAM_COUNT	0
	#define DELEGATE_TEMPLATE_TYPE
	#define DELEGATE_TYPE
	#define DELEGATE_TYPE_VALUE		
	#define DELEGATE_VALUE
	#include "SKDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE

	#define DELEGATE_PARAM_COUNT	1	
	#define DELEGATE_TEMPLATE_TYPE	typename A1
	#define DELEGATE_TYPE			A1
	#define DELEGATE_TYPE_VALUE		A1 a1
	#define DELEGATE_VALUE			a1
	#include "SKDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE

	#define DELEGATE_PARAM_COUNT	2	
	#define DELEGATE_TEMPLATE_TYPE	typename A1,typename A2
	#define DELEGATE_TYPE			A1,A2
	#define DELEGATE_TYPE_VALUE		A1 a1,A2 a2
	#define DELEGATE_VALUE			a1,a2
	#include "SKDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE


	#define DELEGATE_PARAM_COUNT	3	
	#define DELEGATE_TEMPLATE_TYPE	typename A1,typename A2,typename A3
	#define DELEGATE_TYPE			A1,A2,A3
	#define DELEGATE_TYPE_VALUE		A1 a1,A2 a2,A3 a3
	#define DELEGATE_VALUE			a1,a2,a3
	#include "SKDelegateTemplate.h"
	#undef DELEGATE_PARAM_COUNT
	#undef DELEGATE_TEMPLATE_TYPE
	#undef DELEGATE_TYPE
	#undef DELEGATE_TYPE_VALUE		
	#undef DELEGATE_VALUE
}



#endif