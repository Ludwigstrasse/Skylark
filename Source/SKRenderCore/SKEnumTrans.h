#ifndef SKENUMTRANS_H
#define SKENUMTRANS_H
#include "SKString.h"
#include "SKMap.h"
#include "SKArray.h"
#include "SKGraphic.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKEnumTrans
	{
	public:
		SKEnumTrans();
		~SKEnumTrans();
		static SKEnumTrans& GetInstance();
		void Add(const TCHAR * EnumName,const TCHAR * Value);
		void Get(const TCHAR * EnumName, SKArray<SKString>& AS);
	protected:
		SKMap<SKString,SKArray<SKString>> m_EnumSaver;
	};
}
#define ADD_ENUM(EnumName,Value) \
	SKEnumTrans::GetInstance().Add(_T(#EnumName),_T(#Value));
#define GET_ENUMARRAY(EnumName,Array) \
	SKEnumTrans::GetInstance().Get(_T(#EnumName),Array);
#endif
