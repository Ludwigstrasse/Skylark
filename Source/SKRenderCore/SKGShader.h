#ifndef SKGSHADER_H
#define SKGSHADER_H
#include "SKShader.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKGShader : public SKShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKGShader();
		SKGShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile = false);
		SKGShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile = false);
		virtual ~SKGShader();

	public:


		static const SKGShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		static SKPointer<SKGShader> Default;
	};
	DECLARE_Ptr(SKGShader);
	SKTYPE_MARCO(SKGShader);
}
#endif