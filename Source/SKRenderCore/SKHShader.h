#ifndef SKHSHADER_H
#define SKHSHADER_H
#include "SKShader.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKHShader : public SKShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKHShader();
		SKHShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile = false);
		SKHShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile = false);
		virtual ~SKHShader();

	public:


		static const SKHShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		static SKPointer<SKHShader> Default;
	};
	DECLARE_Ptr(SKHShader);
	SKTYPE_MARCO(SKHShader);
}
#endif