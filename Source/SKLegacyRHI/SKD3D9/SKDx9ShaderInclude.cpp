#include "SKDx9ShaderInclude.h"
#include "SKFile.h"
#include "SKResourceManager.h"
#include "SKShader.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
SKDx9ShaderInclude::SKDx9ShaderInclude()
{

}
SKDx9ShaderInclude::~SKDx9ShaderInclude()
{

}
HRESULT SKDx9ShaderInclude::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	assert(D3DXINC_LOCAL == IncludeType);

	SKString FileName(pFileName);
	unsigned int uiIndex = m_FileMap.Find(FileName);
	if (uiIndex != m_FileMap.GetNum())
	{
		*ppData = (void *)m_FileMap[uiIndex].Value.GetBuffer();
		*pBytes = m_FileMap[uiIndex].Value.GetLength();
	}
	else
	{
		SKFile DynamicShaderFile;
		SKString DynamicShaderPath = /*SKResourceManager::ms_ShaderPath +*/ FileName;
		if(!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),SKFile::OM_RB))
		{
			SKString PreShaderPath = SKResourceManager::GetRenderTypeShaderPath(SKRenderer::RAT_DIRECTX9);
			DynamicShaderPath = SKResourceManager::ms_ShaderPath + PreShaderPath + FileName;
			if (!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),SKFile::OM_RB))
			{
				return S_FALSE;
			}
		}
		unsigned int uiSize = DynamicShaderFile.GetFileSize();
		if (!uiSize)
		{
			return S_FALSE;
		}
		SKString VDynamicShaderString;
		VDynamicShaderString.SetTCHARBufferNum(uiSize);
		if(!DynamicShaderFile.Read(VDynamicShaderString.GetBuffer(),uiSize,1))
		{
			return S_FALSE;
		}
		m_FileMap.AddElement(FileName,VDynamicShaderString);
		*ppData = (void *)m_FileMap[m_FileMap.GetNum() - 1].Value.GetBuffer();
		*pBytes = m_FileMap[m_FileMap.GetNum() - 1].Value.GetLength();

	}
	

	return S_OK;
}

HRESULT SKDx9ShaderInclude::Close(LPCVOID pData)
{
	// do nothing
	return S_OK;
}