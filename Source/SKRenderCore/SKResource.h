#ifndef SKRESOURCE_H
#define SKRESOURCE_H
#include "SKName.h"
#include "SKObject.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKResource
	{
	public:
		enum	//Resource Type
		{
			RT_TEXTURE,
			RT_SKELECTON_MODEL,
			RT_STATIC_MODEL,
			RT_ACTION,
			RT_MATERIAL,
			RT_POSTEFFECT,
			RT_SHADER,
			RT_ANIMTREE,
			RT_MORPHTREE,
			RT_TERRAIN,	
			RT_FSM,
			RT_ACTOR,
			RT_MAP,
			RT_FONTTYPE,
			RT_MAX
		};
		SKResource();
		virtual ~SKResource() = 0;
		FORCEINLINE const SKUsedName & GetResourceName()const
		{
			return m_ResourceName;
		}

		FORCEINLINE void SetResourceName(const SKUsedName & ResourceName)
		{
			m_ResourceName = ResourceName;
		}
		virtual unsigned int GetResourceType()const = 0;
		static const SKString &GetFileSuffix(unsigned int uiFileSuffix); 
	protected:
		SKUsedName	m_ResourceName;
		static SKString ms_FileSuffix[]; 
	};
	class SKGRAPHIC_API SKResourceProxyBase : public SKReference , public SKMemObject
	{
	public:
		SKResourceProxyBase();
		virtual ~SKResourceProxyBase() = 0;
		virtual bool IsEndableASYNLoad() = 0;
		virtual bool IsEndableGC() = 0;
		virtual bool IsLoaded() = 0;
		virtual const SKUsedName & GetResourceName() = 0;
		virtual unsigned int GetResourceType() = 0;
		void LoadEvent()
		{
			for (unsigned int i = 0 ; i < m_LoadedEventObject.GetNum() ; i++)
			{
				m_LoadedEventObject[i].Key->LoadedEvent(this, m_LoadedEventObject[i].Value);
			}
			m_LoadedEventObject.Destroy();
		}
		void AddLoadEventObject(SKObject * pObject,int Data = 0)
		{
			if (!pObject)
			{
				return ;
			}
			if (m_bIsLoaded == false)
			{
				m_LoadedEventObject.AddElement(pObject,Data);	
			}
			else
			{
				pObject->LoadedEvent(this);
			}
		}
	protected:
		SKMap<SKObjectPtr,int> m_LoadedEventObject;
		bool	m_bIsLoaded;
		SKUsedName	m_ResourceName;
		void Loaded()
		{
			m_bIsLoaded = true;
			LoadEvent();
		}
		void SetResourceName(const SKUsedName & ResourceName)
		{
			m_ResourceName = ResourceName;
		}
	};
	DECLARE_Ptr(SKResourceProxyBase);
	template<class T>
	class SKResourceProxy : public SKResourceProxyBase
	{
	public:
		SKResourceProxy();
		virtual ~SKResourceProxy();
		T * GetResource();
		virtual const SKUsedName & GetResourceName();
		friend class SKTextureRJob;
		friend class SKMaterialRJob;
		friend class SKAnimRJob;
		friend class SKRefPosRJob;
		friend class SKStaticMeshRJob;
		friend class SKSkelectonMeshRJob;
		friend class SKResourceManager;
		friend class SKVShaderRJob;
		friend class SKPShaderRJob;
		friend class SKAnimTreeRJob;
		friend class SKMorphTreeRJob;
		friend class SKActorRJob;
		friend class SKSceneMapRJob;
		friend class SKFontRJob;
		virtual bool IsLoaded();
		virtual bool IsEndableASYNLoad();
		virtual bool IsEndableGC();
		virtual unsigned int GetResourceType();
		void SetNewResource(SKPointer<T> pResource);
		static SKResourceProxy<T> * Create( T * pResource);
	protected:
		SKPointer<T> m_pResource;
		SKPointer<T> m_pPreResource;
		
		
	};
	template<class T>
	SKResourceProxy<T> * SKResourceProxy<T>::Create( T * pResource)
	{
		SKResourceProxy<T> * pProxy = SK_NEW SKResourceProxy<T>();
		pProxy->SetResourceName(pResource->GetResourceName());
		pProxy->SetNewResource(pResource);
		pProxy->Loaded();
		return pProxy;
	}
	template<class T>
	SKResourceProxy<T>::SKResourceProxy()
	{
		m_bIsLoaded = false;
		m_pPreResource = (T *)T::GetDefalut();
		m_pResource = NULL;
	}
	template<class T>
	SKResourceProxy<T>::~SKResourceProxy()
	{
		m_pPreResource = NULL;
		m_pResource = NULL;
	}
	template<class T>
	void SKResourceProxy<T>::SetNewResource(SKPointer<T> pResource)
	{
		m_pResource = pResource;
		m_pResource->SetResourceName(m_ResourceName);
	}
	template<class T>
	bool SKResourceProxy<T>::IsEndableASYNLoad()
	{
		return T::ms_bIsEnableASYNLoader;
	}
	template<class T>
	bool SKResourceProxy<T>::IsEndableGC()
	{
		return T::ms_bIsEnableGC;
	}
	template<class T>
	bool SKResourceProxy<T>::IsLoaded()
	{
		return m_bIsLoaded;
	}
	template<class T>
	T * SKResourceProxy<T>::GetResource()
	{
		if (m_bIsLoaded )
		{
			return m_pResource;
		}
		else
		{
			return m_pPreResource;
		}
	}
	template<class T>
	unsigned int SKResourceProxy<T>::GetResourceType()
	{
		return GetResource()->GetResourceType();
	}

	template<class T>
	const SKUsedName & SKResourceProxy<T>::GetResourceName()
	{
		return m_ResourceName;
	}
#define DECLARE_Proxy(ClassName)	\
	typedef SKResourceProxy<ClassName> ##ClassName##R;\
	typedef SKPointer<SKResourceProxy<ClassName>> ##ClassName##RPtr;

	
}
#endif