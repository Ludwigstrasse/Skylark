#ifndef SKRASTERIZERSTATE_H
#define SKRASTERIZERSTATE_H
#include "SKBind.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKRasterizerDesc : public SKObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Cull Type
		{
			CT_NONE,
			CT_CW,
			CT_CCW,
			CT_MAX

		};
		SKRasterizerDesc()
		{
			m_bWireEnable = false;
			m_uiCullType = CT_CCW;	
			m_bClipPlaneEnable = false;
			m_bScissorTestEnable = false;
			m_fDepthBias = 0.0f;

		}
		~SKRasterizerDesc()
		{

		}

		
		bool m_bWireEnable;
		unsigned char m_uiCullType;	
		bool m_bClipPlaneEnable;
		bool m_bScissorTestEnable;
		SKREAL	m_fDepthBias;
		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(SKRasterizerDesc)-sizeof(SKObject);
			return (void *)&m_bWireEnable;
		}
	};
	SKTYPE_MARCO(SKRasterizerDesc);
	class SKGRAPHIC_API SKRasterizerState : public SKBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~SKRasterizerState();
	protected:
		SKRasterizerState();
		SKRasterizerDesc m_RasterizerDesc;
	protected:
		static SKPointer<SKRasterizerState> Default;
		static SKPointer<SKRasterizerState> ScissorEnable;
	public:
		static const SKRasterizerState *GetDefault()
		{
			return Default;
		}

		static const SKRasterizerState *GetScissorEnable()
		{
			return ScissorEnable;
		}
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		FORCEINLINE const SKRasterizerDesc & GetRasterizerDesc()const
		{
			return m_RasterizerDesc;
		}
		friend class SKResourceManager;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
	};
	DECLARE_Ptr(SKRasterizerState);
	SKTYPE_MARCO(SKRasterizerState);
}
#endif