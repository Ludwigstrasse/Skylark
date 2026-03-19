#ifndef SKCUSTOMARCHIVEOBJECT_H
#define SKCUSTOMARCHIVEOBJECT_H
#include "SKRtti.h"
#include "SKType.h"
#include "SKMap.h"
namespace SKEngine2
{
	class SKStream;
	class SKEditorElement;
	//ָ֧л֧postload  postclone
	//ԱVSType ֵ֧
	class SKGRAPHIC_API SKCustomArchiveObject:public SKMemObject
	{
	public:
		SKCustomArchiveObject();
		virtual ~SKCustomArchiveObject();
		virtual void Archive(SKStream & Stream) = 0;
		virtual void CopyFrom(SKCustomArchiveObject *,SKMap<SKObject *,SKObject*>& CloneMap) = 0;
		virtual SKEditorElement *  CreateEElement(SKString& Name,SKObject * pOwner);
	};
	CUSTOMTYPE_MARCO(SKCustomArchiveObject)
}
#endif