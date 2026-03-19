#ifndef SKSHADER_H
#define SKSHADER_H
#include "SKObject.h"
#include "SKUserConstant.h"
#include "SKController.h"
#include "SKUserSampler.h"
#include "SKResourceManager.h"
#include "SKBind.h"
#include "SKResource.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKShader : public SKBind,public SKResource
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		SKShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile = false);
		SKShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile = false);
		SKShader();
		virtual ~SKShader() = 0;
		virtual unsigned int GetResourceType()const
		{
			return RT_SHADER;
		}
	public:
		


		virtual bool SetParam(const SKUsedName &Name,void * pDate);
		virtual bool SetParam(const SKUsedName &Name,SKTexAllState * pTexture,unsigned int uiIndex = 0);
		virtual void ClearInfo();

		
		const SKString & GetBuffer()const{ return m_Buffer;} 
		FORCEINLINE const void *GetCacheBuffer()const
		{
			return m_pCacheBuffer;
		}
		FORCEINLINE unsigned int GetCacheBufferSize()const
		{
			return m_uiCacheBufferSize;
		}
		bool SetCacheBuffer(void * pBuffer,unsigned int uiSize);
		const SKString &GetMainFunName()const
		{
			return m_MainFunName;
		}
		unsigned int m_uiConstBufferSize;
	protected:
		SKString m_Buffer;
		unsigned char * m_pCacheBuffer;
		unsigned int m_uiCacheBufferSize;
		SKString	m_MainFunName;
		
	public:	
		void SetShaderString(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile = false);
		void SetShaderString(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile = false);
	public:
		SKArray<SKUserConstantPtr>		m_pUserConstant;
		SKArray<SKUserSamplerPtr>		m_pUserSampler;
		SKShaderKey						m_ShaderKey;
		bool		m_bCreatePara;
		unsigned int m_uiArithmeticInstructionSlots;
		unsigned int m_uiTextureInstructionSlots;
	};
	DECLARE_Ptr(SKShader);
	SKTYPE_MARCO(SKShader);

}
#endif