FORCEINLINE const SKMatrix3X3W & SKTransform::GetCombine()
{

	Combine();

	return m_mCombine;
}
FORCEINLINE const SKMatrix3X3W & SKTransform::GetCombineInverse()
{

	Combine();

	if (!m_bIsHaveInverse)
	{
		m_bIsHaveInverse = 1;
		m_mCombineInverse.InverseOf(m_mCombine);
	}
	return m_mCombineInverse;

}
FORCEINLINE const SKVector3&  SKTransform::GetScale()const
{
	return m_fScale;

}
FORCEINLINE const SKVector3 & SKTransform::GetTranslate()const
{
	return m_vTranslate;

}
FORCEINLINE const SKMatrix3X3 & SKTransform::GetRotate()const
{
	return m_mRotate;

}
FORCEINLINE void SKTransform::SetScale(const SKVector3& fScale)
{

	m_fScale = fScale;

	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
FORCEINLINE void SKTransform::SetTranslate(const SKVector3& Translate)
{
	m_vTranslate = Translate;
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
FORCEINLINE void SKTransform::SetRotate(const SKMatrix3X3 & Rotate)
{
	m_mRotate = Rotate;
	
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;

}
void SKTransform::GetDir(SKVector3 &Dir,SKVector3 &Up,SKVector3 & Right)const
{
	Right.x = m_mRotate._00;
	Right.y = m_mRotate._01;
	Right.z = m_mRotate._02;

	Up.x = m_mRotate._10;
	Up.y = m_mRotate._11;
	Up.z = m_mRotate._12;

	Dir.x = m_mRotate._20;
	Dir.y = m_mRotate._21;
	Dir.z = m_mRotate._22;
}