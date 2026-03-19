#ifndef SKCAPTURETEXALLSTATE_H
#define SKCAPTURETEXALLSTATE_H
#include "SKTexAllState.h"
#include "SKViewFamily.h"
#include "SKCubeTexture.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKCaptureTexAllState : public SKTexAllState
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		SKCaptureTexAllState();

		virtual ~SKCaptureTexAllState();

		bool SetViewCapture(const SKString & ViewCaptureName);

		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual void ForceUpdate(bool OnlyUpdateOneTime);
		virtual void NotifyEndDraw();
		virtual bool BeforeSave( void * pDate = NULL);
		virtual bool PostSave( void * pDate = NULL);
		void SetMipLevel(unsigned int uiMipLevel);
		SKCaptureViewFamily * GetViewFamily();
	protected:
		SKString  m_ViewCaptureName;
		bool m_bDynamic;
		SKTexturePtr m_pStaticTexture;
		unsigned int m_uiMipLevel;
		void CreateStaticTexture();
	};
	DECLARE_Ptr(SKCaptureTexAllState);
	SKTYPE_MARCO(SKCaptureTexAllState);
	DECLARE_Proxy(SKCaptureTexAllState);
}
#endif