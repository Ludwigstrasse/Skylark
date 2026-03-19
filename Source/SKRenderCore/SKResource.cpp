#include "SKResource.h"
#include "SKStream.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
SKString SKResource::ms_FileSuffix[] = 
{
	_T("TEXTURE"),
	_T("SKMODEL"),
	_T("STMODEL"),
	_T("ACTION"),
	_T("MATERIAL"),
	_T("POSTEFFECT"),
	_T("SHADER"),
	_T("ANIMTREE"),
	_T("MORPHTREE"),
	_T("TERRAIN"),
	_T("FSM"),
	_T("ACTOR"),
	_T("MAP"),
	_T("FONT"),
	_T("")


}; 
const SKString &SKResource::GetFileSuffix(unsigned int uiFileSuffix)
{
	if(uiFileSuffix > RT_MAX)
		return ms_FileSuffix[RT_MAX];	
	return ms_FileSuffix[uiFileSuffix];
}
SKResource::SKResource()
{

}
SKResource::~SKResource()
{

}

SKResourceProxyBase::SKResourceProxyBase()
{

}
SKResourceProxyBase::~SKResourceProxyBase()
{

}
