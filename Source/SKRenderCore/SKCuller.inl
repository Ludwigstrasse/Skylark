FORCEINLINE unsigned int SKCuller::GetPlaneNum()const
{
	return m_uiPlaneNum;

}
FORCEINLINE void SKCuller::SetPlaneState(unsigned int uiPlaneState)
{
	m_uiPlaneState = uiPlaneState;
}
FORCEINLINE unsigned int SKCuller::GetPlaneState()const
{

	return m_uiPlaneState;
}
FORCEINLINE unsigned int SKCuller::GetAllVisibleNum()const
{
	unsigned int uiNum = 0;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < SKT_MAX ; i++)
		{
			uiNum += m_VisibleSet[j][i].GetNum();
		}
	}
	
	return uiNum;
}
FORCEINLINE unsigned int SKCuller::GetVisibleNum(unsigned int uiVisibleSetType,unsigned int uiRenderGroup)const
{
	if (uiVisibleSetType >= SKT_MAX || uiRenderGroup >= RG_MAX)
	{
		return 0;
	}
	return m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum();
}
FORCEINLINE unsigned int SKCuller::GetRenderGroupVisibleNum(unsigned int uiRenderGroup)const
{
	unsigned int uiNum = 0;
	for (unsigned int i = 0 ; i < SKT_MAX ; i++)
	{
		uiNum += m_VisibleSet[uiRenderGroup][i].GetNum();
	}
	return uiNum;
}
FORCEINLINE void SKCuller::ClearVisibleSet()
{
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < SKT_MAX ; i++)
		{
			m_VisibleSet[j][i].Clear();
		}
	}
	
}
FORCEINLINE void SKCuller::ClearPlaneState()
{
	m_uiPlaneState =0xffffffff;
}
FORCEINLINE void SKCuller::ClearAllPlane()
{
	m_uiPlaneNum = 0;
}
FORCEINLINE void SKCuller::SetCamera(SKCamera &Camera)
{
	m_pCamera = &Camera;
}
FORCEINLINE SKRenderContext& SKCuller::GetVisibleSpatial(unsigned int i,unsigned int uiVisibleSetType,unsigned int uiRenderGroup)const
{
	if(uiVisibleSetType >= SKT_MAX || uiRenderGroup > RG_MAX || i >=m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum())
	{
		SKMAC_ASSERT(0);
		return SKRenderContext::ms_RenderContextNULL;
	}
	return m_VisibleSet[uiRenderGroup][uiVisibleSetType][i];
}
FORCEINLINE unsigned int SKCuller::GetLightNum()const
{
	return m_LightSet.GetNum();
}
FORCEINLINE SKLight * SKCuller::GetLight(unsigned int i)const
{
	if (i >= m_LightSet.GetNum())
	{
		return NULL;
	}

	return m_LightSet[i];
}
FORCEINLINE void SKCuller::ClearLight()
{
	m_LightSet.Clear();
}
FORCEINLINE SKCamera * SKCuller::GetCamera()const
{
	return m_pCamera;
}