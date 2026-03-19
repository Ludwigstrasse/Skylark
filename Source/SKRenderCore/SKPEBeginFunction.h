#ifndef SKPEBEGINUNCTION_H
#define SKPEBEGINUNCTION_H
#include "SKPostEffectFunction.h"
namespace SKEngine2
{
	class SKPEBeginFunction : public SKPostEffectFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		enum
		{
			OUT_COLOR
		};
		SKPEBeginFunction(const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet);
		~SKPEBeginFunction();
		virtual SKRenderTarget * GetMainColorOutPutRenderTarget()
		{
			if (m_pBeginTargetArray && m_pBeginTargetArray->GetNum() > 0)
			{
				return (*m_pBeginTargetArray)[0];
			}
			return NULL;
		}
		virtual SKRenderTarget * GetMainColorInputPutRenderTarget()
		{
			if (m_pBeginTargetArray && m_pBeginTargetArray->GetNum() > 0)
			{
				return (*m_pBeginTargetArray)[0];
			}
			return NULL;
		}
		virtual SKRenderTarget * GetRenderTarget(const SKOutputNode * pPutNode)
		{
			for (unsigned int i = 0 ; i < m_pOutput.GetNum() ;i++)
			{
				if (pPutNode == m_pOutput[i])
				{
					if (m_pBeginTargetArray && i < m_pBeginTargetArray->GetNum())
					{
						return (*m_pBeginTargetArray)[i];
					}
					else
					{
						return NULL;
					}
				}
			}
			return NULL;
		}
		FORCEINLINE void SetPara(SKArray<SKRenderTarget *> * pBeginTargetArray)
		{
			m_pBeginTargetArray = pBeginTargetArray;
		}
		virtual bool SetEndTarget(SKRenderTarget *pEndTarget)
		{
			return false;
		}
	protected:
		SKPEBeginFunction();
		SKArray<SKRenderTarget *> * m_pBeginTargetArray;
		virtual SKPostEffectSceneRender * CreateSceneRender();
		virtual void OnDraw(SKCuller & Culler,double dAppTime);
	};
	DECLARE_Ptr(SKPEBeginFunction);
	SKTYPE_MARCO(SKPEBeginFunction);
}
#endif