#ifndef SKDX11RESOURCE_H
#define SKDX11RESOURCE_H
#ifdef SKDX11RENDERER_EXPORTS
#define  SKDX11RENDERER_API __declspec(dllexport) 
#else
#define  SKDX11RENDERER_API __declspec(dllimport)
#endif
#include <d3d11.h>
#include "SKBind.h"
namespace SKEngine2
{
	class  SKDX11RENDERER_API SKSamplerStateID : public SKResourceIdentifier
	{
	public:
		SKSamplerStateID()
		{
			m_pSamplerState = NULL;
		}

		virtual ~SKSamplerStateID()
		{
			SKMAC_RELEASE(m_pSamplerState)
		}
		ID3D11SamplerState* m_pSamplerState;

	};
	class  SKDX11RENDERER_API SKBlendStateID : public SKResourceIdentifier
	{
	public:
		SKBlendStateID()
		{
			m_pBlendState = NULL;
		}

		virtual ~SKBlendStateID()
		{
				SKMAC_RELEASE(m_pBlendState)
		}
		ID3D11BlendState* m_pBlendState;

	};
	class  SKDX11RENDERER_API SKRasterStateID : public SKResourceIdentifier
	{
	public:
		SKRasterStateID()
		{
			m_pRasterState = NULL;
		}
		virtual ~SKRasterStateID()
		{
			SKMAC_RELEASE(m_pRasterState)

		}
		ID3D11RasterizerState* m_pRasterState;  //Ⱦ

	};
	class  SKDX11RENDERER_API SKDepthStencilStateID : public SKResourceIdentifier
	{
	public:
		SKDepthStencilStateID()
		{
			m_pDepthStencilState = NULL;
		}
		virtual ~SKDepthStencilStateID()
		{
			SKMAC_RELEASE(m_pDepthStencilState)

		}
		ID3D11DepthStencilState* m_pDepthStencilState;  //Ⱦ

	};
	class  SKDX11RENDERER_API SKVProgramID : public SKResourceIdentifier
	{
	public:
		SKVProgramID()
		{
			m_pVertexShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~SKVProgramID()
		{
			SKMAC_RELEASE(m_pVertexShader)
			SKMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11VertexShader*      m_pVertexShader;  //Ⱦ
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  SKDX11RENDERER_API SKPProgramID : public SKResourceIdentifier
	{
	public:
		SKPProgramID()
		{
			m_pPixelShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~SKPProgramID()
		{
			SKMAC_RELEASE(m_pPixelShader)
			SKMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11PixelShader*      m_pPixelShader;  //Ⱦ
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  SKDX11RENDERER_API SKGProgramID : public SKResourceIdentifier
	{
	public:
		SKGProgramID()
		{
			m_pGeometryShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~SKGProgramID()
		{
			SKMAC_RELEASE(m_pGeometryShader)
				SKMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11GeometryShader*      m_pGeometryShader;  //Ⱦ
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  SKDX11RENDERER_API SKDProgramID : public SKResourceIdentifier
	{
	public:
		SKDProgramID()
		{
			m_pDomainShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~SKDProgramID()
		{
			SKMAC_RELEASE(m_pDomainShader)
				SKMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11DomainShader*      m_pDomainShader;  //Ⱦ
		ID3D11Buffer*			m_pShaderBuffer;
	};
	class  SKDX11RENDERER_API SKHProgramID : public SKResourceIdentifier
	{
	public:
		SKHProgramID()
		{
			m_pHullShader = NULL;
			m_pShaderBuffer = NULL;
		}
		virtual ~SKHProgramID()
		{
			SKMAC_RELEASE(m_pHullShader)
				SKMAC_RELEASE(m_pShaderBuffer)
		}
		ID3D11HullShader*      m_pHullShader;  //Ⱦ
		ID3D11Buffer*			m_pShaderBuffer;
	};
// 
	class  SKDX11RENDERER_API SKRenderTargetID : public SKResourceIdentifier
	{
	public:
		SKRenderTargetID()
		{
			m_pSaveRenderTarget = NULL;
			m_pSaveDepthStencilBuffer = NULL;
			m_pRenderTarget = NULL;
			m_pTextureSurface = NULL;
			m_pOldTexture = NULL;
			m_pStagTexture = NULL;
		}
		~SKRenderTargetID()
		{
			SKMAC_RELEASE(m_pRenderTarget);
			SKMAC_RELEASE(m_pTextureSurface);
			SKMAC_RELEASE(m_pStagTexture);
			m_pOldTexture = NULL;
			m_pSaveRenderTarget = NULL;
			m_pSaveDepthStencilBuffer = NULL;
		}
		ID3D11Resource*				m_pOldTexture;
		ID3D11Resource*				m_pTextureSurface;
		ID3D11RenderTargetView *	m_pRenderTarget;
		ID3D11RenderTargetView *	m_pSaveRenderTarget;
		ID3D11DepthStencilView *	m_pSaveDepthStencilBuffer;
		ID3D11Resource*				m_pStagTexture;
	};
	class  SKDX11RENDERER_API SKTextureID : public SKResourceIdentifier
	{
	public:
		SKTextureID()
		{
			
			m_pTexture = NULL;
			m_pShaderResourceView = NULL;
		}
		virtual ~SKTextureID()
		{
			SKMAC_RELEASE(m_pShaderResourceView);
			SKMAC_RELEASE(m_pTexture);		
		}

		ID3D11Resource*      m_pTexture;
		ID3D11ShaderResourceView* m_pShaderResourceView;
	};
	class SKDX11RENDERER_API SKDepthStencilID : public SKResourceIdentifier
	{
	public:
		SKDepthStencilID()
		{
			m_pTexture = NULL;
			m_pDepthStencilBuffer = NULL;
			m_pSaveDepthStencilBuffer = NULL;
		}
		virtual ~SKDepthStencilID()
		{
			SKMAC_RELEASE(m_pDepthStencilBuffer);
			SKMAC_RELEASE(m_pTexture);
			m_pSaveDepthStencilBuffer = NULL;
		}
		ID3D11Resource*      m_pTexture;
		ID3D11DepthStencilView *	m_pDepthStencilBuffer;
		ID3D11DepthStencilView *	m_pSaveDepthStencilBuffer;
	};
	class SKDX11RENDERER_API SKVBufferID : public SKResourceIdentifier
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
		ID3D11Buffer* m_pVertexBuffer;
	
	};
	class SKDX11RENDERER_API SKVBufferFormatID : public SKResourceIdentifier
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
		ID3D11InputLayout* m_pDeclaration;
	
	};
	class SKDX11RENDERER_API SKIBufferID : public SKResourceIdentifier
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
		ID3D11Buffer* m_IndexBuffer;
	};
}
#endif