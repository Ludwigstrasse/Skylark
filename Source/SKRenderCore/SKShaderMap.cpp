#include "SKShaderMap.h"
#include "SKGeometry.h"
#include "SKBoneNode.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKShaderMapLoadSave,SKObject)
BEGIN_ADD_PROPERTY(SKShaderMapLoadSave,SKObject)
REGISTER_PROPERTY(m_ShaderMap,ShaderMap,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKShaderMapLoadSave)
IMPLEMENT_INITIAL_END
SKShaderMap::SKShaderMap(SKString ShaderMapName)
{
	m_ShaderMapName = ShaderMapName;
}
SKShaderMap::~SKShaderMap()
{
	Clear();
}
void SKShaderMap::SetShader(const SKUsedName & Name,const SKShaderKey & Key,SKShader * pShader)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		m_ShaderMap[index].Value.AddElement(Key,pShader);
	}
	else
	{
		SKShaderSet Temp;
		Temp.AddElement(Key,pShader);
		m_ShaderMap.AddElement(Name,Temp);
	}
}
SKShaderSet * SKShaderMap::GetShaderSet(const SKUsedName &Name)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		return &m_ShaderMap[index].Value;
	}
	else
	{
		return NULL;
	}
}
void SKShaderMap::DeleteShaderSet(const SKUsedName & Name)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		m_ShaderMap.Erase(index);
	}
}
SKShader * SKShaderMap::GetShader(const SKUsedName & Name,const SKShaderKey &Key)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		SKShaderSet * pShaderSet = &m_ShaderMap[index].Value;
		unsigned int index2 =(*pShaderSet).Find(Key);
		if (index2 != (*pShaderSet).GetNum())
		{
			return (*pShaderSet)[index2].Value;
		}
	}
	return NULL;
}
void SKShaderMap::DeleteShader(const SKUsedName & Name,const SKShaderKey &Key)
{
	unsigned int index = m_ShaderMap.Find(Name);
	if (index != m_ShaderMap.GetNum())
	{
		SKShaderSet * pShaderSet = &m_ShaderMap[index].Value;
		unsigned int index2 = (*pShaderSet).Find(Key);
		if (index2 != (*pShaderSet).GetNum())
		{
			return (*pShaderSet).Erase(index2);
		}
	}
}