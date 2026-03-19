#include "SKNullRenderer.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
SKNullRenderer::SKNullRenderer()
{
	
}
SKNullRenderer::~SKNullRenderer()
{

}
void SKNullRenderer::ClearBuffers(unsigned int uiClearFlag){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ClearBackBuffer(){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ClearZBuffer(){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ClearStencilBuffer(){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ClearBuffers(){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
	int iHeight){
	SKMAC_ASSERT(0); return;
}
void SKNullRenderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight){
	SKMAC_ASSERT(0); return;
}
void SKNullRenderer::ClearZBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight){
	SKMAC_ASSERT(0); return;
}
void SKNullRenderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight){
	SKMAC_ASSERT(0); return;
}
void SKNullRenderer::ClearBuffers(int iXPos, int iYPos, int iWidth,
	int iHeight){
	SKMAC_ASSERT(0); return;
}
bool SKNullRenderer::SetViewPort(SKViewPort *pViewPort){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::UseWindow(int uiWindowID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::CooperativeLevel(){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::BeginRendering(){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::EndRendering(){SKMAC_ASSERT(0);return 0;}

void SKNullRenderer::DeviceLost(){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ResetDevice(){SKMAC_ASSERT(0);return ;}
bool SKNullRenderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow){SKMAC_ASSERT(0);return 0 ;}
bool SKNullRenderer::DrawMesh(SKGeometry * pGeometry, SKRenderState * pRenderState, SKVShader * pVShader, SKPShader * pPShader, SKGShader * pGShader){ SKMAC_ASSERT(0); return 0; }
bool SKNullRenderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4]){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum){
	SKMAC_ASSERT(0); return 0;
}
const SKAdapterInfo* SKNullRenderer::GetAdapterInfo(unsigned int & uiAdapterNum)
{
	SKMAC_ASSERT(0); return NULL;
}
unsigned int SKNullRenderer::SetBlendState(SKBlendState * pBlendState, bool bForceSet){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetDepthStencilState(SKDepthStencilState * pDepthStencilState, bool bForceSet){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetRasterizerState(SKRasterizerState * pRasterizerState, bool bForceSet){ SKMAC_ASSERT(0); return 0; }
void SKNullRenderer::SetClipPlane(SKArray<SKPlane3> &Plane, bool bEnable){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::SetScissorRect(SKArray<SKRect2> &Rect, bool bEnable){SKMAC_ASSERT(0);return ;}

unsigned int SKNullRenderer::SetVShader(SKVShader * pVShader){SKMAC_ASSERT(0);return 0;}
unsigned int SKNullRenderer::SetPShader(SKPShader * pPShader){SKMAC_ASSERT(0);return 0;}
unsigned int SKNullRenderer::SetGShader(SKGShader * pGShader){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetDShader(SKDShader * pDShader){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetHShader(SKHShader * pHShader){ SKMAC_ASSERT(0); return 0; }

unsigned int SKNullRenderer::SetVBuffer(SKVertexBuffer * pVBuffer){SKMAC_ASSERT(0);return 0;}
unsigned int SKNullRenderer::SetIBuffer(SKIndexBuffer* pIBuffer){SKMAC_ASSERT(0);return 0;}
unsigned int SKNullRenderer::SetTexture(SKTexture* pTexture, unsigned int i){SKMAC_ASSERT(0);return 0;}
unsigned int SKNullRenderer::SetVTexture(SKTexture* pTexture, unsigned int i){SKMAC_ASSERT(0);return 0;}
unsigned int SKNullRenderer::SetGTexture(SKTexture* pTexture, unsigned int i){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetDTexture(SKTexture* pTexture, unsigned int i){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetHTexture(SKTexture* pTexture, unsigned int i){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetVSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetGSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetDSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet){ SKMAC_ASSERT(0); return 0; }
unsigned int SKNullRenderer::SetHSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet){ SKMAC_ASSERT(0); return 0; }

bool SKNullRenderer::SetDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::EndDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::IsSupportMulBufferSwtich()const{ SKMAC_ASSERT(0); return 0; }
bool SKNullRenderer::IsSupportGS()const
{
	SKMAC_ASSERT(0); return 0;
}
void SKNullRenderer::SetVShaderConstant(SKVShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetVShaderSampler(SKVShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetPShaderConstant(SKPShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetPShaderSampler(SKPShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetGShaderConstant(SKGShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetGShaderSampler(SKGShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetDShaderConstant(SKDShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetDShaderSampler(SKDShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetHShaderConstant(SKHShader * pShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::SetHShaderSampler(SKHShader * pShader){ SKMAC_ASSERT(0); return; }

bool SKNullRenderer::SetVShaderProgram(SKVShader*	pVShaderProgram){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::SetPShaderProgram(SKPShader*pPShaderProgram){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::SetGShaderProgram(SKGShader*pGShaderProgram){ SKMAC_ASSERT(0); return 0; }
bool SKNullRenderer::SetDShaderProgram(SKDShader*	pDShaderProgram){ SKMAC_ASSERT(0); return 0; }
bool SKNullRenderer::SetHShaderProgram(SKHShader*	pHShaderProgram){ SKMAC_ASSERT(0); return 0; }
//Load Release
bool SKNullRenderer::OnLoadVShaderProgram(SKVShader * pVShaderProgram, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseVShaderProgram(SKResourceIdentifier * pVShaderProgramID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadPShaderProgram(SKPShader* pPShaderProgram, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleasePShaderProgram(SKResourceIdentifier* pPShaderProgramID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadGShaderProgram(SKGShader* pGShaderProgram, SKResourceIdentifier *&pID){ SKMAC_ASSERT(0); return 0; }
bool SKNullRenderer::OnReleaseGShaderProgram(SKResourceIdentifier* pGShaderProgramID){ SKMAC_ASSERT(0); return 0; }

bool SKNullRenderer::OnLoadDShaderProgram(SKDShader * pDShaderProgram, SKResourceIdentifier *&pID){ SKMAC_ASSERT(0); return 0; }
bool SKNullRenderer::OnReleaseDShaderProgram(SKResourceIdentifier * pDShaderProgramID){ SKMAC_ASSERT(0); return 0; }

bool SKNullRenderer::OnLoadHShaderProgram(SKHShader * pHShaderProgram, SKResourceIdentifier *&pID){ SKMAC_ASSERT(0); return 0; }
bool SKNullRenderer::OnReleaseHShaderProgram(SKResourceIdentifier * pHShaderProgramID) {SKMAC_ASSERT(0); return 0; }

bool SKNullRenderer::OnLoadTexture(SKTexture * pTexture, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseTexture(SKResourceIdentifier * pTextureID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadDepthStencil(SKDepthStencil * pDepthStencil, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseDepthStencil(SKResourceIdentifier * pDepthStencilID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadRenderTarget(SKRenderTarget * pRenderTarget, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseRenderTarget(SKResourceIdentifier * pRenderTarget){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadVBufferDate(SKVertexBuffer * pVBuffer, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseVBufferDate(SKResourceIdentifier* pVBufferID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadVBufferFormat(SKVertexFormat * pVertexFormat, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseVBufferFormat(SKResourceIdentifier* pVBufferID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadIBuffer(SKIndexBuffer* pIBuffer, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseIBuffer(SKResourceIdentifier* pIBufferID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadBlendState(SKBlendState * pBlendState, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseBlendState(SKResourceIdentifier * pBlendStateID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadDepthStencilState(SKDepthStencilState * pDepthStencilState, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseDepthStencilState(SKResourceIdentifier * pDepthStencilStateID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadRasterizerState(SKRasterizerState * pRasterizerState, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseRasterizerState(SKResourceIdentifier * pRasterizerStateID){SKMAC_ASSERT(0);return 0;}

bool SKNullRenderer::OnLoadSamplerState(SKSamplerState * pSamplerState, SKResourceIdentifier *&pID){SKMAC_ASSERT(0);return 0;}
bool SKNullRenderer::OnReleaseSamplerState(SKResourceIdentifier * pSamplerStateID){SKMAC_ASSERT(0);return 0;}

unsigned int SKNullRenderer::SetVertexFormat(SKVertexFormat * pVertexFormat){SKMAC_ASSERT(0);return 0;}

void * SKNullRenderer::Lock(SKVertexBuffer * pVertexBuffer){SKMAC_ASSERT(0);return NULL;}
void SKNullRenderer::UnLock(SKVertexBuffer * pVertexBuffer){SKMAC_ASSERT(0);return ;}

void * SKNullRenderer::Lock(SKIndexBuffer * pIndexBuffer){ SKMAC_ASSERT(0); return NULL; }
void SKNullRenderer::UnLock(SKIndexBuffer * pIndexBuffer){SKMAC_ASSERT(0);return ;}

void * SKNullRenderer::Lock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace){ SKMAC_ASSERT(0); return NULL; }
void SKNullRenderer::UnLock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace){SKMAC_ASSERT(0);return ;}
bool SKNullRenderer::CopyResourceBuffer(VS2DTexture * pSource, SKCubeTexture * pDest, unsigned int uiFace){SKMAC_ASSERT(0);return 0;}
void * SKNullRenderer::Lock(SKRenderTarget * pRenderTarget){ SKMAC_ASSERT(0); return NULL; };
void SKNullRenderer::UnLock(SKRenderTarget * pRenderTarget){ SKMAC_ASSERT(0); return; };

void * SKNullRenderer::Lock(SKDepthStencil * pDepthStencil){ SKMAC_ASSERT(0); return NULL; };
void SKNullRenderer::UnLock(SKDepthStencil * pDepthStencil){ SKMAC_ASSERT(0); return; };

SKString SKNullRenderer::Tex2D(const VS2DTexSampler * p2DTexSampler) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Tex1D(const VS1DTexSampler * p1DTexSampler) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::TexColorBuffer(const SKColorBuffer * pColorBuffer) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Mul(const SKMul * pMul) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Saturate(const SKSaturate * pSaturate) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Div(const SKDiv * pDiv) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Dot(const SKDot * pDot) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Sub(const SKSub * pSub) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Add(const SKAdd * pAdd) const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::MakeValue(const SKMakeValue * pMakeValue)const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }

SKString SKNullRenderer::Float()const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Float2()const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Float3()const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Float4()const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Return()const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }

SKString SKNullRenderer::GetDeclareValue(const SKString &ShowName, unsigned int uiValueType, unsigned int uiRegisterIndex)const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::GetDeclareSampler(const SKString &ShowName, unsigned int SamplerType, unsigned int uiRegisterIndex)const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }

SKString SKNullRenderer::FloatConst(const SKString & Value1)const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Float2Const(const SKString & Value1, const SKString & Value2)const{ SKMAC_ASSERT(0); return SKString::ms_StringNULL; }
SKString SKNullRenderer::Float3Const(const SKString & Value1,
	const SKString & Value2, const SKString & Value3)const{
	SKMAC_ASSERT(0); return SKString::ms_StringNULL;
}
SKString SKNullRenderer::Float4Const(const SKString & Value1, const SKString & Value2,
	const SKString & Value3, const SKString & Value4)const{
	SKMAC_ASSERT(0); return SKString::ms_StringNULL;
}

void SKNullRenderer::CustomPointLightUseString(SKString & PointLightUseString, SKString & PointAttenuationDivString){SKMAC_ASSERT(0);return ;}

void SKNullRenderer::CustomSpotLightUseString(SKString & SpotLightUseString, SKString & SpotAttenuationDivString){SKMAC_ASSERT(0);return ;}




//shader custom function
void SKNullRenderer::CreateVInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreateVOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreateVUserConstant(SKVShader * pVShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreateVFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}

void SKNullRenderer::SetMaterialVShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKVShader *pVShader){SKMAC_ASSERT(0);return ;}

void SKNullRenderer::CreatePInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreatePOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreatePUserConstant(SKPShader * pPShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreatePFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}

void SKNullRenderer::SetMaterialPShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKPShader *pPShader){SKMAC_ASSERT(0);return ;}

void SKNullRenderer::CreateGInputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreateGOutputDeclare(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreateGUserConstant(SKGShader * pGShader, MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CreateGFuntion(MaterialShaderPara &MSPara, unsigned int uiPassType,
	SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::SetMaterialGShaderConstant(MaterialShaderPara &MSPara, unsigned int uiPassType, SKGShader *pGShader){ SKMAC_ASSERT(0); return; }
void SKNullRenderer::GetIncludeShader(SKString &OutString){SKMAC_ASSERT(0);return ;}

void SKNullRenderer::GetDynamicShader(SKString &OutString){SKMAC_ASSERT(0);return ;}


void SKNullRenderer::SkyLight(const SKString &WorldNormal, const SKString &UpColor, const SKString & DownColor, const SKString & DiffuseColor, SKString & OutString){SKMAC_ASSERT(0);return ;}
void SKNullRenderer::DirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
	const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CustomDirectionalLight(int iLightNum, const SKString &CustomString, SKArray<SKString> ShadowString, SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::OrenNayarDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &RoughnessSquared,
	const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::MinnaertDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Power,
	const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::StraussDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &Smoothness,
	const SKString &Metalness, const SKString &Transparency, const SKString &K,
	const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ShirleyDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &NU, const SKString &NV, const SKString &Rs,
	const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::SchlickDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
	const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CookTorranceDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &Roughness, const SKString &C, const SKString &R,
	const SKString &WorldNormal, const SKString &WorldCameraDir, unsigned int uiSpecularType, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::IsotropicWardDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &Roughness,
	const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::AnisotropicWardDirectionalLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &RoughnessX, const SKString &RoughnessY,
	const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::PointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CustomPointLight(int iLightNum, const SKString &CustomString, const SKString & WorldPos, SKArray<SKString> ShadowString, SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::OrenNayarPointLight(int iLightNum, const SKString &Diffuse, const SKString &RoughnessSquared,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::MinnaertPointLight(int iLightNum, const SKString &Diffuse, const SKString &Power,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::StraussPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &Smoothness,
	const SKString &Metalness, const SKString &Transparency, const SKString &K,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ShirleyPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &NU, const SKString &NV, const SKString &Rs,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::SchlickPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CookTorrancePointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &Roughness, const SKString &C, const SKString &R,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, unsigned int uiSpecularType, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::IsotropicWardPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &Roughness,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::AnisotropicWardPointLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &RoughnessX, const SKString &RoughnessY,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::SpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CustomSpotLight(int iLightNum, const SKString &CustomString, const SKString & WorldPos, SKArray<SKString> ShadowString, SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::OrenNayarSpotLight(int iLightNum, const SKString &Diffuse, const SKString &RoughnessSquared,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::MinnaertSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Power,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::StraussSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &Smoothness,
	const SKString &Metalness, const SKString &Transparency, const SKString &K,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ShirleySpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &NU, const SKString &NV, const SKString &Rs,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::SchlickSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular, const SKString &SpecularPow,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::CookTorranceSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &Roughness, const SKString &C, const SKString &R,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, unsigned int uiSpecularType, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::IsotropicWardSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &Roughness,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, bool bLookUpTable, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::AnisotropicWardSpotLight(int iLightNum, const SKString &Diffuse, const SKString &Specular,
	const SKString &RoughnessX, const SKString &RoughnessY,
	const SKString & WorldPos, const SKString &WorldNormal, const SKString &WorldCameraDir, SKArray<SKString> ShadowString,
	SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::LocalToWorldPos(const SKString & LocalPos, SKString &OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::WorldToViewPos(const SKString & LocalPos, SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::LocalToWorldNormal(const SKString & LocalNormal, SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::WorldToViewNormal(const SKString & WorldNormal, SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::GetWorldViewDir(SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::LocalToViewNormal(const SKString & LocalNormal, SKString &OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::TransProjPos(const SKString & Pos, const SKString & Matrix, SKString &OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::BumpNormal(const SKString &TexNormal, SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::ComputeDir(const SKString & Orgin, const SKString & End, SKString &OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ComputeLength(const SKString & Orgin, const SKString & End, SKString &OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::ComputeBoneVector(const SKString& BoneIndex, const SKString & BoneWeight, const SKString &BoneVector,
	const  SKString & U, const SKString & V, const SKString &N, SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::BoneTranPos(const SKString & LocalPos, const  SKString & U, const SKString & V, const SKString &N, SKString &OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::BoneTranNormal(const SKString & LocalNormal, const  SKString & U, const SKString & V, const SKString &N, SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::DecodeNormal1(const SKString & Normal, SKString &OutString) const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::RecodeNormal1(const SKString & Normal, SKString &OutString) const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::DecodeNormal2(const SKString & Normal, const SKString & Tengent, SKString &OutString) const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::DecodeNormal3(const SKString & Normal, SKString &OutString) const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::RecodeNormal3(const SKString & Normal, SKString &OutString) const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::Reciprocal(const SKString & Value, SKString &OutString)const{ SKMAC_ASSERT(0); return; }
void SKNullRenderer::Clip(const SKString & ClipValue, SKString &OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::GreaterZeroPow(const SKString & Value, SKREAL Exp, SKString &OutString) const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::SaturatePow(const SKString & Value, SKREAL Exp, SKString &OutString) const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::Saturate(const SKString & Value, SKString &OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::PointLightCubeShadow(const SKString & PointLightName, const SKString & CubeShadowSampler, const SKString & ViewMatrix,
	const SKString & WorldPos, SKString& OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::PointLightCubeShadow(const SKString & PointLightName, const SKString & CubeShadowSampler,
	const SKString & WorldPos, SKString& OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::PointLightVolumeShadow(const SKString & PointLightName, const SKString & VolumeShadowSampler,
	const SKString & WorldPos, SKString& OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::PointLightDualParaboloidShadow(const SKString & PointLightName, const SKString & ShadowSampler,
	const SKString & WorldPos, SKString& OutString)const{SKMAC_ASSERT(0);return ;}
void SKNullRenderer::DirectionLightVolumeShadow(const SKString & DirLightName, const SKString & VolumeShadowSampler,
	const SKString & WorldPos, SKString& OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::DirectionLightCSMShadow(const SKString & DirLightName, const SKString & ShadowSampler,
	const SKString & WorldPos, const SKString & ViewPos, const SKString &FarZ, SKString& OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::DirectionLightShadow(const SKString & DirLightName, const SKString & ShadowSampler,
	const SKString & WorldPos, SKString& OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::SpotLightShadow(const SKString & SpotLightName, const SKString & ShadowSampler,
	const SKString & WorldPos, SKString& OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::TranLightToTemp(SKArray<SKLight *> LightArray, SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::GetLightFunction(SKArray<SKLight *> LightArray, const SKString & WorldPos, SKString & OutString)const{SKMAC_ASSERT(0);return ;}

void SKNullRenderer::EncodeReflect(const SKString & Mip, const SKString & Pow, SKString& OutString){SKMAC_ASSERT(0);return ;}

void SKNullRenderer::DecodeReflect(const SKString & Mip, const SKString & Pow, const SKString & Value, SKString& OutString){SKMAC_ASSERT(0);return ;}
unsigned int SKNullRenderer::GetTextureCompressLen(unsigned int uiLen)
{
	return Max((unsigned int)1, (unsigned int)(uiLen / 4));
}
SKString SKNullRenderer::GetValueElement(const SKPutNode * pPutNode,
	unsigned char uiVE)const
{
	SKMAC_ASSERT(pPutNode);
	if (!pPutNode)
		return SKString::ms_StringNULL;
	SKString Temp = pPutNode->GetNodeName().GetString();
	if (uiVE > 0)
	{
		if (pPutNode->GetValueType() == SKPutNode::VT_1 && (uiVE & VE_R))
		{
			return Temp;
		}
		else if (pPutNode->GetValueType() == SKPutNode::VT_1)
		{
			SKMAC_ASSERT(0);
			return SKString::ms_StringNULL;
		}
		SKString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		unsigned int Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp += _T(".");

		for (unsigned int i = 0; i < 4; i++)
		{
			if (i <= pPutNode->GetValueType())
			{
				if (uiVE & Mask[i])
				{
					Temp += Value[i];
				}
			}

		}

	}


	return Temp;
}
SKString SKNullRenderer::GetValueElement(const SKString & InputString,
	unsigned char uiVE)const
{
	SKString Temp = InputString;
	if (uiVE > 0)
	{
		SKString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		unsigned int Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp += _T(".");

		for (unsigned int i = 0; i < 4; i++)
		{
			if (uiVE & Mask[i])
			{
				Temp += Value[i];
			}
		}
	}
	return Temp;
}