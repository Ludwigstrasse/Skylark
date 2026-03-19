#ifndef SKSTREAM_H
#define SKSTREAM_H
#include "SKRtti.h"
#include "SKPointer.h"
#include "SKArray.h"
#include "SKMap.h"
#include "SKString.h"
#include "SKFile.h"
#include "SKType.h"
#include "SKResource.h"
#include "SKCustomArchiveObject.h"
#include "SKObject.h"
namespace SKEngine2
{
DECLARE_Ptr(SKObject);


// ĬcloneָSKType property ǻ´һʵȻٿݣΪò´propertyʱpropertyҪVSProperty::F_CLONE־
//Ȼpostclone Լ ֻĬϵֵǴʵF_COPY
template<typename T>
void Copy(T & Dest,T & Src,SKMap<SKObject *,SKObject*>& CloneMap)
{
	if (TIsVSResourceType<T>::Value)
	{
		Dest = Src;
	}
	else if(TIsVSPointerType<T>::Value)
	{
		SKObject* & TempSrc = *(SKObject**)(void *)&Src;	
		SKObject* & TempDest = *(SKObject**)(void *)&Dest;
		TempDest = SKObject::_CloneCreateObject(TempSrc,CloneMap);
	}
	else if (TIsVSType<T>::Value)
	{
		SKObject* TempSrc = (SKObject *)&Src;	
		SKObject* TempDest = (SKObject *)&Dest;
		SKObject::_CloneObject(TempSrc,TempDest,CloneMap);
	}
	else if(TIsVSSmartPointerType<T>::Value)
	{
		SKObjectPtr & TempSrc = *(SKObjectPtr*)(void *)&Src;
		SKObjectPtr & TempDest = *(SKObjectPtr*)(void *)&Dest;
		TempDest = SKObject::_CloneCreateObject(TempSrc,CloneMap);
	}
	else if (TIsCustomType<T>::Value)
	{
		SKCustomArchiveObject * TempSrc = (SKCustomArchiveObject*)(void *)&Src;
		SKCustomArchiveObject * TempDest = (SKCustomArchiveObject*)(void *)&Dest;
		TempDest->CopyFrom(TempSrc,CloneMap);
	}
	else 
	{
		Dest = Src;
	}
}
template<typename T,SKMemManagerFun MMFun>
void Copy(SKArray<T,MMFun> & Dest,SKArray<T,MMFun> & Src,SKMap<SKObject *,SKObject*>& CloneMap)
{
	Dest.Clear();
	Dest.SetBufferNum(Src.GetNum());
	for (unsigned int i = 0 ; i < Src.GetNum() ;i++)
	{
		Copy(Dest[i],Src[i],CloneMap);
	}
}

template<class Key,class Value,SKMemManagerFun MMFun>
void Copy(SKMap<Key,Value,MMFun> & Dest,SKMap<Key,Value,MMFun> & Src,SKMap<SKObject *,SKObject*>& CloneMap)
{
	Dest.Clear();
	Dest.SetBufferNum(Src.GetNum());
	for (unsigned int i = 0 ; i < Src.GetNum() ;i++)
	{
		MapElement<Key,Value> &ElementSrc = Src[i];
		MapElement<Key,Value> &ElementDest = Dest[i];
		Copy(ElementDest.Key, ElementSrc.Key, CloneMap);
		Copy(ElementDest.Value, ElementSrc.Value, CloneMap);
	}
}
class SKGRAPHIC_API SKStream
{
public:
	enum // Archive Type
	{
		AT_SAVE,
		AT_LOAD,
		AT_LINK,
		AT_REGISTER,
		AT_SIZE,
		AT_POSTLOAD,
		AT_OBJECT_COLLECT_GC,
		AT_CLEAR_OBJECT_PROPERTY_GC,
		AT_LOAD_OBJECT_COLLECT_GC,
	};
	template<class T>
	void Archive(T & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			
			if (TIsVSResourceProxyPointType<T>::Value)
			{
				SKResourceProxyBasePtr & Temp = *(SKResourceProxyBasePtr*)(void *)&Io;
				ReadResource(Temp);
			}
			else if(TIsVSPointerType<T>::Value)
			{
				SKObject* & Temp = *(SKObject**)(void *)&Io;			 
				ReadObjectPtr(Temp);
			}
			else if (TIsVSType<T>::Value)
			{
				SKObject* Key = (SKObject *)&Io;	
				SKObject * Value = NULL;
				ReadObjectPtr(Value);
				m_pmVSTypeLoadMap.AddElement(Key,Value);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				SKObjectPtr & Temp = *(SKObjectPtr*)(void *)&Io;
				ReadObjectPtr(Temp);
			}
			else if (TIsVSStringType<T>::Value)
			{
				SKString & Temp = *(SKString*)(void *)&Io;
				ReadString(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				SKCustomArchiveObject * Temp = (SKCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else 
			{
				Read((void*)&Io,sizeof(T));	
			}
		}
		else if (m_uiStreamFlag == AT_SAVE)
		{
			if (TIsVSResourceProxyPointType<T>::Value)
			{
				SKResourceProxyBasePtr & Temp = *(SKResourceProxyBasePtr*)(void *)&Io;
				WriteResource(Temp);
				
			}
			else if(TIsVSPointerType<T>::Value)
			{
				SKObject* & Temp = *(SKObject**)(void *)&Io;	
				WriteObjectPtr(Temp);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				SKObjectPtr & Temp = *(SKObjectPtr*)(void *)&Io;
				WriteObjectPtr(Temp);
			}
			else if (TIsVSStringType<T>::Value)
			{
				SKString & Temp = *(SKString*)(void *)&Io;
				WriteString(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				SKCustomArchiveObject * Temp = (SKCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else if (TIsVSType<T>::Value)
			{
				SKObject*  Temp = (SKObject *)&Io;	
				WriteObjectPtr(Temp);
			}
			else 
			{
				Write((void*)&Io,sizeof(T));	
			}
		}	
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_POSTLOAD)
		{
			if(TIsVSPointerType<T>::Value)
			{
				SKObject* & Temp = *(SKObject**)(void *)&Io;	
				ArchiveAll(Temp);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				SKObjectPtr & Temp = *(SKObjectPtr*)(void *)&Io;
				ArchiveAll(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				SKCustomArchiveObject * Temp = (SKCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}			
			}
			else if (TIsVSType<T>::Value)
			{
				SKObject*  Temp = (SKObject *)&Io;	
				ArchiveAll(Temp);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			if (TIsVSResourceProxyPointType<T>::Value)
			{
				SKResourceProxyBasePtr & Temp = *(SKResourceProxyBasePtr*)(void *)&Io;
				m_uiArchivePropertySize += GetResourceDistUse(Temp) + sizeof(bool);
			}
			else if(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsVSType<T>::Value)
			{
				m_uiArchivePropertySize += 4;
			}
			else if (TIsVSStringType<T>::Value)
			{
				SKString & Temp = *(SKString*)(void *)&Io;
				m_uiArchivePropertySize += GetStrDistUse(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				SKCustomArchiveObject * Temp = (SKCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}			
			}

			else
			{
				m_uiArchivePropertySize += sizeof(T);
			}
		}
		else if (m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC)
		{
			if (TIsVSPointerType<T>::Value)
			{
				SKObject* & Temp = *(SKObject**)(void *)&Io;
				
				if (Temp)
				{
					if (Temp->IsHasFlag(SKObject::OF_PendingKill))
					{
						Temp = NULL;
						return;
					}
					ArchiveAll(Temp);
				}
				
			}
			else if (TIsVSSmartPointerType<T>::Value)
			{
				SKObjectPtr & Temp = *(SKObjectPtr*)(void *)&Io;
				if (Temp)
				{
					if (Temp->IsHasFlag(SKObject::OF_PendingKill))
					{
						Temp = NULL;
						return;
					}
					ArchiveAll(Temp);
				}
				
				
			}
			else if (TIsCustomType<T>::Value)
			{
				SKCustomArchiveObject * Temp = (SKCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else if (TIsVSType<T>::Value)
			{
				SKObject*  Temp = (SKObject *)&Io;
				ArchiveAll(Temp);
			}
		}
		else if (m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			if (TIsVSPointerType<T>::Value)
			{
				SKObject* & Temp = *(SKObject**)(void *)&Io;
				Temp = NULL;
			}
			else if (TIsVSSmartPointerType<T>::Value)
			{
				SKObjectPtr & Temp = *(SKObjectPtr*)(void *)&Io;
				SKObject * LocalTemp = Temp;
				Temp = NULL;
				if (LocalTemp)
				{
					if (LocalTemp->IsHasFlag(SKObject::OF_REACH))
					{
						LocalTemp->ClearFlag(SKObject::OF_PendingKill);
					}
				}
				

			}
			else if (TIsCustomType<T>::Value)
			{
				SKCustomArchiveObject * Temp = (SKCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else if (TIsVSType<T>::Value)
			{
				SKObject*  Temp = (SKObject *)&Io;
				ArchiveAll(Temp);
			}
		}
		else if (m_uiStreamFlag == AT_LINK)
		{
			if(TIsVSPointerType<T>::Value)
			{
				SKObject* & Temp = *(SKObject**)(void *)&Io;
				LinkObjectPtr(Temp);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				SKObjectPtr & Temp = *(SKObjectPtr*)(void *)&Io;
				LinkObjectPtr(Temp);
			}
			else if (TIsVSType<T>::Value)
			{
				SKObject*  Key = (SKObject *)&Io;	
				SKObject * Value = NULL;
				Value = GetVSTypeMapValue(Key);
				LinkObjectPtr(Value);
				SKMAC_ASSERT(Value);
				SKObject::CloneObject(Value,Key);
// 				unsigned int uiIndex = m_pVObjectArray.FindElement(Value);
// 				SKMAC_ASSERT(uiIndex < m_CopyUsed.GetNum());
// 				m_CopyUsed[uiIndex] = true;
			}
			else if (TIsCustomType<T>::Value)
			{
				SKCustomArchiveObject * Temp = (SKCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
		}
	}
	template<class T,SKMemManagerFun MMFun>
	void Archive(SKArray<T,MMFun> & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			unsigned int uiNum = 0;
			Archive(uiNum);
			Io.SetBufferNum(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if(m_uiStreamFlag == AT_SAVE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK 
			|| m_uiStreamFlag == AT_POSTLOAD || m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
			|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			unsigned int uiNum = Io.GetNum();
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}

	}

	template<class T,SKMemManagerFun MMFun>
	void Archive(SKArrayOrder<T,MMFun> & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			unsigned int uiNum = 0;
			Archive(uiNum);
			Io.SetBufferNum(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if(m_uiStreamFlag == AT_SAVE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK 
			|| m_uiStreamFlag == AT_POSTLOAD || m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
			|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			unsigned int uiNum = Io.GetNum();
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}

	}

	template<class Key,class Value,SKMemManagerFun MMFun>
	void Archive(SKMap<Key,Value,MMFun> & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			unsigned int uiNum = 0;
			Archive(uiNum);
			Io.SetBufferNum(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
		else if(m_uiStreamFlag == AT_SAVE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD
			|| m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
			|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			unsigned int uiNum = Io.GetNum();
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
	}
	
	template<class Key,class Value,SKMemManagerFun MMFun>
	void Archive(SKMapOrder<Key,Value,MMFun> & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			unsigned int uiNum = 0;
			Archive(uiNum);
			Io.SetBufferNum(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
		else if(m_uiStreamFlag == AT_SAVE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD
			|| m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
			|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			unsigned int uiNum = Io.GetNum();
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			unsigned int uiNum = Io.GetNum();
			Archive(uiNum);
			for (unsigned int i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				Archive(Element.Key);
				Archive(Element.Value);
			}
		}
	}
	bool WriteResource(SKResourceProxyBasePtr& Resource);

	bool ReadResource(SKResourceProxyBasePtr& Resource);

	unsigned int GetStreamFlag()const
	{
		return m_uiStreamFlag;
	}
	void SetStreamFlag(unsigned int uiStreamFlag)
	{
		m_uiStreamFlag = uiStreamFlag;
	}
	void AddBufferSize(unsigned int uiSize)
	{
		m_uiArchivePropertySize += uiSize;
	}

	struct ObjectPropertyTable
	{
		ObjectPropertyTable()
		{
			m_uiOffSet = 0;
			m_uiSize = 0;
			m_uiNameID = 0;
		}
		SKString m_PropertyName;
		unsigned int m_uiOffSet;
		unsigned int m_uiSize;
		unsigned int m_uiNameID;
	};

	struct ObjectTabel
	{
		ObjectTabel()
		{
			m_pAddr = NULL;
			m_uiOffSet = 0;
			m_uiObjectPropertySize = 0;
			m_uiObjectPropertyTableSize = 0;
			m_uiObjectPropertyNum = 0;
		}
		SKObject * m_pAddr;
		SKString m_RttiName;
		unsigned int m_uiOffSet;
		unsigned int m_uiObjectPropertySize;
		unsigned int m_uiObjectPropertyTableSize;
		unsigned int m_uiObjectPropertyNum;
		SKArray<ObjectPropertyTable> m_ObjectPropertyTable;
	};
	bool m_bLoadUseGC;
protected:
	unsigned int m_uiStreamFlag;
	unsigned int m_uiArchivePropertySize;
public:
	virtual bool NewLoad(const TCHAR * const pcFileName);
	virtual bool NewSave(const TCHAR * const pcFileName);

	virtual bool NewLoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize);

	bool ArchiveAll(SKObject * pObject);
public:
	bool RegisterObject(SKObject * pObject);

	bool RegisterPostLoadObject(SKObject * pObject);

	bool RegisterReachableObject(SKObject * pObject);

	SKStream();
	~SKStream();
	bool Read(void * pvBuffer,unsigned int uiSize);
	bool Write(const void * pvBuffer,unsigned int uiSize);
	bool ReadString(SKString &str);
	bool WriteString(const SKString &str);
	static int GetResourceDistUse(SKResourceProxyBasePtr& Resource);
	static int GetStrDistUse(const SKString &Str);
	static int GetStrDistUse(const TCHAR * pCh);
	const SKObject *GetMapValue(SKObject* pKey)const; 
	SKObject *GetVSTypeMapValue(SKObject *pKey)const;
	const SKObject *GetObjectByRtti(const SKRtti &Rtti);
	bool GetObjectArrayByRtti(const SKRtti &Rtti,SKArray<SKObject *> &ObjectArray,bool IsDerivedFrom = false);
	bool GetAllResourceObject(SKArray<SKObject *> &ObjectArray);
	FORCEINLINE unsigned int GetVerson() const
	{
		return m_uiVerson;
	}

	static unsigned int GetCurVerson()
	{
		return ms_uiCurVerson;
	}
	enum
	{
		UNICODE_FLAG = BIT(0)
	};
	FORCEINLINE bool HasAnyFlag(DWORD dwFlagsToCheck)const
	{
		return (m_dwFlag & dwFlagsToCheck) != 0;
	}
	template<class T>
	bool ReadObjectPtr(SKPointer<T> & Pointer);

	template<class T>
	bool WriteObjectPtr(const SKPointer<T> & Pointer) ;

	template<class T>
	bool LinkObjectPtr(SKPointer<T> & Pointer);

	template<class T>
	bool ReadObjectPtr(T * &pObject);

	template<class T>
	bool WriteObjectPtr(T * const&pObject);

	template<class T>
	bool LinkObjectPtr(T * &pObject);
private:
	unsigned char * m_pcCurBufPtr;
	unsigned char * m_pcBuffer;
	unsigned int m_uiBufferSize;
	SKArray<SKObject *> m_pVObjectArray;
	SKArray<SKObject *> m_pPostLoadObject;

	SKMap<SKObject*,SKObject*> m_pmLoadMap;
	SKMap<SKObject*,SKObject*> m_pmVSTypeLoadMap;
	
	static unsigned int ms_uiCurVerson;
	unsigned int m_uiVerson;
	DWORD	m_dwFlag;
};

template<class T>
bool SKStream::ReadObjectPtr(SKPointer<T> & Pointer)
{
	T * pP = NULL;
	if(!Read( &pP, sizeof(T *) ))
		return 0;
	Pointer.SetObject(pP);
	return 1;
}
/********************************************************************************/
template<class T>
bool SKStream::WriteObjectPtr(const SKPointer<T> & Pointer)
{
	T* pP = Pointer;
	if(!Write( &pP, sizeof(T *) ))
		return 0;
	return 1;
}
/********************************************************************************/
template<class T>
bool SKStream::LinkObjectPtr(SKPointer<T> & Pointer)
{
	SKObject * pP = (SKObject *)Pointer.GetObject();

	Pointer.SetObject(NULL);

	Pointer = (T *)GetMapValue(pP);

	if (!Pointer)
	{
		return 0;
	}
	return 1;
}
/********************************************************************************/
template<class T>
bool SKStream::ReadObjectPtr(T * &pObject)
{
	if(!Read( &pObject, sizeof(T *) ))
		return 0;
	return 1;
}
/********************************************************************************/
template<class T>
bool SKStream::WriteObjectPtr(T * const &pObject)
{
	if(!Write( &pObject, sizeof(T *) ))
		return 0;
	return 1;
}
/********************************************************************************/
template<class T>
bool SKStream::LinkObjectPtr(T * &pObject)
{
	pObject = (T *)GetMapValue(pObject);

	if (!pObject)
	{
		return 0;
	}
	return 1;
}	
}
#endif