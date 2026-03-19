#include "SKCuller.h"
#include "SKSpatial.h"
#include "SKGeometry.h"
#include "SKGraphicInclude.h"
#include "SKSceneManager.h"
#include "SKPointLight.h"
#include "SKGeometry.h"
#include "SKSpotLight.h"
#include "SKMaterial.h"
#include "SKSortLight.h"
using namespace SKEngine2;
SKRenderContext SKRenderContext::ms_RenderContextNULL;
DECLEAR_TIME_PROFILENODE(CullerSort, ViewFamilyUpdate)
SKCuller::SKCuller(int iUseMaterialIndex)
{
	m_uiPlaneNum = 0;
	m_uiPlaneState =0xffffffff;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		for (unsigned int i = 0 ; i < SKT_MAX ; i++)
		{
			m_VisibleSet[j][i].Clear();
		}
	}
	
	m_LightSet.Clear();
	m_pCamera = NULL;
	m_iUseMaterialIndex = iUseMaterialIndex;
}
SKCuller::~SKCuller()
{

}
bool SKCuller::PushPlane(const SKPlane3 & Plane)
{
	if(m_uiPlaneNum > SK_MAX_PLANE_NUM)
		return false;
	m_Plane[m_uiPlaneNum] = Plane;
	m_uiPlaneNum++;
	return true;
}
bool SKCuller::PushCameraPlane(SKCamera &Camera)
{
	SKPlane3 Plane[SKCamera::CP_MAX];
	Camera.GetPlane(Plane);
	for(unsigned int i =0 ; i < SKCamera::CP_MAX; i++)
	{
		if(!PushPlane(Plane[i]))
			return 0;
	}
	SKArray<SKPlane3> & CustomCullPlane = Camera.m_CustomCullPlane;
	for (unsigned int i = 0; i < CustomCullPlane.GetNum(); i++)
	{
		if (!PushPlane(CustomCullPlane[i]))
			return 0;
	}
	m_pCamera = &Camera;
	return 1;
}
bool SKCuller::PopPlane(SKPlane3 &Plane)
{
	if(!m_uiPlaneNum)
		return false;
	Plane = m_Plane[m_uiPlaneNum];
	m_uiPlaneNum--;
	return true;
}
bool SKCuller::InsertObject(SKRenderContext &VisibleContext,unsigned int uiVisibleSetType,unsigned int uiRenderGroup)
{
	if(uiVisibleSetType >= SKT_MAX || uiRenderGroup >= RG_MAX)
		return 0;
	m_VisibleSet[uiRenderGroup][uiVisibleSetType].AddElement(VisibleContext);
	return 1;
}
bool SKCuller::HasLight(SKLight * pLight)
{
	return m_LightSet.FindElement(pLight) != m_LightSet.GetNum();
}
void SKCuller::GetAndSortLight()
{
	if (GetLightNum() == 0)
	{
		return;
	}
	for (unsigned int uiRenderGroup = 0; uiRenderGroup < RG_MAX; uiRenderGroup++)
	{
		for (unsigned int t = 0; t <= SKT_MAX; t++)
		{
			for (unsigned int j = 0; j < GetVisibleNum(t, uiRenderGroup); j++)
			{
				SKRenderContext& VisibleContext = GetVisibleSpatial(j, t, uiRenderGroup);

				SKGeometry * pGeometry = VisibleContext.m_pGeometry;

				for (unsigned int l = 0; l < GetLightNum(); l++)
				{
					SKLight * pLight = GetLight(l);
					if (pLight->IsRelative(pGeometry))
					{
						if (pLight->GetLightType() == SKLight::LT_SKY)
						{
							VisibleContext.m_pInDirectLight.AddElement(pLight);
						}
						else
						{
							VisibleContext.m_pDirectLight.AddElement(pLight);
						}

					}
				}
				VisibleContext.m_pInDirectLight.Sort(0, VisibleContext.m_pInDirectLight.GetNum() - 1, LightPriority());
				VisibleContext.m_pDirectLight.Sort(0, VisibleContext.m_pDirectLight.GetNum() - 1, LightPriority());
			}
		}
	}
}
unsigned int SKCuller::IsVisible(const SKSphere3 &S,bool bClearState)
{
	if(!m_uiPlaneNum)
		return 0;
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = S.RelationWith(m_Plane[iP]);

			if (iSide == SKFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return SKF_NONE;
			}

			if (iSide == SKBACK)
			{

				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return SKF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return SKF_PARTIAL;
	}
}
unsigned int SKCuller::IsVisible(const SKVector3 &Point,bool bClearState)
{
	if(!m_uiPlaneNum)
		return 0;
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = Point.RelationWith(m_Plane[iP]);

			if (iSide == SKFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return SKF_NONE;
			}

			if (iSide == SKBACK)
			{

				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return SKF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return SKF_PARTIAL;
	}
}
unsigned int SKCuller::IsVisible(const SKAABB3 &BV,bool bClearState)
{
	if(!m_uiPlaneNum)
		return 0;
	int iP = m_uiPlaneNum - 1;
	unsigned int uiMask = 1 << iP;
	unsigned int uiPlaneInNum = 0;
	for (unsigned int i = 0; i < m_uiPlaneNum; i++, iP--, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			int iSide = BV.RelationWith(m_Plane[iP]);

			if (iSide == SKFRONT)
			{
				if (bClearState == true)
				{
					ClearPlaneState();
				}
				return SKF_NONE;
			}

			if (iSide == SKBACK)
			{
				
				m_uiPlaneState &= ~uiMask;
				uiPlaneInNum++;
			}
		}
		else
		{
			uiPlaneInNum++;
		}
	}
	if (uiPlaneInNum == m_uiPlaneNum)
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return SKF_ALL;
	}
	else
	{
		if (bClearState == true)
		{
			ClearPlaneState();
		}
		return SKF_PARTIAL;
	}
}
bool SKCuller::RenderPriority::operator()(SKRenderContext & p1,SKRenderContext & p2)
{
	unsigned int uiMaterialAddr1 = 0;
	unsigned int uiMaterialAddr2 = 0;
	unsigned int uiMeshDateAddr1 = 0;
	unsigned int uiMeshDateAddr2 = 0;

	SKMaterial * pMaterial1 = p1.m_pMaterialInstance->GetMaterial();
	SKMaterial * pMaterial2 = p2.m_pMaterialInstance->GetMaterial();
	uiMaterialAddr1 = (unsigned int)pMaterial1;
	uiMaterialAddr2 = (unsigned int)pMaterial2;



	uiMeshDateAddr1 = (unsigned int)(p1.m_pGeometry->GetMeshDate());

	uiMeshDateAddr2 = (unsigned int)(p2.m_pGeometry->GetMeshDate());

	if (pMaterial1->m_uiCustomLayer == pMaterial2->m_uiCustomLayer)
	{
		if (uiMaterialAddr1 > uiMaterialAddr2)
		{
			return 1;
		}
		else if (uiMaterialAddr1 < uiMaterialAddr2)
		{
			return 0;
		}
		else
		{
			if (uiMeshDateAddr1 > uiMeshDateAddr2)
			{
				return 1;
			}
			else if (uiMeshDateAddr1 < uiMeshDateAddr2)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else if (pMaterial1->m_uiCustomLayer > pMaterial2->m_uiCustomLayer)
	{
		return 0;
	}
	else 
	{
		return 1;
	}


}
void SKCuller::Sort()
{
	ADD_TIME_PROFILE(CullerSort);
	SKCamera * pCamera = m_pCamera;
	for (unsigned int j = 0 ; j < RG_MAX ; j++)
	{
		if (m_VisibleSet[j][SKT_BASE].GetNum() > 0)
		{
			m_VisibleSet[j][SKT_BASE].Sort(0,m_VisibleSet[j][SKT_BASE].GetNum() - 1,RenderPriority());

		}
		
		if (m_VisibleSet[j][SKT_ALPHABLEND].GetNum() > 0 && pCamera)
		{
			m_VisibleSet[j][SKT_ALPHABLEND].Sort(0,m_VisibleSet[j][SKT_BASE].GetNum() - 1,AlphaPriority(pCamera));

		}

		if (m_VisibleSet[j][SKT_ALPHATEST].GetNum() > 0)
		{
			m_VisibleSet[j][SKT_ALPHATEST].Sort(0,m_VisibleSet[j][SKT_ALPHATEST].GetNum() - 1,RenderPriority());
		}
	}
	
}
bool SKCuller::AlphaPriority::operator()(SKRenderContext & p1,SKRenderContext & p2)
{
	if (!m_pCamera)
	{
		return 1;
	}
	SKVector3 vLength1 = m_pCamera->GetWorldTranslate() - p1.m_pGeometry->GetWorldTranslate(); 
	SKVector3 vLength2 = m_pCamera->GetWorldTranslate() - p2.m_pGeometry->GetWorldTranslate();

	if (vLength1.GetSqrLength() > vLength2.GetSqrLength())
	{
		return 0;
	}
	return 1;

}
void SKCuller::InsertLight(SKLight * pLight)
{
	if (pLight)
	{
		m_LightSet.AddElement(pLight);
	}
}
bool SKCuller::ForceNoCull(const SKSpatial * pSpatial)
{
	return false;
}
bool SKCuller::CullConditionNode(const SKMeshNode * pMeshNode)
{
	return false;
}
bool SKCuller::CullGeometry(SKGeometry * pGeometry)
{
	SKShadowVolumeGeometry * pSVG = DynamicCast<SKShadowVolumeGeometry>(pGeometry);
	if (pSVG)
	{
		return true;
	}
	return false;

}
void SKCuller::GetAllVisibleAABB(unsigned int uiVisibleSetType, unsigned int uiRenderGroup, SKArray<SKAABB3> & AABBArray, unsigned int GetFlag)
{

	for (unsigned int i = 0 ; i < m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum() ;i++)
	{
		SKGeometry * pGeometry = m_VisibleSet[uiRenderGroup][uiVisibleSetType][i].m_pGeometry;
		if (GetFlag & GF_NO_CASTER_SHADOW)
		{
			if (pGeometry->GetMeshNode()->m_bCastShadow == false)
			{
				continue;
			}
		}

		AABBArray.AddElement(pGeometry->GetWorldAABB());
		
		
	}

}
SKShadowCuller::SKShadowCuller(int iUseMaterialIndex)
:SKCuller(iUseMaterialIndex)
{

}
SKShadowCuller::~SKShadowCuller()
{

}
bool SKShadowCuller::CullConditionNode(const SKMeshNode * pMeshNode)
{
	if (pMeshNode->m_bCastShadow == true)
	{
		return false;
	}
	return true;
}
bool SKShadowCuller::TestSweptSphere(SKSphere3 & Sphere,SKVector3 & SweptDir)
{
	//  algorithm -- get all 12 intersection points of the swept sphere with the view frustum
	//  for all points >0, displace sphere along the sweep driection.  if the displaced sphere
	//  is inside the frustum, return TRUE.  else, return FALSE
	float displacements[12];
	int cnt = 0;
	float a, b;

	for (int i=0; i<6; i++)
	{
		if (SweptSpherePlaneIntersect(a, b, m_Plane[i], Sphere, SweptDir))
		{
			if (a>=0.f)
				displacements[cnt++] = a;
			if (b>=0.f)
				displacements[cnt++] = b;
		}
	}

	for (int i=0; i<cnt; i++)
	{
		SKSphere3 displacedSphere = Sphere;
		displacedSphere.m_Center += SweptDir * displacements[i];
		displacedSphere.m_fRadius *= 1.1f;
		if(TestSphere(displacedSphere))
		{
			return true;
		}
	}
	return false;
}
bool SKShadowCuller::SweptSpherePlaneIntersect(float& t0, float& t1, SKPlane3 &Plane, SKSphere3 & Shpere, SKVector3 & SweptDir)
{
	float b_dot_n = Plane.GetN().Dot(Shpere.m_Center) + Plane.GetfD();
	float d_dot_n = Plane.GetN().Dot(SweptDir);

	if (d_dot_n == 0.f)
	{
		if (b_dot_n <= Shpere.m_fRadius)
		{
			//  effectively infinity
			t0 = 0.f;
			t1 = 1e32f;
			return true;
		}
		else
			return false;
	}
	else
	{
		float tmp0 = ( Shpere.m_fRadius - b_dot_n) / d_dot_n;
		float tmp1 = (-Shpere.m_fRadius - b_dot_n) / d_dot_n;
		t0 = min(tmp0, tmp1);
		t1 = max(tmp0, tmp1);
		return true;
	}
}
bool SKShadowCuller::TestSphere(SKSphere3 & Shpere) const
{	
	for (int i = 0; i < 6 ; i++)
	{
		if (Shpere.RelationWith(m_Plane[i]) == SKFRONT)
		{
			return false;
		}
	}
	return true;
}
void SKShadowCuller::Sort()
{
	// 
}
SKVolumeShadowMapCuller::SKVolumeShadowMapCuller(int iUseMaterialIndex)
:SKShadowCuller(iUseMaterialIndex)
{

}
SKVolumeShadowMapCuller::~SKVolumeShadowMapCuller()
{

}
bool SKVolumeShadowMapCuller::CullConditionNode(const SKMeshNode * pMeshNode)
{
	if (pMeshNode->m_bCastShadow == true || pMeshNode->m_bReceiveShadow == true)
	{
		return false;
	}
	return true;
}
bool SKVolumeShadowMapCuller::ForceNoCull(const SKSpatial * pSpatial)
{
	if (m_pLocalLight->GetLightType() == SKLight::LT_POINT)
	{
		SKPointLight * pPointLight = (SKPointLight *)m_pLocalLight;
		SKAABB3 AABB(pPointLight->GetWorldTranslate(), pPointLight->m_Range, pPointLight->m_Range, pPointLight->m_Range);
		if (AABB.RelationWith(pSpatial->GetWorldAABB()) == SKINTERSECT)
		{
			return true;
		}
	}
	else if (m_pLocalLight->GetLightType() == SKLight::LT_DIRECTION)
	{
		SKAABB3  aabb = pSpatial->GetWorldAABB();

		SKVector3 Center = aabb.GetCenter();
		SKVector3 Temp = aabb.GetMaxPoint() - Center;

		SKSphere3 TempSphere(Center, Temp.GetLength());

		SKVector3 SweptDir, Up, Right;
		m_pLocalLight->GetWorldDir(SweptDir, Up, Right);
		if (TestSweptSphere(TempSphere, SweptDir))
			return true;
	}
	return false;
}
bool SKVolumeShadowMapCuller::CullGeometry(SKGeometry * pGeometry)
{
	
	SKShadowVolumeGeometry * pSVG = DynamicCast<SKShadowVolumeGeometry>(pGeometry);
	if (SKRenderer::ms_pRenderer->IsSupportGS())
	{
		if (!pSVG)
		{
			return true;
		}
		return false;
	}
	else
	{
		if (pSVG)
		{
			return false;
		}
		return true;
	}
	
}
SKDirShadowMapCuller::SKDirShadowMapCuller(int iUseMaterialIndex)
:SKShadowCuller(iUseMaterialIndex)
{

}
SKDirShadowMapCuller::~SKDirShadowMapCuller()
{

}
bool SKDirShadowMapCuller::ForceNoCull(const SKSpatial * pSpatial)
{

	SKAABB3  aabb = pSpatial->GetWorldAABB();

	SKVector3 Center = aabb.GetCenter();
	SKVector3 Temp = aabb.GetMaxPoint() - Center;

	SKSphere3 TempSphere(Center,Temp.GetLength());

	SKDirectionLight * pDirLight = (SKDirectionLight *)m_pLocalLight;
	SKVector3 Dir, Up, Right;
	pDirLight->GetWorldDir(Dir, Up, Right);
	if (TestSweptSphere(TempSphere, Dir))
	 	return true;
	return false;

}


