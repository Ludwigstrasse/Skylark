FORCEINLINE const SKMatrix3X3W & SKCamera::GetViewMatrix()const
{
	return m_ViewMat;

}
// FORCEINLINE const SKVector3& SKCamera::GetWorldPos()const
// {
// 	return m_WorldPos;
// }
// FORCEINLINE void SKCamera::GetLocalDirection(SKVector3 &LocalDir,SKVector3 &LocalUp,SKVector3 & LocalRight)const
// {
// 	LocalDir = m_LocalDir;
// 	LocalUp = m_LocalUp;
// 	LocalRight = m_LocalRight;
// 
// }
// FORCEINLINE void SKCamera::GetWorldDirection(SKVector3 &WorldDir,SKVector3 &WorldUp,SKVector3 & WorldRight)const
// {
// 	WorldDir = m_WorldDir;
// 	WorldUp = m_WorldUp;
// 	WorldRight = m_WorldRight;
// 
// }
FORCEINLINE const SKMatrix3X3W & SKCamera::GetProjMatrix()const
{
	return m_ProjMat;
}
FORCEINLINE SKViewPort * SKCamera::GetViewPort(unsigned int i )const
{
	return &m_ViewPort[i];
}
FORCEINLINE bool SKCamera::AddViewPort(const SKViewPort &ViewPort)//ӿ		
{
	if (ViewPort.XMin < 0 || ViewPort.XMax > 1.0f || ViewPort.XMin > ViewPort.XMax ||
		ViewPort.YMin < 0 ||ViewPort.YMax > 1.0f || ViewPort.YMin > ViewPort.YMax ||
		ViewPort.ZMax > 1.0f || ViewPort.ZMin < 0.0f || ViewPort.ZMin > ViewPort.ZMax)
	{
		return false;
	}
	m_ViewPort.AddElement(ViewPort);
	return true;
}
// FORCEINLINE const SKVector3 &SKCamera::GetLocalDir()const
// {
// 	return m_LocalDir;
// }
// FORCEINLINE const SKVector3 &SKCamera::GetLocalUp()const
// {
// 	return m_LocalUp;
// }
// FORCEINLINE const SKVector3 &SKCamera::GetLocalRight()const
// {
// 	return m_LocalRight;
// }
// FORCEINLINE const SKVector3 &SKCamera::GetWorldDir()const
// {
// 	return m_WorldDir;
// }
// FORCEINLINE const SKVector3 &SKCamera::GetWorldUp()const
// {
// 	return m_WorldUp;
// }
// FORCEINLINE const SKVector3 &SKCamera::GetWorldRight()const
// {
// 	return m_WorldRight;
// }
