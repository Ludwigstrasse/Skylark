#include "SKAnimSequenceFunc.h"
#include "SKBoneNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKAnimSequenceFunc,SKAnimFunction)
BEGIN_ADD_PROPERTY(SKAnimSequenceFunc,SKAnimFunction)
REGISTER_PROPERTY(m_AnimName,AnimName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bOnlyAddtiveOutput, OnlyAddtiveOutput, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAnimSequenceFunc)
IMPLEMENT_INITIAL_END
SKAnimSequenceFunc::SKAnimSequenceFunc(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
					:SKAnimFunction(ShowName,pAnimTree)
{

	

	SKString OutputName = _T("Output");
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::AVT_ANIM,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	m_pAnimR = NULL;
	m_LastKey.Clear();
	m_AdditiveLastKey.Clear();
	m_uiRepeatType = SKController::RT_WRAP;
	m_TriggerBeginStart += TriggerAnimEventType::Handler::FromMethod<SKAnimSequenceFunc, &SKAnimSequenceFunc::BeginStart>(this);
	m_bOnlyAddtiveOutput = false;
	
}
bool SKAnimSequenceFunc::SetObject(SKObject * pObject)
{
	if(SKAnimFunction::SetObject(pObject))
	{
		SKSkelecton * pSkelecton = GetSkelectonMeshNode()->GetSkelecton();
		SKMAC_ASSERT(pSkelecton);
		unsigned int BoneNum = pSkelecton->GetBoneNum();
		m_UsedBoneIndexInAnim.SetBufferNum(BoneNum);
		m_LastKey.SetBufferNum(BoneNum);
		SKMemset(&m_LastKey[0],0,sizeof(LAST_KEY_TYPE) * BoneNum);
		SetAnim(m_AnimName);
		return true;
	}

	return false;
}
SKAnimSequenceFunc::SKAnimSequenceFunc(SKSkelectonMeshNode * pSkelectonMeshNode)
{
	SKMAC_ASSERT(pSkelectonMeshNode);
	m_pObject = pSkelectonMeshNode;
	SKSkelecton * pSkelecton = GetSkelectonMeshNode()->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);


	unsigned int BoneNum = pSkelecton->GetBoneNum();
	m_UsedBoneIndexInAnim.SetBufferNum(BoneNum);

	m_BoneOutPut.SetBufferNum(BoneNum);
	for (unsigned int i = 0; i < m_BoneOutPut.GetNum(); i++)
	{
		m_BoneOutPut[i].FromTransfrom(pSkelecton->GetBoneNode(i)->GetLocalTransform());

	}

	m_pAnimR = NULL;
	m_LastKey.Clear();
	m_LastKey.SetBufferNum(BoneNum);
	SKMemset(&m_LastKey[0],0,sizeof(LAST_KEY_TYPE) * BoneNum);
	m_uiRepeatType = SKController::RT_WRAP;
	m_TriggerBeginStart += TriggerAnimEventType::Handler::FromMethod<SKAnimSequenceFunc, &SKAnimSequenceFunc::BeginStart>(this);
	m_bOnlyAddtiveOutput = false;
}
SKAnimSequenceFunc::SKAnimSequenceFunc()
{
	m_pAnimR = NULL;
	m_uiRepeatType = SKController::RT_WRAP;
	m_TriggerBeginStart += TriggerAnimEventType::Handler::FromMethod<SKAnimSequenceFunc, &SKAnimSequenceFunc::BeginStart>(this);
	m_bOnlyAddtiveOutput = false;
}
SKAnimSequenceFunc::~SKAnimSequenceFunc()
{
	m_pAnimR = NULL;
}
void SKAnimSequenceFunc::SetAnim(const SKUsedName&  AnimName)
{
	m_AnimName = AnimName;
	const SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	if (!pMesh)
	{
		return ;
	}
	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	if (!pSkelecton)
	{
		return;
	}
	const SKAnimSet * pAnimSet = pMesh->GetAnimSet();
	if (!pAnimSet)
	{
		return;
	}
	SKAnimRPtr pAnimR = pAnimSet->GetAnim(AnimName);
	if (!pAnimR)
		return ;
	if (m_pAnimR == pAnimR)
	{
		return;
	}
	m_pAnimR = pAnimR;
	m_pAnimR->AddLoadEventObject(this);
	m_bStart = false;
	
}
void SKAnimSequenceFunc::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	const SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	SKMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return ;
	}
	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return;
	}
	SKAnim * pAnim = m_pAnimR->GetResource();
	
	m_dMaxTime = pAnim->GetAnimLength();
	SKMemset(&m_LastKey[0],0,sizeof(LAST_KEY_TYPE) * m_LastKey.GetNum());

	for (unsigned int i = 0; i < m_UsedBoneIndexInAnim.GetNum(); i++)
	{
		SKBoneNode * pBone = pSkelecton->GetBoneNode(i);
		if (pBone)
		{
			bool bIsFound = false;
			for (unsigned int j = 0; j < pAnim->GetBoneKeyNum(); j++)
			{
				if (pBone->m_cName == pAnim->GetBoneKey(j)->m_cName)
				{
					m_UsedBoneIndexInAnim[i] = j;
					bIsFound = true;
					break;
				}
			}
			if (!bIsFound)
			{
				m_UsedBoneIndexInAnim[i] = SKMAX_INTEGER;
			}
		}

	}

	m_AdditiveLastKey.Destroy();
	m_AdditiveBoneOutPut.Destroy();
	if (pAnim->IsAdditive())
	{
		m_AdditiveLastKey.SetBufferNum(m_LastKey.GetNum());
		SKMemset(&m_AdditiveLastKey[0], 0, sizeof(LAST_KEY_TYPE)* m_AdditiveLastKey.GetNum());
		m_AdditiveBoneOutPut.SetBufferNum(m_UsedBoneIndexInAnim.GetNum());
		for (unsigned int i = 0; i < m_AdditiveBoneOutPut.GetNum();i++)
		{
			m_AdditiveBoneOutPut[i] = m_BoneOutPut[i];
		}
	}
	

}
void SKAnimSequenceFunc::ComputeAnim(SKAnim * pAnim, SKArray<LAST_KEY_TYPE> & LastKey, SKArray<SKAnimAtom> &BoneOutPut)
{
	for (unsigned int i = 0; i < m_UsedBoneIndexInAnim.GetNum(); i++)
	{
		if (m_UsedBoneIndexInAnim[i] != SKMAX_INTEGER)
		{
			SKBoneKey * pBoneKey = pAnim->GetBoneKey(m_UsedBoneIndexInAnim[i]);
			unsigned int ScaleNum = (unsigned int)pBoneKey->m_ScaleArray.GetNum();
			if (ScaleNum)
			{

				if (pBoneKey->m_ScaleArray[LastKey[i].uiLKScale].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKScale = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKScale; j < ScaleNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_ScaleArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == ScaleNum)
				{
					j = ScaleNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKScale = Key1;
				double dDiff = pBoneKey->m_ScaleArray[Key2].m_dKeyTime - pBoneKey->m_ScaleArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				SKREAL fFactor = (SKREAL)((m_dNowAnimTime - pBoneKey->m_ScaleArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				SKVector3 Scale = pBoneKey->m_ScaleArray[Key1].m_Vector * (1 - fFactor) + pBoneKey->m_ScaleArray[Key2].m_Vector * fFactor;
				BoneOutPut[i].m_fScale = Scale;

			}

			unsigned int RotatorNum = (unsigned int)pBoneKey->m_RotatorArray.GetNum();
			if (RotatorNum)
			{
				if (pBoneKey->m_RotatorArray[LastKey[i].uiLKRotator].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKRotator = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKRotator; j < RotatorNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_RotatorArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == RotatorNum)
				{
					j = RotatorNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKRotator = Key1;
				double dDiff = pBoneKey->m_RotatorArray[Key2].m_dKeyTime - pBoneKey->m_RotatorArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				SKREAL fFactor = (SKREAL)((m_dNowAnimTime - pBoneKey->m_RotatorArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				SKQuat Rotator;
#ifdef USE_ROTATOR_LINE_INTERPOLATION
				Rotator.Slerp(fFactor, pBoneKey->m_RotatorArray[Key1].m_Quat, pBoneKey->m_RotatorArray[Key2].m_Quat);
#else
				Rotator = LineInterpolation(pBoneKey->m_RotatorArray[Key1].m_Quat, pBoneKey->m_RotatorArray[Key2].m_Quat, fFactor);
#endif
				BoneOutPut[i].m_Rotator = Rotator;

			}

			unsigned int TranslationNum = (unsigned int)pBoneKey->m_TranslationArray.GetNum();
			if (TranslationNum)
			{
				if (pBoneKey->m_TranslationArray[LastKey[i].uiLKTranslation].m_dKeyTime > m_dNowAnimTime)
				{
					LastKey[i].uiLKTranslation = 0;
				}
				unsigned int j;
				for (j = LastKey[i].uiLKTranslation; j < TranslationNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->m_TranslationArray[j].m_dKeyTime)
					{
						break;
					}
				}
				if (j == TranslationNum)
				{
					j = TranslationNum - 1;
				}
				unsigned int Key1;
				unsigned int Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				LastKey[i].uiLKTranslation = Key1;
				double dDiff = pBoneKey->m_TranslationArray[Key2].m_dKeyTime - pBoneKey->m_TranslationArray[Key1].m_dKeyTime;
				if (dDiff <= 0.0)
					dDiff = 1.0;
				SKREAL fFactor = (SKREAL)((m_dNowAnimTime - pBoneKey->m_TranslationArray[Key1].m_dKeyTime) / dDiff);

				if (fFactor < 0)
					fFactor = 0;
				if (fFactor > 1.0f)
					fFactor = 1.0f;

				SKVector3 Translation = pBoneKey->m_TranslationArray[Key1].m_Vector * (1 - fFactor) + pBoneKey->m_TranslationArray[Key2].m_Vector * fFactor;

				BoneOutPut[i].m_Pos = Translation;

			}


		}
		else
		{
			// 			BoneOutPut[i].Identity();
			SKMAC_ASSERT(0);
		}

	}
}
bool SKAnimSequenceFunc::Update(double dAppTime)
{
	if(!SKAnimFunction::Update(dAppTime))
		return false;
	
	SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	SKMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return false;
	}
	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return false;
	}
	if (m_pAnimR == NULL)
	{
		return false;	
	}

	if (!m_pAnimR->IsLoaded())
	{
		return false;
	}

	
	
	SKAnim * pAnim = m_pAnimR->GetResource();
	SKAnim * pBlendAnim = NULL;
	if (pAnim->IsAdditive())
	{
		SKAnimR * pBlendAnimR = pAnim->GetBlendAnim();
		if (!pBlendAnimR->IsLoaded())
		{
			return false;
		}

		pBlendAnim = pBlendAnimR->GetResource();
	}
	
	ComputeAnim(pAnim, m_LastKey, m_BoneOutPut);

	if (pBlendAnim && !m_bOnlyAddtiveOutput)
	{
		ComputeAnim(pBlendAnim, m_AdditiveLastKey, m_AdditiveBoneOutPut);

		for (unsigned int i = 0; i < m_BoneOutPut.GetNum();i++)
		{
			m_BoneOutPut[i].AddTwo(m_AdditiveBoneOutPut[i]);
		}
	}
	if (pAnim->IsRootAnim())
	{

		SKMatrix3X3W CurTansform;
		m_BoneOutPut[0].GetMatrix(CurTansform);
		SKTransform SkelectonT = pSkelecton->GetLocalTransform();
		SKMatrix3X3W RootMatrix = SkelectonT.GetCombineInverse() * m_SaveStartRootBoneTransform.GetCombineInverse() * CurTansform  * SkelectonT.GetCombine() * m_SaveStartMeshComponetTransform.GetCombine();
		m_BoneOutPut[0].FromTransfrom(m_SaveStartRootBoneTransform);
		m_RootAtom.FromMatrix(RootMatrix);

	}
	else
	{
		SKSpatial * pMeshComponent = pMesh->GetParent();
		if (pMeshComponent)
		{
			m_RootAtom.FromTransfrom(pMeshComponent->GetLocalTransform());
		}
	}
	return true;

}

void SKAnimSequenceFunc::UpDateBone()
{
	if (!m_bEnable)
	{
		return;
	}
	SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	SKMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return ;
	}
	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return ;
	}
	if (!m_pAnimR || !m_pAnimR->IsLoaded())
	{
		return ;
	}
	SKAnim * pAnim = m_pAnimR->GetResource();
	SKSpatial * pMeshComponent = pMesh->GetParent();
	if (pMeshComponent)
	{

		SKMatrix3X3W RootMatrix;
		m_RootAtom.GetMatrix(RootMatrix);
		pMeshComponent->SetLocalMat(RootMatrix);

	}

	for(unsigned int i = 0 ; i < pSkelecton->GetBoneNum() ; i++)
	{
		SKBoneNode * pBone = pSkelecton->GetBoneNode(i);
		if(pBone)
		{
			//ǰû skelectonMesh ûAnimTree,addivtive
			//Ҳÿιͷ󶼻ӣanimtreeÿanimtreeϺ󣬻ùͷ
			//Ȼadditive ȥȷ.
			SKMatrix3X3W BoneOutMat;	
			m_BoneOutPut[i].GetMatrix(BoneOutMat);

			pBone->SetLocalMat(BoneOutMat);

			
		}
	}
	
}
void SKAnimSequenceFunc::BeginStart()
{
	SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	SKMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return;
	}
	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return;
	}
	SKBoneNode * pBone = pSkelecton->GetBoneNode(0);
	m_SaveStartRootBoneTransform = pBone->GetLocalTransform();
	SKSpatial * pMeshComponent = pMesh->GetParent();
	if (pMeshComponent)
	{

		m_SaveStartMeshComponetTransform = pMeshComponent->GetLocalTransform();
	}
}
SKREAL SKAnimSequenceFunc::GetAnimTime()
{
	if (!m_bEnable)
	{
		return 0.0f;
	}
	const SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	SKMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return 0.0f;
	}
	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return 0.0f;
	}
	if (!m_pAnimR || !m_pAnimR->IsLoaded())
	{
		return 0.0f;
	}
	SKAnim * pAnim = m_pAnimR->GetResource();
	return pAnim->GetAnimLength() * (SKREAL)m_dFrequency;
}