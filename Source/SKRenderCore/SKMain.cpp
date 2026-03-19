#include "SKMain.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
SKArray<SKMain::Element> *SKMain::ms_pInitialArray = NULL;
SKArray<SKMain::Element> *SKMain::ms_pTerminalArray = NULL;
SKArray<FunctionProperty> *SKMain::ms_pInitialPropertyArray = NULL;
SKArray<Function> *SKMain::ms_pTerminalPropertyArray = NULL;
unsigned int SKMain::ms_uiInitialObject = 0;
unsigned int SKMain::ms_uiTerminalObject = 0;
#include "SKResourceManager.h"
void SKMain::AddInitialPropertyFuntion(FunctionProperty Func)
{
	if(!Func)
		return;
	if(!ms_pInitialPropertyArray)
	{
		ms_pInitialPropertyArray = SK_NEW SKArray<FunctionProperty>;
	}
	ms_pInitialPropertyArray->AddElement(Func);
}
void SKMain::AddTerminalPropertyFuntion(Function Func)
{
	if(!Func)
		return;
	if(!ms_pTerminalPropertyArray)
	{
		ms_pTerminalPropertyArray = SK_NEW SKArray<Function>;
	}
	ms_pTerminalPropertyArray->AddElement(Func);
}
void SKMain::AddInitialFuntion(Function Func)
{
	AddInitialFuntion(Func,NULL);
}
void SKMain::AddInitialFuntion(Function Func,SKPriority *pPriority)
{
	if(!Func)
		return;
	if(!ms_pInitialArray)
	{
		ms_pInitialArray = SK_NEW SKArray<Element>;
	}
	Element e;
	e.Func = Func;
	e.pPriority = pPriority;
	ms_pInitialArray->AddElement(e);

}
void SKMain::AddTerminalFuntion(Function Func)
{
	AddTerminalFuntion(Func,NULL);
}
void SKMain::AddTerminalFuntion(Function Func,SKPriority *pPriority)
{
	if(!Func)
		return;
	if(!ms_pTerminalArray)
	{
		ms_pTerminalArray = SK_NEW SKArray<Element>;
	}
	Element e;
	e.Func = Func;
	e.pPriority = pPriority;
	ms_pTerminalArray->AddElement(e);

}
bool SKMain::Initialize()
{
	
	for(unsigned int i = 0 ; i < ms_pInitialPropertyArray->GetNum(); i++)
	{
		/*(*( (*ms_pInitialArray)[i].Func ))();*/

		if( !(*( (*ms_pInitialPropertyArray)[i] ))(NULL) )
		{
			SKMAC_ASSERT(0);
			return 0;
		}
	}
	ms_pInitialArray->Sort(0,ms_pInitialArray->GetNum() - 1,PriorityCompare());
	for(unsigned int i = 0 ; i < ms_pInitialArray->GetNum(); i++)
	{
		/*(*( (*ms_pInitialArray)[i].Func ))();*/

		if( !(*( (*ms_pInitialArray)[i].Func ))() )
		{
			SKMAC_ASSERT(0);
			return 0;
		}
	}
	

	ms_pInitialArray->Clear();
	SKMAC_DELETE(ms_pInitialArray);
	ms_uiInitialObject = SKObject::GetObjectManager().GetObjectNum();
	ms_pInitialPropertyArray->Clear();
	SKMAC_DELETE(ms_pInitialPropertyArray);
	return 1;
}
bool SKMain::Terminate()
{
	SKResourceManager::CacheResource();
	ms_pTerminalArray->Sort(0,ms_pTerminalArray->GetNum(),PriorityCompare());
	ms_uiTerminalObject = SKObject::GetObjectManager().GetObjectNum();

	for (int i = ms_pTerminalArray->GetNum() - 1; i >= 0; i--)
	{
		/*Function fun = NULL;
		fun = (*ms_pTerminalArray)[i].Func;
		(*fun)();
		//(*( (*ms_pTerminalArray)[i].Func ))();*/
		if( !(*( (*ms_pTerminalArray)[i].Func ))() )
		{
			SKMAC_ASSERT(0);
			return 0;
		}

	}
	ms_pTerminalArray->Clear();
	SKMAC_DELETE(ms_pTerminalArray);

	for(unsigned int i = 0 ; i < ms_pTerminalPropertyArray->GetNum(); i++)
	{

		if( !(*( (*ms_pTerminalPropertyArray)[i]))() )
		{
			SKMAC_ASSERT(0);
			return 0;
		}
	}
	ms_pTerminalPropertyArray->Clear();
	SKMAC_DELETE(ms_pTerminalPropertyArray);
	SKResourceManager::GCObject();
	SKResourceManager::RunAllGCTask();
	SKMAC_ASSERT(SKResourceManager::IsReleaseAll());
	ms_uiTerminalObject = SKObject::GetObjectManager().GetObjectNum();
	SKMAC_ASSERT(ms_uiTerminalObject == 0);
	return 1;

}