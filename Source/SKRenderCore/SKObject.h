#ifndef SKOBJECT_H
#define SKOBJECT_H
#include "SKSystem.h"
#include "SKReference.h"
#include "SKPriority.h"
#include "SKRtti.h"
#include "SKLog.h"
#include "SKMain.h"
#include "SKMap.h"
#include "SKString.h"
#include "SKName.h"
#include "SKEditorProperty.h"
#include "SKInitial.marc"
#include "SKList.h"
namespace  SKEngine2
{
class SKObject;
typedef SKObject *(*FactoryFunction)();
class SKRenderer;
class SKResourceIdentifier;
class SKStream;
class SKResourceProxyBase;
class SKFastObjectManager
{
public:
	enum 
	{
		MAX_OBJECT_FLAG = 100000,
		MAX_OBJECT_NUM = MAX_OBJECT_FLAG - 1
	};
	SKFastObjectManager();
	~SKFastObjectManager();
	unsigned int AddObject(SKObject * p);
	void DeleteObject(SKObject * p);
	bool IsClear();
	void PrepareForGC();
	unsigned int GetObjectNum();
protected:
	SKObject * m_ObjectArray[MAX_OBJECT_FLAG];
	SKArray<unsigned int> m_FreeTable;

};
class SKGRAPHIC_API SKObject:public SKReference , public SKMemObject
{			
public:
	
	friend class SKStream;
	
	virtual ~SKObject() = 0;
	SKObject(const SKObject & object);
	SKObject & operator =(const SKObject &object);
	SKObject();

//RTTI
	DECLARE_RTTI;
public:
	bool IsSameType(const SKObject *pObject)const;
	bool IsDerived(const SKObject *pObject)const;
	bool IsSameType(const SKRtti &Type)const;
	bool IsDerived(const SKRtti &Type)const;

	DECLARE_INITIAL_NO_CLASS_FACTORY;
	

//Stream
public:
	static SKObject * GetInstance(const SKString& sRttiName);
	static SKObject * GetInstance(const SKRtti& Rtti);
	template<typename T>
	static T * GetInstance()
	{
		return (T *)GetInstance(T::ms_Type);
	}
	virtual bool BeforeSave( void * pDate = NULL);
	virtual bool PostSave( void * pDate = NULL);
	virtual bool PostLoad(void * pDate = NULL);
protected:
	static SKMapOrder<SKUsedName,FactoryFunction> ms_ClassFactory;
	static SKObject * GetNoGCInstance(const SKString& sRttiName);
	unsigned int m_uiObjectID;
	
public:
	friend class SKFastObjectManager;
	static SKFastObjectManager & GetObjectManager()
	{
		static SKFastObjectManager ms_ObjectManager;
		return  ms_ObjectManager;
	}
	virtual void LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data = 0);

	
//debug
public:
	bool DebugLevel(SKLog & log)const;
	static SKObject* _CloneCreateObject(SKObject * pObject,SKMap<SKObject *,SKObject*>& CloneMap);
	static void _CloneObject(SKObject * pObjectSrc,SKObject * pObjectDest,SKMap<SKObject *,SKObject*>& CloneMap);

	static SKObject* CloneCreateObject(SKObject * pObject);
	static void CloneObject(SKObject * pObjectSrc,SKObject * pObjectDest);
	

	virtual bool PostClone(SKObject * pObjectSrc);
	virtual void ValueChange(SKString & Name);
	virtual void DelayUpdate();
	bool Process(SKUsedName & FunName, void * para, void *ret = NULL , int ParaNum = -1);

	void CallVoidFun(SKUsedName &FunName)
	{
		Process(FunName, NULL);
	}
	template<class Type1>
	void CallVoidFun(SKUsedName &FunName,Type1& t1)
	{
		struct MyStruct
		{
			Type1 t1;
		}Temp;
		Temp.t1 = t1;
		Process(FunName, (void *)&Temp, NULL, 1);
	}
	template<class Type1, class Type2>
	void CallVoidFun(SKUsedName &FunName, Type1& t1, Type2& t2)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Process(FunName, (void *)&Temp, NULL, 2);
	}

	template<class Type1, class Type2, class Type3>
	void CallVoidFun(SKUsedName &FunName, Type1& t1, Type2& t2, Type3& t3)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Process(FunName, (void *)&Temp, NULL, 3);
	}

	template<class Type1, class Type2, class Type3,class Type4>
	void CallVoidFun(SKUsedName &FunName, Type1& t1, Type2& t2, Type3& t3, Type4& t4)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
			Type4 t4;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Temp.t4 = t4;
		Process(FunName, (void *)&Temp, NULL, 4);
	}
	template<class ReturnType>
	void CallFun(SKUsedName &FunName, ReturnType& ReturnValue)
	{
		Process(FunName, NULL, (void *)&ReturnValue);
	}
	template<class ReturnType,class Type1>
	void CallFun(SKUsedName &FunName, ReturnType& ReturnValue, Type1& t1)
	{
		struct MyStruct
		{
			Type1 t1;
		}Temp;
		Temp.t1 = t1;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 1);
	}
	template<class ReturnType,class Type1, class Type2>
	void CallFun(SKUsedName &FunName, ReturnType& ReturnValue, Type1& t1, Type2& t2)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 2);
	}

	template<class ReturnType,class Type1, class Type2, class Type3>
	void CallFun(SKUsedName &FunName, ReturnType& ReturnValue, Type1& t1, Type2& t2, Type3& t3)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 3);
	}

	template<class ReturnType,class Type1, class Type2, class Type3, class Type4>
	void CallFun(SKUsedName &FunName, ReturnType& ReturnValue,Type1& t1, Type2& t2, Type3& t3, Type4& t4)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
			Type4 t4;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Temp.t4 = t4;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 4);
	}
public:
	enum //Object Flag
	{
		OF_REACH = 0x01,
		OF_UNREACH = 0x02,
		OF_PendingKill = 0x04,
		OF_GCObject = 0x08,
		OF_RootObject = 0x10,
		OF_MAX
	};
	FORCEINLINE void SetFlag(unsigned int uiFlag)
	{

		m_uiFlag |= uiFlag;
	}
	FORCEINLINE void ClearFlag(unsigned int uiFlag)
	{
		m_uiFlag &= ~uiFlag;
	}
	FORCEINLINE bool IsHasFlag(unsigned int uiFlag)
	{
		return (m_uiFlag & uiFlag) != 0;
	}
	unsigned int m_uiFlag;
	FORCEINLINE void DecreRef()
	{
		SKLockedDecrement((long *)&m_iReference);
		if (!m_iReference)
		{
			if (IsHasFlag(OF_GCObject))
			{
				SetFlag(OF_PendingKill);
			}
			else
			{
				SK_DELETE this;
			}
			
		}
	}
	
};
DECLARE_Ptr(SKObject);
SKTYPE_MARCO(SKObject);
template <class T>
T* StaticCast (SKObject* pkObj)
{
	return (T*)pkObj;
}
template <class T>
const T* StaticCast (const SKObject* pkObj)
{
	return (const T*)pkObj;
}
template<class T>
T * DynamicCast(SKObject * pObj)
{
	return pObj && pObj->IsDerived(T::ms_Type)?(T*)pObj:0;
}

template<class T>
const T * DynamicCast(const SKObject * pObj)
{
	return pObj && pObj->IsDerived(T::ms_Type)?(const T*)pObj:0;
}
}
#endif