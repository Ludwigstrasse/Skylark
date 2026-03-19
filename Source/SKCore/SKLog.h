#ifndef SKLOG_H
#define SKLOG_H

#include "SKFile.h"

namespace SKEngine2
{
	// Legacy log file helper. Keep for SKEngine2 compatibility.
	class SKSYSTEM_API SKLog : public SKFile
	{
	public:
		SKLog();
		~SKLog();

		bool Open(const TCHAR* pFileName);
		bool WriteInfo(const TCHAR* pcString, ...) const;
	};
}

#endif
