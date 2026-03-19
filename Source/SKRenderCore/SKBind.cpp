#include "SKBind.h"
#include "SKRenderer.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
SKArray<SKBind *> SKBind::ms_DynamicTwoBindArray;
SKArray<SKBind *> SKBind::ms_BindArray;
IMPLEMENT_RTTI_NoCreateFun(SKBind,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKBind,SKObject)
REGISTER_PROPERTY(m_bIsStatic,IsStatic,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiSwapChainNum,SwapChainNum,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiLockFlag,LockFlag,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiMemType,MemType,SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKResourceIdentifier* SKBind::GetIdentifier ()
{
	if (!m_InfoArray.GetNum())
	{
		return NULL;
	}
	SKResourceIdentifier * pID = NULL;
	pID = m_InfoArray[m_uiCurID].ID;
	return pID;
}
void SKBind::ExChange()
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		m_uiCurID = (m_uiCurID + 1) % m_uiSwapChainNum;
	}	
}
unsigned int SKBind::GetInfoQuantity () const
{
	return m_InfoArray.GetNum();
}
SKBind::SKBind(unsigned int uiMemType)
{
	m_InfoArray.Clear();
	m_bIsStatic = 1;
	m_uiSwapChainNum = 1;
	m_uiCurID = 0;
	m_uiLockFlag = LF_NOOVERWRITE;
	m_pUser = NULL;
	m_uiMemType = uiMemType;
	m_uiClearState = MCS_NONE;
	ms_BindArray.AddElement(this);
}
SKBind::~SKBind()
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		for (unsigned int i = 0 ;i  < ms_DynamicTwoBindArray.GetNum() ; i++)
		{
			if (ms_DynamicTwoBindArray[i] == this)
			{
				ms_DynamicTwoBindArray.Erase(i);
				break;
			}
		}
	}
	for (unsigned int i = 0 ;i  < ms_BindArray.GetNum() ; i++)
	{
		if (ms_BindArray[i] == this)
		{
			ms_BindArray.Erase(i);
			break;
		}
	}
	ReleaseResource();
}
void SKBind::Bind(SKResourceIdentifier* pID)
{
	if(!pID)
		return ;
	INFO_TYPE Info;
	Info.ID = pID;
	m_InfoArray.AddElement(Info);
}

bool SKBind::LoadResource(SKRenderer * pRender)
{
	if(!pRender)
		return 0;
	if (m_uiMemType == MT_RAM)
	{
		return 1;
	}
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
		return 1;
	else
	{
		m_pUser = pRender;
		for (unsigned int i = 0 ; i < m_uiSwapChainNum ; i++)
		{
			SKResourceIdentifier *pID = NULL;
			if(!OnLoadResource(pID))
				return 0;
			if(!pID)
				return 0;
			Bind(pID);
		}
		if (!SKResourceManager::ms_bRenderThread)
		{
			ClearInfo();
		}
		return 1;	
	
	}
	
}
bool SKBind::ReleaseResource()
{
	for (unsigned int i = 0 ; i < m_InfoArray.GetNum() ; i++)
	{
		INFO_TYPE &rInfo = m_InfoArray[i];
		if(!OnReleaseResource(rInfo.ID))
			return 0;
	}
	m_InfoArray.Clear();
	return 1;
}
void SKBind::ClearInfo()
{

}
void SKBind::ASYNClearInfo()
{
	if (m_uiMemType == MT_VRAM)
	{
		if(m_uiSwapChainNum == m_InfoArray.GetNum())
		{
			if (m_uiClearState == MCS_NONE)
			{
				m_uiClearState = MCS_READY;
				return ;
			}
			else if (m_uiClearState == MCS_READY)
			{
				m_uiClearState = MCS_DONE;
				ClearInfo();
				return ;
			}
		}
	}
}
void SKBind::SetStatic(bool bIsStatic,bool bMulThreadUse)
{
	if (!m_bIsStatic && m_uiSwapChainNum == 2)
	{
		for (unsigned int i = 0; i < ms_DynamicTwoBindArray.GetNum(); i++)
		{
			if (ms_DynamicTwoBindArray[i] == this)
			{
				ms_DynamicTwoBindArray.Erase(i);
				break;
			}
		}
	}
	m_bIsStatic = bIsStatic;
	
	if(m_bIsStatic)
	{
		m_uiSwapChainNum = 1;	
	}
	else
	{
		if (SKResourceManager::ms_bRenderThread && bMulThreadUse && SKRenderer::ms_pRenderer->IsSupportMulBufferSwtich())
		{

			m_uiSwapChainNum = 2;
			ms_DynamicTwoBindArray.AddElement(this);

		}
		else
		{
			m_uiSwapChainNum = 1;
		}

	}

	m_uiCurID = 0;

}
unsigned int SKBind::GetByteSize()const
{
	return 0;
}
