#ifndef SKSURFACE3SUBDIVISIONBYUNIFORM_H
#define SKSURFACE3SUBDIVISIONBYUNIFORM_H
#include "SKSurface3Subdivision.h"
/*
ȷ淽


*/
namespace SKEngine2
{
	class SKMATH_API SKSurface3SubdivisionByUniform : public SKSurface3Subdivision
	{
	public:
		SKSurface3SubdivisionByUniform(unsigned int uiRow,unsigned int uiColumn,SKREAL U1 = 0.0f,SKREAL U2 = 1.0f,SKREAL V1 = 0.0f,SKREAL V2 = 1.0f);
		virtual ~SKSurface3SubdivisionByUniform();
		virtual bool Subdivision();
		virtual unsigned int GetSubdivisionMethod(){return SM_UNIFORM;}
	};


}
#endif