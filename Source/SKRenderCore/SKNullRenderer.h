#ifndef NULLRENDERER_H
#define NULLRENDERER_H
#include "SKRenderer.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKNullRenderer : public SKRenderer
	{
	public:
		SKNullRenderer();
		virtual ~SKNullRenderer();
		virtual int GetRendererType() const{ return RAT_NULL; }
		virtual const TCHAR * GetRendererStringType() const{ return _T("NullRender"); }
		virtual void ClearBuffers(unsigned int uiClearFlag);
		virtual void ClearBackBuffer();
		virtual void ClearZBuffer();
		virtual void ClearStencilBuffer();
		virtual void ClearBuffers();

		virtual void ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBackBuffer(int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearZBuffer(int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
			int iHeight);
		virtual void ClearBuffers(int iXPos, int iYPos, int iWidth,
			int iHeight);

		virtual bool SetViewPort(SKViewPort *pViewPort = NULL);
		virtual bool UseWindow(int uiWindowID = -1);

		virtual bool CooperativeLevel();
		virtual bool BeginRendering();
		virtual bool EndRendering();

		virtual void DeviceLost();
		virtual void ResetDevice();
		virtual bool ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow);
		virtual bool DrawMesh(SKGeometry * pGeometry, SKRenderState * pRenderState, SKVShader * pVShader, SKPShader * pPShader, SKGShader * pGShader);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]);
		virtual bool DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
			SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
		virtual bool DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
			SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum);
		virtual unsigned int SetBlendState(SKBlendState * pBlendState, bool bForceSet = false);
		virtual unsigned int SetDepthStencilState(SKDepthStencilState * pDepthStencilState, bool bForceSet = false);
		virtual unsigned int SetRasterizerState(SKRasterizerState * pRasterizerState, bool bForceSet = false);
		virtual void SetClipPlane(SKArray<SKPlane3> &Plane, bool bEnable);
		virtual void SetScissorRect(SKArray<SKRect2> &Rect, bool bEnable);

		virtual unsigned int SetVShader(SKVShader * pVShader);
		virtual unsigned int SetPShader(SKPShader * pPShader);
		virtual unsigned int SetGShader(SKGShader * pGShader);
		virtual unsigned int SetDShader(SKDShader * pDShader);
		virtual unsigned int SetHShader(SKHShader * pHShader);

		virtual unsigned int SetVBuffer(SKVertexBuffer * pVBuffer);
		virtual unsigned int SetIBuffer(SKIndexBuffer* pIBuffer);

		virtual unsigned int SetTexture(SKTexture* pTexture, unsigned int i);
		virtual unsigned int SetVTexture(SKTexture* pTexture, unsigned int i);
		virtual unsigned int SetGTexture(SKTexture* pTexture, unsigned int i);
		virtual unsigned int SetDTexture(SKTexture* pTexture, unsigned int i);
		virtual unsigned int SetHTexture(SKTexture* pTexture, unsigned int i);

		virtual unsigned int SetSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetVSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetGSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetDSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);
		virtual unsigned int SetHSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet = false);

		virtual bool SetDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer);
		virtual bool EndDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer);
		virtual unsigned int GetTextureCompressLen(unsigned int uiLen);
		virtual const SKAdapterInfo* GetAdapterInfo(unsigned int & uiAdapterNum);
		virtual bool IsSupportMulBufferSwtich()const;
		virtual bool IsSupportGS()const;
	protected:

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

		virtual bool SetVShaderProgram(SKVShader*	pVShaderProgram);
		virtual bool SetPShaderProgram(SKPShader*	pPShaderProgram);
		virtual bool SetGShaderProgram(SKGShader*	pGShaderProgram);
		virtual bool SetDShaderProgram(SKDShader*	pDShaderProgram);
		virtual bool SetHShaderProgram(SKHShader*	pHShaderProgram);
		//Load Release
		virtual bool OnLoadVShaderProgram(SKVShader * pVShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleaseVShaderProgram(SKResourceIdentifier * pVShaderProgramID);

		virtual bool OnLoadPShaderProgram(SKPShader* pPShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleasePShaderProgram(SKResourceIdentifier* pPShaderProgramID);

		virtual bool OnLoadGShaderProgram(SKGShader * pGShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleaseGShaderProgram(SKResourceIdentifier * pGShaderProgramID);

		virtual bool OnLoadDShaderProgram(SKDShader * pDShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleaseDShaderProgram(SKResourceIdentifier * pDShaderProgramID);

		virtual bool OnLoadHShaderProgram(SKHShader * pHShaderProgram, SKResourceIdentifier *&pID);
		virtual bool OnReleaseHShaderProgram(SKResourceIdentifier * pHShaderProgramID);

		virtual bool OnLoadTexture(SKTexture * pTexture, SKResourceIdentifier *&pID);
		virtual bool OnReleaseTexture(SKResourceIdentifier * pTextureID);

		virtual bool OnLoadDepthStencil(SKDepthStencil * pDepthStencil, SKResourceIdentifier *&pID);
		virtual bool OnReleaseDepthStencil(SKResourceIdentifier * pDepthStencilID);

		virtual bool OnLoadRenderTarget(SKRenderTarget * pRenderTarget, SKResourceIdentifier *&pID);
		virtual bool OnReleaseRenderTarget(SKResourceIdentifier * pRenderTarget);

		virtual bool OnLoadVBufferDate(SKVertexBuffer * pVBuffer, SKResourceIdentifier *&pID);
		virtual bool OnReleaseVBufferDate(SKResourceIdentifier* pVBufferID);

		virtual bool OnLoadVBufferFormat(SKVertexFormat * pVertexFormat, SKResourceIdentifier *&pID);
		virtual bool OnReleaseVBufferFormat(SKResourceIdentifier* pVBufferID);

		virtual bool OnLoadIBuffer(SKIndexBuffer* pIBuffer, SKResourceIdentifier *&pID);
		virtual bool OnReleaseIBuffer(SKResourceIdentifier* pIBufferID);


		virtual bool OnLoadBlendState(SKBlendState * pBlendState, SKResourceIdentifier *&pID);
		virtual bool OnReleaseBlendState(SKResourceIdentifier * pBlendStateID);

		virtual bool OnLoadDepthStencilState(SKDepthStencilState * pDepthStencilState, SKResourceIdentifier *&pID);
		virtual bool OnReleaseDepthStencilState(SKResourceIdentifier * pDepthStencilStateID);

		virtual bool OnLoadRasterizerState(SKRasterizerState * pRasterizerState, SKResourceIdentifier *&pID);
		virtual bool OnReleaseRasterizerState(SKResourceIdentifier * pRasterizerStateID);

		virtual bool OnLoadSamplerState(SKSamplerState * pSamplerState, SKResourceIdentifier *&pID);
		virtual bool OnReleaseSamplerState(SKResourceIdentifier * pSamplerStateID);

		virtual unsigned int SetVertexFormat(SKVertexFormat * pVertexFormat);

		virtual void * Lock(SKVertexBuffer * pVertexBuffer);
		virtual void UnLock(SKVertexBuffer * pVertexBuffer);

		virtual void * Lock(SKIndexBuffer * pIndexBuffer);
		virtual void UnLock(SKIndexBuffer * pIndexBuffer);

		virtual void * Lock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace);
		virtual void UnLock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace);

		virtual void * Lock(SKRenderTarget * pRenderTarget);
		virtual void UnLock(SKRenderTarget * pRenderTarget);

		virtual void * Lock(SKDepthStencil * pDepthStencil);
		virtual void UnLock(SKDepthStencil * pDepthStencil);

		virtual bool CopyResourceBuffer(VS2DTexture * pSource, SKCubeTexture * pDest, unsigned int uiFace);

	public:
		//shader function
		virtual SKString TexColorBuffer(const SKColorBuffer * pColorBuffer) const;
		virtual SKString Tex2D(const VS2DTexSampler * p2DTexSampler) const;
		virtual SKString Tex1D(const VS1DTexSampler * p1DTexSampler) const;
		virtual SKString Mul(const SKMul * pMul) const;
		virtual SKString Div(const SKDiv * pDiv) const;
		virtual SKString Dot(const SKDot * pDot) const;
		virtual SKString Sub(const SKSub * pSub) const;
		virtual SKString Add(const SKAdd * pAdd) const;
		virtual SKString MakeValue(const SKMakeValue * pMakeValue) const;
		virtual SKString Saturate(const SKSaturate * pSaturate) const;

		virtual SKString Float()const;
		virtual SKString Float2()const;
		virtual SKString Float3()const;
		virtual SKString Float4()const;
		virtual SKString Return()const;

		virtual SKString FloatConst(const SKString & Value1)const;
		virtual SKString Float2Const(const SKString & Value1, const SKString & Value2)const;
		virtual SKString Float3Const(const SKString & Value1,
			const SKString & Value2, const SKString & Value3)const;
		virtual SKString Float4Const(const SKString & Value1, const SKString & Value2,
			const SKString & Value3, const SKString & Value4)const;

		virtual void CustomPointLightUseString(SKString & PointLightUseString, SKString & PointAttenuationDivString);

		virtual void CustomSpotLightUseString(SKString & SpotLightUseString, SKString & SpotAttenuationDivString);


		virtual SKString GetValueElement(const SKPutNode * pPutNode,
			unsigned char uiVE)const;
		virtual SKString GetValueElement(const SKString & InputString,
			unsigned char uiVE)const;

		virtual SKString GetDeclareValue(const SKString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const;
		virtual SKString GetDeclareSampler(const SKString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const;

		//shader custom function
		virtual void CreateVInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateVOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateVUserConstant(SKVShader * pVShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateVFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);

		virtual void SetMaterialVShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKVShader *pVShader);

		virtual void CreatePInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreatePOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreatePUserConstant(SKPShader * pPShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreatePFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);

		virtual void SetMaterialPShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKPShader *pPShader);

		virtual void CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateGUserConstant(SKGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
			SKString & OutString);
		virtual void SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKGShader *pGShader);

		virtual void GetIncludeShader(SKString &OutString);

		virtual void GetDynamicShader(SKString &OutString);


		virtual void SkyLight(const SKString &WorldNormal, const SKString &UpColor, const SKString & DownColor, const SKString & DiffuseColor, SKString & OutString);
		virtual void DirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
			const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void CustomDirectionalLight(int iLightNum, const SKString &CustomString, SKArray<SKString> ShadowString, SKString & OutString)const;

		virtual void OrenNayarDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &RoughnessSquared,
			const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void MinnaertDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Power,
			const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void StraussDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &Smoothness,
			const SKString &Metalness, const SKString &Transparency, const SKString &K,
			const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void ShirleyDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &NU, const SKString &NV, const SKString &Rs,
			const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void SchlickDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
			const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void CookTorranceDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &Roughness, const SKString &C, const SKString &R,
			const SKString &WorldNormal, const SKString &WorldCameraDir, unsigned int uiSpecularType, SKArray<SKString> ShadowString,
			SKString & OutString)const;

		virtual void IsotropicWardDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &Roughness,
			const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void AnisotropicWardDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &RoughnessX, const SKString &RoughnessY,
			const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;

		virtual void PointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void CustomPointLight(int iLightNum, const SKString &CustomString, const SKString & WorldPos, SKArray<SKString> ShadowString, SKString & OutString)const;

		virtual void OrenNayarPointLight(int iLightNum, const SKString &Diffuse, const SKString &RoughnessSquared,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void MinnaertPointLight(int iLightNum, const SKString &Diffuse, const SKString &Power,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void StraussPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &Smoothness,
			const SKString &Metalness, const SKString &Transparency, const SKString &K,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void ShirleyPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &NU, const SKString &NV, const SKString &Rs,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void SchlickPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void CookTorrancePointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &Roughness, const SKString &C, const SKString &R,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, unsigned int uiSpecularType, SKArray<SKString> ShadowString,
			SKString & OutString)const;

		virtual void IsotropicWardPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &Roughness,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void AnisotropicWardPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &RoughnessX, const SKString &RoughnessY,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;

		virtual void SpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void CustomSpotLight(int iLightNum, const SKString &CustomString, const SKString & WorldPos, SKArray<SKString> ShadowString, SKString & OutString)const;
		virtual void OrenNayarSpotLight(int iLightNum, const SKString &Diffuse, const SKString &RoughnessSquared,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void MinnaertSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Power,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void StraussSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &Smoothness,
			const SKString &Metalness, const SKString &Transparency, const SKString &K,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void ShirleySpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &NU, const SKString &NV, const SKString &Rs,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void SchlickSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void CookTorranceSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &Roughness, const SKString &C, const SKString &R,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, unsigned int uiSpecularType, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void IsotropicWardSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &Roughness,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
			SKString & OutString)const;
		virtual void AnisotropicWardSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
			const SKString &RoughnessX, const SKString &RoughnessY,
			const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
			SKString & OutString)const;

		virtual void LocalToWorldPos(const SKString & LocalPos, SKString &OutString)const;
		virtual void WorldToViewPos(const SKString & LocalPos, SKString &OutString)const;

		virtual void LocalToWorldNormal(const SKString & LocalNormal, SKString &OutString)const;

		virtual void WorldToViewNormal(const SKString & WorldNormal, SKString &OutString)const;

		virtual void GetWorldViewDir(SKString &OutString)const;

		virtual void LocalToViewNormal(const SKString & LocalNormal, SKString &OutString)const;
		virtual void TransProjPos(const SKString & Pos, const SKString & Matrix, SKString &OutString)const;
		virtual void BumpNormal(const SKString &TexNormal, SKString &OutString)const;

		virtual void ComputeDir(const SKString & Orgin, const SKString & End, SKString &OutString)const;
		virtual void ComputeLength(const SKString & Orgin, const SKString & End, SKString &OutString)const;
		virtual void ComputeBoneVector(const SKString& BoneIndex, const SKString & BoneWeight, const SKString &BoneVector,
			const  SKString & U, const SKString & V, const SKString &N, SKString &OutString)const;

		virtual void BoneTranPos(const SKString & LocalPos, const  SKString & U, const SKString & V, const SKString &N, SKString &OutString)const;
		virtual void BoneTranNormal(const SKString & LocalNormal, const  SKString & U, const SKString & V, const SKString &N, SKString &OutString)const;

		virtual void DecodeNormal1(const SKString & Normal, SKString &OutString) const;
		virtual void RecodeNormal1(const SKString & Normal, SKString &OutString) const;
		virtual void DecodeNormal2(const SKString & Normal, const SKString & Tengent, SKString &OutString) const;

		virtual void DecodeNormal3(const SKString & Normal, SKString &OutString) const;
		virtual void RecodeNormal3(const SKString & Normal, SKString &OutString) const;

		virtual void Reciprocal(const SKString & Value, SKString &OutString)const;
		virtual void Clip(const SKString & ClipValue, SKString &OutString)const;
		virtual void GreaterZeroPow(const SKString & Value, SKREAL Exp, SKString &OutString) const;
		virtual void SaturatePow(const SKString & Value, SKREAL Exp, SKString &OutString) const;
		virtual void Saturate(const SKString & Value, SKString &OutString)const;

		virtual void PointLightCubeShadow(const SKString & PointLightName, const SKString & CubeShadowSampler, const SKString & ViewMatrix,
			const SKString & WorldPos, SKString& OutString)const;

		virtual void PointLightCubeShadow(const SKString & PointLightName, const SKString & CubeShadowSampler,
			const SKString & WorldPos, SKString& OutString)const;

		virtual void PointLightVolumeShadow(const SKString & PointLightName, const SKString & VolumeShadowSampler,
			const SKString & WorldPos, SKString& OutString)const;

		virtual void PointLightDualParaboloidShadow(const SKString & PointLightName, const SKString & ShadowSampler,
			const SKString & WorldPos, SKString& OutString)const;
		virtual void DirectionLightVolumeShadow(const SKString & DirLightName, const SKString & VolumeShadowSampler,
			const SKString & WorldPos, SKString& OutString)const;

		virtual void DirectionLightCSMShadow(const SKString & DirLightName, const SKString & ShadowSampler,
			const SKString & WorldPos, const SKString & ViewPos, const SKString &FarZ, SKString& OutString)const;

		virtual void DirectionLightShadow(const SKString & DirLightName, const SKString & ShadowSampler,
			const SKString & WorldPos, SKString& OutString)const;

		virtual void SpotLightShadow(const SKString & SpotLightName, const SKString & ShadowSampler,
			const SKString & WorldPos, SKString& OutString)const;

		virtual void TranLightToTemp(SKArray<SKLight *> LightArray, SKString & OutString)const;

		virtual void GetLightFunction(SKArray<SKLight *> LightArray, const SKString & WorldPos, SKString & OutString)const;

		virtual void EncodeReflect(const SKString & Mip, const SKString & Pow, SKString& OutString);

		virtual void DecodeReflect(const SKString & Mip, const SKString & Pow, const SKString & Value, SKString& OutString);

	};
	
}
#endif
