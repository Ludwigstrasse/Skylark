#include "SKApplication.h"
#include "SKDx9Renderer.h"
#include "SKEngineDSKXInput.h"
#include "SKTimer.h"
#include "SKSceneManager.h"
#include "SKMonitor.h"
#include "SKASYNLoader.h"
#include "SKRenderThread.h"
#include "SKUpdateThread.h"
#include "SKWorld.h"
#include "SKProfiler.h"
#include "SKNullRenderer.h"
#include "SKDx11Renderer.h"
#ifdef WINDOWS
#include <WINDOWSX.H>
#endif
using namespace SKEngine2;
#ifdef WINDOWS
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow) 
{
	SKInitSystem();
	SKInitMath();
	SKMain::Initialize();
	if (SKApplication::ms_pApplication)
	{
		if (!SKApplication::ms_pApplication->Main(hInst,lpCmdLine,nCmdShow))
		{
			SKMAC_ASSERT(0);
		}
	}
	
	SKMain::Terminate();

	return 1;
}
int  main(int argc, char* argv[])
{
	SKInitSystem();
	SKInitMath();
	SKMain::Initialize();
	if (SKApplication::ms_pApplication)
	{
		if (!SKApplication::ms_pApplication->Main(argc, argv))
		{
			SKMAC_ASSERT(0);
		}
	}

	SKMain::Terminate();

	return 1;
}
#endif
DECLEAR_ONLYTIME_PROFILENODE(FPS,)
DECLEAR_TIME_PROFILENODE(ApplicationUpdate,)
DECLEAR_TIME_PROFILENODE(Update, ApplicationUpdate)
SKApplication *SKApplication::ms_pApplication = NULL;
SKApplication::SKApplication()
{

	m_iUpdateThreadNum = 0;
}
SKApplication::~SKApplication()
{

}

bool SKApplication::CreateRenderer()
{
	return true;
}
bool SKApplication::CreateTimer()
{
	if (!SKTimer::ms_pTimer)
	{
		SKTimer * pTimer = SK_NEW SKTimer();
		if (!pTimer)
		{
			return false;
		}
	}
	SKRandInit(SKTimer::ms_pTimer->GetRandSeed());
	return true;
}
bool SKApplication::CreateSceneManager()
{
	if (!SKSceneManager::ms_pSceneManager)
	{
		SKSceneManager * pSceneManager = SK_NEW SKSceneManager();
		if (!pSceneManager)
		{
			return false;
		}
	}
	return true;
}
bool SKApplication::CreateWorld()
{
	if (!SKWorld::ms_pWorld)
	{
		SKWorld * pWorld = SK_NEW SKWorld();
		if (!pWorld)
		{
			return false;
		}
	}
	return true;
}
bool SKApplication::CreateMonitor()
{
	return true;
}
bool SKApplication::CreateASYNLoader()
{
	if(!SKASYNLoadManager::ms_pASYNLoadManager)
	{
		SKASYNLoadManager * pASYNLoadManager = SK_NEW SKASYNLoadManager();
		if (!pASYNLoadManager)
		{
			return false;
		}
	}
	return true;
}
bool SKApplication::CreateUpdateThread()
{
	if (!SKResourceManager::ms_bUpdateThread || !m_iUpdateThreadNum)
	{
		return true;
	}
	if(!SKUpdateThreadSys::ms_pUpdateThreadSys)
	{
		SKUpdateThreadSys * pUpdateThreadSys = SK_NEW SKUpdateThreadSys(m_iUpdateThreadNum);
		if (!pUpdateThreadSys)
		{
			return false;
		}
	}
	return true;
}
bool SKApplication::CreateRenderThread()
{
	if (!SKResourceManager::ms_bRenderThread)
	{
		return true;
	}
	if(!SKRenderThreadSys::ms_pRenderThreadSys)
	{
		SKRenderThreadSys * pRenderTreadSys = SK_NEW SKRenderThreadSys();
		if (!pRenderTreadSys)
		{
			return false;
		}
	}
	return true;
}
bool SKApplication::CreateInput()
{
	
	return true;
}
bool SKApplication::ReleaseRenderer()
{
	SKMAC_DELETE(SKRenderer::ms_pRenderer);
	return true;
}
bool SKApplication::ReleaseInput()
{
	SKMAC_DELETE(SKEngineInput::ms_pInput);
	return true;
}
bool SKApplication::ReleaseTimer()
{
	SKMAC_DELETE(SKTimer::ms_pTimer);
	return true;
}
bool SKApplication::ReleaseSceneManager()
{
	SKMAC_DELETE(SKSceneManager::ms_pSceneManager);
	return true;
}
bool SKApplication::ReleaseWorld()
{
	SKMAC_DELETE(SKWorld::ms_pWorld);
	return true;
}
bool SKApplication::ReleaseASYNLoader()
{
	SKMAC_DELETE(SKASYNLoadManager::ms_pASYNLoadManager);
	return true;
}
bool SKApplication::ReleaseRenderThread()
{
	if (!SKResourceManager::ms_bRenderThread)
	{
		return true;
	}
	SKMAC_DELETE(SKRenderThreadSys::ms_pRenderThreadSys);
	return true;
}
bool SKApplication::ReleaseUpdateThread()
{
	if (!SKResourceManager::ms_bUpdateThread)
	{
		return true;
	}
	SKMAC_DELETE(SKUpdateThreadSys::ms_pUpdateThreadSys);
	return true;
}
bool SKApplication::ReleaseMonitor()
{
	return true;
}
bool SKApplication::Run()
{
	double fTime = 0.0f;
	double fFPS = 0.0f;
	double DetTime = 0.0f;
	ADD_TIME_PROFILE(ApplicationUpdate)
	GetStackMemManager().Clear();
	if (SKTimer::ms_pTimer)
	{
		SKTimer::ms_pTimer->UpdateFPS();
		fTime = SKTimer::ms_pTimer->GetGamePlayTime();
		fFPS = SKTimer::ms_pTimer->GetFPS();
		ADD_ONLYTIME_PROFILE(FPS,fFPS)
	}
	if (SKRenderer::ms_pRenderer)
	{

		if(SKRenderer::ms_pRenderer->CooperativeLevel())
		{
			if (SKRenderThreadSys::ms_pRenderThreadSys && SKResourceManager::ms_bRenderThread)
			{
				SKRenderThreadSys::ms_pRenderThreadSys->Begin();
			}
			{
				ADD_TIME_PROFILE(Update)
				if (SKEngineInput::ms_pInput)
				{
					SKEngineInput::ms_pInput->Update();
				}

#ifdef WINDOWS
				if (SKResourceMonitor::ms_pResourceMonitor)
				{
					SKResourceMonitor::ms_pResourceMonitor->Update(fTime);
				}
#endif
				if (SKASYNLoadManager::ms_pASYNLoadManager)
				{
					SKASYNLoadManager::ms_pASYNLoadManager->Update(fTime);
				}

				
				PreUpdate();

				if (SKSceneManager::ms_pSceneManager)
				{
					SKSceneManager::ms_pSceneManager->Update(fTime);
				}	

				if (SKWorld::ms_pWorld)
				{
					SKWorld::ms_pWorld->Update(fTime);
				}
				PostUpdate();
				SKRenderer::ms_pRenderer->BeginRendering();

				if (SKSceneManager::ms_pSceneManager)
				{
					SKSceneManager::ms_pSceneManager->Draw(fTime);
				}

				if (!OnDraw())
				{
					return false;
				}
				SKProfiler::Draw();
				SKRenderer::ms_pRenderer->EndRendering();
		
			}
			if (SKRenderThreadSys::ms_pRenderThreadSys && SKResourceManager::ms_bRenderThread)
			{
				SKRenderThreadSys::ms_pRenderThreadSys->ExChange();
			}		
		}
		else
		{
			if (SKRenderThreadSys::ms_pRenderThreadSys)
			{
				SKRenderThreadSys::ms_pRenderThreadSys->Clear();
			}
		}
		SKResourceManager::ClearDynamicBufferGeometry();
		SKResourceManager::DelayUpdate(fTime);
		SKResourceManager::GC();
		
	}
	return true;
}
bool SKApplication::CreateEngine()
{
	if (!CreateMonitor())
	{
		return false;
	}
	if (!CreateASYNLoader())
	{
		return false;
	}
	if (!CreateRenderThread())
	{
		return false;
	}
	if (!CreateUpdateThread())
	{
		return false;
	}
	if (!CreateRenderer())
	{
		return false;
	}
	if (!CreateInput())
	{
		return false;
	}
	if (!CreateSceneManager())
	{
		return false;
	}
	if (!CreateWorld())
	{
		return false;
	}
	return true;
}
bool SKApplication::ReleaseEngine()
{
	if (!ReleaseWorld())
	{
		return false;
	}
	if (!ReleaseSceneManager())
	{
		return false;
	}
	if (!ReleaseInput())
		return false;
	if (!ReleaseRenderer())
	{
		return false;
	}
	if (!ReleaseUpdateThread())
	{
		return false;
	}
	if (!ReleaseRenderThread())
	{
		return false;
	}
	if (!ReleaseASYNLoader())
	{
		return false;
	}
	if (!ReleaseMonitor())
	{
		return false;
	}
	return true;
}
bool SKApplication::Main(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow)
{
	return false;
}
bool SKApplication::Main(int argc, char* argv[])
{
	return false;
}
bool SKApplication::OnTerminal()
{
	return true;
}
bool SKApplication::PostUpdate()
{
	return true;
}
bool SKApplication::PreUpdate()
{
	return true;
}
bool SKApplication::OnDraw()
{
	return true;
}
bool SKApplication::OnInitial()
{
	SKProfiler::ClearAll();
	return true;
}
bool SKApplication::PreInitial()
{

	m_uiScreenWidth = 1024;
	m_uiScreenHeight = 768;
	m_uiAnisotropy = 1;
	m_uiMultisample = SKRenderer::MS_NONE;

	int iUpdateMaxThreadNum = SKGetCpuNum();
	iUpdateMaxThreadNum--;// Main Thread all time run
	iUpdateMaxThreadNum--;// AsynLoad Thread ,Monitor Thread

	if (iUpdateMaxThreadNum < 0)// Render Thread all time run
	{
		SKResourceManager::ms_bRenderThread = false;
	}
	else
	{
		SKResourceManager::ms_bRenderThread = true;
		iUpdateMaxThreadNum--;
	}

	iUpdateMaxThreadNum--; // for physics , audio or else third part lib


	if (iUpdateMaxThreadNum <= 0)
	{

		SKResourceManager::ms_bUpdateThread = false;
	}
	else
	{
		m_iUpdateThreadNum = iUpdateMaxThreadNum;
		SKResourceManager::ms_bUpdateThread = true;

	}
	return true;
}
bool SKApplication::CreateAppWindow()
{	
	return true;
}
void SKApplication::ChangeScreenSize(unsigned int uiWidth,unsigned int uiHeight,bool bWindow,bool IsMaxScreen)
{
	if (SKRenderThreadSys::ms_pRenderThreadSys)
	{
		SKRenderThreadSys::ms_pRenderThreadSys->Clear();
	}
	
	if(SKRenderer::ms_pRenderer->ChangeScreenSize(uiWidth,uiHeight,bWindow))
	{
		m_uiScreenWidth = uiWidth;
		m_uiScreenHeight = uiHeight;
	}
}
void SKApplication::OnMove(int xPos,int yPos)
{

}
void SKApplication::OnReSize(int iWidth,int iHeight)
{

}
void SKApplication::OnKeyDown(unsigned int uiKey)
{

}
void SKApplication::OnKeyUp(unsigned int uiKey)
{

}
void SKApplication::OnLButtonDown(int xPos,int yPos)
{

}
void SKApplication::OnLButtonUp(int xPos,int yPos)
{

}
void SKApplication::OnRButtonDown(int xPos,int yPos)
{

}
void SKApplication::OnRButtonUp(int xPos,int yPos)
{

}
void SKApplication::OnMButtonDown(int xPos,int yPos)
{

}
void SKApplication::OnMButtonUp(int xPos,int yPos)
{

}
void SKApplication::OnMouseMove(int xPos,int yPos)
{

}
void SKApplication::OnMouseWheel(int xPos,int yPos,int zDet)
{

}
#ifdef WINDOWS
DWORD SKWindowApplication::ms_WinodwKeyToVS[] =
{
	SKEngineInput::BK_MAX,		//					0x00
	SKEngineInput::BK_MAX,		//VK_LBUTTON        0x01
	SKEngineInput::BK_MAX,		//VK_RBUTTON        0x02
	SKEngineInput::BK_MAX,		//VK_CANCEL         0x03
	SKEngineInput::BK_MAX,		//VK_MBUTTON        0x04
	SKEngineInput::BK_MAX,		//VK_XBUTTON1       0x05
	SKEngineInput::BK_MAX,		//VK_XBUTTON2       0x06
	SKEngineInput::BK_MAX,		//				    0x07
	SKEngineInput::BK_BACK,		//VK_BACK           0x08
	SKEngineInput::BK_TAB,		//VK_TAB            0x09
	SKEngineInput::BK_MAX,		//					0x0A
	SKEngineInput::BK_MAX,		//					0x0B
	SKEngineInput::BK_MAX,		//VK_CLEAR          0x0C
	SKEngineInput::BK_RETURN,	//VK_RETURN         0x0D
	SKEngineInput::BK_MAX,		//VK_CLEAR          0x0E
	SKEngineInput::BK_MAX,		//VK_CLEAR          0x0F
	SKEngineInput::BK_MAX,		//VK_SHIFT          0x10
	SKEngineInput::BK_MAX,		//VK_CONTROL        0x11
	SKEngineInput::BK_MAX,		//VK_MENU           0x12
	SKEngineInput::BK_PAUSE,	//VK_PAUSE          0x13
	SKEngineInput::BK_CAPITAL,	//VK_CAPITAL        0x14
	SKEngineInput::BK_MAX,		//					0x15
	SKEngineInput::BK_MAX,		//					0x16
	SKEngineInput::BK_MAX,		//					0x17
	SKEngineInput::BK_MAX,		//					0x18
	SKEngineInput::BK_MAX,		//					0x19
	SKEngineInput::BK_MAX,		//					0x1A
	SKEngineInput::BK_ESCAPE,	//VK_ESCAPE         0x1B
	SKEngineInput::BK_MAX,		//					0x1C
	SKEngineInput::BK_MAX,		//					0x1D
	SKEngineInput::BK_MAX,		//					0x1E
	SKEngineInput::BK_MAX,		//					0x1F
	SKEngineInput::BK_SPACE,	//VK_SPACE          0x20
	SKEngineInput::BK_MAX,		//VK_PRIOR          0x21
	SKEngineInput::BK_MAX,		//VK_NEXT           0x22
	SKEngineInput::BK_END,		//VK_END			0x23
	SKEngineInput::BK_HOME,		//VK_HOME           0x24
	SKEngineInput::BK_LEFT,		//VK_LEFT           0x25
	SKEngineInput::BK_UP,		//VK_UP             0x26
	SKEngineInput::BK_RIGHT,	//VK_RIGHT          0x27
	SKEngineInput::BK_DOWN,		//VK_DOWN           0x28
	SKEngineInput::BK_MAX,		//VK_SELECT         0x29
	SKEngineInput::BK_MAX,		//					0x2A
	SKEngineInput::BK_MAX,		//					0x2B
	SKEngineInput::BK_MAX,		//					0x2C
	SKEngineInput::BK_INSERT,	//VK_INSERT         0x2D
	SKEngineInput::BK_DELETE,	//VK_DELETE         0x2E
	SKEngineInput::BK_MAX,		//					0x2F
	SKEngineInput::BK_0,		//VK_0				0x30
	SKEngineInput::BK_1,		//VK_1				0x31
	SKEngineInput::BK_2,		//VK_2				0x32
	SKEngineInput::BK_3,		//VK_3				0x33
	SKEngineInput::BK_4,		//VK_4				0x34
	SKEngineInput::BK_5,		//VK_5				0x35
	SKEngineInput::BK_6,		//VK_6				0x36
	SKEngineInput::BK_7,		//VK_7				0x37
	SKEngineInput::BK_8,		//VK_8				0x38
	SKEngineInput::BK_9,		//VK_9				0x39
	SKEngineInput::BK_MAX,		//					0x4A
	SKEngineInput::BK_MAX,		//					0x4B
	SKEngineInput::BK_MAX,		//					0x4C
	SKEngineInput::BK_MAX,		//					0x4D
	SKEngineInput::BK_MAX,		//					0x4E
	SKEngineInput::BK_MAX,		//					0x4F
	SKEngineInput::BK_MAX,		//					0x40
	SKEngineInput::BK_A,		//VK_A				0x41
	SKEngineInput::BK_B,		//VK_B				0x42
	SKEngineInput::BK_C,		//VK_C				0x43
	SKEngineInput::BK_D,		//VK_D				0x44
	SKEngineInput::BK_E,		//VK_E				0x45
	SKEngineInput::BK_F,		//VK_F				0x46
	SKEngineInput::BK_G,		//VK_G				0x47
	SKEngineInput::BK_H,		//VK_H				0x48
	SKEngineInput::BK_I,		//VK_I				0x49
	SKEngineInput::BK_J,		//VK_J				0x4A
	SKEngineInput::BK_K,		//VK_K				0x4B
	SKEngineInput::BK_L,		//VK_L				0x4C
	SKEngineInput::BK_M,		//VK_M				0x4D
	SKEngineInput::BK_N,		//VK_N				0x4E
	SKEngineInput::BK_O,		//VK_O				0x4F
	SKEngineInput::BK_P,		//VK_P				0x50
	SKEngineInput::BK_Q,		//VK_Q				0x51
	SKEngineInput::BK_R,		//VK_R				0x52
	SKEngineInput::BK_S,		//VK_S				0x53
	SKEngineInput::BK_T,		//VK_T				0x54
	SKEngineInput::BK_U,		//VK_U				0x55
	SKEngineInput::BK_V,		//VK_V				0x56
	SKEngineInput::BK_W,		//VK_W				0x57
	SKEngineInput::BK_X,		//VK_X				0x58
	SKEngineInput::BK_Y,		//VK_Y				0x59
	SKEngineInput::BK_Z,		//VK_Z				0x5A
	SKEngineInput::BK_MAX,		//					0x5B
	SKEngineInput::BK_MAX,		//					0x5C
	SKEngineInput::BK_MAX,		//					0x5D
	SKEngineInput::BK_MAX,		//					0x5E
	SKEngineInput::BK_MAX,		//					0x5F
	SKEngineInput::BK_NP0,		//VK_NUMPAD0        0x60
	SKEngineInput::BK_NP1,		//VK_NUMPAD1        0x61
	SKEngineInput::BK_NP2,		//VK_NUMPAD2        0x62
	SKEngineInput::BK_NP3,		//VK_NUMPAD3        0x63
	SKEngineInput::BK_NP4,		//VK_NUMPAD4        0x64
	SKEngineInput::BK_NP5,		//VK_NUMPAD5        0x65
	SKEngineInput::BK_NP6,		//VK_NUMPAD6        0x66
	SKEngineInput::BK_NP7,		//VK_NUMPAD7        0x67
	SKEngineInput::BK_NP8,		//VK_NUMPAD8        0x68
	SKEngineInput::BK_NP9,		//VK_NUMPAD9        0x69
	SKEngineInput::BK_NPMULTIPLY,//VK_MULTIPLY       0x6A
	SKEngineInput::BK_NPPLUS,	//VK_ADD            0x6B
	SKEngineInput::BK_MAX,		//VK_SEPARATOR      0x6C
	SKEngineInput::BK_NPMINUS,	//VK_SUBTRACT       0x6D
	SKEngineInput::BK_NPDECIMAL,//VK_DECIMAL        0x6E
	SKEngineInput::BK_NPDIVIDE,	//VK_DIVIDE         0x6F
	SKEngineInput::BK_F1,		//VK_F1             0x70
	SKEngineInput::BK_F2,		//VK_F2             0x71
	SKEngineInput::BK_F3,		//VK_F3             0x72
	SKEngineInput::BK_F4,		//VK_F4             0x73
	SKEngineInput::BK_F5,		//VK_F5             0x74
	SKEngineInput::BK_F6,		//VK_F6             0x75
	SKEngineInput::BK_F7,		//VK_F7             0x76
	SKEngineInput::BK_F8,		//VK_F8             0x77
	SKEngineInput::BK_F9,		//VK_F9             0x78
	SKEngineInput::BK_F10,		//VK_F10            0x79
	SKEngineInput::BK_F11,		//VK_F11            0x7A
	SKEngineInput::BK_F12,		//VK_F12            0x7B
	SKEngineInput::BK_MAX,		//					0x7C
	SKEngineInput::BK_MAX,		//					0x7D
	SKEngineInput::BK_MAX,		//					0x7E
	SKEngineInput::BK_MAX,		//					0x7F
	SKEngineInput::BK_MAX,		//					0x80
	SKEngineInput::BK_MAX,		//					0x81
	SKEngineInput::BK_MAX,		//					0x82
	SKEngineInput::BK_MAX,		//					0x83
	SKEngineInput::BK_MAX,		//					0x84
	SKEngineInput::BK_MAX,		//					0x85
	SKEngineInput::BK_MAX,		//					0x86
	SKEngineInput::BK_MAX,		//					0x87
	SKEngineInput::BK_MAX,		//					0x88
	SKEngineInput::BK_MAX,		//					0x89
	SKEngineInput::BK_MAX,		//					0x8A
	SKEngineInput::BK_MAX,		//					0x8B
	SKEngineInput::BK_MAX,		//					0x8C
	SKEngineInput::BK_MAX,		//					0x8D
	SKEngineInput::BK_MAX,		//					0x8E
	SKEngineInput::BK_MAX,		//					0x8F
	SKEngineInput::BK_NUMLOCK,	//VK_NUMLOCK        0x90
	SKEngineInput::BK_SCROLL,	//VK_SCROLL         0x91
	SKEngineInput::BK_MAX,		//					0x92
	SKEngineInput::BK_MAX,		//					0x93
	SKEngineInput::BK_MAX,		//					0x94
	SKEngineInput::BK_MAX,		//					0x95
	SKEngineInput::BK_MAX,		//					0x96
	SKEngineInput::BK_MAX,		//					0x97
	SKEngineInput::BK_MAX,		//					0x98
	SKEngineInput::BK_MAX,		//					0x99
	SKEngineInput::BK_MAX,		//					0x9A
	SKEngineInput::BK_MAX,		//					0x9B
	SKEngineInput::BK_MAX,		//					0x9C
	SKEngineInput::BK_MAX,		//					0x9D
	SKEngineInput::BK_MAX,		//					0x9E
	SKEngineInput::BK_MAX,		//					0x9F
	SKEngineInput::BK_LSHIFT,		//VK_LSHIFT         0xA0
	SKEngineInput::BK_RSHIFT,		//VK_RSHIFT         0xA1
	SKEngineInput::BK_LCTRL,		//VK_LCONTROL       0xA2
	SKEngineInput::BK_RCTRL,		//VK_RCONTROL       0xA3
	SKEngineInput::BK_MAX,			//VK_LMENU			0xA4
	SKEngineInput::BK_MAX,			//VK_RMENU			0xA5
	SKEngineInput::BK_MAX,			//VK_BROWSER_BACK	0xA6
	SKEngineInput::BK_MAX,			//VK_BROWSER_FORWARD			0xA7
	SKEngineInput::BK_MAX,			//VK_BROWSER_REFRESH			0xA8
	SKEngineInput::BK_MAX,			//VK_BROWSER_STOP			0xA9
	SKEngineInput::BK_MAX,			//VK_BROWSER_SEARCH			0xAA
	SKEngineInput::BK_MAX,			//VK_BROWSER_FAVORITES			0xAB
	SKEngineInput::BK_MAX,			//VK_BROWSER_HOME			0xAC
	SKEngineInput::BK_MAX,			//VK_VOLUME_MUTE			0xAD
	SKEngineInput::BK_MAX,			//VK_VOLUME_DOWN			0xAE
	SKEngineInput::BK_MAX,			//VK_VOLUME_UP				0xAF
	SKEngineInput::BK_MAX,			// VK_MEDIA_NEXT_TRACK    0xB0
	SKEngineInput::BK_MAX,			// VK_MEDIA_PREV_TRACK    0xB1
	SKEngineInput::BK_MAX,			// VK_MEDIA_STOP          0xB2
	SKEngineInput::BK_MAX,			// VK_MEDIA_PLAY_PAUSE    0xB3
	SKEngineInput::BK_MAX,			// VK_LAUNCH_MAIL         0xB4
	SKEngineInput::BK_MAX,			// VK_LAUNCH_MEDIA_SELECT 0xB5
	SKEngineInput::BK_MAX,			// VK_LAUNCH_APP1         0xB6
	SKEngineInput::BK_MAX,			// VK_LAUNCH_APP2         0xB7
	SKEngineInput::BK_MAX,			//						0xB8
	SKEngineInput::BK_MAX,			//				         0xB9
	SKEngineInput::BK_SEMICOLON,			// VK_OEM_1          0xBA   // ';:' for US
	SKEngineInput::BK_EQUALS,			// VK_OEM_PLUS       0xBB   // '+' any country
	SKEngineInput::BK_COMMA,			// VK_OEM_COMMA      0xBC   // ',' any country
	SKEngineInput::BK_MINUS,			// VK_OEM_MINUS      0xBD   // '-' any country
	SKEngineInput::BK_PERIOD,			// VK_OEM_PERIOD     0xBE   // '.' any country
	SKEngineInput::BK_MAX,			// VK_OEM_2          0xBF   // '/?' for US
	SKEngineInput::BK_MAX,			// VK_OEM_3          0xC0   // '`~' for US
	SKEngineInput::BK_MAX,			//           0xC1
	SKEngineInput::BK_MAX,			//           0xC2
	SKEngineInput::BK_MAX,			//           0xC3
	SKEngineInput::BK_MAX,			//	         0xC4
	SKEngineInput::BK_MAX,			//           0xC5
	SKEngineInput::BK_MAX,			//           0xC6
	SKEngineInput::BK_MAX,			//           0xC7
	SKEngineInput::BK_MAX,			//           0xC8
	SKEngineInput::BK_MAX,			//           0xC9
	SKEngineInput::BK_MAX,			//           0xCA
	SKEngineInput::BK_MAX,			//           0xCB
	SKEngineInput::BK_MAX,			//           0xCC
	SKEngineInput::BK_MAX,			//           0xCD
	SKEngineInput::BK_MAX,			//           0xCE
	SKEngineInput::BK_MAX,			//           0xCF
	SKEngineInput::BK_MAX,			//           0xD0
	SKEngineInput::BK_MAX,			//           0xD1
	SKEngineInput::BK_MAX,			//           0xD2
	SKEngineInput::BK_MAX,			//           0xD3
	SKEngineInput::BK_MAX,			//           0xD4
	SKEngineInput::BK_MAX,			//           0xD5
	SKEngineInput::BK_MAX,			//           0xD6
	SKEngineInput::BK_MAX,			//           0xD7
	SKEngineInput::BK_MAX,			//           0xD8
	SKEngineInput::BK_MAX,			//           0xD9
	SKEngineInput::BK_MAX,			//           0xDA
	SKEngineInput::BK_LBRACKET,		//           0xDB  //  '[{' for US
	SKEngineInput::BK_MAX,			//           0xDC  //  '\|' for US
	SKEngineInput::BK_RBRACKET,		//           0xDD  //  ']}' for US
	SKEngineInput::BK_MAX,			//           0xDE  //  ''"' for US
	SKEngineInput::BK_MAX			//           0xDF


};
SKWindowApplication::SKWindowApplication()
{
	m_hInst = NULL;
	m_MainHwnd = NULL;

}
SKWindowApplication::~SKWindowApplication()
{

}
bool SKWindowApplication::CreateDx9()
{
	if (SKRenderer::ms_pRenderer)
	{
		if (SKRenderer::ms_pRenderer->GetRendererType() == SKRenderer::RAT_DIRECTX9)
		{
			return true;
		}
		else
		{
			SKMAC_DELETE(SKRenderer::ms_pRenderer);
			SKDX9Renderer * pRenderer =
				SK_NEW SKDX9Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
				m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
			if (SKRenderer::ms_pRenderer)
			{
				return true;
			}
		}
	}
	else
	{
		SKDX9Renderer * pRenderer =
			SK_NEW SKDX9Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
			m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
		if (SKRenderer::ms_pRenderer)
		{
			return true;
		}
	}

	return true;

}
bool SKWindowApplication::CreateDx10()
{
	return false;
}
bool SKWindowApplication::CreateDx11()
{
	if (SKRenderer::ms_pRenderer)
	{
		if (SKRenderer::ms_pRenderer->GetRendererType() == SKRenderer::RAT_DIRECTX11)
		{
			return true;
		}
		else
		{
			SKMAC_DELETE(SKRenderer::ms_pRenderer);
			SKDX11Renderer * pRenderer =
				SK_NEW SKDX11Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
				m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
			if (SKRenderer::ms_pRenderer)
			{
				return true;
			}
		}
	}
	else
	{
		SKDX11Renderer * pRenderer =
			SK_NEW SKDX11Renderer(m_MainHwnd, m_uiScreenWidth, m_uiScreenHeight,
			m_bIsWindowed, m_uiAnisotropy, m_uiMultisample, m_ArrayChildHwnd.GetBuffer(), m_ArrayChildHwnd.GetNum());
		if (SKRenderer::ms_pRenderer)
		{
			return true;
		}
	}

	return false;
}
bool SKWindowApplication::CreateOPGL()
{
	return false;
}
bool SKWindowApplication::CreateRenderer()
{
	SKApplication::CreateRenderer();
	if (m_uiRenderAPIType == SKRenderer::RAT_DIRECTX9)
	{
		return CreateDx9();
	}
	else if (m_uiRenderAPIType == SKRenderer::RAT_OPENGL)
	{
		return CreateOPGL();
	}
	else if (m_uiRenderAPIType == SKRenderer::RAT_DIRECTX10)
	{
		return CreateDx10();
	}
	else if (m_uiRenderAPIType == SKRenderer::RAT_DIRECTX11)
	{
		return CreateDx11();
	}
	else
	{
		return false;
	}
}
bool SKWindowApplication::CreateInput()
{
	if (SKEngineInput::ms_pInput)
	{
		if (SKEngineInput::ms_pInput->GetInputAPIType() == m_uiInputAPIType)
		{
			return true;
		}
		else
		{
			if (m_uiInputAPIType == SKEngineInput::IAT_DX)
			{
				SKEngineInput * pInput = SK_NEW SKEngineDXInput(m_hInst, m_MainHwnd, NULL);
				if (!pInput)
				{
					return false;
				}
				pInput->SetMsgProc(SKApplication::InputMsgProc);
			}
		}

	}
	else
	{
		if (m_uiInputAPIType == SKEngineInput::IAT_DX)
		{
			SKEngineInput * pInput = SK_NEW SKEngineDXInput(m_hInst, m_MainHwnd, NULL);
			if (!pInput)
			{
				return false;
			}
			pInput->SetMsgProc(SKApplication::InputMsgProc);
		}
	}

	return true;
}
bool SKWindowApplication::Main(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow)
{
	m_pCommand = SK_NEW SKCommand(lpCmdLine);
	m_hInst = hInst;
	m_bIsRunning = true;
	m_bIsActive = false;
	if (!CreateTimer())
	{
		return false;
	}

	if (!PreInitial())
	{
		return false;
	}

	if (!CreateAppWindow())
	{
		return false;
	}

	if (!CreateEngine())
	{
		return false;
	}

	if (!OnInitial())
	{
		return false;
	}
	MSG			msg;
	bool bError = false;
	while (m_bIsRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (m_bIsActive)
		{
			if (!Run())
			{
				m_bIsRunning = false;
				bError = true;
			}

		}
		else
		{
			if (SKRenderThreadSys::ms_pRenderThreadSys)
			{
				SKRenderThreadSys::ms_pRenderThreadSys->Clear();
			}
		}
	}
	if (!OnTerminal())
	{
		bError = true;
	}
	if (!ReleaseEngine())
	{
		bError = true;
	}
	if (!ReleaseTimer())
	{
		bError = true;
	}
	SKMAC_DELETE(m_pCommand);
	UnregisterClass(m_WindowClassName.GetBuffer(), m_hInst);
	return !bError;
}
bool SKWindowApplication::PreInitial()
{
	SKApplication::PreInitial();
	m_uiRenderAPIType = SKRenderer::RAT_DIRECTX11;
	m_uiScreenWidth = 1024;
	m_uiScreenHeight = 768;
	m_bIsWindowed = true;
	m_uiAnisotropy = 1;
	m_uiMultisample = SKRenderer::MS_NONE;
	m_Tile = _T("SKEngine2.0 by azhe --- Demo");
	m_WindowClassName = _T("FrameWorktest");
	m_uiInputAPIType = SKEngineInput::IAT_WINDOWS;
	m_dwWindowedStyle = WS_OVERLAPPEDWINDOW & (~WS_OVERLAPPED) | WS_VISIBLE;
	m_dwFullscreenStyle = WS_POPUP | WS_SYSMENU | WS_VISIBLE;
	
	return true;
}
bool SKWindowApplication::CreateAppWindow()
{
	WNDCLASSEX	wndclass;
	HWND		   hWnd = NULL;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wndclass.lpfnWndProc = SKWindowApplication::MsgProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_WindowClassName.GetBuffer();
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (RegisterClassEx(&wndclass) == 0)
		return 0;

	// create window
	if (!(hWnd = CreateWindowEx(NULL, m_WindowClassName.GetBuffer(),
		m_Tile.GetBuffer(),
		m_dwWindowedStyle,
		0,
		0,
		m_uiScreenWidth, m_uiScreenHeight, NULL, NULL, m_hInst, NULL)))
		return 0;

	m_MainHwnd = hWnd;




	if (m_bIsWindowed)
	{
		RECT window_rect = { 0, 0, m_uiScreenWidth, m_uiScreenHeight };
		AdjustWindowRect(&window_rect, m_dwWindowedStyle, GetMenu(m_MainHwnd) != NULL);
		int NewWidth = window_rect.right - window_rect.left;
		int NewHeight = window_rect.bottom - window_rect.top;
		//ƶ
		MoveWindow(m_MainHwnd,
			0, // x position
			0, // y position
			NewWidth, // width
			NewHeight, // height
			TRUE);
	}



	return true;
}
void SKWindowApplication::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow, bool IsMaxScreen)
{
	SKApplication::ChangeScreenSize(uiWidth, uiHeight, bWindow, IsMaxScreen);
	if (bWindow)
	{
		if (IsMaxScreen)
		{
			RECT window_rect = { 0, 0, m_uiScreenWidth, m_uiScreenHeight };
			AdjustWindowRect(&window_rect, m_dwFullscreenStyle, GetMenu(m_MainHwnd) != NULL);
			SetWindowLong(m_MainHwnd, GWL_STYLE, m_dwFullscreenStyle);
			SetWindowPos(m_MainHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			//ƶ
			MoveWindow(m_MainHwnd,
				0, // x position
				0, // y position
				GetSystemMetrics(SM_CXSCREEN), // width
				GetSystemMetrics(SM_CYSCREEN), // height
				TRUE);
		}
		else
		{
			RECT window_rect = { 0, 0, m_uiScreenWidth, m_uiScreenHeight };
			AdjustWindowRect(&window_rect, m_dwWindowedStyle, GetMenu(m_MainHwnd) != NULL);
			SetWindowLong(m_MainHwnd, GWL_STYLE, m_dwWindowedStyle);
			unsigned int NewWidth = window_rect.right - window_rect.left;
			unsigned int NewHeight = window_rect.bottom - window_rect.top;
			//ƶ
			MoveWindow(m_MainHwnd,
				0, // x position
				0, // y position
				NewWidth, // width
				NewHeight, // height
				TRUE);
		}
	}


}
void SKApplication::InputMsgProc(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z)
{
	if (uiInputType == SKEngineInput::IT_KEYBOARD)
	{
		if (uiEvent == SKEngineInput::IE_DOWN)
		{
			SKApplication::ms_pApplication->OnKeyDown(uiKey);
		}
		else if (uiEvent == SKEngineInput::IE_UP)
		{
			SKApplication::ms_pApplication->OnKeyUp(uiKey);
		}

	}
	else if (uiInputType == SKEngineInput::IT_MOUSE)
	{
		if (uiEvent == SKEngineInput::IE_DOWN)
		{
			if (uiKey == SKEngineInput::MK_RIGHT)
			{
				SKApplication::ms_pApplication->OnRButtonDown(x, y);
			}
			else if (uiKey == SKEngineInput::MK_LEFT)
			{
				SKApplication::ms_pApplication->OnLButtonDown(x, y);
			}
			else if (uiKey == SKEngineInput::MK_MIDDLE)
			{
				SKApplication::ms_pApplication->OnMButtonDown(x, y);
			}
		}
		else if (uiEvent == SKEngineInput::IE_UP)
		{
			if (uiKey == SKEngineInput::MK_RIGHT)
			{
				SKApplication::ms_pApplication->OnRButtonUp(x, y);
			}
			else if (uiKey == SKEngineInput::MK_LEFT)
			{
				SKApplication::ms_pApplication->OnLButtonUp(x, y);
			}
			else if (uiKey == SKEngineInput::MK_MIDDLE)
			{
				SKApplication::ms_pApplication->OnMButtonUp(x, y);
			}
		}
		else if (uiEvent == SKEngineInput::IE_MOUSE_MOVE)
		{
			SKApplication::ms_pApplication->OnMouseMove(x, y);
		}
		else if (uiEvent == SKEngineInput::IE_WHEEL_MOVE)
		{
			SKApplication::ms_pApplication->OnMouseWheel(x, y, z);
		}
	}
	if (SKWorld::ms_pWorld)
	{
		SKWorld::ms_pWorld->ProcessInput(uiInputType, uiEvent, uiKey, x, y, z);
	}
}
unsigned int SKWindowApplication::CheckVirtualKeyDown(unsigned int VK)
{
	if (GetKeyState(VK_LSHIFT) < 0)
	{
		return VK_LSHIFT;
	}
	else if (GetKeyState(VK_RSHIFT) < 0)
	{
		return VK_RSHIFT;
	}
	else if (GetKeyState(VK_LCONTROL) < 0)
	{
		return VK_LCONTROL;
	}
	else if (GetKeyState(VK_RCONTROL) < 0)
	{
		return VK_RCONTROL;
	}
	else
	{
		return 0;
	}

}
unsigned int SKWindowApplication::CheckVirtualKeyUp(unsigned int VK)
{
	if (GetKeyState(VK_LSHIFT) > 0)
	{
		return VK_LSHIFT;
	}
	else if (GetKeyState(VK_RSHIFT) > 0)
	{
		return VK_RSHIFT;
	}
	else if (GetKeyState(VK_LCONTROL) > 0)
	{
		return VK_LCONTROL;
	}
	else if (GetKeyState(VK_RCONTROL) > 0)
	{
		return VK_RCONTROL;
	}
	else
	{
		return 0;
	}
}
LRESULT WINAPI SKWindowApplication::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!SKWindowApplication::ms_pApplication)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	switch (msg)
	{
		case WM_KILLFOCUS:
		{
			SKWindowApplication::ms_pApplication->m_bIsActive = false;
			break;
		}
		case WM_SETFOCUS:
		{
			SKWindowApplication::ms_pApplication->m_bIsActive = true;
			break;
		}

		case WM_ACTIVATE:
		{
			SKWindowApplication::ms_pApplication->m_bIsActive = (bool)(wParam);
			break;
		}
		case WM_MOVE:
		{
			int xPos = (int)(LOWORD(lParam));
			int yPos = (int)(HIWORD(lParam));
			SKApplication::ms_pApplication->OnMove(xPos, yPos);
			break;
		}
		case WM_SIZE:
		{
			int width = (int)(LOWORD(lParam));
			int height = (int)(HIWORD(lParam));
			SKApplication::ms_pApplication->OnReSize(width, height);
			break;
		}
		case WM_KEYDOWN:
		{
			if (!SKEngineInput::ms_pInput)
			{
				unsigned int virtKey = (unsigned int)wParam;
				if (virtKey < sizeof(ms_WinodwKeyToVS) / sizeof(DWORD))
				{ 
					SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_KEYBOARD, SKEngineInput::IE_DOWN, ms_WinodwKeyToVS[virtKey],0,0,0);
				}

			}

			switch (wParam)
			{
				case VK_ESCAPE:
				{
					SKApplication::ms_pApplication->m_bIsRunning = false;
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0;
				}

			}
			break;
		}
		case WM_KEYUP:
		{
			if (!SKEngineInput::ms_pInput)
			{
				unsigned int virtKey = (unsigned int)wParam;
				if (virtKey < sizeof(ms_WinodwKeyToVS) / sizeof(DWORD))
				{
					SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_KEYBOARD, SKEngineInput::IE_UP, ms_WinodwKeyToVS[virtKey], 0, 0, 0);
				}

			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_DOWN, SKEngineInput::MK_LEFT, xPos, yPos, 0);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_UP, SKEngineInput::MK_LEFT, xPos, yPos, 0);
			}
			break;
		}
		case WM_MBUTTONDOWN:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_DOWN, SKEngineInput::MK_MIDDLE, xPos, yPos, 0);
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_UP, SKEngineInput::MK_MIDDLE, xPos, yPos, 0);
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_DOWN, SKEngineInput::MK_RIGHT, xPos, yPos, 0);
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_UP, SKEngineInput::MK_RIGHT, xPos, yPos, 0);
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_MOUSE_MOVE, SKEngineInput::MK_MAX, xPos, yPos, 0);
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			if (!SKEngineInput::ms_pInput)
			{
				int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				int xPos = (int)(LOWORD(lParam));
				int yPos = (int)(HIWORD(lParam));
				SKApplication::ms_pApplication->InputMsgProc(SKEngineInput::IT_MOUSE, SKEngineInput::IE_WHEEL_MOVE, SKEngineInput::MK_MAX, xPos, yPos, delta);
			}
			break;
		}
		case WM_DESTROY:
		{
			SKApplication::ms_pApplication->m_bIsRunning = false;
			PostQuitMessage(0);
			return 1;
		} break;

		default: break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
bool SKWindowApplication::CreateMonitor()
{
	if (!SKResourceMonitor::ms_pResourceMonitor)
	{
		SKResourceMonitor * pResourceMonitor = SK_NEW SKResourceMonitor();
		if (!pResourceMonitor)
		{
			return false;
		}
		pResourceMonitor->Open();
	}
	return true;
}
bool SKWindowApplication::ReleaseMonitor()
{

	SKResourceMonitor::ms_pResourceMonitor->Close();
	SKMAC_DELETE(SKResourceMonitor::ms_pResourceMonitor);
	return true;
}



SKConsoleApplication::SKConsoleApplication()
{
}
SKConsoleApplication::~SKConsoleApplication()
{
}
bool SKConsoleApplication::CreateRenderer()
{
	SKApplication::CreateRenderer();
	if (!SKRenderer::ms_pRenderer)
	{
		SKNullRenderer * pRenderer = SK_NEW SKNullRenderer();
		if (SKRenderer::ms_pRenderer)
		{
			return true;
		}
	}

	return true;
}
bool SKConsoleApplication::CreateInput()
{
	return true;
}
bool SKConsoleApplication::Main(int argc, char* argv[])
{
	m_pCommand = SK_NEW SKCommand(argc, argv);
	m_bIsRunning = true;
	m_bIsActive = false;
	if (!CreateTimer())
	{
		return false;
	}

	if (!PreInitial())
	{
		return false;
	}

	if (!CreateAppWindow())
	{
		return false;
	}

	if (!CreateEngine())
	{
		return false;
	}

	if (!OnInitial())
	{
		return false;
	}
	bool bError = false;
	while (m_bIsRunning)
	{
		if (m_bIsActive)
		{
			if (!Run())
			{
				m_bIsRunning = false;
				bError = true;
			}

		}
	}
	if (!OnTerminal())
	{
		bError = true;
	}
	if (!ReleaseEngine())
	{
		bError = true;
	}
	if (!ReleaseTimer())
	{
		bError = true;
	}
	SKMAC_DELETE(m_pCommand);
	return !bError;
}
bool SKConsoleApplication::PreInitial()
{
	SKApplication::PreInitial();

	return true;
}
bool SKConsoleApplication::CreateAppWindow()
{
	
	return true;
}
void SKConsoleApplication::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow, bool IsMaxScreen)
{
	SKApplication::ChangeScreenSize(uiWidth, uiHeight, bWindow, IsMaxScreen);
}

#endif