#ifndef SKBLENDSTATE_H
#define SKBLENDSTATE_H
#include "SKBind.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKBlendDesc : public SKObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum
		{
			MAX_RENDER_TARGET_NUM = 8
		};
		enum //Blend Parameter
		{
			BP_ZERO,
			BP_ONE,
			BP_SRCCOLOR,
			BP_INVSRCCOLOR,
			BP_SRCALPHA,
			BP_INVSRCALPHA,
			BP_DESTALPHA,
			BP_INVDESTALPHA,
			BP_DESTCOLOR,
			BP_INVDESTCOLOR,
			BP_MAX
		};
		enum// Blend OP
		{
			BO_ADD,
			BO_SUBTRACT,
			BO_REVSUBTRACT,
			BO_MIN_SRC_DEST,
			BO_MAX_SRC_DEST, 
			BO_MAX

		};
		enum //Write Mask
		{
			WM_NONE	 = 0,
			WM_ALPHA = BIT(0),
			WM_RED = BIT(1),
			WM_Green = BIT(2),
			WM_BLUE = BIT(3),
			WM_ALL = 0X0F
		};
		SKBlendDesc()
		{
			bAlphaToCoverageEnable = false;
			bIndependentBlendEnable = false;
			for (unsigned int i = 0 ; i < MAX_RENDER_TARGET_NUM ;i++)
			{
				bBlendEnable[i] = false;
				ucSrcBlend[i] = BP_ONE;
				ucDestBlend[i] = BP_ZERO;
				ucBlendOp[i] = BO_ADD;

				bAlphaBlendEnable[i] = false;
				ucSrcBlendAlpha[i] = BP_ONE;
				ucDestBlendAlpha[i] = BP_ZERO;
				ucBlendOpAlpha[i] = BO_ADD;
				ucWriteMask[i] = WM_ALL;
			}
// 			fBlendColor[0] = fBlendColor[1] = fBlendColor[2] = fBlendColor[3] = 0.0f;
// 			ucSampleMask = 0xffffffff;
		}
		~SKBlendDesc()
		{

		}
		bool			bAlphaToCoverageEnable;
		bool			bIndependentBlendEnable;
// 		SKREAL			fBlendColor[4];
// 		unsigned int	ucSampleMask;
		bool			bBlendEnable[MAX_RENDER_TARGET_NUM];
		unsigned char   ucSrcBlend[MAX_RENDER_TARGET_NUM];
		unsigned char   ucDestBlend[MAX_RENDER_TARGET_NUM];
		unsigned char	ucBlendOp[MAX_RENDER_TARGET_NUM];

		bool			bAlphaBlendEnable[MAX_RENDER_TARGET_NUM];
		unsigned char   ucSrcBlendAlpha[MAX_RENDER_TARGET_NUM];
		unsigned char	ucDestBlendAlpha[MAX_RENDER_TARGET_NUM];
		unsigned char	ucBlendOpAlpha[MAX_RENDER_TARGET_NUM];
		unsigned char   ucWriteMask[MAX_RENDER_TARGET_NUM];
		bool IsBlendUsed(unsigned int uiChannal = 0)const
		{
			return (bBlendEnable[uiChannal] || bAlphaBlendEnable[uiChannal]);
		}
		void * GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(SKBlendDesc)-sizeof(SKObject);
			return (void *)&bAlphaToCoverageEnable;
		}
	};
	SKTYPE_MARCO(SKBlendDesc);
	class SKGRAPHIC_API SKBlendState : public SKBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:	
		virtual ~SKBlendState();
	protected:
		SKBlendState();
		SKBlendDesc m_BlendDesc;
	protected:
		static SKPointer<SKBlendState> Default;
	public:
		static const SKBlendState *GetDefault()
		{
			return Default;
		}
	public:
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	public:
		FORCEINLINE const SKBlendDesc & GetBlendDesc()const
		{
			return m_BlendDesc;
		}
		friend class SKResourceManager;
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
	};
	DECLARE_Ptr(SKBlendState);
	SKTYPE_MARCO(SKBlendState);
}
#endif