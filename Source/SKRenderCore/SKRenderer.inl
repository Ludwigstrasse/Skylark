FORCEINLINE DWORD SKRenderer::ShaderVersion(UINT Major,UINT Minor)
{
	return (0xFFFE0000|((Major)<<8)|(Minor));
}
FORCEINLINE unsigned int SKRenderer::GetScreenWith()const
{	
	return m_uiScreenWidth;
}
FORCEINLINE unsigned int SKRenderer::GetScreenHeight()const
{
	return m_uiScreenHeight;
}
FORCEINLINE unsigned int SKRenderer::GetDisplayFormat()const
{
	return m_uiDisplayFormat;
}
FORCEINLINE unsigned int SKRenderer::GetCurAnisotropy()const
{
	return m_uiCurAnisotropy;
}
FORCEINLINE unsigned int SKRenderer::GetCurMultisample()const
{	
	return m_uiCurMultisample;
}
FORCEINLINE unsigned int SKRenderer::GetMaxMultisample()const
{
	return m_uiMaxMultisample;
}
FORCEINLINE unsigned int SKRenderer::GetDepthStencilFormat()const
{
	return m_uiDepthStencilFormat;
}
FORCEINLINE bool SKRenderer::IsWindowed()const
{
	return m_bWindowed;
}
FORCEINLINE unsigned int SKRenderer::WindoweNum()const
{
	if (m_bWindowed)
	{
		return m_iNumChildWindow;
	}
	return 0;
}
FORCEINLINE unsigned int SKRenderer::GetMaxTextureWidth()const
{
	return m_uiMaxTextureWidth;
}
FORCEINLINE unsigned int SKRenderer::GetMaxTextureHeight()const
{
	return m_uiMaxTextureHeight;
}
FORCEINLINE unsigned int SKRenderer::GetMaxUseClipPlane()const
{
	return m_uiMaxUseClipPlane;
}
FORCEINLINE unsigned int SKRenderer::GetMaxVTexture()const
{
	return m_uiMaxVTexture;
}
FORCEINLINE unsigned int SKRenderer::GetMaxGTexture()const
{
	return m_uiMaxGTexture;
}
FORCEINLINE unsigned int SKRenderer::GetMaxTexture()const
{
	return m_uiMaxTexture;
}
FORCEINLINE unsigned int SKRenderer::GetBufferFormat()const
{
	return m_uiBufferFormat;
}
FORCEINLINE unsigned int SKRenderer::GetMaxAnisotropy()const
{
	return m_uiMaxAnisotropy;
}
FORCEINLINE unsigned int SKRenderer::GetMaxRTNum()const
{
	return m_uiMaxRTNum;
}
FORCEINLINE SKString SKRenderer::GetAdapterName()const
{
	return m_AdapterName;
}
//----------------------------------------------------------------------------
FORCEINLINE void SKRenderer::SetClearColor (const SKColorRGBA& ClearColor)
{
	m_ClearColor = ClearColor;

}
//----------------------------------------------------------------------------
FORCEINLINE const SKColorRGBA& SKRenderer::GetClearColor () const
{
	return m_ClearColor;
}
//----------------------------------------------------------------------------
FORCEINLINE void SKRenderer::SetClearDepth (SKREAL fClearDepth)
{
	m_fClearDepth = fClearDepth;

}
//----------------------------------------------------------------------------
FORCEINLINE SKREAL SKRenderer::GetClearDepth () const
{
	return m_fClearDepth;
}
//----------------------------------------------------------------------------
FORCEINLINE void SKRenderer::SetClearStencil (unsigned int uiClearStencil)
{
	m_uiClearStencil = uiClearStencil;

}
//----------------------------------------------------------------------------
FORCEINLINE unsigned int SKRenderer::GetClearStencil () const
{
	return m_uiClearStencil;
}
FORCEINLINE unsigned int SKRenderer::GetCurRTWidth()const
{
	return m_uiCurRTWidth;
}
FORCEINLINE unsigned int SKRenderer::GetCurRTHeight()const
{
	return m_uiCurRTHeight;
}
FORCEINLINE unsigned int SKRenderer::GetCurRTMultisampler()const
{
	return m_uiCurRTMultisampler;
}
FORCEINLINE unsigned int SKRenderer::GetSuitableMultisample(unsigned int Multisample)const
{
	return Multisample > m_uiMaxMultisample ? m_uiMaxMultisample : Multisample;
}

