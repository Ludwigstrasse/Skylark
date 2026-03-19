#ifndef SKSHADERMAP_H
#define SKSHADERMAP_H
#include "SKObject.h"
namespace SKEngine2
{
	DECLARE_Ptr(SKShader);
	DECLARE_Ptr(SKShaderKey);
	typedef SKMapOrder<SKShaderKey,SKShaderPtr> SKShaderSet;
	class SKGRAPHIC_API SKShaderMapLoadSave : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKShaderMapLoadSave(){}
		~SKShaderMapLoadSave(){}
		SKMapOrder<SKUsedName,SKShaderSet> m_ShaderMap;

	};
	DECLARE_Ptr(SKShaderMapLoadSave);
	SKTYPE_MARCO(SKShaderMapLoadSave);
	class SKGRAPHIC_API SKShaderMap
	{
		
	public:
		SKShaderMap(SKString ShaderMapName);
		~SKShaderMap();
	protected:

		
		SKMapOrder<SKUsedName,SKShaderSet> m_ShaderMap;
	public:	
		SKMapOrder<SKUsedName,SKShaderSet>&GetShaderMap()
		{
			return m_ShaderMap;
		}
		void SetShader(const SKUsedName & Name,const SKShaderKey & Key,SKShader * pShader);
		SKShaderSet * GetShaderSet(const SKUsedName &Name);
		void DeleteShaderSet(const SKUsedName & Name);
		SKShader * GetShader(const SKUsedName & Name,const SKShaderKey & Key);
		void DeleteShader(const SKUsedName & Name,const SKShaderKey & Key);
		void Clear()
		{
			m_ShaderMap.Clear();
		}
		void GCResource()
		{

		}
		bool IsRealeseAll()
		{
			return (!m_ShaderMap.GetNum());
		}
		SKString m_ShaderMapName;
	};
	
}
#endif