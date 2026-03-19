#ifndef SKVSHADER_H
#define SKVSHADER_H
#include "SKShader.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKVShader : public SKShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKVShader();
		SKVShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile = false);
		SKVShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile = false);
		virtual ~SKVShader();

	public:


		static const SKVShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		static SKPointer<SKVShader> Default;
	};
	DECLARE_Ptr(SKVShader);
	SKTYPE_MARCO(SKVShader);
}
#endif