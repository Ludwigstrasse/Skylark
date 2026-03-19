#include "SKEnumTrans.h"
using namespace SKEngine2;
SKEnumTrans& SKEnumTrans::GetInstance()
{
	static SKEnumTrans EnumTrans;
	return EnumTrans;
}
SKEnumTrans::SKEnumTrans()
{

}
SKEnumTrans::~SKEnumTrans()
{

}
void SKEnumTrans::Add(const TCHAR * EnumName,const TCHAR * Value)
{
	unsigned int i = m_EnumSaver.Find(EnumName);
	if (i == m_EnumSaver.GetNum())
	{
		MapElement<SKString,SKArray<SKString>> Element;
		Element.Key = EnumName;
		Element.Value.AddElement(Value);
		m_EnumSaver.AddElement(Element);
		return;
	}
	MapElement<SKString,SKArray<SKString>> &Element = m_EnumSaver[i];
	Element.Value.AddElement(Value);
}
void SKEnumTrans::Get(const TCHAR * EnumName, SKArray<SKString>& AS)
{
	unsigned int i = m_EnumSaver.Find(EnumName);
	if (i == m_EnumSaver.GetNum())
	{
		return ;
	}
	AS = m_EnumSaver[i].Value;
}