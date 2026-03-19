/********************************************************************************/
FORCEINLINE const SKString &SKRtti::GetName()const
{
	return m_cRttiName;

}//end GetName
/********************************************************************************/
FORCEINLINE bool SKRtti::IsSameType(const SKRtti &Tpye) const
{
	return  (&Tpye == this);
}//end IsSameType
/********************************************************************************/
FORCEINLINE bool SKRtti::IsDerived(const SKRtti &Tpye) const
{
	const SKRtti * pTemp = this;
	while(!pTemp->IsSameType(Tpye))
	{
		if(pTemp->m_pBase)
		{
			pTemp = pTemp->m_pBase;
		
		}
		else
		{
			return 0;
		}
	
	}
	return 1;
}//end IsDerived
/********************************************************************************/