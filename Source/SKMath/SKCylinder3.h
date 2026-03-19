#ifndef SKCYLINDER3
#define SKCYLINDER3
#include "SKVector3.h"
namespace SKEngine2
{
	class SKMATH_API SKCylinder3
	{
	public: 
		SKCylinder3(SKVector3 Center,SKVector3 Dir,SKREAL fRadius, SKREAL fHalfHeight);
		~SKCylinder3();
		SKVector3	m_Center;
		SKVector3	m_Dir;
		SKREAL		m_fRadius;
		SKREAL		m_fHalfHeight;
	};
}

#endif