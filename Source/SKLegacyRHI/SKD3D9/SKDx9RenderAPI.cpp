#include "SKDx9Renderer.h"
#include "SKRenderThread.h"
#include "SKGraphicInclude.h"
#include "SK2DTexture.h"
#include "SKCubeTexture.h"
#include "SKDx9Resource.h"
#include "SKProfiler.h"
using namespace SKEngine2;
DECLEAR_COUNT_PROFILENODE(Dx9DrawPrimitiveNum, )
DECLEAR_COUNT_PROFILENODE(Dx9DrawGeometryNum, )
DECLEAR_COUNT_PROFILENODE(Dx9DrawPassNum, )
bool SKDX9Renderer::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	struct SKDx9RenderStatePara
	{
		D3DRENDERSTATETYPE State;
		DWORD Value;
	};
	HRESULT hResult = NULL;
	SKDx9RenderStatePara RenderStatePara;
	RenderStatePara.State = State;
	RenderStatePara.Value = Value;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetRenderStateCommand,
		SKDx9RenderStatePara,RenderStatePara,RenderStatePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetRenderState(RenderStatePara.State,RenderStatePara.Value);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->SetRenderState(RenderStatePara.State,RenderStatePara.Value);	
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	
	struct SKDx9SamplerStatePara
	{
		DWORD Sampler;
		D3DSAMPLERSTATETYPE Type;
		DWORD Value;
	};
	HRESULT hResult = NULL;
	SKDx9SamplerStatePara SamplerStatePara;
	SamplerStatePara.Sampler = Sampler;
	SamplerStatePara.Type = Type;
	SamplerStatePara.Value = Value;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetSamplerStateCommand,
		SKDx9SamplerStatePara,SamplerStatePara,SamplerStatePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetSamplerState(SamplerStatePara.Sampler,
			SamplerStatePara.Type,SamplerStatePara.Value);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->SetSamplerState(SamplerStatePara.Sampler,
		SamplerStatePara.Type,SamplerStatePara.Value);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,SKREAL Z,DWORD Stencil)
{
	struct SKDx9ClearPara
	{
		DWORD Count;
		D3DRECT Rects;
		DWORD Flags;
		D3DCOLOR Color;
		SKREAL Z;
		DWORD Stencil;
	};
	HRESULT hResult = NULL;
	SKDx9ClearPara ClearPara;
	ClearPara.Count = Count;
	if (pRects)
	{
		ClearPara.Rects = *pRects;
	}
	ClearPara.Flags = Flags;
	ClearPara.Color = Color;
	ClearPara.Z = Z;
	ClearPara.Stencil = Stencil;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9ClearCommand,
		SKDx9ClearPara,ClearPara,ClearPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		if (!ClearPara.Count)
		{
			hResult = m_pDevice->Clear(ClearPara.Count,NULL,ClearPara.Flags,ClearPara.Color,ClearPara.Z,ClearPara.Stencil);
		}
		else
		{
			hResult = m_pDevice->Clear(ClearPara.Count,&ClearPara.Rects,ClearPara.Flags,ClearPara.Color,ClearPara.Z,ClearPara.Stencil);
		}
		SKMAC_ASSERT(!FAILED(hResult));
	})
		if (!ClearPara.Count)
		{
			hResult = m_pDevice->Clear(ClearPara.Count,NULL,ClearPara.Flags,ClearPara.Color,ClearPara.Z,ClearPara.Stencil);
		}
		else
		{
			hResult = m_pDevice->Clear(ClearPara.Count,&ClearPara.Rects,ClearPara.Flags,ClearPara.Color,ClearPara.Z,ClearPara.Stencil);
		}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetClipPlane(DWORD Index,CONST SKREAL* pPlane)
{
	struct SKDx9ClipPlanePara
	{
		DWORD Index;
		SKPlane3 Plane;
	};
	HRESULT hResult = NULL;
	SKDx9ClipPlanePara ClipPlanePara ;
	ClipPlanePara.Index = Index;
	ClipPlanePara.Plane = *((SKPlane3 *)pPlane);

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetClipPlaneCommand,
		SKDx9ClipPlanePara,ClipPlanePara,ClipPlanePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetClipPlane(ClipPlanePara.Index,
			(SKREAL *)&ClipPlanePara.Plane);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->SetClipPlane(ClipPlanePara.Index,
		(SKREAL *)&ClipPlanePara.Plane);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetScissorRect(CONST RECT* pRect)
{
	struct SKDx9ScissorRectPara
	{
		RECT rect;
		bool bRect;
	};
	HRESULT hResult = NULL;
	SKDx9ScissorRectPara ScissorRectPara ;
	if (pRect)
	{
		ScissorRectPara.bRect = true;
		ScissorRectPara.rect = *pRect;
	}
	else
	{
		ScissorRectPara.bRect = false;
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetScissorRectCommand,
		SKDx9ScissorRectPara,ScissorRectPara,ScissorRectPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		if (ScissorRectPara.bRect)
		{
			hResult = m_pDevice->SetScissorRect(&ScissorRectPara.rect);
		}
		else
		{
			hResult = m_pDevice->SetScissorRect(NULL);
		}
		
		SKMAC_ASSERT(!FAILED(hResult));
	})
		if (ScissorRectPara.bRect)
		{
			hResult = m_pDevice->SetScissorRect(&ScissorRectPara.rect);
		}
		else
		{
			hResult = m_pDevice->SetScissorRect(NULL);
		}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetVertexShader(IDirect3DVertexShader9** pShader)
{
	struct SKDx9VertexShaderPara
	{
		IDirect3DVertexShader9** pShader;
	};
	HRESULT hResult = NULL;
	SKDx9VertexShaderPara VertexShaderPara ;
	VertexShaderPara.pShader = pShader;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetVertexShaderCommand,
		SKDx9VertexShaderPara,VertexShaderPara,VertexShaderPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		IDirect3DVertexShader9* pShader = NULL;
		if (VertexShaderPara.pShader)
		{
			pShader = *VertexShaderPara.pShader;
		}
		hResult = m_pDevice->SetVertexShader(pShader);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		IDirect3DVertexShader9* pShader = NULL;
		if (VertexShaderPara.pShader)
		{
			pShader = *VertexShaderPara.pShader;
		}
		hResult = m_pDevice->SetVertexShader(pShader);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetPixelShader(IDirect3DPixelShader9** pShader)
{
	struct SKDx9PixelShaderPara
	{
		IDirect3DPixelShader9** pShader;
	};
	HRESULT hResult = NULL;
	SKDx9PixelShaderPara PixelShaderPara ;
	PixelShaderPara.pShader = pShader;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetPixelShaderCommand,
		SKDx9PixelShaderPara,PixelShaderPara,PixelShaderPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		IDirect3DPixelShader9* pShader = NULL;
		if (PixelShaderPara.pShader)
		{
			pShader = *PixelShaderPara.pShader;
		}
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetPixelShader(pShader);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		IDirect3DPixelShader9* pShader = NULL;
		if (PixelShaderPara.pShader)
		{
			pShader = *PixelShaderPara.pShader;
		}
		hResult = m_pDevice->SetPixelShader(pShader);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetTexture(DWORD Stage,IDirect3DBaseTexture9** pTexture)
{
	struct SKDx9TexturePara
	{
		DWORD Stage;
		IDirect3DBaseTexture9** pTexture;
	};
	HRESULT hResult = NULL;
	SKDx9TexturePara TexturePara ;
	TexturePara.pTexture = pTexture;
	TexturePara.Stage = Stage;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetTextureCommand,
		SKDx9TexturePara,TexturePara,TexturePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		IDirect3DBaseTexture9* pTexture = NULL;
		if (TexturePara.pTexture)
		{
			pTexture = *TexturePara.pTexture;
		}
		hResult = m_pDevice->SetTexture(TexturePara.Stage,pTexture);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
		IDirect3DBaseTexture9* pTexture = NULL;
		if (TexturePara.pTexture)
		{
			pTexture = *TexturePara.pTexture;
		}
		hResult = m_pDevice->SetTexture(TexturePara.Stage,pTexture);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** pStreamData,UINT OffsetInBytes,UINT Stride)
{
	struct SKDx9StreamSourcePara
	{
		UINT StreamNumber;
		IDirect3DVertexBuffer9** pStreamData;
		UINT OffsetInBytes;
		UINT Stride;
	};
	HRESULT hResult = NULL;
	SKDx9StreamSourcePara StreamSourcePara ;
	StreamSourcePara.StreamNumber = StreamNumber;
	StreamSourcePara.pStreamData = pStreamData;
	StreamSourcePara.OffsetInBytes = OffsetInBytes;
	StreamSourcePara.Stride = Stride;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetStreamSourceCommand,
		SKDx9StreamSourcePara,StreamSourcePara,StreamSourcePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		IDirect3DVertexBuffer9* pStreamData = NULL;
		if(StreamSourcePara.pStreamData)
		{
			pStreamData = *StreamSourcePara.pStreamData;
		}
		hResult = m_pDevice->SetStreamSource(StreamSourcePara.StreamNumber,pStreamData,
											StreamSourcePara.OffsetInBytes,StreamSourcePara.Stride);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		IDirect3DVertexBuffer9* pStreamData = NULL;
		if(StreamSourcePara.pStreamData)
		{
			pStreamData = *StreamSourcePara.pStreamData;
		}
		hResult = m_pDevice->SetStreamSource(StreamSourcePara.StreamNumber,pStreamData,
											StreamSourcePara.OffsetInBytes,StreamSourcePara.Stride);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetVertexDeclaration(IDirect3DVertexDeclaration9** pDecl)
{
	struct SKDx9VertexDeclarationPara
	{
		IDirect3DVertexDeclaration9** pDecl;
	};
	HRESULT hResult = NULL;
	SKDx9VertexDeclarationPara VertexDeclarationPara ;
	VertexDeclarationPara.pDecl = pDecl;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetVertexDeclarationCommand,
		SKDx9VertexDeclarationPara,VertexDeclarationPara,VertexDeclarationPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		IDirect3DVertexDeclaration9* pDecl = NULL;
		if (VertexDeclarationPara.pDecl)
		{
			pDecl = *VertexDeclarationPara.pDecl;
		}
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetVertexDeclaration(pDecl);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		IDirect3DVertexDeclaration9* pDecl = NULL;
		if (VertexDeclarationPara.pDecl)
		{
			pDecl = *VertexDeclarationPara.pDecl;
		}
		hResult = m_pDevice->SetVertexDeclaration(pDecl);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetIndices(IDirect3DIndexBuffer9** pIndexData)
{
	struct SKDx9IndexBufferPara
	{
		IDirect3DIndexBuffer9** pIndexData;
	};
	HRESULT hResult = NULL;
	SKDx9IndexBufferPara IndexBufferPara ;
	IndexBufferPara.pIndexData = pIndexData;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetIndicesCommand,
		SKDx9IndexBufferPara,IndexBufferPara,IndexBufferPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		IDirect3DIndexBuffer9* pIndexData = NULL;
		if (IndexBufferPara.pIndexData)
		{
			pIndexData = *IndexBufferPara.pIndexData;
		}
		hResult = m_pDevice->SetIndices(pIndexData);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
		IDirect3DIndexBuffer9* pIndexData = NULL;
		if (IndexBufferPara.pIndexData)
		{
			pIndexData = *IndexBufferPara.pIndexData;
		}
		hResult = m_pDevice->SetIndices(pIndexData);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	struct SKDx9ViewportPara
	{
		D3DVIEWPORT9 Viewport;
	};
	HRESULT hResult = NULL;
	SKDx9ViewportPara ViewportPara ;
	ViewportPara.Viewport = *pViewport;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetViewportCommand,
		SKDx9ViewportPara,ViewportPara,ViewportPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetViewport(&ViewportPara.Viewport);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->SetViewport(&ViewportPara.Viewport);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetVertexShaderConstant(unsigned int uiStartRegister,void * pDate,
										unsigned int RegisterNum,unsigned int uiType)
{
	struct SKDx9VertexShaderConstantPara
	{
		unsigned int uiStartRegister;
		void * pDate; 
		unsigned int RegisterNum;
		unsigned int uiType;
	};
	HRESULT hResult = NULL;
	SKDx9VertexShaderConstantPara VertexShaderConstantPara;
	VertexShaderConstantPara.uiStartRegister = uiStartRegister;
	VertexShaderConstantPara.RegisterNum = RegisterNum;
	VertexShaderConstantPara.uiType = uiType;
	if (SKResourceManager::ms_bRenderThread)
	{
		VertexShaderConstantPara.pDate = SKRenderThreadSys::ms_pRenderThreadSys->Assign(uiType,RegisterNum);
		SKMemcpy(VertexShaderConstantPara.pDate,pDate,RegisterNum * sizeof(SKREAL) * 4);
	}
	else
	{
		VertexShaderConstantPara.pDate = pDate;
	}
	

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetVertexShaderConstantCommand,
		SKDx9VertexShaderConstantPara,VertexShaderConstantPara,VertexShaderConstantPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		if(VertexShaderConstantPara.uiType == SKUserConstant::VT_BOOL)
		{
			hResult = m_pDevice->SetVertexShaderConstantB(VertexShaderConstantPara.uiStartRegister,(const BOOL *)VertexShaderConstantPara.pDate,VertexShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(VertexShaderConstantPara.uiType == SKUserConstant::VT_FLOAT)
		{
			hResult = m_pDevice->SetVertexShaderConstantF(VertexShaderConstantPara.uiStartRegister,(const SKREAL *)VertexShaderConstantPara.pDate,VertexShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(VertexShaderConstantPara.uiType == SKUserConstant::VT_INT)
		{
			hResult = m_pDevice->SetVertexShaderConstantI(VertexShaderConstantPara.uiStartRegister,(const int *)VertexShaderConstantPara.pDate,VertexShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else 
		{
			SKMAC_ASSERT(0);
		}
	})
		if(VertexShaderConstantPara.uiType == SKUserConstant::VT_BOOL)
		{
			hResult = m_pDevice->SetVertexShaderConstantB(VertexShaderConstantPara.uiStartRegister,(const BOOL *)VertexShaderConstantPara.pDate,VertexShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(VertexShaderConstantPara.uiType == SKUserConstant::VT_FLOAT)
		{
			hResult = m_pDevice->SetVertexShaderConstantF(VertexShaderConstantPara.uiStartRegister,(const SKREAL *)VertexShaderConstantPara.pDate,VertexShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(VertexShaderConstantPara.uiType == SKUserConstant::VT_INT)
		{
			hResult = m_pDevice->SetVertexShaderConstantI(VertexShaderConstantPara.uiStartRegister,(const int *)VertexShaderConstantPara.pDate,VertexShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else 
		{
			SKMAC_ASSERT(0);
		}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);

	
	return 1;
}
bool SKDX9Renderer::SetPixelShaderConstant(unsigned int uiStartRegister,void * pDate,
							unsigned int RegisterNum,unsigned int uiType)
{
	struct SKDx9PixelShaderConstantPara
	{
		unsigned int uiStartRegister;
		void * pDate; 
		unsigned int RegisterNum;
		unsigned int uiType;
	};
	HRESULT hResult = NULL;
	SKDx9PixelShaderConstantPara PixelShaderConstantPara;
	PixelShaderConstantPara.uiStartRegister = uiStartRegister;
	PixelShaderConstantPara.RegisterNum = RegisterNum;
	PixelShaderConstantPara.uiType = uiType;
	if (SKResourceManager::ms_bRenderThread)
	{
		PixelShaderConstantPara.pDate = SKRenderThreadSys::ms_pRenderThreadSys->Assign(uiType,RegisterNum);
		SKMemcpy(PixelShaderConstantPara.pDate,pDate,RegisterNum * sizeof(SKREAL) * 4);
	}
	else
	{
		PixelShaderConstantPara.pDate = pDate;
	}


	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetPixelShaderConstantCommand,
		SKDx9PixelShaderConstantPara,PixelShaderConstantPara,PixelShaderConstantPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		if(PixelShaderConstantPara.uiType == SKUserConstant::VT_BOOL)
		{
			hResult = m_pDevice->SetPixelShaderConstantB(PixelShaderConstantPara.uiStartRegister,(const BOOL *)PixelShaderConstantPara.pDate,PixelShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(PixelShaderConstantPara.uiType == SKUserConstant::VT_FLOAT)
		{
			hResult = m_pDevice->SetPixelShaderConstantF(PixelShaderConstantPara.uiStartRegister,(const SKREAL *)PixelShaderConstantPara.pDate,PixelShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(PixelShaderConstantPara.uiType == SKUserConstant::VT_INT)
		{
			hResult = m_pDevice->SetPixelShaderConstantI(PixelShaderConstantPara.uiStartRegister,(const int *)PixelShaderConstantPara.pDate,PixelShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else 
		{
			SKMAC_ASSERT(0);
		}
	})
		if(PixelShaderConstantPara.uiType == SKUserConstant::VT_BOOL)
		{
			hResult = m_pDevice->SetPixelShaderConstantB(PixelShaderConstantPara.uiStartRegister,(const BOOL *)PixelShaderConstantPara.pDate,PixelShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(PixelShaderConstantPara.uiType == SKUserConstant::VT_FLOAT)
		{
			hResult = m_pDevice->SetPixelShaderConstantF(PixelShaderConstantPara.uiStartRegister,(const SKREAL *)PixelShaderConstantPara.pDate,PixelShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if(PixelShaderConstantPara.uiType == SKUserConstant::VT_INT)
		{
			hResult = m_pDevice->SetPixelShaderConstantI(PixelShaderConstantPara.uiStartRegister,(const int *)PixelShaderConstantPara.pDate,PixelShaderConstantPara.RegisterNum);
			SKMAC_ASSERT(!FAILED(hResult));
		}
		else 
		{
			SKMAC_ASSERT(0);
		}
		ENQUEUE_UNIQUE_RENDER_COMMAND_END
			SKMAC_ASSERT(!FAILED(hResult));
		return !FAILED(hResult);


		return 1;
}
bool SKDX9Renderer::DrawDynamicBufferMesh(SKDynamicBufferGeometry *pDBGeometry)
{
	HRESULT hResult = NULL;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9DrawDynamicBufferMeshCommand,
		SKGeometry *,pDBGeometry,pDBGeometry,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		ADD_COUNT_PROFILE(Dx9DrawGeometryNum, 1)
		while(true)
		{
			unsigned int uiReturn = pDBGeometry->UpdateGeometry();

			if (uiReturn == SKGeometry::UGRI_FAIL)
			{
				break;
			}
			ADD_COUNT_PROFILE(Dx9DrawPassNum, 1)
			if (!pDBGeometry->GetMeshDate()->GetIndexBuffer())
			{
				hResult = m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pDBGeometry->GetMeshDate()->GetMeshDateType()],0,
					pDBGeometry->GetActiveNum());
			}
			else
			{
				hResult = 
					m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pDBGeometry->GetMeshDate()->GetMeshDateType()],
					0,0,pDBGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(),0,pDBGeometry->GetActiveNum());
			}
			ADD_COUNT_PROFILE(Dx9DrawPrimitiveNum, pDBGeometry->GetActiveNum())
			SKMAC_ASSERT(!FAILED(hResult));

			if (uiReturn == SKGeometry::UGRI_END)
			{
				break;
			}
		}
	})
		ADD_COUNT_PROFILE(Dx9DrawGeometryNum, 1)
	while(true)
	{
		HRESULT hResult = NULL;
		unsigned int uiReturn = pDBGeometry->UpdateGeometry();

		if (uiReturn == SKGeometry::UGRI_FAIL)
		{
			break;
		}
		ADD_COUNT_PROFILE(Dx9DrawPassNum, 1)
		if (!pDBGeometry->GetMeshDate()->GetIndexBuffer())
		{
			hResult = m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pDBGeometry->GetMeshDate()->GetMeshDateType()],0,
				pDBGeometry->GetActiveNum());
		}
		else
		{
			hResult = 
				m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pDBGeometry->GetMeshDate()->GetMeshDateType()],
				0,0,pDBGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(),0,pDBGeometry->GetActiveNum());
		}
		ADD_COUNT_PROFILE(Dx9DrawPrimitiveNum, pDBGeometry->GetActiveNum())
		SKMAC_ASSERT(!FAILED(hResult));

		if (uiReturn == SKGeometry::UGRI_END)
		{
			break;
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		return !FAILED(hResult);
}
bool SKDX9Renderer::DrawMesh(SKGeometry * pGeometry)
{
	HRESULT hResult = NULL;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9DrawMeshCommand,
		SKGeometry *,pGeometry,pGeometry,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		ADD_COUNT_PROFILE(Dx9DrawGeometryNum, 1)
		ADD_COUNT_PROFILE(Dx9DrawPassNum, 1)
		pGeometry->UpdateGeometry();
		if (!pGeometry->GetMeshDate()->GetIndexBuffer())
		{
			hResult = m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pGeometry->GetMeshDate()->GetMeshDateType()],0,
				pGeometry->GetActiveNum());
		}
		else
		{
			hResult = 
				m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pGeometry->GetMeshDate()->GetMeshDateType()],
				0,0,pGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(),0,pGeometry->GetActiveNum());
		}
		ADD_COUNT_PROFILE(Dx9DrawPrimitiveNum, pGeometry->GetActiveNum())
		SKMAC_ASSERT(!FAILED(hResult));

	})
		ADD_COUNT_PROFILE(Dx9DrawGeometryNum, 1)
		
		ADD_COUNT_PROFILE(Dx9DrawPassNum, 1)
		pGeometry->UpdateGeometry();
		if (!pGeometry->GetMeshDate()->GetIndexBuffer())
		{
			hResult = m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pGeometry->GetMeshDate()->GetMeshDateType()],0,
				pGeometry->GetActiveNum());
		}
		else
		{
			hResult = 
				m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[pGeometry->GetMeshDate()->GetMeshDateType()],
				0,0,pGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum(),0,pGeometry->GetActiveNum());
		}
		ADD_COUNT_PROFILE(Dx9DrawPrimitiveNum, pGeometry->GetActiveNum())
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	return !FAILED(hResult);

	
}
bool SKDX9Renderer::DrawMesh1(SKGeometry * pGeometry)
{
	struct SKDrawMesh1Para
	{
		SKGeometry * pGeometry;
		unsigned int uiMeshType;
		unsigned int uiVertexNum;
		unsigned int uiActiveNum;
	};
	SKDrawMesh1Para DrawMesh1Para;
	DrawMesh1Para.pGeometry = pGeometry;
	DrawMesh1Para.uiMeshType = pGeometry->GetMeshDate()->GetMeshDateType();
	DrawMesh1Para.uiActiveNum = pGeometry->GetActiveNum();
	DrawMesh1Para.uiVertexNum = pGeometry->GetMeshDate()->GetVertexBuffer()->GetVertexNum();
	HRESULT hResult = NULL;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9DrawMesh1Command,
		SKDrawMesh1Para ,DrawMesh1Para,DrawMesh1Para,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		ADD_COUNT_PROFILE(Dx9DrawGeometryNum, 1)

			ADD_COUNT_PROFILE(Dx9DrawPassNum, 1)
		if (!DrawMesh1Para.pGeometry->GetMeshDate()->GetIndexBuffer())
		{
			hResult = m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[DrawMesh1Para.uiMeshType],0,
				DrawMesh1Para.uiActiveNum);
		}
		else
		{
			hResult = 
				m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[DrawMesh1Para.uiMeshType],
				0,0,DrawMesh1Para.uiVertexNum,0,DrawMesh1Para.uiActiveNum);
		}
		ADD_COUNT_PROFILE(Dx9DrawPrimitiveNum, DrawMesh1Para.uiActiveNum)
		SKMAC_ASSERT(!FAILED(hResult));

	})
		ADD_COUNT_PROFILE(Dx9DrawGeometryNum, 1)

		ADD_COUNT_PROFILE(Dx9DrawPassNum, 1)
	if (!DrawMesh1Para.pGeometry->GetMeshDate()->GetIndexBuffer())
	{
		hResult = m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[DrawMesh1Para.uiMeshType],0,
			DrawMesh1Para.uiActiveNum);
	}
	else
	{
		hResult = 
			m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)ms_dwPrimitiveType[DrawMesh1Para.uiMeshType],
			0,0,DrawMesh1Para.uiVertexNum,0,DrawMesh1Para.uiActiveNum);
	}
	ADD_COUNT_PROFILE(Dx9DrawPrimitiveNum, DrawMesh1Para.uiActiveNum)
	SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		return !FAILED(hResult);
}
bool SKDX9Renderer::DrawScreenFontEX(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiNumTri)
{
	struct ScreenFontData
	{
		SCREEN_FONT_TYPE* pScreenFontBuffer;
		SKUSHORT_INDEX * pIndexBuffer;
		unsigned int uiVertexNum;
		unsigned int uiNumTri;
	};
	HRESULT hResult = NULL;
	ScreenFontData Screen;
	Screen.uiVertexNum = uiVertexNum;
	Screen.uiNumTri = uiNumTri;
	if (SKResourceManager::ms_bRenderThread)
	{
		unsigned int uiVertexLen = sizeof(SCREEN_FONT_TYPE)* uiVertexNum;
		Screen.pScreenFontBuffer = (SCREEN_FONT_TYPE *)SKRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexLen);
		SKMemcpy(Screen.pScreenFontBuffer, pScreenFontBuffer, uiVertexLen);

		unsigned int uiIndexLen = sizeof(SKUSHORT_INDEX)* uiNumTri * 3;
		Screen.pIndexBuffer = (SKUSHORT_INDEX *)SKRenderThreadSys::ms_pRenderThreadSys->Assign(uiIndexLen);
		SKMemcpy(Screen.pIndexBuffer, pIndexBuffer, uiIndexLen);

	}
	else
	{
		Screen.pScreenFontBuffer = pScreenFontBuffer;
		Screen.pIndexBuffer = pIndexBuffer;
	}

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9DrawScreenFontEXCommand,
		ScreenFontData, Screen, Screen, LPDIRECT3DDEVICE9, m_pDevice, m_pDevice,
		{
		HRESULT hResult = NULL;
		D3DFORMAT DateFormat;

		DateFormat = D3DFMT_INDEX16;

		hResult =
			m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[SKMeshDate::MDT_TRIANGLE],
			0, Screen.uiVertexNum, Screen.uiNumTri, Screen.pIndexBuffer, DateFormat, Screen.pScreenFontBuffer, sizeof(SCREEN_FONT_TYPE));
		SKMAC_ASSERT(!FAILED(hResult));
	})
		D3DFORMAT DateFormat;
	DateFormat = D3DFMT_INDEX16;
	hResult =
		m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[SKMeshDate::MDT_TRIANGLE],
		0, Screen.uiVertexNum, Screen.uiNumTri, Screen.pIndexBuffer, DateFormat, Screen.pScreenFontBuffer, sizeof(SCREEN_FONT_TYPE));
	SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
		SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::DrawScreenEX2(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
									SKUSHORT_INDEX * pIndexBuffer,unsigned int uiNumTri)
{
	
	struct ScreenData
	{
		SCREEN_QUAD_TYPE* pScreenBuffer;
		SKUSHORT_INDEX * pIndexBuffer;
		unsigned int uiVertexNum;
		unsigned int uiNumTri;
	};
	HRESULT hResult = NULL;
	ScreenData Screen;
	Screen.uiVertexNum = uiVertexNum;
	Screen.uiNumTri = uiNumTri;
	if (SKResourceManager::ms_bRenderThread)
	{
		unsigned int uiVertexLen = sizeof(SCREEN_QUAD_TYPE) * uiVertexNum;
		Screen.pScreenBuffer = (SCREEN_QUAD_TYPE *)SKRenderThreadSys::ms_pRenderThreadSys->Assign(uiVertexLen);
		SKMemcpy(Screen.pScreenBuffer,pScreenBuffer,uiVertexLen);
		
		unsigned int uiIndexLen = sizeof(SKUSHORT_INDEX) * uiNumTri * 3;
		Screen.pIndexBuffer = (SKUSHORT_INDEX *)SKRenderThreadSys::ms_pRenderThreadSys->Assign(uiIndexLen);
		SKMemcpy(Screen.pIndexBuffer,pIndexBuffer,uiIndexLen);
		
	}
	else
	{
		Screen.pScreenBuffer = pScreenBuffer;
		Screen.pIndexBuffer = pIndexBuffer;
	}

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9DrawScreenEX2Command,
		ScreenData,Screen,Screen,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		D3DFORMAT DateFormat;

		DateFormat =  D3DFMT_INDEX16;

		hResult = 
			m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[SKMeshDate::MDT_TRIANGLE],
			0,Screen.uiVertexNum,Screen.uiNumTri,Screen.pIndexBuffer,DateFormat,Screen.pScreenBuffer,sizeof(SCREEN_QUAD_TYPE));
		SKMAC_ASSERT(!FAILED(hResult));
	})
		D3DFORMAT DateFormat;
		DateFormat =  D3DFMT_INDEX16;
		hResult = 
			m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[SKMeshDate::MDT_TRIANGLE],
			0,Screen.uiVertexNum,Screen.uiNumTri,Screen.pIndexBuffer,DateFormat,Screen.pScreenBuffer,sizeof(SCREEN_QUAD_TYPE));
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::DrawScreenEX1(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	struct ScreenData
	{
		SCREEN_QUAD_TYPE ScreenQuad[4];
	};
	HRESULT hResult = NULL;
	ScreenData Screen;
	SKMemcpy(&Screen.ScreenQuad[0],&ScreenQuad[0],sizeof(SCREEN_QUAD_TYPE) * 4);

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9DrawScreenEX1Command,
		ScreenData,Screen,Screen,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		D3DFORMAT DateFormat;
		DateFormat =  D3DFMT_INDEX16;
		hResult = m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[SKMeshDate::MDT_TRIANGLE],
			0,4,2,ms_FullScreenI,DateFormat,Screen.ScreenQuad,sizeof(SCREEN_QUAD_TYPE));
		SKMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
		D3DFORMAT DateFormat;
		DateFormat =  D3DFMT_INDEX16;
		hResult = m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[SKMeshDate::MDT_TRIANGLE],
			0,4,2,ms_FullScreenI,DateFormat,Screen.ScreenQuad,sizeof(SCREEN_QUAD_TYPE));
	SKMAC_ASSERT(!FAILED(hResult));
	SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget)
{
	struct SKDx9RenderTargetPara
	{
		DWORD RenderTargetIndex;
		IDirect3DSurface9 ** pRenderTarget;
	};
	HRESULT hResult = NULL;
	SKDx9RenderTargetPara RenderTargetPara ;
	RenderTargetPara.pRenderTarget = pRenderTarget;
	RenderTargetPara.RenderTargetIndex = RenderTargetIndex;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetRenderTargetCommand,
		SKDx9RenderTargetPara,RenderTargetPara,RenderTargetPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		IDirect3DSurface9 * pRenderTarget = NULL;
		if (RenderTargetPara.pRenderTarget)
		{
			pRenderTarget = *RenderTargetPara.pRenderTarget;
		}
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetRenderTarget(RenderTargetPara.RenderTargetIndex,pRenderTarget);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		IDirect3DSurface9 * pRenderTarget = NULL;
		if (RenderTargetPara.pRenderTarget)
		{
			pRenderTarget = *RenderTargetPara.pRenderTarget;
		}
		hResult = m_pDevice->SetRenderTarget(RenderTargetPara.RenderTargetIndex,pRenderTarget);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	struct SKDx9RenderTargetPara
	{
		DWORD RenderTargetIndex;
		IDirect3DSurface9** ppRenderTarget;
	};
	HRESULT hResult = NULL;
	SKDx9RenderTargetPara RenderTargetPara ;
	RenderTargetPara.ppRenderTarget = ppRenderTarget;
	RenderTargetPara.RenderTargetIndex = RenderTargetIndex;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9GetRenderTargetCommand,
		SKDx9RenderTargetPara,RenderTargetPara,RenderTargetPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->GetRenderTarget(RenderTargetPara.RenderTargetIndex,RenderTargetPara.ppRenderTarget);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->GetRenderTarget(RenderTargetPara.RenderTargetIndex,RenderTargetPara.ppRenderTarget);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil)
{
	struct SKDx9DepthStencilSurfacePara
	{
		IDirect3DSurface9 ** pNewZStencil;
	};
	HRESULT hResult = NULL;
	SKDx9DepthStencilSurfacePara DepthStencilSurfacePara ;
	DepthStencilSurfacePara.pNewZStencil = pNewZStencil;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetDepthStencilSurfaceCommand,
		SKDx9DepthStencilSurfacePara,DepthStencilSurfacePara,DepthStencilSurfacePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		IDirect3DSurface9 * pNewZStencil = NULL;
		if (DepthStencilSurfacePara.pNewZStencil)
		{
			pNewZStencil = *DepthStencilSurfacePara.pNewZStencil;
		}

		hResult = m_pDevice->SetDepthStencilSurface(pNewZStencil);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		IDirect3DSurface9 * pNewZStencil = NULL;
		if (DepthStencilSurfacePara.pNewZStencil)
		{
			pNewZStencil = *DepthStencilSurfacePara.pNewZStencil;
		}
		hResult = m_pDevice->SetDepthStencilSurface(pNewZStencil);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	struct SKDx9DepthStencilSurfacePara
	{
		IDirect3DSurface9** ppZStencilSurface;
	};
	HRESULT hResult = NULL;
	SKDx9DepthStencilSurfacePara DepthStencilSurfacePara ;
	DepthStencilSurfacePara.ppZStencilSurface = ppZStencilSurface;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9GetDepthStencilSurfaceCommand,
		SKDx9DepthStencilSurfacePara,DepthStencilSurfacePara,DepthStencilSurfacePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->GetDepthStencilSurface(DepthStencilSurfacePara.ppZStencilSurface);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->GetDepthStencilSurface(DepthStencilSurfacePara.ppZStencilSurface);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget,IDirect3DSurface9** ppRenderTarget)
{
	struct SKDx9RenderTargetPara
	{
		DWORD RenderTargetIndex;
		IDirect3DSurface9 ** pRenderTarget;
		IDirect3DSurface9** ppRenderTarget;
	};
	HRESULT hResult = NULL;
	SKDx9RenderTargetPara RenderTargetPara ;
	RenderTargetPara.pRenderTarget = pRenderTarget;
	RenderTargetPara.ppRenderTarget = ppRenderTarget;
	RenderTargetPara.RenderTargetIndex = RenderTargetIndex;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetRenderTargetCommand,
		SKDx9RenderTargetPara,RenderTargetPara,RenderTargetPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		SKMAC_RELEASE((*RenderTargetPara.ppRenderTarget));
		hResult = m_pDevice->GetRenderTarget(RenderTargetPara.RenderTargetIndex,RenderTargetPara.ppRenderTarget);
		//i != 0  buffer⣬ûм
		SKMAC_ASSERT(!FAILED(hResult));

		IDirect3DSurface9 * pRenderTarget = NULL;
		if(RenderTargetPara.pRenderTarget)
		{
			pRenderTarget = *RenderTargetPara.pRenderTarget;
		}
		hResult = m_pDevice->SetRenderTarget(RenderTargetPara.RenderTargetIndex,pRenderTarget);
		SKMAC_ASSERT(!FAILED(hResult));
	})
	SKMAC_RELEASE((*ppRenderTarget));
	hResult = m_pDevice->GetRenderTarget(RenderTargetPara.RenderTargetIndex,RenderTargetPara.ppRenderTarget);
	//i != 0  buffer⣬ûм
	SKMAC_ASSERT(!FAILED(hResult));
	IDirect3DSurface9 * pRenderTarget = NULL;
	if(RenderTargetPara.pRenderTarget)
	{
		pRenderTarget = *RenderTargetPara.pRenderTarget;
	}
	hResult = m_pDevice->SetRenderTarget(RenderTargetPara.RenderTargetIndex,pRenderTarget);
	SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::EndRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget)
{
	struct SKDx9RenderTargetPara
	{
		DWORD RenderTargetIndex;
		IDirect3DSurface9 ** pRenderTarget;
	};
	HRESULT hResult = NULL;
	SKDx9RenderTargetPara RenderTargetPara ;
	RenderTargetPara.pRenderTarget = pRenderTarget;
	RenderTargetPara.RenderTargetIndex = RenderTargetIndex;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9EndRenderTargetCommand,
		SKDx9RenderTargetPara,RenderTargetPara,RenderTargetPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetRenderTarget(RenderTargetPara.RenderTargetIndex,*RenderTargetPara.pRenderTarget);
		SKMAC_ASSERT(!FAILED(hResult));
		SKMAC_RELEASE((*RenderTargetPara.pRenderTarget));
	})
	hResult = m_pDevice->SetRenderTarget(RenderTargetPara.RenderTargetIndex,*RenderTargetPara.pRenderTarget);
	SKMAC_RELEASE((*RenderTargetPara.pRenderTarget));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil,IDirect3DSurface9** ppZStencilSurface)
{
	struct SKDx9DepthStencilSurfacePara
	{
		IDirect3DSurface9 ** pNewZStencil;
		IDirect3DSurface9** ppZStencilSurface;
	};
	HRESULT hResult = NULL;
	SKDx9DepthStencilSurfacePara DepthStencilSurfacePara ;
	DepthStencilSurfacePara.pNewZStencil = pNewZStencil;
	DepthStencilSurfacePara.ppZStencilSurface = ppZStencilSurface;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetDepthStencilSurfaceCommand,
		SKDx9DepthStencilSurfacePara,DepthStencilSurfacePara,DepthStencilSurfacePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		SKMAC_RELEASE((*DepthStencilSurfacePara.ppZStencilSurface))
			hResult = m_pDevice->GetDepthStencilSurface(DepthStencilSurfacePara.ppZStencilSurface);
		SKMAC_ASSERT(!FAILED(hResult));
		IDirect3DSurface9 * pNewZStencil = NULL;
		if (DepthStencilSurfacePara.pNewZStencil)
		{
			pNewZStencil = *DepthStencilSurfacePara.pNewZStencil;
		}
		hResult = m_pDevice->SetDepthStencilSurface(pNewZStencil);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		SKMAC_RELEASE((*DepthStencilSurfacePara.ppZStencilSurface))
		hResult = m_pDevice->GetDepthStencilSurface(DepthStencilSurfacePara.ppZStencilSurface);
		SKMAC_ASSERT(!FAILED(hResult));
		IDirect3DSurface9 * pNewZStencil = NULL;
		if (DepthStencilSurfacePara.pNewZStencil)
		{
			pNewZStencil = *DepthStencilSurfacePara.pNewZStencil;
		}
		hResult = m_pDevice->SetDepthStencilSurface(pNewZStencil);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::EndDepthStencilSurface(IDirect3DSurface9** pNewZStencil)
{
	struct SKDx9DepthStencilSurfacePara
	{
		IDirect3DSurface9 ** pNewZStencil;
	};
	HRESULT hResult = NULL;
	SKDx9DepthStencilSurfacePara DepthStencilSurfacePara ;
	DepthStencilSurfacePara.pNewZStencil = pNewZStencil;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetDepthStencilSurfaceCommand,
		SKDx9DepthStencilSurfacePara,DepthStencilSurfacePara,DepthStencilSurfacePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->SetDepthStencilSurface(*DepthStencilSurfacePara.pNewZStencil);
		SKMAC_ASSERT(!FAILED(hResult));
		SKMAC_RELEASE((*DepthStencilSurfacePara.pNewZStencil));
	})
		hResult = m_pDevice->SetDepthStencilSurface(*DepthStencilSurfacePara.pNewZStencil);
		SKMAC_RELEASE((*DepthStencilSurfacePara.pNewZStencil))
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::StretchRect(IDirect3DSurface9** pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9** pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
	struct SKDx9StretchRectPara
	{
		IDirect3DSurface9** pSourceSurface;
		RECT pSourceRect;
		IDirect3DSurface9** pDestSurface;
		RECT pDestRect;
		D3DTEXTUREFILTERTYPE Filter;
	};
	HRESULT hResult = NULL;
	SKDx9StretchRectPara StretchRectPara ;
	StretchRectPara.pSourceSurface = pSourceSurface;
	StretchRectPara.pSourceRect = *pSourceRect;
	StretchRectPara.pDestSurface = pDestSurface;
	StretchRectPara.pDestRect = *pDestRect;
	StretchRectPara.Filter = Filter;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetStretchRectCommand,
		SKDx9StretchRectPara,StretchRectPara,StretchRectPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->StretchRect(*StretchRectPara.pSourceSurface,&StretchRectPara.pSourceRect,
										*StretchRectPara.pDestSurface,&StretchRectPara.pDestRect,StretchRectPara.Filter);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->StretchRect(*StretchRectPara.pSourceSurface,&StretchRectPara.pSourceRect,
		*StretchRectPara.pDestSurface,&StretchRectPara.pDestRect,StretchRectPara.Filter);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::BeginRender()
{
	HRESULT hResult = NULL;
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(SKDx9BeginRenderCommand,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->BeginScene();
		SKMAC_ASSERT(!FAILED(hResult));
	})
		hResult = m_pDevice->BeginScene();
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::EndRender()
{
	struct SKDx9EndRenderPara
	{
		LPDIRECT3DSWAPCHAIN9  m_pChain;
	};
	HRESULT hResult = NULL;
	SKDx9EndRenderPara EndRenderPara ;
	if (m_bWindowed && (m_iNumChildWindow > 0) && m_iCurWindowID > 0) 
	{
		EndRenderPara.m_pChain = m_pChain[m_iCurWindowID];
	}
	else
	{
		EndRenderPara.m_pChain = m_pMainChain;
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetEndRenderCommand,
		SKDx9EndRenderPara,EndRenderPara,EndRenderPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		m_pDevice->EndScene();
		hResult = EndRenderPara.m_pChain->Present(NULL, NULL, NULL, NULL, 0);

	})
		m_pDevice->EndScene();
		hResult = EndRenderPara.m_pChain->Present(NULL, NULL, NULL, NULL, 0);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	SKMAC_ASSERT(!FAILED(hResult));
	return !FAILED(hResult);
}
bool SKDX9Renderer::UseWindowEx(int uiWindowID)
{
	struct SKDx9UseWindowPara
	{
		LPDIRECT3DSWAPCHAIN9  m_pChain;
		LPDIRECT3DSURFACE9	  m_pDepth;
	};
	HRESULT hResult = NULL;
	SKDx9UseWindowPara UseWindowPara ;
	if (uiWindowID == - 1) 
	{
		UseWindowPara.m_pChain = m_pMainChain;	
		UseWindowPara.m_pDepth = m_pMainDepthStencilBuffer;
	}
	else
	{
		UseWindowPara.m_pChain = m_pChain[m_iCurWindowID];
		if (m_pChainnDepthStencilBuffer[m_iCurWindowID])
		{
			UseWindowPara.m_pDepth = m_pChainnDepthStencilBuffer[m_iCurWindowID];
		}
		else
		{
			UseWindowPara.m_pDepth = m_pMainDepthStencilBuffer;
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKDx9SetUseWindowCommand,
		SKDx9UseWindowPara,UseWindowPara,UseWindowPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		LPDIRECT3DSURFACE9 pBack=NULL;
		hResult = UseWindowPara.m_pChain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBack);
		m_pDevice->SetRenderTarget(0, pBack);
		SKMAC_RELEASE(pBack);
		hResult = m_pDevice->SetDepthStencilSurface(UseWindowPara.m_pDepth);

	})
		LPDIRECT3DSURFACE9 pBack=NULL;
		hResult = UseWindowPara.m_pChain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBack);
		SKMAC_ASSERT(!FAILED(hResult));
		m_pDevice->SetRenderTarget(0, pBack);
		SKMAC_RELEASE(pBack);
		hResult = m_pDevice->SetDepthStencilSurface(UseWindowPara.m_pDepth);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END	
	return !FAILED(hResult);
}
bool SKDX9Renderer::CopyResourceBuffer(unsigned int uiLevel,LPDIRECT3DTEXTURE9 * pSource, LPDIRECT3DCUBETEXTURE9 * pDest,CONST RECT* pRect,D3DTEXTUREFILTERTYPE Filter ,D3DCUBEMAP_FACES CubFace)
{
	struct SKCopyResourceBufferPara
	{
		LPDIRECT3DTEXTURE9 * pSource;
		LPDIRECT3DCUBETEXTURE9 * pDest;
		RECT Rect;
		D3DTEXTUREFILTERTYPE Filter ;
		D3DCUBEMAP_FACES CubFace;
		unsigned int uiLevel;
	};
	SKCopyResourceBufferPara CopyResourceBufferPara;
	CopyResourceBufferPara.pSource = pSource;
	CopyResourceBufferPara.pDest = pDest;
	CopyResourceBufferPara.Rect = *pRect;
	CopyResourceBufferPara.Filter = Filter;
	CopyResourceBufferPara.CubFace = CubFace;
	CopyResourceBufferPara.uiLevel = uiLevel;
	HRESULT hResult = NULL;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCopyResourceBufferCommand,
		SKCopyResourceBufferPara,CopyResourceBufferPara,CopyResourceBufferPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		LPDIRECT3DSURFACE9 pSourceSurface = NULL;
		LPDIRECT3DSURFACE9 pDestSurface = NULL;
		hResult = (*CopyResourceBufferPara.pSource)->GetSurfaceLevel(CopyResourceBufferPara.uiLevel,&pSourceSurface);
		SKMAC_ASSERT(!FAILED(hResult));

		hResult = (*CopyResourceBufferPara.pDest)->GetCubeMapSurface(CopyResourceBufferPara.CubFace,CopyResourceBufferPara.uiLevel,&pDestSurface);
		SKMAC_ASSERT(!FAILED(hResult));

		
		hResult = m_pDevice->StretchRect(pSourceSurface,&CopyResourceBufferPara.Rect,pDestSurface,&CopyResourceBufferPara.Rect,CopyResourceBufferPara.Filter);
		SKMAC_ASSERT(!FAILED(hResult));

		SKMAC_RELEASE(pSourceSurface);
		SKMAC_RELEASE(pDestSurface);
	})
		LPDIRECT3DSURFACE9 pSourceSurface = NULL;
		LPDIRECT3DSURFACE9 pDestSurface = NULL;
		hResult = (*CopyResourceBufferPara.pSource)->GetSurfaceLevel(CopyResourceBufferPara.uiLevel,&pSourceSurface);
		SKMAC_ASSERT(!FAILED(hResult));

		hResult = (*CopyResourceBufferPara.pDest)->GetCubeMapSurface(CopyResourceBufferPara.CubFace,CopyResourceBufferPara.uiLevel,&pDestSurface);
		SKMAC_ASSERT(!FAILED(hResult));

		HRESULT hResult = NULL;
		hResult = m_pDevice->StretchRect(pSourceSurface,&CopyResourceBufferPara.Rect,pDestSurface,&CopyResourceBufferPara.Rect,CopyResourceBufferPara.Filter);
		SKMAC_ASSERT(!FAILED(hResult));

		SKMAC_RELEASE(pSourceSurface);
		SKMAC_RELEASE(pDestSurface);

	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	return !FAILED(hResult);
}
void SKDX9Renderer::CreateIndexBuffer(unsigned int Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,void * pCopyBuffer,DWORD LockFlag)
{
	struct SKCreateIndexBufferPara
	{
		unsigned int Length;
		DWORD Usage;
		D3DFORMAT Format;
		D3DPOOL Pool;
		IDirect3DIndexBuffer9** ppIndexBuffer;
		void * pCopyBuffer;
		DWORD LockFlag;

	};
	SKCreateIndexBufferPara CreateIndexBufferPara;
	CreateIndexBufferPara.Format = Format;
	CreateIndexBufferPara.Length = Length;
	CreateIndexBufferPara.LockFlag = LockFlag;
	CreateIndexBufferPara.pCopyBuffer = pCopyBuffer;
	CreateIndexBufferPara.Pool = Pool;
	CreateIndexBufferPara.ppIndexBuffer = ppIndexBuffer;
	CreateIndexBufferPara.Usage = Usage;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateIndexBufferCommand,
		SKCreateIndexBufferPara,CreateIndexBufferPara,CreateIndexBufferPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateIndexBuffer(CreateIndexBufferPara.Length,CreateIndexBufferPara.Usage,CreateIndexBufferPara.Format,CreateIndexBufferPara.Pool,CreateIndexBufferPara.ppIndexBuffer,0);
		SKMAC_ASSERT(!FAILED(hResult));

		if (CreateIndexBufferPara.pCopyBuffer)
		{
			unsigned char* pcIndices;
			hResult = (*CreateIndexBufferPara.ppIndexBuffer)->Lock(0,CreateIndexBufferPara.Length,(void**)(&pcIndices),CreateIndexBufferPara.LockFlag);
			SKMAC_ASSERT(!FAILED(hResult));
			SKMemcpy(pcIndices,CreateIndexBufferPara.pCopyBuffer,CreateIndexBufferPara.Length);
			hResult = (*CreateIndexBufferPara.ppIndexBuffer)->Unlock();
			SKMAC_ASSERT(!FAILED(hResult));
		}
	})
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateIndexBuffer(CreateIndexBufferPara.Length,CreateIndexBufferPara.Usage,CreateIndexBufferPara.Format,CreateIndexBufferPara.Pool,CreateIndexBufferPara.ppIndexBuffer,0);
		SKMAC_ASSERT(!FAILED(hResult));

		if (CreateIndexBufferPara.pCopyBuffer)
		{
			unsigned char* pcIndices;
			hResult = (*CreateIndexBufferPara.ppIndexBuffer)->Lock(0,CreateIndexBufferPara.Length,(void**)(&pcIndices),CreateIndexBufferPara.LockFlag);
			SKMAC_ASSERT(!FAILED(hResult));
			SKMemcpy(pcIndices,CreateIndexBufferPara.pCopyBuffer,CreateIndexBufferPara.Length);
			hResult = (*CreateIndexBufferPara.ppIndexBuffer)->Unlock();
			SKMAC_ASSERT(!FAILED(hResult));
		}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	

	
}
void SKDX9Renderer::CreateVertexBuffer(UINT Length,DWORD Usage,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,DWORD Flags,SKVertexBuffer * pVBuffer,unsigned int uiOneVextexSize)
{
	struct SKCreateVertexBufferPara
	{
		UINT Length;
		DWORD Usage;
		D3DPOOL Pool;
		IDirect3DVertexBuffer9** ppVertexBuffer;
		SKVertexBuffer * pVBuffer;
		DWORD LockFlag;
		unsigned int uiOneVextexSize;

	};	
	SKCreateVertexBufferPara CreateVertexBufferPara;
	CreateVertexBufferPara.Length = Length;
	CreateVertexBufferPara.Usage = Usage;
	CreateVertexBufferPara.Pool = Pool;
	CreateVertexBufferPara.ppVertexBuffer = ppVertexBuffer;
	CreateVertexBufferPara.pVBuffer = pVBuffer;
	CreateVertexBufferPara.LockFlag = Flags;
	CreateVertexBufferPara.uiOneVextexSize = uiOneVextexSize;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateVertexBufferCommand,
		SKCreateVertexBufferPara,CreateVertexBufferPara,CreateVertexBufferPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateVertexBuffer(CreateVertexBufferPara.Length,CreateVertexBufferPara.Usage,0,CreateVertexBufferPara.Pool,
			CreateVertexBufferPara.ppVertexBuffer,0);
		SKMAC_ASSERT(!FAILED(hResult));
		unsigned char *pResource;
		hResult = (*CreateVertexBufferPara.ppVertexBuffer)->Lock(0,CreateVertexBufferPara.Length,(void**)(&pResource),CreateVertexBufferPara.LockFlag);
		SKMAC_ASSERT(!FAILED(hResult));

		SKDataBuffer * pDate;


		unsigned int iVertexSize = 0;
		for(unsigned int i = 0 ; i < CreateVertexBufferPara.pVBuffer->GetPositionLevel(); i++)
		{
			pDate = CreateVertexBufferPara.pVBuffer->GetPositionDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}

		}

		for(unsigned int i = 0 ; i < CreateVertexBufferPara.pVBuffer->GetTexCoordLevel(); i++)
		{
			pDate = CreateVertexBufferPara.pVBuffer->GetTexCoordDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}

		for(unsigned int i = 0 ; i < CreateVertexBufferPara.pVBuffer->GetNormalLevel(); i++)
		{
			pDate = CreateVertexBufferPara.pVBuffer->GetNormalDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}
		pDate = CreateVertexBufferPara.pVBuffer->GetTangentDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}


		pDate = CreateVertexBufferPara.pVBuffer->GetBinormalDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
		pDate = CreateVertexBufferPara.pVBuffer->GetPSizeDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		for(unsigned int i = 0 ; i <CreateVertexBufferPara. pVBuffer->GetColorLevel(); i++)
		{

			pDate = CreateVertexBufferPara.pVBuffer->GetColorDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetFogDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetDepthDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetBlendWeightDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetBlendIndicesDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}


		hResult = (*CreateVertexBufferPara.ppVertexBuffer)->Unlock();
		SKMAC_ASSERT(!FAILED(hResult));
		
		
	})
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateVertexBuffer(CreateVertexBufferPara.Length,CreateVertexBufferPara.Usage,0,CreateVertexBufferPara.Pool,
			CreateVertexBufferPara.ppVertexBuffer,0);
		SKMAC_ASSERT(!FAILED(hResult));
		unsigned char *pResource;
		hResult = (*CreateVertexBufferPara.ppVertexBuffer)->Lock(0,CreateVertexBufferPara.Length,(void**)(&pResource),CreateVertexBufferPara.LockFlag);
		SKMAC_ASSERT(!FAILED(hResult));

		SKDataBuffer * pDate;


		unsigned int iVertexSize = 0;
		for(unsigned int i = 0 ; i < CreateVertexBufferPara.pVBuffer->GetPositionLevel(); i++)
		{
			pDate = CreateVertexBufferPara.pVBuffer->GetPositionDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}

		}

		for(unsigned int i = 0 ; i < CreateVertexBufferPara.pVBuffer->GetTexCoordLevel(); i++)
		{
			pDate = CreateVertexBufferPara.pVBuffer->GetTexCoordDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}

		for(unsigned int i = 0 ; i < CreateVertexBufferPara.pVBuffer->GetNormalLevel(); i++)
		{
			pDate = CreateVertexBufferPara.pVBuffer->GetNormalDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}
		pDate = CreateVertexBufferPara.pVBuffer->GetTangentDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}


		pDate = CreateVertexBufferPara.pVBuffer->GetBinormalDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}
		pDate = CreateVertexBufferPara.pVBuffer->GetPSizeDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		for(unsigned int i = 0 ; i <CreateVertexBufferPara. pVBuffer->GetColorLevel(); i++)
		{

			pDate = CreateVertexBufferPara.pVBuffer->GetColorDate(i);
			if(pDate)
			{
				for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
				{
					SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
						(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

				}
				iVertexSize += pDate->GetStride();
			}
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetFogDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetDepthDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetBlendWeightDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}

		pDate = CreateVertexBufferPara.pVBuffer->GetBlendIndicesDate();
		if(pDate)
		{
			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
			{
				SKMemcpy(pResource + CreateVertexBufferPara.uiOneVextexSize * j + iVertexSize,
					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());

			}
			iVertexSize += pDate->GetStride();
		}


		hResult = (*CreateVertexBufferPara.ppVertexBuffer)->Unlock();
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END


}
void SKDX9Renderer::CreateVertexDeclaration(const SKArray<D3DVERTEXELEMENT9> & Element, IDirect3DVertexDeclaration9** ppDecl)
{
	struct SKCreateVertexDeclarationPara
	{
		SKArray<D3DVERTEXELEMENT9> Element;
		IDirect3DVertexDeclaration9** ppDecl;
	};	
	SKCreateVertexDeclarationPara CreateVertexDeclarationPara;
	CreateVertexDeclarationPara.Element = Element;
	CreateVertexDeclarationPara.ppDecl = ppDecl;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateVertexDeclarationCommand,
		SKCreateVertexDeclarationPara,CreateVertexDeclarationPara,CreateVertexDeclarationPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateVertexDeclaration(&CreateVertexDeclarationPara.Element[0],
			CreateVertexDeclarationPara.ppDecl);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateVertexDeclaration(&CreateVertexDeclarationPara.Element[0],
			CreateVertexDeclarationPara.ppDecl);
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
void SKDX9Renderer::CreateDepthStencilSurface(D3DFORMAT FormatType ,D3DMULTISAMPLE_TYPE MultiSampleTypes,
							   unsigned int uiWidth ,unsigned int uiHeight,IDirect3DSurface9** ppSurface)
{
	struct SKCreateDepthStencilSurfacePara
	{
		unsigned int uinAdapter;
		D3DDEVTYPE DevTypes;
		D3DFORMAT FormatType;
		D3DMULTISAMPLE_TYPE MultiSampleTypes;
		unsigned int uiWidth ;
		unsigned int uiHeight;
		IDirect3DSurface9** ppSurface;
		bool bWindowed;
	};
	SKCreateDepthStencilSurfacePara CreateDepthStencilSurfacePara;
	CreateDepthStencilSurfacePara.uinAdapter = m_uinAdapter;
	CreateDepthStencilSurfacePara.DevTypes = (D3DDEVTYPE)ms_dwDevTypes[m_uiDevType];
	CreateDepthStencilSurfacePara.FormatType = FormatType;
	CreateDepthStencilSurfacePara.MultiSampleTypes = MultiSampleTypes;
	CreateDepthStencilSurfacePara.uiHeight = uiHeight;
	CreateDepthStencilSurfacePara.uiWidth = uiWidth;
	CreateDepthStencilSurfacePara.ppSurface = ppSurface;
	CreateDepthStencilSurfacePara.bWindowed = m_bWindowed;
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(SKCreateDepthStencilSurfaceCommand,
		SKCreateDepthStencilSurfacePara,CreateDepthStencilSurfacePara,CreateDepthStencilSurfacePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,LPDIRECT3D9,ms_pMain,ms_pMain,
	{
		HRESULT hResult = NULL;
		DWORD uiMulSample;
		hResult = ms_pMain->CheckDeviceMultiSampleType(CreateDepthStencilSurfacePara.uinAdapter,CreateDepthStencilSurfacePara.DevTypes, 
			CreateDepthStencilSurfacePara.FormatType,CreateDepthStencilSurfacePara.bWindowed,CreateDepthStencilSurfacePara.MultiSampleTypes, &uiMulSample);
		if(FAILED(hResult))			
		{
			SKMAC_ASSERT(0);
		}

		hResult =m_pDevice->CreateDepthStencilSurface(CreateDepthStencilSurfacePara.uiWidth,CreateDepthStencilSurfacePara.uiHeight,
			CreateDepthStencilSurfacePara.FormatType,CreateDepthStencilSurfacePara.MultiSampleTypes,uiMulSample - 1,FALSE,
			CreateDepthStencilSurfacePara.ppSurface,NULL);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
		DWORD uiMulSample;
		hResult = ms_pMain->CheckDeviceMultiSampleType(CreateDepthStencilSurfacePara.uinAdapter,CreateDepthStencilSurfacePara.DevTypes, 
			CreateDepthStencilSurfacePara.FormatType,CreateDepthStencilSurfacePara.bWindowed,CreateDepthStencilSurfacePara.MultiSampleTypes, &uiMulSample);
		if(FAILED(hResult))			
		{
			SKMAC_ASSERT(0);
		}

		hResult =m_pDevice->CreateDepthStencilSurface(CreateDepthStencilSurfacePara.uiWidth,CreateDepthStencilSurfacePara.uiHeight,
			CreateDepthStencilSurfacePara.FormatType,CreateDepthStencilSurfacePara.MultiSampleTypes,uiMulSample - 1,FALSE,
			CreateDepthStencilSurfacePara.ppSurface,NULL);
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END


}
void SKDX9Renderer::GetSurfaceLevel(LPDIRECT3DTEXTURE9 * pTexture,unsigned int uiLevel,IDirect3DSurface9** ppSurfaceLevel)
{
	struct SKGetSurfaceLevelPara
	{
		LPDIRECT3DTEXTURE9 * pTexture;
		unsigned int uiLevel;
		IDirect3DSurface9** ppSurfaceLevel;
	};
	SKGetSurfaceLevelPara GetSurfaceLevelPara;
	GetSurfaceLevelPara.pTexture = pTexture;
	GetSurfaceLevelPara.uiLevel = uiLevel;
	GetSurfaceLevelPara.ppSurfaceLevel = ppSurfaceLevel;
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(SKGetSurfaceLevelCommand,
		SKGetSurfaceLevelPara,GetSurfaceLevelPara,GetSurfaceLevelPara,
	{
		HRESULT hResult = NULL;
		hResult = (*GetSurfaceLevelPara.pTexture)->GetSurfaceLevel(GetSurfaceLevelPara.uiLevel ,GetSurfaceLevelPara.ppSurfaceLevel);
		SKMAC_ASSERT(!FAILED(hResult));
	})

		HRESULT hResult = NULL;
		hResult = (*GetSurfaceLevelPara.pTexture)->GetSurfaceLevel(GetSurfaceLevelPara.uiLevel ,GetSurfaceLevelPara.ppSurfaceLevel);
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
void SKDX9Renderer::GetCubeMapSurface(LPDIRECT3DCUBETEXTURE9 * pCubTexture,D3DCUBEMAP_FACES FaceType,UINT Level,IDirect3DSurface9** ppCubeMapSurface)
{
	struct SKGetCubeMapSurfacePara
	{
		LPDIRECT3DCUBETEXTURE9 * pCubTexture;
		D3DCUBEMAP_FACES FaceType;
		unsigned int uiLevel;
		IDirect3DSurface9** ppCubeMapSurface;
	};
	SKGetCubeMapSurfacePara GetCubeMapSurfacePara;
	GetCubeMapSurfacePara.pCubTexture = pCubTexture;
	GetCubeMapSurfacePara.uiLevel = Level;
	GetCubeMapSurfacePara.ppCubeMapSurface = ppCubeMapSurface;
	GetCubeMapSurfacePara.FaceType = FaceType;
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(SKGetCubeMapSurfaceCommand,
		SKGetCubeMapSurfacePara,GetCubeMapSurfacePara,GetCubeMapSurfacePara,
	{
		HRESULT hResult = NULL;
		hResult = (*GetCubeMapSurfacePara.pCubTexture)->GetCubeMapSurface(GetCubeMapSurfacePara.FaceType,GetCubeMapSurfacePara.uiLevel,GetCubeMapSurfacePara.ppCubeMapSurface);
		SKMAC_ASSERT(!FAILED(hResult));
	})

		HRESULT hResult = NULL;
		hResult = (*GetCubeMapSurfacePara.pCubTexture)->GetCubeMapSurface(GetCubeMapSurfacePara.FaceType,GetCubeMapSurfacePara.uiLevel,GetCubeMapSurfacePara.ppCubeMapSurface);
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void SKDX9Renderer::CreateRenderTarget(D3DFORMAT FormatType,D3DMULTISAMPLE_TYPE MultiSampleType,unsigned int uiWidth, unsigned int uiHeight,IDirect3DSurface9** ppSurface)
{
	struct SKCreateRenderTargetPara
	{
		unsigned int uinAdapter;
		D3DDEVTYPE DevTypes;
		bool 	bWindowed;
		D3DFORMAT FormatType;
		D3DMULTISAMPLE_TYPE MultiSampleType;
		unsigned int uiWidth;
		unsigned int uiHeight;
		IDirect3DSurface9** ppSurface;
	};
	SKCreateRenderTargetPara CreateRenderTargetPara;
	CreateRenderTargetPara.uinAdapter = m_uinAdapter;
	CreateRenderTargetPara.DevTypes = (D3DDEVTYPE)ms_dwDevTypes[m_uiDevType];
	CreateRenderTargetPara.bWindowed = m_bWindowed;
	CreateRenderTargetPara.FormatType = FormatType;
	CreateRenderTargetPara.MultiSampleType = MultiSampleType;
	CreateRenderTargetPara.uiWidth = uiWidth;
	CreateRenderTargetPara.uiHeight = uiHeight;
	CreateRenderTargetPara.ppSurface = ppSurface;
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(SKCreateRenderTargetCommand,
		SKCreateRenderTargetPara,CreateRenderTargetPara,CreateRenderTargetPara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,LPDIRECT3D9,ms_pMain,ms_pMain,
	{
		HRESULT hResult = NULL;
		DWORD uiMulSample;
		hResult = ms_pMain->CheckDeviceMultiSampleType(CreateRenderTargetPara.uinAdapter,CreateRenderTargetPara.DevTypes, 
			CreateRenderTargetPara.FormatType,CreateRenderTargetPara.bWindowed,CreateRenderTargetPara.MultiSampleType, &uiMulSample);
		if(FAILED(hResult))			
		{
			SKMAC_ASSERT(0);
		}
		hResult = m_pDevice->CreateRenderTarget(CreateRenderTargetPara.uiWidth,CreateRenderTargetPara.uiHeight,
			CreateRenderTargetPara.FormatType,CreateRenderTargetPara.MultiSampleType,
			uiMulSample - 1,false,CreateRenderTargetPara.ppSurface,NULL);
		SKMAC_ASSERT(!FAILED(hResult));
	})
	HRESULT hResult = NULL;
	DWORD uiMulSample;
	hResult = ms_pMain->CheckDeviceMultiSampleType(CreateRenderTargetPara.uinAdapter, CreateRenderTargetPara.DevTypes,
		CreateRenderTargetPara.FormatType, CreateRenderTargetPara.bWindowed, CreateRenderTargetPara.MultiSampleType, &uiMulSample);
	if (FAILED(hResult))
	{
		SKMAC_ASSERT(0);
	}
	hResult = m_pDevice->CreateRenderTarget(CreateRenderTargetPara.uiWidth, CreateRenderTargetPara.uiHeight,
		CreateRenderTargetPara.FormatType, CreateRenderTargetPara.MultiSampleType,
		uiMulSample - 1, false, CreateRenderTargetPara.ppSurface, NULL);
	SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
void SKDX9Renderer::Create2DTexture(SKTexture * pTexture,
				   DWORD                     Usage,
				   D3DFORMAT                 Format,
				   D3DPOOL                   Pool,
				   DWORD LockFlag,
				   IDirect3DBaseTexture9**   ppTexture)
{
	struct SKCreateTexturePara
	{
			SKTexture * pTexture;
			DWORD                     Usage;
			D3DFORMAT                 Format;
			D3DPOOL                   Pool;
			DWORD						LockFlag;
			IDirect3DBaseTexture9**       ppTexture;
	};
	SKCreateTexturePara CreateTexturePara;
	CreateTexturePara.pTexture = pTexture;
	CreateTexturePara.Usage = Usage;
	CreateTexturePara.Pool = Pool;
	CreateTexturePara.Format = Format;
	CreateTexturePara.LockFlag = LockFlag;
	CreateTexturePara.ppTexture = ppTexture;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateTextureCommand,
		SKCreateTexturePara,CreateTexturePara,CreateTexturePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DTEXTURE9 pDXTexture;
		HRESULT hResult = NULL;
		hResult = D3DXCreateTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),CreateTexturePara.pTexture->GetHeight(0),CreateTexturePara.pTexture->GetMipLevel(),
			CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
		SKMAC_ASSERT(!FAILED(hResult));
		*CreateTexturePara.ppTexture = pDXTexture;
		if(!((VS2DTexture *)CreateTexturePara.pTexture)->IsRenderTarget())
		{
			for (unsigned int i = 0 ; i < CreateTexturePara.pTexture->GetMipLevel() ; i++)
			{
				if (!CreateTexturePara.pTexture->GetBuffer(i))
				{
					continue;
				}
				hResult = pDXTexture->LockRect(i,&LockRect,0,CreateTexturePara.LockFlag);
				SKMAC_ASSERT(!FAILED(hResult));
				SKMemcpy(LockRect.pBits,CreateTexturePara.pTexture->GetBuffer(i),CreateTexturePara.pTexture->GetByteSize(i));
				hResult = pDXTexture->UnlockRect(i);
				SKMAC_ASSERT(!FAILED(hResult));
			}		


		}

	})
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DTEXTURE9 pDXTexture;
		HRESULT hResult = NULL;
		hResult = D3DXCreateTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),CreateTexturePara.pTexture->GetHeight(0),CreateTexturePara.pTexture->GetMipLevel(),
			CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
		SKMAC_ASSERT(!FAILED(hResult));
		*CreateTexturePara.ppTexture = pDXTexture;
		if(!((VS2DTexture *)CreateTexturePara.pTexture)->IsRenderTarget())
		{
			for (unsigned int i = 0 ; i < CreateTexturePara.pTexture->GetMipLevel() ; i++)
			{
				if (!CreateTexturePara.pTexture->GetBuffer(i))
				{
					continue;
				}
				hResult = pDXTexture->LockRect(i,&LockRect,0,CreateTexturePara.LockFlag);
				SKMAC_ASSERT(!FAILED(hResult));
				SKMemcpy(LockRect.pBits,CreateTexturePara.pTexture->GetBuffer(i),CreateTexturePara.pTexture->GetByteSize(i));
				hResult = pDXTexture->UnlockRect(i);
				SKMAC_ASSERT(!FAILED(hResult));
			}		


		}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
	
}
void SKDX9Renderer::Create1DTexture(SKTexture * pTexture,
									DWORD                     Usage,
									D3DFORMAT                 Format,
									D3DPOOL                   Pool,
									DWORD LockFlag,
									IDirect3DBaseTexture9**   ppTexture)
{
	struct SKCreateTexturePara
	{
		SKTexture * pTexture;
		DWORD                     Usage;
		D3DFORMAT                 Format;
		D3DPOOL                   Pool;
		DWORD						LockFlag;
		IDirect3DBaseTexture9**       ppTexture;
	};
	SKCreateTexturePara CreateTexturePara;
	CreateTexturePara.pTexture = pTexture;
	CreateTexturePara.Usage = Usage;
	CreateTexturePara.Pool = Pool;
	CreateTexturePara.Format = Format;
	CreateTexturePara.LockFlag = LockFlag;
	CreateTexturePara.ppTexture = ppTexture;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateTextureCommand,
		SKCreateTexturePara,CreateTexturePara,CreateTexturePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DTEXTURE9 pDXTexture;
		HRESULT hResult = NULL;
		hResult = D3DXCreateTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),CreateTexturePara.pTexture->GetHeight(0),CreateTexturePara.pTexture->GetMipLevel(),
			CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
		SKMAC_ASSERT(!FAILED(hResult));
		*CreateTexturePara.ppTexture = pDXTexture;
		
		for (unsigned int i = 0 ; i < CreateTexturePara.pTexture->GetMipLevel() ; i++)
		{
			if (!CreateTexturePara.pTexture->GetBuffer(i))
			{
				continue;
			}
			hResult = pDXTexture->LockRect(i,&LockRect,0,CreateTexturePara.LockFlag);
			SKMAC_ASSERT(!FAILED(hResult));
			SKMemcpy(LockRect.pBits,CreateTexturePara.pTexture->GetBuffer(i),CreateTexturePara.pTexture->GetByteSize(i));
			hResult = pDXTexture->UnlockRect(i);
			SKMAC_ASSERT(!FAILED(hResult));
		}		


	})
		D3DLOCKED_RECT LockRect;
	LPDIRECT3DTEXTURE9 pDXTexture;
	HRESULT hResult = NULL;
	hResult = D3DXCreateTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),CreateTexturePara.pTexture->GetHeight(0),CreateTexturePara.pTexture->GetMipLevel(),
		CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
	SKMAC_ASSERT(!FAILED(hResult));
	*CreateTexturePara.ppTexture = pDXTexture;

	for (unsigned int i = 0 ; i < CreateTexturePara.pTexture->GetMipLevel() ; i++)
	{
		if (!CreateTexturePara.pTexture->GetBuffer(i))
		{
			continue;
		}
		hResult = pDXTexture->LockRect(i,&LockRect,0,CreateTexturePara.LockFlag);
		SKMAC_ASSERT(!FAILED(hResult));
		SKMemcpy(LockRect.pBits,CreateTexturePara.pTexture->GetBuffer(i),CreateTexturePara.pTexture->GetByteSize(i));
		hResult = pDXTexture->UnlockRect(i);
		SKMAC_ASSERT(!FAILED(hResult));
	}		


	
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
void SKDX9Renderer::CreateCubeTexture(SKTexture * pTexture,
					   DWORD                     Usage,
					   D3DFORMAT                 Format,
					   D3DPOOL                   Pool,
					   DWORD						LockFlag,
					   IDirect3DBaseTexture9**   ppTexture)
{
	struct SKCreateTexturePara
	{
		SKTexture * pTexture;
		DWORD                     Usage;
		D3DFORMAT                 Format;
		D3DPOOL                   Pool;
		DWORD						LockFlag;
		IDirect3DBaseTexture9**       ppTexture;
	};
	SKCreateTexturePara CreateTexturePara;
	CreateTexturePara.pTexture = pTexture;
	CreateTexturePara.Usage = Usage;
	CreateTexturePara.Pool = Pool;
	CreateTexturePara.Format = Format;
	CreateTexturePara.LockFlag = LockFlag;
	CreateTexturePara.ppTexture = ppTexture;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateTextureCommand,
		SKCreateTexturePara,CreateTexturePara,CreateTexturePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DCUBETEXTURE9 pDXTexture;
		HRESULT hResult = NULL;
		hResult = D3DXCreateCubeTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),
			CreateTexturePara.pTexture->GetMipLevel(),CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
		SKMAC_ASSERT(!FAILED(hResult));
		*CreateTexturePara.ppTexture = pDXTexture;

		SKCubeTexture * pCubeTexture =DynamicCast<SKCubeTexture>(CreateTexturePara.pTexture);
		if(!pCubeTexture->IsRenderTarget())
		{
			for (unsigned int j = 0 ; j < CreateTexturePara.pTexture->GetMipLevel() ; j++)
			{
				for(unsigned int i = 0 ; i < 6 ; i++)
				{
					if(!pCubeTexture->GetFaceBuffer(j,i))
						continue;
					hResult = pDXTexture->LockRect((D3DCUBEMAP_FACES)(i),j,&LockRect,NULL,CreateTexturePara.LockFlag);
					SKMAC_ASSERT(!FAILED(hResult));
					SKMemcpy(LockRect.pBits,pCubeTexture->GetFaceBuffer(j,i),pCubeTexture->GetFaceByteSize(j));
					hResult = pDXTexture->UnlockRect((D3DCUBEMAP_FACES)(i),j);
					SKMAC_ASSERT(!FAILED(hResult));
				}
			}
		}

	})
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DCUBETEXTURE9 pDXTexture;
		HRESULT hResult = NULL;
		hResult = D3DXCreateCubeTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),
			CreateTexturePara.pTexture->GetMipLevel(),CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
		SKMAC_ASSERT(!FAILED(hResult));
		*CreateTexturePara.ppTexture = pDXTexture;

		SKCubeTexture * pCubeTexture =DynamicCast<SKCubeTexture>(CreateTexturePara.pTexture);
		if(!pCubeTexture->IsRenderTarget())
		{
			for (unsigned int j = 0 ; j < CreateTexturePara.pTexture->GetMipLevel() ; j++)
			{
				for(unsigned int i = 0 ; i < 6 ; i++)
				{
					if(!pCubeTexture->GetFaceBuffer(j,i))
						continue;
					hResult = pDXTexture->LockRect((D3DCUBEMAP_FACES)(i),j,&LockRect,NULL,CreateTexturePara.LockFlag);
					SKMAC_ASSERT(!FAILED(hResult));
					SKMemcpy(LockRect.pBits,pCubeTexture->GetFaceBuffer(j,i),pCubeTexture->GetFaceByteSize(j));
					hResult = pDXTexture->UnlockRect((D3DCUBEMAP_FACES)(i),j);
					SKMAC_ASSERT(!FAILED(hResult));
				}
			}
		}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END	
}
void SKDX9Renderer::CreateVolumeTexture(SKTexture * pTexture,
						 DWORD                     Usage,
						 D3DFORMAT                 Format,
						 D3DPOOL                   Pool,
						 DWORD						LockFlag,
						 IDirect3DBaseTexture9** ppTexture)
{
	struct SKCreateTexturePara
	{
		SKTexture * pTexture;
		DWORD                     Usage;
		D3DFORMAT                 Format;
		D3DPOOL                   Pool;
		DWORD						LockFlag;
		IDirect3DBaseTexture9**       ppTexture;
	};
	SKCreateTexturePara CreateTexturePara;
	CreateTexturePara.pTexture = pTexture;
	CreateTexturePara.Usage = Usage;
	CreateTexturePara.Pool = Pool;
	CreateTexturePara.Format = Format;
	CreateTexturePara.LockFlag = LockFlag;
	CreateTexturePara.ppTexture = ppTexture;

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateTextureCommand,
		SKCreateTexturePara,CreateTexturePara,CreateTexturePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		LPDIRECT3DVOLUMETEXTURE9 pDXTexture;
		HRESULT hResult = NULL;
		hResult = D3DXCreateVolumeTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),CreateTexturePara.pTexture->GetHeight(0),
			CreateTexturePara.pTexture->GetLength(0),CreateTexturePara.pTexture->GetMipLevel(),CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
		SKMAC_ASSERT(!FAILED(hResult));
		*CreateTexturePara.ppTexture = pDXTexture;
		for (unsigned int i = 0 ; i < CreateTexturePara.pTexture->GetMipLevel() ; i++)
		{
			if (!CreateTexturePara.pTexture->GetBuffer(i))
			{
				continue;
			}
			D3DLOCKED_BOX kLockBox;
			hResult = pDXTexture->LockBox(i,&kLockBox,0,CreateTexturePara.LockFlag);
			SKMAC_ASSERT(!FAILED(hResult));
			SKMemcpy(kLockBox.pBits,CreateTexturePara.pTexture->GetBuffer(i),CreateTexturePara.pTexture->GetByteSize(i));
			hResult = pDXTexture->UnlockBox(i);
			SKMAC_ASSERT(!FAILED(hResult));
		}

	})
		LPDIRECT3DVOLUMETEXTURE9 pDXTexture;
		HRESULT hResult = NULL;
		hResult = D3DXCreateVolumeTexture(m_pDevice,CreateTexturePara.pTexture->GetWidth(0),CreateTexturePara.pTexture->GetHeight(0),
			CreateTexturePara.pTexture->GetLength(0),CreateTexturePara.pTexture->GetMipLevel(),CreateTexturePara.Usage,CreateTexturePara.Format,CreateTexturePara.Pool,&pDXTexture);
		SKMAC_ASSERT(!FAILED(hResult));
		*CreateTexturePara.ppTexture = pDXTexture;
		for (unsigned int i = 0 ; i < CreateTexturePara.pTexture->GetMipLevel() ; i++)
		{
			if (!CreateTexturePara.pTexture->GetBuffer(i))
			{
				continue;
			}
			D3DLOCKED_BOX kLockBox;
			hResult = pDXTexture->LockBox(i,&kLockBox,0,CreateTexturePara.LockFlag);
			SKMAC_ASSERT(!FAILED(hResult));
			SKMemcpy(kLockBox.pBits,CreateTexturePara.pTexture->GetBuffer(i),CreateTexturePara.pTexture->GetByteSize(i));
			hResult = pDXTexture->UnlockBox(i);
			SKMAC_ASSERT(!FAILED(hResult));
		}
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void SKDX9Renderer::CreateVShaderFromString(SKVShader * pVShaderProgram,SKVProgramID *pID)
{
	struct SKCreateVShaderFromStringPara
	{
		SKVShader * pVShaderProgram;
		SKVProgramID *pID;
	};
	SKCreateVShaderFromStringPara CreateVShaderFromStringPara;
	CreateVShaderFromStringPara.pID = pID;
	CreateVShaderFromStringPara.pVShaderProgram = pVShaderProgram;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateVShaderFromStringCommand,
		SKCreateVShaderFromStringPara,CreateVShaderFromStringPara,CreateVShaderFromStringPara,SKDX9Renderer *,m_pRender,this,
	{
		m_pRender->OnLoadVShaderFromString(CreateVShaderFromStringPara.pVShaderProgram,CreateVShaderFromStringPara.pID);
	})
		OnLoadVShaderFromString(CreateVShaderFromStringPara.pVShaderProgram,CreateVShaderFromStringPara.pID);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END

}
void SKDX9Renderer::CreateVShaderFromCache(SKVShader * pVShaderProgram,SKVProgramID *pID)
{
	struct SKCreateVShaderFromCachePara
	{
		SKVShader * pVShaderProgram;
		SKVProgramID *pID;
	};
	SKCreateVShaderFromCachePara CreateVShaderFromCachePara;
	CreateVShaderFromCachePara.pID = pID;
	CreateVShaderFromCachePara.pVShaderProgram = pVShaderProgram;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreateVShaderFromStringCommand,
		SKCreateVShaderFromCachePara,CreateVShaderFromCachePara,CreateVShaderFromCachePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateVertexShader((DWORD*)CreateVShaderFromCachePara.pVShaderProgram->GetCacheBuffer(),&CreateVShaderFromCachePara.pID->m_pVertexShader );
		SKMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreateVertexShader((DWORD*)CreateVShaderFromCachePara.pVShaderProgram->GetCacheBuffer(),&CreateVShaderFromCachePara.pID->m_pVertexShader );
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void SKDX9Renderer::CreatePShaderFromString(SKPShader* pPShaderProgram,SKPProgramID *pID)
{
	struct SKCreatePShaderFromStringPara
	{
		SKPShader * pPShaderProgram;
		SKPProgramID *pID;
	};
	SKCreatePShaderFromStringPara CreatePShaderFromStringPara;
	CreatePShaderFromStringPara.pID = pID;
	CreatePShaderFromStringPara.pPShaderProgram = pPShaderProgram;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreatePShaderFromStringCommand,
		SKCreatePShaderFromStringPara,CreatePShaderFromStringPara,CreatePShaderFromStringPara,SKDX9Renderer *,m_pRender,this,
	{
		m_pRender->OnLoadPShaderFromString(CreatePShaderFromStringPara.pPShaderProgram,CreatePShaderFromStringPara.pID);
	})
		OnLoadPShaderFromString(CreatePShaderFromStringPara.pPShaderProgram,CreatePShaderFromStringPara.pID);
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}
void SKDX9Renderer::CreatePShaderFromCache(SKPShader* pPShaderProgram,SKPProgramID *pID)
{
	struct SKCreatePShaderFromCachePara
	{
		SKPShader * pPShaderProgram;
		SKPProgramID *pID;
	};
	SKCreatePShaderFromCachePara CreatePShaderFromCachePara;
	CreatePShaderFromCachePara.pID = pID;
	CreatePShaderFromCachePara.pPShaderProgram = pPShaderProgram;
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKCreatePShaderFromStringCommand,
		SKCreatePShaderFromCachePara,CreatePShaderFromCachePara,CreatePShaderFromCachePara,LPDIRECT3DDEVICE9,m_pDevice,m_pDevice,
	{
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreatePixelShader((DWORD*)CreatePShaderFromCachePara.pPShaderProgram->GetCacheBuffer(),&CreatePShaderFromCachePara.pID->m_pPixelShader);
		SKMAC_ASSERT(!FAILED(hResult));
	})
		HRESULT hResult = NULL;
		hResult = m_pDevice->CreatePixelShader((DWORD*)CreatePShaderFromCachePara.pPShaderProgram->GetCacheBuffer(),&CreatePShaderFromCachePara.pID->m_pPixelShader);
		SKMAC_ASSERT(!FAILED(hResult));
	ENQUEUE_UNIQUE_RENDER_COMMAND_END
}