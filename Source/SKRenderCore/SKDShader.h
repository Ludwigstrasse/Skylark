#ifndef SKDSHADER_H
#define SKDSHADER_H
#include "SKShader.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKDShader : public SKShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKDShader();
		SKDShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile = false);
		SKDShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile = false);
		virtual ~SKDShader();

	public:


		static const SKDShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		static SKPointer<SKDShader> Default;
	};
	DECLARE_Ptr(SKDShader);
	SKTYPE_MARCO(SKDShader);
}
#endif