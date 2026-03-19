#ifndef SKBIND_H
#define SKBIND_H
#include "SKObject.h"
namespace SKEngine2
{

	class SKGRAPHIC_API SKResourceIdentifier
	{
	public:
		virtual ~SKResourceIdentifier () {/**/}

	protected:
		SKResourceIdentifier () {/**/}
	};
	class SKStream;
	class SKGRAPHIC_API SKBind : public SKObject
	{
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum	//Lock Flag
		{
			LF_NOOVERWRITE,
			LF_DISCARD,		
			LF_READONLY,
			LF_MAX
		};
		enum	//Mem Type
		{
			MT_BOTH,
			MT_RAM,
			MT_VRAM,
			MT_MAX	
		};
		enum	// MemType Clear State
		{
			MCS_NONE,
			MCS_READY,
			MCS_DONE,
			MCS_MAX
		};
		SKBind(unsigned int uiMemType = MT_BOTH);
		virtual ~SKBind() = 0;

		SKResourceIdentifier* GetIdentifier();
		unsigned int GetInfoQuantity () const;
		bool IsStatic()const {return m_bIsStatic;}
		//ԴҪlockǶ߳ȾΪЧöЧʣlock߳ʹbmulthreadUseΪtrue
		void SetStatic(bool bIsStatic,bool bMulThreadUse = false);
		static SKArray<SKBind *> ms_DynamicTwoBindArray;
		static SKArray<SKBind *> ms_BindArray;
		void ExChange();
		virtual void ASYNClearInfo();
		
	protected:
		virtual void ClearInfo();
		friend class SKRenderer;

		void Bind(SKResourceIdentifier* pID);

		struct INFO_TYPE
		{
			INFO_TYPE()
			{
				ID = NULL;
			}
			~INFO_TYPE()
			{

			}
			SKResourceIdentifier* ID;
		};
		SKArray<INFO_TYPE> m_InfoArray;
		unsigned int m_uiSwapChainNum;
		unsigned int m_uiCurID;
		bool m_bIsStatic;
		SKRenderer* m_pUser;
		

		unsigned int m_uiLockFlag;

		unsigned int m_uiMemType;
		unsigned int m_uiClearState;
	public:


		virtual	bool LoadResource(SKRenderer * pRender);
		virtual bool ReleaseResource();


		virtual unsigned int GetByteSize()const;

		FORCEINLINE void SetLockFlag(unsigned int uiLockFlag)
		{
			if (uiLockFlag > LF_MAX)
			{
				uiLockFlag = LF_NOOVERWRITE;
			}
			m_uiLockFlag = uiLockFlag;
		}
		FORCEINLINE unsigned int GetLockFlag()const
		{
			return m_uiLockFlag;
		}

		FORCEINLINE void SetMemType(unsigned int uiMemType)
		{
			if (uiMemType < MT_MAX)
			{
				m_uiMemType = uiMemType;
			}
		}

		FORCEINLINE unsigned int GetMemType()const
		{
			return m_uiMemType;
		}
		FORCEINLINE unsigned int GetSwapChainNum()const
		{
			return m_uiSwapChainNum;
		}
	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID) = 0;		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID) = 0;
		
	};
	DECLARE_Ptr(SKBind);
	SKTYPE_MARCO(SKBind);

}
#endif