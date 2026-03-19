#include "SKAnimSet.h"
#include "SKResourceManager.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKBoneKey,SKObject)
BEGIN_ADD_PROPERTY(SKBoneKey,SKObject)
REGISTER_PROPERTY(m_TranslationArray,TranslationArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ScaleArray,ScaleArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotatorArray,RotatorArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_cName,BoneName,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKBoneKey)
IMPLEMENT_INITIAL_END
SKBoneKey::SKBoneKey()
{
}
SKBoneKey:: ~SKBoneKey()
{
}
void SKBoneKey::CompressSameFrame()
{
	// process translation
	if (m_TranslationArray.GetNum() >= 2)
	{
		SKArray<SKKeyTimeVector> NewTranslationArray;
		SKKeyTimeVector Fisrt = m_TranslationArray[0];
		NewTranslationArray.AddElement(Fisrt);
		unsigned int Index = 0;
		for (unsigned int i = 1 ; i < m_TranslationArray.GetNum() ; i++)
		{
			if (i != m_TranslationArray.GetNum() - 1)
			{			
				if (Fisrt.m_Vector == m_TranslationArray[i].m_Vector)
				{
					continue;
				}
			}
			
			if (Index != i - 1)
			{
				NewTranslationArray.AddElement(m_TranslationArray[i - 1]);
			}
			NewTranslationArray.AddElement(m_TranslationArray[i]);
			Fisrt = m_TranslationArray[i];
			Index = i;
		}

		m_TranslationArray = NewTranslationArray;
	}

	// process scale
	if (m_ScaleArray.GetNum() >= 2)
	{
		SKArray<SKKeyTimeVector> NewScaleArray;
		SKKeyTimeVector Fisrt = m_ScaleArray[0];
		NewScaleArray.AddElement(Fisrt);
		unsigned int Index = 0;
		for (unsigned int i = 1 ; i < m_ScaleArray.GetNum(); i++)
		{
			if (i != m_ScaleArray.GetNum() - 1)
			{			
				if (Fisrt.m_Vector == m_ScaleArray[i].m_Vector)
				{
					continue;
				}
			}

			if (Index != i - 1)
			{
				NewScaleArray.AddElement(m_ScaleArray[i - 1]);
			}
			NewScaleArray.AddElement(m_ScaleArray[i]);
			Fisrt = m_ScaleArray[i];
			Index = i;
		}

		m_ScaleArray = NewScaleArray;
	}
	// process Rotator
	if (m_RotatorArray.GetNum() >= 2)
	{
		SKArray<SKKeyTimeQuaternion> NewRotatorArray;
		SKKeyTimeQuaternion Fisrt = m_RotatorArray[0];
		NewRotatorArray.AddElement(Fisrt);
		unsigned int Index = 0;
		for (unsigned int i = 1 ; i < m_RotatorArray.GetNum(); i++)
		{
			if (i != m_RotatorArray.GetNum() - 1)
			{			
				if (Fisrt.m_Quat == m_RotatorArray[i].m_Quat)
				{
					continue;
				}
			}

			if (Index != i - 1)
			{
				NewRotatorArray.AddElement(m_RotatorArray[i - 1]);
			}
			NewRotatorArray.AddElement(m_RotatorArray[i]);
			Fisrt = m_RotatorArray[i];
			Index = i;
		}

		m_RotatorArray = NewRotatorArray;
	}
	
}
void SKBoneKey::Get(SKBoneKeyCompress * pBoneKeyCompress,
		 const SKVector3 & MaxTranslation , const SKVector3 & MinTranslation ,
		 const SKVector3 MaxScale,const SKVector3 MinScale)
{
	pBoneKeyCompress->m_cName = m_cName;
	for (unsigned int i = 0 ; i < m_ScaleArray.GetNum() ; i++)
	{
		SKKeyTimeVectorCompress Compress;
		Compress.m_dKeyTime = m_ScaleArray[i].m_dKeyTime;
		Compress.m_X = CompressFloat(m_ScaleArray[i].m_Vector.x,MaxScale.x,MinScale.x);
		Compress.m_Y = CompressFloat(m_ScaleArray[i].m_Vector.y,MaxScale.y,MinScale.y);
		Compress.m_Z = CompressFloat(m_ScaleArray[i].m_Vector.z,MaxScale.z,MinScale.z);
		pBoneKeyCompress->m_ScaleArray.AddElement(Compress);
	}

	for (unsigned int i = 0 ; i < m_TranslationArray.GetNum() ; i++)
	{
		SKKeyTimeVectorCompress Compress;
		Compress.m_dKeyTime = m_TranslationArray[i].m_dKeyTime;
		Compress.m_X = CompressFloat(m_TranslationArray[i].m_Vector.x,MaxTranslation.x,MinTranslation.x);
		Compress.m_Y = CompressFloat(m_TranslationArray[i].m_Vector.y,MaxTranslation.y,MinTranslation.y);
		Compress.m_Z = CompressFloat(m_TranslationArray[i].m_Vector.z,MaxTranslation.z,MinTranslation.z);
		pBoneKeyCompress->m_TranslationArray.AddElement(Compress);
	}

	for (unsigned int i = 0 ; i < m_RotatorArray.GetNum() ; i++)
	{
		SKKeyTimeQuaternionCompress Compress;
		Compress.m_dKeyTime = m_RotatorArray[i].m_dKeyTime;
		Compress.m_X = CompressFloat(m_RotatorArray[i].m_Quat.x,1.0f,-1.0f);
		Compress.m_Y = CompressFloat(m_RotatorArray[i].m_Quat.y,1.0f,-1.0f);
		Compress.m_Z = CompressFloat(m_RotatorArray[i].m_Quat.z,1.0f,-1.0f);
		Compress.m_W = CompressFloat(m_RotatorArray[i].m_Quat.w,1.0f,-1.0f);
		pBoneKeyCompress->m_RotatorArray.AddElement(Compress);
	}
}
IMPLEMENT_RTTI(SKBoneKeyCompress,SKObject)
BEGIN_ADD_PROPERTY(SKBoneKeyCompress,SKObject)
REGISTER_PROPERTY(m_TranslationArray,TranslationArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ScaleArray,ScaleArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotatorArray,RotatorArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_cName,BoneName,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKBoneKeyCompress)
IMPLEMENT_INITIAL_END
SKBoneKeyCompress::SKBoneKeyCompress()
{
}
SKBoneKeyCompress:: ~SKBoneKeyCompress()
{
}
void SKBoneKeyCompress::Get(SKBoneKey * pBoneKey,
					const SKVector3 & MaxTranslation , const SKVector3 & MinTranslation ,
					const SKVector3 MaxScale,const SKVector3 MinScale)
{
	pBoneKey->m_cName = m_cName;
	for (unsigned int i = 0 ; i < m_ScaleArray.GetNum() ; i++)
	{
		SKKeyTimeVector Decompress;
		Decompress.m_dKeyTime = m_ScaleArray[i].m_dKeyTime;
		Decompress.m_Vector.x = DecompressFloat(m_ScaleArray[i].m_X,MaxScale.x,MinScale.x);
		Decompress.m_Vector.y = DecompressFloat(m_ScaleArray[i].m_Y,MaxScale.y,MinScale.y);
		Decompress.m_Vector.z = DecompressFloat(m_ScaleArray[i].m_Z,MaxScale.z,MinScale.z);
		pBoneKey->m_ScaleArray.AddElement(Decompress);
	}

	for (unsigned int i = 0 ; i < m_TranslationArray.GetNum() ; i++)
	{
		SKKeyTimeVector Decompress;
		Decompress.m_dKeyTime = m_TranslationArray[i].m_dKeyTime;
		Decompress.m_Vector.x = DecompressFloat(m_TranslationArray[i].m_X,MaxTranslation.x,MinTranslation.x);
		Decompress.m_Vector.y = DecompressFloat(m_TranslationArray[i].m_Y,MaxTranslation.y,MinTranslation.y);
		Decompress.m_Vector.z = DecompressFloat(m_TranslationArray[i].m_Z,MaxTranslation.z,MinTranslation.z);
		pBoneKey->m_TranslationArray.AddElement(Decompress);
	}

	for (unsigned int i = 0 ; i < m_RotatorArray.GetNum() ; i++)
	{
		SKKeyTimeQuaternion Decompress;
		Decompress.m_dKeyTime = m_RotatorArray[i].m_dKeyTime;
		Decompress.m_Quat.x = DecompressFloat(m_RotatorArray[i].m_X,1.0f,-1.0f);
		Decompress.m_Quat.y = DecompressFloat(m_RotatorArray[i].m_Y,1.0f,-1.0f);
		Decompress.m_Quat.z = DecompressFloat(m_RotatorArray[i].m_Z,1.0f,-1.0f);
		Decompress.m_Quat.w = DecompressFloat(m_RotatorArray[i].m_W,1.0f,-1.0f);
		pBoneKey->m_RotatorArray.AddElement(Decompress);
	}
}
bool SKAnim::ms_bIsEnableASYNLoader = true;
bool SKAnim::ms_bIsEnableGC = true;
SKAnimPtr SKAnim::Default = NULL;
IMPLEMENT_RTTI(SKAnim,SKObject)
BEGIN_ADD_PROPERTY(SKAnim,SKObject)
REGISTER_PROPERTY(m_fLength,Length,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pBoneKeyArray,BoneKeyArray,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pBoneKeyCompressArray, BoneKeyCompressArray, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bCompress,Compress,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MaxCompressTranslation,MaxCompressTranslation,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MinCompressTranslation,MinCompressTranslation,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MinCompressScale,MinCompressScale,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MaxCompressScale,MaxCompressScale,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pBlendAnim, BlendAnim, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bRootAnimPlay, bRootAnimPlay, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnim)
IMPLEMENT_INITIAL_END
SKAnim::SKAnim()
{
	m_pBoneKeyArray.Clear();
	m_fLength = 0.0f;
	m_bCompress = false;
	m_MaxCompressTranslation = SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	m_MinCompressTranslation = SKVector3(-SKMAX_REAL,-SKMAX_REAL,-SKMAX_REAL);
	m_MaxCompressScale = SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	m_MinCompressScale = SKVector3(-SKMAX_REAL,-SKMAX_REAL,-SKMAX_REAL);
	m_pBlendAnim = NULL;
	m_bRootAnimPlay = false;
}
SKAnim:: ~SKAnim()
{
	m_pBoneKeyArray.Clear();
}
void SKAnim::ComputeAnimLength()
{
	if (m_pBlendAnim)
	{
		while (!m_pBlendAnim->IsLoaded())
		{

		}
		SKAnim * pBlend = m_pBlendAnim->GetResource();
		m_fLength = pBlend->GetAnimLength();
	}
	else
	{
		m_fLength = 0.0f;
		for (unsigned int i = 0; i < m_pBoneKeyArray.GetNum(); i++)
		{
			SKBoneKey * pBoneKey = m_pBoneKeyArray[i];
			for (unsigned int j = 0; j < pBoneKey->m_ScaleArray.GetNum(); j++)
			{
				if (m_fLength < pBoneKey->m_ScaleArray[j].m_dKeyTime)
					m_fLength = (SKREAL)pBoneKey->m_ScaleArray[j].m_dKeyTime;
			}
			for (unsigned int j = 0; j < pBoneKey->m_TranslationArray.GetNum(); j++)
			{
				if (m_fLength < pBoneKey->m_TranslationArray[j].m_dKeyTime)
					m_fLength = (SKREAL)pBoneKey->m_TranslationArray[j].m_dKeyTime;
			}
			for (unsigned int j = 0; j < pBoneKey->m_RotatorArray.GetNum(); j++)
			{
				if (m_fLength < pBoneKey->m_RotatorArray[j].m_dKeyTime)
					m_fLength = (SKREAL)pBoneKey->m_RotatorArray[j].m_dKeyTime;
			}
		}
		if (m_fLength < 0.0f)
		{
			m_fLength = 0.0f;
		}
	}
}
void SKAnim::AddBoneKey(SKBoneKey * pBoneKey)
{
	if(pBoneKey)
	{
		m_pBoneKeyArray.AddElement(pBoneKey);
	}
}
void SKAnim::Compress()
{
	if (!m_pBoneKeyArray.GetNum())
	{
		return ;
	}
	m_bCompress = true;
	m_pBoneKeyCompressArray.Clear();

	m_MaxCompressScale = SKVector3(-SKMAX_REAL,-SKMAX_REAL,-SKMAX_REAL);
	m_MaxCompressTranslation = SKVector3(-SKMAX_REAL,-SKMAX_REAL,-SKMAX_REAL);

	m_MinCompressScale = SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	m_MinCompressTranslation = SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);

	for (unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ;i++)
	{
		for (unsigned int j = 0 ; j < m_pBoneKeyArray[i]->m_ScaleArray.GetNum() ; j++)
		{
			for (unsigned int k = 0 ; k < 3 ; k++)
			{
				if (m_MaxCompressScale.m[k] < m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k])
				{
					m_MaxCompressScale.m[k] = m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k];
				}

				if (m_MinCompressScale.m[k] > m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k])
				{
					m_MinCompressScale.m[k] = m_pBoneKeyArray[i]->m_ScaleArray[j].m_Vector.m[k];
				}
			}
			
		}
		
		for (unsigned int j = 0 ; j < m_pBoneKeyArray[i]->m_TranslationArray.GetNum() ; j++)
		{
			for (unsigned int k = 0 ; k < 3 ; k++)
			{
				if (m_MaxCompressTranslation.m[k] < m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k])
				{
					m_MaxCompressTranslation.m[k] = m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k];
				}

				if (m_MinCompressTranslation.m[k] > m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k])
				{
					m_MinCompressTranslation.m[k] = m_pBoneKeyArray[i]->m_TranslationArray[j].m_Vector.m[k];
				}
			}

		}
	}
	for (unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ; i++)
	{
		m_pBoneKeyArray[i]->CompressSameFrame();
	}
	for (unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ; i++)
	{
		SKBoneKeyCompress * pBoneKeyCompress = SK_NEW SKBoneKeyCompress();
		m_pBoneKeyArray[i]->Get(pBoneKeyCompress,m_MaxCompressTranslation,m_MinCompressTranslation,m_MaxCompressScale,m_MinCompressScale);
		m_pBoneKeyCompressArray.AddElement(pBoneKeyCompress);
	}
	m_pBoneKeyArray.Destroy();

}
bool SKAnim::PostLoad(void * pDate)
{
	SKObject::PostLoad(pDate);
	if (m_bCompress)
	{
		if (m_pBoneKeyCompressArray.GetNum() ==0 || m_pBoneKeyArray.GetNum() > 0)
		{
			SKMAC_ASSERT(0);
			return false;
		}
		for (unsigned int i = 0 ; i < m_pBoneKeyCompressArray.GetNum() ; i++)
		{
			SKBoneKey * pBoneKey = SK_NEW SKBoneKey();
			m_pBoneKeyCompressArray[i]->Get(pBoneKey,m_MaxCompressTranslation,m_MinCompressTranslation,m_MaxCompressScale,m_MinCompressScale);
			m_pBoneKeyArray.AddElement(pBoneKey);
		}
		m_pBoneKeyCompressArray.Destroy();
	}
	return true;
}
SKBoneKey * SKAnim::GetBoneKey(const SKUsedName & AnimName)const
{
	for(unsigned int i = 0 ; i < m_pBoneKeyArray.GetNum() ; i++)
		if(m_pBoneKeyArray[i])
		{
			if(m_pBoneKeyArray[i]->m_cName == AnimName)
				return m_pBoneKeyArray[i];
		}

		return NULL;

}
SKBoneKey * SKAnim::GetBoneKey(unsigned int i)const
{
	if(i >=  m_pBoneKeyArray.GetNum())
		return NULL;
	return m_pBoneKeyArray[i];
}
SKVector3 SKAnim::GetTranslation(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKBoneKey * pBoneKey = GetBoneKey(UseName);
	unsigned int uiKeyNum = pBoneKey->m_TranslationArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return SKVector3(0,0,0);
	}
	
	if (uiRepeatType == SKController::RT_NONE || uiRepeatType == SKController::RT_MAX)
	{
		uiRepeatType = SKController::RT_CLAMP;
	}
	SKREAL fNewTime = (SKREAL)SKController::GetTime(fTime,pBoneKey->m_TranslationArray[0].m_dKeyTime,
						pBoneKey->m_TranslationArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_TranslationArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}
	
	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_TranslationArray[uiIndex2].m_dKeyTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	SKREAL fFactor = (SKREAL)((fNewTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_TranslationArray[uiIndex1].m_Vector * (1.0f - fFactor) + pBoneKey->m_TranslationArray[uiIndex2].m_Vector * fFactor ;
}
SKVector3 SKAnim::GetScale(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKBoneKey * pBoneKey = GetBoneKey(UseName);
	unsigned int uiKeyNum = pBoneKey->m_ScaleArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return SKVector3(1.0f,1.0f,1.0f);
	}

	if (uiRepeatType == SKController::RT_NONE || uiRepeatType == SKController::RT_MAX)
	{
		uiRepeatType = SKController::RT_CLAMP;
	}
	SKREAL fNewTime = (SKREAL)SKController::GetTime(fTime,pBoneKey->m_ScaleArray[0].m_dKeyTime,
		pBoneKey->m_ScaleArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_ScaleArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_ScaleArray[uiIndex2].m_dKeyTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	SKREAL fFactor = (SKREAL)((fNewTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_ScaleArray[uiIndex1].m_Vector * (1.0f - fFactor) + pBoneKey->m_ScaleArray[uiIndex2].m_Vector * fFactor ;
}
SKQuat	  SKAnim::GetQuat(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKBoneKey * pBoneKey = GetBoneKey(UseName);
	unsigned int uiKeyNum = pBoneKey->m_RotatorArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return SKQuat();
	}

	if (uiRepeatType == SKController::RT_NONE || uiRepeatType == SKController::RT_MAX)
	{
		uiRepeatType = SKController::RT_CLAMP;
	}
	SKREAL fNewTime = (SKREAL)SKController::GetTime(fTime,pBoneKey->m_RotatorArray[0].m_dKeyTime,
		pBoneKey->m_RotatorArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_RotatorArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_RotatorArray[uiIndex2].m_dKeyTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	SKREAL fFactor = (SKREAL)((fNewTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	SKQuat Rotator;
#ifdef USE_ROTATOR_LINE_INTERPOLATION
	Rotator.Slerp(fFactor,pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat);
#else
	Rotator = LineInterpolation(pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat,fFactor);
#endif
	return Rotator;
}
SKMatrix3X3W SKAnim::GetMat(const SKUsedName & UseName,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKVector3 Scale = GetScale(UseName,fTime,uiRepeatType);
	SKQuat Rotator = GetQuat(UseName,fTime,uiRepeatType);
	SKVector3 Translate = GetTranslation(UseName,fTime,uiRepeatType);

	SKMatrix3X3W OutPut;

	SKMatrix3X3 mRotate;
	Rotator.GetMatrix(mRotate);
	SKMatrix3X3 Mat;
	Mat = SKMatrix3X3(	mRotate._00 * Scale.x,mRotate._01 * Scale.x,mRotate._02 * Scale.x,
						mRotate._10 * Scale.y,mRotate._11 * Scale.y,mRotate._12 * Scale.y,
						mRotate._20 * Scale.z,mRotate._21 * Scale.z,mRotate._22 * Scale.z);

	OutPut.AddTranslate(Translate);
	OutPut.Add3X3(Mat);
	return OutPut;	

}

SKVector3 SKAnim::GetTranslation(unsigned int uiIndex,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKBoneKey * pBoneKey = GetBoneKey(uiIndex);
	unsigned int uiKeyNum = pBoneKey->m_TranslationArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return SKVector3(0,0,0);
	}

	if (uiRepeatType == SKController::RT_NONE || uiRepeatType == SKController::RT_MAX)
	{
		uiRepeatType = SKController::RT_CLAMP;
	}
	SKREAL fNewTime = (SKREAL)SKController::GetTime(fTime,pBoneKey->m_TranslationArray[0].m_dKeyTime,
		pBoneKey->m_TranslationArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_TranslationArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_TranslationArray[uiIndex2].m_dKeyTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	SKREAL fFactor = (SKREAL)((fNewTime - pBoneKey->m_TranslationArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_TranslationArray[uiIndex1].m_Vector * (1 - fFactor) + pBoneKey->m_TranslationArray[uiIndex2].m_Vector * fFactor ;
}
SKVector3 SKAnim::GetScale(unsigned int uiIndex,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKBoneKey * pBoneKey = GetBoneKey(uiIndex);
	unsigned int uiKeyNum = pBoneKey->m_ScaleArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return SKVector3(1.0f,1.0f,1.0f);
	}

	if (uiRepeatType == SKController::RT_NONE || uiRepeatType == SKController::RT_MAX)
	{
		uiRepeatType = SKController::RT_CLAMP;
	}
	SKREAL fNewTime = (SKREAL)SKController::GetTime(fTime,pBoneKey->m_ScaleArray[0].m_dKeyTime,
		pBoneKey->m_ScaleArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_ScaleArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_ScaleArray[uiIndex2].m_dKeyTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	SKREAL fFactor = (SKREAL)((fNewTime - pBoneKey->m_ScaleArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	return pBoneKey->m_ScaleArray[uiIndex1].m_Vector * (1 - fFactor) + pBoneKey->m_ScaleArray[uiIndex2].m_Vector * fFactor ;
}
SKQuat	  SKAnim::GetQuat(unsigned int uiIndex,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKBoneKey * pBoneKey = GetBoneKey(uiIndex);
	unsigned int uiKeyNum = pBoneKey->m_RotatorArray.GetNum();
	if (!pBoneKey || !uiKeyNum)
	{
		return SKQuat();
	}

	if (uiRepeatType == SKController::RT_NONE || uiRepeatType == SKController::RT_MAX)
	{
		uiRepeatType = SKController::RT_CLAMP;
	}
	SKREAL fNewTime = (SKREAL)SKController::GetTime(fTime,pBoneKey->m_RotatorArray[0].m_dKeyTime,
		pBoneKey->m_RotatorArray[uiKeyNum - 1].m_dKeyTime,uiRepeatType);
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;
	for (unsigned int i = 0 ; i <  uiKeyNum ; i++)
	{
		if (fNewTime >= pBoneKey->m_RotatorArray[i].m_dKeyTime)
		{
			uiIndex1 = i;
		}
	}

	uiIndex2 = uiIndex1 + 1;

	if (uiIndex2 == uiKeyNum)
	{
		uiIndex2 = uiIndex1;
	}

	double dDiff = pBoneKey->m_RotatorArray[uiIndex2].m_dKeyTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime;
	if(dDiff <= 0.0)
		dDiff = 1.0;
	SKREAL fFactor = (SKREAL)((fNewTime - pBoneKey->m_RotatorArray[uiIndex1].m_dKeyTime )/dDiff);

	if(fFactor < 0)
		fFactor = 0;
	if(fFactor > 1.0f)
		fFactor = 1.0f;	
	SKQuat Rotator;
#ifdef USE_ROTATOR_LINE_INTERPOLATION
	Rotator.Slerp(fFactor,pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat);
#else	
	Rotator = LineInterpolation(pBoneKey->m_RotatorArray[uiIndex1].m_Quat,pBoneKey->m_RotatorArray[uiIndex2].m_Quat,fFactor);
#endif	
	return Rotator;
}
SKMatrix3X3W SKAnim::GetMat(unsigned int uiIndex,SKREAL fTime,unsigned int uiRepeatType)const
{
	SKVector3 Scale = GetScale(uiIndex,fTime,uiRepeatType);
	SKQuat Rotator = GetQuat(uiIndex,fTime,uiRepeatType);
	SKVector3 Translate = GetTranslation(uiIndex,fTime,uiRepeatType);

	SKMatrix3X3W OutPut;
	SKMatrix3X3 mRotate;
	Rotator.GetMatrix(mRotate);
	SKMatrix3X3 Mat;
	Mat = SKMatrix3X3(	mRotate._00 * Scale.x,mRotate._01 * Scale.x,mRotate._02 * Scale.x,
						mRotate._10 * Scale.y,mRotate._11 * Scale.y,mRotate._12 * Scale.y,
						mRotate._20 * Scale.z,mRotate._21 * Scale.z,mRotate._22 * Scale.z);

	OutPut.AddTranslate(Translate);
	OutPut.Add3X3(Mat);
	return OutPut;
	
}
/*******************************SKAnimSet*****************************************/
IMPLEMENT_RTTI(SKAnimSet,SKObject)
BEGIN_ADD_PROPERTY(SKAnimSet,SKObject)
REGISTER_PROPERTY(m_pAnimArray,AnimArray,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimSet)
IMPLEMENT_INITIAL_END
SKAnimSet::SKAnimSet()
{
	m_pAnimArray.Clear();
}
SKAnimSet:: ~SKAnimSet()
{
	m_pAnimArray.Clear();
	
}
void SKAnimSet::AddAnim(SKUsedName AnimName,SKAnimR * pAnim)
{
	if(!pAnim)
		return;
	m_pAnimArray.AddElement(AnimName,pAnim);
	m_AddAnimEvent();
}
SKAnimR * SKAnimSet::GetAnim(const SKUsedName & AnimName)const
{
	unsigned int Index = m_pAnimArray.Find(AnimName);
	if (Index < m_pAnimArray.GetNum())
	{
		return m_pAnimArray[Index].Value;
	}
	return NULL;
}
SKAnimR * SKAnimSet::GetAnim(unsigned int i)const
{
	if(i >= m_pAnimArray.GetNum())
		return NULL;
	return m_pAnimArray[i].Value;
}

