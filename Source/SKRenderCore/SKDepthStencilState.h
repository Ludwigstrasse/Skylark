#ifndef SKDEPTHSENTCILSTATE_H
#define SKDEPTHSENTCILSTATE_H
#include "SKBind.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKDepthStencilDesc : public SKObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Compare Nethod
		{
			CM_NEVER,
			CM_LESS,
			CM_EQUAL,
			CM_LESSEQUAL,
			CM_GREATER,
			CM_NOTEQUAL,
			CM_GREATEREQUAL,
			CM_ALWAYS,
			CM_MAX

		};
		enum //OperationType
		{
			OT_KEEP,
			OT_ZERO,
			OT_REPLACE,
			OT_INCREMENT,
			OT_DECREMENT,
			OT_INVERT,
			OT_INCREMENT_STA,
			OT_DECREMENT_STA,
			OT_MAX
		};
		SKDepthStencilDesc()
		{
			m_bDepthEnable = true;
			m_bDepthWritable = true;
			m_uiDepthCompareMethod = CM_LESSEQUAL;


			m_bStencilEnable = false;             
			m_uiStencilCompareMethod = CM_ALWAYS;		
			m_uiReference = 0x0;   
			m_uiMask = 0xFF;       
			m_uiWriteMask = 0xFF;   
			m_uiSPassZPassOP = OT_KEEP;     
			m_uiSPassZFailOP = OT_KEEP;    
			m_uiSFailZPassOP = OT_KEEP;

			m_uiCCW_StencilCompareMethod = CM_ALWAYS;	
			m_uiCCW_SPassZPassOP = OT_KEEP;   
			m_uiCCW_SPassZFailOP = OT_KEEP;   
			m_uiCCW_SFailZPassOP = OT_KEEP;
			m_bTwoSideStencilMode = false;
		}
		~SKDepthStencilDesc()
		{

		}


		bool m_bDepthEnable;
		bool m_bDepthWritable;
		unsigned char m_uiDepthCompareMethod;
		

		bool m_bStencilEnable;             
		unsigned char m_uiStencilCompareMethod;		
		unsigned char m_uiReference;   
		unsigned char m_uiMask;       
		unsigned char m_uiWriteMask;   
		unsigned char m_uiSPassZPassOP;     
		unsigned char m_uiSPassZFailOP;    
		unsigned char m_uiSFailZPassOP;

		unsigned char m_uiCCW_StencilCompareMethod;	
		unsigned char m_uiCCW_SPassZPassOP;     
		unsigned char m_uiCCW_SPassZFailOP;    
		unsigned char m_uiCCW_SFailZPassOP;
		bool m_bTwoSideStencilMode;

		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(SKDepthStencilDesc) - sizeof(SKObject);
			return (void *)&m_bDepthEnable;
		}
	};
	SKTYPE_MARCO(SKDepthStencilDesc);
	class SKGRAPHIC_API SKDepthStencilState : public SKBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~SKDepthStencilState();
	protected:
		SKDepthStencilState();
		SKDepthStencilDesc m_DepthStencilDesc;
	protected:
		static SKPointer<SKDepthStencilState> Default;
	public:
		static const SKDepthStencilState *GetDefault()
		{
			return Default;
		}

		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		FORCEINLINE const SKDepthStencilDesc& GetDepthStencilDesc()const
		{
			return m_DepthStencilDesc;
		}
		friend class SKResourceManager;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
	};
	DECLARE_Ptr(SKDepthStencilState);
	SKTYPE_MARCO(SKDepthStencilState);
}
#endif