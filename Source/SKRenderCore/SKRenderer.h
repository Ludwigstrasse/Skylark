#ifndef SKRENDERER_H
#define SKRENDERER_H
#include "SKVector2.h"
#include "SKVector3W.h"
#include "SKRenderState.h"
#include "SKSamplerState.h"
#include "SKShaderKey.h"
#include "SKCamera.h"
namespace SKEngine2
{
	class SKCamera;
	class SKBind;
	class SKFont;
	class SKCamera;
	class SKTexAllState;
	class SKGeometry;
	class SKSpatial;
	class VS2DTexture;
	class VS3DTexture;
	class SKMaterial;
	class SKMaterialInstance;
	class SKMeshDate;
	class VS2DTexSampler;
	class VS1DTexSampler;
	class SKPutNode;
	class SKLight;
	class SKGeometry;
	class SKIndexBuffer;
	class SKMaterialInstance;
	class SKMul;
	class SKDiv;
	class SKDot;
	class SKSub;
	class SKAdd;
	class SKColorBuffer;
	class SKMakeValue;
	class SKSaturate;
	DECLARE_Ptr(SKDVGeometry);
	DECLARE_Ptr(SKTexture);
	DECLARE_Ptr(SKVertexFormat);
	DECLARE_Proxy(SKFont);
	typedef struct SKDISPLAYMODE_STURCT
	{
		UINT            uiWidth;			//ʾֱ
		UINT            uiHeight;
		UINT            uiRefreshRate;		//ˢ
		UINT			uiBitFormat;			
		SKString			StringExpress;		//ַʾ
	}SKDisplayMode, *SKDisplayModePtr;

	typedef struct SKDEVICEINFO_STURCT
	{
		UINT		uiMaxMultisample;
		UINT		uiMaxTextureWidth;
		UINT		uiMaxTextureHeight;
		UINT		uiMaxUseClipPlane;
		UINT		uiMaxTexture;
		UINT		fMaxAnisotropy;
		UINT		VertexShaderVersion;
		UINT		PixelShaderVersion;
		UINT		uiMaxRTNum;	
	}SKDeviceInfo,*SKDeviceInfoPtr;

	typedef struct SKADAPTERINFO_STURCT
	{
		unsigned int		m_uiAdapter;
		SKString			AdapterName;					
		SKArray<SKDisplayMode>	DisplayMode;	
		UINT			uiDisplayModeNum;
		//HAL SW REF
		SKDeviceInfo	DeviceInfo[3];	
	}SKAdapterInfo,*SKAdapterInfoPtr;
	class SKGRAPHIC_API SKRenderer
	{
	public:
		SKRenderer();
		virtual ~SKRenderer() = 0;
		friend class SKVertexBuffer;
		friend class SKVertexFormat;
		friend class SKDataBuffer;
		friend class SKDepthStencil;
		friend class SKTexture;
		friend class SKRenderTarget;
		friend class SKIndexBuffer;
		friend class SKCustomMaterial;
		friend class SKVShader;
		friend class SKPShader;
		friend class SKGShader;
		friend class SKDShader;
		friend class SKHShader;
		friend class SKCubeTexture;
		friend class SKSamplerState;
		friend class SKBlendState;
		friend class SKDepthStencilState;
		friend class SKRasterizerState;
		friend class SKRenderThreadSys;
		enum	//Render API Type
		{
			RAT_NULL,
			RAT_OPENGL,
			RAT_DIRECTX9,
			RAT_DIRECTX10,
			RAT_DIRECTX11,
			RAT_SOFTWARE,
			RAT_MAX
		};
		enum //function return information
		{
			FRI_FAIL,		
			FRI_SUCCESS,
			FRI_SAMERESOURCE
		};
		enum //Multisample
		{
			MS_NONE,
			MS_2,
			MS_4,
			MS_8,
			MS_16,
			MS_MAX
		};
		enum	//Surface Format Type
		{
			SFT_A8R8G8B8,      //BGRA
			SFT_D16,          
			SFT_D24X8,         
			SFT_D32,           
			SFT_A32B32G32R32F, //RGBA
			SFT_A16B16G16R16F, //RGBA
			SFT_G16R16F, 
			SFT_R16F, 
			SFT_R32F,
			SFT_DISCARD,
			SFT_D24S8,
			SFT_G32R32F,//RG
			SFT_R5G6B5,//BGR
			SFT_A8,
			SFT_DXT3,	//BC2
			SFT_DXT5,	//BC3
			SFT_MAX
		};
		
		
		virtual int GetRendererType () const = 0;
		virtual const TCHAR * GetRendererStringType() const = 0;
		// ɫ  ģ buffer
		FORCEINLINE void SetClearColor (const SKColorRGBA& ClearColor);
		FORCEINLINE const SKColorRGBA& GetClearColor () const;

		FORCEINLINE void SetClearDepth (SKREAL fClearDepth);
		FORCEINLINE SKREAL GetClearDepth () const;

		FORCEINLINE void SetClearStencil (unsigned int uiClearValue);
		FORCEINLINE unsigned int GetClearStencil () const;
		


		

		enum //Clear Flag
		{
			CF_NONE = 0,
			CF_COLOR = 1 << 0,
			CF_DEPTH = 1 << 1,
			CF_STENCIL = 1 << 2,
			CF_USE_MAX = CF_COLOR | CF_DEPTH | CF_STENCIL
		};

		virtual void ClearBuffers(unsigned int uiClearFlag) = 0;
		virtual void ClearBackBuffer () = 0;
		virtual void ClearZBuffer () = 0;
		virtual void ClearStencilBuffer () = 0;
		virtual void ClearBuffers () = 0;
		
		
		virtual void ClearBuffers(unsigned int uiClearFlag,int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearBackBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearZBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearStencilBuffer (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;
		virtual void ClearBuffers (int iXPos, int iYPos, int iWidth,
			int iHeight) = 0;

		void DrawText(int iX, int iY, const DWORD rColor,const TCHAR* acText, ...);		
		virtual bool SetViewPort(SKViewPort *pViewPort = NULL) = 0;

		
		void SetFont(SKFontR * pFont);

		virtual bool UseWindow(int uiWindowID = -1) = 0;
		
		virtual bool CooperativeLevel() = 0;
		virtual bool BeginRendering() = 0;
		virtual bool EndRendering() = 0;
		
		virtual void DeviceLost() = 0;
		virtual void ResetDevice() = 0;
		virtual bool ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow) = 0;

		virtual const SKAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum) = 0;
		static unsigned int GetBytesPerPixel(unsigned int uiFormatType);
		static unsigned int GetChannelPerPixel(unsigned int uiFormatType);
		static const SKString & GetVShaderProgramMain();
		static const SKString & GetPShaderProgramMain();
		static const SKString & GetGShaderProgramMain();
		FORCEINLINE unsigned int GetScreenWith()const;
		FORCEINLINE unsigned int GetScreenHeight()const;
		FORCEINLINE unsigned int GetDisplayFormat()const;

		FORCEINLINE unsigned int GetCurAnisotropy()const;
		FORCEINLINE unsigned int GetCurMultisample()const;
		FORCEINLINE unsigned int GetMaxMultisample()const;
		FORCEINLINE unsigned int GetDepthStencilFormat()const;
		FORCEINLINE bool IsWindowed()const;
		FORCEINLINE unsigned int WindoweNum()const;
		FORCEINLINE unsigned int GetMaxTextureWidth()const;
		FORCEINLINE unsigned int GetMaxTextureHeight()const;
		
		FORCEINLINE unsigned int GetSuitableMultisample(unsigned int Multisample)const;
		FORCEINLINE unsigned int GetMaxUseClipPlane()const;
		FORCEINLINE unsigned int GetMaxTexture()const;
		FORCEINLINE unsigned int GetMaxVTexture()const;
		FORCEINLINE unsigned int GetMaxGTexture()const;
		FORCEINLINE unsigned int GetBufferFormat()const;
		FORCEINLINE unsigned int GetMaxAnisotropy()const;

		FORCEINLINE unsigned int GetMaxRTNum()const;

		FORCEINLINE SKString GetAdapterName()const;

		FORCEINLINE unsigned int GetCurRTWidth()const;
		FORCEINLINE unsigned int GetCurRTHeight()const;
		FORCEINLINE unsigned int GetCurRTMultisampler()const;
		//Load Release
		bool LoadVShaderProgram (SKVShader * pVShaderProgram);
		bool ReleaseVShaderProgram(SKVShader * pVShaderProgram);

		bool LoadPShaderProgram (SKPShader * pPShaderProgram);
		bool ReleasePShaderProgram (SKPShader* pPShaderProgram);

		bool LoadGShaderProgram(SKGShader * pGShaderProgram);
		bool ReleaseGShaderProgram(SKGShader * pGShaderProgram);

		bool LoadDShaderProgram(SKDShader * pDShaderProgram);
		bool ReleaseDShaderProgram(SKDShader * pDShaderProgram);

		bool LoadHShaderProgram(SKHShader * pHShaderProgram);
		bool ReleaseHShaderProgram(SKHShader * pHShaderProgram);

		bool LoadTexture (SKTexture * pTexture);
		bool ReleaseTexture (SKTexture * pTexture);

		bool LoadVBuffer(SKVertexBuffer * pVBuffer);
		bool ReleaseVBuffer(SKVertexBuffer * pVBuffer);
		

		bool LoadIBuffer (SKIndexBuffer* pIBuffer);
		bool ReleaseIBuffer (SKIndexBuffer* pIBuffer);
	
		bool LoadDepthStencil (SKDepthStencil * pDepthStencil);
		bool ReleaseDepthStencil (SKDepthStencil * pDepthStencil);
		
		bool LoadRenderTarget (SKRenderTarget  * pRenderTarget);
		bool ReleaseRenderTarget  (SKRenderTarget * pRenderTarget);

		bool LoadBlendState (SKBlendState * pBlendState);
		bool ReleaseBlendState  (SKBlendState * pBlendState);

		bool LoadDepthStencilState (SKDepthStencilState * pDepthStencilState);
		bool ReleaseDepthStencilState  (SKDepthStencilState * pDepthStencilState);

		bool LoadRasterizerState(SKRasterizerState * pRasterizerState);
		bool ReleaseRasterizerState  (SKRasterizerState * pRasterizerState);

		bool LoadSamplerState(SKSamplerState * pSamplerState);
		bool ReleaseSamplerState  (SKSamplerState * pSamplerState);

		virtual bool DrawMesh(SKGeometry * pGeometry,SKRenderState * pRenderState,SKVShader * pVShader, SKPShader * pPShader,SKGShader * pGShader) = 0;
		struct SCREEN_QUAD_TYPE
		{
			SKVector3 Point;
		};
		struct SCREEN_FONT_TYPE
		{
			SKVector3 Point;
			SKVector2 UV;
			DWORD Color;
		};
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]) = 0;
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer,unsigned int uiVertexNum,
								SKUSHORT_INDEX * pIndexBuffer,unsigned int uiIndexNum) = 0;
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
			SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum) = 0;
		void SetUseState(SKRenderState & RenderState,unsigned int uiRenderStateInheritFlag);
		void ClearUseState();
		FORCEINLINE const SKRenderState & GetUseState() const
		{
			return m_UseState;
		}
		FORCEINLINE unsigned int GetRenderStateInheritFlag()const
		{
			return m_uiRenderStateInheritFlag;
		}
		void SetRenderState(SKRenderState & RenderState);
		virtual unsigned int SetBlendState(SKBlendState * pBlendState,bool bForceSet = false) = 0;
		virtual unsigned int SetDepthStencilState(SKDepthStencilState * pDepthStencilState, bool bForceSet = false) = 0;
		virtual unsigned int SetRasterizerState(SKRasterizerState * pRasterizerState, bool bForceSet = false) = 0;
		virtual void SetClipPlane(SKArray<SKPlane3> &Plane,bool bEnable) = 0;
		virtual void SetScissorRect(SKArray<SKRect2> &Rect,bool bEnable) = 0;

		virtual void SetTexAllState(SKTexAllState * pTexAllState,unsigned int i);
		virtual void SetVTexAllState(SKTexAllState * pTexAllState,unsigned int i);
		virtual void SetGTexAllState(SKTexAllState * pTexAllState, unsigned int i);
		virtual void SetDTexAllState(SKTexAllState * pTexAllState, unsigned int i);
		virtual void SetHTexAllState(SKTexAllState * pTexAllState, unsigned int i);

		virtual unsigned int SetVShader(SKVShader * pVShader) = 0;
		virtual unsigned int SetPShader(SKPShader * pPShader) = 0;
		virtual unsigned int SetGShader(SKGShader * pGShader) = 0;
		virtual unsigned int SetDShader(SKDShader * pDShader) = 0;
		virtual unsigned int SetHShader(SKHShader * pHShader) = 0;

		virtual unsigned int SetVBuffer(SKVertexBuffer * pVBuffer) = 0;
		virtual unsigned int SetIBuffer (SKIndexBuffer* pIBuffer) = 0;

		virtual unsigned int SetTexture (SKTexture* pTexture,unsigned int i) = 0;
		virtual unsigned int SetVTexture (SKTexture* pTexture,unsigned int i) = 0;
		virtual unsigned int SetGTexture(SKTexture* pTexture, unsigned int i) = 0;
		virtual unsigned int SetDTexture(SKTexture* pTexture, unsigned int i) = 0;
		virtual unsigned int SetHTexture(SKTexture* pTexture, unsigned int i) = 0;

		unsigned int SetMesh(SKMeshDate *pMeshDate);



		virtual unsigned int SetSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetVSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetGSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetDSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;
		virtual unsigned int SetHSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false) = 0;

		virtual bool SetRenderTargets(SKRenderTarget * pRenderTargets[], unsigned int uiNum);
		virtual bool EndRenderTargets(SKRenderTarget * pRenderTargets[], unsigned int uiNum);
		virtual bool SetRenderTarget(SKRenderTarget *pRenderTarget,unsigned int i);
		virtual bool EndRenderTarget(SKRenderTarget *pRenderTarget,unsigned int i);
		virtual bool SetDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer) = 0;
		virtual bool EndDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer) = 0;
		virtual unsigned int GetTextureCompressLen(unsigned int uiLen) = 0;

		void SetVSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b);
		void SetPSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b);
		void SetGSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b);
		void SetDSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b);
		void SetHSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b);

		void RenderStateToDefault();
		
		void EnableTextureRTUsed(SKTexture * pTexture);
		void DisableTextureRTUsed(SKTexture * pTexture);

		bool CheckIsTextureCanSet(SKTexture * pTexture);
		struct ChildWindowInfo
		{
			HWND	m_hHwnd;
			unsigned int m_uiWidth;
			unsigned int m_uiHeight;
			bool		 m_bDepth;
		};
		ChildWindowInfo * GetChildWindowInfo(int uiID);

		virtual bool IsSupportMulBufferSwtich()const = 0;
		virtual bool IsSupportGS()const = 0;
protected:
		
		HWND	m_hMainWindow;          // 						
		ChildWindowInfo * m_pChildWindowInfo;
		int		m_iNumChildWindow;          // ڸ
		int		m_iCurWindowID;
		bool	m_bIsRendering;


		SKColorRGBA m_ClearColor;
		SKREAL m_fClearDepth;
		unsigned int m_uiClearStencil;
	

		SKFontRPtr			m_pFont;
		

		SKVertexFormat*		m_pVertexFormat;
		SKVertexBuffer*		m_pVertexBuffer;
		SKIndexBuffer*		m_pIndexBuffer;
		SKVShader*			m_pVShader;
		SKPShader*			m_pPshader;
		SKGShader*			m_pGShader;
		SKDShader*			m_pDShader;
		SKHShader*			m_pHShader;

		SKVertexFormatPtr   m_pQuadVertexFormat;	

		SKTexture *m_pVTex[TEXLEVEL];
		SKSamplerState *	m_pVSamplerState[TEXLEVEL];

		SKTexture *m_pPTex[TEXLEVEL];
		SKSamplerState *	m_pPSamplerState[TEXLEVEL];

		
		SKTexture *m_pGTex[TEXLEVEL];
		SKSamplerState *	m_pGSamplerState[TEXLEVEL];

		SKTexture *m_pDTex[TEXLEVEL];
		SKSamplerState *	m_pDSamplerState[TEXLEVEL];

		SKTexture *m_pHTex[TEXLEVEL];
		SKSamplerState *	m_pHSamplerState[TEXLEVEL];

		SKRenderState m_LocalRenderState;
		
		SKBlendState * m_pBlendState;
		SKDepthStencilState * m_pDepthStencilState;
		SKRasterizerState * m_pRasterizerState;
		bool				m_bClipPlaneEnable;
		bool				m_bScissorRectEnable;
		SKRenderState		m_UseState;
		unsigned int		m_uiRenderStateInheritFlag;

		enum //BACK_MACTH_TYPE
		{
			BMT_NONE = 0x00,
			BMT_RENDER_TARGET = 0x01,
		};
		unsigned int		m_uiBackMacthType;
		virtual void SetVShaderConstant(SKVShader * pShader) = 0;
		virtual void SetVShaderSampler(SKVShader * pShader) = 0;

		virtual void SetGShaderConstant(SKGShader * pShader) = 0;
		virtual void SetGShaderSampler(SKGShader * pShader) = 0;

		virtual void SetDShaderConstant(SKDShader * pShader) = 0;
		virtual void SetDShaderSampler(SKDShader * pShader) = 0;

		virtual void SetHShaderConstant(SKHShader * pShader) = 0;
		virtual void SetHShaderSampler(SKHShader * pShader) = 0;

		virtual void SetPShaderConstant(SKPShader * pShader) = 0;
		virtual void SetPShaderSampler(SKPShader * pShader) = 0;

		virtual bool SetVShaderProgram(SKVShader*	pVShaderProgram) = 0;
		virtual bool SetPShaderProgram(SKPShader*	pPShaderProgram) = 0;
		virtual bool SetGShaderProgram(SKGShader*	pGShaderProgram) = 0;
		virtual bool SetDShaderProgram(SKDShader*	pDShaderProgram) = 0;
		virtual bool SetHShaderProgram(SKHShader*	pHShaderProgram) = 0;
		//Load Release
		virtual bool OnLoadVShaderProgram (SKVShader * pVShaderProgram,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseVShaderProgram(SKResourceIdentifier * pVShaderProgramID) = 0;

		virtual bool OnLoadPShaderProgram (SKPShader* pPShaderProgram,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleasePShaderProgram(SKResourceIdentifier* pGShaderProgramID) = 0;

		virtual bool OnLoadGShaderProgram(SKGShader * pGShaderProgram, SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseGShaderProgram(SKResourceIdentifier * pGShaderProgramID) = 0;

		virtual bool OnLoadDShaderProgram(SKDShader * pDShaderProgram, SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseDShaderProgram(SKResourceIdentifier * pDShaderProgramID) = 0;

		virtual bool OnLoadHShaderProgram(SKHShader * pHShaderProgram, SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseHShaderProgram(SKResourceIdentifier * pHShaderProgramID) = 0;

		virtual bool OnLoadTexture (SKTexture * pTexture,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseTexture (SKResourceIdentifier * pTextureID) = 0;

		virtual bool OnLoadDepthStencil (SKDepthStencil * pDepthStencil,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseDepthStencil (SKResourceIdentifier * pDepthStencilID) = 0;
		
		virtual bool OnLoadRenderTarget (SKRenderTarget * pRenderTarget,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseRenderTarget (SKResourceIdentifier * pRenderTarget) = 0;

		virtual bool OnLoadVBufferDate (SKVertexBuffer * pVBuffer,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseVBufferDate (SKResourceIdentifier* pVBufferID) = 0;

		virtual bool OnLoadVBufferFormat (SKVertexFormat * pVertexFormat,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseVBufferFormat (SKResourceIdentifier* pVBufferID) = 0;

		virtual bool OnLoadIBuffer (SKIndexBuffer* pIBuffer,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseIBuffer (SKResourceIdentifier* pIBufferID) = 0;

		virtual bool OnLoadBlendState (SKBlendState * pBlendState,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseBlendState  (SKResourceIdentifier * pBlendStateID) = 0;

		virtual bool OnLoadDepthStencilState (SKDepthStencilState * pDepthStencilState,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseDepthStencilState  (SKResourceIdentifier * pDepthStencilStateID) = 0;

		virtual bool OnLoadRasterizerState(SKRasterizerState * pRasterizerState,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseRasterizerState  (SKResourceIdentifier * pRasterizerStateID) = 0;

		virtual bool OnLoadSamplerState(SKSamplerState * pSamplerState,SKResourceIdentifier *&pID) = 0;
		virtual bool OnReleaseSamplerState  (SKResourceIdentifier * pSamplerStateID) = 0;

		virtual unsigned int SetVertexFormat(SKVertexFormat * pVertexFormat) = 0;

		virtual void * Lock(SKVertexBuffer * pVertexBuffer) = 0;
		virtual void UnLock(SKVertexBuffer * pVertexBuffer) = 0;

		virtual void * Lock(SKIndexBuffer * pIndexBuffer) = 0;
		virtual void UnLock(SKIndexBuffer * pIndexBuffer) = 0;

		virtual void * Lock(SKTexture * pTexture,unsigned int uiLevel,unsigned int uiFace) = 0;
		virtual void UnLock(SKTexture * pTexture,unsigned int uiLevel,unsigned int uiFace) = 0;

		virtual void * Lock(SKRenderTarget * pRenderTarget) = 0;
		virtual void UnLock(SKRenderTarget * pRenderTarget) = 0;

		virtual void * Lock(SKDepthStencil * pDepthStencil) = 0;
		virtual void UnLock(SKDepthStencil * pDepthStencil) = 0;

		virtual bool CopyResourceBuffer(VS2DTexture * pSource,SKCubeTexture * pDest,unsigned int uiFace) = 0;
		
		bool SetDefaultValue();
		bool ReleaseDefaultValue();

		//shader 
		FORCEINLINE static DWORD ShaderVersion(UINT Major,UINT Minor); 
	
		
		

		virtual bool ReleaseBindObjectResource();
	protected:
		enum	//device type
		{
			DT_HAL = 0,
			DT_REF = 1,
			DT_MAX
		};
		

		unsigned int	m_uinAdapter;
		unsigned int	m_uiDevType;
		SKString		m_AdapterName;
		UINT		m_uiScreenWidth;
		UINT		m_uiScreenHeight;
		UINT		m_uiCurRTWidth;
		UINT		m_uiCurRTHeight;
		UINT		m_uiDisplayFormat;
		UINT		m_uiBufferFormat;
		bool		m_bWindowed;


		UINT		m_uiDepthStencilFormat;
		UINT		m_uiCurAnisotropy;
		UINT		m_uiCurMultisample;
		UINT		m_uiCurRTMultisampler;
		DWORD		m_dwMultisampleQuality;
		UINT		m_uiMaxMultisample;		
		UINT		m_uiMaxTextureWidth;
		UINT		m_uiMaxTextureHeight;

		UINT		m_uiMaxUseClipPlane;
		UINT		m_uiMaxTexture;
		UINT		m_uiMaxVTexture;
		UINT		m_uiMaxGTexture;
		UINT		m_uiMaxDTexture;
		UINT		m_uiMaxHTexture;
		UINT		m_uiMaxAnisotropy;

		UINT		m_uiMaxRTNum;
		
	

		static SKString ms_VShaderProgramMain;
		static SKString ms_PShaderProgramMain;
		static SKString ms_GShaderProgramMain;

		static unsigned int ms_uiBytesPerPixel[SFT_MAX];
		static unsigned int ms_uiChannelPerPixel[SFT_MAX];
		static SCREEN_QUAD_TYPE ms_FullScreen[4];
		static SKUSHORT_INDEX ms_FullScreenI[6];
		SKArray<SCREEN_FONT_TYPE> m_FontVertex;
		SKArray<SKUSHORT_INDEX> m_FontIndex;

		SKViewPort m_CurViewPort;
	//shader string
	public:
		//shader function
		virtual SKString TexColorBuffer(const SKColorBuffer * pColorBuffer) const = 0;
		virtual SKString Tex2D(const VS2DTexSampler * p2DTexSampler) const= 0;
		virtual SKString Tex1D(const VS1DTexSampler * p1DTexSampler) const= 0;
		virtual SKString Mul(const SKMul * pMul) const= 0;
		virtual SKString Div(const SKDiv * pDiv) const = 0;
		virtual SKString Dot(const SKDot * pDot) const= 0;
		virtual SKString Sub(const SKSub * pSub) const= 0;
		virtual SKString Add(const SKAdd * pAdd) const = 0;
		virtual SKString MakeValue(const SKMakeValue * pMakeValue) const = 0;
		virtual SKString Saturate(const SKSaturate * pSaturate) const = 0;
		//shader key word
		enum //Float Index
		{
			FI_1,
			FI_2,
			FI_3,
			FI_4,
			FI_MAX

		};
		SKString Float(unsigned int uiIndex)const;
		virtual SKString Float()const =  0;
		virtual SKString Float2()const =  0;
		virtual SKString Float3()const =  0;
		virtual SKString Float4()const =  0;
		virtual SKString Return()const = 0;

		virtual SKString FloatConst(const SKString & Value1)const =  0;
		virtual SKString Float2Const(const SKString & Value1,const SKString & Value2)const =  0;
		virtual SKString Float3Const(const SKString & Value1,
						const SKString & Value2,const SKString & Value3)const =  0;
		virtual SKString Float4Const(const SKString & Value1,const SKString & Value2,
					const SKString & Value3,const SKString & Value4)const =  0;

		virtual void CustomPointLightUseString(SKString & PointLightUseString ,SKString & PointAttenuationDivString) = 0;

		virtual void CustomSpotLightUseString(SKString & SpotLightUseString ,SKString & SpotAttenuationDivString) = 0;

		enum //Value Element
		{
			VE_NONE = 0,
			VE_A = BIT(0),
			VE_R = BIT(1),
			VE_G = BIT(2),
			VE_B = BIT(3),
			DF_ALL = 0X0F
		};
		virtual SKString GetValueElement(const SKPutNode * pPutNode,
			unsigned char uiVE)const = 0;
		virtual SKString GetValueElement(const SKString & InputString,
			unsigned char uiVE)const = 0;
		
		virtual SKString GetDeclareValue(const SKString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const = 0;
		virtual SKString GetDeclareSampler(const SKString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const = 0;
		
		//shader custom function
		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;
		virtual void CreateVUserConstant(SKVShader * pVShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;
		virtual void CreateVFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;

		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,SKVShader *pVShader) = 0;

		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;
		virtual void CreatePUserConstant(SKPShader * pPShader,MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;
		virtual void CreatePFuntion(MaterialShaderPara &MSPara,unsigned int uiPassType,
										SKString & OutString) = 0;

		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara,unsigned int uiPassType,SKPShader *pPShader) = 0;

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString) = 0;
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString) = 0;
		virtual void CreateGUserConstant(SKGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString) = 0;
		virtual void CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString) = 0;
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKGShader *pGShader) = 0;

		virtual void GetIncludeShader(SKString &OutString) = 0;
		
		virtual void GetDynamicShader(SKString &OutString) = 0;

// 		enum //GpuSkin Flag
// 		{
// 			GKF_POSITION,
// 			GKF_POSITION_NORMAL,
// 			GKF_POSITION_BUMP,
// 			GKF_MAX
// 		};
// 		virtual void GpuSkin(unsigned int uiFlag,
// 						const SKString & BlendIndex,const SKString &BlendWeight,const BoneMatrixName,
// 							const SKString & InputPos,const SKString &InputNormal,const SKString &InputTangent,const SKString &InputBinormal,
// 							SKString & OutPos,SKString &Outormal,SKString &OutTangent,SKString &OutBinormal)const = 0;

		virtual void SkyLight(const SKString &WorldNormal,const SKString &UpColor,const SKString & DownColor, const SKString & DiffuseColor,SKString & OutString) = 0;
		virtual void DirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
									const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
									SKString & OutString)const = 0;
		virtual void CustomDirectionalLight(int iLightNum,const SKString &CustomString,SKArray<SKString> ShadowString,SKString & OutString)const = 0;

		virtual void OrenNayarDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
									const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
									SKString & OutString)const = 0;
		virtual void MinnaertDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
									const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
									SKString & OutString)const = 0;
		virtual void StraussDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
									const SKString &Metalness,const SKString &Transparency,const SKString &K,
									const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
									SKString & OutString)const = 0;
		virtual void ShirleyDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
									const SKString &NU,const SKString &NV,const SKString &Rs,
									const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
									SKString & OutString)const = 0;
		virtual void SchlickDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
										const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
										SKString & OutString)const = 0;
		virtual void CookTorranceDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
										const SKString &Roughness , const SKString &C , const SKString &R,
										const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
										SKString & OutString)const = 0;

		virtual void IsotropicWardDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
			const SKString &Roughness ,
			const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;
		virtual void AnisotropicWardDirectionalLight(int iLightNum,const SKString &Diffuse,const SKString &Specular , 
			const SKString &RoughnessX ,const SKString &RoughnessY ,
			const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;

		virtual void PointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
							const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							SKString & OutString)const = 0;
		virtual void CustomPointLight(int iLightNum,const SKString &CustomString,const SKString & WorldPos,SKArray<SKString> ShadowString,SKString & OutString)const = 0;

		virtual void OrenNayarPointLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
							const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
							SKString & OutString)const = 0;
		virtual void MinnaertPointLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
							const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							SKString & OutString)const = 0;
		virtual void StraussPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
							const SKString &Metalness,const SKString &Transparency,const SKString &K,
							const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
							SKString & OutString)const = 0;
		virtual void ShirleyPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
							const SKString &NU,const SKString &NV,const SKString &Rs,
							const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							SKString & OutString)const = 0;
		virtual void SchlickPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
							const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
							SKString & OutString)const = 0;
		virtual void CookTorrancePointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness , const SKString &C , const SKString &R,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;

		virtual void IsotropicWardPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;
		virtual void AnisotropicWardPointLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &RoughnessX ,const SKString &RoughnessY ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;

		virtual void SpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const = 0;
		virtual void CustomSpotLight(int iLightNum,const SKString &CustomString,const SKString & WorldPos,SKArray<SKString> ShadowString,SKString & OutString)const = 0;
		virtual void OrenNayarSpotLight(int iLightNum,const SKString &Diffuse,const SKString &RoughnessSquared,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
								SKString & OutString)const = 0;
		virtual void MinnaertSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Power,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const = 0;
		virtual void StraussSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,const SKString &Smoothness,
								const SKString &Metalness,const SKString &Transparency,const SKString &K,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
								SKString & OutString)const = 0;
		virtual void ShirleySpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular,
								const SKString &NU,const SKString &NV,const SKString &Rs,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const = 0;
		virtual void SchlickSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,const SKString &SpecularPow,
								const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
								SKString & OutString)const = 0;
		virtual void CookTorranceSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness , const SKString &C , const SKString &R,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,unsigned int uiSpecularType,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;
		virtual void IsotropicWardSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &Roughness ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,bool bLookUpTable,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;
		virtual void AnisotropicWardSpotLight(int iLightNum,const SKString &Diffuse,const SKString &Specular ,
			const SKString &RoughnessX ,const SKString &RoughnessY ,
			const SKString & WorldPos,const SKString &WorldNormal,const SKString &WorldCameraDir,SKArray<SKString> ShadowString,
			SKString & OutString)const = 0;

		virtual void LocalToWorldPos(const SKString & LocalPos,SKString &OutString)const = 0;
		virtual void WorldToViewPos(const SKString & LocalPos,SKString &OutString)const = 0;

		virtual void LocalToWorldNormal(const SKString & LocalNormal,SKString &OutString)const = 0;

		virtual void WorldToViewNormal(const SKString & WorldNormal,SKString &OutString)const = 0;

		virtual void GetWorldViewDir(SKString &OutString)const = 0;

		virtual void LocalToViewNormal(const SKString & LocalNormal,SKString &OutString)const = 0;
		virtual void TransProjPos(const SKString & Pos,const SKString & Matrix,SKString &OutString)const = 0;
		virtual void BumpNormal(const SKString &TexNormal,SKString &OutString)const = 0;

		virtual void ComputeDir(const SKString & Orgin,const SKString & End,SKString &OutString)const = 0;
		virtual void ComputeLength(const SKString & Orgin,const SKString & End,SKString &OutString)const = 0;
		virtual void ComputeBoneVector(const SKString& BoneIndex, const SKString & BoneWeight,const SKString &BoneVector,
					 const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const = 0;

		virtual void BoneTranPos( const SKString & LocalPos,const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const = 0;
		virtual void BoneTranNormal( const SKString & LocalNormal,const  SKString & U,const SKString & V,const SKString &N,SKString &OutString)const = 0;

		virtual void DecodeNormal1(const SKString & Normal,SKString &OutString) const= 0;
		virtual void RecodeNormal1(const SKString & Normal,SKString &OutString) const = 0;
		virtual void DecodeNormal2(const SKString & Normal,const SKString & Tengent,SKString &OutString) const= 0;

		virtual void DecodeNormal3(const SKString & Normal,SKString &OutString) const = 0;
		virtual void RecodeNormal3(const SKString & Normal,SKString &OutString) const = 0;

		virtual void Reciprocal(const SKString & Value, SKString &OutString)const = 0;
		virtual void Clip(const SKString & Value,SKString &OutString)const = 0;
		virtual void GreaterZeroPow(const SKString & Value,SKREAL Exp,SKString &OutString) const= 0;
		virtual void SaturatePow(const SKString & Value,SKREAL Exp,SKString &OutString) const= 0;
		virtual void Saturate(const SKString & Value,SKString &OutString)const = 0;

		virtual void PointLightCubeShadow(const SKString & PointLightName , const SKString & CubeShadowSampler , const SKString & ViewMatrix,
									const SKString & WorldPos, SKString& OutString)const = 0;

		virtual void PointLightCubeShadow(const SKString & PointLightName , const SKString & CubeShadowSampler ,
			const SKString & WorldPos, SKString& OutString)const = 0;

		virtual void PointLightVolumeShadow(const SKString & PointLightName , const SKString & VolumeShadowSampler , 
									const SKString & WorldPos, SKString& OutString)const = 0;

		virtual void PointLightDualParaboloidShadow(const SKString & PointLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const = 0;
		virtual void DirectionLightVolumeShadow(const SKString & DirLightName , const SKString & VolumeShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const = 0;

		virtual void DirectionLightCSMShadow(const SKString & DirLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, const SKString & ViewPos,const SKString &FarZ,SKString& OutString)const = 0;

		virtual void DirectionLightShadow(const SKString & DirLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const = 0;

		virtual void SpotLightShadow(const SKString & SpotLightName , const SKString & ShadowSampler , 
			const SKString & WorldPos, SKString& OutString)const = 0;

		virtual void TranLightToTemp(SKArray<SKLight *> LightArray,SKString & OutString)const = 0;

		virtual void GetLightFunction(SKArray<SKLight *> LightArray,const SKString & WorldPos,SKString & OutString)const = 0;

		virtual void EncodeReflect(const SKString & Mip , const SKString & Pow , SKString& OutString) = 0;

		virtual void DecodeReflect(const SKString & Mip , const SKString & Pow , const SKString & Value,SKString& OutString) = 0;

	public:
		static SKRenderer * ms_pRenderer;
	
	};

#include "SKRenderer.inl"
}


#endif