#include "SKRevolutionSurface3.h"
using namespace SKEngine2;
SKRevolutionSurface3::SKRevolutionSurface3()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
SKRevolutionSurface3::~SKRevolutionSurface3()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
bool SKRevolutionSurface3::SetCurve(SKCurve3 * pCurve,unsigned int Axis,unsigned int uiPlane)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	m_uiAxis = Axis;
	m_uiPlane = uiPlane;
	return 1;
}
/*----------------------------------------------------------------*/
SKVector3 SKRevolutionSurface3::GetPoint(SKREAL U,SKREAL V)
{
	SKVector3 Temp = m_pCurve->GetPoint(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return SKVector3(Temp.x,Temp.y * COS(VS2PI * V),Temp.y *SIN(VS2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return SKVector3(Temp.x,Temp.z * COS(VS2PI * V),Temp.z *SIN(VS2PI * V));
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return SKVector3(Temp.x * COS(VS2PI * V),Temp.y ,Temp.x *SIN(VS2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return SKVector3(Temp.z * COS(VS2PI * V),Temp.y ,Temp.z *SIN(VS2PI * V));
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return SKVector3(Temp.x * COS(VS2PI * V),Temp.x * SIN(VS2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return SKVector3(Temp.y * COS(VS2PI * V),Temp.y * SIN(VS2PI * V),Temp.z );
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
SKVector3 SKRevolutionSurface3::GetUPartialDerivative(SKREAL U,SKREAL V)
{
	SKVector3 Temp = m_pCurve->GetFirstDerivative(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return SKVector3(Temp.x,Temp.y * COS(VS2PI * V),Temp.y *SIN(VS2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return SKVector3(Temp.x,Temp.z * COS(VS2PI * V),Temp.z *SIN(VS2PI * V));
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return SKVector3(Temp.x * COS(VS2PI * V),Temp.y ,Temp.x *SIN(VS2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return SKVector3(Temp.z * COS(VS2PI * V),Temp.y ,Temp.z *SIN(VS2PI * V));
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return SKVector3(Temp.x * COS(VS2PI * V),Temp.x * SIN(VS2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return SKVector3(Temp.y * COS(VS2PI * V),Temp.y * SIN(VS2PI * V),Temp.z );
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
SKVector3 SKRevolutionSurface3::GetVPartialDerivative(SKREAL U,SKREAL V)
{
	SKVector3 Temp = m_pCurve->GetPoint(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return SKVector3(Temp.x,Temp.y * -COS(VS2PI * V),Temp.y *COS(VS2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return SKVector3(Temp.x,Temp.z * -COS(VS2PI * V),Temp.z *COS(VS2PI * V));
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return SKVector3(Temp.x * -COS(VS2PI * V),Temp.y ,Temp.x *COS(VS2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return SKVector3(Temp.z * -COS(VS2PI * V),Temp.y ,Temp.z *COS(VS2PI * V));
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return SKVector3(Temp.x * -COS(VS2PI * V),Temp.x * COS(VS2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return SKVector3(Temp.y * -COS(VS2PI * V),Temp.y * COS(VS2PI * V),Temp.z );
		}
		else
			return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/