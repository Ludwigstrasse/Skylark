#include "SKAIState.h"
#include "SKActor.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKAIState,SKObject)
bool SKAIState::HandleMessage(SKMessage & Message)
{
	return true;
}
bool SKAIState::Update(double Time)
{
	for (unsigned int i = 0 ; i < m_pStateInputNode.GetNum() ; i++)
	{
		if (m_pStateInputNode[i]->CheckState())
		{
			const SKOutputNode * pOutputNode = m_pStateInputNode[i]->GetOutputLink();
			if (!pOutputNode)
			{
				continue;
			}
			SKAIState * pState = DynamicCast<SKAIState>(pOutputNode->GetOwner());
			if (pState)
			{
				pState->m_pOwner->ChangeState(pState);
			}
			break;
		}
	}
	return true;
}