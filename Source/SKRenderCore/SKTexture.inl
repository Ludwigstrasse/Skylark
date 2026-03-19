FORCEINLINE unsigned int SKTexture::GetFormatType()const
{
	return m_uiFormatType;
}
FORCEINLINE unsigned int SKTexture::GetBytePerPixel()const
{
	return SKRenderer::GetBytesPerPixel(m_uiFormatType);
}
FORCEINLINE unsigned int SKTexture::GetChannelPerPixel()const
{
	return SKRenderer::GetChannelPerPixel(m_uiFormatType);
}
FORCEINLINE unsigned int SKTexture::GetMipLevel()const
{
	return m_uiMipLevel;
}
FORCEINLINE unsigned char * SKTexture::GetBuffer(unsigned int uiLevel)const
{

	return m_pBufferArray[uiLevel];
}
