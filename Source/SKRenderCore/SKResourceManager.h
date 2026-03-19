#ifndef SKRESOURCEMANAGER_H
#define SKRESOURCEMANAGER_H
#include "SKObject.h"
#include "SKString.h"
#include "SKArray.h"
#include "SKMap.h"
#include "SKName.h"
#include "SKVertexFormat.h"
#include "SKShaderMap.h"
#include "SK1DTexture.h"
#define  USE_VECTOR_TRANSFORM_BONE
#include "SKCubeTexture.h"
#include "SK2DTexture.h"
#include "SK3DTexture.h"
namespace SKEngine2
{

	class SKResouceControll
	{
	public:
		SKResouceControll(unsigned int uiGCMaxTimeCount);
		virtual ~SKResouceControll();
		virtual void GCResource();
	protected:
		SKArray<unsigned int> m_TimeCount;
		unsigned int m_uiGCMaxTimeCount;
	};
	template<class T>
	class SKResourceArrayControll : public SKResouceControll
	{
	public:
		SKResourceArrayControll(unsigned int uiGCMaxTimeCount = 5000);
		~SKResourceArrayControll();
		bool AddResource(const T & R);
		bool DeleteResource(const T & R);
		virtual void GCResource();
		bool IsRealeseAll();
		void ClearAll();
		T  GetResource(unsigned int i);
		void ClearTimeCount(unsigned int i);
		unsigned int GetResourceNum();
	protected:
		SKArrayOrder<T> m_ResourceArray;
	};
	template<class T>
	SKResourceArrayControll<T>::SKResourceArrayControll(unsigned int uiGCMaxTimeCount)
		: SKResouceControll(uiGCMaxTimeCount)
	{

	}
	template<class T>
	SKResourceArrayControll<T>::~SKResourceArrayControll()
	{

	}
	template<class T>
	T  SKResourceArrayControll<T>::GetResource(unsigned int i)
	{
		if (i >= m_ResourceArray.GetNum())
		{
			return NULL;
		}

		return m_ResourceArray[i];
	}
	template<class T>
	unsigned int SKResourceArrayControll<T>::GetResourceNum()
	{
		return m_ResourceArray.GetNum();
	}
	template<class T>
	void SKResourceArrayControll<T>::ClearTimeCount(unsigned int i)
	{
		m_TimeCount[i] = 0;
	}
	template<class T>
	bool SKResourceArrayControll<T>::AddResource(const T & R)
	{
		m_ResourceArray.AddElement(R);
		m_TimeCount.AddElement(0);
		return true;
	}
	template<class T>
	bool SKResourceArrayControll<T>::DeleteResource(const T & R)
	{
		unsigned int i = m_ResourceArray.FindElement(R);
		if (i == m_ResourceArray.GetNum())
		{
			return false;
		}
		else
		{
			m_ResourceArray.Erase(i);
			m_TimeCount.Erase(i);
		}
		return true;
	}
	template<class T>
	void SKResourceArrayControll<T>::GCResource()
	{
		unsigned int i = 0 ;
		while(i < m_ResourceArray.GetNum())
		{
			if(m_ResourceArray[i]->GetRef() == 1)
			{
				m_TimeCount[i]++;
				if (m_TimeCount[i] > m_uiGCMaxTimeCount)
				{
					m_ResourceArray.Erase(i);
					m_TimeCount.Erase(i);
					break;
				}
				else
				{
					i++;
				}			
			}
			else
			{
				m_TimeCount[i] = 0;
				i++;
			}
		}
	}
	template<class T>
	bool SKResourceArrayControll<T>::IsRealeseAll()
	{
		return m_ResourceArray.GetNum() == 0;
	}
	template<class T>
	void SKResourceArrayControll<T>::ClearAll()
	{
		m_ResourceArray.Clear();
	}
	template<class KEY,class VALUE>
	class SKResourceSet : public SKResouceControll
	{
	protected:
		SKMapOrder<KEY,VALUE> m_Resource;
	public:
		SKResourceSet(unsigned int uiGCMaxTimeCount = 5000);
		~SKResourceSet();
		//дÿԴģԴ򷵻Դָ룬ڼԴ洴Դ
		VALUE  CheckIsHaveTheResource(const KEY & Key);
		
		bool AddResource(const KEY & Key,VALUE pObject);
		//дÿԴ
		bool DeleteResource(const KEY & Key);


		unsigned int GetResourceNum();

		bool IsRealeseAll();
		void ClearAll();
		const MapElement<KEY,VALUE> * GetResource(unsigned int i);
		void GCResource();
		unsigned int GetResourceIndexByKey(const KEY & Key)const;


	};
	template<class KEY,class VALUE>
	class SKProxyResourceSet : public SKResourceSet<KEY,VALUE>
	{
	public:
		SKProxyResourceSet(unsigned int uiGCMaxTimeCount = 5000);
		~SKProxyResourceSet();
		void GCResource();
	};
	template<class KEY,class VALUE>
	unsigned int SKResourceSet<KEY,VALUE>::GetResourceIndexByKey(const KEY & Key)const
	{

		unsigned int i = m_Resource.Find(Key);
		return i;


	}
	template<class KEY,class VALUE>
	void SKResourceSet<KEY,VALUE>::GCResource()
	{
		unsigned int i = 0 ; 
		while(i < m_Resource.GetNum())
		{
			const MapElement<KEY,VALUE> & Resource = m_Resource[i];
			if(Resource.Value->GetRef() == 1)
			{
				m_TimeCount[i]++;
				if (m_TimeCount[i] > m_uiGCMaxTimeCount)
				{
					m_Resource.Erase(i);
					m_TimeCount.Erase(i);
					break;
				}
				else
				{
					i++;
				}
			}
			else
			{
				m_TimeCount[i] = 0;
				i++;
			}
		}

	}
	template<class KEY,class VALUE>
	SKResourceSet<KEY,VALUE>::SKResourceSet(unsigned int uiGCMaxTimeCount)
		: SKResouceControll(uiGCMaxTimeCount)
	{
		m_Resource.Clear();
	}
	template<class KEY,class VALUE>
	SKResourceSet<KEY,VALUE>::~SKResourceSet()
	{
		m_Resource.Clear();
	}
	template<class KEY,class VALUE>
	VALUE  SKResourceSet<KEY,VALUE>::CheckIsHaveTheResource(const KEY & Key)
	{
		unsigned int i = m_Resource.Find(Key);
		if(i == m_Resource.GetNum())
			return NULL;
		//Clear TimeCout
		m_TimeCount[i] = 0;
		return m_Resource[i].Value;

	}

	template<class KEY,class VALUE>
	bool SKResourceSet<KEY,VALUE>::AddResource(const KEY & Key,VALUE  pObject)
	{
		if(!pObject)
			return 0;


		m_Resource.AddElement(Key,pObject);
		m_TimeCount.AddElement(0);
		
		return 1;
	}
	template<class KEY,class VALUE>
	bool SKResourceSet<KEY,VALUE>::DeleteResource(const KEY & Key)
	{

		unsigned int i = m_Resource.Find(Key);
		if(i == m_Resource.GetNum())
			return false;
		m_Resource.Erase(i);
		m_TimeCount.Erase(i);
		return true;


	}
	
	template<class KEY,class VALUE>
	unsigned int SKResourceSet<KEY,VALUE>::GetResourceNum()
	{
		return m_Resource.GetNum();
	}

	template<class KEY,class VALUE>
	bool SKResourceSet<KEY,VALUE>::IsRealeseAll()
	{
		return !m_Resource.GetNum();
	}
	template<class KEY,class VALUE>
	void SKResourceSet<KEY,VALUE>::ClearAll()
	{
		m_Resource.Clear();
	}
	template<class KEY,class VALUE>
	const MapElement<KEY,VALUE> * SKResourceSet<KEY,VALUE>::GetResource(unsigned int i)
	{
		SKMAC_ASSERT(i < m_Resource.GetNum());
		if (i >= m_Resource.GetNum())
		{
			return NULL;
		}
		return &m_Resource[i];
	}
	
	template<class KEY,class VALUE>
	SKProxyResourceSet<KEY,VALUE>::SKProxyResourceSet(unsigned int uiGCMaxTimeCount)
		:SKResourceSet<KEY,VALUE>(uiGCMaxTimeCount)
	{
	}
	template<class KEY,class VALUE>
	SKProxyResourceSet<KEY,VALUE>::~SKProxyResourceSet()
	{

	}
	template<class KEY,class VALUE>
	void SKProxyResourceSet<KEY,VALUE>::GCResource()
	{
		unsigned int i = 0;
		while(i < m_Resource.GetNum())
		{
			const MapElement<KEY,VALUE> & Resource = m_Resource[i];
			if(Resource.Value->IsEndableGC() && Resource.Value->IsLoaded() && Resource.Value->GetRef() == 1)
			{
				m_TimeCount[i]++;
				if (m_TimeCount[i] > m_uiGCMaxTimeCount)
				{
					m_Resource.Erase(i);
					m_TimeCount.Erase(i);
				}
				else
				{
					i++;
				}

			}
			else
			{
				m_TimeCount[i] = 0;
				i++;
			}
		}

	}

	class SKTexture;
	class SKDataBuffer;
	class SKFont;
	class SKShaderProgram;
	class SKVertexBuffer;
	class SKVertexFormat;
	class SKGeometry;
	class SKMaterial;
	class SKLight;
	class SKVShader;
	class SKPShader;
	class SKName;
	class SKTexAllState;
	class SKSkelectonMeshNode;
	class SKAnimSet;
	class SKCaptureTexAllState;
	class SKActor;
	class SKSceneMap;
	DECLARE_Ptr(SKTexAllState);
	DECLARE_Ptr(SKName);
	DECLARE_Ptr(SKVShader);
	DECLARE_Ptr(SKPShader);
	DECLARE_Ptr(SKGShader);
	DECLARE_Ptr(SKSkelectonMeshNode);

	DECLARE_Ptr(SKStaticMeshNode);
	DECLARE_Ptr(SKAnimSet);
	DECLARE_Ptr(SKMaterial);
	DECLARE_Ptr(SKFont);
	DECLARE_Ptr(SKVertexFormat);
	DECLARE_Ptr(SKFSM);
	DECLARE_Ptr(SKAnimTree);
	DECLARE_Ptr(SKMorphTree);
	DECLARE_Ptr(SKAnim);


	class SKDynamicBufferGeometry;


	DECLARE_Ptr(SKPostEffectSet);

	DECLARE_Proxy(SKTexAllState);
	DECLARE_Proxy(SKMaterial);
	DECLARE_Proxy(SKAnim);
	DECLARE_Proxy(SKFont);

	DECLARE_Proxy(SKStaticMeshNode);
	DECLARE_Proxy(SKSkelectonMeshNode);
	DECLARE_Proxy(SKPostEffectSet);
	DECLARE_Proxy(SKAnimTree);
	DECLARE_Proxy(SKMorphTree);
	DECLARE_Proxy(SKActor);
	DECLARE_Proxy(SKSceneMap);
	DECLARE_Ptr(SKRenderTarget);
	DECLARE_Ptr(SKDepthStencil);
	DECLARE_Ptr(SKBlendState);
	DECLARE_Ptr(SKDepthStencilState);
	DECLARE_Ptr(SKRasterizerState);
	DECLARE_Ptr(SKSamplerState);
	DECLARE_Ptr(SKDynamicBufferGeometry);
	class SKBlendDesc;
	class SKDepthStencilDesc;
	class SKRasterizerDesc;
	class SKSamplerDesc;
	class SKGCTask;
	struct DynamicBufferIndex
	{
		SKVertexFormat * pVertexFormat;
		unsigned int uiMeshDataType;
		DynamicBufferIndex()
		{
			pVertexFormat = NULL;
			uiMeshDataType = 0;
		}	
	};
	bool operator ==(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
	bool operator >(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
	bool operator <(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2);
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef SKDelegateEvent<void(void)> DelayUpdatObjectOneFrame;
	typedef SKDelegateEvent<void(SKREAL)> DelayUpdatObject;
#else
	typedef SKDelegateEvent0<void> DelayUpdatObjectOneFrame;
	typedef SKDelegateEvent1<void,SKREAL> DelayUpdatObject;
#endif
	class SKGRAPHIC_API SKResourceManager 
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		SKResourceManager();
		~SKResourceManager();
	
#define GET_INNER_RESOUCE_SET(ResouceName)\
		static SKResourceSet<unsigned int, VS##ResouceName##Ptr> & Get##ResouceName##Set()\
		{\
			static SKResourceSet<unsigned int, VS##ResouceName##Ptr> s_##ResouceName##Set; \
			return s_##ResouceName##Set; \
		}

		//ڲresource
		GET_INNER_RESOUCE_SET(VertexFormat);
		GET_INNER_RESOUCE_SET(Name);
		GET_INNER_RESOUCE_SET(BlendState);
		GET_INNER_RESOUCE_SET(DepthStencilState);
		GET_INNER_RESOUCE_SET(RasterizerState);
		GET_INNER_RESOUCE_SET(SamplerState);

#define GET_SHADER_MAP(ShaderMapName)\
		static SKShaderMap & Get##ShaderMapName()\
		{\
			static SKShaderMap s_##ShaderMapName(_T(#ShaderMapName)); \
			return s_##ShaderMapName; \
		}
		GET_SHADER_MAP(MaterialShaderMap);
		GET_SHADER_MAP(IndirectShaderMap);
		GET_SHADER_MAP(VertexShaderMap);
		GET_SHADER_MAP(PixelShaderMap);
		GET_SHADER_MAP(GeometryShaderMap);
		GET_SHADER_MAP(NormalDepthShaderMap);
		GET_SHADER_MAP(CubShadowShaderMap);
		GET_SHADER_MAP(VolumeShadowShaderMap);
		GET_SHADER_MAP(ShadowShaderMap);
		GET_SHADER_MAP(DualParaboloidShadowShaderMap);
		GET_SHADER_MAP(InnerVertexShaderMap);
		GET_SHADER_MAP(InnerPixelShaderMap);
		GET_SHADER_MAP(InnerGeometryShaderMap);
		static SKDVGeometry *GetDVGeometry(SKVertexFormat * pVertexFormat, unsigned int MeshDataType, unsigned int VertexNum);

		static void DeleteDynamicBufferGeometry(SKDynamicBufferGeometry * pDBG);

		static void ClearDynamicBufferGeometry();

		static void InitCacheShader(unsigned int RenderTypeAPI);
		static void LoadDefaultResource(unsigned int RenderTypeAPI);
		static const TCHAR* GetRenderTypeShaderPath(unsigned int RenderTypeAPI);

		static bool IsReleaseAll();
		static void CacheResource();
		static void GC();
		template <class T>
		static SKTexAllState * Create2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		template <class T>
		static SKTexAllState * Create1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		template <class T>
		static SKTexAllState * CreateCubTexture(unsigned int uiWidth,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		template <class T>
		static SKTexAllState * Create3DTexture(unsigned int uiWidth,unsigned int uiHeight, unsigned int uiLength,unsigned int uiFormatType,
			unsigned int uiMipLevel,T *pBuffer);

		static SKCaptureTexAllState * CreateCaptureTexture(const SKString & ViewFamilyName, unsigned int uiMipLevel = 1);

		static SKTexAllState * Load2DTexture(const TCHAR *pFileName,SKSamplerStatePtr pSamplerState = NULL,bool bSRGB = false);
		//0 no compress 1 dx3  2 dx5
		static SKTexAllState * Load2DTextureCompress(const TCHAR *pFileName,SKSamplerStatePtr pSamplerState = NULL,
			unsigned int uiCompressType = 0,bool bIsNormal = false,bool bSRGB = false);







		static SKTexAllState * NewLoadTexture(const TCHAR *pFileName);
		static bool NewSaveTexture(SKTexAllState * pTexture, const TCHAR * PathName, bool bSaveResourcePath = false);

		static SKMaterial * NewLoadMaterial(const TCHAR *pFileName);
		static bool NewSaveMaterial(SKMaterial * pMaterial, const TCHAR * PathName, bool bSaveResourcePath = false);


		static SKStaticMeshNode * NewLoadStaticMesh(const TCHAR *pFileName);
		static bool NewSaveStaticMesh(SKStaticMeshNode * pStaticMeshNode, const TCHAR * PathName, bool bSaveResourcePath = false);

		static SKSkelectonMeshNode * NewLoadSkelectonMesh(const TCHAR *pFileName);
		static bool NewSaveSkelectonMeshNode(SKSkelectonMeshNode * pSkelectonMesh, const TCHAR * PathName, bool bSaveResourcePath = false);

		static SKPostEffectSet * NewLoadPostEffect(const TCHAR *pFileName);
		static bool NewSavePostEffect(SKPostEffectSet * pPostEffectSet, const TCHAR * PathName, bool bSaveResourcePath = false);


		static bool NewSaveAction(SKAnim * pAnim, const TCHAR * PathName, bool bSaveResourcePath = false);
		static SKAnim * NewLoadAction(const TCHAR *pFileName);


		static bool NewSaveAnimTree(SKAnimTree * pAnim, const TCHAR * PathName, bool bSaveResourcePath = false);
		static SKAnimTree * NewLoadAnimTree(const TCHAR *pFileName);

		static bool NewSaveMorphTree(SKMorphTree * pMorphTree, const TCHAR * PathName, bool bSaveResourcePath = false);
		static SKMorphTree * NewLoadMorphTree(const TCHAR *pFileName);

		static bool NewSaveActor(SKActor * pActor, const TCHAR * PathName, bool bSaveResourcePath = false);
		static SKActor * NewLoadActor(const TCHAR *pFileName);

		static bool NewSaveMap(SKSceneMap * pFont, const TCHAR * PathName, bool bSaveResourcePath = false);
		static SKSceneMap * NewLoadMap(const TCHAR *pFileName);

		static bool NewSaveFont(SKFont * pFont, const TCHAR * PathName, bool bSaveResourcePath = false);
		static SKFont * NewLoadFont(const TCHAR *pFileName);

		static SKVertexFormat *LoadVertexFormat(SKVertexBuffer * pVertexBuffer,SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> *pFormatArray = NULL);
		static SKName * CreateName(const TCHAR * pChar);
		static SKName * CreateName(const SKString & String);
		static SKVShader * CreateVShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID);
		static SKPShader * CreatePShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID);
		static SKGShader * CreateGShader(MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID);
		static SKBlendState * CreateBlendState(const SKBlendDesc & BlendDesc);
		static SKDepthStencilState * CreateDepthStencilState(const SKDepthStencilDesc & DepthStencilDesc);
		static SKRasterizerState * CreateRasterizerState(const SKRasterizerDesc &RasterizerDesc);
		static SKSamplerState * CreateSamplerState(const SKSamplerDesc &SamplerDesc);

		
		//asynchronization

		static SKProxyResourceSet<SKUsedName ,SKTexAllStateRPtr> & GetASYNTextureSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKTexAllStateRPtr> s_ASYNTextureSet;
			return s_ASYNTextureSet;
		}
		static SKTexAllStateRPtr ms_DefaultTextureResource;
		static SKProxyResourceSet<SKUsedName ,SKMaterialRPtr> & GetASYNMaterialSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKMaterialRPtr> s_ASYNMaterialSet;
			return s_ASYNMaterialSet;
		}
		static SKMaterialRPtr ms_DefaultMaterialResource;
		static SKMaterialRPtr ms_DefaultOnlyColorMaterialResource;
		static SKStaticMeshNodeRPtr ms_DefaultStaticMeshNodeResource;
		static SKSkelectonMeshNodeRPtr ms_DefaultSkelectonMeshNodeResource;
		static SKProxyResourceSet<SKUsedName ,SKAnimRPtr> & GetASYNAnimSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKAnimRPtr> s_ASYNAnimSet;
			return s_ASYNAnimSet;
		}

		static SKProxyResourceSet<SKUsedName ,SKStaticMeshNodeRPtr> & GetASYNStaticMeshSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKStaticMeshNodeRPtr> s_ASYNStaticMeshSet;
			return s_ASYNStaticMeshSet;
		}
		
		static SKProxyResourceSet<SKUsedName ,SKSkelectonMeshNodeRPtr> & GetASYNSkelectonMeshSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKSkelectonMeshNodeRPtr> s_ASYNSkelectonMeshSet;
			return s_ASYNSkelectonMeshSet;
		}
		static SKProxyResourceSet<SKUsedName ,SKPostEffectSetRPtr> & GetASYNPostEffectSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKPostEffectSetRPtr> s_ASYNPostEffectSet;
			return s_ASYNPostEffectSet;
		}
		static SKProxyResourceSet<SKUsedName ,SKAnimTreeRPtr> & GetASYNAnimTreeSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKAnimTreeRPtr> s_ASYNAnimTreeSet;
			return s_ASYNAnimTreeSet;
		}
		static SKProxyResourceSet<SKUsedName ,SKMorphTreeRPtr> & GetASYNMorphTreeSet()
		{
			static SKProxyResourceSet<SKUsedName ,SKMorphTreeRPtr> s_ASYNMorphTreeSet;
			return s_ASYNMorphTreeSet;
		}
		static SKProxyResourceSet<SKUsedName, SKActorRPtr> & GetASYNActorSet()
		{
			static SKProxyResourceSet<SKUsedName, SKActorRPtr> s_ASYNActorSet;
			return s_ASYNActorSet;
		}
		static SKProxyResourceSet<SKUsedName, SKSceneMapRPtr> & GetASYNMapSet()
		{
			static SKProxyResourceSet<SKUsedName, SKSceneMapRPtr> s_ASYNMapSet;
			return s_ASYNMapSet;
		}
		static SKProxyResourceSet<SKUsedName, SKFontRPtr> & GetASYNFontSet()
		{
			static SKProxyResourceSet<SKUsedName, SKFontRPtr> s_ASYNFontSet;
			return s_ASYNFontSet;
		}
		static SKTexAllStateR * LoadASYN2DTexture(const TCHAR *pFileName,bool IsAsyn,SKSamplerStatePtr pSamplerState = NULL,bool bSRGB = false);
		//0 no compress 1 dx3  2 dx5
		static SKTexAllStateR * LoadASYN2DTextureCompress(const TCHAR *pFileName,bool IsAsyn,SKSamplerStatePtr pSamplerState = NULL,
			unsigned int uiCompressType = 0,bool bIsNormal = false,bool bSRGB = false);

		static SKTexAllStateR * LoadASYNTexture(const TCHAR *pFileName,bool IsAsyn);
		static SKMaterialR * LoadASYNMaterial(const TCHAR *pFileName,bool IsAsyn);

		static SKStaticMeshNodeR * LoadASYNStaticMesh(const TCHAR *pFileName,bool IsAsyn);
		static SKSkelectonMeshNodeR * LoadASYNSkelectonMesh(const TCHAR *pFileName,bool IsAsyn);
		static SKPostEffectSetR * LoadASYNPostEffect(const TCHAR *pFileName,bool IsAsyn);
		static SKAnimR * LoadASYNAction(const TCHAR *pFileName,bool IsAsyn);


		static SKAnimTreeR * LoadASYNAnimTree(const TCHAR *pFileName,bool IsAsyn);
		static SKMorphTreeR * LoadASYNMorphTree(const TCHAR *pFileName,bool IsAsyn);
		static SKActorR * LoadASYNActor(const TCHAR *pFileName, bool IsAsyn);

		static SKSceneMapR * LoadASYNMap(const TCHAR *pFileName, bool IsAsyn);

		static SKFontR * LoadASYNFont(const TCHAR *pFileName, bool IsAsyn);

		static SKResourceProxyBase * LoadResource(const TCHAR *pFileName,bool IsAsyn);

		static void DeleteMapResource(const TCHAR *  pFileName);
		static void DeleteAllMapResource();
		static SKRenderTarget * CreateRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
			unsigned int uiMulSample);
		static SKRenderTarget * CreateRenderTarget(SKTexture * pCreateBy,unsigned int uiMulSample = 0
			,unsigned int uiLevel = 0,unsigned int uiFace = 0);
		static SKDepthStencil * CreateDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
			unsigned int uiMulSample,unsigned int uiFormatType);

		//ȡģƽûʹõRTΪ˽ʡԴռ䣬һRTtexture󶨵ģͼmiplevel1
		static SKRenderTarget * Get2DRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample);

		static void Release2DRenderTarget(SKRenderTarget * pRenderTarget);

		static void Disable2DRenderTarget(SKRenderTarget * &pRenderTarget);

		static void Disable2DRenderTarget(SKRenderTargetPtr &pRenderTarget);

		static bool GetCubRenderTarget(unsigned int uiWidth, unsigned int uiFormatType,unsigned int uiMulSample,SKRenderTarget * OutRT[SKCubeTexture::F_MAX]);
		
		static void ReleaseCubRenderTarget(SKRenderTarget * RT[SKCubeTexture::F_MAX]);

		static void DisableCubRenderTarget(SKRenderTarget * RT[SKCubeTexture::F_MAX]);

		static SKDepthStencil * GetDepthStencil(unsigned int uiWidth,unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample);

		static void ReleaseDepthStencil(SKDepthStencil * pDepthStencil);

		static void DisableDepthStencil(SKDepthStencil * &pDepthStencil);
		static void DisableDepthStencil(SKDepthStencilPtr &pDepthStencil);

		//Source - Target
		static SKAnim * CreateAdditiveAnim(SKAnim * pSourceAnim, SKAnim * pTargetAnim, SKAnimR * pBlendAnimR);

		static SKAnim * CreateAdditiveAnim(SKAnim * pSourceAnim, SKAnim * pTargetAnim, SKAnimR * pBlendAnimR, SKREAL fTargetTime = 0.0f);

		static SKAnim * CreateAdditiveAnim(SKAnim * pSourceAnim, SKAnim * pTargetAnim, SKAnimR * pBlendAnimR, SKREAL fSourceTime = 0.0f, SKREAL fTargetTime = 0.0f);

		static SKAnim * CreateAnim(SKAnim * pSourceAnim, SKREAL fSourceTime = 0.0f);
		DECLARE_INITIAL_ONLY
	private:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		static bool CacheName();
		static bool CacheShader();
		static bool CacheVertexFormat();



		static void InitConfig();

		static SKResourceSet<DynamicBufferIndex, SKDVGeometryPtr> &GetDVGeometryArray()
		{
			static SKResourceSet<DynamicBufferIndex, SKDVGeometryPtr> s_DVGeometryArray;
			return s_DVGeometryArray;
		}
		static SKArray<SKDynamicBufferGeometryPtr> ms_SaveDelete;

		static SKResourceArrayControll<SKRenderTargetPtr> &GetRenderTargetArray()
		{
			static SKResourceArrayControll<SKRenderTargetPtr> s_RenderTargetArray;
			return s_RenderTargetArray;
		}
		static SKResourceArrayControll<SKDepthStencilPtr> &GetDepthStencilArray()
		{
			static SKResourceArrayControll<SKDepthStencilPtr> s_RenderTargetArray;
			return s_RenderTargetArray;
		}

		static SKResourceArrayControll<SKRenderTargetPtr> &GetRenderTargetBufferArray()
		{
			static SKResourceArrayControll<SKRenderTargetPtr> s_RenderTargetBufferArray;
			return s_RenderTargetBufferArray;
		}
		static SKResourceArrayControll<SKDepthStencilPtr> &GetDepthStencilBufferArray()
		{
			static SKResourceArrayControll<SKDepthStencilPtr> s_RenderTargetBufferArray;
			return s_RenderTargetBufferArray;
		}
		static bool IsResourceType(SKRtti &Rtti)
		{
			for (unsigned int i = 0; i < ms_ResourceRtti.GetNum();i++)
			{
				if (Rtti.IsDerived(*ms_ResourceRtti[i]))
				{
					return true;
				}
			}
			return false;
		}
	protected:
		static SKCriticalSection ms_TextureCri;
		static SKCriticalSection ms_MaterialCri;
		static SKCriticalSection ms_AnimCri;
		static SKCriticalSection ms_StaticMeshCri;
		static SKCriticalSection ms_SkeletonMeshCri;
		static SKCriticalSection ms_VShaderCri;
		static SKCriticalSection ms_PShaderCri;
		static SKCriticalSection ms_GShaderCri;
		static SKCriticalSection ms_PostEffectCri;
		static SKCriticalSection ms_NameCri;
		static SKCriticalSection ms_AnimTreeCri;
		static SKCriticalSection ms_MorphTreeCri;
		static SKCriticalSection ms_ActorCri;
		static SKCriticalSection ms_MapCri;
		static SKCriticalSection ms_FontCri;
		static SKArray<SKRtti *> ms_ResourceRtti;
	public:
		//help function
		template <class T>
		static bool GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,
			T * pOutDate,unsigned int uiChannel);
		template <class T>
		static bool GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,unsigned int uiInDateLenght,
			T * pOutDate,unsigned int uiChannel);
	public:
		static unsigned int GetGpuSkinBoneNum()
		{
			return ms_uiGpuSkinBoneNum;
		}
		static bool DrawSkelecton()
		{
			return ms_bDrawSkelecton;
		}
		static bool GetActiveDebugDraw()
		{
			return ms_bActiveDebugDraw;
		}

		static bool IsCacheShader()
		{
			return ms_bIsCacheShader;
		}
		static bool IsCacheName()
		{
			return ms_bIsCacheName;
		}
		static bool IsCacheVertexFormat()
		{
			return ms_bIsCacheVertexFormat;

		}

		static bool ms_bRenderThread;
		static bool ms_bUpdateThread;

		static unsigned int ms_CurRenderAPIType;
	protected:
		static unsigned int ms_uiGpuSkinBoneNum;
		static bool ms_bDrawSkelecton;
		static bool ms_bActiveDebugDraw;
		


		static bool ms_bIsCacheShader;
		static bool ms_bIsCacheName;
		static bool ms_bIsCacheVertexFormat;
public:
		static SKString ms_ActorPath;
		static SKString ms_MapPath;
		static SKString ms_FontPath;
		static SKString ms_TexturePath;
		static SKString ms_AnimPath;
		static SKString	ms_MaterialPath;
		static SKString	ms_PostEffectPath;
		static SKString ms_SkelectionMeshPath;
		static SKString ms_MorphPath;
		static SKString ms_OutputShaderCodePath;
		static SKString ms_OutPutLogPath;
		static SKString ms_ShaderPath;
		static SKString ms_StaticMeshPath;
		static SKString ms_TerrainPath;
		static SKString ms_ResourcePath;
		static bool ms_EnableAsynLoad;
protected:
		static SKArray<SKObject *> ms_pRootObject;
		static SKArrayOrder<SKObject *> ms_pGCObject;
public:
	/*
		GC
		ĬϴVS_NEWOBJECT GCԲᱻGCҪԼ
		ֻдObject GetInstance ĲŲGCҪGCAddRootObject
		ָԱ֤ҪGCObjectԷʵĵͻᱻGC
	
	*/
		static DelayUpdatObjectOneFrame & GetDelayUpdateObjectOneFrame()
		{
			static DelayUpdatObjectOneFrame s_DelayUpdateObjectOneFrame;
			return s_DelayUpdateObjectOneFrame;
		}
		static DelayUpdatObject & GetDelayUpdateObject()
		{
			static DelayUpdatObject s_DelayUpdateObject;
			return s_DelayUpdateObject;
		}
		static void DelayUpdate(SKREAL fTime)
		{
			GetDelayUpdateObjectOneFrame()();
			GetDelayUpdateObjectOneFrame().Reset();
			GetDelayUpdateObject()(fTime);
		}
		static void AddRootObject(SKObject * p)
		{
			p->SetFlag(SKObject::OF_RootObject);
			ms_pRootObject.AddElement(p);
		}
		static void DeleteRootObject(SKObject * p)
		{
			for (unsigned int i = 0; i < ms_pRootObject.GetNum(); i++)
			{
				if (ms_pRootObject[i] == p)
				{
					p->ClearFlag(SKObject::OF_RootObject);
					ms_pRootObject.Erase(i);
					break;
				}
			}
		}
		static void AddGCObject(SKObject * p)
		{
			ms_pGCObject.AddElement(p);
			p->SetFlag(SKObject::OF_GCObject);
		}

		static void GCObject();
		
		protected:
			friend class SKStream;
			static SKGCTask * ms_pCurGCTask;
			static SKGCTask * ms_pEndGCTask;
			static void RunGCTask();
			static void AddCanGCObject(SKArray<SKObject *>& CanGCObject);
		public:
			static void RunAllGCTask();

};
	template <class T>
	bool SKResourceManager::GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,
		T * pOutDate , unsigned int uiChannel)
	{
		SKMAC_ASSERT(pInDate && pOutDate);
		if (!pInDate || !pOutDate)
		{
			return false;
		}

		unsigned int uiOutDateWidth = uiInDateWidth >> 1;
		if (!uiOutDateWidth)
		{
			uiOutDateWidth = 1;
		}
		unsigned int uiOutDateHeight = uiInDateHeight >> 1;
		if (!uiOutDateHeight)
		{
			uiOutDateHeight = 1;
		}
		for (unsigned int i = 0 ;i < uiOutDateWidth ; i++)
		{
			unsigned int uiRow0 = i * 2;
			unsigned int uiRow1 = i * 2 + 1;

			if (uiRow1 >= uiInDateWidth)
			{
				uiRow1 = uiInDateWidth - 1;
			}
			for (unsigned int j = 0 ; j < uiOutDateHeight ; j++)
			{

				unsigned int uiCol0 = j * 2;
				unsigned int uiCol1 = j * 2 + 1;
				if (uiCol1 >= uiInDateHeight)
				{
					uiCol1 = uiInDateHeight - 1;
				}

				for (unsigned int k = 0 ; k < uiChannel ; k++)
				{
					pOutDate[(j + i * uiOutDateHeight) * uiChannel + k] 
					= (T)((pInDate[(uiCol0 + uiRow0 * uiInDateHeight) * uiChannel + k] + pInDate[(uiCol1 + uiRow0 * uiInDateHeight) * uiChannel + k]
					+ pInDate[(uiCol0 + uiRow1 * uiInDateHeight) * uiChannel + k] + pInDate[(uiCol1 + uiRow1 * uiInDateHeight) * uiChannel + k]) * 0.25f);
				}

			}
		}
		return true;
	}
	template <class T>
	bool SKResourceManager::GetNextMipDate(const T * pInDate,unsigned int uiInDateWidth,unsigned int uiInDateHeight,unsigned int uiInDateLength,
		T * pOutDate , unsigned int uiChannel)
	{
		SKMAC_ASSERT(pInDate && pOutDate);
		if (!pInDate || !pOutDate)
		{
			return false;
		}

		unsigned int uiOutDateWidth = uiInDateWidth >> 1;
		if (!uiOutDateWidth)
		{
			uiOutDateWidth = 1;
		}
		unsigned int uiOutDateHeight = uiInDateHeight >> 1;
		if (!uiOutDateHeight)
		{
			uiOutDateHeight = 1;
		}
		unsigned int uiOutDateLength = uiInDateLength >> 1;
		if (!uiOutDateLength)
		{
			uiOutDateLength = 1;
		}
		for (unsigned int s = 0 ; s < uiOutDateLength ; s++)
		{
			unsigned int uiL0 = s * 2;
			unsigned int uiL1 = s * 2 + 1;

			if (uiL1 >= uiOutDateLength)
			{
				uiL1 = uiOutDateLength - 1;
			}

			for (unsigned int i = 0 ;i < uiOutDateWidth ; i++)
			{
				unsigned int uiRow0 = i * 2;
				unsigned int uiRow1 = i * 2 + 1;

				if (uiRow1 >= uiInDateWidth)
				{
					uiRow1 = uiInDateWidth - 1;
				}
				for (unsigned int j = 0 ; j < uiOutDateHeight ; j++)
				{

					unsigned int uiCol0 = j * 2;
					unsigned int uiCol1 = j * 2 + 1;
					if (uiCol1 >= uiInDateHeight)
					{
						uiCol1 = uiInDateHeight - 1;
					}

					for (unsigned int k = 0 ; k < uiChannel ; k++)
					{
						pOutDate[(j + i * uiOutDateHeight + s * uiOutDateHeight * uiOutDateWidth) * uiChannel + k] 
						= (T)((pInDate[(uiCol0 + uiRow0 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow0 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol0 + uiRow1 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow1 * uiInDateHeight + uiL0 * uiInDateHeight * uiInDateWidth) * uiChannel + k] +
							pInDate[(uiCol0 + uiRow0 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow0 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol0 + uiRow1 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] + 
							pInDate[(uiCol1 + uiRow1 * uiInDateHeight + uiL1 * uiInDateHeight * uiInDateWidth) * uiChannel + k] +
							) * 0.125f);
					}

				}
			}
		}
		
		return true;
	}
	template <class T>
	SKTexAllState * SKResourceManager::Create2DTexture(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth || !uiHeight)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
		{
			SKMAC_ASSERT(0);
			return NULL;
		}
		SKTexAllState * pTexAllState = NULL;
		VS2DTexture * pTexture = NULL;
		pTexture = SK_NEW VS2DTexture(uiWidth,uiHeight,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			SKMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					SKMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}
		

		pTexAllState = SK_NEW SKTexAllState(pTexture);

		return pTexAllState;
	}
	
	template <class T>
	SKTexAllState * SKResourceManager::Create1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth))
		{
			SKMAC_ASSERT(0);
			return NULL;
		}
		SKTexAllState * pTexAllState = NULL;
		VS1DTexture * pTexture = NULL;
		pTexture = SK_NEW VS1DTexture(uiWidth,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			SKMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					SKMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}

		pTexAllState = SK_NEW SKTexAllState(pTexture);

		return pTexAllState;
	}
	template <class T>
	SKTexAllState * SKResourceManager::CreateCubTexture(unsigned int uiWidth,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth))
		{
			SKMAC_ASSERT(0);
			return NULL;
		}
		SKTexAllState * pTexAllState = NULL;
		SKCubeTexture * pTexture = NULL;
		pTexture = SK_NEW SKCubeTexture(uiWidth,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			SKMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			for (unsigned int s = 0 ; s < SKCubeTexture::F_MAX ; s++)
			{
				T * pLast = (T *)pTexture->GetFaceBuffer(0,s);
				for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
				{
					T *pNow = (T *)pTexture->GetFaceBuffer(i,s);

					if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
					{
						SKMAC_DELETE(pTexture);
						return NULL;
					}
					pLast = (T *)pTexture->GetFaceBuffer(i,s);
				}
			}
		}

		pTexAllState = SK_NEW SKTexAllState(pTexture);

		return pTexAllState;
	}
	template <class T>
	SKTexAllState * SKResourceManager::Create3DTexture(unsigned int uiWidth,unsigned int uiHeight, unsigned int uiLength,unsigned int uiFormatType,
		unsigned int uiMipLevel,T *pBuffer)
	{
		if (!uiWidth || !uiHeight || !uiLength)
		{
			return NULL;
		}
		if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight) || !IsTwoPower(uiLength))
		{
			SKMAC_ASSERT(0);
			return NULL;
		}
		SKTexAllState * pTexAllState = NULL;
		VS3DTexture * pTexture = NULL;
		pTexture = SK_NEW VS3DTexture(uiWidth,uiHeight,uiLength,uiFormatType,uiMipLevel,1);
		if(!pTexture)
		{
			return NULL;
		}
		pTexture->CreateRAMDate();
		if (pBuffer)
		{
			T *pDestBuffer = (T *)pTexture->GetBuffer(0);
			SKMemcpy(pDestBuffer,pBuffer,pTexture->GetByteSize(0));

			T * pLast = (T *)pTexture->GetBuffer(0);
			for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
			{
				T *pNow = (T *)pTexture->GetBuffer(i);

				if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pTexture->GetLength(i - 1),pNow,pTexture->GetChannelPerPixel()))
				{
					SKMAC_DELETE(pTexture);
					return NULL;
				}
				pLast = (T *)pTexture->GetBuffer(i);
			}
		}

		pTexAllState = SK_NEW SKTexAllState(pTexture);

		return pTexAllState;
	}
}
#endif