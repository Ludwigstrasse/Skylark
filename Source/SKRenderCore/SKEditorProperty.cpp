#include "SKEditorProperty.h"
#include "SKProperty.h"
using namespace SKEngine2;
void SKECheckBox::CallBackValue(bool Value)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Value);
	}
}
void SKELable::CallBackValue(SKString & Str)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Str);
	}
}
void SKECombo::CallBackValue(SKString & Str)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Str);
	}
}
void SKESlider::CallBackValue(unsigned int uiValue)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&uiValue);
	}
}
void SKEText::CallBackValue(SKString & Str)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Str);
	}
}
void SKEColorTable::CallBackValue(SKColorRGBA & Color)
{
	if (m_pOwner)
	{
		m_pOwner->CallBackValue(this,&Color);
	}
}
SKEditorUIPropertyCreator::SKEditorUIPropertyCreator()
{
	m_pCreate = NULL;
}
SKEditorUIPropertyCreator::~SKEditorUIPropertyCreator()
{

}
SKEditorUIPropertyCreator& SKEditorUIPropertyCreator::GetInstance()
{
	static SKEditorUIPropertyCreator EditorUICreator;
	return EditorUICreator;
}

void SKEditorUIPropertyCreator::Set(CreateEditorUIProperty pCreate)
{
	m_pCreate = pCreate;
}
SKEditorSingle * SKEditorUIPropertyCreator::CreateUIProperty(SKEditorSingle::CUSTOM_UI_TYPE type,SKString Name)
{
	return (*m_pCreate)(type,Name);
}
bool SKEditorProperty::CallBackValue(SKEditorSingle * pElem, void * pValue)
{
	if (pElem)
	{
		m_pOwner->ValueChange(pElem->GetName());
		return true;
	}
	return false;
	
}
namespace SKEngine2
{
SKEditorElement * CreateObjectEditorElement(SKObject * pObject,SKString & Name)
{
	SKRtti & Rtti = pObject->GetType();
	SKECollection * pParent = NULL;
	if (Rtti.GetPropertyNum() > 0)
	{
		pParent = (SKECollection *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_COLLECTION,Name);
	}
	for (unsigned int i = 0 ; i < Rtti.GetPropertyNum() ; i++)
	{
		SKProperty * pProperty = Rtti.GetProperty(i);
		if (pProperty->GetFlag() & SKProperty::F_REFLECT_NAME)
		{
			SKString Name = pProperty->GetName().GetString();
			pProperty->AddEditorElement(pObject,pParent,Name);
		}

	}
	return pParent;
}
void  ReleaseObjectEditorElement(SKECollection * Root)
{
	for (unsigned int i = 0; i < Root->ChildElement.GetNum(); i++)
	{
		if (Root->ChildElement[i]->IsCollection())
		{
			ReleaseObjectEditorElement((SKECollection*)Root->ChildElement[i]);
		}
		else
		{
			SKMAC_DELETE(Root->ChildElement[i]);
		}
	}
	SKMAC_DELETE(Root);
}
}