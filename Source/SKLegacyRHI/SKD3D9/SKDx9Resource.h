#ifndef SKDX9RESOURCE_H
#define SKDX9RESOURCE_H
#ifdef SKDX9RENDERER_EXPORTS
#define  SKDX9RENDERER_API __declspec(dllexport) 
#else
#define  SKDX9RENDERER_API __declspec(dllimport)
#endif
#include <SKd3d9.h>
#include <SKd3dx9tex.h>
#include "SKBind.h"
namespace SKEngine2
{
	
	class  SKDX9RENDERER_API SKVProgramID : public SKResourceIdentifier
	{
	public:
		SKVProgramID()
		{
			m_pVertexShader = NULL;
		}
		virtual ~SKVProgramID()
		{
			SKMAC_RELEASE(m_pVertexShader)
			
		}
		IDirect3DVertexShader9*      m_pVertexShader;  //Ⱦ
		
	};
	class  SKDX9RENDERER_API SKPProgramID : public SKResourceIdentifier
	{
	public:
		SKPProgramID()
		{
			m_pPixelShader = NULL;

		}
		virtual ~SKPProgramID()
		{
			SKMAC_RELEASE(m_pPixelShader)

		}
		IDirect3DPixelShader9*      m_pPixelShader;  //Ⱦ

	};

	class  SKDX9RENDERER_API SKRenderTargetID : public SKResourceIdentifier
	{
	public:
		SKRenderTargetID()
		{
			m_pSaveRenderTarget = NULL;
			m_pRenderTarget = NULL;
			m_pTextureSurface = NULL;
		}
		~SKRenderTargetID()
		{
			SKMAC_RELEASE(m_pRenderTarget);
			SKMAC_RELEASE(m_pSaveRenderTarget);
			SKMAC_RELEASE(m_pTextureSurface);
		}
		LPDIRECT3DSURFACE9			m_pTextureSurface;
		LPDIRECT3DSURFACE9			m_pRenderTarget;
		LPDIRECT3DSURFACE9			m_pSaveRenderTarget;
	};
	class  SKDX9RENDERER_API SKTextureID : public SKResourceIdentifier
	{
	public:
		SKTextureID()
		{
			
			m_pTexture = NULL;
			
		}
		virtual ~SKTextureID()
		{
			
			SKMAC_RELEASE(m_pTexture);
			
			
		}
		IDirect3DBaseTexture9*      m_pTexture;	
		
	};
	class SKDX9RENDERER_API SKDepthStencilID : public SKResourceIdentifier
	{
	public:
		SKDepthStencilID()
		{
			m_pDepthStencilBuffer = NULL;
			m_pSaveDepthStencilBuffer = NULL;
		}
		virtual ~SKDepthStencilID()
		{
			SKMAC_RELEASE(m_pDepthStencilBuffer);
			SKMAC_RELEASE(m_pSaveDepthStencilBuffer);
		}
		LPDIRECT3DSURFACE9	m_pDepthStencilBuffer;
		LPDIRECT3DSURFACE9	m_pSaveDepthStencilBuffer;
	};
	class SKDX9RENDERER_API SKVBufferID : public SKResourceIdentifier
	{
	public:
		SKVBufferID()
		{
			m_pVertexBuffer = NULL;
		}
		virtual ~SKVBufferID()
		{
			SKMAC_RELEASE(m_pVertexBuffer);
		}
		IDirect3DVertexBuffer9* m_pVertexBuffer;
	
	};
	class SKDX9RENDERER_API SKVBufferFormatID : public SKResourceIdentifier
	{
	public:
		SKVBufferFormatID()
		{

			m_pDeclaration = NULL;
		}
		virtual ~SKVBufferFormatID()
		{
			SKMAC_RELEASE(m_pDeclaration);
		}
		IDirect3DVertexDeclaration9* m_pDeclaration;
	
	};
	class SKDX9RENDERER_API SKIBufferID : public SKResourceIdentifier
	{
	public:
		SKIBufferID()
		{
			m_IndexBuffer = NULL;
		}
		virtual ~SKIBufferID()
		{
			SKMAC_RELEASE(m_IndexBuffer);

		}
		LPDIRECT3DINDEXBUFFER9 m_IndexBuffer;
	};
}
#endif