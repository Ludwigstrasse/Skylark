#ifndef SKCOMMAND_H
#define SKCOMMAND_H
#include "SKMath.h"
#include "SKString.h"
#include "SKMap.h"
#include "SKMemManager.h"
#include "SKSystem.h"
namespace SKEngine2
{
	class SKCommand : public SKMemObject
	{
	public:

		SKCommand (int numArguments, TCHAR** arguments);
		SKCommand (TCHAR* commandLine);
		~SKCommand ();

		bool GetName(const TCHAR* name); 
		bool GetInteger (const TCHAR* name, int& value);
		bool GetReal (const TCHAR* name, SKREAL& value);
		bool GetString (const TCHAR* name, SKString & value);


	protected:

		SKMap<SKString, SKString> m_CommandMap;
	};
}
#endif