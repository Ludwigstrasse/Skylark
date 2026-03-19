#ifndef SKSKYLIGHT_H
#define SKSKYLIGHT_H
#include "SKLight.h"
namespace SKEngine2
{
	
	class SKStream;
	class SKGRAPHIC_API SKSkyLight : public SKIndirectLight
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKSkyLight();
		virtual ~SKSkyLight();
		SKColorRGBA m_UpColor;
		SKColorRGBA m_DownColor;
		virtual unsigned int GetLightType()const
		{
			return LT_SKY;
		}
	};
	DECLARE_Ptr(SKSkyLight);
	SKTYPE_MARCO(SKSkyLight);
}
#endif