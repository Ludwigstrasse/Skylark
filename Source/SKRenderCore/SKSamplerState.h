#ifndef SKSAMPLERSTATE_H
#define SKSAMPLERSTATE_H
#include "SKBind.h"
#include "SKVector3W.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKSamplerDesc : public SKObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Coordinate Mode
		{
			CM_CLAMP,
			CM_WRAP,
			CM_MIRROR,					
			CM_BORDER,
			CM_MAX


		};
		enum //Filter Method
		{
			FM_NONE,
			FM_POINT,
			FM_LINE,
			FM_ANISOTROPIC,
			FM_MAX

		};
		SKSamplerDesc()
		{
			m_uiMag = FM_POINT;
			m_uiMin = FM_POINT;
			m_uiMip = FM_NONE;
			m_uiMipLevel = 0;
			m_uiAniLevel = 1;

			m_uiCoordU = CM_WRAP;
			m_uiCoordV = CM_WRAP;
			m_uiCoordW = CM_WRAP;
			m_BorderColor = SKColorRGBA(0.0f,0.0f,0.0f,0.0f);
			m_MipLODBias = 0.0f;
		}
		~SKSamplerDesc()
		{

		}

		

		unsigned char m_uiMag;
		unsigned char m_uiMin;
		unsigned char m_uiMip;
		unsigned char m_uiMipLevel;
		unsigned char m_uiAniLevel;

		unsigned char m_uiCoordU;
		unsigned char m_uiCoordV;
		unsigned char m_uiCoordW;
		SKREAL        m_MipLODBias;
		SKColorRGBA  m_BorderColor;
		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(SKSamplerDesc)-sizeof(SKObject);
			return (void *)&m_uiMag;
		}
	};
	SKTYPE_MARCO(SKSamplerDesc);
	class SKGRAPHIC_API SKSamplerState : public SKBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~SKSamplerState();
	protected:
		SKSamplerState();
		SKSamplerDesc m_SamplerDesc;
	protected:
		static SKPointer<SKSamplerState> Default;
		static SKPointer<SKSamplerState> TriLine;
		static SKPointer<SKSamplerState> DoubleLine;
		static SKPointer<SKSamplerState> ShadowMapSampler;
		static SKPointer<SKSamplerState> BorderARGB0Sampler;
		static SKPointer<SKSamplerState> LightFunctionSampler;
	public:
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		const SKSamplerDesc & GetSamplerDesc()const
		{

			return m_SamplerDesc;
		}
		static const SKSamplerState *GetDefault()
		{
			return Default;
		}
		static const SKSamplerState *GetTriLine()
		{
			return TriLine;
		}
		static const SKSamplerState *GetDoubleLine()
		{
			return DoubleLine;
		}
		static const SKSamplerState * GetShadowMapSampler()
		{
			return ShadowMapSampler;
		}
		static const SKSamplerState * GetBorderARGB0Sampler()
		{
			return BorderARGB0Sampler;
		}
		static const SKSamplerState * GetLightFunctionSampler()
		{
			return LightFunctionSampler;
		}
		friend class SKResourceManager;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
	};
	DECLARE_Ptr(SKSamplerState);
	SKTYPE_MARCO(SKSamplerState);
};
#endif