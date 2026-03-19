#include "SKTerrainNode.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
#include "SKStream.h"
IMPLEMENT_RTTI_NoCreateFun(SKTerrainNode,SKMeshNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKTerrainNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKTerrainNode,SKMeshNode)
REGISTER_PROPERTY(m_uiTesselationLevel,TesselationLevel,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNumX,NumX,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiNumZ,NumZ,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fHeightScale,HeightScale,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pHeight,m_uiTotleNum,HeightData);
END_ADD_PROPERTY

SKTerrainNode::SKTerrainNode()
{
	m_uiTesselationLevel = MIN_TESSELATION_LEVEL;
	m_uiNumX = MIN_NUM;
	m_uiNumZ = MIN_NUM;

	m_pHeight = NULL;
	m_fHeightScale = 1.0f;
	m_uiTotleNum = 0;
}

SKTerrainNode::~SKTerrainNode()
{
	SKMAC_DELETEA(m_pHeight);
}

void SKTerrainNode::SetTesselationLevel(unsigned int uiTesselationLevel)
{
	m_uiTesselationLevel = uiTesselationLevel;
	if(m_uiTesselationLevel > m_uiNumX)
	{
		m_uiTesselationLevel = m_uiNumX;
	}

	if(m_uiTesselationLevel > m_uiNumZ)
	{
		m_uiTesselationLevel = m_uiNumZ;
	}

	if (m_uiTesselationLevel > MAX_TESSELATION_LEVEL)
	{
		m_uiTesselationLevel = MAX_TESSELATION_LEVEL;
	}
	if (m_uiTesselationLevel < MIN_TESSELATION_LEVEL)
	{
		m_uiTesselationLevel = MIN_TESSELATION_LEVEL;
	}
}
void SKTerrainNode::SetNum(unsigned int uiNumX,unsigned int uiNumZ)
{
	m_uiNumX = uiNumX;
	m_uiNumZ = uiNumZ;
	if (m_uiNumX > MAX_NUM)
	{
		m_uiNumX = MAX_NUM;
	}
	if(m_uiNumX < MIN_NUM)
	{
		m_uiNumX = MIN_NUM;
	}
	
	if (m_uiNumZ > MAX_NUM)
	{
		m_uiNumZ = MAX_NUM;
	}
	if(m_uiNumZ < MIN_NUM)
	{
		m_uiNumZ = MIN_NUM;
	}
	if (m_uiTesselationLevel > m_uiNumX || m_uiTesselationLevel > m_uiNumZ)
	{
		SetTesselationLevel(m_uiTesselationLevel);
	}
	m_uiTotleNum = ((1 << m_uiNumX) + 1) * ((1 << m_uiNumZ) + 1);
}

bool SKTerrainNode::CreateTerrain(unsigned int uiNumX,unsigned int uiNumZ,unsigned int uiTesselationLevel)
{
	
	SetNum(uiNumX,uiNumZ);
	SetTesselationLevel(uiTesselationLevel);
	SKMAC_DELETEA(m_pHeight);
	m_pHeight = SK_NEW unsigned char[m_uiTotleNum];
	if (!m_pHeight)
	{
		return 0;
	}
	SKMemset(m_pHeight,0,m_uiTotleNum * sizeof(unsigned char));
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}
bool SKTerrainNode::CreateTarrainFromHeightMap(const TCHAR *pFileName,unsigned int uiTesselationLevel,SKREAL fHeightScale)
{
	if (!pFileName)
	{
		return 0;
	}
	
	SKFile *pFile = NULL;
	pFile = SK_NEW SKFile();
	if (!pFile)
	{
		return 0;
	}

	unsigned int uiHeightSize = 0;
	SKMAC_DELETEA(m_pHeight);

	SKString TerrainPath = SKResourceManager::ms_TerrainPath + pFileName;
	if(!pFile->Open(TerrainPath.GetBuffer(),SKFile::OM_RB))
	{
		SKMAC_DELETE(pFile);
		return 0;
	}
	
	pFile->Read(&uiHeightSize, 1, sizeof(unsigned int));
	m_pHeight= SK_NEW unsigned char [uiHeightSize * uiHeightSize];

	if (!m_pHeight)
	{
		return 0;
	}

	pFile->Read(m_pHeight, 1, uiHeightSize * uiHeightSize);

	SKMAC_DELETE(pFile);

	unsigned int uiNum = 0;
	uiHeightSize = uiHeightSize - 1;
	_asm bsr eax, uiHeightSize
	_asm mov uiNum, eax

	SetNum(uiNum,uiNum);
	SetTesselationLevel(uiTesselationLevel);
	m_fHeightScale = fHeightScale;
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return 1;
}

unsigned char SKTerrainNode::GetRawHeight(unsigned int uiX,unsigned int uiZ)
{
	unsigned int uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ];
	}
	return 0;
}
SKREAL SKTerrainNode::GetHeight(unsigned int uiX,unsigned int uiZ)
{
	unsigned int uiLen = ((1 << m_uiNumX) + 1);
	if (m_pHeight && uiX < uiLen && uiZ < uiLen)
	{
		return m_pHeight[uiX * uiLen + uiZ] * m_fHeightScale;
	}
	return 0.0f;

}
