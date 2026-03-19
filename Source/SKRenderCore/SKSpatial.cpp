#include "SKSpatial.h"
#include "SKController.h"
#include "SKRenderer.h"
#include "SKGeometry.h"
#include "SKNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKMaterial.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKSpatial,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKSpatial)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKSpatial, SKObject)
REGISTER_PROPERTY(m_WorldBV,WorldBV,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiCullMode,CullMode,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_World, WorldTransform, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_Local, LocalTransform, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_pParent, Parent, SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bInheritScale, InheritScale, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bInheritRotate, InheritRotate, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bInheritTranlate, InheritTranlate, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bEnable, Enable, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_ControllerArray, ControllerArray, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME );
REGISTER_PROPERTY(m_bIsStatic, IsStatic, SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bIsChanged, IsChanged, SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKSpatial::SKSpatial()
{
	

	m_uiCullMode = CM_DYNAMIC;

	m_bIsVisibleUpdate = false;

	//m_bForceNoCull = false;
	m_pParent = NULL;
	m_bInheritScale = true;
	m_bInheritRotate = true;
	m_bInheritTranlate = true;
	m_ControllerArray.Clear();
	m_bIsStatic = 1;
	m_bIsChanged = true;
	m_bEnable = true;
}
SKSpatial::~SKSpatial()
{
	m_pParent = NULL;

	DeleteAllController();
}

void SKSpatial::UpdateAll(double dAppTime)
{
	UpdateNodeAll(dAppTime);

	if (m_bIsVisibleUpdate)
	{
		m_bEnable = false;
	}

}
void SKSpatial::ComputeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
	if(m_uiCullMode == CM_ALAWAYS)
		return ;
	if(m_uiCullMode == CM_NEVER)
		bNoCull = true;

	unsigned int uiSavePlaneState = Culler.GetPlaneState();
	if(bNoCull)
	{
		ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
		if (m_bIsVisibleUpdate)
		{
			m_bEnable = true;
		}
	}
	else
	{
		unsigned int uiVSF = Culler.IsVisible(m_WorldBV);
		if (uiVSF == SKCuller::SKF_ALL)
		{
			bNoCull = true;
			ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
			if (m_bIsVisibleUpdate)
			{
				m_bEnable = true;
			}
		}
		else if (uiVSF == SKCuller::SKF_PARTIAL || Culler.ForceNoCull(this))
		{
			ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);	
			if (m_bIsVisibleUpdate)
			{
				m_bEnable = true;
			}
		}

	}
	Culler.SetPlaneState(uiSavePlaneState);

}
void SKSpatial::UpDateView(SKCuller & Culler,double dAppTime)
{
}
void SKSpatial::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}
const SKAABB3 & SKSpatial::GetWorldAABB()const
{
	return m_WorldBV;

}
const SKVector3 & SKSpatial::GetLocalScale()const
{
	return m_Local.GetScale();
}
const SKVector3 & SKSpatial::GetLocalTranslate()const
{
	return m_Local.GetTranslate();
}
const SKMatrix3X3 & SKSpatial::GetLocalRotate()const
{
	return m_Local.GetRotate();
}

const SKTransform & SKSpatial::GetLocalTransform()
{
	m_Local.GetCombine();
	return m_Local;
}
SKSpatial *SKSpatial::GetParent()const
{
	return m_pParent;

}
void SKSpatial::SetParent(SKSpatial * pParent)
{

	m_pParent = pParent;
	m_bIsChanged = true;
}

void SKSpatial::UpdateTransform(double dAppTime)
{
	if (m_pParent)
	{
		if (!m_pParent->m_bIsStatic)
		{
			m_bIsStatic = 0;
		}
		if (m_pParent->m_bIsChanged)
		{
			m_bIsChanged = true;
		}
	}
	if (m_bIsChanged)
	{
		if (m_pParent)
		{
			unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
			m_World.Product(m_Local, m_pParent->m_World, TransFormFlag);

		}
		else
			m_World = m_Local;
	}
	


	/*updateNodeController*/

}

void SKSpatial::GetLocalDir(SKVector3 &Dir, SKVector3 &Up, SKVector3 & Right)const
{
	m_Local.GetDir(Dir, Up, Right);
}
void SKSpatial::GetWorldDir(SKVector3 &Dir, SKVector3 &Up, SKVector3 & Right)
{
	m_World.GetDir(Dir, Up, Right);
}
const SKVector3 & SKSpatial::GetWorldScale()
{
	return m_World.GetScale();
}
const SKVector3 & SKSpatial::GetWorldTranslate()
{
	return m_World.GetTranslate();
}
const SKMatrix3X3 & SKSpatial::GetWorldRotate()
{
	return m_World.GetRotate();
}
const SKTransform & SKSpatial::GetWorldTransform()
{
	m_World.GetCombine();
	return m_World;
}
void SKSpatial::SetLocalTransform(const SKTransform & LocalTransform)
{
	m_bIsChanged = true;
	m_Local = LocalTransform;
	UpdateAll(0.0f);
}
void SKSpatial::SetLocalMat(const SKMatrix3X3W SKMat)
{
	m_bIsChanged = true;
	m_Local.SetMatrix(SKMat);
	UpdateAll(0.0f);
}
void SKSpatial::SetLocalScale(const SKVector3 & fScale)
{
	m_bIsChanged = true;
	m_Local.SetScale(fScale);
	UpdateAll(0.0f);

}
void SKSpatial::SetLocalTranslate(const SKVector3& Translate)
{
	m_bIsChanged = true;
	m_Local.SetTranslate(Translate);
	UpdateAll(0.0f);

}
void SKSpatial::SetLocalRotate(const SKMatrix3X3 & Rotate)
{
	m_bIsChanged = true;
	m_Local.SetRotate(Rotate);
	UpdateAll(0.0f);

}
void SKSpatial::SetWorldScale(const SKVector3 & fScale)
{
	SKSpatial * pParent = GetParent();
	if (pParent)
	{
		SKTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		SKTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetScale(fScale);
		SKTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalScale(fScale);
	}
}
void SKSpatial::SetWorldTranslate(const SKVector3& Translate)
{
	m_bIsChanged = true;

	SKSpatial * pParent = GetParent();
	if (pParent)
	{
		SKTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		SKTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetTranslate(Translate);
		SKTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalTranslate(Translate);
	}
}
void SKSpatial::SetWorldRotate(const SKMatrix3X3 & Rotate)
{
	SKSpatial * pParent = GetParent();
	if (pParent)
	{
		SKTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		SKTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetRotate(Rotate);
		SKTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalRotate(Rotate);
	}
}
void SKSpatial::SetWorldTransform(const SKTransform & LocalTransform)
{
	SKSpatial * pParent = GetParent();
	if (pParent)
	{
		SKTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		SKTransform NewWorld;
		NewWorld = LocalTransform;

		SKTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalTransform(LocalTransform);
	}
}
void SKSpatial::SetWorldMat(const SKMatrix3X3W SKMat)
{
	SKSpatial * pParent = GetParent();
	if (pParent)
	{
		SKTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);
		SKTransform NewWorld;
		NewWorld.SetMatrix(SKMat);
		SKTransform NewLocal;
		unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
		NewLocal.Product(NewWorld, Inv, TransFormFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalMat(SKMat);
	}
}
SKArray<SKPointer<SKController>> * SKSpatial::GetControllerList()
{
	return &m_ControllerArray;
}
void SKSpatial::SetDynamic(bool bIsDynamic)
{
	if (!m_ControllerArray.GetNum())
	{
		m_bIsStatic = !bIsDynamic;
	}
}
bool SKSpatial::AddController(SKController * pController)
{
	if (!pController)
		return false;
	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		if (m_ControllerArray[i] == pController)
			return 1;
	}
	if (!pController->SetObject(this))
		return false;
	m_ControllerArray.AddElement(pController);

	m_bIsStatic = 0;
	return 1;
}

SKController * SKSpatial::DeleteController(unsigned int ID)
{
	if (ID >= m_ControllerArray.GetNum())
		return NULL;
	SKController * Temp = m_ControllerArray[ID];
	m_ControllerArray.Erase(ID);

	Temp->SetObject(NULL);
	return Temp;
}

unsigned int SKSpatial::DeleteController(SKController *pController)
{
	if (!pController)
		return SKMAX_INTEGER;
	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{

		if (m_ControllerArray[i] == pController)
		{
			pController->SetObject(NULL);
			m_ControllerArray.Erase(i);

			return i;
		}
	}

	return SKMAX_INTEGER;
}

void SKSpatial::DeleteAllController()
{
	m_ControllerArray.Clear();
}

SKController *SKSpatial::GetController(unsigned int ID)const
{
	if (ID >= m_ControllerArray.GetNum())
		return NULL;
	return m_ControllerArray[ID];
}

void SKSpatial::UpdateController(double dAppTime)
{

	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		if (m_ControllerArray[i]->m_bIsVisibleUpdate)
		{
			if (!m_bEnable)
			{
				continue;
			}
		}
		m_ControllerArray[i]->SetObject(this);
		m_ControllerArray[i]->Update(dAppTime);

	}

}

void SKSpatial::ClearAllTime()
{
	for (unsigned int i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		m_ControllerArray[i]->ClearTime();
	}
}
