#include "SKVariant.h"
#include "SKName.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
TCHAR * SKVariant::ms_VariantMap[V_MAX] = 
{
	_T("Null"),
	_T("Bool"),
	_T("Int"),
	_T("Real"),
	_T("Vector2"),
	_T("Vector3"),
	_T("Vector3W"),
	_T("Matrix3X3"),
	_T("Matrix3X3W"),
	_T("String"),
	_T("Object")
};
unsigned int SKVariant::MapVariantType(const SKString& inType)
{
	for (unsigned int i = 0 ; i < V_MAX ; i++)
	{
		if (inType == ms_VariantMap[i])
		{
			return i;
		}
	}
	return V_MAX;
}
SKString SKVariant::MapVariantString(unsigned int inType)
{
	if (inType >= V_MAX)
	{
		return _T("");
	}
	return ms_VariantMap[inType];
}
SKVariant::SKVariant(unsigned int Type)
{
	m_uiType = Type;	
	m_pP = NULL;
}
SKVariant::~SKVariant()
{
	Clear();
}
void SKVariant::Clear()
{
	if (m_uiType == V_STRING || m_uiType == V_OBJECT)
	{
		if(m_pP)
		{
			((SKReference *)m_pP)->DecreRef();
			m_pP = NULL;
		}			
	}
	m_uiType = V_NULL;	
}
SKVariant::SKVariant(bool b)
{
	*this = b;
}
SKVariant::SKVariant(int i)
{
	*this = i;
}
SKVariant::SKVariant(SKREAL f)
{
	*this = f;
}
SKVariant::SKVariant(const SKVector2 & v)
{
	*this = v;
}
SKVariant::SKVariant(const SKVector3 & v)
{
	*this = v;
}
SKVariant::SKVariant(const SKVector3W & v)
{
	*this = v;
}
SKVariant::SKVariant(const SKMatrix3X3 & m)
{
	*this = m;
}
SKVariant::SKVariant(const SKMatrix3X3W & m)
{
	*this = m;
}
SKVariant::SKVariant(SKObject * p)
{
	*this = p;
}
SKVariant::SKVariant(const TCHAR * s)
{
	*this = s;
}
SKVariant::SKVariant(const SKVariant & Va)
{
	*this = Va;
}
bool SKVariant::operator==(const SKVariant& rhs) const
{
	if (m_uiType != rhs.m_uiType)
	{
		return false;
	}
	switch (m_uiType)
	{
	case V_BOOL:
		return GetBool() == rhs.GetBool();
	case V_INT:
		return GetInt() == rhs.GetInt();
	case V_REAL:
		return GetReal() == rhs.GetReal();
	case V_VECTOR2:
		return GetVector2() == rhs.GetVector2();
	case V_VECTOR3:
		return GetVector3() == rhs.GetVector3();
	case V_VECTOR3W:
		return GetVector3W() == rhs.GetVector3W();
	case V_MATRIX3X3:
		return GetMatrix3X3() == rhs.GetMatrix3X3();
	case V_MATRIX3X3W:
		return GetMatrix3X3W() == rhs.GetMatrix3X3W();
 	case V_STRING:
 		return m_pP ==rhs.m_pP;
 	case V_OBJECT:
 		return GetObjectPtr() == rhs.GetObjectPtr();
	default:
		return false;
	}
}
bool SKVariant::operator==(bool b)const
{
	if (m_uiType != V_BOOL)
	{
		return false;
	}
	return GetBool() == b;
}
bool SKVariant::operator==(int i)const
{
	if (m_uiType != V_INT)
	{
		return false;
	}
	return GetInt() == i;	
}
bool SKVariant::operator==(SKREAL f)const
{
	if (m_uiType != V_REAL)
	{
		return false;
	}
	return GetReal() == f;
}
bool SKVariant::operator==(const SKVector2 & v)const
{
	if (m_uiType != V_VECTOR2)
	{
		return false;
	}
	return GetVector2() == v;
}
bool SKVariant::operator==(const SKVector3 & v)const
{
	if (m_uiType != V_VECTOR3)
	{
		return false;
	}
	return GetVector3() == v;
}
bool SKVariant::operator==(const SKVector3W & v)const
{
	if (m_uiType != V_VECTOR3W)
	{
		return false;
	}
	return GetVector3W() == v;
}
bool SKVariant::operator==(const SKMatrix3X3 & m)const
{
	if (m_uiType != V_MATRIX3X3)
	{
		return false;
	}
	return GetMatrix3X3() == m;
}
bool SKVariant::operator==(const SKMatrix3X3W & m)const
{
	if (m_uiType != V_MATRIX3X3W)
	{
		return false;
	}
	return GetMatrix3X3W() == m;
}
bool SKVariant::operator==(const SKObject * p)const
{
	if (m_uiType != V_OBJECT)
	{
		return false;
	}
	return GetObjectPtr() == p;
}
bool SKVariant::operator==(const TCHAR * s)const
{
	if (m_uiType != V_STRING)
	{
		return false;
	}
	return ((SKName *)m_pP)->GetString() == s;
	
}
SKVariant &SKVariant::operator=(const SKVariant &Va)
{
	Clear();
	m_uiType = Va.m_uiType;
	if (Va.m_uiType == V_STRING || Va.m_uiType == V_OBJECT)
	{
		m_pP = Va.m_pP;
		if (m_pP)
		{
			((SKReference *)m_pP)->IncreRef();
		}
		
	}
	else
	{
		SKMemcpy(m_fVec,Va.m_fVec,sizeof(m_fVec));
	}
	return *this;
}
SKVariant &SKVariant::operator=(bool b)
{
	Clear();
	m_uiType = V_BOOL;
	m_bB = b;
	return *this;
}
SKVariant &SKVariant::operator=(int i)
{
	Clear();
	m_uiType = V_INT;
	m_iI = i;
	return *this;
}
SKVariant &SKVariant::operator=(SKREAL f)
{
	Clear();
	m_uiType = V_REAL;
	m_fVec[0] = f;
	return *this;
}
SKVariant &SKVariant::operator=(const SKVector2 & v)
{
	Clear();
	m_uiType = V_VECTOR2;
	*((SKVector2 *)m_fVec) = v;
	return *this;
}	
SKVariant &SKVariant::operator=(const SKVector3 & v)
{
	Clear();
	m_uiType = V_VECTOR3;
	*((SKVector3 *)m_fVec) = v;
	return *this;
}
SKVariant &SKVariant::operator=(const SKVector3W & v)
{
	Clear();
	m_uiType = V_VECTOR3W;
	*((SKVector3W *)m_fVec) = v;
	return *this;
}
SKVariant &SKVariant::operator=(const SKMatrix3X3 & m)
{
	Clear();
	m_uiType = V_MATRIX3X3;
	*((SKMatrix3X3 *)m_fVec) = m;
	return *this;
}
SKVariant &SKVariant::operator=(const SKMatrix3X3W & m)
{
	Clear();
	m_uiType = V_MATRIX3X3W;
	*((SKMatrix3X3W *)m_fVec) = m;
	return *this;
}
SKVariant &SKVariant::operator=(SKObject * p)
{
	Clear();
	m_uiType = V_OBJECT;
	m_pP = p;
	if (m_pP)
	{
		((SKReference *)m_pP)->IncreRef();
	}
	
	return *this;
}
SKVariant &SKVariant::operator=(const TCHAR * s)
{
	Clear();
	m_uiType = V_STRING;
	m_pP = (void *)SKResourceManager::CreateName(s);
	if (m_pP)
	{
		((SKReference *)m_pP)->IncreRef();
	}
	return *this;
}
unsigned int SKVariant::GetType() const
{
	return m_uiType;
}
bool SKVariant::IsValid()const
{
	return m_uiType != V_NULL;
}
bool SKVariant::GetBool() const
{
	SKMAC_ASSERT(m_uiType == V_BOOL);
	return m_bB;
}
int SKVariant::GetInt() const
{
	SKMAC_ASSERT(m_uiType == V_INT);
	return m_iI;
}
SKREAL SKVariant::GetReal() const
{
	SKMAC_ASSERT(m_uiType == V_REAL);
	return m_fVec[0];
}
const SKVector2& SKVariant::GetVector2()const
{
	SKMAC_ASSERT(m_uiType == V_VECTOR2);
	return *((SKVector2*)m_fVec);
}
const SKVector3& SKVariant::GetVector3() const
{
	SKMAC_ASSERT(m_uiType == V_VECTOR3);
	return *((SKVector3*)m_fVec);
}
const SKVector3W& SKVariant::GetVector3W() const
{
	SKMAC_ASSERT(m_uiType == V_VECTOR3W);
	return *((SKVector3W*)m_fVec);
}
const SKMatrix3X3& SKVariant::GetMatrix3X3() const
{
	SKMAC_ASSERT(m_uiType == V_MATRIX3X3);
	return *((SKMatrix3X3*)m_fVec);
}
const SKMatrix3X3W& SKVariant::GetMatrix3X3W() const
{
	SKMAC_ASSERT(m_uiType == V_MATRIX3X3W);
	return *((SKMatrix3X3W*)m_fVec);
}
const TCHAR* SKVariant::GetString()const
{
	SKMAC_ASSERT(m_uiType == V_STRING);
	return ((SKName *)m_pP)->GetBuffer();
}
SKObject* SKVariant::GetObjectPtr()const
{
	SKMAC_ASSERT(m_uiType == V_OBJECT);
	return (SKObject *)m_pP;
}
bool SKVariant::GetValueToString(SKString& outVal)const
{
	return true;
}
bool SKVariant::SetValueFromString(const SKString& inVal)
{
	return true;
}