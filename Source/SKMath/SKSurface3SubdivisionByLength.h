#ifndef SKSURFACE3SUBDIVISIONBYLENGTH_H
#define SKSURFACE3SUBDIVISIONBYLENGTH_H
#include "SKSurface3Subdivision.h"
/*
ȷ淽


*/
namespace SKEngine2
{
	class SKMATH_API SKSurface3SubdivisionByLength : public SKSurface3Subdivision
	{
	public:
		SKSurface3SubdivisionByLength(unsigned int Row,unsigned int Column,SKREAL U1 = 0.0f,SKREAL U2 = 1.0f,SKREAL V1 = 0.0f,SKREAL V2 = 1.0f,unsigned int uiIterations = 32);
		virtual ~SKSurface3SubdivisionByLength();
		virtual bool Subdivision();
		virtual unsigned int GetSubdivisionMethod(){return SM_LENGTH;}
	protected:
		unsigned int m_uiIterations;
	};


}
#endif