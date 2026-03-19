#include "SKSurface3Subdivision.h"
#include "SKMemManager.h"
using namespace SKEngine2;
SKSurface3Subdivision::SKSurface3Subdivision()
{
	m_pSurface = NULL;
	m_pPoint = NULL;
	m_uiRow = 0;
	m_uiColumn = 0;
	m_pUV = NULL;
}
/*----------------------------------------------------------------*/
SKSurface3Subdivision::~SKSurface3Subdivision()
{
	SKMAC_DELETEA(m_pPoint);
	SKMAC_DELETEA(m_pUV);
	m_pSurface = NULL;
	m_uiRow = 0;
	m_uiColumn = 0;
}
/*----------------------------------------------------------------*/