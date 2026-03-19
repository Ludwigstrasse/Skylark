#ifndef SKPSHADER_H
#define SKPSHADER_H
#include "SKShader.h"
namespace SKEngine2
{
	class SKStream;
	class SKPShader : public SKShader
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKPShader();
		SKPShader(const TCHAR * pBuffer,const SKString & MainFunName,bool IsFromFile = false);
		SKPShader(const SKString &Buffer,const SKString & MainFunName,bool IsFromFile = false);
		virtual ~SKPShader();

	public:


		

		static const SKPShader *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		static SKPointer<SKPShader> Default;
	};
	DECLARE_Ptr(SKPShader);
	SKTYPE_MARCO(SKPShader);

}
#endif