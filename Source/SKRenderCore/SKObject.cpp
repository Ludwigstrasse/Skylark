#include "SKObject.h"
#include "SKStream.h"
#include "SKProperty.h"
#include "SKGraphicInclude.h"
#include "SKProfiler.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoParent_NoCreateFun(SKObject)
SKMapOrder<SKUsedName,FactoryFunction> SKObject::ms_ClassFactory;
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY_ROOT(SKObject)
REGISTER_PROPERTY(m_uiFlag, Flag,SKProperty::F_CLONE);
END_ADD_PROPERTY
DECLEAR_NOCLEAR_COUNT_PROFILENODE(ObjectNum,)
SKFastObjectManager::SKFastObjectManager()
{
	m_FreeTable.SetBufferNum(MAX_OBJECT_NUM);
	for (unsigned int i = 0; i < MAX_OBJECT_NUM; i++)
	{
		m_FreeTable[i] = i;
		m_ObjectArray[i] = NULL;
	}
	m_ObjectArray[MAX_OBJECT_NUM] = NULL;
}
SKFastObjectManager::~SKFastObjectManager()
{

}
unsigned int SKFastObjectManager::AddObject(SKObject * p)
{
	SKMAC_ASSERT(m_FreeTable.GetNum() > 0);
	unsigned int ID = m_FreeTable[m_FreeTable.GetNum() - 1];
	m_ObjectArray[ID] = p;
	m_FreeTable.Erase(m_FreeTable.GetNum() - 1);
	ADD_COUNT_PROFILE(ObjectNum, 1)
	return ID;
}
void SKFastObjectManager::DeleteObject(SKObject * p)
{
	if (m_ObjectArray[p->m_uiObjectID] != NULL)
	{
		m_FreeTable.AddElement(p->m_uiObjectID);
		m_ObjectArray[p->m_uiObjectID] = NULL;
		ADD_COUNT_PROFILE(ObjectNum, -1)
		p->m_uiObjectID = MAX_OBJECT_FLAG;
	}
	
}
bool SKFastObjectManager::IsClear()
{
	return m_FreeTable.GetNum() == MAX_OBJECT_NUM;
}
unsigned int SKFastObjectManager::GetObjectNum()
{
	return MAX_OBJECT_NUM - m_FreeTable.GetNum();
}
void SKFastObjectManager::PrepareForGC()
{
	for (unsigned int i = 0; i < MAX_OBJECT_NUM; i++)
	{
		SKObject * p = m_ObjectArray[i];
		if (p)
		{
			p->ClearFlag(SKObject::OF_REACH);
			p->SetFlag(SKObject::OF_UNREACH);
		}
	}
}
SKObject* SKObject::CloneCreateObject(SKObject * pObject)
{
	SKMap<SKObject *,SKObject*> CloneMap; 
	SKObject * pNewObject =  _CloneCreateObject(pObject,CloneMap);
	for (unsigned int i = 0 ; i < CloneMap.GetNum() ;i++)
	{
		CloneMap[i].Value->PostClone(CloneMap[i].Key);
	}
	return pNewObject;
}
void SKObject::CloneObject(SKObject * pObjectSrc,SKObject * pObjectDest)
{
	SKMap<SKObject *,SKObject*> CloneMap; 
	_CloneObject(pObjectSrc,pObjectDest,CloneMap);
	for (unsigned int i = 0 ; i < CloneMap.GetNum() ;i++)
	{
		CloneMap[i].Value->PostClone(CloneMap[i].Key);
	}
}
SKObject* SKObject::_CloneCreateObject(SKObject * pObject,SKMap<SKObject *,SKObject*>& CloneMap)
{
	SKObject* pNewObject = NULL;
	if (pObject)
	{
		unsigned int uiIndex = CloneMap.Find(pObject);

		if (uiIndex == CloneMap.GetNum())
		{
			SKRtti & SrcRtti = pObject->GetType();
			if (pObject->IsHasFlag(OF_GCObject))
			{
				pNewObject = SKObject::GetInstance(SrcRtti.GetName());
			}
			else
			{
				pNewObject = SKObject::GetNoGCInstance(SrcRtti.GetName());
			}
			CloneMap.AddElement(pObject,pNewObject);
			SKRtti & DestRtti = pNewObject->GetType();

			for (unsigned int i = 0 ; i < SrcRtti.GetPropertyNum() ; i++)
			{
				SKProperty * pProperty = SrcRtti.GetProperty(i);
				if (pProperty->GetFlag() & SKProperty::F_CLONE)
				{
					pProperty->CloneData(pObject,pNewObject,CloneMap);
				}
				else if(pProperty->GetFlag() & SKProperty::F_COPY)
				{
					pProperty->CopyData(pObject,pNewObject);
				}

			}
		}
		else
		{
			pNewObject = CloneMap[uiIndex].Value;
		}
	}
	return pNewObject;
}
void SKObject::_CloneObject(SKObject * pObjectSrc,SKObject * pObjectDest,SKMap<SKObject *,SKObject*>& CloneMap)
{
	if (!pObjectSrc)
	{
		return ;
	}
	unsigned int uiIndex = CloneMap.Find(pObjectSrc);

	if (uiIndex == CloneMap.GetNum())
	{
		SKRtti & SrcRtti = pObjectSrc->GetType();
		SKRtti & DestRtti = pObjectDest->GetType();
		for (unsigned int i = 0 ; i < SrcRtti.GetPropertyNum() ; i++)
		{
			SKProperty * pProperty = SrcRtti.GetProperty(i);
			if (pProperty->GetFlag() & SKProperty::F_CLONE)
			{
				pProperty->CloneData(pObjectSrc,pObjectDest,CloneMap);
			}
			else if(pProperty->GetFlag() & SKProperty::F_COPY)
			{
				pProperty->CopyData(pObjectSrc,pObjectDest);
			}
		}
	}
	else
	{
		SKMAC_ASSERT(0);
	}
}
/********************************************************************************/
SKObject * SKObject::GetNoGCInstance(const SKString& sRttiName)
{
	unsigned int i = ms_ClassFactory.Find(sRttiName);
	if (i == ms_ClassFactory.GetNum())
		return NULL;

	SKObject * pObject = ms_ClassFactory[i].Value();
	return pObject;

}
SKObject * SKObject::GetInstance(const SKString& sRttiName)
{
	unsigned int i = ms_ClassFactory.Find(sRttiName);
	if(i == ms_ClassFactory.GetNum())
		return NULL;

	SKObject * pObject = ms_ClassFactory[i].Value();
	SKResourceManager::AddGCObject(pObject);
	return pObject;

}
SKObject * SKObject::GetInstance(const SKRtti& Rtti)
{
	SKObject * pObject = (Rtti.m_CreateFun)();
	SKResourceManager::AddGCObject(pObject);
	return pObject;
}
/********************************************************************************/
SKObject::SKObject()
{
	m_uiFlag = 0;
	m_uiObjectID = GetObjectManager().AddObject(this);
	


}//end construct
SKObject::SKObject(const SKObject & object)
{
	m_uiFlag = object.m_uiFlag;
	m_uiObjectID = GetObjectManager().AddObject(this);
}
SKObject & SKObject::operator =(const SKObject &object)
{
	m_uiFlag = object.m_uiFlag;
	return *this;
}
/********************************************************************************/

SKObject::~SKObject()
{
	GetObjectManager().DeleteObject(this);
}//end destruct


bool SKObject::IsSameType(const SKObject *pObject)const
{

	return pObject && GetType().IsSameType(pObject->GetType());

}//end IsSameType

/********************************************************************************/

bool SKObject::IsDerived(const SKObject *pObject)const
{
	return pObject && GetType().IsDerived(pObject->GetType());
}//IsDerived

/********************************************************************************/

bool SKObject::IsSameType(const SKRtti &Type)const
{
	
	return GetType().IsSameType(Type);
}//IsSameType

/********************************************************************************/

bool SKObject::IsDerived(const SKRtti &Type)const
{
	return GetType().IsDerived(Type);
	
}//IsDerived

void SKObject::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{

}

bool SKObject::BeforeSave( void * pDate)
{
	return 1;
}
bool SKObject::PostSave( void * pDate)
{
	return 1;
}
bool SKObject::PostLoad( void * pDate)
{
	return 1;
}
bool SKObject::PostClone(SKObject * pObjectSrc)
{
	return 1;
}
void SKObject::ValueChange(SKString & Name)
{

}
void SKObject::DelayUpdate()
{

}
bool SKObject::Process(SKUsedName & FunName, void * para, void *ret, int ParaNum)
{
	SKRtti & t = GetType();
	for (unsigned int i = 0; i < t.GetFunctionNum(); i++)
	{
		SKFunction * p = t.GetFunction(i);
		if (p->GetName() == FunName)
		{
			if (ParaNum == -1)
			{
				p->ObjectFun(this, p, para, ret);
				return true;
			}
			else if (p->GetPropertyNum() == ParaNum)
			{
				p->ObjectFun(this, p, para, ret);
				return true;
			}
			
		}
	}
	return false;
}