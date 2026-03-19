#include "SKSteer.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKSteerComponent,SKObject)
SKSteerComponent::SKSteerComponent()
{
	m_pSteer = NULL;
}
SKSteerComponent::~SKSteerComponent()
{
	m_pSteer = NULL;
}
IMPLEMENT_RTTI_NoCreateFun(SKSteer,SKObject)
SKSteer::SKSteer()
{
	m_pActor = NULL;
}
SKSteer::~SKSteer()
{
	m_pActor = NULL;
	m_ComponentArray.Clear();
}
SKVector3 SKSteer::Compute()
{
	SKVector3 Force;
	for (unsigned int i = 0 ; i < m_ComponentArray.GetNum() ;i++)
	{
		if (!AddForce(Force,m_ComponentArray[i]->Compute() * m_ComponentArray[i]->m_fWeight))
		{
			return Force;
		}
	}
	return Force;
}
bool SKSteer::AddForce(SKVector3 & CurForce,SKVector3 & AddForce)
{
	SKREAL CurForceLen = CurForce.GetLength();
	SKREAL MaxDriverForce = GetOwner()->GetMaxDriverForce();
	SKREAL RemainForce = MaxDriverForce - CurForceLen;
	if (RemainForce <= 0.0f)
	{
		return false;
	}
	SKREAL AddForceLen = AddForce.GetLength();
	if (AddForceLen < RemainForce)
	{
		CurForce += AddForce;
	}
	else
	{
		AddForce.Normalize();
		CurForce += AddForce * RemainForce;
	}
	return true;
}