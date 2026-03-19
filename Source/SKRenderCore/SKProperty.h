#ifndef SKPROPERTY_H
#define SKPROPERTY_H
#include "SKRtti.h"
#include "SKSystem.h"
#include "SKArray.h"
#include "SKMap.h"
#include "SKProperty.marc"
#include "SKName.h"
#include "SKType.h"
#include "SKStream.h"
#include "SKEditorProperty.h"
#include "SKEnumTrans.h"
namespace SKEngine2
{
	


	template < class T > FORCEINLINE bool operator < (const T& v1, const T& v2)
	{
		return false;
	}
	template < class T > FORCEINLINE bool operator > (const T& v1, const T& v2)
	{
		return false;
	}
	class SKFunction;
	// Value Point Resource Struct Enum
	class SKProperty
	{
	public:
		enum	//Property Type
		{
			PT_VALUE,
			PT_ENUM,
			PT_DATA,
			PT_ARRAY,
			PT_MAP,
			PT_MAX
		};
		enum	// Flag 
		{
			F_NONE = 0X00,
			F_SAVE_LOAD = 0X01,
			F_CLONE = 0X02,
			F_COPY = 0X04,
			F_SAVE_LOAD_CLONE = 0X03,
			F_SAVE_LOAD_COPY = 0X05,
			F_REFLECT_NAME = 0X08,
			F_NO_USE_GC = 0X10,
			F_MAX
		};
		SKProperty()
		{
			m_pRttiOwner = NULL;
			m_pFuntionOwner = NULL;
			m_uiFlag = 0;
			m_uiElementOffset = 0;
		}
		SKProperty(SKRtti & Owner,const SKUsedName & Name,unsigned int uiElementOffset,unsigned int uiFlag)
			:m_pRttiOwner(&Owner)
		{
			m_Name = Name;
			m_uiElementOffset = uiElementOffset;
			m_uiFlag = uiFlag;
			m_pFuntionOwner = NULL;
		}
		SKProperty(SKFunction & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag)
			:m_pFuntionOwner(&Owner)
		{
			m_Name = Name;
			m_uiElementOffset = uiElementOffset;
			m_uiFlag = uiFlag;
			m_pRttiOwner = NULL;
		}
		virtual ~SKProperty()
		{

		}
		virtual unsigned int GetPropertyType()const = 0;
		FORCEINLINE SKRtti * GetRtti()const
		{
			return m_pRttiOwner;
		}
		FORCEINLINE SKFunction * GetFunction()const
		{
			return m_pFuntionOwner;
		}
		FORCEINLINE const SKUsedName & GetName()const
		{
			return m_Name;
		}
		virtual void* GetValueAddress(void* pObj)const
		{
			return (void*)(((unsigned char*)pObj) + m_uiElementOffset);
		}
		virtual bool Archive(SKStream &Stream,void* pObj) = 0;
		virtual SKProperty * GetInstance() = 0;
		virtual void CloneData(void* pSrcObj,void* pDestObj,SKMap<SKObject *,SKObject*>& CloneMap) = 0;
		virtual void CopyData(void* pSrcObj,void* pDestObj) = 0;
		virtual void AddEditorElement(void * pSrcObj,SKECollection * pParent,SKString &Name) = 0;
		virtual bool Clone(SKProperty * p)
		{
			if ((p->GetRtti() == GetRtti() && GetRtti()) || (p->GetFunction() == GetFunction() && GetFunction()))
			{
				m_Name = p->m_Name;
				m_uiFlag = p->m_uiFlag;
				m_uiElementOffset = p->m_uiElementOffset;
				return true;		
			}
			
			return false;
		}
		void SetOwner(SKRtti & Owner)
		{
			m_pRttiOwner = &Owner;
		}
		void SetOwner(SKFunction & Function)
		{
			m_pFuntionOwner = &Function;
		}
		unsigned int GetFlag()const
		{
			return m_uiFlag;
		}
		void SetFlag(unsigned int uiFlag)
		{
			m_uiFlag = uiFlag;
		}
	protected:
		SKRtti * m_pRttiOwner;
		SKFunction * m_pFuntionOwner;
		SKUsedName m_Name;
		unsigned int m_uiFlag;
		unsigned int m_uiElementOffset;
	};
	template<typename T>
	class SKEnumProperty : public SKProperty
	{
	public:
		SKEnumProperty()
		{
			SKMAC_ASSERT(sizeof(T) == 4);
		}
		SKEnumProperty(SKRtti & Owner,const SKUsedName & Name,const SKUsedName & EnumName,unsigned int uiElementOffset,unsigned int uiFlag)
			:SKProperty(Owner,Name,uiElementOffset,uiFlag)
		{
			SKMAC_ASSERT(sizeof(T) == 4);
			m_EnumName = EnumName;
		}
		SKEnumProperty(SKFunction & Owner, const SKUsedName & Name, const SKUsedName & EnumName, unsigned int uiElementOffset, unsigned int uiFlag)
			:SKProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			SKMAC_ASSERT(sizeof(T) == 4);
			m_EnumName = EnumName;
		}
		virtual ~SKEnumProperty()
		{

		}
		virtual bool Clone(SKProperty * p)
		{
			SKEnumProperty * Temp = (SKEnumProperty *)p;
			if (!SKProperty::Clone(Temp))
				return false;
			m_EnumName = Temp->m_EnumName;
			return true;

		}
		virtual bool SetValue(void* pObj, unsigned int& pDataSrc) const
		{

			*(unsigned int*)(((unsigned char*)pObj) + m_uiElementOffset) =  pDataSrc;
			return true;

		}
		virtual bool GetValue(void* pObj, unsigned int& pDataDest) const
		{
			pDataDest = *(unsigned int*)(((unsigned char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual bool GetValue(const void* pObj, unsigned int& pDataDest) const
		{
			pDataDest = *(const unsigned int*)(((const char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual unsigned int& Value(void* pObj)const
		{
			return *(unsigned int*)(((const char*)pObj) + m_uiElementOffset);
		}
		virtual bool Archive(SKStream &Stream,void* pObj)
		{
			Stream.Archive(Value(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,SKMap<SKObject *,SKObject*>& CloneMap)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,SKECollection * pParent,SKString &Name)
		{
			SKEEnumProperty * pEp = SK_NEW SKEEnumProperty((unsigned int*)(((const char*)pSrcObj) + m_uiElementOffset),Name,(SKObject*)pSrcObj);
			pParent->AddElement(pEp);
			SKArray<SKString> AS;
			SKString EnumName = m_EnumName.GetString();
			GET_ENUMARRAY(EnumName,AS);
			pEp->AddEnumString(AS);
		}
		virtual SKProperty * GetInstance()
		{
			return SK_NEW SKEnumProperty();
		}
		SKUsedName & GetEnumName()
		{
			return m_EnumName;
		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_ENUM;
		}
	protected:
		SKUsedName m_EnumName;
	};
	template<typename T,typename NumType>
	class SKDataProperty : public SKProperty
	{
	public:
		SKDataProperty(SKRtti & Owner,const SKUsedName & Name,unsigned int uiElementOffset,unsigned int uiDataNum,bool bDynamicCreate)
			:SKProperty(Owner,Name,uiElementOffset,F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = uiDataNum;
			m_bDynamicCreate = bDynamicCreate;
		}
		SKDataProperty(SKFunction & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiDataNum, bool bDynamicCreate)
			:SKProperty(Owner, Name, uiElementOffset, F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = uiDataNum;
			m_bDynamicCreate = bDynamicCreate;
		}
		SKDataProperty()
		{

		}
		SKDataProperty(SKRtti & Owner,const SKUsedName & Name,unsigned int uiElementOffset,unsigned int uiNumElementOffset)
			:SKProperty(Owner, Name, uiElementOffset, F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = 0;
			m_bDynamicCreate = true;
			m_uiNumElementOffset = uiNumElementOffset;
		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_DATA;
		}
		virtual void AddEditorElement(void * pSrcObj,SKECollection * pParent,SKString &Name)
		{
			return ;
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			SKMAC_ASSERT(0);
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,SKMap<SKObject *,SKObject*>& CloneMap)
		{
			T * SrcValueAddres = *(T**)GetValueAddress(pSrcObj);

			if (m_uiDataNum > 0)
			{
				T** Temp = (T**)GetValueAddress(pDestObj);
				if (m_bDynamicCreate)
				{
					*Temp = SK_NEW T[m_uiDataNum];	
					SKMemcpy((void *)(*Temp),(void *)SrcValueAddres,m_uiDataNum * sizeof(T));
				}
				else
				{
					SKMemcpy((void *)(*Temp),(void *)SrcValueAddres,m_uiDataNum * sizeof(T));
				}
			}
			else
			{
				T** Temp = (T**)GetValueAddress(pDestObj);
				
				
				void * SrcNumOffSet = (void*)(((unsigned char*)pSrcObj) + m_uiNumElementOffset);
				void * DestNumOffSet = (void*)(((unsigned char*)pDestObj) + m_uiNumElementOffset);
				*(NumType*)DestNumOffSet = *(NumType*)SrcNumOffSet;
				NumType uiNum = *(NumType*)SrcNumOffSet;

				*Temp = SK_NEW T[uiNum];
				SKMemcpy((void *)(*Temp),(void *)SrcValueAddres,uiNum * sizeof(T));
			}
		}
		virtual bool Archive(SKStream &Stream,void* pObj)
		{
			unsigned int uiStreamFlag = Stream.GetStreamFlag();
			if (uiStreamFlag == SKStream::AT_SAVE)
			{
				T * ValueAddres = *(T**)GetValueAddress(pObj);

				if (m_uiDataNum > 0)
				{
					Stream.Write(ValueAddres,m_uiDataNum * sizeof(T));
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumElementOffset);
					NumType uiNum = *(NumType*)NumOffSet;
					Stream.Write(NumOffSet,sizeof(NumType));
					Stream.Write(ValueAddres,uiNum * sizeof(T));
				}
			}
			else if (uiStreamFlag == SKStream::AT_LOAD)
			{
				T** Temp = (T**)GetValueAddress(pObj);
				if (m_uiDataNum > 0)
				{
					
					if (m_bDynamicCreate)
					{
						*Temp = SK_NEW T[m_uiDataNum];									
						Stream.Read((void *)(*Temp),m_uiDataNum * sizeof(T));
						
					}
					else
					{
						Stream.Read((void *)(*Temp),m_uiDataNum * sizeof(T));
					}
					
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumElementOffset);
					Stream.Read(NumOffSet,sizeof(NumType));
					NumType uiNum = *(NumType*)NumOffSet;
					*Temp = SK_NEW T[uiNum];									
					Stream.Read((void *)(*Temp),uiNum * sizeof(T));
				}
			}
			else if (uiStreamFlag == SKStream::AT_SIZE)
			{
				if (m_uiDataNum > 0)
				{
					Stream.AddBufferSize(m_uiDataNum * sizeof(T));
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumElementOffset);
					NumType uiNum = *(NumType*)NumOffSet;
					Stream.AddBufferSize(sizeof(NumType));
					Stream.AddBufferSize(uiNum * sizeof(T));
				}
			}
			return true;
		}
		virtual SKProperty * GetInstance()
		{
			return SK_NEW SKDataProperty<T,NumType>();
		}
		virtual bool Clone(SKProperty * p)
		{
			SKDataProperty<T,NumType> * Temp = (SKDataProperty<T,NumType> *)p;
			if (!SKProperty::Clone(Temp))
				return false;
			m_bDynamicCreate = Temp->m_bDynamicCreate;
			m_uiDataNum = Temp->m_uiDataNum;
			m_uiNumElementOffset = Temp->m_uiNumElementOffset;
			return true;
		}
	protected:
		bool m_bDynamicCreate;
		unsigned int m_uiDataNum;
		unsigned int m_uiNumElementOffset;
	};
	
	template<typename T>
	class SKValueBaseProperty : public SKProperty
	{
	public:
		SKValueBaseProperty()
		{

		}
		SKValueBaseProperty(SKRtti & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:SKProperty(Owner,Name,uiElementOffset,uiFlag)
		{
			m_LowValue = LowValue;
			m_HightValue = HighValue;
			m_fStep = fStep;
			m_bRange = Range;
		}
		SKValueBaseProperty(SKFunction & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:SKProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			m_LowValue = LowValue;
			m_HightValue = HighValue;
			m_fStep = fStep;
			m_bRange = Range;
		}
		virtual ~SKValueBaseProperty()
		{
		
		}
		virtual bool Clone(SKProperty * p)
		{
			SKValueBaseProperty<T> * Temp = (SKValueBaseProperty<T> *)p;
			if (!SKProperty::Clone(Temp))
				return false;
			m_LowValue = Temp->m_LowValue;
			m_HightValue = Temp->m_HightValue;
			return true;
		}
	protected:
		T m_LowValue;
		T m_HightValue;
		T m_fStep;
		bool m_bRange;
	};
	template<typename T>
	class SKValueProperty : public SKValueBaseProperty<T>
	{
	public:
		SKValueProperty()
		{

		}
		SKValueProperty(SKRtti & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false ,T HighValue = T(), T LowValue = T(), T fStep = T())
			:SKValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range,HighValue, LowValue, fStep)
		{

		}
		SKValueProperty(SKFunction & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:SKValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		virtual ~SKValueProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_VALUE;
		}
		virtual bool SetValue(void* pObj, T& pDataSrc) const
		{

			if (pDataSrc > m_HightValue || pDataSrc < m_LowValue)
			{
				return false;
			}

			*(T*)(((unsigned char*)pObj) + m_uiElementOffset) =  pDataSrc;
			return true;
				
		}
		virtual bool GetValue(void* pObj, T& pDataDest) const
		{
			pDataDest = *(T*)(((unsigned char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual bool GetValue(const void* pObj, T& pDataDest) const
		{
			pDataDest = *(const T*)(((const char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual T& Value(void* pObj)const
		{
			return *(T*)(((const char*)pObj) + m_uiElementOffset);
		}
		virtual bool Archive(SKStream &Stream,void* pObj)
		{
			Stream.Archive(Value(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,SKMap<SKObject *,SKObject*>& CloneMap)
		{
			Copy(Value(pDestObj),Value(pSrcObj),CloneMap);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,SKECollection * pParent,SKString &Name)
		{
			CreateEditorElement(Value(pSrcObj), (SKObject *)pSrcObj, pParent, Name, m_bRange,m_HightValue, m_LowValue, m_fStep);
		}
		virtual SKProperty * GetInstance()
		{
			return SK_NEW SKValueProperty<T>();
		}
	};

	template<typename ArrayType,typename T>
	class SKArrayProperty : public SKValueBaseProperty<T>
	{
	public:
		SKArrayProperty()
		{

		}
		SKArrayProperty(SKRtti & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:SKValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		SKArrayProperty(SKFunction & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:SKValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		virtual ~SKArrayProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_ARRAY;
		}
		FORCEINLINE ArrayType & GetContaine(void* pObj)const
		{
			return (*(ArrayType*)(((unsigned char*)pObj) + m_uiElementOffset));
		}
		FORCEINLINE bool AddElement(void* pObj,unsigned int uiIndex,T& pDataSrc)
		{
			GetContaine(pObj).AddElement(pDataSrc);
			return true;
		}
		FORCEINLINE bool Erase(void* pObj,unsigned int i)
		{
			GetContaine(pObj).Erase(i);
		}
		virtual bool SetValue(void* pObj,unsigned int uiIndex,T& pDataSrc) const
		{

			if (pDataSrc > m_HightValue || pDataSrc < m_LowValue)
			{
				return false;
			}
			(GetContaine(pObj)[uiIndex]) =  pDataSrc;
			return true;
				
		}
		virtual bool GetValue(void* pObj, unsigned int uiIndex, T& pDataDest) const
		{
			pDataDest = (GetContaine(pObj)[uiIndex]);
			return true;
		}
		virtual bool Archive(SKStream &Stream,void* pObj)
		{
			Stream.Archive(GetContaine(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,SKMap<SKObject *,SKObject*>& CloneMap)
		{
			Copy(GetContaine(pDestObj),GetContaine(pSrcObj),CloneMap);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			GetContaine(pDestObj) = GetContaine(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,SKECollection * pParent,SKString &Name)
		{
			CreateEditorElement(GetContaine(pSrcObj), (SKObject *)pSrcObj, pParent, Name, m_bRange, m_HightValue, m_LowValue, m_fStep);
		}
		virtual SKProperty * GetInstance()
		{
			return SK_NEW SKArrayProperty<ArrayType,T>();
		}
	};


	template<typename MapType,typename KEY,typename VALUE>
	class SKMapProperty : public SKValueBaseProperty<VALUE>
	{
	public:
		SKMapProperty(SKRtti & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false,VALUE HighValue = VALUE(), VALUE LowValue = VALUE(), VALUE fStep = VALUE())
			:SKValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		SKMapProperty(SKFunction & Owner, const SKUsedName & Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, VALUE HighValue = VALUE(), VALUE LowValue = VALUE(), VALUE fStep = VALUE())
			:SKValueBaseProperty(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		SKMapProperty()
		{

		}
		virtual ~SKMapProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_MAP;
		}
		FORCEINLINE MapType & GetContaine(void* pObj)const
		{
			return (*(MapType*)(((unsigned char*)pObj) + m_uiElementOffset));
		}
		FORCEINLINE bool AddElement(void* pObj,unsigned int uiIndex,MapElement<KEY,VALUE>& pDataSrc)
		{
			GetContaine(pObj).AddElement(pDataSrc);
			return true;
		}
		FORCEINLINE bool Erase(void* pObj,unsigned int i)
		{
			GetContaine(pObj).Erase(i);
		}
		virtual bool SetValue(void* pObj,unsigned int uiIndex,KEY & Key,VALUE& Value) const
		{
			
			if (Value > m_HightValue || Value < m_LowValue)
			{
				return false;
			}

			MapElement<KEY,VALUE> Temp(Key,Value);
			(GetContaine(pObj)[uiIndex]) =  Temp;
			return true;

		}
		virtual bool GetValue(void* pObj, unsigned int uiIndex, MapElement<KEY,VALUE>& pDataDest) const
		{
			pDataDest = (GetContaine(pObj)[uiIndex]);
			return true;
		}
		virtual bool Archive(SKStream &Stream,void* pObj)
		{
			Stream.Archive(GetContaine(pObj));
			return true;
		}
		virtual void CloneData(void* pSrcObj,void* pDestObj,SKMap<SKObject *,SKObject*>& CloneMap)
		{
			Copy(GetContaine(pDestObj),GetContaine(pSrcObj),CloneMap);
		}
		virtual void CopyData(void* pSrcObj,void* pDestObj)
		{
			GetContaine(pDestObj) = GetContaine(pSrcObj);
		}
		virtual void AddEditorElement(void * pSrcObj,SKECollection * pParent,SKString &Name)
		{
			CreateEditorElement(GetContaine(pSrcObj), (SKObject *)pSrcObj, pParent, Name, m_bRange, m_HightValue, m_LowValue, m_fStep);
		}
		virtual SKProperty * GetInstance()
		{
			return SK_NEW SKMapProperty<MapType,KEY,VALUE>();
		}
	};


	template<class T,class NumType>
	struct DataPropertyCreator
	{
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,unsigned int NumOffset)
		{

			return SK_NEW SKDataProperty<T,NumType>(Owner, Name,Offset,NumOffset);
		}
		SKProperty* CreateFunctionProperty(const SKUsedName & Name, SKFunction & Owner, unsigned int Offset, unsigned int NumOffset)
		{

			return SK_NEW SKDataProperty<T, NumType>(Owner, Name, Offset, NumOffset);
		}
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,unsigned int uiDataNum,bool bDynamicCreate)
		{

			return SK_NEW SKDataProperty<T,NumType>(Owner, Name,Offset,uiDataNum,bDynamicCreate);
		}
	};
	template<class T>
	struct AutoPropertyCreator
	{
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value))
			{
				uiFlag |= SKProperty::F_NO_USE_GC;
			}
			return SK_NEW SKValueProperty<T>(Owner, Name,Offset,uiFlag);
	
			
		}
		SKProperty* CreateFunctionProperty(const SKUsedName & Name, SKFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return SK_NEW SKValueProperty<T>(Owner, Name, Offset, uiFlag);
		}
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,T HighValue ,T LowValue,SKREAL fStep,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value))
			{
				uiFlag |= SKProperty::F_NO_USE_GC;
			}
			return SK_NEW SKValueProperty<T>(Owner, Name, Offset, uiFlag,true,HighValue, LowValue, fStep);
		}
	};

	template<class T,SKMemManagerFun MMFun>
	struct AutoPropertyCreator<SKArray<T,MMFun>>
	{
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= SKProperty::F_NO_USE_GC;
			}
			return SK_NEW SKArrayProperty<SKArray<T,MMFun>,T>(Owner, Name,Offset,uiFlag);

		}
		SKProperty* CreateFunctionProperty(const SKUsedName & Name, SKFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return SK_NEW SKArrayProperty<SKArray<T, MMFun>, T>(Owner, Name, Offset, uiFlag);

		}
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,T HighValue ,T LowValue,T fStep,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= SKProperty::F_NO_USE_GC;
			}
			return SK_NEW SKArrayProperty<SKArray<T, MMFun>, T>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);

		}
	};


	template<class T,SKMemManagerFun MMFun>
	struct AutoPropertyCreator<SKArrayOrder<T,MMFun>>
	{
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= SKProperty::F_NO_USE_GC;
			}
			return SK_NEW SKArrayProperty<SKArrayOrder<T,MMFun>,T>(Owner, Name,Offset,uiFlag);
		}
		SKProperty* CreateFunctionProperty(const SKUsedName & Name, SKFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return SK_NEW SKArrayProperty<SKArrayOrder<T, MMFun>, T>(Owner, Name, Offset, uiFlag);
		}
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,T HighValue ,T LowValue,SKREAL fStep,unsigned int uiFlag)
		{
			if (!(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsVSType<T>::Value || ValueBase<T>::NeedsConstructor))
			{
				uiFlag |= SKProperty::F_NO_USE_GC;
			}
			return SK_NEW SKArrayProperty<SKArrayOrder<T, MMFun>, T>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);

		}
	};

	template<class KEY,class VALUE,SKMemManagerFun MMFun>
	struct AutoPropertyCreator<SKMap<KEY,VALUE,MMFun>>
	{
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{

			return SK_NEW SKMapProperty<SKMap<KEY,VALUE,MMFun>,KEY,VALUE>(Owner, Name,Offset,uiFlag);


		}
		SKProperty* CreateFunctionProperty(const SKUsedName & Name, SKFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return SK_NEW SKMapProperty<SKMap<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag);


		}
		SKProperty* CreateProperty(const SKUsedName & Name, SKRtti & Owner, unsigned int Offset,VALUE HighValue ,VALUE LowValue,SKREAL fStep,unsigned int uiFlag)
		{

			return SK_NEW SKMapProperty<SKMap<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);


		}
	};
	
	template<class KEY,class VALUE,SKMemManagerFun MMFun>
	struct AutoPropertyCreator<SKMapOrder<KEY,VALUE,MMFun>>
	{
		SKProperty* CreateProperty(const SKUsedName & Name,SKRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{

			return SK_NEW SKMapProperty<SKMapOrder<KEY,VALUE,MMFun>,KEY,VALUE>(Owner, Name,Offset,uiFlag);

		}
		SKProperty* CreateFunctionProperty(const SKUsedName & Name, SKFunction & Owner, unsigned int Offset, unsigned int uiFlag)
		{

			return SK_NEW SKMapProperty<SKMapOrder<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag);

		}
		SKProperty* CreateProperty(const SKUsedName & Name,SKRtti & Owner, unsigned int Offset,VALUE HighValue ,VALUE LowValue,SKREAL fStep,unsigned int uiFlag)
		{

			return SK_NEW SKMapProperty<SKMapOrder<KEY, VALUE, MMFun>, KEY, VALUE>(Owner, Name, Offset, uiFlag, HighValue, LowValue, fStep);
	
		}
		
	};

	
	class PropertyCreator
	{
	public:
		template<class ValueType>
		static AutoPropertyCreator<ValueType>& GetAutoPropertyCreator(ValueType& valueTypeDummyRef)
		{
			static AutoPropertyCreator<ValueType> apc;
			return apc;
		}
		template<class ValueType,class NumType>
		static DataPropertyCreator<ValueType,NumType>& GetAutoPropertyCreator(ValueType *& valueTypeDummyRef,NumType& valueNumTypeDummyRef)
		{
			static DataPropertyCreator<ValueType,NumType> apc;
			return apc;
		}
		template<class ValueType>
		static SKProperty* CreateEnumProperty(ValueType& valueTypeDummyRef,const SKUsedName & Name, const SKUsedName & EnumName,SKRtti & Owner, unsigned int Offset,unsigned int uiFlag)
		{
			return SK_NEW SKEnumProperty<ValueType>(Owner, Name, EnumName, Offset, uiFlag | SKProperty::F_NO_USE_GC);
		}
	};
	typedef void(*FunctionTemplatePtr)(SKObject * p, SKFunction * pFun, void * para, void *ret);
	class SKFunction
	{
	public:
		enum	// Flag 
		{
			F_DEFAULT	= 0X00,
			F_CLIENT	= 0X01,
			F_SERVER	= 0X02,
			F_REFLECT_UI =0X04,
			F_MAX
		};
		SKFunction()
		{
			m_pReturnProperty = NULL;
			ObjectFun = NULL;
		}
		~SKFunction()
		{
			for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
			{
				if (m_PropertyArray[i])
				{
					SKMAC_DELETE(m_PropertyArray[i])
				}
			}
			SKMAC_DELETE(m_pReturnProperty);
		}
		SKFunction(SKRtti & Owner, const SKUsedName & Name, unsigned int uiFlag)
		:m_pOwner(&Owner)
		{
			m_Name = Name;
			m_uiFlag = uiFlag;
			m_pReturnProperty = NULL;
			ObjectFun = NULL;
		}
		FORCEINLINE SKRtti * GetRtti()const
		{
			return m_pOwner;
		}
		FORCEINLINE const SKUsedName & GetName()const
		{
			return m_Name;
		}
		virtual bool Clone(SKFunction * f)
		{
			if ((f->GetRtti() == GetRtti() && GetRtti()))
			{
				m_Name = f->m_Name;
				m_uiFlag = f->m_uiFlag;
				for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
				{
					SKProperty * p = f->m_PropertyArray[i]->GetInstance();
					m_PropertyArray.AddElement(p);
					p->Clone(f->m_PropertyArray[i]);
				}
				return true;
			}
			return false;

		}
		virtual SKFunction * GetInstance()
		{
			return SK_NEW SKFunction();
		}
		void SetOwner(SKRtti & Owner)
		{
			m_pOwner = &Owner;
		}
		unsigned int GetFlag()const
		{
			return m_uiFlag;
		}
		void AddProperty(SKProperty * P)
		{
			m_PropertyArray.AddElement(P);
		}
		SKProperty * GetProperty(unsigned int i)
		{
			return m_PropertyArray[i];
		}
		unsigned int GetPropertyNum()const
		{
			return m_PropertyArray.GetNum();
		}
		void SetReturnType(SKProperty *p)
		{
			m_pReturnProperty = p;
		}
		bool IsReturnVoid()
		{
			return !m_pReturnProperty;
		}
		FunctionTemplatePtr ObjectFun;
		void SetTotleSize(unsigned int uiTotleSize)
		{
			m_uiTotleSize = uiTotleSize;
		}
		bool IsSame(SKFunction * p)
		{
			if (m_pOwner != p->m_pOwner)
			{
				return false;
			}
			else if (m_Name != p->m_Name)
			{
				return false;
			}
			else if (m_uiTotleSize != p->m_uiTotleSize)
			{
				return false;
			}
			else if (m_PropertyArray.GetNum() != p->m_PropertyArray.GetNum())
			{
				return false;
			}
			else 
			{
				return true;
			}
		}
	protected:
		SKRtti * m_pOwner;
		SKUsedName m_Name;
		unsigned int m_uiFlag;
		SKArray<SKProperty *> m_PropertyArray;
		SKProperty * m_pReturnProperty;
		unsigned int m_uiTotleSize;
	};

	

}


#endif