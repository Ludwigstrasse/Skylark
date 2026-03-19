#ifndef SKANIMSEQUENCE_H
#define SKANIMSEQUENCE_H
#include "SKObject.h"
#include "SKVector3.h"
#include "SKQuat.h"
#include "SKMatrix3X3W.h"
#include "SKName.h"
#include "SKController.h"
#include "SKResource.h"
#include <SKDelegateList.h>
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKKeyTimeInfo
	{
	public:
		SKKeyTimeInfo(){};
		~SKKeyTimeInfo(){};
		SKREAL m_dKeyTime;
	};
	class SKGRAPHIC_API SKKeyTimeVector : public SKKeyTimeInfo
	{
	public:
		SKKeyTimeVector(){};
		~SKKeyTimeVector(){};
		SKVector3 m_Vector;
	};
	class SKGRAPHIC_API SKKeyTimeReal	:public SKKeyTimeInfo
	{
	public:
		SKKeyTimeReal(){}
		~SKKeyTimeReal(){}
		SKREAL	m_Real;
	};
	class SKGRAPHIC_API SKKeyTimeQuaternion : public SKKeyTimeInfo
	{
	public:
		SKKeyTimeQuaternion(){};
		~SKKeyTimeQuaternion(){};	
		SKQuat m_Quat;

	};

	class SKGRAPHIC_API SKKeyTimeVectorCompress : public SKKeyTimeInfo
	{
	public:
		SKKeyTimeVectorCompress(){};
		~SKKeyTimeVectorCompress(){};
		unsigned short m_X;
		unsigned short m_Y;
		unsigned short m_Z;
	};
	class SKGRAPHIC_API SKKeyTimeRealCompress	:public SKKeyTimeInfo
	{
	public:
		SKKeyTimeRealCompress(){}
		~SKKeyTimeRealCompress(){}
		unsigned short m_Real;
	};
	class SKGRAPHIC_API SKKeyTimeQuaternionCompress : public SKKeyTimeInfo
	{
	public:
		SKKeyTimeQuaternionCompress(){};
		~SKKeyTimeQuaternionCompress(){};	
		unsigned short m_X;
		unsigned short m_Y;
		unsigned short m_Z;
		unsigned short m_W;
	};



	DECLARE_Ptr(SKBoneKeyCompress);
	class SKGRAPHIC_API SKBoneKey : public SKObject
	{
		//RTTI
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		virtual ~SKBoneKey();

	public:
		SKBoneKey();
		SKArray<SKKeyTimeVector>			m_TranslationArray;
		SKArray<SKKeyTimeVector>			m_ScaleArray;
		SKArray<SKKeyTimeQuaternion>		m_RotatorArray;
		SKUsedName							m_cName;
		void CompressSameFrame();
		void Get(SKBoneKeyCompress * pBoneKeyCompress,
			const SKVector3 & MaxTranslation , const SKVector3 & MinTranslation ,
			const SKVector3 MaxScale,const SKVector3 MinScale);
	};
	DECLARE_Ptr(SKBoneKey);
	SKTYPE_MARCO(SKBoneKey);
	
	class SKGRAPHIC_API SKBoneKeyCompress : public SKObject
	{
		//RTTI
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL

	public:
		virtual ~SKBoneKeyCompress();

	public:
		SKBoneKeyCompress();
		SKArray<SKKeyTimeVectorCompress>			m_TranslationArray;
		SKArray<SKKeyTimeVectorCompress>			m_ScaleArray;
		SKArray<SKKeyTimeQuaternionCompress>		m_RotatorArray;
		SKUsedName									m_cName;
		void Get(SKBoneKey * pBoneKey,
			const SKVector3 & MaxTranslation , const SKVector3 & MinTranslation ,
			const SKVector3 MaxScale,const SKVector3 MinScale);
	};
	DECLARE_Ptr(SKBoneKeyCompress);
	SKTYPE_MARCO(SKBoneKeyCompress);

	
	class SKAnim;
	DECLARE_Proxy(SKAnim);
	class SKGRAPHIC_API SKAnim : public SKObject,public SKResource
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		virtual ~SKAnim();

	public:
		friend class SKResourceManager;
		SKAnim();
		virtual unsigned int GetResourceType()const
		{

			return RT_ACTION;
		}
		FORCEINLINE bool IsAdditive()const
		{
			return m_pBlendAnim != NULL;
		}
		FORCEINLINE bool IsRootAnim()const
		{
			return m_bRootAnimPlay;
		}

		FORCEINLINE void SetRootMotion(bool bRootAnimPlay)
		{
			m_bRootAnimPlay = bRootAnimPlay;
		}

		SKAnimR * GetBlendAnim()const
		{
			return m_pBlendAnim;
		}
	protected:
		SKArray<SKBoneKeyPtr> m_pBoneKeyArray;
		SKArray<SKBoneKeyCompressPtr> m_pBoneKeyCompressArray;
		SKREAL m_fLength;
		bool m_bCompress;
		SKVector3 m_MaxCompressTranslation;
		SKVector3 m_MinCompressTranslation;
		SKVector3 m_MaxCompressScale;
		SKVector3 m_MinCompressScale;
		bool m_bRootAnimPlay;
		SKAnimRPtr m_pBlendAnim;
	public:
		// only use fbx
		void Compress();
		void ComputeAnimLength();
		FORCEINLINE SKREAL GetAnimLength()const
		{
			return m_fLength;
		}
		void AddBoneKey(SKBoneKey * pBoneKey);
		SKBoneKey * GetBoneKey(const SKUsedName & AnimName)const;
		SKBoneKey * GetBoneKey(unsigned int uiIndex)const;
		FORCEINLINE unsigned int GetBoneKeyNum()const
		{
			return m_pBoneKeyArray.GetNum();
		}
		virtual bool PostLoad(void * pDate = NULL);
		SKUsedName m_cName;

		SKVector3 GetTranslation(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;
		SKVector3 GetScale(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;
		SKQuat	  GetQuat(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;
		SKMatrix3X3W GetMat(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;


		SKVector3 GetTranslation(unsigned int i,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;
		SKVector3 GetScale(unsigned int i,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;
		SKQuat	  GetQuat(unsigned int i,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;
		SKMatrix3X3W GetMat(unsigned int i,SKREAL fTime,unsigned int uiRepeatType = SKController::RT_CLAMP)const;
	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
		static const SKAnim *GetDefalut()
		{
			return Default;
		}
		static SKPointer<SKAnim> Default;
	};
	DECLARE_Ptr(SKAnim);
	SKTYPE_MARCO(SKAnim);
	DECLARE_Proxy(SKAnim);
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef SKDelegateEvent<void(void)> AddAnimEventType;
#else
	typedef SKDelegateEvent0<void> AddAnimEventType;
#endif
	
	class SKGRAPHIC_API SKAnimSet : public SKObject
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		virtual ~SKAnimSet();
	public:
		SKAnimSet();
	protected:
		SKMapOrder<SKUsedName,SKAnimRPtr> m_pAnimArray;

 	public:
		
		void AddAnim(SKUsedName AnimName,SKAnimR * pAnim);
		SKAnimR * GetAnim(const SKUsedName & AnimName)const;
		SKAnimR * GetAnim(unsigned int i)const;
		FORCEINLINE unsigned int GetAnimNum()const
		{
			return m_pAnimArray.GetNum();
		}



		AddAnimEventType m_AddAnimEvent;
		
	
		
		
	};
	DECLARE_Ptr(SKAnimSet);
	SKTYPE_MARCO(SKAnimSet);
	
}
#endif