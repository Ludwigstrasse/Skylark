#include "SKString.h"
using namespace SKEngine2;
SKString SKString::ms_StringNULL;
TCHAR	SKString::TempBuffer[BUFFER_SIZE] = {0};
SKString::SKString()
{
	m_pBuffer = NULL;
	m_pBuffer= SK_NEW TCHAR[1];
	SKMAC_ASSERT(m_pBuffer);
	m_pBuffer[0] =_T('\0');
}
SKString::SKString(const TCHAR * String)
{
	SKMAC_ASSERT(String);
	m_pBuffer = NULL;
	if(String)
	{
		unsigned int  uiLength = (unsigned int)SKStrLen(String);
		m_pBuffer= SK_NEW TCHAR[uiLength + 1];
		SKMAC_ASSERT(m_pBuffer);

		SKStrCopy(m_pBuffer,uiLength + 1,String);
	}
	else
	{
		m_pBuffer= SK_NEW TCHAR[1];
		SKMAC_ASSERT(m_pBuffer);
		m_pBuffer[0] =_T('\0');
	}
}
SKString::SKString(const SKString &String)
{
	m_pBuffer = NULL;
	unsigned int  uiLength = String.GetLength();
	m_pBuffer= SK_NEW TCHAR[uiLength + 1];
	SKMAC_ASSERT(m_pBuffer);

	SKStrCopy(m_pBuffer,uiLength + 1,String.GetBuffer());
}
SKString::~SKString()
{

	SKMAC_DELETEA(m_pBuffer);

}
void SKString::SetTCHARBufferNum(unsigned int uiLength)
{
	SKMAC_DELETEA(m_pBuffer);
	m_pBuffer= SK_NEW TCHAR[uiLength + 1];
	m_pBuffer[uiLength] =_T('\0');
}
void SKString::Clear()
{

	SKMAC_DELETEA(m_pBuffer);
	m_pBuffer= SK_NEW TCHAR[1];
	SKMAC_ASSERT(m_pBuffer);
	m_pBuffer[0] =_T('\0');
}
SKString & SKString::operator =(const SKString &String)
{


	SKMAC_DELETEA(m_pBuffer);
	unsigned int  uiLength = String.GetLength();
	m_pBuffer= SK_NEW TCHAR[uiLength + 1];
	SKMAC_ASSERT(m_pBuffer);

	SKStrCopy(m_pBuffer,uiLength + 1,String.GetBuffer());
	return *this;
}
SKString & SKString::operator =(const TCHAR *String)
{
	SKMAC_ASSERT(String);
	SKMAC_DELETEA(m_pBuffer);
	if(String)
	{
		unsigned int  uiLength = (unsigned int)SKStrLen(String);
		m_pBuffer= SK_NEW TCHAR[uiLength + 1];
		SKMAC_ASSERT(m_pBuffer);

		SKStrCopy(m_pBuffer,uiLength + 1,String);
	}
	else
	{
		SKString();
	}
	return *this;
}

TCHAR &SKString::operator[](unsigned int i)
{
	return m_pBuffer[i];
}

const SKString & SKString::operator +=(const SKString &String)
{
	unsigned int uiLength1 = GetLength();
	unsigned int uiLength2 = String.GetLength();
	if(!uiLength1)
	{
		*this = String;
		return *this;
	}
	else if(!uiLength2)
		return *this;
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = SK_NEW TCHAR[uiLength];
		SKMAC_ASSERT(pBuffer);

		SKStrCopy(pBuffer,uiLength,GetBuffer());
		SKStrcat(pBuffer,uiLength,String.GetBuffer());

		SKMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;
		return *this;

	}
}
const SKString & SKString::operator +=(const TCHAR *String)
{
	unsigned int uiLength1 = GetLength();
	unsigned int uiLength2 = 0;
	if (String)
	{
		uiLength2 = (unsigned int)SKStrLen(String);
	}
	if(!uiLength1)
	{
		*this = String;
		return *this;
	}
	else if(!uiLength2)
		return *this;
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = SK_NEW TCHAR[uiLength];
		SKMAC_ASSERT(pBuffer);

		SKStrCopy(pBuffer,uiLength,GetBuffer());
		SKStrcat(pBuffer,uiLength,String);

		SKMAC_DELETEA(m_pBuffer);
		m_pBuffer = pBuffer;
		return *this;

	}
}
bool SKString::GetString(const SKString & String,unsigned int uiFind,bool bIsFront, bool bIsHaveFind)
{
	if (uiFind >= String.GetLength())
	{
		return false;
	}
	
	unsigned int uiLength = 0;
	unsigned int uiIndex = 0;
	if ( bIsFront )
	{
		uiLength = uiFind;
		uiIndex = 0;
	}
	else
	{
		uiLength = String.GetLength() - uiFind - 1;
		uiIndex = uiFind + 1;
	}
	if (bIsHaveFind)
	{
		uiLength++;
		if ( !bIsFront )
		{
			uiIndex--;
		}
	}
	if (uiIndex >= String.GetLength())
	{
		return false;
	}
	SKMAC_DELETEA(m_pBuffer);
	m_pBuffer = SK_NEW TCHAR[uiLength + 1];
	SKMemcpy(m_pBuffer,&String.m_pBuffer[uiIndex],uiLength,uiLength + 1);
	m_pBuffer[uiLength] = _T('\0');
	return true;

}
bool SKString::GetString(const SKString & String,TCHAR cFind,int iFIndNum,bool bIsFront, bool bIsHaveFind)
{
	if (iFIndNum == 0)
	{
		return false;
	}
	unsigned int uiFIndNumTemp = 0;
	int uiIndex = -1;
	for (unsigned int i = 0 ; i < String.GetLength() ; i++)
	{
		if (cFind == String.m_pBuffer[i])
		{
			uiFIndNumTemp++;
			uiIndex = i;
		}
		if (uiFIndNumTemp == iFIndNum)
		{		
			break;
		}
	}
	if (uiIndex == -1)
	{
		return false;
	}
	return GetString(String,uiIndex,bIsFront,bIsHaveFind);

}
int SKString::GetSubStringIndex(const SKString &String, int iFindNum)const
{
	if (GetLength() < String.GetLength())
	{
		return -1;
	}
	unsigned int uiFindNumTemp = 0;
	int iFindIndex = -1;
	for (unsigned int Index = 0; Index < GetLength() ; Index++)
	{
		unsigned int j = 0;
		for (unsigned int i = Index ; i < String.GetLength() + Index ; i++)
		{
			if (m_pBuffer[i] == String.m_pBuffer[j])
			{
				j++;
				continue;
			}
			else
			{
				break;
			}
		}
		if (j == String.GetLength())
		{
			iFindIndex = Index;
			uiFindNumTemp++;
			if (uiFindNumTemp == iFindNum)
			{
				
				return iFindIndex;
			}
		}
	}

	return iFindIndex;

}
bool SKString::StripChars(const SKString &String,TCHAR * pStripChars)
{
	if (String.GetLength() > BUFFER_SIZE || !pStripChars)
	{
		return false;
	}
	
	unsigned int strip_chars_len = SKStrLen(pStripChars);
	TCHAR *_string_in = String.GetBuffer() , *_string_out = TempBuffer;
	while(*_string_in != _T('\0'))
	{
		int flag=0;
		for(unsigned int i = 0 ; i < strip_chars_len ; i++)
		{
			if(*_string_in == pStripChars[i])
			{
				flag=1;
				break;
			}
		}
		if(flag == 0)
		{
			*_string_out=*_string_in;
			_string_out++;


		}
		_string_in++;

	}
	*_string_out=_T('\0');
	*this = TempBuffer;
	return true;

}

bool SKString::ReplaceChars(const SKString &String ,TCHAR * pReplaceChars,TCHAR UseChar)
{
	if (String.GetLength() > BUFFER_SIZE || !pReplaceChars)
	{
		return false;
	}
	unsigned int replace_chars_len=SKStrLen(pReplaceChars);
	TCHAR *_string_in = String.GetBuffer(),*_string_out = TempBuffer;
	while(*_string_in != _T('\0'))
	{
		int flag=0;
		for(unsigned int i = 0 ; i < replace_chars_len ; i++)
		{
			if(*_string_in==pReplaceChars[i])
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			*_string_out = *_string_in;

		}
		else
		{
			*_string_out = UseChar;

		}
		_string_out++;
		_string_in++;

	}
	*_string_out=_T('\0');

	*this = TempBuffer;
	return true;
}

void SKString::StringLtrim(const SKString &String)
{
	int index=0;
	TCHAR * pString = String.GetBuffer();
	while(_istspace(pString[index]))
		index++;
	if(index==0)
		return;
	else
	{
		unsigned int uiLength=String.GetLength();
		SKMemcpy((void *)TempBuffer, (void *)(pString + index),  ((uiLength - index) + 1) * sizeof(TCHAR) );
		*this = TempBuffer;
	}
}

void SKString::StringRtrim(const SKString &String)
{
	unsigned int uiLength=String.GetLength();
	if (!uiLength)
	{
		return ;
	}
	unsigned int index = uiLength - 1;
	TCHAR * pString = String.GetBuffer();
	while(_istspace(pString[index]))
		index--;

	SKMemcpy((void *)TempBuffer, (void *)(pString),  (index + 1) * sizeof(TCHAR));
	index++;
	TempBuffer[index]=_T('\0');
	*this = TempBuffer;
}
void SKString::Format(const TCHAR * pcString, ...)
{
	static TCHAR LogBuffer[LOG_BUFFER_SIZE];
	char *pArgs;
	pArgs = (char*) &pcString + sizeof(pcString);
	SKSprintf(LogBuffer, LOG_BUFFER_SIZE,pcString, pArgs);
	*this = LogBuffer;
}
bool  SKFileName::GetExtension(SKString & Extension)
{
	return Extension.GetString(*this,_T('.'),-1,false,false);
}
bool SKFileName::GetPathAndName(SKString & PathAndName)
{
	return PathAndName.GetString(*this,_T('.'),-1);
}
bool SKFileName::GetBaseName(SKString & BaseName)
{
	bool bResult = false;
	SKString BaseNameAndExtension;
	bResult = GetBaseNameAndExtension(BaseNameAndExtension);
	if (!bResult)
	{
		return false;
	}
	bResult = BaseName.GetString(BaseNameAndExtension,_T('.'),-1);
	return bResult;
}
bool SKFileName::GetBaseNameAndExtension(SKString & BaseNameAndExtension)
{
	bool bResult = false;
	bResult = BaseNameAndExtension.GetString(*this,_T('\\'),-1,false,false);
	if (!bResult)
	{
		bResult = BaseNameAndExtension.GetString(*this,_T('/'),-1,false,false);
	}
	return bResult;
}
bool SKFileName::GetPath(SKString & Path)
{
	bool bResult = false;
	bResult = Path.GetString(*this,_T('\\'),-1,true,false);
	if (!bResult)
	{
		bResult = Path.GetString(*this,_T('/'),-1,true,false);
	}
	return bResult;
}
namespace SKEngine2
{
SKString operator +(const SKString & String1, const SKString &String2)
{
	unsigned int uiLength1 = String1.GetLength();
	unsigned int uiLength2 = String2.GetLength();
	if (!uiLength2)
		return String1;
	else if(!uiLength1)
		return String2; 
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = SK_NEW TCHAR[uiLength];
		SKMAC_ASSERT(pBuffer);

		SKStrCopy(pBuffer,uiLength,String1.GetBuffer());
		SKStrcat(pBuffer,uiLength,String2.GetBuffer());
		SKString ReturnValue(pBuffer);
		SKMAC_DELETEA(pBuffer);
		return ReturnValue;
	}
}
SKString operator +(const SKString &String1, const TCHAR *String2)
{
	SKMAC_ASSERT(String2);
	unsigned int uiLength1 = String1.GetLength();
	unsigned int uiLength2 = 0;
	if (String2)
	{
		uiLength2 = (unsigned int)SKStrLen(String2);
	}
	
	if (!uiLength2)
		return String1;
	else if(!uiLength1)
		return SKString(String2); 
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = SK_NEW TCHAR[uiLength];
		SKMAC_ASSERT(pBuffer);

		SKStrCopy(pBuffer,uiLength,String1.GetBuffer());
		SKStrcat(pBuffer,uiLength,String2);
		SKString ReturnValue(pBuffer);
		SKMAC_DELETEA(pBuffer);
		return ReturnValue;
	}
}
SKString operator +(const TCHAR *String1, const SKString &String2)
{
	SKMAC_ASSERT(String1);
	unsigned int uiLength2 = String2.GetLength();
	unsigned int uiLength1 = 0;
	if (String1)
	{
		uiLength1 = (unsigned int)SKStrLen(String1);
	}

	if (!uiLength1)
		return String2;
	else if(!uiLength2)
		return SKString(String1); 
	else
	{
		TCHAR *pBuffer = NULL;
		unsigned int uiLength = uiLength2 + uiLength1 + 1;
		pBuffer = SK_NEW TCHAR[uiLength];
		SKMAC_ASSERT(pBuffer);

		SKStrCopy(pBuffer,uiLength,String1);
		SKStrcat(pBuffer,uiLength,String2.GetBuffer());
		SKString ReturnValue(pBuffer);
		SKMAC_DELETEA(pBuffer);
		return ReturnValue;
	}
}
int CompareString(const SKString &String1,const SKString &String2)
{
	return SKStrCmp(String1.GetBuffer(),String2.GetBuffer());

}
int CompareString(const SKString &String1,const TCHAR *String2)
{
	return SKStrCmp(String1.GetBuffer(),String2);
}
int CompareString(const TCHAR *String1,const SKString &String2)
{
	return SKStrCmp(String1,String2.GetBuffer());
}



bool operator ==(const SKString &String1,const SKString &String2)
{
	return (SKStrCmp(String1.GetBuffer(),String2.GetBuffer()) == 0);
}
bool operator ==(const SKString &String1,const TCHAR *String2)
{
	return (SKStrCmp(String1.GetBuffer(),String2) == 0);
}
bool operator ==(const TCHAR *String1,SKString &String2)
{
	return (SKStrCmp(String1,String2.GetBuffer()) == 0);
}

bool operator !=(const SKString &String1,const SKString &String2)
{
	return (SKStrCmp(String1.GetBuffer(),String2.GetBuffer()) != 0);
}
bool operator !=(const SKString &String1,const TCHAR *String2)
{
	return (SKStrCmp(String1.GetBuffer(),String2) != 0);	
}
bool operator !=(const TCHAR *String1,const SKString &String2)
{
	return (SKStrCmp(String1,String2.GetBuffer()) != 0);
}
int StringToInt(const SKString & String)
{
	int i = 0 ;
	SKScanf(String.GetBuffer(), "%d", (TCHAR *)&i);
	return i;
}
SKString IntToString(int Value)
{
	SKSprintf(SKString::TempBuffer,2048,_T("%d"),Value);
	return SKString(SKString::TempBuffer);
}
unsigned int StringToUInt(const SKString & String)
{
	unsigned int i = 0;
	SKScanf(String.GetBuffer(), "%d", (TCHAR *)&i);
	return i;
}
SKString UIntToString(unsigned int Value)
{
	SKSprintf(SKString::TempBuffer, 2048, _T("%d"), Value);
	return SKString(SKString::TempBuffer);
}
SKREAL StringToReal(const SKString & String)
{
	SKREAL f = 0.0f;
	_stscanf_s(String.GetBuffer(),"%f",&f);
	//SKScanf(String.GetBuffer(), "%d", (TCHAR *)&f);
	return f;
}
SKString RealToString(SKREAL Value)
{
	SKSprintf(SKString::TempBuffer,2048,_T("%f"),Value);
	return SKString(SKString::TempBuffer);

}
bool StringToBool(const SKString & String)
{
	if (String == _T("True") || String == _T("true") || String == _T("TRUE"))
	{
		return true;
	}
	else if (String == _T("False") || String == _T("false") || String == _T("FALSE"))
	{
		return false;
	}

	int d = 0;
	_stscanf_s(String.GetBuffer(),"%d",&d);
	if (d)
	{
		return true;
	}
	else
		return false;
}
SKString BoolToString(bool i)
{
	if (i)
	{
		return _T("True");
	}
	else
	{
		return _T("False");
	}
}
SKString BoolToString(unsigned int i)
{
	if (i)
	{
		return _T("True");
	}
	else
	{
		return _T("False");
	}
}
}