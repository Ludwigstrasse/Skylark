#include "SKCommand.h"
using namespace SKEngine2;
//----------------------------------------------------------------------------
SKCommand::SKCommand (int numArguments, TCHAR** arguments)
{
	for (int i = 0; i < numArguments; ++i)
	{
		SKString Command(arguments[i]);
		SKString Value;
		if (Value.GetString(Command, '=', 1, false))
		{
			SKString Key;
			Key.GetString(Command, '=', 1);
			m_CommandMap.AddElement(Key, Value);
		}
		else
		{
			m_CommandMap.AddElement(Command, Value);
		}
	}
}
//----------------------------------------------------------------------------
SKCommand::SKCommand (TCHAR* commandLine)
{

	if (!commandLine)
	{
		return;
	}

	TCHAR * pNextToken = NULL;
	TCHAR* token = SKStrtok(commandLine, " ",&pNextToken);
	while (token)
	{
		SKString Command(token);
		SKString Value;
		if (Command.GetString(Value, '=', 1, false))
		{
			SKString Key;
			Command.GetString(Key, '=', 1);
			m_CommandMap.AddElement(Key, Value);
		}
		else
		{
			m_CommandMap.AddElement(Command, Value);
		}

		token = SKStrtok(NULL, " ",&pNextToken);
	}


}
//----------------------------------------------------------------------------
SKCommand::~SKCommand ()
{

}
//----------------------------------------------------------------------------
bool SKCommand::GetName(const TCHAR* name)
{

	if (m_CommandMap.Find(name) != m_CommandMap.GetNum())
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool SKCommand::GetInteger (const TCHAR* name, int& value)
{
	unsigned int i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = StringToInt(m_CommandMap[i].Value);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool SKCommand::GetReal(const TCHAR* name, SKREAL& value)
{
	unsigned int i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = StringToReal(m_CommandMap[i].Value);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool SKCommand::GetString (const TCHAR* name, SKString & value)
{
	unsigned int i = m_CommandMap.Find(name);
	if (i != m_CommandMap.GetNum())
	{
		value = m_CommandMap[i].Value;
		return true;
	}
	
	return false;
}
