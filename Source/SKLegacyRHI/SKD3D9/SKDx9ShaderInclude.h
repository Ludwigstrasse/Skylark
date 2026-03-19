#ifndef SKDX9SHADERINCLUDE_H
#define SKDX9SHADERINCLUDE_H
#include <SKd3dx9shader.h>
#include "SKString.h"
#include "SKMap.h"

namespace SKEngine2
{
	class SKDx9ShaderInclude : public ID3DXInclude
	{
	public:
		SKDx9ShaderInclude();
		virtual ~SKDx9ShaderInclude();
		STDMETHOD(Open)(THIS_ D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(THIS_ LPCVOID pData);	
	private:
		SKMap<SKString,SKString> m_FileMap;
	};
};
#endif