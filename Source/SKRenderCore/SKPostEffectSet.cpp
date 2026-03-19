#include "SKPostEffectSet.h"
#include "SKPEBeginFunction.h"
#include "SKPEEndFunction.h"
#include "SKGraphicInclude.h"
#include "SKPEGray.h"
#include "SKPEMaterial.h"
#include "SKRenderTarget.h"
#include "SKStream.h"
using namespace SKEngine2;
SKPointer<SKPostEffectSet> SKPostEffectSet::Default = NULL;
bool SKPostEffectSet::ms_bIsEnableASYNLoader = false;
bool SKPostEffectSet::ms_bIsEnableGC = true;
IMPLEMENT_RTTI(SKPostEffectSet,SKObject)
BEGIN_ADD_PROPERTY(SKPostEffectSet,SKObject)
REGISTER_PROPERTY(m_ShowName,ShowName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPEBeginFunc,BeginFuntion,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPEEndFunc,EndFunction,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPostEffectFunctionArray,PostEffectFunctionArray,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPostEffectSet)
IMPLEMENT_INITIAL_END
SKPostEffectSet::SKPostEffectSet()
{
	m_pPostEffectFunctionArray.Clear();
}
SKPostEffectSet::SKPostEffectSet(const SKUsedName &ShowName)
{
	m_pPostEffectFunctionArray.Clear();
	m_ShowName = ShowName;
	m_pPEBeginFunc = SK_NEW SKPEBeginFunction(_T("Begin"),this);
	m_pPEEndFunc = SK_NEW SKPEEndFunction(_T("End"),this);
}
SKPostEffectSet::~SKPostEffectSet()
{
	for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ; i++)
	{
		SKPostEffectFunction * pShaderFunction = m_pPostEffectFunctionArray[i];
		m_pPostEffectFunctionArray[i] = NULL;
		SKMAC_DELETE(pShaderFunction);
	}
	m_pPostEffectFunctionArray.Clear();

	m_pPEBeginFunc = NULL;
	m_pPEEndFunc = NULL;
}
void SKPostEffectSet::AddPostEffectFunction(SKPostEffectFunction * pPostEffectFunction)
{
	if(pPostEffectFunction)
	{
		m_pPostEffectFunctionArray.AddElement(pPostEffectFunction);
	}

}
void SKPostEffectSet::DeletePostEffectFunction(SKPostEffectFunction * pPostEffectFunction)
{
	if(pPostEffectFunction)
	{
		for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ; i++)
		{
			if(m_pPostEffectFunctionArray[i] == pPostEffectFunction)
			{
				m_pPostEffectFunctionArray.Erase(i);
				return ;
			}
		}

	}

}

void SKPostEffectSet::Draw(SKCuller & Culler,double dAppTime)
{
	m_pPEEndFunc->ClearFlag();
	m_pPEEndFunc->Draw(Culler,dAppTime);
}
void SKPostEffectSet::SetBeginTargetArray(SKArray<SKRenderTarget *> * pBeginTargetArray)
{
	m_pPEBeginFunc->SetPara(pBeginTargetArray);
}
void SKPostEffectSet::GetRT(unsigned int uiWidth,unsigned int uiHeight)
{
	m_pPEEndFunc->GetRT(uiWidth,uiHeight);
}
void SKPostEffectSet::DisableRT()
{
	m_pPEEndFunc->DisableRT();
}
SKRenderTarget * SKPostEffectSet::GetEndTarget(unsigned int i)
{
	if (i < m_pPEEndFunc->m_pInput.GetNum())
	{
		SKRenderTarget * pTexture = m_pPEEndFunc->GetRenderTarget(m_pPEEndFunc->m_pInput[i]);
		return pTexture;
	}
	return NULL;
}
bool SKPostEffectSet::SetEndTarget(SKRenderTarget *pEndTarget)
{
	for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ;i++)
	{
		m_pPostEffectFunctionArray[i]->m_bLastOne = false;
	}
	return m_pPEEndFunc->SetEndTarget(pEndTarget);

}
SKPostEffectFunction * SKPostEffectSet::GetPEFunctionFromShowName(const SKUsedName & ShowName)
{
	for (unsigned int i = 0 ; i < m_pPostEffectFunctionArray.GetNum() ;i++)
	{
		if (m_pPostEffectFunctionArray[i]->GetShowName() == ShowName)
		{
			return m_pPostEffectFunctionArray[i];
		}
	}
	return NULL;
}
SKPESetGray::SKPESetGray(const SKUsedName &ShowName) : SKPostEffectSet(ShowName)
{
	SKPEGray * pPEGray = SK_NEW SKPEGray(_T("Gray"),this);
	m_pPEEndFunc->GetInputNode(SKPEEndFunction::INPUT_COLOR)->Connection(pPEGray->GetOutputNode(SKPEGray::OUT_COLOR));
	pPEGray->GetInputNode(SKPEGray::INPUT_COLOR)->Connection(m_pPEBeginFunc->GetOutputNode(SKPEBeginFunction::OUT_COLOR));
}
SKPESetGray::~SKPESetGray()
{
}

SKPESetMaterialAndGray::SKPESetMaterialAndGray(const SKUsedName &ShowName, SKMaterialR * pMaterial) : SKPostEffectSet(ShowName)
{
	SKPEGray * pPEGray = SK_NEW SKPEGray(_T("Gray"), this);
	SKPEMaterial * pPEMaterial = SK_NEW SKPEMaterial(_T("Material"),this);
	pPEMaterial->SetMaterial(pMaterial);
	m_pPEEndFunc->GetInputNode(SKPEEndFunction::INPUT_COLOR)->Connection(pPEMaterial->GetOutputNode(SKPEMaterial::OUT_COLOR));
	pPEMaterial->GetInputNode(SKPEMaterial::INPUT_COLOR)->Connection(pPEGray->GetOutputNode(SKPEGray::OUT_COLOR));
	pPEGray->GetInputNode(SKPEGray::INPUT_COLOR)->Connection(m_pPEBeginFunc->GetOutputNode(SKPEBeginFunction::OUT_COLOR));
}
SKPESetMaterialAndGray::~SKPESetMaterialAndGray()
{
}
