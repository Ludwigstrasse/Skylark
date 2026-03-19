#include "SKProfiler.h"
#include "SKTimer.h"
#include "SKRenderer.h"
#include "SKGraphicInclude.h"
#include "SKRenderThread.h"
using namespace SKEngine2;
SKProfilerNode::SKProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
{
	m_Name = Name;
	if (ParentName)
	{
		m_ParentName = ParentName;
	}
	
	SKProfiler::AddProfileNode(this);
	m_pParentNode = NULL;
}
void SKProfilerNode::AddChild(SKProfilerNode * pChild)
{
	SKMAC_ASSERT(pChild && !pChild->m_pParentNode);
	pChild->m_pParentNode = this;
	m_pChildNode.AddElement(pChild);
}
void SKProfilerNode::Draw(unsigned int uiLayer)
{
	OnDraw(uiLayer);
	SKProfiler::ms_uiCurCount++;
	for (unsigned int i = 0; i < m_pChildNode.GetNum();i++)
	{
		m_pChildNode[i]->Draw(uiLayer + 1);
	}
}
IMPLEMENT_INITIAL_ONLY_BEGIN(SKProfiler);
ADD_INITIAL_FUNCTION(InitialDefaultState)
IMPLEMENT_INITIAL_ONLY_END
unsigned int SKProfiler::ms_uiCurCount = 0;
SKArray<SKProfilerNode *> SKProfiler::m_pRoot;
TCHAR *  SKProfiler::ms_LayerFlag[] = { _T(""),
										_T("  "),
										_T("    "),
										_T("      "),
										_T("        "),
										_T("          "),
										_T("            "),
										_T("              "),
										_T("                "),
										_T("                  ") };
bool SKProfiler::InitialDefaultState()
{
	for (unsigned int i = 0; i < GetProfilerNode().GetNum(); i++)
	{
		if (GetProfilerNode()[i]->m_ParentName == SKString::ms_StringNULL)
		{
			m_pRoot.AddElement(GetProfilerNode()[i]);
		}
		else
		{
			bool bFound = false;
			for (unsigned int j = 0; j < GetProfilerNode().GetNum(); j++)
			{
				if (GetProfilerNode()[i]->m_ParentName == GetProfilerNode()[j]->m_Name)
				{
					GetProfilerNode()[j]->AddChild(GetProfilerNode()[i]);
					bFound = true;
					break;
				}
			}
			if (bFound == false)
			{
				m_pRoot.AddElement(GetProfilerNode()[i]);
			}
			
		}
	}
	return 1;
}
void SKProfiler::Draw()
{
#ifdef PROFILER
	for (unsigned int i = 0; i < m_pRoot.GetNum(); i++)
	{
		m_pRoot[i]->Draw( 0);
	}
	ClearAll();
#endif	
}
void SKProfiler::ClearAll()
{
	for (unsigned int i = 0; i < GetProfilerNode().GetNum(); i++)
	{
		GetProfilerNode()[i]->Clear();
	}
	ms_uiCurCount = 0;
}

void SKProfiler::AddProfileNode(SKProfilerNode * pProfilerNode)
{
	GetProfilerNode().AddElement(pProfilerNode);
}
void SKProfiler::GetProfilerNameString(const SKString & Name, unsigned int uiLayer, SKString & OutString)
{
	OutString = SKProfiler::ms_LayerFlag[uiLayer] + Name;
}
SKTimeProfilerNode::SKProfilerNodeTImer::SKProfilerNodeTImer(SKTimeProfilerNode * pProfilerNode)
{
	m_fBeginTime = (SKTimer::ms_pTimer != NULL ? (SKREAL)SKTimer::ms_pTimer->GetGamePlayTime() : 0.0f);
	m_pOwner = pProfilerNode;
}
SKTimeProfilerNode::SKProfilerNodeTImer::~SKProfilerNodeTImer()
{
	m_pOwner->m_fProfilerTime += (SKTimer::ms_pTimer != NULL ? (SKREAL)SKTimer::ms_pTimer->GetGamePlayTime() : 0.0f) - m_fBeginTime;
}
SKTimeProfilerNode::SKTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:SKProfilerNode(Name, ParentName)
{
	m_fProfilerTime = 0.0f;
}
void SKTimeProfilerNode::OnDraw(unsigned int uiLayer)
{
	SKString Out;
	SKProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%fms");
	SKRenderer::ms_pRenderer->DrawText(0, SKProfiler::ms_uiCurCount * 20, SKDWCOLORABGR(255, 0, 255, 0), Out.GetBuffer(), m_fProfilerTime);
}


SKCountProfilerNode::SKProfilerNodeCounter::SKProfilerNodeCounter(SKCountProfilerNode * pProfilerNode, int uiCount)
{
	pProfilerNode->m_uiCounter += uiCount;
}

SKCountProfilerNode::SKCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:SKProfilerNode(Name, ParentName)
{
	m_uiCounter = 0;
}
void SKCountProfilerNode::OnDraw(unsigned int uiLayer)
{
	SKString Out;
	SKProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%d");
	SKRenderer::ms_pRenderer->DrawText(0, SKProfiler::ms_uiCurCount * 20, SKDWCOLORABGR(255, 255, 255, 255), Out.GetBuffer(), m_uiCounter);
}
SKNoClearCountProfilerNode::SKNoClearCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:SKCountProfilerNode(Name, ParentName)
{
	m_uiCounter = 0;
}
SKOnlyTimeProfilerNode::SKProfilerNodeOnlyTImer::SKProfilerNodeOnlyTImer(SKOnlyTimeProfilerNode * pProfilerNode, float fProfilerTime)
{
	pProfilerNode->m_fProfilerTime = fProfilerTime;
}

SKOnlyTimeProfilerNode::SKOnlyTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName)
:SKProfilerNode(Name, ParentName)
{
	m_fProfilerTime = 0.0f;
}
void SKOnlyTimeProfilerNode::OnDraw(unsigned int uiLayer)
{
	SKString Out;
	SKProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%f");
	SKRenderer::ms_pRenderer->DrawText(0, SKProfiler::ms_uiCurCount * 20, SKDWCOLORABGR(255, 255, 255, 0), Out.GetBuffer(), m_fProfilerTime);
}