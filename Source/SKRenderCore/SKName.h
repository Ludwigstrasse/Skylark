#ifndef SKNAME_H
#define SKNAME_H
#include "SKString.h"
#include "SKReference.h"
#include "SKPriority.h"
#include "SKCustomArchiveObject.h"
#include "SKInitial.marc"
namespace SKEngine2
{
	class SKGRAPHIC_API SKName : public SKReference,public SKMemObject
	{
	public:
		friend class SKResourceManager;
		~SKName();
		explicit SKName(const TCHAR * pChar,unsigned int uiID);
		explicit SKName(const SKString & String,unsigned int uiID);

		FORCEINLINE const TCHAR * GetBuffer()const
		{
			return m_String.GetBuffer();
		}
		FORCEINLINE const SKString & GetString()const
		{
			return m_String;
		}
		FORCEINLINE unsigned int GetLength()const
		{
			return m_String.GetLength();
		}

		FORCEINLINE unsigned int GetID()const
		{
			return m_uiID;
		}
		
	protected:
		
		unsigned int m_uiID;
		SKString  m_String;
	
	};
	DECLARE_Ptr(SKName);

	class SKStream;
	class SKGRAPHIC_API SKUsedName : public SKCustomArchiveObject
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		~SKUsedName();
		SKUsedName();
		/*explicit */SKUsedName(const TCHAR * pChar);
		/*explicit */SKUsedName(const SKString & String);

		//=
		void operator =(const SKString &String);
		void operator =(const TCHAR *pChar);
		void operator =(const SKUsedName & Name);
		FORCEINLINE const TCHAR * GetBuffer()const
		{
			if (!m_pName)
			{
				return NULL;
			}
			return m_pName->GetBuffer();
		}
		FORCEINLINE const SKString & GetString()const
		{
			if (!m_pName)
			{
				return SKString::ms_StringNULL;
			}
			return m_pName->GetString();
		}
		FORCEINLINE unsigned int GetLength()const
		{
			if (!m_pName)
			{
				return 0;
			}
			return m_pName->GetLength();
		}
		FORCEINLINE unsigned int GetNameCode()const
		{
			if (!m_pName)
			{
				return 0;
			}
			return m_pName->GetID();
		}

	
		SKGRAPHIC_API friend bool operator >(const SKUsedName &Name1,const SKUsedName &Name2);
	
		SKGRAPHIC_API friend bool operator <(const SKUsedName &Name1,const SKUsedName &Name2);

		//==
		SKGRAPHIC_API friend bool operator ==(const SKUsedName &Name1,const SKUsedName &Name2);

		//!=
		SKGRAPHIC_API friend bool operator !=(const SKUsedName &Name1,const SKUsedName &Name2);

		//==
		SKGRAPHIC_API friend bool operator ==(const SKUsedName &Name,const SKString & String);

		//!=
		SKGRAPHIC_API friend bool operator !=(const SKUsedName &Name,const SKString & String);

		//==
		SKGRAPHIC_API friend bool operator ==(const SKUsedName &Name,const TCHAR * pChar);

		//!=
		SKGRAPHIC_API friend bool operator !=(const SKUsedName &Name,const TCHAR * pChar);

		virtual void Archive(SKStream & Stream);
		virtual void CopyFrom(SKCustomArchiveObject *,SKMap<SKObject *,SKObject*>& CloneMap);
	protected:
		SKNamePtr m_pName;
	
		DECLARE_INITIAL_ONLY
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		static SKUsedName ms_cPassID;
		static SKUsedName ms_cPrezBeUsedBone;
		static SKUsedName ms_cMaterialVertexFormat;
		static SKUsedName ms_cVolumeVertexFormat;
		static SKUsedName ms_cLightFunKey;
		static SKUsedName ms_cMaterialLightKey;
		static SKUsedName ms_cLighted;

		static SKUsedName ms_cMaterialVertex;
		static SKUsedName ms_cPrezVertex;
		static SKUsedName ms_cNormalDepthVertex;
		static SKUsedName ms_cCubShadowVertex;
		static SKUsedName ms_cVolumeShadowVertex;
		static SKUsedName ms_cShadowVertex;
		static SKUsedName ms_cDualParaboloidShadowVertex;
		static SKUsedName ms_cLightFunVertex;
		static SKUsedName ms_cPostEffectVertex;

		static SKUsedName ms_cCubShadowFov;

		static SKUsedName ms_cPrezPiexl;
		static SKUsedName ms_cPostGray;
		static SKUsedName ms_cGammaCorrect;
		static SKUsedName ms_cPostScreenQuad;
		static SKUsedName ms_cPostVolumeShadowMap;

		static SKUsedName ms_cPostInputTexture;
		static SKUsedName ms_cPostInv_Width;
		static SKUsedName ms_cPostInv_Height;
		
		static SKUsedName ms_cFontColor;

		static SKUsedName ms_cMainScene;
		static SKUsedName ms_cPointLightShadowNum;
		static SKUsedName ms_cPointLightVolumeShadowNum;
		static SKUsedName ms_cPointLightParaboloidShadowNum;
		static SKUsedName ms_cDirectionLightShadowNum;
		static SKUsedName ms_cDirectionLightVolumeShadowNum;
		static SKUsedName ms_cDirectionLightCSMShadowNum;
		static SKUsedName ms_cSpotLightShadowNum;

		// shader use
		static SKUsedName ms_cBlinnPhong;
		static SKUsedName ms_cOrenNayarLookUpTable;
		static SKUsedName ms_cStraussLookUpTable;
		static SKUsedName ms_cCookTorranceSpecluarType;
		static SKUsedName ms_cIsotropicWarLookUpTable;

		static SKUsedName ms_cProjectShadowPreVertex;
		static SKUsedName ms_cProjectShadowPrePixel;

		static SKUsedName ms_cProjectShadowVertex;
		static SKUsedName ms_cProjectShadowPixel;
	};
	CUSTOMTYPE_MARCO(SKUsedName)
}
#endif