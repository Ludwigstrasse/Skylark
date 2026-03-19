FORCEINLINE SKDataBuffer * SKVertexBuffer::GetPositionDate(unsigned int uiLevel)const
{
	if(uiLevel < (unsigned int)m_pDate[SKVertexFormat::VF_POSITION].GetNum())
		return m_pDate[SKVertexFormat::VF_POSITION][uiLevel];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetNormalDate(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pDate[SKVertexFormat::VF_NORMAL].GetNum())
		return m_pDate[SKVertexFormat::VF_NORMAL][uiLevel];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetPSizeDate()const
{	
	if(m_pDate[SKVertexFormat::VF_PSIZE].GetNum())
		return m_pDate[SKVertexFormat::VF_PSIZE][0];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetColorDate(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pDate[SKVertexFormat::VF_COLOR].GetNum())
		return m_pDate[SKVertexFormat::VF_COLOR][uiLevel];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetTexCoordDate(unsigned int uiLevel)const
{
	if(uiLevel <  (unsigned int)m_pDate[SKVertexFormat::VF_TEXCOORD].GetNum())
		return m_pDate[SKVertexFormat::VF_TEXCOORD][uiLevel];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetBlendWeightDate()const
{
	if(m_pDate[SKVertexFormat::VF_BLENDWEIGHT].GetNum())
		return m_pDate[SKVertexFormat::VF_BLENDWEIGHT][0];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetBlendIndicesDate()const
{
	if(m_pDate[SKVertexFormat::VF_BLENDINDICES].GetNum())
		return m_pDate[SKVertexFormat::VF_BLENDINDICES][0];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetTangentDate()const
{
	if(m_pDate[SKVertexFormat::VF_TANGENT].GetNum())
		return m_pDate[SKVertexFormat::VF_TANGENT][0];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetBinormalDate()const
{
	if(m_pDate[SKVertexFormat::VF_BINORMAL].GetNum())
		return m_pDate[SKVertexFormat::VF_BINORMAL][0];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetFogDate()const
{
	if(m_pDate[SKVertexFormat::VF_FOG].GetNum())
		return m_pDate[SKVertexFormat::VF_FOG][0];
	else
		return NULL;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetDepthDate()const
{
	if(m_pDate[SKVertexFormat::VF_DEPTH].GetNum())
		return m_pDate[SKVertexFormat::VF_DEPTH][0];
	else
		return NULL;
}
FORCEINLINE unsigned int SKVertexBuffer::GetPositionLevel()const
{
	return GetSemanticsNum(SKVertexFormat::VF_POSITION);
}
FORCEINLINE unsigned int SKVertexBuffer::GetColorLevel()const
{
	return GetSemanticsNum(SKVertexFormat::VF_COLOR);
}
FORCEINLINE unsigned int SKVertexBuffer::GetNormalLevel()const
{
	return GetSemanticsNum(SKVertexFormat::VF_NORMAL);
}
FORCEINLINE unsigned int SKVertexBuffer::GetTexCoordLevel()const
{
	return GetSemanticsNum(SKVertexFormat::VF_TEXCOORD);
}
FORCEINLINE unsigned int SKVertexBuffer::GetVertexNum()const
{
	return m_uiVertexNum;
}
FORCEINLINE SKVertexFormat * SKVertexBuffer::GetVertexFormat()const
{
	return m_pVertexFormat;
}
FORCEINLINE unsigned int SKVertexBuffer::GetOneVertexSize()const
{
	return m_uiOneVertexSize;
}
FORCEINLINE SKDataBuffer * SKVertexBuffer::GetDate(unsigned int uiVF,unsigned int uiLevel)const
{
	if(uiVF >= SKVertexFormat::VF_MAX)
		return NULL;
	if(uiLevel >= (unsigned int)m_pDate[uiVF].GetNum())
		return NULL;
	return m_pDate[uiVF][uiLevel];
}
FORCEINLINE unsigned int SKVertexBuffer::GetLevel(unsigned int uiVF)const
{
	if (uiVF >= SKVertexFormat::VF_MAX)
		return 0;
	else
		return GetSemanticsNum(uiVF);
}
FORCEINLINE bool SKVertexBuffer::GetLockDateOffSet(unsigned int uiVF,unsigned int uiLevel,unsigned int &uiOffSet)const
{
	if (!m_pVertexFormat)
	{
		SKDataBuffer * pDate = NULL;
		for(unsigned int i = 0 ; i < uiVF ; i++)
		{
			for (unsigned int j = 0 ; j < m_pDate[i].GetNum() ; j++)
			{
				pDate = GetDate(i,j);
				if(pDate)
					uiOffSet += pDate->GetStride();
			}
		}
		for (unsigned int j = 0 ; j < uiLevel ; j++)
		{
			pDate = GetDate(uiVF,j);
			if(pDate)
				uiOffSet += pDate->GetStride();
		}
		if (!pDate)
		{
			return false;
		}
		return true;
	}
	else
	{
		for (unsigned int i = 0 ;i < m_pVertexFormat->m_FormatArray.GetNum() ; i++)
		{
			if(m_pVertexFormat->m_FormatArray[i].Semantics == uiVF)
			{
				if (m_pVertexFormat->m_FormatArray[i].SemanticsIndex == uiLevel)
				{
					uiOffSet = m_pVertexFormat->m_FormatArray[i].OffSet;
					return true;
				}
			}
		}
		return false;
	}
	
}
FORCEINLINE void *SKVertexBuffer::GetLockDate(unsigned int uiVF,unsigned int uiLevel)const
{
	if(uiVF >= SKVertexFormat::VF_MAX || !m_pLockDate)
		return NULL;
	if(GetSemanticsNum(uiVF) <= uiLevel)
		return NULL;

	unsigned int uiOffSet = 0;
	if(!GetLockDateOffSet(uiVF,uiLevel,uiOffSet))
		return	NULL;	
	return (unsigned char *)m_pLockDate + uiOffSet; 
}
FORCEINLINE void *SKVertexBuffer::GetLockPositionDate(unsigned int uiLevel)const
{
	return GetLockDate(SKVertexFormat::VF_POSITION,uiLevel);
}
FORCEINLINE void *SKVertexBuffer::GetLockNormalDate(unsigned int uiLevel)const
{
	return GetLockDate(SKVertexFormat::VF_NORMAL,uiLevel);
}
FORCEINLINE void *SKVertexBuffer::GetLockPSizeDate()const
{
	return GetLockDate(SKVertexFormat::VF_PSIZE,0);
}
FORCEINLINE void *SKVertexBuffer::GetLockColorDate(unsigned int uiLevel)const
{
	return GetLockDate(SKVertexFormat::VF_COLOR,uiLevel);
}
FORCEINLINE void *SKVertexBuffer::GetLockBlendWeightDate()const
{
	return GetLockDate(SKVertexFormat::VF_BLENDWEIGHT,0);
}
FORCEINLINE void *SKVertexBuffer::GetLockBlendIndicesDate()const
{
	return GetLockDate(SKVertexFormat::VF_BLENDINDICES,0);	
}
FORCEINLINE void *SKVertexBuffer::GetLockTangentDate()const
{
	return GetLockDate(SKVertexFormat::VF_TANGENT,0);
}
FORCEINLINE void *SKVertexBuffer::GetLockBinormalDate()const
{
	return GetLockDate(SKVertexFormat::VF_BINORMAL,0);
}
FORCEINLINE void *SKVertexBuffer::GetLockFogDate()const
{
	return GetLockDate(SKVertexFormat::VF_FOG,0);
}
FORCEINLINE void *SKVertexBuffer::GetLockDepthDate()const
{
	return GetLockDate(SKVertexFormat::VF_DEPTH,0);
}
FORCEINLINE void *SKVertexBuffer::GetLockTexCoordDate(unsigned int uiLevel)const
{
	return GetLockDate(SKVertexFormat::VF_TEXCOORD,uiLevel);
}
FORCEINLINE bool SKVertexBuffer::HavePositionInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(SKVertexFormat::VF_POSITION) > uiLevel;
}
FORCEINLINE bool SKVertexBuffer::HaveNormalInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(SKVertexFormat::VF_NORMAL) > uiLevel;
}
FORCEINLINE bool SKVertexBuffer::HavePSizeInfo()const
{
	return GetSemanticsNum(SKVertexFormat::VF_PSIZE) > 0;
}
FORCEINLINE bool SKVertexBuffer::HaveColorInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(SKVertexFormat::VF_COLOR) > uiLevel;
}
FORCEINLINE bool SKVertexBuffer::HaveBlendWeightInfo()const
{
	return GetSemanticsNum(SKVertexFormat::VF_BLENDWEIGHT) > 0;
}
FORCEINLINE bool SKVertexBuffer::HaveBlendIndicesInfo()const
{
	return GetSemanticsNum(SKVertexFormat::VF_BLENDINDICES) > 0;
}
FORCEINLINE bool SKVertexBuffer::HaveTangentInfo()const
{
	return GetSemanticsNum(SKVertexFormat::VF_TANGENT) > 0;
}
FORCEINLINE bool SKVertexBuffer::HaveBinormalInfo()const
{
	return GetSemanticsNum(SKVertexFormat::VF_BINORMAL) > 0;
}
FORCEINLINE bool SKVertexBuffer::HaveFogInfo()const
{
	return GetSemanticsNum(SKVertexFormat::VF_FOG) > 0;
}
FORCEINLINE bool SKVertexBuffer::HaveDepthInfo()const
{
	return GetSemanticsNum(SKVertexFormat::VF_DEPTH) > 0;
}
FORCEINLINE bool SKVertexBuffer::HaveTexCoordInfo(unsigned int uiLevel)const
{
	return GetSemanticsNum(SKVertexFormat::VF_TEXCOORD) > uiLevel;
}
FORCEINLINE unsigned int SKVertexBuffer::PositionChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(SKVertexFormat::VF_POSITION,uiLevel);
}
FORCEINLINE unsigned int SKVertexBuffer::NormalChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(SKVertexFormat::VF_NORMAL,uiLevel);
}
FORCEINLINE unsigned int SKVertexBuffer::PSizeChannel()const
{
	return GetSemanticsChannel(SKVertexFormat::VF_PSIZE,0);
}
FORCEINLINE unsigned int SKVertexBuffer::ColorChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(SKVertexFormat::VF_COLOR,uiLevel);
}
FORCEINLINE unsigned int SKVertexBuffer::BlendWeightChannel()const
{
	return GetSemanticsChannel(SKVertexFormat::VF_BLENDWEIGHT,0);
}
FORCEINLINE unsigned int SKVertexBuffer::BlendIndicesChannel()const
{
	return GetSemanticsChannel(SKVertexFormat::VF_BLENDINDICES,0);
}
FORCEINLINE unsigned int SKVertexBuffer::TangentChannel()const
{
	return GetSemanticsChannel(SKVertexFormat::VF_TANGENT,0);
}
FORCEINLINE unsigned int SKVertexBuffer::BinormalChannel()const
{
	return GetSemanticsChannel(SKVertexFormat::VF_BINORMAL,0);
}
FORCEINLINE unsigned int SKVertexBuffer::FogChannel()const
{
	return GetSemanticsChannel(SKVertexFormat::VF_FOG,0);
}
FORCEINLINE unsigned int SKVertexBuffer::DepthChannel()const
{
	return GetSemanticsChannel(SKVertexFormat::VF_DEPTH,0);
}
FORCEINLINE unsigned int SKVertexBuffer::TexCoordChannel(unsigned int uiLevel)const
{
	return GetSemanticsChannel(SKVertexFormat::VF_TEXCOORD,uiLevel);
}
FORCEINLINE unsigned int SKVertexBuffer::NormalDateType(unsigned int uiLevel)const
{
	return GetSemanticsDateType(SKVertexFormat::VF_NORMAL,uiLevel);
}
FORCEINLINE unsigned int SKVertexBuffer::TangentDateType()const
{
	return GetSemanticsDateType(SKVertexFormat::VF_TANGENT,0);
}
FORCEINLINE unsigned int SKVertexBuffer::BinormalDateType()const
{
	return GetSemanticsDateType(SKVertexFormat::VF_BINORMAL,0);
}