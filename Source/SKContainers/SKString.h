#ifndef SKSTRING_H
#define SKSTRING_H
#include "SKContainer.h"
#include "SKMath.h"
namespace SKEngine2
{
class SKDATESTRUCT_API SKString : public SKContainer<TCHAR>
{
public:
	enum
	{
		BUFFER_SIZE = 2048
	};
	SKString();
	SKString(const TCHAR * String);
	SKString(const SKString &String);
	~SKString();
	static SKString ms_StringNULL;
	static TCHAR TempBuffer[BUFFER_SIZE];
	FORCEINLINE TCHAR * GetBuffer()const
	{
		return m_pBuffer;
	}
	FORCEINLINE unsigned int GetLength()const
	{
		return (unsigned int)SKStrLen(m_pBuffer);
	}
	void SetTCHARBufferNum(unsigned int uiLength);
	void Clear();
	//=
	SKString & operator =(const SKString &String);
	SKString & operator =(const TCHAR *String);

	//[]
	TCHAR &operator[](unsigned int i);

	//+=
	const SKString & operator +=(const SKString &String);
	const SKString & operator +=(const TCHAR *String);

	//ȡָ±ǰַߺַ
	bool GetString(const SKString & String,unsigned int uiFind,bool bIsFront = true, bool bIsHaveFind = false);
	//ҵiFindNumγֵַָȻȡǰַߺַiFIndNum = -1ʾһγֵġ
	bool GetString(const SKString & String,TCHAR cFind,int iFIndNum,bool bIsFront = true, bool bIsHaveFind = false);
	//ҵǰַеiFindNumγֵString±꣬-1ʾʧ
	int GetSubStringIndex(const SKString &String, int iFindNum)const;
	//ȥַָ
	bool StripChars(const SKString &String,TCHAR * pStripChars);
	//滻ַָ
	bool ReplaceChars(const SKString &String ,TCHAR * pReplaceChars,TCHAR UseChar);
	//ȥַпո
	void StringLtrim(const SKString &String);
	//ȥַұпո
	void StringRtrim(const SKString &String);

	void Format(const TCHAR * pcString, ...);
protected:
	TCHAR * m_pBuffer;

};
	//+
	SKDATESTRUCT_API SKString operator +(const SKString & String1, const SKString &String2);
	SKDATESTRUCT_API SKString operator +(const SKString &String1, const TCHAR *String2);
	SKDATESTRUCT_API SKString operator +(const TCHAR *String1, const SKString &String2);

	//ڷش0СڷС0ڷ0 
	SKDATESTRUCT_API int CompareString(const SKString &String1,const SKString &String2);
	SKDATESTRUCT_API int CompareString(const SKString &String1,const TCHAR *String2);
	SKDATESTRUCT_API int CompareString(const TCHAR *String1,const SKString &String2);

	//==
	SKDATESTRUCT_API bool operator ==(const SKString &String1,const SKString &String2);
	SKDATESTRUCT_API bool operator ==(const SKString &String1,const TCHAR *String2);
	SKDATESTRUCT_API bool operator ==(const TCHAR *String1,SKString &String2);
	//!=
	SKDATESTRUCT_API bool operator !=(const SKString &String1,const SKString &String2);
	SKDATESTRUCT_API bool operator !=(const SKString &String1,const TCHAR *String2);
	SKDATESTRUCT_API bool operator !=(const TCHAR *String1,const SKString &String2);

	SKDATESTRUCT_API int StringToInt(const SKString & String);
	SKDATESTRUCT_API SKString IntToString(int i);

	SKDATESTRUCT_API unsigned int StringToUInt(const SKString & String);
	SKDATESTRUCT_API SKString UIntToString(unsigned int i);

	SKDATESTRUCT_API bool StringToBool(const SKString & String);
	SKDATESTRUCT_API SKString BoolToString(bool i);
	SKDATESTRUCT_API SKString BoolToString(unsigned int i);

	SKDATESTRUCT_API SKREAL StringToReal(const SKString & String);
	SKDATESTRUCT_API SKString RealToString(SKREAL Value);


	class SKDATESTRUCT_API SKFileName : public SKString
	{
	public:
		SKFileName()
		{

		}
		SKFileName(const TCHAR * String):SKString(String)
		{
		}
		SKFileName(const SKString &String):SKString(String)
		{

		}
		~SKFileName()
		{

		}
		bool GetExtension(SKString & Extension);
		bool GetBaseName(SKString & BaseName);
		bool GetBaseNameAndExtension(SKString & BaseNameAndExtension);
		bool GetPath(SKString & Path);
		bool GetPathAndName(SKString & PathAndName);
	};
}
#endif