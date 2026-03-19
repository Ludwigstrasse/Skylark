
#ifndef SKSINGLETON_H
#define SKSINGLETON_H

#include "SKSystem.h"

namespace SKEngine2
{

template <typename T> 
class  SKSingleton
{
	static T *m_pSingleton;

public:
	SKSingleton()
	{
		SKMAC_ASSERT( !m_pSingleton );
		m_pSingleton = static_cast<T*>(this);
	}

	~SKSingleton()
	{
		SKMAC_ASSERT( m_pSingleton );
		m_pSingleton = NULL;
	}

	static T &GetSingleton()
	{ 
		SKMAC_ASSERT( m_pSingleton );
		return (*m_pSingleton); 
	}

	static T *GetSingletonPtr()
	{ 
		return (m_pSingleton);
	}
};

template <typename T> T *SKSingleton<T>::m_pSingleton = NULL;

}

#endif //__C_SINGLETON_H__
