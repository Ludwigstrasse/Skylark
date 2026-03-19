#ifndef SKVARIANT_H
#define SKVARIANT_H
#include "SKMemManager.h"
#include "SKObject.h"
#include "SKVector2.h"
#include "SKVector3.h"
#include "SKVector3W.h"
#include "SKMatrix3X3.h"
#include "SKMatrix3X3W.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKVariant : public SKMemObject
	{
	public:
		enum 
		{
			V_NULL,
			V_BOOL,
			V_INT,
			V_REAL,
			V_VECTOR2,
			V_VECTOR3,
			V_VECTOR3W,
			V_MATRIX3X3,
			V_MATRIX3X3W,
			V_STRING,
			V_OBJECT,
			V_MAX
		};
		SKVariant(unsigned int Type);
		~SKVariant();
		SKVariant(bool b);
		SKVariant(int i);
		SKVariant(SKREAL f);
		SKVariant(const SKVector2 & v);
		SKVariant(const SKVector3 & v);
		SKVariant(const SKVector3W & v);
		SKVariant(const SKMatrix3X3 & m);
		SKVariant(const SKMatrix3X3W & m);
		SKVariant(SKObject * p);
		SKVariant(const TCHAR * s);
		SKVariant(const SKVariant & Va);

		SKVariant &operator=(const SKVariant &Va);
		SKVariant &operator=(bool b);
		SKVariant &operator=(int i);
		SKVariant &operator=(SKREAL f);
		SKVariant &operator=(const SKVector2 & v);
		SKVariant &operator=(const SKVector3 & v);
		SKVariant &operator=(const SKVector3W & v);
		SKVariant &operator=(const SKMatrix3X3 & m);
		SKVariant &operator=(const SKMatrix3X3W & m);
		SKVariant &operator=(SKObject * p);
		SKVariant &operator=(const TCHAR * s);

		bool operator==(const SKVariant& rhs) const;
		bool operator==(bool b)const;
		bool operator==(int i)const;
		bool operator==(SKREAL f)const;
		bool operator==(const SKVector2 & v)const;
		bool operator==(const SKVector3 & v)const;
		bool operator==(const SKVector3W & v)const;
		bool operator==(const SKMatrix3X3 & m)const;
		bool operator==(const SKMatrix3X3W & m)const;
		bool operator==(const SKObject * p)const;
		bool operator==(const TCHAR * s)const;

		unsigned int GetType() const;
		bool IsValid()const;
		bool GetBool() const;
		int GetInt() const;
		SKREAL GetReal() const;
		const SKVector2& GetVector2()const;
		const SKVector3& GetVector3() const;
		const SKVector3W& GetVector3W() const;
		const SKMatrix3X3& GetMatrix3X3() const;
		const SKMatrix3X3W& GetMatrix3X3W() const;
		const TCHAR* GetString()const;
		SKObject* GetObjectPtr()const;

		void Clear();

		bool GetValueToString(SKString& outVal)const;
		bool SetValueFromString(const SKString& inVal);
		static unsigned int MapVariantType(const SKString& inType);
		static SKString MapVariantString(unsigned int inType);
		
	private:
		unsigned int m_uiType;
		union
		{
			bool m_bB;
			int m_iI;
			SKREAL m_fVec[16];
			void * m_pP;
		};
		static TCHAR *  ms_VariantMap[V_MAX];
	};
}
#endif