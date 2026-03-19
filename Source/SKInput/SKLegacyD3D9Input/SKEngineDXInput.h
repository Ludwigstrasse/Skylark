#ifndef SKENGINEDXINPUT_H
#define SKENGINEDXINPUT_H
#include <SKdinput.h>
#include "SKEngineInput.h"
#ifdef SKDX9INPUT_EXPORTS
#define  SKDX9INPUT_API __declspec(dllexport) 
#else
#define  SKDX9INPUT_API __declspec(dllimport)
#endif
namespace SKEngine2
{
	class SKDX9INPUT_API SKEngineDXInput : public SKEngineInput
	{
	public:
		SKEngineDXInput(HINSTANCE hInstance,HWND hWnd,const MouseRange *pCage);
		virtual ~SKEngineDXInput();
		virtual unsigned int GetInputAPIType()const
		{
			return IAT_DX;
		}
	protected:
		enum
		{
			BUFFER_SIZE = 16
		};
		virtual bool IsKeyBoardPressed(unsigned int  uiBoardKey);
		virtual bool IsKeyBoardReleased(unsigned int  uiBoardKey);
		virtual void KeyBoardUpdate();


		virtual bool IsMousePressed(UINT nBtn);
		virtual	bool IsMouseReleased(UINT nBtn);
		virtual void MouseUpdate();

		HRESULT					m_hResult;
		LPDIRECTINPUT8			m_pDI;
		LPDIRECTINPUTDEVICE8	m_pMouseDevice;
		LPDIRECTINPUTDEVICE8	m_pKeyBoardDevice;
		static DWORD ms_BoardKey[BK_MAX];
		HWND				m_hWndMain;         //ھ
		HINSTANCE			m_hInstance;        //ʵ
	};


}
#endif