#include "SKResourceManager.h"
#include "SKGeometry.h"
#include "SKLight.h"
#include "SKMaterial.h"
#include "SKVShader.h"
#include "SKPShader.h"
#include "SKGShader.h"
#include "SKDShader.h"
#include "SKHShader.h"
#include "SKName.h"
#include "SKTexAllState.h"
#include "SKImage.h"
#include "SKShaderStringFactory.h"
#include "SKMatch.h"
#include "SKNVCompression.h"
#include "SK2DTexture.h"
#include "SKGraphicInclude.h"
#include "SKASYNLoader.h"
#include "SKRenderTarget.h"
#include "SKDepthStencil.h"
#include "SKCubeTexture.h"
#include "SKStream.h"
#include "SKPostEffectSet.h"
#include "SKAnimTree.h"
#include "SKMorphTree.h"
#include "SKCaptureTexAllState.h"
#include "SKProfiler.h"
#include "SKActor.h"
#include "SKSceneMap.h"
#include "SKWorld.h"
#include "SKDynamicBufferGeometry.h"
using namespace SKEngine2;
namespace SKEngine2
{

	class SKGCTask :public SKMemObject
	{
	public:
		SKGCTask(SKArray<SKObject *>& CanGCObject, unsigned int CanGCNum = 50)
		{
			m_CanGCNum = CanGCNum;
			m_CanGCObject = CanGCObject;
			CurClearIndex = 0;
			CurDeleteIndex = 0;
			GCStream.SetStreamFlag(SKStream::AT_CLEAR_OBJECT_PROPERTY_GC);
			m_pNextTask = NULL;
		}
		void Run()
		{
			if (CurClearIndex >= m_CanGCObject.GetNum())
			{
				unsigned int MaxDeleteIndex = CurDeleteIndex + m_CanGCNum;
				for (; CurDeleteIndex < MaxDeleteIndex && CurDeleteIndex < m_CanGCObject.GetNum(); CurDeleteIndex++)
				{
					SKMAC_DELETE(m_CanGCObject[CurDeleteIndex]);
				}
			}
			else
			{
				unsigned int MaxClearIndex = CurClearIndex + m_CanGCNum;
				for (; CurClearIndex < MaxClearIndex && CurClearIndex < m_CanGCObject.GetNum(); CurClearIndex++)
				{
					GCStream.ArchiveAll(m_CanGCObject[CurClearIndex]);
				}
			}
		}
		SKGCTask * m_pNextTask;
		bool IsEnd()
		{
			return CurDeleteIndex >= m_CanGCObject.GetNum();
		}
	private:
		SKArray<SKObject *> m_CanGCObject;
		unsigned int m_CanGCNum;
		unsigned int CurClearIndex;
		unsigned int CurDeleteIndex;
		SKStream GCStream;

	};
}
SKResouceControll::SKResouceControll(unsigned int uiGCMaxTimeCount)
{
	m_uiGCMaxTimeCount = uiGCMaxTimeCount;
}
SKResouceControll::~SKResouceControll()
{

}
void SKResouceControll::GCResource()
{

}
namespace SKEngine2
{
	bool operator ==(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		return DBI1.pVertexFormat == DBI2.pVertexFormat && DBI1.uiMeshDataType == DBI2.uiMeshDataType;
	}
	bool operator >(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		if (DBI1.pVertexFormat > DBI2.pVertexFormat)
		{
			return true;
		}
		else if (DBI1.pVertexFormat == DBI2.pVertexFormat)
		{
			return DBI1.uiMeshDataType > DBI2.uiMeshDataType;
		}
		else
		{
			return false;
		}
	}
	bool operator < (const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		if (DBI1.pVertexFormat < DBI2.pVertexFormat)
		{
			return true;
		}
		else if (DBI1.pVertexFormat == DBI2.pVertexFormat)
		{
			return DBI1.uiMeshDataType < DBI2.uiMeshDataType;
		}
		else
		{
			return false;
		}
	}
}
SKArray<SKDynamicBufferGeometryPtr> SKResourceManager::ms_SaveDelete;
unsigned int SKResourceManager::ms_uiGpuSkinBoneNum = 70;
bool SKResourceManager::ms_bActiveDebugDraw = true;
bool SKResourceManager::ms_bDrawSkelecton = true;
bool SKResourceManager::ms_bRenderThread = false;
bool SKResourceManager::ms_bUpdateThread = false;
bool SKResourceManager::ms_bIsCacheName = false;
bool SKResourceManager::ms_bIsCacheShader = true;
bool SKResourceManager::ms_bIsCacheVertexFormat = false;
SKString SKResourceManager::ms_TexturePath;
SKString SKResourceManager::ms_AnimPath;
SKString SKResourceManager::ms_MaterialPath;
SKString SKResourceManager::ms_SkelectionMeshPath;
SKString SKResourceManager::ms_MorphPath;
SKString SKResourceManager::ms_OutputShaderCodePath;
SKString SKResourceManager::ms_OutPutLogPath;
SKString SKResourceManager::ms_ShaderPath;
SKString SKResourceManager::ms_StaticMeshPath;
SKString SKResourceManager::ms_TerrainPath;
SKString SKResourceManager::ms_ActorPath;
SKString SKResourceManager::ms_MapPath;
SKString SKResourceManager::ms_FontPath;
SKString SKResourceManager::ms_PostEffectPath;
SKString SKResourceManager::ms_ResourcePath;
SKCriticalSection SKResourceManager::ms_TextureCri;
SKCriticalSection SKResourceManager::ms_MaterialCri ;
SKCriticalSection SKResourceManager::ms_AnimCri ;
SKCriticalSection SKResourceManager::ms_StaticMeshCri ;
SKCriticalSection SKResourceManager::ms_SkeletonMeshCri ;
SKCriticalSection SKResourceManager::ms_VShaderCri ;
SKCriticalSection SKResourceManager::ms_PShaderCri ;
SKCriticalSection SKResourceManager::ms_GShaderCri;
SKCriticalSection SKResourceManager::ms_PostEffectCri ;
SKCriticalSection SKResourceManager::ms_NameCri;
SKCriticalSection SKResourceManager::ms_AnimTreeCri;
SKCriticalSection SKResourceManager::ms_MorphTreeCri;
SKCriticalSection SKResourceManager::ms_ActorCri;
SKCriticalSection SKResourceManager::ms_MapCri;
SKCriticalSection SKResourceManager::ms_FontCri;
unsigned int SKResourceManager::ms_CurRenderAPIType = 0;
SKArray<SKObject *>SKResourceManager::ms_pRootObject;
SKArrayOrder<SKObject *>SKResourceManager::ms_pGCObject;
SKGCTask * SKResourceManager::ms_pCurGCTask = NULL;
SKGCTask * SKResourceManager::ms_pEndGCTask = NULL;
SKArray<SKRtti *> SKResourceManager::ms_ResourceRtti;
SKStaticMeshNodeRPtr SKResourceManager::ms_DefaultStaticMeshNodeResource = NULL;
SKSkelectonMeshNodeRPtr SKResourceManager::ms_DefaultSkelectonMeshNodeResource = NULL;
SKTexAllStateRPtr SKResourceManager::ms_DefaultTextureResource = NULL;
SKMaterialRPtr SKResourceManager::ms_DefaultMaterialResource = NULL;
SKMaterialRPtr SKResourceManager::ms_DefaultOnlyColorMaterialResource = NULL;
bool SKResourceManager::ms_EnableAsynLoad = true;
IMPLEMENT_PRIORITY(SKResourceManager)
IMPLEMENT_INITIAL_ONLY_BEGIN(SKResourceManager);
ADD_PRIORITY(SKVertexFormat);
ADD_PRIORITY(SKVShader);
ADD_PRIORITY(SKPShader);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_ONLY_END
bool SKResourceManager::TerminalDefaultState()
{
	ms_DefaultStaticMeshNodeResource = NULL;
	ms_DefaultSkelectonMeshNodeResource = NULL;
	ms_DefaultTextureResource = NULL;
	ms_DefaultMaterialResource = NULL;
	ms_DefaultOnlyColorMaterialResource = NULL;
	return true;
}
bool SKResourceManager::InitialDefaultState()
{
	InitConfig();
	ms_ResourceRtti.AddElement(&SKAnimTree::ms_Type);
	ms_ResourceRtti.AddElement(&SKAnim::ms_Type);
	ms_ResourceRtti.AddElement(&SKMorphTree::ms_Type);
	ms_ResourceRtti.AddElement(&SKMaterialInstance::ms_Type);
	ms_ResourceRtti.AddElement(&SKShader::ms_Type);
	ms_ResourceRtti.AddElement(&SKMeshNode::ms_Type);
	ms_ResourceRtti.AddElement(&SKPostEffectSet::ms_Type);
	ms_ResourceRtti.AddElement(&SKTexAllState::ms_Type);
	ms_ResourceRtti.AddElement(&SKFont::ms_Type);
	return 1;
}
SKResourceManager::SKResourceManager()
{
	
}
SKResourceManager::~SKResourceManager()
{

}
void SKResourceManager::InitConfig()
{

	SKMatch Match;
	if(!Match.Open(_T("Resource\\SKConfig.txt")))
		return;

	//map
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return;
	} // end if

	if (Match.PatternMatch("['MapPath'][s>0]"))
	{
		ms_MapPath = Match.pstrings[0];
	} // end if

	//actor
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return;
	} // end if

	if (Match.PatternMatch("['ActorPath'][s>0]"))
	{
		ms_ActorPath = Match.pstrings[0];
	} // end if

	//texture
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['TexturePath'][s>0]"))
	{
		ms_TexturePath = Match.pstrings[0];
	} // end if

	// anim
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['AnimPath'][s>0]"))
	{
		ms_AnimPath = Match.pstrings[0];
	} // end if

	//material
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['MaterialPath'][s>0]"))
	{
		ms_MaterialPath = Match.pstrings[0];
	} // end if
	
	//material effect

	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['PostEffectPath'][s>0]"))
	{
		ms_PostEffectPath = Match.pstrings[0];
	} // end if
	
	//skelecton mesh
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['SKelectonMeshPath'][s>0]"))
	{
		ms_SkelectionMeshPath = Match.pstrings[0];
	} // end if

	//Morph
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['MorphPath'][s>0]"))
	{
		ms_MorphPath = Match.pstrings[0];
	} // end if

	//output
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['OutputShaderCodePath'][s>0]"))
	{
		ms_OutputShaderCodePath = Match.pstrings[0];
	} // end if

	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['OutputLogPath'][s>0]"))
	{
		ms_OutPutLogPath = Match.pstrings[0];
	} // end if

	//Shader
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['ShaderPath'][s>0]"))
	{
		ms_ShaderPath = Match.pstrings[0];
	} // end if

	//StaticMesh
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['StaticMeshPath'][s>0]"))
	{
		ms_StaticMeshPath = Match.pstrings[0];
	} // end if

	//Terrain

	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['TerrainPath'][s>0]"))
	{
		ms_TerrainPath = Match.pstrings[0];
	} // end if
	
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return ;
	} // end if

	if (Match.PatternMatch("['ResourcePath'][s>0]"))
	{
		ms_ResourcePath = Match.pstrings[0];
	}
	
	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return;
	} // end if
	if (Match.PatternMatch("['EnableAsynLoad'][s>0]"))
	{
		SKString TrueString = _T("true");
		ms_EnableAsynLoad = (Match.pstrings[0] == TrueString);
	}

	if (!Match.Getline())
	{
		SKMAC_ASSERT(0);
		return;
	} // end if
	if (Match.PatternMatch("['FontPath'][s>0]"))
	{
		ms_FontPath = Match.pstrings[0];
	}
}
SKDVGeometry *SKResourceManager::GetDVGeometry(SKVertexFormat * pVertexFormat, unsigned int MeshDataType, unsigned int VertexNum)
{
	if (!pVertexFormat || !VertexNum || MeshDataType>= SKMeshDate::MDT_MAX)
	{
		return NULL;
	}
	SKDVGeometryPtr  pBuffer = NULL;
	DynamicBufferIndex DBI;
	DBI.pVertexFormat = pVertexFormat;
	DBI.uiMeshDataType = MeshDataType;
	pBuffer = GetDVGeometryArray().CheckIsHaveTheResource(DBI);
	if (pBuffer == NULL)
	{
		pBuffer = NULL;
		pBuffer = SK_NEW SKDVGeometry();
		pBuffer->Create(pVertexFormat->m_FormatArray, MeshDataType, VertexNum);
		GetDVGeometryArray().AddResource(DBI, pBuffer);
	}
	else
	{	
		if (pBuffer->GetMaxRenderVertexNum() < VertexNum)
		{		
			ms_SaveDelete.AddElement(pBuffer.GetObject());
			pBuffer = SK_NEW SKDVGeometry();
			pBuffer->Create(pVertexFormat->m_FormatArray, MeshDataType, VertexNum);
			MapElement<DynamicBufferIndex, SKDVGeometryPtr> * PTemp = (MapElement<DynamicBufferIndex, SKDVGeometryPtr> *)(GetDVGeometryArray().GetResource(GetDVGeometryArray().GetResourceIndexByKey(DBI)));
			PTemp->Value = pBuffer;
		}
	}

	return pBuffer;
}
void SKResourceManager::ClearDynamicBufferGeometry()
{
	for (unsigned int i = 0; i < GetDVGeometryArray().GetResourceNum(); i++)
	{
		GetDVGeometryArray().GetResource(i)->Value->ClearInfo();
	}
	if (SKResourceManager::ms_bRenderThread)
	{
		for (unsigned int i = 0 ; i < SKBind::ms_BindArray.GetNum() ; i++)
		{
			SKBind * pBind  = SKBind::ms_BindArray[i];
			pBind->ASYNClearInfo();
		}
	}
	ms_SaveDelete.Clear();	
}
const TCHAR* SKResourceManager::GetRenderTypeShaderPath(unsigned int RenderTypeAPI)
{
	if (RenderTypeAPI == SKRenderer::RAT_DIRECTX9)
	{
		return _T("Dx9/");
	}
	else if (RenderTypeAPI == SKRenderer::RAT_DIRECTX11)
	{
		return _T("Dx11/");
	}
	else
	{
		return _T("");
	}
}
void SKResourceManager::LoadDefaultResource(unsigned int RenderTypeAPI)
{
	InitCacheShader(RenderTypeAPI);

	ms_DefaultTextureResource = SKTexAllStateR::Create((SKTexAllState *)SKTexAllState::GetDefalut());
	SKMaterial::LoadDefault();
	ms_DefaultMaterialResource = SKMaterialR::Create((SKMaterial *)SKMaterial::GetDefalut());
	SKGeometry::GetDefaultRenderCube()->AddMaterialInstance(ms_DefaultMaterialResource);
	ms_DefaultStaticMeshNodeResource = SKStaticMeshNodeR::Create((SKStaticMeshNode *)SKStaticMeshNode::GetDefalut());
	ms_DefaultSkelectonMeshNodeResource = SKSkelectonMeshNodeR::Create((SKSkelectonMeshNode*)SKSkelectonMeshNode::GetDefalut());
	ms_DefaultOnlyColorMaterialResource = SKMaterialR::Create((SKMaterial *)SKMaterial::GetDefaultOnlyColor());

}
void SKResourceManager::InitCacheShader(unsigned int RenderTypeAPI)
{
	ms_CurRenderAPIType = RenderTypeAPI;
	if (ms_CurRenderAPIType == SKRenderer::RAT_NULL)
	{
		return;
	}
	if (!SKResourceManager::IsCacheShader())
	{
		return;
	}
	SKString RenderAPIPre = SKResourceManager::GetRenderTypeShaderPath(ms_CurRenderAPIType);
#define LOAD_SHADER_CACHE(ShaderMapName) \
	{\
		SKStream LoadStream;\
		SKString FileName = ms_ShaderPath + RenderAPIPre + Get##ShaderMapName().m_ShaderMapName; \
		FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_SHADER);\
		LoadStream.NewLoad(FileName.GetBuffer());\
		SKShaderMapLoadSave * pShaderMapLoadSave = (SKShaderMapLoadSave *)LoadStream.GetObjectByRtti(SKShaderMapLoadSave::ms_Type);\
		if (pShaderMapLoadSave)\
		{\
			Get##ShaderMapName().GetShaderMap() = pShaderMapLoadSave->m_ShaderMap; \
			SKDelete(pShaderMapLoadSave); \
		}\
	}


	LOAD_SHADER_CACHE(MaterialShaderMap);
	LOAD_SHADER_CACHE(IndirectShaderMap);
	LOAD_SHADER_CACHE(VertexShaderMap);
	LOAD_SHADER_CACHE(PixelShaderMap);
	LOAD_SHADER_CACHE(GeometryShaderMap);
	LOAD_SHADER_CACHE(NormalDepthShaderMap);
	LOAD_SHADER_CACHE(CubShadowShaderMap);
	LOAD_SHADER_CACHE(VolumeShadowShaderMap);
	LOAD_SHADER_CACHE(ShadowShaderMap);
	LOAD_SHADER_CACHE(DualParaboloidShadowShaderMap);
	LOAD_SHADER_CACHE(InnerVertexShaderMap);
	LOAD_SHADER_CACHE(InnerPixelShaderMap);
	LOAD_SHADER_CACHE(InnerGeometryShaderMap);
}
bool SKResourceManager::CacheName()
{


	return 1;
}
bool SKResourceManager::CacheVertexFormat()
{

	return 1;
}
void SKResourceManager::AddCanGCObject(SKArray<SKObject *>& CanGCObject)
{
	if (CanGCObject.GetNum() == 0)
	{
		return;
	}
	if (!ms_pCurGCTask)
	{
		ms_pCurGCTask = SK_NEW SKGCTask(CanGCObject);
		ms_pEndGCTask = ms_pCurGCTask;
	}
	else
	{
		ms_pEndGCTask->m_pNextTask = SK_NEW SKGCTask(CanGCObject);
		ms_pEndGCTask = ms_pEndGCTask->m_pNextTask;
	}
}
void SKResourceManager::RunGCTask()
{
	if (ms_pCurGCTask)
	{
		ms_pCurGCTask->Run();
		if (ms_pCurGCTask->IsEnd())
		{
			SKGCTask * Temp = ms_pCurGCTask;
			ms_pCurGCTask = ms_pCurGCTask->m_pNextTask;
			SKMAC_DELETE(Temp);
		}
	}
}
void SKResourceManager::RunAllGCTask()
{
	ms_pRootObject.Clear();
	while (ms_pCurGCTask)
	{
		RunGCTask();
	}
	SKMAC_ASSERT(ms_pGCObject.GetNum() == 0);
}
void SKResourceManager::GCObject()
{
	{
		SKObject::GetObjectManager().PrepareForGC();
	}
	// collect not reachable object
	{
		SKStream GCCollectStream;
		GCCollectStream.SetStreamFlag(SKStream::AT_OBJECT_COLLECT_GC);
		for (unsigned int i = 0; i < ms_pRootObject.GetNum(); i++)
		{
			GCCollectStream.ArchiveAll(ms_pRootObject[i]);
		}
	}

	{
		SKArray<SKObject *> CanGCObject;
		for (unsigned int i = 0; i < ms_pGCObject.GetNum();)
		{
			SKObject * p = ms_pGCObject[i];
			if (p->IsHasFlag(SKObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				ms_pGCObject.Erase(i);
			}
			else
			{
				i++;
			}
		}

		AddCanGCObject(CanGCObject);
		RunGCTask();
	}

}
bool SKResourceManager::CacheShader()
{
	if (ms_CurRenderAPIType == SKRenderer::RAT_NULL)
	{
		return true;
	}
	SKString RenderAPIPre = SKResourceManager::GetRenderTypeShaderPath(ms_CurRenderAPIType);
#define SAVE_SHADER_CACHE(ShaderMapName)\
	{\
		SKStream SaveStream;\
		SaveStream.SetStreamFlag(SKStream::AT_REGISTER);\
		SKShaderMapLoadSave * pShaderMapLoadSave = SK_NEW SKShaderMapLoadSave();\
		pShaderMapLoadSave->m_ShaderMap = Get##ShaderMapName().GetShaderMap(); \
		SaveStream.ArchiveAll(pShaderMapLoadSave);\
		SKString FileName = ms_ShaderPath + RenderAPIPre + Get##ShaderMapName().m_ShaderMapName; \
		FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_SHADER);\
		SaveStream.NewSave(FileName.GetBuffer());\
		SKDelete(pShaderMapLoadSave);\
	}

	SAVE_SHADER_CACHE(MaterialShaderMap);
	SAVE_SHADER_CACHE(IndirectShaderMap);
	SAVE_SHADER_CACHE(VertexShaderMap);
	SAVE_SHADER_CACHE(PixelShaderMap);
	SAVE_SHADER_CACHE(GeometryShaderMap);
	SAVE_SHADER_CACHE(NormalDepthShaderMap);
	SAVE_SHADER_CACHE(CubShadowShaderMap);
	SAVE_SHADER_CACHE(VolumeShadowShaderMap);
	SAVE_SHADER_CACHE(ShadowShaderMap);
	SAVE_SHADER_CACHE(DualParaboloidShadowShaderMap);
	SAVE_SHADER_CACHE(InnerVertexShaderMap);
	SAVE_SHADER_CACHE(InnerPixelShaderMap);
	SAVE_SHADER_CACHE(InnerGeometryShaderMap);
	
	return 1;
}
void SKResourceManager::CacheResource()
{
	GetASYNMorphTreeSet().ClearAll();
	GetASYNAnimTreeSet().ClearAll();
	GetASYNSkelectonMeshSet().ClearAll();	
	GetASYNStaticMeshSet().ClearAll();	
	GetASYNMaterialSet().ClearAll();


	if (SKResourceManager::IsCacheShader())
	{
		CacheShader();

	}
#define CLEAR_SHADER_CACHE(ShaderMapName) 	Get##ShaderMapName().Clear(); 

	CLEAR_SHADER_CACHE(MaterialShaderMap);
	CLEAR_SHADER_CACHE(IndirectShaderMap);
	CLEAR_SHADER_CACHE(VertexShaderMap);
	CLEAR_SHADER_CACHE(PixelShaderMap);
	CLEAR_SHADER_CACHE(GeometryShaderMap);
	CLEAR_SHADER_CACHE(NormalDepthShaderMap);
	CLEAR_SHADER_CACHE(CubShadowShaderMap);
	CLEAR_SHADER_CACHE(VolumeShadowShaderMap);
	CLEAR_SHADER_CACHE(ShadowShaderMap);
	CLEAR_SHADER_CACHE(DualParaboloidShadowShaderMap);
	CLEAR_SHADER_CACHE(InnerVertexShaderMap);
	CLEAR_SHADER_CACHE(InnerPixelShaderMap);
	CLEAR_SHADER_CACHE(InnerGeometryShaderMap);
	GetASYNAnimSet().ClearAll();
	GetASYNTextureSet().ClearAll();
	GetASYNPostEffectSet().ClearAll();

	GetASYNActorSet().ClearAll();
	GetASYNMapSet().ClearAll();

	GetASYNFontSet().ClearAll();
	
	if (SKResourceManager::IsCacheVertexFormat())
	{
		CacheVertexFormat();
	}

	GetVertexFormatSet().ClearAll();

	GetBlendStateSet().ClearAll();
	GetDepthStencilStateSet().ClearAll();
	GetRasterizerStateSet().ClearAll();
	GetSamplerStateSet().ClearAll();

	if (SKResourceManager::IsCacheName())
	{
		CacheName();
	}

	GetNameSet().ClearAll();

	GetRenderTargetArray().ClearAll();
	GetDepthStencilArray().ClearAll();
	GetRenderTargetBufferArray().ClearAll();
	GetDepthStencilBufferArray().ClearAll();
	GetDVGeometryArray().ClearAll();
}
DECLEAR_TIME_PROFILENODE(GCTime,ApplicationUpdate)
void SKResourceManager::GC()
{
	ADD_TIME_PROFILE(GCTime)
	GetASYNTextureSet().GCResource();
	GetASYNAnimSet().GCResource();
	GetASYNMaterialSet().GCResource();
	GetASYNStaticMeshSet().GCResource();
	GetASYNSkelectonMeshSet().GCResource();
	GetASYNPostEffectSet().GCResource();
	GetASYNAnimTreeSet().GCResource();
	GetASYNMorphTreeSet().GCResource();
	GetASYNActorSet().GCResource();
	//GetASYNMapSet().GCResource();

	GetRenderTargetBufferArray().GCResource();
	GetDepthStencilBufferArray().GCResource();
	GCObject();
}
bool SKResourceManager::IsReleaseAll()
{
	if (!GetASYNStaticMeshSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNSkelectonMeshSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNAnimSet().IsRealeseAll())
	{
		return false;
	}
	
	if (!GetASYNTextureSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNMaterialSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNPostEffectSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNAnimTreeSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNMorphTreeSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNActorSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetASYNMapSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetASYNFontSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetVertexFormatSet().IsRealeseAll())
	{
		return false;
	}

	if (!GetMaterialShaderMap().IsRealeseAll())
	{
		return false;
	}
	if (!GetIndirectShaderMap().IsRealeseAll())
	{
		return false;
	}
	if (!GetVertexShaderMap().IsRealeseAll())
	{
		return false;
	}

	if (!GetPixelShaderMap().IsRealeseAll())
	{
		return false;
	}
	
	if (!GetInnerVertexShaderMap().IsRealeseAll())
	{
		return false;
	}

	if (!GetInnerPixelShaderMap().IsRealeseAll())
	{
		return false;
	}

	if (!GetNameSet().IsRealeseAll())
	{
		return false;
	}
	
	if (!GetBlendStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetSamplerStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetDepthStencilStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetRasterizerStateSet().IsRealeseAll())
	{
		return false;
	}
	if (!GetDVGeometryArray().IsRealeseAll())
	{
		return false;
	}
	SKMAC_ASSERT(ms_pRootObject.GetNum() == 0);
	SKMAC_ASSERT(ms_pGCObject.GetNum() == 0);
	return true;
}
SKCaptureTexAllState * SKResourceManager::CreateCaptureTexture(const SKString & ViewFamilyName,unsigned int uiMipLevel)
{
	SKCaptureTexAllState * pCaptureTexture = SK_NEW SKCaptureTexAllState();
	pCaptureTexture->SetViewCapture(ViewFamilyName);

	pCaptureTexture->SetMipLevel(uiMipLevel);

	return pCaptureTexture;
}
SKTexAllState * SKResourceManager::Load2DTexture(const TCHAR *pFileName,SKSamplerStatePtr pSamplerState,bool bSRGB)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 

	if (!FileName.GetExtension(Extension))
	{
		
		
		return NULL;
	}
	SKTexAllState * pTexAllState = NULL;


	SKImage *pImage = NULL;

	if (Extension == SKImage::ms_ImageFormat[SKImage::IF_BMP])
	{
		pImage = SK_NEW SKBMPImage();
	}
	else if (Extension == SKImage::ms_ImageFormat[SKImage::IF_TGA])
	{
		pImage = SK_NEW SKTGAImage();
	}
	else 
	{
		
		return NULL;
	}
	if(!pImage->Load(FileName.GetBuffer()))
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = SK_NEW VS2DTexture(uiWidth,uiHeight,SKRenderer::SFT_A8R8G8B8,0,1);
	if(!pTexture)
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for
	
	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			SKMAC_DELETE(pTexture);
			SKMAC_DELETE(pImage);
			
			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		SKMAC_DELETE(pImage);
	}
	pTexAllState = SK_NEW SKTexAllState(pTexture);

	if (pSamplerState)
	{
		pTexAllState->SetSamplerState(pSamplerState);
	}
	
	pTexAllState->SetSRGBEable(bSRGB);
	return pTexAllState;
}
SKTexAllState * SKResourceManager::Load2DTextureCompress(const TCHAR *pFileName,SKSamplerStatePtr pSamplerState,
											 unsigned int uiCompressType,bool bIsNormal,bool bSRGB)
{
	if (bIsNormal)
	{
		bSRGB = false;
	}
	
	if (!pFileName)
	{
		
		return NULL;
	}
	
	if (uiCompressType > SKNVCompression::NV_MAX)
	{
		
		return NULL;
	}

	SKFileName FileName = pFileName;
	SKString Extension; 

	if (!FileName.GetExtension(Extension))
	{
		
		return NULL;
	}
	SKTexAllState * pTexAllState = NULL;

	SKImage *pImage = NULL;
	if (Extension == SKImage::ms_ImageFormat[SKImage::IF_BMP])
	{
		pImage = SK_NEW SKBMPImage();
	}
	else if (Extension == SKImage::ms_ImageFormat[SKImage::IF_TGA])
	{
		pImage = SK_NEW SKTGAImage();
	}
	else 
	{
		
		return NULL;
	}
	if(!pImage->Load(FileName.GetBuffer()))
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = SK_NEW VS2DTexture(uiWidth, uiHeight, SKRenderer::SFT_A8R8G8B8, 0, 1);
	if(!pTexture)
	{
		SKMAC_DELETE(pImage);
		
		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for

	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			SKMAC_DELETE(pTexture);
			SKMAC_DELETE(pImage);
			
			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		SKMAC_DELETE(pImage);
	}

	unsigned int uiCompressFormat = SKNVCompression::NV_ARGB;
	unsigned int uiVSTextureTpye = SKRenderer::SFT_A8R8G8B8;
	if (uiCompressType == 0)
	{
		uiCompressFormat = SKNVCompression::NV_ARGB;
		uiVSTextureTpye = SKRenderer::SFT_A8R8G8B8;
	}
	else if (uiCompressType == 1)
	{
		uiCompressFormat = SKNVCompression::NV_DXT3;
		uiVSTextureTpye = SKRenderer::SFT_DXT3;
	}
	else if (uiCompressType == 2)
	{
		uiCompressFormat = SKNVCompression::NV_DXT5;
		uiVSTextureTpye = SKRenderer::SFT_DXT5;
	}
	VS2DTexture * pNewTexture = SK_NEW VS2DTexture(uiWidth,uiHeight,uiVSTextureTpye,0,1);
	pNewTexture->CreateRAMDate();
	for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);
		SKNVCompression Help;
		Help.Setup(pNow,uiCompressFormat,pTexture->GetWidth(i),pTexture->GetHeight(i),false,bIsNormal);
		Help.Process();
		SKMemcpy(pNewTexture->GetBuffer(i),Help.OutputHandler.m_CompressDate.GetBuffer(),Help.OutputHandler.m_CompressDate.GetNum());
	}
	
	SKMAC_DELETE(pTexture);
	pTexAllState = SK_NEW SKTexAllState(pNewTexture);
	if (pSamplerState)
	{
		pTexAllState->SetSamplerState(pSamplerState);
	}
	pTexAllState->SetSRGBEable(bSRGB);
	return pTexAllState;
}

SKTexAllStateR * SKResourceManager::LoadASYN2DTexture(const TCHAR *pFileName,bool IsAsyn,SKSamplerStatePtr pSamplerState,bool bSRGB)
{
	if (!pFileName)
	{
		return NULL;
	}
	
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKImage::ms_ImageFormat[SKImage::IF_BMP] && Extension != SKImage::ms_ImageFormat[SKImage::IF_TGA])
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_TexturePath + FileName;
			
		}
	}
	else
	{
		return NULL;
	}
	ms_TextureCri.Lock();
	SKTexAllStateRPtr pTexAllState = NULL;
	pTexAllState = (SKTexAllStateR *)SKResourceManager::GetASYNTextureSet().CheckIsHaveTheResource(ResourceName);
	if(pTexAllState)
	{
		ms_TextureCri.Unlock();	
		return pTexAllState;
	}
	pTexAllState = SK_NEW SKTexAllStateR();

	pTexAllState->SetResourceName(ResourceName);


	SKResourceManager::GetASYNTextureSet().AddResource(ResourceName,pTexAllState);

	
	if (IsAsyn)
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddTextureLoad(pTexAllState,FileName,false,pSamplerState,false,0,false,bSRGB);
	}
	else
	{	
		SKTexAllState * pTex = Load2DTexture(FileName.GetBuffer(),pSamplerState,bSRGB);
		if (pTex)
		{
			pTexAllState->SetNewResource(pTex);
			pTexAllState->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}
		
	}
	ms_TextureCri.Unlock();
	return pTexAllState;
}
//0 no compress 1 dx3  2 dx5
SKTexAllStateR * SKResourceManager::LoadASYN2DTextureCompress(const TCHAR *pFileName,bool IsAsyn,SKSamplerStatePtr pSamplerState,
												  unsigned int uiCompressType,bool bIsNormal,bool bSRGB)
{
	if (!pFileName)
	{
		return NULL;
	}
	if (uiCompressType > SKNVCompression::NV_MAX)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKImage::ms_ImageFormat[SKImage::IF_BMP] && Extension != SKImage::ms_ImageFormat[SKImage::IF_TGA])
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_TexturePath + FileName;
			
		}
	}
	else
	{
		return NULL;
	}
	ms_TextureCri.Lock();
	SKTexAllStateRPtr pTexAllState = NULL;
	pTexAllState = (SKTexAllStateR *)SKResourceManager::GetASYNTextureSet().CheckIsHaveTheResource(ResourceName);
	if(pTexAllState)
	{
		ms_TextureCri.Unlock();
		return pTexAllState;
	}
	pTexAllState = SK_NEW SKTexAllStateR();

	pTexAllState->SetResourceName(ResourceName);


	SKResourceManager::GetASYNTextureSet().AddResource(ResourceName,pTexAllState);

	if (IsAsyn)
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddTextureLoad(pTexAllState,FileName,false,pSamplerState,true,uiCompressType,bIsNormal,bSRGB);
	}
	else
	{	
		SKTexAllState * pTex = Load2DTextureCompress(FileName.GetBuffer(),pSamplerState,uiCompressType,bIsNormal,bSRGB);
		if (pTex)
		{
			pTexAllState->SetNewResource(pTex);
			pTexAllState->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}
		
	}
	ms_TextureCri.Unlock();
	return pTexAllState;
}
SKResourceProxyBase * SKResourceManager::LoadResource(const TCHAR *pFileName,bool IsAsyn)
{
	SKFileName FileName = pFileName;
	SKString Extension; 
	FileName.GetExtension(Extension);
	SKResourceProxyBase * pResource = NULL;
	if (Extension == SKResource::GetFileSuffix(SKResource::RT_TEXTURE))
	{
		pResource = LoadASYNTexture(pFileName,IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_MATERIAL))
	{
		pResource = LoadASYNMaterial(pFileName,IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_ACTION))
	{
		pResource = LoadASYNAction(pFileName,IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_STATIC_MODEL))
	{
		pResource = LoadASYNStaticMesh(pFileName,IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_SKELECTON_MODEL))
	{
		pResource = LoadASYNSkelectonMesh(pFileName,IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_POSTEFFECT))
	{
		pResource = LoadASYNPostEffect(pFileName,IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_ANIMTREE))
	{
		pResource = LoadASYNAnimTree(pFileName, IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_MORPHTREE))
	{
		pResource = LoadASYNMorphTree(pFileName, IsAsyn);
	}
	else if (Extension == SKResource::GetFileSuffix(SKResource::RT_ACTOR))
	{
		pResource = LoadASYNActor(pFileName, IsAsyn);
	}
	return pResource;
}
SKTexAllStateR * SKResourceManager::LoadASYNTexture(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_TEXTURE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_TexturePath + FileName;
			
		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_TEXTURE);
		FileName =  ms_TexturePath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_TEXTURE);
		
	}
	ms_TextureCri.Lock();
	SKTexAllStateRPtr pTexAllState = NULL;
	pTexAllState = (SKTexAllStateR *)SKResourceManager::GetASYNTextureSet().CheckIsHaveTheResource(ResourceName);
	if(pTexAllState)
	{
		ms_TextureCri.Unlock();
		return pTexAllState;
	}
	pTexAllState = SK_NEW SKTexAllStateR();

	pTexAllState->SetResourceName(ResourceName);


	SKResourceManager::GetASYNTextureSet().AddResource(ResourceName,pTexAllState);

	
	if (IsAsyn && ms_EnableAsynLoad && pTexAllState->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddTextureLoad(pTexAllState,FileName);
	}
	else
	{	
		SKTexAllState * pTex = NewLoadTexture(FileName.GetBuffer());
		if (pTex)
		{
			pTexAllState->SetNewResource(pTex);
			pTexAllState->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);
		}
		
	}
	ms_TextureCri.Unlock();
	return pTexAllState;
	
}
SKStaticMeshNodeR * SKResourceManager::LoadASYNStaticMesh(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_STATIC_MODEL))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_StaticMeshPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_STATIC_MODEL);
		FileName =  ms_StaticMeshPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_STATIC_MODEL);

	}
	ms_StaticMeshCri.Lock();
	SKStaticMeshNodeRPtr pStaticMeshNodeR = NULL;
	pStaticMeshNodeR = (SKStaticMeshNodeR *)SKResourceManager::GetASYNStaticMeshSet().CheckIsHaveTheResource(ResourceName);
	if(pStaticMeshNodeR)
	{
		ms_StaticMeshCri.Unlock();	
		return pStaticMeshNodeR;
	}
	pStaticMeshNodeR = SK_NEW SKStaticMeshNodeR();

	pStaticMeshNodeR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNStaticMeshSet().AddResource(ResourceName,pStaticMeshNodeR);


	if (IsAsyn && ms_EnableAsynLoad && pStaticMeshNodeR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddStaticMesh(pStaticMeshNodeR,FileName);
	}
	else
	{	
		SKStaticMeshNode * pStaticMeshNode = NewLoadStaticMesh(FileName.GetBuffer());
		if(pStaticMeshNode)
		{
			pStaticMeshNodeR->SetNewResource(pStaticMeshNode);
			pStaticMeshNodeR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);
		}
	}
	ms_StaticMeshCri.Unlock();	
	return pStaticMeshNodeR;
}
SKSkelectonMeshNodeR * SKResourceManager::LoadASYNSkelectonMesh(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_SKELECTON_MODEL))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_SkelectionMeshPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_SKELECTON_MODEL);
		FileName =  ms_SkelectionMeshPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_SKELECTON_MODEL);

	}
	ms_SkeletonMeshCri.Lock();
	SKSkelectonMeshNodeRPtr pSkelectonMeshNodeR = NULL;
	pSkelectonMeshNodeR = (SKSkelectonMeshNodeR *)SKResourceManager::GetASYNSkelectonMeshSet().CheckIsHaveTheResource(ResourceName);
	if(pSkelectonMeshNodeR)
	{
		ms_SkeletonMeshCri.Unlock();
		return pSkelectonMeshNodeR;
	}
	pSkelectonMeshNodeR = SK_NEW SKSkelectonMeshNodeR();

	pSkelectonMeshNodeR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNSkelectonMeshSet().AddResource(ResourceName,pSkelectonMeshNodeR);


	if (IsAsyn && ms_EnableAsynLoad && pSkelectonMeshNodeR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddSkelectonMesh(pSkelectonMeshNodeR,FileName);
	}
	else
	{	
		SKSkelectonMeshNode * pSkelectonMesh = NewLoadSkelectonMesh(FileName.GetBuffer());
		if (pSkelectonMesh)
		{
			pSkelectonMeshNodeR->SetNewResource(pSkelectonMesh);
			pSkelectonMeshNodeR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);
		}
		}
	ms_SkeletonMeshCri.Unlock();
	return pSkelectonMeshNodeR;
}
SKPostEffectSetR * SKResourceManager::LoadASYNPostEffect(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_POSTEFFECT))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_PostEffectPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_POSTEFFECT);
		FileName =  ms_PostEffectPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_POSTEFFECT);

	}
	ms_PostEffectCri.Lock();
	SKPostEffectSetRPtr pPostEffectSetR = NULL;
	pPostEffectSetR = (SKPostEffectSetR *)SKResourceManager::GetASYNPostEffectSet().CheckIsHaveTheResource(ResourceName);
	if(pPostEffectSetR)
	{
		ms_PostEffectCri.Unlock();
		return pPostEffectSetR;
	}
	pPostEffectSetR = SK_NEW SKPostEffectSetR();

	pPostEffectSetR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNPostEffectSet().AddResource(ResourceName,pPostEffectSetR);


	if (IsAsyn && ms_EnableAsynLoad && pPostEffectSetR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		//SKASYNLoadManager::ms_pASYNLoadManager->AddSkelectonMesh(pPostEffectSetR,FileName);
	}
	else
	{	
		SKPostEffectSet * pPostEffectSet = NewLoadPostEffect(FileName.GetBuffer());
		if (pPostEffectSet)
		{
			pPostEffectSetR->SetNewResource(pPostEffectSet);
			pPostEffectSetR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);
		}
	}
	ms_PostEffectCri.Unlock();
	return pPostEffectSetR;
}
SKAnimR * SKResourceManager::LoadASYNAction(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_ACTION))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_AnimPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_ACTION);
		FileName =  ms_AnimPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_ACTION);

	}
	ms_AnimCri.Lock();
	SKAnimRPtr pAnimR = NULL;
	pAnimR = (SKAnimR *)SKResourceManager::GetASYNAnimSet().CheckIsHaveTheResource(ResourceName);
	if(pAnimR)
	{
		ms_AnimCri.Unlock();
		return pAnimR;
	}
	pAnimR = SK_NEW SKAnimR();

	pAnimR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNAnimSet().AddResource(ResourceName,pAnimR);


	if (IsAsyn && ms_EnableAsynLoad && pAnimR->IsEndableASYNLoad() )
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddAnim(pAnimR,FileName);
	}
	else
	{	
		SKAnim * pAnim = NewLoadAction(FileName.GetBuffer());
		if (pAnim)
		{
			pAnimR->SetNewResource(pAnim);
			pAnimR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);
		}
		
	}
	ms_AnimCri.Unlock();
	return pAnimR;
}
SKAnimTreeR * SKResourceManager::LoadASYNAnimTree(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_ANIMTREE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_AnimPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_ANIMTREE);
		FileName =  ms_AnimPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_ANIMTREE);

	}
	ms_AnimTreeCri.Lock();
	SKAnimTreeRPtr pAnimTreeR = NULL;
	pAnimTreeR = (SKAnimTreeR *)SKResourceManager::GetASYNAnimTreeSet().CheckIsHaveTheResource(ResourceName);
	if(pAnimTreeR)
	{
		ms_AnimTreeCri.Unlock();
		return pAnimTreeR;
	}
	pAnimTreeR = SK_NEW SKAnimTreeR();

	pAnimTreeR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNAnimTreeSet().AddResource(ResourceName,pAnimTreeR);


	if (IsAsyn && ms_EnableAsynLoad && pAnimTreeR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddAnimTree(pAnimTreeR,FileName);
	}
	else
	{	
		SKAnimTree * pAnimTree = NewLoadAnimTree(FileName.GetBuffer());
		if (pAnimTree)
		{
			pAnimTreeR->SetNewResource(pAnimTree);
			pAnimTreeR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}

	}
	ms_AnimTreeCri.Unlock();
	return pAnimTreeR;
}
void SKResourceManager::DeleteAllMapResource()
{
	ms_MapCri.Lock();
	for (unsigned int i = 0; i < SKResourceManager::GetASYNMapSet().GetResourceNum(); )
	{
		SKSceneMapRPtr pMapR = SKResourceManager::GetASYNMapSet().GetResource(i)->Value;
		if (pMapR->IsLoaded())
		{
			if (SKWorld::ms_pWorld)
			{
				SKWorld::ms_pWorld->DestroyScene(pMapR->GetResource()->m_Name);
			}
		}
		else
		{
			SKFileName FileName = ms_MapPath + pMapR->GetResourceName().GetString();
			SKASYNLoadManager::ms_pASYNLoadManager->DeleteLoadResource(FileName);
		}
		SKResourceManager::GetASYNMapSet().DeleteResource(pMapR->GetResourceName());
	}
	
	ms_MapCri.Unlock();
}
void SKResourceManager::DeleteMapResource(const TCHAR *  pFileName)
{
	SKFileName FileName = pFileName;
	SKString Extension;
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_MAP))
		{
			return;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_MapPath + FileName;
		}
	}
	else
	{
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MAP);
		FileName = ms_MapPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MAP);
	}

	ms_MapCri.Lock();
	SKSceneMapRPtr pMapR = (SKSceneMapR *)SKResourceManager::GetASYNMapSet().CheckIsHaveTheResource(ResourceName);
	if (pMapR == NULL)
	{
		return;
	}
	if (pMapR->IsLoaded())
	{
		if (SKWorld::ms_pWorld)
		{
			SKWorld::ms_pWorld->DestroyScene(pMapR->GetResource()->m_Name);
		}	
	}
	else
	{
		SKASYNLoadManager::ms_pASYNLoadManager->DeleteLoadResource(FileName);
	}	
	SKResourceManager::GetASYNMapSet().DeleteResource(ResourceName);
	ms_MapCri.Unlock();

}
SKFontR * SKResourceManager::LoadASYNFont(const TCHAR *pFileName, bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension;
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_FONTTYPE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_FontPath + FileName;

		}
	}
	else
	{
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_FONTTYPE);
		FileName = ms_FontPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_FONTTYPE);

	}
	ms_FontCri.Lock();
	SKFontRPtr pFontR = NULL;
	pFontR = (SKFontR *)SKResourceManager::GetASYNFontSet().CheckIsHaveTheResource(ResourceName);
	if (pFontR)
	{
		ms_FontCri.Unlock();
		//Have Map Return NULL;
		return NULL;
	}
	pFontR = SK_NEW SKFontR();

	pFontR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNFontSet().AddResource(ResourceName, pFontR);


	if (IsAsyn && ms_EnableAsynLoad && pFontR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddFont(pFontR, FileName);
	}
	else
	{
		SKFont * PFont = NewLoadFont(FileName.GetBuffer());
		if (PFont)
		{
			pFontR->SetNewResource(PFont);
			pFontR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}

	}
	ms_FontCri.Unlock();
	return pFontR;
}
SKSceneMapR * SKResourceManager::LoadASYNMap(const TCHAR *pFileName, bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension;
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_MAP))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_MapPath + FileName;

		}
	}
	else
	{
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MAP);
		FileName = ms_MapPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MAP);

	}
	ms_MapCri.Lock();
	SKSceneMapRPtr pMapR = NULL;
	pMapR = (SKSceneMapR *)SKResourceManager::GetASYNMapSet().CheckIsHaveTheResource(ResourceName);
	if (pMapR)
	{
		ms_MapCri.Unlock();
		//Have Map Return NULL;
		return NULL;
	}
	pMapR = SK_NEW SKSceneMapR();

	pMapR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNMapSet().AddResource(ResourceName, pMapR);


	if (IsAsyn && ms_EnableAsynLoad && pMapR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddMap(pMapR, FileName);
	}
	else
	{
		SKSceneMap * pMap = NewLoadMap(FileName.GetBuffer());
		if (pMap)
		{
			pMapR->SetNewResource(pMap);
			pMapR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}

	}
	ms_MapCri.Unlock();
	return pMapR;
}
SKActorR * SKResourceManager::LoadASYNActor(const TCHAR *pFileName, bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension;
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_ACTOR))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_ActorPath + FileName;

		}
	}
	else
	{
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_ACTOR);
		FileName = ms_ActorPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_ACTOR);

	}
	ms_ActorCri.Lock();
	SKActorRPtr pActorR = NULL;
	pActorR = (SKActorR *)SKResourceManager::GetASYNActorSet().CheckIsHaveTheResource(ResourceName);
	if (pActorR)
	{
		ms_ActorCri.Unlock();
		return pActorR;
	}
	pActorR = SK_NEW SKActorR();

	pActorR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNActorSet().AddResource(ResourceName, pActorR);


	if (IsAsyn && ms_EnableAsynLoad && pActorR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddActor(pActorR, FileName);
	}
	else
	{
		SKActor * pActor = NewLoadActor(FileName.GetBuffer());
		if (pActor)
		{
			pActorR->SetNewResource(pActor);
			pActorR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}

	}
	ms_ActorCri.Unlock();
	return pActorR;
}
SKMorphTreeR * SKResourceManager::LoadASYNMorphTree(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_MORPHTREE))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName = ms_MorphPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MORPHTREE);
		FileName = ms_MorphPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MORPHTREE);

	}
	ms_MorphTreeCri.Lock();
	SKMorphTreeRPtr pMorphTreeR = NULL;
	pMorphTreeR = (SKMorphTreeR *)SKResourceManager::GetASYNMorphTreeSet().CheckIsHaveTheResource(ResourceName);
	if(pMorphTreeR)
	{
		ms_MorphTreeCri.Unlock();
		return pMorphTreeR;
	}
	pMorphTreeR = SK_NEW SKMorphTreeR();

	pMorphTreeR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNMorphTreeSet().AddResource(ResourceName,pMorphTreeR);


	if (IsAsyn && ms_EnableAsynLoad && pMorphTreeR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddMorphTree(pMorphTreeR,FileName);
	}
	else
	{	
		SKMorphTree * pMorphTree = NewLoadMorphTree(FileName.GetBuffer());
		if (pMorphTree)
		{
			pMorphTreeR->SetNewResource(pMorphTree);
			pMorphTreeR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}

	}
	ms_MorphTreeCri.Unlock();
	return pMorphTreeR;
}
SKMaterialR * SKResourceManager::LoadASYNMaterial(const TCHAR *pFileName,bool IsAsyn)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKFileName FileName = pFileName;
	SKString Extension; 
	SKUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != SKResource::GetFileSuffix(SKResource::RT_MATERIAL))
		{
			return NULL;
		}
		else
		{
			ResourceName = FileName;
			FileName =  ms_MaterialPath + FileName;

		}
	}
	else
	{		
		ResourceName = FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MATERIAL);
		FileName =  ms_MaterialPath + FileName + _T(".") + SKResource::GetFileSuffix(SKResource::RT_MATERIAL);

	}
	ms_MaterialCri.Lock();
	SKMaterialRPtr pMaterialR = NULL;
	pMaterialR = (SKMaterialR *)SKResourceManager::GetASYNMaterialSet().CheckIsHaveTheResource(ResourceName);
	if(pMaterialR)
	{
		ms_MaterialCri.Unlock();
		return pMaterialR;
	}
	pMaterialR = SK_NEW SKMaterialR();

	pMaterialR->SetResourceName(ResourceName);


	SKResourceManager::GetASYNMaterialSet().AddResource(ResourceName,pMaterialR);


	if (IsAsyn && ms_EnableAsynLoad && pMaterialR->IsEndableASYNLoad())
	{
		SKMAC_ASSERT(SKASYNLoadManager::ms_pASYNLoadManager);
		SKASYNLoadManager::ms_pASYNLoadManager->AddMaterial(pMaterialR,FileName);
	}
	else
	{	
		SKMaterial * pMaterial = NewLoadMaterial(FileName.GetBuffer());
		if (pMaterial)
		{
			pMaterialR->SetNewResource(pMaterial);
			pMaterialR->Loaded();
		}
		else
		{
			SKMAC_ASSERT(0);

		}
		
	}
	ms_MaterialCri.Unlock();
	return pMaterialR;
}
SKTexAllState * SKResourceManager::NewLoadTexture(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKTexAllState * pTexAllState = (SKTexAllState *)LoadStream.GetObjectByRtti(SKTexAllState::ms_Type);
	if (!pTexAllState)
	{		
		return NULL;
	}
	if(!pTexAllState->m_pTex)
	{
		SKMAC_DELETE(pTexAllState);
		
		return NULL;
	}
	return pTexAllState;

}

bool SKResourceManager::NewSaveTexture(SKTexAllState * pTexture, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pTexture || !pTexture->m_pTex || !pTexture->m_pTex->IsStatic())
	{
		return false;
	}

	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pTexture);
	SKString FileName = ms_TexturePath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_TEXTURE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}

SKMaterial * SKResourceManager::NewLoadMaterial(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKMaterial * pMaterial = (SKMaterial *)LoadStream.GetObjectByRtti(SKMaterial::ms_Type);
	if (!pMaterial)
	{
		return NULL;
	}
	return pMaterial;
}
bool SKResourceManager::NewSaveMaterial(SKMaterial * pMaterial, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pMaterial)
	{
		return false;
	}

	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pMaterial);
	SKString FileName = ms_MaterialPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_MATERIAL);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}

SKStaticMeshNode * SKResourceManager::NewLoadStaticMesh(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKStaticMeshNode * pStaticMesh = (SKStaticMeshNode *)LoadStream.GetObjectByRtti(SKStaticMeshNode::ms_Type);
	if (!pStaticMesh)
	{
		return NULL;
	}
	return pStaticMesh;
}
bool SKResourceManager::NewSaveStaticMesh(SKStaticMeshNode * pStaticMeshNode, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pStaticMeshNode)
	{
		return false;
	}

	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pStaticMeshNode);
	SKString FileName = ms_StaticMeshPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_STATIC_MODEL);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
SKPostEffectSet * SKResourceManager::NewLoadPostEffect(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}
	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKPostEffectSet * pPostEffectSet = (SKPostEffectSet *)LoadStream.GetObjectByRtti(SKPostEffectSet::ms_Type);
	if (!pPostEffectSet)
	{
		return NULL;
	}
	return pPostEffectSet;
}
bool SKResourceManager::NewSaveAnimTree(SKAnimTree * pAnim, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pAnim)
	{
		return false;
	}
	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pAnim);
	SKString FileName = ms_AnimPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_ANIMTREE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
SKAnimTree * SKResourceManager::NewLoadAnimTree(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKAnimTree * pAnim = (SKAnimTree *)LoadStream.GetObjectByRtti(SKAnimTree::ms_Type);
	if (!pAnim)
	{
		return NULL;
	}
	return pAnim;
}
bool SKResourceManager::NewSaveActor(SKActor * pActor, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pActor)
	{
		return false;
	}
	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pActor);
	SKString FileName = ms_ActorPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_ACTOR);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
SKActor * SKResourceManager::NewLoadActor(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKActor * pActor = (SKActor *)LoadStream.GetObjectByRtti(SKActor::ms_Type);
	if (!pActor)
	{
		return NULL;
	}
	return pActor;
}
bool SKResourceManager::NewSaveMap(SKSceneMap * pMap, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pMap)
	{
		return false;
	}
	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pMap);
	SKString FileName = ms_MapPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_MAP);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
SKSceneMap * SKResourceManager::NewLoadMap(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKSceneMap * pMap = (SKSceneMap *)LoadStream.GetObjectByRtti(SKSceneMap::ms_Type);
	if (!pMap)
	{
		return NULL;
	}
	return pMap;
}
bool SKResourceManager::NewSaveFont(SKFont * pFont, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pFont)
	{
		return false;
	}
	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pFont);
	SKString FileName = ms_FontPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_FONTTYPE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
SKFont * SKResourceManager::NewLoadFont(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKFont * pFont = (SKFont *)LoadStream.GetObjectByRtti(SKFont::ms_Type);
	if (!pFont)
	{
		return NULL;
	}
	return pFont;
}
bool SKResourceManager::NewSaveMorphTree(SKMorphTree * pMorphTree, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pMorphTree)
	{
		return false;
	}
	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pMorphTree);
	SKString FileName = ms_MorphPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_MORPHTREE);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
SKMorphTree * SKResourceManager::NewLoadMorphTree(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKMorphTree * pMorphTree = (SKMorphTree *)LoadStream.GetObjectByRtti(SKMorphTree::ms_Type);
	if (!pMorphTree)
	{
		return NULL;
	}
	return pMorphTree;
}
SKAnim * SKResourceManager::NewLoadAction(const TCHAR *pFileName)
{
	if (!pFileName)
	{
		return NULL;
	}

	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKAnim * pAnim = (SKAnim *)LoadStream.GetObjectByRtti(SKAnim::ms_Type);
	if (!pAnim)
	{
		return NULL;
	}
	return pAnim;
}
bool SKResourceManager::NewSaveAction(SKAnim * pAnim, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pAnim)
	{
		return false;
	}
	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pAnim);
 	SKString FileName = ms_AnimPath + PathName;
 	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_ACTION);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
bool SKResourceManager::NewSavePostEffect(SKPostEffectSet * pPostEffectSet, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pPostEffectSet)
	{
		return false;
	}

	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pPostEffectSet);
	SKString FileName = ms_PostEffectPath + PathName;
	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_POSTEFFECT);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}
SKSkelectonMeshNode * SKResourceManager::NewLoadSkelectonMesh(const TCHAR *pFileName)
{
	
	if (!pFileName)
	{
		
		return NULL;
	}
	SKStream LoadStream;
	LoadStream.NewLoad(pFileName);
	SKSkelectonMeshNode * pSkelecton = (SKSkelectonMeshNode *)LoadStream.GetObjectByRtti(SKSkelectonMeshNode::ms_Type);
	if (!pSkelecton)
	{		
		return NULL;
	}
	return pSkelecton;
}
bool SKResourceManager::NewSaveSkelectonMeshNode(SKSkelectonMeshNode * pSkelectonMesh, const TCHAR * PathName, bool bSaveResourcePath)
{
	if (!pSkelectonMesh)
	{
		return false;
	}

	SKStream SaveStream;
	SaveStream.SetStreamFlag(SKStream::AT_REGISTER);
	SaveStream.ArchiveAll(pSkelectonMesh);
 	SKString FileName = ms_SkelectionMeshPath + PathName;
 	FileName += _T(".") + SKResource::GetFileSuffix(SKResource::RT_SKELECTON_MODEL);
	return SaveStream.NewSave(bSaveResourcePath ? FileName.GetBuffer() : PathName);
}

SKVertexFormat *SKResourceManager::LoadVertexFormat(SKVertexBuffer * pVertexBuffer,SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> *pFormatArray)
{
	if(!pVertexBuffer && !pFormatArray)
		return NULL;
	if (pVertexBuffer)
	{
		if(pVertexBuffer->m_pVertexFormat)
			return pVertexBuffer->m_pVertexFormat;
	}
	
	SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> FormatArray;
	if (!pFormatArray)
	{	
		if(!pVertexBuffer->GetVertexFormat(FormatArray))
			return	NULL;
		pFormatArray = &FormatArray;
	}
	else
	{
		if (!pFormatArray->GetNum())
		{
			return	NULL;
		}
	}

	unsigned int lVertexFormatCode = CRC32Compute(pFormatArray->GetBuffer(), sizeof(SKVertexFormat::VERTEXFORMAT_TYPE) * pFormatArray->GetNum());
	
	SKVertexFormat * pVertexFormat = NULL;
	pVertexFormat = (SKVertexFormat *)SKResourceManager::GetVertexFormatSet().CheckIsHaveTheResource(lVertexFormatCode);
	if(pVertexFormat)
	{
		if (pVertexBuffer)
		{
			pVertexBuffer->m_pVertexFormat = pVertexFormat;
		}
		return pVertexFormat;
	}
	pVertexFormat = SK_NEW SKVertexFormat();

	SKResourceManager::GetVertexFormatSet().AddResource(lVertexFormatCode,pVertexFormat);
	if (pVertexBuffer)
	{
		pVertexBuffer->m_pVertexFormat = pVertexFormat;
	}
	pVertexFormat->m_FormatArray = *pFormatArray;
	pVertexFormat->m_uiVertexFormatCode = lVertexFormatCode;
	pVertexFormat->LoadResource(SKRenderer::ms_pRenderer);
	return pVertexFormat;

}
SKName * SKResourceManager::CreateName(const TCHAR * pChar)
{
	if (!pChar)
	{
		return NULL;
	}
	ms_NameCri.Lock();
	unsigned int uiCRCCode  = CRC32Compute(pChar,(unsigned int)SKStrLen(pChar));
	SKName * pName = NULL;
	pName = SKResourceManager::GetNameSet().CheckIsHaveTheResource(uiCRCCode);
	if (!pName)
	{
		pName = SK_NEW SKName(pChar,uiCRCCode);
		if (!pName)
		{
			ms_NameCri.Unlock();
			return NULL;
		}

		SKResourceManager::GetNameSet().AddResource(pName->m_uiID,pName);
	}
	ms_NameCri.Unlock();
	return pName;
}
SKName * SKResourceManager::CreateName(const SKString & String)
{

	ms_NameCri.Lock();
	unsigned int uiCRCCode  = CRC32Compute(String.GetBuffer(),String.GetLength());
	SKName * pName = NULL;
	pName = SKResourceManager::GetNameSet().CheckIsHaveTheResource(uiCRCCode);
	if (!pName)
	{
		pName = SK_NEW SKName(String,uiCRCCode);
		if (!pName)
		{
			ms_NameCri.Unlock();
			return NULL;
		}

		SKResourceManager::GetNameSet().AddResource(pName->m_uiID,pName);
	}
	ms_NameCri.Unlock();
	return pName;
}
SKVShader * SKResourceManager::CreateVShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID)
{
	ms_VShaderCri.Lock();
	if (!MSPara.pGeometry || !MSPara.pMaterialInstance)
	{
		ms_VShaderCri.Unlock();
		return NULL;
	}
	
	bool bCreateShaderString = false;
	SKVShader * pVShader = NULL;
	if (MSPara.m_VSShaderPath.GetLength())
	{
		if (!MSPara.m_VMainFunName.GetLength())
		{
			SKMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pVShader = SK_NEW SKVShader(MSPara.m_VSShaderPath.GetBuffer(),MSPara.m_VMainFunName.GetBuffer(),true);
		}
	}
	else
	{	
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			ms_VShaderCri.Unlock();
			return NULL;
		}
		pVShader = SK_NEW SKVShader();
		bCreateShaderString = true;
	}

	if(!pVShader)
	{
		ms_VShaderCri.Unlock();
		return NULL;
	}
	if (bCreateShaderString)
	{
		SKString VShaderString;
	#ifdef _DEBUG
		SKString ShaderFileName;
		if(!SKShaderStringFactory::CreateVShaderString(pVShader,MSPara,uiPassType,uiShaderID,VShaderString,ShaderFileName))
	#else
		if(!SKShaderStringFactory::CreateVShaderString(pVShader,MSPara,uiPassType,uiShaderID,VShaderString))
	#endif
		{
			ms_VShaderCri.Unlock();
			SKMAC_DELETE(pVShader);
			return NULL;
		}
	#ifdef _DEBUG
		pVShader->SetResourceName(ShaderFileName);
	#endif
		pVShader->SetShaderString(VShaderString,SKRenderer::GetVShaderProgramMain());
	}
	SKShaderKey::SetMaterialVShaderKey(&pVShader->m_ShaderKey,MSPara,uiPassType);
	SKRenderer::ms_pRenderer->LoadVShaderProgram(pVShader);
	ms_VShaderCri.Unlock();
	return pVShader;
}
SKGShader * SKResourceManager::CreateGShader(MaterialShaderPara &MSPara, unsigned int uiPassType, unsigned int uiShaderID)
{
	ms_GShaderCri.Lock();
	if (!MSPara.pGeometry || !MSPara.pMaterialInstance)
	{
		ms_GShaderCri.Unlock();
		return NULL;
	}

	bool bCreateShaderString = false;
	SKGShader * pGShader = NULL;
	if (MSPara.m_GSShaderPath.GetLength())
	{
		if (!MSPara.m_GMainFunName.GetLength())
		{
			SKMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pGShader = SK_NEW SKGShader(MSPara.m_GSShaderPath.GetBuffer(), MSPara.m_GMainFunName.GetBuffer(), true);
		}
	}
	else
	{
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			ms_GShaderCri.Unlock();
			return NULL;
		}
		pGShader = SK_NEW SKGShader();
		bCreateShaderString = true;
	}

	if (!pGShader)
	{
		ms_GShaderCri.Unlock();
		return NULL;
	}

	if (bCreateShaderString)
	{
		SKString GShaderString;
#ifdef _DEBUG
		SKString ShaderFileName;
		if (!SKShaderStringFactory::CreateGShaderString(pGShader, MSPara, uiPassType, uiShaderID, GShaderString, ShaderFileName))
#else
		if (!SKShaderStringFactory::CreateGShaderString(pGShader, MSPara, uiPassType, uiShaderID, GShaderString))
#endif
		{
			ms_GShaderCri.Unlock();
			SKMAC_DELETE(pGShader);
			return NULL;
		}
#ifdef _DEBUG
		pGShader->SetResourceName(ShaderFileName);
#endif
		pGShader->SetShaderString(GShaderString, SKRenderer::GetGShaderProgramMain());
	}


	SKShaderKey::SetMaterialGShaderKey(&pGShader->m_ShaderKey, MSPara, uiPassType);
	SKRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
	ms_GShaderCri.Unlock();
	return pGShader;
}
SKPShader * SKResourceManager::CreatePShader(MaterialShaderPara &MSPara,unsigned int uiPassType,unsigned int uiShaderID)
{
	ms_PShaderCri.Lock();
	if (!MSPara.pGeometry || !MSPara.pMaterialInstance)
	{
		ms_PShaderCri.Unlock();
		return NULL;
	}
	
	bool bCreateShaderString = false;
	SKPShader * pPShader = NULL;
	if (MSPara.m_PSShaderPath.GetLength())
	{
		if (!MSPara.m_PMainFunName.GetLength())
		{
			SKMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pPShader = SK_NEW SKPShader(MSPara.m_PSShaderPath.GetBuffer(),MSPara.m_PMainFunName.GetBuffer(),true);
		}
	}
	else
	{
		SKMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			ms_PShaderCri.Unlock();
			return NULL;
		}
		pPShader = SK_NEW SKPShader();
		bCreateShaderString = true;
	}
	
	if(!pPShader)
	{
		ms_PShaderCri.Unlock();
		return NULL;
	}

	if (bCreateShaderString)
	{
		SKString PShaderString;
#ifdef _DEBUG
		SKString ShaderFileName;
		if (!SKShaderStringFactory::CreatePShaderString(pPShader,MSPara,uiPassType,uiShaderID,PShaderString,ShaderFileName))
#else
		if (!SKShaderStringFactory::CreatePShaderString(pPShader,MSPara,uiPassType,uiShaderID,PShaderString))
#endif
		{
			ms_PShaderCri.Unlock();
			SKMAC_DELETE(pPShader);
			return NULL;
		}
#ifdef _DEBUG
		pPShader->SetResourceName(ShaderFileName);
#endif
		pPShader->SetShaderString(PShaderString,SKRenderer::GetPShaderProgramMain());
	}
	
	
	SKShaderKey::SetMaterialPShaderKey(&pPShader->m_ShaderKey,MSPara,uiPassType);
	SKRenderer::ms_pRenderer->LoadPShaderProgram(pPShader);
	ms_PShaderCri.Unlock();
	return pPShader;
}


SKBlendState * SKResourceManager::CreateBlendState(const SKBlendDesc & BlendDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = BlendDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	SKBlendState * pBlendState = NULL;
	pBlendState = (SKBlendState *)SKResourceManager::GetBlendStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pBlendState)
	{

		return pBlendState;
	}
	pBlendState = SK_NEW SKBlendState();
	pBlendState->m_BlendDesc = BlendDesc;
	SKResourceManager::GetBlendStateSet().AddResource(uiHashCode,pBlendState);
	return pBlendState;
}
SKDepthStencilState * SKResourceManager::CreateDepthStencilState(const SKDepthStencilDesc & DepthStencilDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = DepthStencilDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	SKDepthStencilState * pDepthStencilState = NULL;
	pDepthStencilState = (SKDepthStencilState *)SKResourceManager::GetDepthStencilStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pDepthStencilState)
	{

		return pDepthStencilState;
	}
	pDepthStencilState = SK_NEW SKDepthStencilState();
	pDepthStencilState->m_DepthStencilDesc = DepthStencilDesc;
	SKResourceManager::GetDepthStencilStateSet().AddResource(uiHashCode,pDepthStencilState);
	return pDepthStencilState;
}
SKRasterizerState * SKResourceManager::CreateRasterizerState(const SKRasterizerDesc &RasterizerDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = RasterizerDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	SKRasterizerState * pRasterizerState = NULL;
	pRasterizerState = (SKRasterizerState *)SKResourceManager::GetRasterizerStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pRasterizerState)
	{

		return pRasterizerState;
	}
	pRasterizerState = SK_NEW SKRasterizerState();
	pRasterizerState->m_RasterizerDesc = RasterizerDesc;
	SKResourceManager::GetRasterizerStateSet().AddResource(uiHashCode,pRasterizerState);
	return pRasterizerState;
}

SKSamplerState * SKResourceManager::CreateSamplerState(const SKSamplerDesc &SamplerDesc)
{
	unsigned int uiDataSize = 0;
	void * pData = SamplerDesc.GetCRC32Data(uiDataSize);
	unsigned int uiHashCode = CRC32Compute(pData, uiDataSize);

	SKSamplerState * pSamplerState = NULL;
	pSamplerState = (SKSamplerState *)SKResourceManager::GetSamplerStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pSamplerState)
	{

		return pSamplerState;
	}
	pSamplerState = SK_NEW SKSamplerState();
	pSamplerState->m_SamplerDesc = SamplerDesc;
	SKResourceManager::GetSamplerStateSet().AddResource(uiHashCode,pSamplerState);
	return pSamplerState;
}
SKRenderTarget * SKResourceManager::CreateRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,
									unsigned int uiMulSample)
{
	SKRenderTarget *Temp = SK_NEW SKRenderTarget(uiWidth,uiHeight,uiFormatType,uiMulSample);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
SKRenderTarget * SKResourceManager::CreateRenderTarget(SKTexture * pCreateBy,unsigned int uiMulSample
									,unsigned int uiLevel,unsigned int uiFace)
{
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	SKMAC_ASSERT(uiMulSample < SKRenderer::MS_MAX)
	if (uiMulSample >= SKRenderer::MS_MAX)
	{
		return NULL;
	}
	SKMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if (uiFace >= SKCubeTexture::F_MAX)
	{
		return NULL;
	}
	SKRenderTarget *Temp = SK_NEW SKRenderTarget(pCreateBy,uiMulSample,uiLevel,uiFace);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
SKDepthStencil * SKResourceManager::CreateDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
									unsigned int uiMulSample,unsigned int uiFormatType)
{
	SKDepthStencil *Temp = SK_NEW SKDepthStencil(uiWidth,uiHeight,uiMulSample,uiFormatType);
	GetDepthStencilArray().AddResource(Temp);
	return Temp;
}
SKRenderTarget * SKResourceManager::Get2DRenderTarget(unsigned int uiWidth, unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample)
{
	SKResourceArrayControll<SKRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	for (unsigned int i = 0 ; i < RenderTargetArray.GetResourceNum() ; i++)
	{
		SKRenderTargetPtr pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && pRt->GetCreateBy()->GetTexType() == SKTexture::TT_2D)
		{
			VS2DTexture * p2DTex = (VS2DTexture *)pRt->GetCreateBy();
			if (p2DTex->HasAnySlot())
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth && pRt->m_uiHeight == uiHeight && pRt->m_uiFormatType == uiFormatType && pRt->m_uiMulSample == uiMulSample)
			{
				pRt->m_bUsed = true;
				RenderTargetArray.ClearTimeCount(i);
				return pRt;
			}
		}
	}
	VS2DTexture * pTexture = SK_NEW VS2DTexture(uiWidth,uiHeight,uiFormatType);

	SKMAC_ASSERT(uiMulSample < SKRenderer::MS_MAX)
	if (uiMulSample >= SKRenderer::MS_MAX)
	{
		return NULL;
	}

	SKRenderTarget *  pNewRt = SK_NEW SKRenderTarget(pTexture,uiMulSample);
	GetRenderTargetBufferArray().AddResource(pNewRt);

	pNewRt->m_bUsed = true;
	return pNewRt;

}
void SKResourceManager::Release2DRenderTarget(SKRenderTarget * pRenderTarget)
{
	if (pRenderTarget)
	{
		SKMAC_ASSERT(pRenderTarget->m_bUsed == true);
		pRenderTarget->m_bUsed = false;
	}
}
void SKResourceManager::Disable2DRenderTarget(SKRenderTarget * &pRenderTarget)
{
	Release2DRenderTarget(pRenderTarget);
	VS2DTexture * p2DTexture = DynamicCast<VS2DTexture>(pRenderTarget->GetCreateBy());
	SKMAC_ASSERT(p2DTexture);
	p2DTexture->ClearAllSlot();
	pRenderTarget = NULL;
}
void SKResourceManager::Disable2DRenderTarget(SKRenderTargetPtr &pRenderTarget)
{
	Release2DRenderTarget(pRenderTarget);
	VS2DTexture * p2DTexture = DynamicCast<VS2DTexture>(pRenderTarget->GetCreateBy());
	SKMAC_ASSERT(p2DTexture);
	p2DTexture->ClearAllSlot();
	pRenderTarget = NULL;
}
void SKResourceManager::DisableCubRenderTarget(SKRenderTarget * RT[SKCubeTexture::F_MAX])
{
	ReleaseCubRenderTarget(RT);
	SKCubeTexture * pCubTexture = DynamicCast<SKCubeTexture>(RT[0]->GetCreateBy());
	SKMAC_ASSERT(pCubTexture);
	pCubTexture->ClearAllSlot();
	for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
	{
		RT[i] = NULL;
	}
}
void SKResourceManager::ReleaseCubRenderTarget(SKRenderTarget * RT[SKCubeTexture::F_MAX])
{
	unsigned int uiUseNum = 0;
	unsigned int uiNullNum = 0;
	for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
	{
		if (RT[i])
		{
			uiNullNum++;
			if (RT[i]->m_bUsed == true)
			{
				uiUseNum++;
			}
			RT[i]->m_bUsed = false;
		}
	}
	SKMAC_ASSERT(uiNullNum == SKCubeTexture::F_MAX);
	SKMAC_ASSERT(uiUseNum == SKCubeTexture::F_MAX);
}
void SKResourceManager::ReleaseDepthStencil(SKDepthStencil * pDepthStencil)
{
	if (pDepthStencil)
	{
		SKMAC_ASSERT(pDepthStencil->m_bUsed == true);
		pDepthStencil->m_bUsed = false;
	}
}
void SKResourceManager::DisableDepthStencil(SKDepthStencilPtr &pDepthStencil)
{
	ReleaseDepthStencil(pDepthStencil);
	pDepthStencil = NULL;
}

void SKResourceManager::DisableDepthStencil(SKDepthStencil * &pDepthStencil)
{
	ReleaseDepthStencil(pDepthStencil);
	pDepthStencil = NULL;
}
bool SKResourceManager::GetCubRenderTarget(unsigned int uiWidth,unsigned int uiFormatType,unsigned int uiMulSample,SKRenderTarget * OutRT[SKCubeTexture::F_MAX])
{
	
	SKResourceArrayControll<SKRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	SKCubeTexture * IndexCubTexture = NULL;
	for(unsigned int k = 0 ; k < SKCubeTexture::F_MAX ; k++)
	{
		OutRT[k] = NULL;
	}
	for (unsigned int i = 0 ; i < RenderTargetArray.GetResourceNum() ; i++)
	{
		SKRenderTargetPtr pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && pRt->GetCreateBy()->GetTexType() == SKTexture::TT_CUBE)
		{
			SKCubeTexture * pCubDTex = (SKCubeTexture *)pRt->GetCreateBy();
			if (pCubDTex->HasAnySlot())
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth  && pRt->m_uiFormatType == uiFormatType && pRt->m_uiMulSample == uiMulSample)
			{
				SKMAC_ASSERT(pRt->m_uiParam < SKCubeTexture::F_MAX);

				if (!IndexCubTexture)
				{
					IndexCubTexture = pCubDTex;
					if (!OutRT[pRt->m_uiParam])
					{
						OutRT[pRt->m_uiParam] = pRt;
						RenderTargetArray.ClearTimeCount(i);
					}
					else
					{
						for(unsigned int k = 0 ; k < SKCubeTexture::F_MAX ; k++)
						{
							OutRT[k] = NULL;
						}
						return false;
					}
					
				}
				else
				{
					if (IndexCubTexture == pCubDTex)
					{
						if (!OutRT[pRt->m_uiParam])
						{
							RenderTargetArray.ClearTimeCount(i);
							OutRT[pRt->m_uiParam] = pRt;
						}
						else
						{
							for(unsigned int k = 0 ; k < SKCubeTexture::F_MAX ; k++)
							{
								OutRT[k] = NULL;
							}
							return false;
						}
					}
				}
				
				
			}
		}
	}
	unsigned int uiFindNum = 0;
	for(unsigned int k = 0 ; k < SKCubeTexture::F_MAX ; k++)
	{
		if (OutRT[k])
		{
			uiFindNum++;
		}
	}

	if (uiFindNum == 0)
	{
		SKCubeTexture *pCubTexture = SK_NEW SKCubeTexture(uiWidth,uiFormatType);
		for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
		{
			SKMAC_ASSERT(uiMulSample < SKRenderer::MS_MAX)
			if (uiMulSample >= SKRenderer::MS_MAX)
			{
				return NULL;
			}

			OutRT[i] = SK_NEW SKRenderTarget(pCubTexture,uiMulSample,0,i);
			RenderTargetArray.AddResource(OutRT[i]);
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
	else if (uiFindNum == SKCubeTexture::F_MAX)
	{
		for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
		{
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
	else // GC will delete some ,so no longer  use any rt
	{
		//set other rt used , so you cat't find it
		for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
		{
			if (OutRT[i])
			{
				OutRT[i]->m_bUsed = true;
			}	
		}

		// create new one
		SKCubeTexture *pCubTexture = SK_NEW SKCubeTexture(uiWidth,uiFormatType);
		for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
		{
			SKMAC_ASSERT(uiMulSample < SKRenderer::MS_MAX)
			if (uiMulSample >= SKRenderer::MS_MAX)
			{
				return NULL;
			}

			OutRT[i] = SK_NEW SKRenderTarget(pCubTexture,uiMulSample,0,i);
			RenderTargetArray.AddResource(OutRT[i]);
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
}

SKDepthStencil * SKResourceManager::GetDepthStencil(unsigned int uiWidth,unsigned int uiHeight,unsigned int uiFormatType,unsigned int uiMulSample)
{
	SKResourceArrayControll<SKDepthStencilPtr> & DepthStencilArray = GetDepthStencilBufferArray();
	for (unsigned int i = 0 ; i < DepthStencilArray.GetResourceNum() ; i++)
	{
		SKDepthStencilPtr pDS = DepthStencilArray.GetResource(i);
		if (pDS->m_bUsed)
		{
			continue;;
		}
		if (pDS->m_uiWidth == uiWidth && pDS->m_uiHeight == uiHeight && pDS->m_uiFormatType == uiFormatType && pDS->m_uiMulSample == uiMulSample)
		{
			pDS->m_bUsed = true;
			DepthStencilArray.ClearTimeCount(i);
			return pDS;
		}

	}

	SKDepthStencil* pNewDS= SK_NEW SKDepthStencil(uiWidth,uiHeight,uiMulSample,uiFormatType);
	GetDepthStencilBufferArray().AddResource(pNewDS);
	pNewDS->m_bUsed = true;
	return pNewDS;
}
SKAnim * SKResourceManager::CreateAdditiveAnim(SKAnim * pSourceAnim, SKAnim * pTargetAnim, SKAnimR * pBlendAnimR)
{
	if (!pSourceAnim || !pTargetAnim || !pBlendAnimR)
	{
		return NULL;
	}

	while (!pBlendAnimR->IsLoaded())
	{

	}
	SKAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}



	SKAnim * pAddAnim = SK_NEW SKAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pBlendAnim->IsRootAnim());


	SKREAL fSourceTimeScale = pSourceAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	SKREAL fTargetTimeScale = pTargetAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	for (unsigned int i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		SKBoneKey * pBlendBoneKey = pSourceAnim->GetBoneKey(i);
		
		if (!pBlendBoneKey)
		{
			continue;
		}
		SKBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		SKBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		SKBoneKey * pAddBoneKey = SK_NEW SKBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;
		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = SKVector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = SKVector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = SKQuat();
			return false;
		}
		else
		{
			

			pAddBoneKey->m_TranslationArray.SetBufferNum(pBlendBoneKey->m_TranslationArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_TranslationArray.GetNum(); j++)
			{
				SKREAL fSourceTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fSourceTimeScale;
				SKREAL fTargetTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_TranslationArray[j].m_dKeyTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime;
				SKVector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
				SKVector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_TranslationArray[j].m_Vector = SourceTranslation - TargetTranslation;
					

			}

			pAddBoneKey->m_ScaleArray.SetBufferNum(pBlendBoneKey->m_ScaleArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_ScaleArray.GetNum(); j++)
			{
				SKREAL fSourceTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fSourceTimeScale;
				SKREAL fTargetTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_ScaleArray[j].m_dKeyTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime;

				SKVector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);
				SKVector3 TargetScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fTargetTime);
				SKMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);
				SKMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
				pAddBoneKey->m_ScaleArray[j].m_Vector = SourceScale / TargetScale;


			}


			pAddBoneKey->m_RotatorArray.SetBufferNum(pBlendBoneKey->m_RotatorArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_RotatorArray.GetNum();j++)
			{
				SKREAL fSourceTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fSourceTimeScale;
				SKREAL fTargetTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_RotatorArray[j].m_dKeyTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime;
				SKQuat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
				SKQuat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_RotatorArray[j].m_Quat = SourceRoator * TargetRoator;

			}
		}
		

		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	
	return pAddAnim;
}

SKAnim * SKResourceManager::CreateAdditiveAnim(SKAnim * pSourceAnim, SKAnim * pTargetAnim, SKAnimR * pBlendAnimR,SKREAL fTargetTime)
{

	if (!pSourceAnim || !pTargetAnim)
	{
		return NULL;
	}


	while (!pBlendAnimR->IsLoaded())
	{

	}
	SKAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}

	SKAnim * pAddAnim = SK_NEW SKAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pBlendAnim->IsRootAnim());

	SKREAL fSourceTimeScale = pSourceAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	for (unsigned int i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		SKBoneKey * pBlendBoneKey = pBlendAnim->GetBoneKey(i);
		if (!pBlendBoneKey)
		{
			continue;
		}
		SKBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		SKBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		SKBoneKey * pAddBoneKey = SK_NEW SKBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;

		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = SKVector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = SKVector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = SKQuat();
			return false;
		}
		else
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(pBlendBoneKey->m_TranslationArray.GetNum());
			SKVector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
			for (unsigned int j = 0; j < pBlendBoneKey->m_TranslationArray.GetNum(); j++)
			{
				SKREAL fSourceTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fSourceTimeScale;

				pAddBoneKey->m_TranslationArray[j].m_dKeyTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime;
				SKVector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
				pAddBoneKey->m_TranslationArray[j].m_Vector = SourceTranslation - TargetTranslation;


			}

			pAddBoneKey->m_ScaleArray.SetBufferNum(pBlendBoneKey->m_ScaleArray.GetNum());
			SKVector3 TargetScale = pTargetAnim->GetScale(pSourceBoneKey->m_cName, fTargetTime);
			SKMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
			for (unsigned int j = 0; j < pBlendBoneKey->m_ScaleArray.GetNum(); j++)
			{
				SKREAL fSourceTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fSourceTimeScale;


				pAddBoneKey->m_ScaleArray[j].m_dKeyTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime;

				SKVector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);

				SKMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);

				pAddBoneKey->m_ScaleArray[j].m_Vector = SourceScale / TargetScale;


			}

			SKQuat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
			pAddBoneKey->m_RotatorArray.SetBufferNum(pBlendBoneKey->m_RotatorArray.GetNum());
			for (unsigned int j = 0; j < pBlendBoneKey->m_RotatorArray.GetNum(); j++)
			{
				SKREAL fSourceTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fSourceTimeScale;


				pAddBoneKey->m_RotatorArray[j].m_dKeyTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime;

				SKQuat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_RotatorArray[j].m_Quat = SourceRoator * TargetRoator;

			}
		}
		



		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}
SKAnim * SKResourceManager::CreateAdditiveAnim(SKAnim * pSourceAnim, SKAnim * pTargetAnim, SKAnimR * pBlendAnimR,SKREAL fSourceTime, SKREAL fTargetTime)
{
	if (!pSourceAnim || !pTargetAnim)
	{
		return NULL;
	}


	while (!pBlendAnimR->IsLoaded())
	{

	}
	SKAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}

	SKAnim * pAddAnim = SK_NEW SKAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pSourceAnim->IsRootAnim());


	for (unsigned int i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		SKBoneKey * pBlendBoneKey = pBlendAnim->GetBoneKey(i);
		if (!pBlendBoneKey)
		{
			continue;
		}
		SKBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		SKBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		SKBoneKey * pAddBoneKey = SK_NEW SKBoneKey();
		pAddBoneKey->m_cName = pBlendBoneKey->m_cName;

		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = SKVector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = SKVector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = SKQuat();
			return false;
		}
		else
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			SKVector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
			pAddBoneKey->m_TranslationArray[0].m_dKeyTime = 0.0f;
			SKVector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
			pAddBoneKey->m_TranslationArray[0].m_Vector = SourceTranslation - TargetTranslation;



			pAddBoneKey->m_ScaleArray.SetBufferNum(1);
			SKVector3 TargetScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fTargetTime);
			SKMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
			pAddBoneKey->m_ScaleArray[0].m_dKeyTime = 0.0f;
			SKVector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);
			SKMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);
			pAddBoneKey->m_ScaleArray[0].m_Vector = SourceScale / TargetScale;



			SKQuat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
			pAddBoneKey->m_RotatorArray.SetBufferNum(1);
			pAddBoneKey->m_RotatorArray[0].m_dKeyTime = 0.0f;
			SKQuat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fSourceTime);
			pAddBoneKey->m_RotatorArray[0].m_Quat = SourceRoator * TargetRoator;


		}




		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}

SKAnim * SKResourceManager::CreateAnim(SKAnim * pSourceAnim, SKREAL fSourceTime)
{
	if (!pSourceAnim)
	{
		return NULL;
	}



	SKAnim * pAddAnim = SK_NEW SKAnim();
	pAddAnim->SetRootMotion(pSourceAnim->IsRootAnim());


	for (unsigned int i = 0; i < pSourceAnim->GetBoneKeyNum(); i++)
	{
		SKBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(i);
		if (!pSourceBoneKey)
		{
			continue;
		}


		SKBoneKey * pAddBoneKey = SK_NEW SKBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;

		pAddBoneKey->m_TranslationArray.SetBufferNum(1);

		SKVector3 Translate = pSourceAnim->GetTranslation(pSourceBoneKey->m_cName, fSourceTime);
		pAddBoneKey->m_TranslationArray[0].m_dKeyTime = 0.0F;
		pAddBoneKey->m_TranslationArray[0].m_Vector = Translate;

		

		pAddBoneKey->m_ScaleArray.SetBufferNum(1);
		SKVector3 Scale = pSourceAnim->GetScale(pSourceBoneKey->m_cName, fSourceTime);

		SKMAC_ASSERT(Scale.x > EPSILON_E4 && Scale.y > EPSILON_E4 && Scale.z > EPSILON_E4);

		pAddBoneKey->m_ScaleArray[0].m_dKeyTime = 0.0f;
		pAddBoneKey->m_ScaleArray[0].m_Vector = Scale;



		pAddBoneKey->m_RotatorArray.SetBufferNum(1);
		SKQuat Rotator = pSourceAnim->GetQuat(pSourceBoneKey->m_cName, fSourceTime);


		pAddBoneKey->m_RotatorArray[0].m_dKeyTime = 0.0f;
		pAddBoneKey->m_RotatorArray[0].m_Quat = Rotator;


		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}