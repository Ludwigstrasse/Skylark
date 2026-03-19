#ifndef SKDX9RENDERER_H
#define SKDX9RENDERER_H
#ifdef SKDX9RENDERER_EXPORTS
#define  SKDX9RENDERER_API __declspec(dllexport) 
#else
#define  SKDX9RENDERER_API __declspec(dllimport)
#endif
#include <SKd3d9.h>
#include <SKd3dx9tex.h>
#include "SKRenderer.h"
#include "SKResourceManager.h"

namespace SKEngine2
{
	#define SKMAC_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
	class SKDx9ShaderInclude;
	class SKVProgramID;
	class SKPProgramID;
	class SKDX9RENDERER_API SKDX9Renderer : public SKRenderer
	{
	public:
		SKDX9Renderer(HWND hMainWindow,unsigned int uiScreenWidth,unsigned int uiScreenHeight,bool bIsWindowed,
					unsigned int uiAnisotropy,unsigned int uiMultisample,
					ChildWindowInfo * pChildWindow,int uiNumChildWindow);
		virtual ~SKDX9Renderer();
		virtual int GetRendererType () const;
		virtual const TCHAR * GetRendererStringType() const;
		virtual void ClearBackBuffer ();
		virtual void ClearZBuffer ();
		virtual void ClearStencilBuffer ();
		virtual void ClearBuffers ();
		virtual void ClearBuffers(unsigned int uiClearFlag);
		//virtual void DisplayBackBuffer ();
		virtual void DeviceLost();
		virtual void ResetDevice();
		virtual bool ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow);

		virtual void ClearBackBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearZBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearStencilBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers (int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers(unsigned int uiClearFlag,int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual unsigned int GetTextureCompressLen(unsigned int uiLen);	

		virtual bool CooperativeLevel();
		virtual bool BeginRendering();
		virtual bool EndRendering();
		virtual bool DrawMesh(SKGeometry * pGeometry, SKRenderState * pRenderState, SKVShader * pVShader, SKPShader * pPShader, SKGShader * pGShader);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
								SKUSHORT_INDEX * pIndexBuffer,unsigned int uiIndexNum);
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
								SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
	protected:
		
// 		virtual bool SetVProgramConstant (SKString &NameInShaderProgram,void * pDate,unsigned int uiSize);
// 		virtual bool SetPProgramConstant (SKString &NameInShaderProgram,void * pDate,unsigned int uiSize);
		virtual bool SetVProgramConstant (unsigned int uiStartRegister,void * pDate,
											unsigned int RegisterNum,unsigned int uiType);
		virtual bool SetPProgramConstant (unsigned int uiStartRegister,void * pDate,
											unsigned int RegisterNum,unsigned int uiType);
		//Load Release
		virtual bool OnLoadVShaderProgram (SKVShader * pVShaderProgram,SKResourceIdentifier *&pID);
		virtual bool OnReleaseVShaderProgram(SKResourceIdentifier * pVShaderProgramID);

		virtual bool OnLoadPShaderProgram (SKPShader* pPShaderProgram,SKResourceIdentifier *&pID);
		virtual bool OnReleasePShaderProgram (SKResourceIdentifier* pPShaderProgramID);

		virtual bool OnLoadGShaderProgram(SKGShader * pGShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleaseGShaderProgram(SKResourceIdentifier * pGShaderProgramID);

		virtual bool OnLoadDShaderProgram(SKDShader * pDShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleaseDShaderProgram(SKResourceIdentifier * pDShaderProgramID);

		virtual bool OnLoadHShaderProgram(SKHShader * pHShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleaseHShaderProgram(SKResourceIdentifier * pHShaderProgramID);

		virtual bool OnLoadTexture (SKTexture * pTexture,SKResourceIdentifier *&pID);
		virtual bool OnReleaseTexture (SKResourceIdentifier * pTextureID);

		virtual bool OnLoadDepthStencil (SKDepthStencil * pDepthStencil,SKResourceIdentifier *&pID);
		virtual bool OnReleaseDepthStencil (SKResourceIdentifier * pDepthStencilID);
		
		virtual bool OnLoadRenderTarget (SKRenderTarget * pRenderTarget,SKResourceIdentifier *&pID);
		virtual bool OnReleaseRenderTarget (SKResourceIdentifier * pRenderTarget);

		virtual bool OnLoadVBufferDate (SKVertexBuffer * pVBuffer,SKResourceIdentifier *&pID);
		virtual bool OnReleaseVBufferDate (SKResourceIdentifier* pVBufferID);

		virtual bool OnLoadVBufferFormat (SKVertexFormat * pVertexFormat,SKResourceIdentifier *&pID);
		virtual bool OnReleaseVBufferFormat (SKResourceIdentifier* pVBufferID);

		virtual bool OnLoadIBuffer (SKIndexBuffer* pIBuffer,SKResourceIdentifier *&pID);
		virtual bool OnReleaseIBuffer (SKResourceIdentifier* pIBufferID);

		virtual bool OnLoadBlendState (SKBlendState * pBlendState,SKResourceIdentifier *&pID);
		virtual bool OnReleaseBlendState  (SKResourceIdentifier * pBlendStateID);

		virtual bool OnLoadDepthStencilState (SKDepthStencilState * pDepthStencilState,SKResourceIdentifier *&pID);
		virtual bool OnReleaseDepthStencilState  (SKResourceIdentifier * pDepthStencilStateID);

		virtual bool OnLoadRasterizerState(SKRasterizerState * pRasterizerState,SKResourceIdentifier *&pID);
		virtual bool OnReleaseRasterizerState  (SKResourceIdentifier * pRasterizerStateID);

		virtual bool OnLoadSamplerState(SKSamplerState * pRasterizerState,SKResourceIdentifier *&pID);
		virtual bool OnReleaseSamplerState  (SKResourceIdentifier * pRasterizerStateID);

		virtual bool SetVShaderProgram(SKVShader*	pVShaderProgram);
		virtual bool SetPShaderProgram(SKPShader*	pPShaderProgram);
		virtual bool SetGShaderProgram(SKGShader*	pGShaderProgram);
		virtual bool SetDShaderProgram(SKDShader*	pDShaderProgram);
		virtual bool SetHShaderProgram(SKHShader*	pHShaderProgram);

		virtual unsigned int SetVertexFormat(SKVertexFormat * pVertexFormat);

		

		
		virtual void SetVShaderConstant(SKVShader * pShader);
		virtual void SetVShaderSampler(SKVShader * pShader);
		virtual void SetPShaderConstant(SKPShader * pShader);
		virtual void SetPShaderSampler(SKPShader * pShader);
		virtual void SetGShaderConstant(SKGShader * pShader);
		virtual void SetGShaderSampler(SKGShader * pShader);
		virtual void SetDShaderConstant(SKDShader * pShader);
		virtual void SetDShaderSampler(SKDShader * pShader);
		virtual void SetHShaderConstant(SKHShader * pShader);
		virtual void SetHShaderSampler(SKHShader * pShader);
		D3DXMACRO * GetDefine(SKMap<SKString,SKString> & Define);


		virtual void * Lock(SKVertexBuffer * pVertexBuffer);
		virtual void UnLock(SKVertexBuffer * pVertexBuffer);
		virtual void * Lock(SKIndexBuffer * pIndexBuffer);
		virtual void UnLock(SKIndexBuffer * pIndexBuffer);
		virtual void * Lock(SKTexture * pTexture,unsigned int uiLevel,unsigned int uiFace);
		virtual void UnLock(SKTexture * pTexture,unsigned int uiLevel,unsigned int uiFace);
		virtual void * Lock(SKRenderTarget * pRenderTarget);
		virtual void UnLock(SKRenderTarget * pRenderTarget);

		virtual void * Lock(SKDepthStencil * pDepthStencil);
		virtual void UnLock(SKDepthStencil * pDepthStencil);
		virtual bool CopyResourceBuffer(VS2DTexture * pSource,SKCubeTexture * pDest,unsigned int uiFace);
	private:
		void OnLoadVShaderFromString(SKVShader * pVShaderProgram,SKVProgramID *pID);
		void OnLoadPShaderFromString(SKPShader* pPShaderProgram,SKPProgramID *pID);
	private:
// 		DX9_API_FUN_TWOPARAMETER(SetRenderState,D3DRENDERSTATETYPE,State,DWORD,Value)
// 		DX9_API_FUN_END
 		bool SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);
 		bool SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
		bool Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,SKREAL Z,DWORD Stencil);
		bool SetClipPlane(DWORD Index,CONST SKREAL* pPlane);
		bool SetScissorRect(CONST RECT* pRect);
		bool SetViewport(CONST D3DVIEWPORT9* pViewport);
		bool SetVertexShaderConstant(unsigned int uiStartRegister,void * pDate,
									unsigned int RegisterNum,unsigned int uiType);
		bool SetPixelShaderConstant(unsigned int uiStartRegister,void * pDate,
			unsigned int RegisterNum,unsigned int uiType);
		bool DrawDynamicBufferMesh(SKDynamicBufferGeometry *pDBGeometry);
		bool DrawMesh(SKGeometry * pGeometry);
		bool DrawMesh1(SKGeometry * pGeometry);
		bool DrawScreenEX2(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
			SKUSHORT_INDEX * pIndexBuffer,unsigned int uiNumTri);
		bool DrawScreenEX1(SCREEN_QUAD_TYPE ScreenQuad[4]);
		
		bool DrawScreenFontEX(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
			SKUSHORT_INDEX * pIndexBuffer, unsigned int uiNumTri);
		bool BeginRender();
		bool EndRender();
		bool UseWindowEx(int uiWindowID);


		//resource 

		bool SetVertexShader(IDirect3DVertexShader9** pShader);
		bool SetPixelShader(IDirect3DPixelShader9** pShader);
		bool SetTexture(DWORD Stage,IDirect3DBaseTexture9** pTexture);
		bool SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** pStreamData,UINT OffsetInBytes,UINT Stride);
		bool SetVertexDeclaration(IDirect3DVertexDeclaration9** pDecl);
		bool SetIndices(IDirect3DIndexBuffer9** pIndexData);
		bool SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget);
		bool SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil);
		bool SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget,IDirect3DSurface9** ppRenderTarget);
		bool EndRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** pRenderTarget);
		bool SetDepthStencilSurface(IDirect3DSurface9** pNewZStencil,IDirect3DSurface9** ppZStencilSurface);
		bool EndDepthStencilSurface(IDirect3DSurface9** pNewZStencil);

		bool GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
		bool GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
		bool StretchRect(IDirect3DSurface9** pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9** pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
		bool CopyResourceBuffer(unsigned int uiLevel,LPDIRECT3DTEXTURE9 * pSource, LPDIRECT3DCUBETEXTURE9 * pDest,CONST RECT* pRect,D3DTEXTUREFILTERTYPE Filter ,D3DCUBEMAP_FACES CubFace);
		
		void CreateIndexBuffer(unsigned int Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,void * pCopyBuffer,DWORD LockFlag);
		void CreateVertexBuffer(UINT Length,DWORD Usage,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,DWORD Flags,SKVertexBuffer * pVBuffer,unsigned int uiOneVextexSize);
		void CreateVertexDeclaration(const SKArray<D3DVERTEXELEMENT9> & Element, IDirect3DVertexDeclaration9** ppDecl);
		void CreateDepthStencilSurface(D3DFORMAT FormatType ,D3DMULTISAMPLE_TYPE MultiSampleTypes,
			unsigned int uiWidth ,unsigned int uiHeight,IDirect3DSurface9** ppSurface);
		void GetSurfaceLevel(LPDIRECT3DTEXTURE9 * pTexture,unsigned int uiLevel,IDirect3DSurface9** ppSurfaceLevel);
		void GetCubeMapSurface(LPDIRECT3DCUBETEXTURE9 * pCubTexture,D3DCUBEMAP_FACES FaceType,UINT Level,IDirect3DSurface9** ppCubeMapSurface);
		void CreateRenderTarget(D3DFORMAT FormatType,D3DMULTISAMPLE_TYPE MultiSampleType,unsigned int uiWidth, unsigned int uiHeight,IDirect3DSurface9** ppSurface);
		void Create2DTexture(SKTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void Create1DTexture(SKTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void CreateCubeTexture(SKTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		void CreateVolumeTexture(SKTexture * pTexture,
			DWORD                     Usage,
			D3DFORMAT                 Format,
			D3DPOOL                   Pool,
			DWORD						LockFlag,
			IDirect3DBaseTexture9**   ppTexture);
		
		
		void CreateVShaderFromString(SKVShader * pVShaderProgram,SKVProgramID *pID);
		void CreateVShaderFromCache(SKVShader * pVShaderProgram,SKVProgramID *pID);

		void CreatePShaderFromString(SKPShader* pPShaderProgram,SKPProgramID *pID);
		void CreatePShaderFromCache(SKPShader* pPShaderProgram,SKPProgramID *pID);

		void GetShaderInstruction(ID3DXBuffer * pShader,unsigned int &uiArithmeticInstructionSlots ,unsigned int &uiTextureInstructionSlots);
	public:


		//Enable Disable
		virtual void SetTexAllState(SKTexAllState * pTexAllState,unsigned int i);
		virtual unsigned int SetVShader(SKVShader * pVShader);
		virtual unsigned int SetPShader(SKPShader * pPShader);
		virtual unsigned int SetGShader(SKGShader * pGShader);
		virtual unsigned int SetDShader(SKDShader * pDShader);
		virtual unsigned int SetHShader(SKHShader * pHShader);

		virtual unsigned int SetVBuffer(SKVertexBuffer * pVBuffer);
		virtual unsigned int SetIBuffer (SKIndexBuffer * pIBuffer);
		virtual unsigned int SetTexture (SKTexture* pTexture,unsigned int i);
		virtual unsigned int SetVTexture (SKTexture* pTexture,unsigned int i);
		virtual unsigned int SetGTexture(SKTexture* pTexture, unsigned int i);
		virtual unsigned int SetDTexture(SKTexture* pTexture, unsigned int i);
		virtual unsigned int SetHTexture(SKTexture* pTexture, unsigned int i);

		virtual unsigned int SetSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetVSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetGSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetDSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetHSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);

		virtual unsigned int SetBlendState(SKBlendState * pBlendState, bool bForceSet = false);
		virtual unsigned int SetDepthStencilState(SKDepthStencilState * pDepthStencilState, bool bForceSet = false);
		virtual unsigned int SetRasterizerState(SKRasterizerState * pRasterizerState, bool bForceSet = false);
		virtual void SetClipPlane(SKArray<SKPlane3> &Plane,bool bEnable);
		virtual void SetScissorRect(SKArray<SKRect2> &Rect,bool bEnable);

		virtual bool SetRenderTarget(SKRenderTarget *pRenderTarget,unsigned int i);
		virtual bool EndRenderTarget(SKRenderTarget *pRenderTarget,unsigned int i);
		virtual bool SetDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer);
		virtual bool EndDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer);
		virtual bool SetViewPort(SKViewPort *pViewPort = NULL);
		virtual bool UseWindow(int uiWindowID);
		
		virtual const SKAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum);
		virtual bool IsSupportMulBufferSwtich()const;
		virtual bool IsSupportGS()const;
	protected:
		
		static SKDx9ShaderInclude * ms_pDx9IncludeShader;
		static LPDIRECT3D9 ms_pMain;
		LPDIRECT3DDEVICE9 m_pDevice;
		LPDIRECT3DSWAPCHAIN9  * m_pChain;	
		LPDIRECT3DSWAPCHAIN9  m_pMainChain;
		D3DPRESENT_PARAMETERS m_Present;

		LPDIRECT3DSURFACE9	m_pMainDepthStencilBuffer;
		LPDIRECT3DSURFACE9*	m_pChainnDepthStencilBuffer;

		static DWORD ms_dwAlphaBlend[];
		static DWORD ms_dwAlphaBlendOP[];
		static DWORD ms_dwCullType[];



		static DWORD ms_dwStencilOperation[];

		static DWORD ms_dwCompare[];

		static DWORD ms_dwTextureFormatType[];
		static DWORD ms_dwTextureFilterType[];
		static DWORD ms_dwTextureAddrType[];

		static DWORD ms_dwPrimitiveType[];
		static DWORD ms_dwDevTypes[]; 
		static DWORD ms_dwMultiSampleTypes[]; 
		static DWORD ms_dwCubeMapFace[];
		static DWORD ms_dwLockFlag[];
		static DWORD ms_dwVertexFormatDateType[];
		static DWORD ms_dwVertexFormatSemantics[];

		static TCHAR ms_cVertexShaderProgramVersion[];
		static TCHAR ms_cPixelShaderProgramVersion[];

		enum
		{
			MAX_SHADER_BUFFER = 1024,
			MAX_TEXTURE_BUFFER = 16
		};
		SKREAL	m_fFloatShaderBuffer[MAX_SHADER_BUFFER];
		int		m_iIntShaderBuffer[MAX_SHADER_BUFFER];
		bool	m_bBoolShaderBuffer[MAX_SHADER_BUFFER];
		SKTexAllState * m_pTexAllStateBuffer[MAX_TEXTURE_BUFFER];

		bool m_bSRGB[TEXLEVEL];
		//PRIORITY
		DECLARE_PRIORITY
		DECLARE_INITIAL_ONLY
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		enum	//Register Type
		{
			RT_B,
			RT_C,
			RT_S,
			RT_MAX
		};
		//function in d3d  so output is string
		virtual SKString TexColorBuffer(const SKColorBuffer * pColorBuffer) const;
		virtual SKString Tex2D(const VS2DTexSampler * p2DTexSampler) const;
		virtual SKString Tex1D(const VS1DTexSampler * p1DTexSampler) const;
		virtual SKString Mul(const SKMul * pMul) const;
		virtual SKString Dot(const SKDot * pDot) const;
		virtual SKString Sub(const SKSub * pSub) const;
		virtual SKString Add(const SKAdd * pAdd) const;
		virtual SKString Div(const SKDiv * pDiv) const;
		virtual SKString MakeValue(const SKMakeValue * pMakeValue) const;
		virtual SKString Saturate(const SKSaturate * pSaturate) const;
		//
		virtual SKString Sampler(unsigned int uiType)const;
		virtual SKString Float()const;
		virtual SKString Float2()const;
		virtual SKString Float3()const;
		virtual SKString Float4()const;
		virtual SKString Return()const;
		virtual SKString FloatConst(const SKString & Value1)const;
		virtual SKString Float2Const(const SKString & Value1,const SKString & Value2)const;
		virtual SKString Float3Const(const SKString & Value1,
			const SKString & Value2,const SKString & Value3)const;
		virtual SKString Float4Const(const SKString & Value1,const SKString & Value2,
			const SKString & Value3,const SKString & Value4)const;

		virtual void CustomPointLightUseString(SKString & PointLightUseString ,SKString & PointAttenuationDivString);
		virtual void CustomSpotLightUseString(SKString & SpotLightUseString ,SKString & SpotAttenuationDivString);

		virtual SKString GetDeclareValue(const SKString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const;
		virtual SKString GetDeclareSampler(const SKString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const;

		virtual SKString SetRegister(unsigned int uiRegisterType,unsigned int uiRegisterIndex)const;
		virtual SKString GetValueElement(const SKPutNode * pPutNode,
										unsigned char uiVE)const;
		virtual SKString GetValueElement(const SKString & InputString,
			unsigned char uiVE)const;

		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString);
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString);
		virtual void CreateVUserConstant(SKVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString);
		virtual void CreateVFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
									SKString & OutString);	
		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,SKVShader *pVShader);


		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString);
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString);
		virtual void CreatePUserConstant(SKPShader* pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString);
		virtual void CreatePFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
									SKString & OutString);	
		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,SKPShader *pPShader);

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateGUserConstant(SKGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKGShader *pGShader);
		/************************************************************************************/
		virtual void GetIncludeShader(SKString &OutString);

		virtual void GetDynamicShader(SKString &OutString);
		virtual void SkyLight(const SKString &WorldNormal,const SKString &UpColor,const SKString & DownColor,const SKString & DiffuseColor,SKString & OutString);
		//fun is custorm output is paramter
		virtual void DirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
			const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,SKString & OutString)const;

		virtual void CustomDirectionalLight(int iLightNum,const SKString &CustomString,SKArray<SKString> ShadowString,SKString & OutString)const;

		virtual void OrenNayarDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
					const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
					SKString & OutString)const;
		virtual void MinnaertDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
											const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
											SKString & OutString)const;

		virtual void StraussDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
												const SKString &Metalness,const SKString &Transparency,const SKString &K,
												const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
												SKString & OutString)const;
		virtual void ShirleyDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
											const SKString &NU,const SKString &NV,const SKString &Rs,
											const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
											SKString & OutString)const ;
		virtual void SchlickDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
											const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
											SKString & OutString)const;
		virtual void CookTorranceDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
			const SKString &Roughness , const SKString &C , const SKString &R,
			const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void IsotropicWardDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
			const SKString &Roughness ,
			const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void AnisotropicWardDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
			const SKString &RoughnessX ,const SKString &RoughnessY ,
			const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
			SKString & OutString)const;

		virtual void PointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const;
		virtual void CustomPointLight(int iLightNum,const SKString &CustomString,const SKString & WorldPos,SKArray<SKString> ShadowString,SKString & OutString)const;

		virtual void OrenNayarPointLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void MinnaertPointLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
										SKString & OutString)const;
		
		virtual void StraussPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
										const SKString &Metalness,const SKString &Transparency,const SKString &K,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
										SKString & OutString)const;

		virtual void ShirleyPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
										const SKString &NU,const SKString &NV,const SKString &Rs,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
										SKString & OutString)const;
		virtual void SchlickPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
										SKString & OutString)const;
		virtual void CookTorrancePointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness , const SKString &C , const SKString &R,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void IsotropicWardPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void AnisotropicWardPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &RoughnessX ,const SKString &RoughnessY ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
			SKString & OutString)const;


		virtual void SpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const;
		virtual void CustomSpotLight(int iLightNum,const SKString &CustomString,const SKString & WorldPos,SKArray<SKString> ShadowString,SKString & OutString)const;

		virtual void OrenNayarSpotLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
										SKString & OutString)const;
		virtual void MinnaertSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
										SKString & OutString)const;

		virtual void StraussSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
										const SKString &Metalness,const SKString &Transparency,const SKString &K,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
										SKString & OutString)const;
		virtual void ShirleySpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
										const SKString &NU,const SKString &NV,const SKString &Rs,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
										SKString & OutString)const;
		virtual void SchlickSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
										const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
										SKString & OutString)const ;
		virtual void CookTorranceSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness , const SKString &C , const SKString &R,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void IsotropicWardSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void AnisotropicWardSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &RoughnessX ,const SKString &RoughnessY ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
			SKString & OutString)const;

		virtual void LocalToWorldPos(const SKString & LocalPos,SKString &OutString)const;
		virtual void WorldToViewPos(const SKString & WorldPos,SKString &OutString)const;
		virtual void LocalToWorldNormal(const SKString & LocalNormal,SKString &OutString)const;
		virtual void WorldToViewNormal(const SKString & WorldNormal,SKString &OutString)const;
		virtual void LocalToViewNormal(const SKString & LocalNormal,SKString &OutString)const;
		virtual void GetWorldViewDir(SKString &OutString)const;
		virtual void TransProjPos(const SKString & Pos,const SKString & Matrix,SKString &OutString)const;
		virtual void BumpNormal(const SKString &TexNormal,SKString &OutString)const;

		virtual void ComputeDir(const SKString & Orgin,const SKString & End,SKString &OutString)const;
		virtual void ComputeLength(const SKString & Orgin,const SKString & End,SKString &OutString)const;
		virtual void GetLightType(unsigned int i,SKString &OutString)const;
		virtual void GetLightTypeTemp(SKString &OutString)const;
		virtual void ComputeBoneVector(const SKString& BoneIndex, const SKString & BoneWeight,const SKString &BoneVector,
			 const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const;

		virtual void BoneTranPos( const SKString & LocalPos,const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const;
		virtual void BoneTranNormal( const SKString & LocalNormal,const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const;

		virtual void DecodeNormal1(const SKString & Normal,SKString &OutString) const;
		virtual void RecodeNormal1(const SKString & Normal,SKString &OutString) const;
		virtual void DecodeNormal2(const SKString & Normal,const SKString & Tengent,SKString &OutString) const;
		virtual void DecodeNormal3(const SKString & Normal,SKString &OutString) const;
		virtual void RecodeNormal3(const SKString & Normal,SKString &OutString) const;
		virtual void EncodeReflect(const SKString & Mip , const SKString & Pow , SKString& OutString);
		virtual void DecodeReflect(const SKString & Mip , const SKString & Pow , const SKString & Value,SKString& OutString);

		virtual void Reciprocal(const SKString & Value, SKString &OutString)const;
		virtual void Clip(const SKString & ClipValue,SKString &OutString)const;
		virtual void GreaterZeroPow(const SKString & Value,SKREAL Exp,SKString &OutString) const;
		virtual void SaturatePow(const SKString & Value,SKREAL Exp,SKString &OutString) const;
		virtual void Saturate(const SKString & Value,SKString &OutString)const;
		virtual void PointLightCubeShadow(const SKString & PointLightName , const SKString & CubeShadowSampler , const SKString & ViewMatrix,
			const SKString & WorldPos, SKString &OutString)const;

		virtual void PointLightCubeShadow(const SKString & PointLightName , const SKString & CubeShadowSampler ,
			const SKString & WorldPos, SKString &OutString)const;

		virtual void PointLightVolumeShadow(const SKString & PointLightName , const SKString & VolumeShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const;

		virtual void PointLightDualParaboloidShadow(const SKString & PointLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const;
		virtual void DirectionLightVolumeShadow(const SKString & DirLightName , const SKString & VolumeShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const;
		virtual void DirectionLightCSMShadow(const SKString & DirLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, const SKString & ViewPos,const SKString &FarZ,SKString& OutString)const;
		virtual void DirectionLightShadow(const SKString & DirLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const;
		virtual void SpotLightShadow(const SKString & SpotLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const;


		virtual void TranLightToTemp(SKArray<SKLight *> LightArray,SKString & OutString)const;
		virtual void GetLightFunction(SKArray<SKLight *> LightArray,const SKString & WorldPos,SKString & OutString)const;
	private:
		void CreateVInputDeclarePosition(SKVertexBuffer * pVBuffer,
			SKString & OutString);
		void CreateVInputDeclareTexCoord(SKVertexBuffer * pVBuffer,
			SKString & OutString);
		void CreateVInputDeclareNormal(SKVertexBuffer * pVBuffer,
			SKString & OutString);
		void CreateVInputDeclareColor(SKVertexBuffer * pVBuffer,
			SKString & OutString);
		void CreateVInputDeclareSkin(SKVertexBuffer * pVBuffer,
			SKString & OutString);


		void CreateVOutputDeclarePosition(
			SKString & OutString);
		void CreateVOutputDeclareTexCoord(SKVertexBuffer * pVBuffer,unsigned int& ID,
			SKString & OutString);
		void CreateVOutputDeclareNormal(SKVertexBuffer * pVBuffer,unsigned int& ID,
			SKString & OutString);
		void CreateVOutputDeclareColor(SKVertexBuffer * pVBuffer,
			SKString & OutString);

		void CreateVOutputDeclareLocalPos(unsigned int& ID,
			SKString & OutString);

		void CreateVOutputDeclareWorldPos(unsigned int& ID,
			SKString & OutString);

		void CreateVOutputDeclareProjectPos(unsigned int& ID,
			SKString & OutString);

		void CreateVOutputDeclareProjectZ(unsigned int& ID,
			SKString & OutString);

		void CreateUserConstantWorldViewProjectMatrix(SKShader * pShader,unsigned int& ID,
			SKString & OutString);

		void CreateUserConstantLightShadowMatrix(SKShader * pShader,unsigned int& ID,
			SKString & OutString);

		void CreateUserConstantProjectMatrix(SKShader * pShader, unsigned int& ID,
			SKString & OutString);

		void CreateUserConstantViewProjectMatrix(SKShader * pShader,unsigned int& ID,
			SKString & OutString);

		void CreateUserConstantViewMatrix(SKShader * pShader,unsigned int& ID,
			SKString & OutString);
		void CreateUserConstantLightWorldPos(SKShader * pShader,unsigned int& ID,
			SKString & OutString);

		void CreateUserConstantLightDirection(SKShader * pShader,unsigned int& ID,
			SKString & OutString);

		void CreateUserConstantWorldViewMatrix(SKShader * pShader,unsigned int& ID,
			SKString & OutString);
		void CreateUserConstantFarZ(SKShader * pShader,unsigned int& uiRegisterID,
			SKString & OutString);
		void CreateUserConstantSkyLightUpColor(SKShader * pShader,unsigned int& uiRegisterID,
			SKString & OutString);
		void CreateUserConstantSkyLightDownColor(SKShader * pShader,unsigned int& uiRegisterID,
			SKString & OutString);

		void CreateUserConstantLight(SKShader * pShader,MaterialShaderPara &MSPara,unsigned int& uiRegisterID,
			SKString & OutString);
		void CreateUserConstantPointLightRange(SKShader * pShader,unsigned int& uiRegisterID,
			SKString & OutString);
		void CreateUserConstantSkin(SKGeometry * pGeometry,SKShader * pShader,unsigned int& ID,
			SKString & OutString);
		void CreateUserConstantWorldMatrix(SKShader * pShader,unsigned int& ID,
			SKString & OutString);
		void CreateUserConstantCameraWorldPos(SKShader * pShader,unsigned int& ID,
			SKString & OutString);
		void CreateUserConstantDirectionLight(SKShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			SKString & OutString);
		void CreateUserConstantPointLight(SKShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			SKString & OutString);
		void CreateUserConstantSpotLight(SKShader * pShader,unsigned int& ID,unsigned int uiLightNum,
			SKString & OutString);

		void CreateUserConstantInvRTWidth(SKShader * pShader,unsigned int& uiRegisterID,
			SKString & OutString);

		void CreateUserConstantShadow(SKPShader* pPShader,SKArray<SKLight*> & LightArray,unsigned int &ID,SKString & OutString);
		void CreateUserConstantLightFuntion(SKPShader* pPShader,SKArray<SKLight*> & LightArray,unsigned int &ID,SKString & OutString);
		void CreateLightModleLookUpTable(SKPShader* pPShader,MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID,SKString & OutString);

		void SetUserConstantLightWorldPos(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantLightWorldDirection(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantWorldViewProjectMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserLightShadowMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);

		void SetUserConstantWorldViewMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantSkin(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantWorldMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantViewProjectMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantViewMatrix(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantProjectMatrix(MaterialShaderPara &MSPara, SKShader * pShader, unsigned int& ID);
		void SetUserConstantCameraPos(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantSkyUpColor(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantSkyDownColor(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantLight(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantShadowSampler(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantLightFuntionSampler(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantFarZ(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantPointLightRange(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);
		void SetUserConstantInvRTWidth(MaterialShaderPara &MSPara,SKShader * pShader,unsigned int& ID);

		void CreateVFuntionPositionAndNormal(MaterialShaderPara &MSPara,SKString & FuntionBody);
		void CreateVFuntionDualParaboloidPositionAndNormal(MaterialShaderPara &MSPara,SKString & FuntionBody);

		void CreateVFuntionVolumeShadowPositionAndNormal(MaterialShaderPara &MSPara,SKString & FuntionBody,unsigned int uiPass);
		void CreateVFuntionLocalPosition(MaterialShaderPara &MSPara,SKString & FuntionBody);
		void CreateVFuntionPosition(MaterialShaderPara &MSPara,SKString & FuntionBody);	
		void CreateVFuntionColor(MaterialShaderPara &MSPara,SKString & FuntionBody);
		void CreateVFuntionTexCoord(MaterialShaderPara &MSPara,SKString & FuntionBody);
		void CreateVFuntionProjectZ(MaterialShaderPara &MSPara,SKString & FuntionBody);
		void CreateVFuntionPost(MaterialShaderPara &MSPara,SKString & FuntionBody);
		void CreateVFuntionWorldPosition(MaterialShaderPara &MSPara,SKString & FuntionBody);
		void CreateVFuntionProjectPosition(MaterialShaderPara &MSPara,SKString & FuntionBody);

		void CreatePInputDeclareTexCoord(SKVertexBuffer * pVBuffer,unsigned int& ID,
			SKString & OutString);
		void CreatePInputDeclareNormal(SKVertexBuffer * pVBuffer,unsigned int& ID,
			SKString & OutString);
		void CreatePInputDeclareColor(SKVertexBuffer * pVBuffer,
			SKString & OutString);

		void CreatePInputDeclareLocalPos(unsigned int& ID,
			SKString & OutString);

		void CreatePInputDeclareProjectZ(unsigned int& ID,
			SKString & OutString);

		void CreatePInputDeclareWorldPos(unsigned int& ID,
			SKString & OutString);
		void CreatePInputDeclareProjectPos(unsigned int& ID,
			SKString & OutString);

		void CreateGBufferSampler(SKPShader* pPShader, MaterialShaderPara &MSPara, unsigned int &uiTexRegisterID, SKString & OutString);
		protected:
			static SKString ms_IncludeShader;

			static SKString ms_DynamicShader;

			static SKAdapterInfo	ms_AdapterInfo[5];
			static UINT			ms_uiAdapterNum;
	};


}
#endif