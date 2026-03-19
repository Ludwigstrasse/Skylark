#ifndef SKEDITORPROPERTY_H
#define SKEDITORPROPERTY_H
#include "SKArray.h"
#include "SKObject.h"
#include "SKVector3.h"
#include "SKTransform.h"
#include "SKVector3W.h"
namespace SKEngine2
{
//#define  TYPE_ADD_TO_UI
	template<typename T> struct TIsVSIntType { enum { Value = false }; };
	template<> struct TIsVSIntType<int> { enum { Value = true }; };

	template<typename T> struct TIsVSUintType { enum { Value = false }; };
	template<> struct TIsVSUintType<unsigned int> { enum { Value = true }; };

	template<typename T> struct TIsVSUCharType { enum { Value = false }; };
	template<> struct TIsVSUCharType<unsigned char> { enum { Value = true }; };

	template<typename T> struct TIsVSTCharType { enum { Value = false }; };
	template<> struct TIsVSTCharType<TCHAR> { enum { Value = true }; };

	template<typename T> struct TIsVSRealType { enum { Value = false }; };
	template<> struct TIsVSRealType<float> { enum { Value = true }; };

	class SKGRAPHIC_API SKEditorElement
	{
	public:
		SKEditorElement()
		{
		}
		virtual ~SKEditorElement() = 0
		{

		}
		virtual bool IsCollection()
		{
			return false;
		}
	protected:
	};
	class SKEditorProperty;
	class SKGRAPHIC_API SKEditorSingle : public SKEditorElement
	{
	public:
		enum CUSTOM_UI_TYPE
		{
			CUT_CHECK,
			CUT_LABLE,
			CUT_TEXT,
			CUT_COMBO,
			CUT_SLIDER,
			CUT_COLOR,
			CUT_VIEW,
			CUT_COLLECTION
		};
		SKEditorSingle(SKString & Name)
		{
			m_Name = Name;
			m_pOwner = NULL;
		}
		virtual ~SKEditorSingle() = 0
		{

		}
		void SetOwner(SKEditorProperty * pOwner)
		{
			m_pOwner = pOwner;
		}	
		virtual void SetValue(void * pValue) = 0;
		SKString & GetName()
		{
			return m_Name;
		}
	protected:
		SKEditorProperty *m_pOwner;
		SKString  m_Name;
	};
	class SKGRAPHIC_API SKECheckBox : public SKEditorSingle
	{
	public:
		SKECheckBox(SKString & Name):SKEditorSingle(Name)
		{

		}
		virtual ~SKECheckBox() = 0
		{

		}
		virtual void CallBackValue(bool Value);
		
	};
// 	class QTCheckBox : public SKECheckBox
// 	{
// 	protected:
// 		QTCheckBox(SKString & Name) :SKECheckBox(Name)
// 		{
// 			m_pCheckBox = SK_NEW QCheckBox(Name);
// 			connect(m_pCheckBox, SIGNAL(clicked()), this, SLOT(Slot1()));
// 		}
// 		virtual ~SKECheckBox() = 0
// 		{
// 			SKMAC_DELETE(m_pCheckBox);
// 		}
// 		void Slot1()
// 		{
// 			CallBackValue(m_pCheckBox->GetChecked());
// 		}
// 		virtual void SetValue(void * pValue)
// 		{
// 			m_pCheckBox->SetChecked(*((bool *)pValue))
// 		}
// 		QCheckBox * m_pCheckBox;
// 	}

	class SKGRAPHIC_API SKELable : public SKEditorSingle
	{
	public:
		SKELable(SKString & Name):SKEditorSingle(Name)
		{

		}
		virtual ~SKELable() = 0
		{

		}
		virtual void CallBackValue(SKString & Str);
		
	};
	class SKGRAPHIC_API SKECombo : public SKEditorSingle 
	{
	public:
		SKECombo(SKString & Name):SKEditorSingle(Name)
		{

		}
		virtual ~SKECombo() = 0
		{

		}
		virtual void AddOption(SKString & String) = 0;

		virtual void CallBackValue(SKString & Str);
		virtual void AddOption(SKArray<SKString> & VS) = 0;
	};
	class SKGRAPHIC_API SKESlider : public SKEditorSingle // unsigned int
	{
	public:
		SKESlider(SKString & Name):SKEditorSingle(Name)
		{

		}
		virtual ~SKESlider() = 0
		{

		}
		virtual void SetRange(unsigned int uiMin,unsigned int uiMax,unsigned int uiStep) = 0
		{
			m_uiMax = uiMax;
			m_uiMin = uiMin;
			m_uiStep = uiStep;
		}
		unsigned int m_uiMax;
		unsigned int m_uiMin;
		unsigned int m_uiStep;
		virtual void CallBackValue(unsigned int uiValue);
	};
	class SKGRAPHIC_API SKEViewWindow : public SKEditorSingle
	{
	
	};
	class SKGRAPHIC_API SKEColorTable : public SKEditorSingle
	{
	public:
		SKEColorTable(SKString & Name):SKEditorSingle(Name)
		{

		}
		virtual ~SKEColorTable()
		{

		}
		virtual void CallBackValue(SKColorRGBA& Value);
	};
	class SKGRAPHIC_API SKEText : public SKEditorSingle // SKREAL
	{
	public:
		SKEText(SKString & Name):SKEditorSingle(Name)
		{

		}
		virtual ~SKEText()
		{

		}
		virtual void CallBackValue(SKString & Str);
	};
	
	class SKGRAPHIC_API SKECollection : public SKEditorSingle
	{
	public:
		SKECollection(SKString & Name):SKEditorSingle(Name)
		{
		}
		~SKECollection()
		{

		}
		virtual void AddElement(SKEditorElement * pElement)
		{
			ChildElement.AddElement(pElement);
		}
		SKArray<SKEditorElement *> ChildElement;
		virtual bool IsCollection()
		{
			return true;
		}
	};

	typedef SKEditorSingle* (*CreateEditorUIProperty)(SKEditorSingle::CUSTOM_UI_TYPE type,SKString Name);
	class SKGRAPHIC_API SKEditorUIPropertyCreator
	{
	public:
		SKEditorUIPropertyCreator();
		~SKEditorUIPropertyCreator();
		static SKEditorUIPropertyCreator& GetInstance();

		void Set(CreateEditorUIProperty pCreate);
		SKEditorSingle * CreateUIProperty(SKEditorSingle::CUSTOM_UI_TYPE type,SKString Name);
	protected:
		CreateEditorUIProperty m_pCreate;
	};
// 	SKEditorSingle* CreateEditorUIProperty(SKEditorSingle::CUSTOM_UI_TYPE type, SKString Name)
// 	{
// 		if (type == SKEditorSingle::CUSTOM_UI_TYPE::CUT_CHECK)
// 		{
// 			return SK_NEW QTCheckBox(Name);
// 		}
// 		else if (type == SKEditorSingle::CUSTOM_UI_TYPE::CUT_TEXT)
// 		{
// 			return SK_NEW QTText(Name);
// 		}
// 		else if (type == SKEditorSingle::CUSTOM_UI_TYPE::CUT_COLOR)
// 		{
// 			return SK_NEW QTColor(Name);
// 		}
// 		.........................................................................
// 	}
	// you must set this macro
#define SETCreateEditorUIProperty(F) SKEditorUIPropertyCreator::GetInstance().Set(F);
#define  CREATE_UI_PROPERTY(type,Name) SKEditorUIPropertyCreator::GetInstance().CreateUIProperty(type,Name);

	class SKGRAPHIC_API SKEditorProperty : public SKEditorElement
	{
	public:
		SKEditorProperty(SKString & Name,SKObject * pOwner)
		{
			m_pName = (SKELable  *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,Name);
			m_pName->SetOwner(this);
			m_pName->SetValue((void*)&Name);
			m_pOwner = pOwner;
		}
		virtual ~SKEditorProperty()
		{
			SKMAC_DELETE(m_pName);
			m_pOwner = NULL;
		}
		virtual void SetValue() = 0;
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue);
	protected:
		SKELable * m_pName;
		SKObject * m_pOwner;
	};
	
	

	class SKGRAPHIC_API SKEBoolProperty : public SKEditorProperty
	{
	public:
		SKEBoolProperty(bool * b,SKString & Name,SKObject * pOwner)
			:SKEditorProperty(Name,pOwner)
		{
			m_pCheckBox = (SKECheckBox *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_CHECK,Name);
			m_pb = b;
			m_pCheckBox->SetOwner(this);
			SetValue();
		}
		virtual void SetValue()
		{		
			m_pCheckBox->SetValue((void *)m_pb);
		}
		virtual ~SKEBoolProperty()
		{
			SKMAC_DELETE(m_pCheckBox);
		}
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pCheckBox)
			{
				(*m_pb) = *((bool *)pValue);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			
			return false;
		}
	protected:
		SKECheckBox * m_pCheckBox;
		bool *m_pb;
	};

	class SKGRAPHIC_API SKEResourceProperty : public SKEditorProperty
	{
	public: 
		SKEResourceProperty(SKResource * & pResource,SKString & Name,SKObject * pOwner)
			:SKEditorProperty(Name,pOwner),m_pResource(pResource)
		{
			m_pView = (SKEViewWindow  *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_VIEW,Name);
			m_pView->SetOwner(this);
			SetValue();
		}
		virtual void SetValue()
		{
			m_pView->SetValue((void *)m_pResource);
		}
		virtual ~SKEResourceProperty()
		{
			SKMAC_DELETE(m_pView);
		}
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pView)
			{
				m_pResource = *((SKResource * *)pValue);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}	
			return false;
		}
	protected:
		SKEViewWindow * m_pView;
		SKResource * & m_pResource;
	};

	class SKGRAPHIC_API SKEColorProperty : public SKEditorProperty
	{
	public:
		SKEColorProperty(SKColorRGBA * pColor,SKString & Name,SKObject * pOwner)
			:SKEditorProperty(Name,pOwner)
		{
			m_pColorTable = (SKEColorTable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_COLOR,Name);
			m_pColorTable->SetOwner(this);
			m_pColor = pColor;
			SetValue();
		}
		virtual void SetValue()
		{
			m_pColorTable->SetValue((void *)m_pColor);
		}
		virtual ~SKEColorProperty()
		{
			SKMAC_DELETE(m_pColorTable);
		}
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pColorTable)
			{
				*m_pColor = *((SKColorRGBA *)pValue);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}		
			return false;
		}
	protected:
		SKEColorTable * m_pColorTable;
		SKColorRGBA * m_pColor;
	};

	class SKGRAPHIC_API SKEEnumProperty : public SKEditorProperty
	{
	public:
		SKEEnumProperty(unsigned int * pData,SKString & Name,SKObject * pOwner)
			:SKEditorProperty(Name,pOwner)
		{
			m_pData = pData;
			m_pCombo = (SKECombo *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_COMBO,Name);
			m_pCombo->SetOwner(this);
			SetValue();
		}
		virtual void SetValue()
		{
			m_pCombo->SetValue((void *)m_pData);
		}
		virtual ~SKEEnumProperty()
		{
			SKMAC_DELETE(m_pCombo);
		}
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pCombo)
			{
				*m_pData = *((unsigned int *)pValue);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			
			return false;
		}
		void AddEnumString(SKArray<SKString>& AS)
		{
			m_pCombo->AddOption(AS);
		}
	protected:
		SKECombo * m_pCombo;
		unsigned int *m_pData;
	};
	template<typename T>
	class SKGRAPHIC_API SKEValueProperty : public SKEditorProperty
	{
	public:
		SKEValueProperty(T * pData,SKString & Name, SKObject * pOwner, bool bRange, T& Max, T& Min, T& Step)
			:SKEditorProperty(Name,pOwner)
		{
			SKMAC_ASSERT(pData != NULL);
			m_pText = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name);
			m_pText->SetOwner(this);
			m_fStep = Step;
			if (bRange)
			{
				if (Min > Max)
				{
					Swap(Max, Min);
				}
				SKMAC_ASSERT(Max - Min > Step);
				m_pSlider = (SKESlider *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_SLIDER,Name);
				m_pSlider->SetOwner(this);
				m_pSlider->SetRange(0, int((Max - Min)/Step),1);
			}
			m_Min = Min;
			m_Max = Max;
			m_pData = pData;
#ifdef TYPE_ADD_TO_UI
			SetValue();
#endif
		}
		virtual void GetValueString(SKString& IntString)
		{
#ifdef TYPE_ADD_TO_UI
			if (TIsVSIntType<T>::Value)
			{
				IntString = IntToString(*m_pData);
			}
			else if (TIsVSUintType<T>::Value)
			{
				IntString = IntToString(*m_pData);
			}
			else if (TIsVSUCharType<T>::Value)
			{
				IntString = IntToString(*m_pData);
			}
			else if (TIsVSTCharType<T>::Value)
			{
				TCHAR tData[2];
				tData[0] = *m_pData;
				tData[1] = _T('\0');
				SKString Temp(tData);
				IntString = Temp;
			}
			else if (TIsVSRealType<T>::Value)
			{
				IntString = RealToString(*m_pData);
			}
#endif
		}
		virtual T GetStringValue(SKString& IntString)
		{
#ifdef TYPE_ADD_TO_UI
			if (TIsVSIntType<T>::Value)
			{
				return (T)StringToInt(IntString);
			}
			else if (TIsVSUintType<T>::Value)
			{
				return (T)StringToInt(IntString);
			}
			else if (TIsVSUCharType<T>::Value)
			{
				return (T)StringToInt(IntString);
			}
			else if (TIsVSTCharType<T>::Value)
			{
				return (T)*IntString.GetBuffer();
			}
			else if (TIsVSRealType<T>::Value)
			{
				return (T)StringToReal(IntString);
			}
#endif
			return T();
		}
		virtual void SetValue()
		{
			SKString IntString;
			GetValueString(IntString);
			m_pText->SetValue((void *)&IntString);
			if (m_pSlider)
			{
				T Value = (*m_pData - m_Min) / m_fStep;
				m_pSlider->SetValue((void *)&Value);
			}
		}
		virtual ~SKEValueProperty()
		{
			SKMAC_DELETE(m_pSlider);
			SKMAC_DELETE(m_pText);
		}
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pText)
			{
				SKString Data = *(SKString *)pValue;
				T Value = GetStringValue(Data);
				if (m_pSlider)
				{
					if (Value < m_Min)
					{
						Value = m_Min;
					}
					else if (Value > m_Max)
					{
						Value = m_Max;
					}
					*m_pData = Value;
				}
				else
				{
					*m_pData = Value;
				}
				SetValue();
				SKEValueProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (m_pSlider && pElem == m_pSlider)
			{
				int Value = *(int *)pValue;
				*m_pData = ((T)(Value * m_fStep)) + m_Min;
				SetValue();
				SKEValueProperty::CallBackValue(pElem, pValue);
			}

			return false;
		}
public:
		SKESlider * m_pSlider;	
		SKEText	  * m_pText; 
		T m_fStep;
		T m_Max;
		T m_Min;
		T * m_pData;
	};
#ifndef TYPE_ADD_TO_UI
	class SKGRAPHIC_API SKEIntProperty : public SKEValueProperty<int>
	{
	public:
		SKEIntProperty(int * pData,SKString & Name,SKObject * pOwner,bool bRange,int Max,int Min,int Step)
			:SKEValueProperty(pData,Name, pOwner, bRange,Max,Min,Step)
		{
			SetValue();
		}
		virtual ~SKEIntProperty()
		{

		}
		virtual void GetValueString(SKString& IntString)
		{
			IntString = IntToString(*m_pData);
		}
		virtual int GetStringValue(SKString& IntString)
		{
			return StringToInt(IntString);
		}
	};
	class SKGRAPHIC_API SKEUnsignedIntProperty : public SKEValueProperty<unsigned int>
	{
	public:
		SKEUnsignedIntProperty(unsigned int * pData, SKString & Name, SKObject * pOwner, bool bRange, unsigned int Max, unsigned int Min, unsigned int Step)
			:SKEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();		
		}
		virtual ~SKEUnsignedIntProperty()
		{
		}
		virtual void GetValueString(SKString& IntString)
		{
			IntString = IntToString(*m_pData);
		}
		virtual unsigned int GetStringValue(SKString& IntString)
		{
			return StringToInt(IntString);
		}
	};
	class SKGRAPHIC_API SKEUnsignedCharProperty : public SKEValueProperty<unsigned char>
	{
	public:
		SKEUnsignedCharProperty(unsigned char * pData, SKString & Name, SKObject * pOwner, bool bRange, unsigned char Max, unsigned char Min, unsigned char Step)
			:SKEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();
		}
		virtual ~SKEUnsignedCharProperty()
		{
		}
		virtual void GetValueString(SKString& IntString)
		{
			IntString = IntToString(*m_pData);
		}
		virtual unsigned char GetStringValue(SKString& IntString)
		{
			return StringToInt(IntString);
		}
	};
	class SKGRAPHIC_API SKECharProperty : public SKEValueProperty<TCHAR>
	{
	public:
		SKECharProperty(TCHAR * pData, SKString & Name, SKObject * pOwner, bool bRange, TCHAR Max, TCHAR Min, TCHAR Step)
			:SKEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();
		}
		virtual void GetValueString(SKString& IntString)
		{
			TCHAR tData[2];
			tData[0] = *m_pData;
			tData[1] = _T('\0');
			SKString Temp(tData);
			IntString = Temp;
		}
		virtual TCHAR GetStringValue(SKString& IntString)
		{
			return *IntString.GetBuffer();
		}
		virtual ~SKECharProperty()
		{

		}
	};
	class SKGRAPHIC_API SKERealProperty : public SKEValueProperty<SKREAL>
	{
	public:
		SKERealProperty(SKREAL * pData,SKString & Name,SKObject * pOwner,bool bRange,SKREAL Max,SKREAL Min,SKREAL Step)
			:SKEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();
		}
		virtual ~SKERealProperty()
		{

		}
		virtual void GetValueString(SKString& IntString)
		{
			IntString = RealToString(*m_pData);
		}
		virtual SKREAL GetStringValue(SKString& IntString)
		{
			return StringToReal(IntString);
		}
	};
#endif
	class SKGRAPHIC_API SKEVector3Property : public SKEditorProperty
	{
	public:
		SKEVector3Property(SKVector3 * pVector3, SKString & Name, SKObject * pOwner, bool bRange, SKVector3 Max, SKVector3 Min, SKVector3 Step)
			:SKEditorProperty(Name,pOwner)
		{
			SKMAC_ASSERT(pVector3 != NULL);

			if (bRange)
			{
				if (Min.x > Max.x)
				{
					Swap(Max.x, Min.x);
				}
				if (Min.y > Max.y)
				{
					Swap(Max.y, Min.y);
				}
				if (Min.z > Max.z)
				{
					Swap(Max.z, Min.z);
				}
				SKMAC_ASSERT(Max.x - Min.x > Step.x);
				SKMAC_ASSERT(Max.y - Min.y > Step.y);
				SKMAC_ASSERT(Max.z - Min.z > Step.z);
			}
			m_pVector3 = pVector3;
			m_fStep = Step;
			m_Min = Min;
			m_Max = Max;

			m_pNameX = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"X");
			m_pNameY = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Y");
			m_pNameZ = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Z");

			m_pTextX = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "X");
			m_pTextX->SetOwner(this);

			m_pTextY = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "Y");
			m_pTextY->SetOwner(this);

			m_pTextZ = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "Z");
			m_pTextZ->SetOwner(this);

			if (bRange)
			{
				m_pSliderX = (SKESlider *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_SLIDER,Name + "X");
				m_pSliderX->SetOwner(this);
				m_pSliderX->SetRange(0, int((Max.x - Min.x) / Step.x), 1);

				m_pSliderZ = (SKESlider *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_SLIDER,Name + "Z");
				m_pSliderZ->SetOwner(this);
				m_pSliderZ->SetRange(0, int((Max.z - Min.z) / Step.z), 1);

				m_pSliderY = (SKESlider *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_SLIDER,Name + "Y");
				m_pSliderY->SetOwner(this);
				m_pSliderY->SetRange(0, int((Max.y - Min.y) / Step.y), 1);
			}
			SetValue();
			
		}
		virtual void SetValue()
		{
			SKREAL fDataY = m_pVector3->y;
			SKString RealStringY = RealToString(fDataY);
			m_pTextY->SetValue((void *)&RealStringY);

			SKREAL fDataX = m_pVector3->x;
			SKString RealStringX = RealToString(fDataX);
			m_pTextX->SetValue((void *)&RealStringX);

			SKREAL fDataZ = m_pVector3->z;
			SKString RealStringZ = RealToString(fDataZ);
			m_pTextZ->SetValue((void *)&RealStringZ);
		}
		virtual ~SKEVector3Property()
		{
			SKMAC_DELETE(m_pTextX);
			SKMAC_DELETE(m_pTextY);
			SKMAC_DELETE(m_pTextZ);

			SKMAC_DELETE(m_pSliderX);
			SKMAC_DELETE(m_pSliderY);
			SKMAC_DELETE(m_pSliderZ);
			
			SKMAC_DELETE(m_pNameX);
			SKMAC_DELETE(m_pNameY);
			SKMAC_DELETE(m_pNameZ);

		}	
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue)
		{

			if (pElem == m_pTextX)
			{
				SKString Data = *(SKString *)pValue;
				SKREAL Value = StringToReal(Data);

				if (m_pSliderX)
				{
					if (Value < m_Min.x)
					{
						Value = m_Min.x;
					}
					else if (Value > m_Max.x)
					{
						Value = m_Max.x;
					}
					m_pVector3->x = Value;
				}
				else
				{
					m_pVector3->x= Value;
				}
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTextY)
			{
				SKString Data = *(SKString *)pValue;
				SKREAL Value = StringToReal(Data);

				if (m_pSliderY)
				{
					if (Value < m_Min.y)
					{
						Value = m_Min.y;
					}
					else if (Value > m_Max.y)
					{
						Value = m_Max.y;
					}
					m_pVector3->y = Value;
				}
				else
				{
					m_pVector3->y= Value;
				}
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTextZ)
			{
				SKString Data = *(SKString *)pValue;
				SKREAL Value = StringToReal(Data);

				if (m_pSliderZ)
				{
					if (Value < m_Min.z)
					{
						Value = m_Min.z;
					}
					else if (Value > m_Max.z)
					{
						Value = m_Max.z;
					}
					m_pVector3->z = Value;
				}
				else
				{
					m_pVector3->z= Value;
				}
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (m_pSliderX && pElem == m_pSliderX)
			{
				int Value = *(int *)pValue;
				m_pVector3->x = ((SKREAL)(Value * m_fStep.x)) + m_Min.x;
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
			}
			else if (m_pSliderY && pElem == m_pSliderY)
			{
				int Value = *(int *)pValue;
				m_pVector3->y = ((SKREAL)(Value * m_fStep.y)) + m_Min.y;
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
			}
			else if (m_pSliderZ && pElem == m_pSliderZ)
			{
				int Value = *(int *)pValue;
				m_pVector3->z = ((SKREAL)(Value * m_fStep.z)) + m_Min.z;
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
			}
			return false;
		}
	protected:

		SKVector3 m_fStep;
		SKELable * m_pNameX;
		SKESlider * m_pSliderX;	
		SKEText	  * m_pTextX;

		SKELable * m_pNameY;
		SKESlider * m_pSliderY;	
		SKEText	  * m_pTextY;

		SKELable * m_pNameZ;
		SKESlider * m_pSliderZ;	
		SKEText	  * m_pTextZ;

		SKVector3 * m_pVector3;
		SKVector3 m_Max;
		SKVector3 m_Min;
	};
	class SKGRAPHIC_API SKETransformProperty : public SKEditorProperty // no range so no slider
	{
	public:
		SKETransformProperty(SKTransform * pTransform,SKString & Name,SKObject * pOwner)
			:SKEditorProperty(Name,pOwner)
		{
			m_pTransform = pTransform;

			m_pTranslateName = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Translate");
			m_pRotationName = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Rotation");
			m_pScaleName = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Scale");
			
			m_pTranslateNameX = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"X");
			m_pTranslateNameY = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Y");
			m_pTranslateNameZ = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Z");

			m_pRotationNameX = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"X");
			m_pRotationNameY = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Y");
			m_pRotationNameZ = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Z");

			m_pScaleNameX = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"X");
			m_pScaleNameY = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Y");
			m_pScaleNameZ = (SKELable *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_LABLE,"Z");

			m_pTranslateTextX = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "TranslateX");
			m_pTranslateTextX->SetOwner(this);

			m_pTranslateTextY = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "TranslateY");
			m_pTranslateTextY->SetOwner(this);

			m_pTranslateTextZ = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "TranslateZ");
			m_pTranslateTextZ->SetOwner(this);

			m_pRotationTextX = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "RotationX");
			m_pRotationTextX->SetOwner(this);

			m_pRotationTextZ = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "RotationY");
			m_pRotationTextZ->SetOwner(this);

			m_pRotationTextY = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "RotationZ");
			m_pRotationTextY->SetOwner(this);

			m_pScaleTextX = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "ScaleX");
			m_pScaleTextX->SetOwner(this);

			m_pScaleTextY = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "ScaleX");
			m_pScaleTextY->SetOwner(this);

			m_pScaleTextZ = (SKEText *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_TEXT,Name + "ScaleX");
			m_pScaleTextZ->SetOwner(this);
			SetValue();
			
		}
		virtual void SetValue()
		{
			SKVector3 Tran = m_pTransform->GetTranslate();
			SKREAL fTranDataX = Tran.x;
			SKString RealStringTranX = RealToString(fTranDataX);
			m_pTranslateTextX->SetValue((void *)&RealStringTranX);

			SKREAL fTranDataY = Tran.y;
			SKString RealStringTranY = RealToString(fTranDataY);
			m_pTranslateTextY->SetValue((void *)&RealStringTranY);

			SKREAL fTranDataZ = Tran.z;
			SKString RealStringTranZ = RealToString(fTranDataZ);
			m_pTranslateTextZ->SetValue((void *)&RealStringTranZ);

			SKMatrix3X3 Mat = m_pTransform->GetRotate();
			SKREAL X,Y,Z;
			Mat.GetEluer(Z, X, Y);;

			SKString RealStringRotateX = RealToString(X);
			m_pRotationTextX->SetValue((void *)&RealStringRotateX);


			SKString RealStringRotateY = RealToString(Y);
			m_pRotationTextY->SetValue((void *)&RealStringRotateY);

			SKString RealStringRotateZ = RealToString(Z);
			m_pRotationTextZ->SetValue((void *)&RealStringRotateZ);

			SKVector3 Scale = m_pTransform->GetScale();

			SKREAL fScaleDataX = Scale.x;
			SKString RealStringScaleX = RealToString(fScaleDataX);
			m_pScaleTextX->SetValue((void *)&RealStringScaleX);

			SKREAL fScaleDataY = Scale.y;
			SKString RealStringScaleY = RealToString(fScaleDataY);
			m_pScaleTextY->SetValue((void *)&RealStringScaleY);

			SKREAL fScaleDataZ = Scale.z;
			SKString RealStringScaleZ = RealToString(fScaleDataZ);
			m_pScaleTextZ->SetValue((void *)&RealStringScaleZ);
		}
		virtual ~SKETransformProperty()
		{
			SKMAC_DELETE(m_pTranslateTextX);
			SKMAC_DELETE(m_pTranslateTextY);
			SKMAC_DELETE(m_pTranslateTextZ);

			SKMAC_DELETE(m_pRotationTextX);
			SKMAC_DELETE(m_pRotationTextY);
			SKMAC_DELETE(m_pRotationTextZ);

			SKMAC_DELETE(m_pScaleTextX);
			SKMAC_DELETE(m_pScaleTextY);
			SKMAC_DELETE(m_pScaleTextZ);

			SKMAC_DELETE(m_pTranslateName);
			SKMAC_DELETE(m_pRotationName);
			SKMAC_DELETE(m_pScaleName);

			SKMAC_DELETE(m_pTranslateNameX);
			SKMAC_DELETE(m_pTranslateNameY);
			SKMAC_DELETE(m_pTranslateNameZ);

			SKMAC_DELETE(m_pRotationNameX);
			SKMAC_DELETE(m_pRotationNameY);
			SKMAC_DELETE(m_pRotationNameZ);

			SKMAC_DELETE(m_pScaleNameX);
			SKMAC_DELETE(m_pScaleNameY);
			SKMAC_DELETE(m_pScaleNameZ);
		}
		virtual bool CallBackValue(SKEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pTranslateTextX)
			{
				SKVector3 Tran = m_pTransform->GetTranslate();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKVector3 NewTran(fData,Tran.y,Tran.z);
				m_pTransform->SetTranslate(NewTran);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTranslateTextY)
			{
				SKVector3 Tran = m_pTransform->GetTranslate();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKVector3 NewTran(Tran.x,fData,Tran.z);
				m_pTransform->SetTranslate(NewTran);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTranslateTextZ)
			{
				SKVector3 Tran = m_pTransform->GetTranslate();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKVector3 NewTran(Tran.x,Tran.y,fData);
				m_pTransform->SetTranslate(NewTran);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pRotationTextX)
			{
				SKMatrix3X3 Mat = m_pTransform->GetRotate();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKREAL X,Y,Z;
				Mat.GetEluer(Z, X, Y);
				Mat.CreateEluer(Z,fData, Y );
				m_pTransform->SetRotate(Mat);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pRotationTextY)
			{
				SKMatrix3X3 Mat = m_pTransform->GetRotate();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKREAL X,Y,Z;
				Mat.GetEluer(Z, X, Y);
				Mat.CreateEluer( Z,X, fData);
				m_pTransform->SetRotate(Mat);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pRotationTextZ)
			{
				SKMatrix3X3 Mat = m_pTransform->GetRotate();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKREAL X,Y,Z;
				Mat.GetEluer(Z,X,Y);
				Mat.CreateEluer(fData, X, Y);
				m_pTransform->SetRotate(Mat);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pScaleTextX)
			{
				SKVector3 Scale = m_pTransform->GetScale();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKVector3 NewScale(fData,Scale.y,Scale.z);
				m_pTransform->SetScale(NewScale);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pScaleTextY)
			{
				SKVector3 Scale = m_pTransform->GetScale();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKVector3 NewScale(Scale.x,fData,Scale.z);
				m_pTransform->SetScale(NewScale);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pScaleTextZ)
			{
				SKVector3 Scale = m_pTransform->GetScale();
				SKString Data = *(SKString *)pValue;
				SKREAL fData = StringToReal(Data);
				SKVector3 NewScale(Scale.x,Scale.y,fData);
				m_pTransform->SetScale(NewScale);
				SetValue();
				SKEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			
			return false;
		}
	protected:
		SKTransform * m_pTransform;
		SKELable * m_pTranslateName;

		SKELable * m_pTranslateNameX;
		SKEText	  * m_pTranslateTextX;

		SKELable * m_pTranslateNameY;
		SKEText	  * m_pTranslateTextY;

		SKELable * m_pTranslateNameZ;	
		SKEText	  * m_pTranslateTextZ;

		SKELable * m_pRotationName;

		SKELable * m_pRotationNameX;
		SKEText	  * m_pRotationTextX;

		SKELable * m_pRotationNameY;
		SKEText	  * m_pRotationTextY;

		SKELable * m_pRotationNameZ;	
		SKEText	  * m_pRotationTextZ;

		SKELable * m_pScaleName;

		SKELable * m_pScaleNameX;
		SKEText	  * m_pScaleTextX;

		SKELable * m_pScaleNameY;
		SKEText	  * m_pScaleTextY;

		SKELable * m_pScaleNameZ;	
		SKEText	  * m_pScaleTextZ;
	};
	class SKGRAPHIC_API SKENoDefineProperty : public SKEditorProperty
	{
	public:
		SKENoDefineProperty()
			:SKEditorProperty(SKString::ms_StringNULL,NULL)
		{

		}
		virtual ~SKENoDefineProperty()
		{

		}
	};
	template<typename T>
	SKEditorElement *  CreateEElement(T& Value,SKString & Name,SKObject * pOwner,bool bRange,T & Max,T & Min,T& fStep)
	{
		SKMAC_ASSERT(0);
		return NULL;
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<SKVector3>(SKVector3& Value, SKString & Name, SKObject * pOwner, bool bRange, SKVector3 & Max, SKVector3 & Min, SKVector3& fStep)
	{
		return SK_NEW SKEVector3Property(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<bool>(bool& Value, SKString & Name, SKObject * pOwner, bool bRange, bool & Max, bool & Min, bool& fStep)
	{
		return SK_NEW SKEBoolProperty(&Value,Name,pOwner);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<SKTransform>(SKTransform& Value, SKString & Name, SKObject * pOwner, bool bRange, SKTransform & Max, SKTransform & Min, SKTransform& fStep)
	{
		return SK_NEW SKETransformProperty(&Value,Name,pOwner);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<SKColorRGBA>(SKColorRGBA& Value, SKString & Name, SKObject * pOwner, bool bRange, SKColorRGBA & Max, SKColorRGBA & Min, SKColorRGBA& fStep)
	{
		return SK_NEW SKEColorProperty(&Value,Name,pOwner);
	}
#ifdef TYPE_ADD_TO_UI
	template<> FORCEINLINE SKEditorElement * CreateEElement<unsigned int>(unsigned int& Value, SKString & Name, SKObject * pOwner, bool bRange, unsigned int & Max, unsigned int & Min, unsigned int& fStep)
	{
		return SK_NEW SKValueProperty<unsigned int>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<int>(int& Value, SKString & Name, SKObject * pOwner, bool bRange, int & Max, int & Min, int& fStep)
	{
		return SK_NEW SKValueProperty<int>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<unsigned char>(unsigned char& Value, SKString & Name, SKObject * pOwner, bool bRange, unsigned char & Max, unsigned char & Min, unsigned char& fStep)
	{
		return SK_NEW SKValueProperty<unsigned char>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<TCHAR>(TCHAR& Value, SKString & Name, SKObject * pOwner, bool bRange, TCHAR & Max, TCHAR & Min, TCHAR & fStep)
	{
		return SK_NEW SKValueProperty<TCHAR>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<SKREAL>(SKREAL& Value, SKString & Name, SKObject * pOwner, bool bRange, SKREAL & Max, SKREAL & Min, SKREAL & fStep)
	{
		return SK_NEW SKValueProperty<SKREAL>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
#else
	template<> FORCEINLINE SKEditorElement * CreateEElement<unsigned int>(unsigned int& Value, SKString & Name, SKObject * pOwner, bool bRange, unsigned int & Max, unsigned int & Min, unsigned int& fStep)
	{
		return SK_NEW SKEUnsignedIntProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<int>(int& Value, SKString & Name, SKObject * pOwner, bool bRange, int & Max, int & Min, int& fStep)
	{
		return SK_NEW SKEIntProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<unsigned char>(unsigned char& Value, SKString & Name, SKObject * pOwner, bool bRange, unsigned char & Max, unsigned char & Min, unsigned char& fStep)
	{
		return SK_NEW SKEUnsignedCharProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<TCHAR>(TCHAR& Value, SKString & Name, SKObject * pOwner, bool bRange, TCHAR & Max, TCHAR & Min, TCHAR & fStep)
	{
		return SK_NEW SKECharProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> FORCEINLINE SKEditorElement * CreateEElement<SKREAL>(SKREAL& Value,SKString & Name,SKObject * pOwner,bool bRange,SKREAL & Max,SKREAL & Min,SKREAL & fStep)
	{
		return SK_NEW SKERealProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
#endif
	/*to else
	---------------------------------------------------
	*/
	SKGRAPHIC_API SKEditorElement * CreateObjectEditorElement(SKObject * pObject, SKString & Name);
	SKGRAPHIC_API void  ReleaseObjectEditorElement(SKECollection * Root);
	template<typename T>
	void CreateEditorElement(T & Value, SKObject * pOwner, SKECollection * pParent, SKString& Name, bool Range = false, T  Max = T(), T  Min = T(), T fStep = T())
	{
		if (TIsVSResourceType<T>::Value)
		{
			SKResource * & Temp = *(SKResource **)(void *)&Value;	
			SKEResourceProperty * pEp = SK_NEW SKEResourceProperty(Temp, Name, pOwner);
			pParent->AddElement(pEp);
		}
		else if(TIsVSPointerType<T>::Value)
		{
			SKObject* & TempSrc = *(SKObject**)(void *)&Value;	
			SKEditorElement * pEp = CreateObjectEditorElement(TempSrc,Name);
			pParent->AddElement(pEp);

		}
		else if (TIsVSType<T>::Value)
		{
			SKObject* TempSrc = (SKObject *)&Value;	
			SKEditorElement * pEp = CreateObjectEditorElement(TempSrc,Name);
			pParent->AddElement(pEp);
		}
		else if(TIsVSSmartPointerType<T>::Value)
		{
			SKObjectPtr & TempSrc = *(SKObjectPtr*)(void *)&Value;
			SKEditorElement * pEp = CreateObjectEditorElement(TempSrc,Name);
			pParent->AddElement(pEp);
		}
		else if (TIsCustomType<T>::Value)
		{
			SKCustomArchiveObject * TempSrc = (SKCustomArchiveObject*)(void *)&Value;
			SKEditorElement * pEp = TempSrc->CreateEElement(Name, pOwner);
			pParent->AddElement(pEp);
		}
		else if (TIsVSEnumType<T>::Value)
		{
			
		}
		else 
		{
			SKEditorElement * pEp = CreateEElement(Value, Name, pOwner,Range, Max, Min, fStep);
			pParent->AddElement(pEp);
		}
	}
	template<typename T,SKMemManagerFun MMFun>
	void CreateEditorElement(SKArray<T, MMFun> & Value, SKObject * pOwner, SKECollection * pParent, SKString& Name, bool Range = false, T  Max = T(), T  Min = T(), T fStep = T())
	{
		SKECollection * pEc = NULL;
		if (Value.GetNum() > 0)
		{
			pEc = (SKECollection *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_COLLECTION,Name);
			pParent->AddElement(pEc);
		}
		for (unsigned int i = 0 ; i < Value.GetNum() ;i++)
		{
			SKString NewName = Name + IntToString(i);
			CreateEditorElement(Value[i], pOwner, pEc, NewName, Range, Max, Min, fStep);
		}
	}

	template<class Key,class T,SKMemManagerFun MMFun>
	void CreateEditorElement(SKMap<Key, T, MMFun> & Value, SKObject * pOwner, SKECollection * pParent, SKString& Name, bool Range = false, T  Max = T(), T  Min = T(), T fStep = T())
	{
		SKECollection * pEc = NULL;
		if (Value.GetNum() > 0)
		{
			pEc = (SKECollection *)CREATE_UI_PROPERTY(SKEditorSingle::CUT_COLLECTION,Name);
			pParent->AddElement(pEc);
		}
		for (unsigned int i = 0 ; i < Value.GetNum() ;i++)
		{
			SKString NewNameKey = Name + _T(" Key");
			SKString NewNameValue = Name + _T(" Value");
			MapElement<Key,T> &ElementDest = Value[i];
			CreateEditorElement(ElementDest.Key,pOwner,pEc,NewNameKey);
			CreateEditorElement(ElementDest.Value, pOwner, pEc, NewNameValue, Range, Max, Min, fStep);
		}
	}

}
#endif