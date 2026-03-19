#ifndef SKSURFACE3SUBDIVISION_H
#define SKSURFACE3SUBDIVISION_H
#include "SKControlSurface3.h"
#include "SKVector2.h"
/*
滮ַ


*/
namespace SKEngine2
{
	class SKMATH_API SKSurface3Subdivision
	{
	public:
		enum  //Subdivision Method
		{
			SM_UNIFORM,
			SM_LENGTH,
			SM_MAX
		};
		SKSurface3Subdivision();
		virtual ~SKSurface3Subdivision() = 0;
		virtual bool Subdivision() = 0;
		virtual unsigned int GetSubdivisionMethod() = 0;
		FORCEINLINE bool SetSurface(SKSurface3 * pSurface);

		FORCEINLINE SKVector3 * GetPoints()const;
		FORCEINLINE unsigned int GetRow()const;
		FORCEINLINE unsigned int GetColumn()const;
		FORCEINLINE SKVector2 * GetUV()const;
	protected:
		SKSurface3 * m_pSurface;
		SKVector3 *m_pPoint;
		unsigned int m_uiRow;
		unsigned int m_uiColumn;
		SKREAL	m_fU1,m_fU2;
		SKREAL	m_fV1,m_fV2;
		SKVector2 * m_pUV;
	};
#include "SKSurface3Subdivision.inl"
}
#endif