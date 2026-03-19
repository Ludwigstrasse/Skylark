#ifndef SKASYNLOADER_H
#define SKASYNLOADER_H
#include "SKThread.h"
#include "SKSafeQueue.h"
#include "SKResourceManager.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKResourceJob : public SKMemObject
	{
	public:
		enum //Load state
		{
			LS_PENDING,
			LS_LOADED,
			LS_FAIL,
		};
		enum // JOB TYPE
		{
			JT_TEXTURE,
			JT_MATERIAL,
			JT_ANIM,
			JT_STATIC_MESH,
			JT_SKELECTON_MESH,
			JT_ANIMTREE,
			JT_MORPHTREE,
			JT_ACTOR,
			JT_MAP,
			JT_FONT,
		};
		enum // JOB STATE
		{
			JS_NONE,
			JS_DELETE,

		};
		SKResourceJob();
		virtual ~SKResourceJob() = 0;
		unsigned int m_uiLoadState;
		unsigned int m_uiJobType;
		unsigned int m_uiJobState;
		virtual bool Process() = 0;
		virtual bool Load() = 0;
	
	};
	class SKGRAPHIC_API SKFileRJob :public SKResourceJob
	{
	public:
		SKFileRJob();
		virtual ~SKFileRJob() = 0;
		virtual bool Process() = 0;
		virtual bool Load();
		SKFileName  m_FileName;
	protected:
		unsigned char * m_pBuffer;
		unsigned int m_uiSize;
	};
	class SKGRAPHIC_API SKTextureRJob : public SKFileRJob
	{
	public:
		SKTextureRJob(SKTexAllStateRPtr& pProxy);
		virtual ~SKTextureRJob();
		SKTexAllStateRPtr m_pProxy;
		bool m_bEngineType;
		bool m_bCompress;
		unsigned int m_uiCompressType;
		bool m_bIsNormal;
		bool m_bSRGB;
		SKSamplerStatePtr	m_pSamplerState;
		
	
		virtual bool Process();
	protected:
		SKTexAllState * LoadTexture();
		SKTexAllState * Load2DTexture();
		SKTexAllState * Load2DTextureCompress();

	};
	class SKGRAPHIC_API SKMaterialRJob : public SKFileRJob
	{
	public:
		SKMaterialRJob(SKMaterialRPtr& pProxy);
		virtual ~SKMaterialRJob();
		SKMaterialRPtr m_pProxy;
		virtual bool Process();
	}; 
	class SKGRAPHIC_API SKAnimRJob : public SKFileRJob
	{
	public:
		SKAnimRJob(SKAnimRPtr& pProxy);
		virtual ~SKAnimRJob();
		SKAnimRPtr m_pProxy;	
		virtual bool Process();
	}; 
	class SKGRAPHIC_API SKStaticMeshRJob : public SKFileRJob
	{
	public:
		SKStaticMeshRJob(SKStaticMeshNodeRPtr& pProxy);
		virtual ~SKStaticMeshRJob();
		SKStaticMeshNodeRPtr m_pProxy;

		virtual bool Process();
	}; 
	class SKGRAPHIC_API SKSkelectonMeshRJob : public SKFileRJob
	{
	public:
		SKSkelectonMeshRJob(SKSkelectonMeshNodeRPtr& pProxy);
		virtual ~SKSkelectonMeshRJob();
		SKSkelectonMeshNodeRPtr m_pProxy;
		virtual bool Process();
	}; 
	class SKGRAPHIC_API SKAnimTreeRJob : public SKFileRJob
	{
	public:
		SKAnimTreeRJob(SKAnimTreeRPtr& pProxy);
		virtual ~SKAnimTreeRJob();
		SKAnimTreeRPtr m_pProxy;
		virtual bool Process();
	}; 
	class SKGRAPHIC_API SKMorphTreeRJob : public SKFileRJob
	{
	public:
		SKMorphTreeRJob(SKMorphTreeRPtr& pProxy);
		virtual ~SKMorphTreeRJob();
		SKMorphTreeRPtr m_pProxy;
		virtual bool Process();
	}; 
	class SKGRAPHIC_API SKActorRJob : public SKFileRJob
	{
	public:
		SKActorRJob(SKActorRPtr& pProxy);
		virtual ~SKActorRJob();
		SKActorRPtr m_pProxy;
		virtual bool Process();
	};
	class SKGRAPHIC_API SKSceneMapRJob : public SKFileRJob
	{
	public:
		SKSceneMapRJob(SKSceneMapRPtr& pProxy);
		virtual ~SKSceneMapRJob();
		SKSceneMapRPtr m_pProxy;
		virtual bool Process();
	};
	class SKGRAPHIC_API SKFontRJob : public SKFileRJob
	{
	public:
		SKFontRJob(SKFontRPtr& pProxy);
		virtual ~SKFontRJob();
		SKFontRPtr m_pProxy;
		virtual bool Process();
	};
	class SKGRAPHIC_API SKResourceLoaderThread : public SKThread
	{
	public:
		SKResourceLoaderThread();
		virtual ~SKResourceLoaderThread();
		void AddJob(SKResourceJob* m_pJob);
		void Stop();
	protected:
		virtual const TCHAR* GetThreadName()
		{
			return _T("SKResourceLoaderThread");
		}
		SKSafeQueue<SKResourceJob*> m_pResourceQueue;
		virtual void Run();	

	};
	
	class SKGRAPHIC_API SKASYNLoadManager
	{
	public:
		SKASYNLoadManager();
		~SKASYNLoadManager();
		void Update(double AppTime);
		void AddTextureLoad(SKTexAllStateRPtr& pTexture,SKFileName & FileName,
							bool bEngineType = true,
							SKSamplerStatePtr	pSamplerState = NULL,					
							bool bCompress = false,
							unsigned int uiCompressType = 0,
							bool bIsNormal = false,
							bool bSRGB = false);
		void AddMaterial(SKMaterialRPtr& pMaterial,SKFileName & FileName);
		void AddAnim(SKAnimRPtr& pAnim,SKFileName & FileName);
		void AddStaticMesh(SKStaticMeshNodeRPtr& pStaticMesh,SKFileName & FileName);
		void AddSkelectonMesh(SKSkelectonMeshNodeRPtr& pSkelectonMesh,SKFileName & FileName);
		void AddAnimTree(SKAnimTreeRPtr& pAnimTree,SKFileName & FileName);
		void AddMorphTree(SKMorphTreeRPtr& pMorphTree,SKFileName & FileName);
		void AddActor(SKActorRPtr& pActor, SKFileName & FileName);
		void AddMap(SKSceneMapRPtr& pMap, SKFileName & FileName);
		void AddFont(SKFontRPtr& pMap, SKFileName & FileName);
		void DeleteLoadResource(SKFileName & FileName);
		static SKASYNLoadManager * ms_pASYNLoadManager;
	protected:
		SKResourceLoaderThread m_ResourceLoadThread;
		SKArray<SKResourceJob * > m_ResourceJobArray;
		void AddJob(SKResourceJob* m_pJob);

	};
}
#endif