FORCEINLINE unsigned int SKNode::GetNodeNum()const
{
	return m_pChild.GetNum();
}
FORCEINLINE SKArray<SKSpatialPtr> * SKNode::GetChildList()
{
	return &m_pChild;
}
