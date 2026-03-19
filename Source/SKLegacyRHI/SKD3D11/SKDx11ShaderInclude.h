#ifndef SKDX11SHADERINCLUDE_H
#define SKDX11SHADERINCLUDE_H
#include <d3d11.h>
#include <SKD3Dcompiler.h>
#include "SKString.h"
#include "SKMap.h"

namespace SKEngine2
{
	class SKDx11ShaderInclude : public ID3D10Include
	{
	public:
		SKDx11ShaderInclude();
		virtual ~SKDx11ShaderInclude();
		STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		STDMETHOD(Close)(THIS_ LPCVOID pData);
	private:
		SKMap<SKString,SKString> m_FileMap;
	};
};
#endif