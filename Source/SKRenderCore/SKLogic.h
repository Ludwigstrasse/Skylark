#ifndef SKLOGIC_H
#define SKLOGIC_H
#include "SKMemManager.h"
#include "SKName.h"
namespace SKEngine2
{
	struct SKLOGIC_INPUT_EVENT 
	{
		unsigned int uiInputType;
		unsigned int uiEvent;
		unsigned int uiKey;
		int x;
		int y;
		int z;
	};
	struct SKMessage
	{
		SKMessage()
		{

		}
		~SKMessage()
		{

		}
		SKUsedName Sender;
		SKUsedName Receiver;
		unsigned int MessageType;
		double DispatchTime;
		void * pData;
		unsigned int uiDataSize;
	};
}

#endif