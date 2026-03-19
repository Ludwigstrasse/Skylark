#include "SKEngineInput.h"
using namespace SKEngine2;
SKEngineInput * SKEngineInput::ms_pInput = NULL;
SKEngineInput::SKEngineInput()
{
	
	SKMemset(m_Keys, 0, sizeof(m_Keys));
	SKMemset(m_KeysOld, 0, sizeof(m_KeysOld));

	SKMemset(m_bPressed,  0, sizeof(bool)*3);
	SKMemset(m_bReleased, 0, sizeof(bool)*3);
	m_lX = m_lY = m_lZ = 0;

	m_Cage.left   = -1;
	m_Cage.right  = -1;
	m_Cage.top    = -1;
	m_Cage.bottom = -1;
	m_Cage.front = -1;
	m_Cage.back = -1;
	m_pMsgProc = NULL;
	ms_pInput = this;
}
SKEngineInput::~SKEngineInput()
{

}
void SKEngineInput::SetMsgProc(InputMsgProc pMsgProc)
{
	m_pMsgProc = pMsgProc;
}
void SKEngineInput::Update()
{
	KeyBoardUpdate();
	MouseUpdate();
	if (m_pMsgProc)
	{
		for(unsigned int i = 0 ;i < BK_MAX ; i++)
		{
			if(IsKeyBoardPressed(i))
			{
				(*m_pMsgProc)(IT_KEYBOARD,IE_DOWN,i,m_lX,m_lY,m_lZ);
			}
			if(IsKeyBoardReleased(i))
			{
				(*m_pMsgProc)(IT_KEYBOARD,IE_UP,i,m_lX,m_lY,m_lZ);
			}
		}

		for (unsigned int i = 0 ; i < MK_MAX ;i++)
		{
			if(IsMousePressed(i))
			{
				(*m_pMsgProc)(IT_MOUSE,IE_DOWN,i,m_lX,m_lY,m_lZ);
			}
			if(IsMouseReleased(i))
			{
				(*m_pMsgProc)(IT_MOUSE,IE_UP,i,m_lX,m_lY,m_lZ);
			}
		}
		if(m_Delta.x != 0 || m_Delta.y != 0)
		{
			(*m_pMsgProc)(IT_MOUSE, IE_MOUSE_MOVE, IE_UP, m_lX, m_lY, m_lZ);
		}
		if(m_Delta.z != 0)
		{
			(*m_pMsgProc)(IT_MOUSE, IE_WHEEL_MOVE, IE_UP, m_lX, m_lY, m_lZ);
		}
	}

}
bool SKEngineInput::IsPressed(unsigned int InputType, UINT nBtn)
{
	if(InputType == IT_KEYBOARD)
		return IsKeyBoardPressed(nBtn);
	else if(InputType == IT_MOUSE)
		return IsMousePressed(nBtn);
	else
		return 0;
}
bool SKEngineInput::IsReleased(unsigned int InputType, UINT nBtn)
{
	if(InputType == IT_KEYBOARD)
		return IsKeyBoardReleased(nBtn);
	else if(InputType == IT_MOUSE)
		return IsMouseReleased(nBtn);
	else
		return 0;
}