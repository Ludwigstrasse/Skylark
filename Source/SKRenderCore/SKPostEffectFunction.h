#ifndef SKPOSTEFFECTFUNCTION_H
#define SKPOSTEFFECTFUNCTION_H
#include "SKObject.h"
#include "SKPutNode.h"
#include "SKName.h"
#include "SKSceneRender.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKPostEffectFunction : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~SKPostEffectFunction() = 0;
		friend class SKPostEffectSet;
	protected:
		SKUsedName m_ShowName;
		SKPostEffectFunction(const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet);
		SKPostEffectFunction();
		SKArray<SKInputNode *> m_pInput;
		SKArray<SKOutputNode *> m_pOutput;
		SKPostEffectSet * m_pOwner;
		bool m_bIsVisited;
		bool m_bLastOne;
		FORCEINLINE void SetOwner(SKPostEffectSet * pOwner)
		{
			SKMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}



		SKPostEffectSceneRenderPtr m_pPostEffectRender;

		virtual SKPostEffectSceneRender * CreateSceneRender() = 0;
		virtual void OnDraw(SKCuller & Culler,double dAppTime) = 0;
	public:
		friend class SKPostEffectSet;
		

		bool ClearFlag();
		

		SKInputNode * GetInputNode(unsigned int uiNodeID)const;
		SKInputNode * GetInputNode(const SKString & NodeName)const;

		SKOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		SKOutputNode * GetOutputNode(const SKString & NodeName)const;

		FORCEINLINE const SKUsedName & GetShowName()const
		{
			return m_ShowName;
		}
		virtual SKRenderTarget * GetMainColorOutPutRenderTarget()
		{
			if (m_pOutput.GetNum() > 0)
			{
				if (m_pOutput[0])
				{
					return GetRenderTarget(m_pOutput[0]);
				}
			}
			return NULL;
		}
		virtual SKRenderTarget * GetMainColorInputRenderTarget()
		{
			if (m_pInput.GetNum() > 0)
			{
				if (m_pInput[0])
				{
					return GetRenderTarget(m_pInput[0]);
				}
			}
			return NULL;
		}
		//ȡԼڵ
		virtual SKRenderTarget * GetRenderTarget(const SKOutputNode * pPutNode)
		{
			for (unsigned int i = 0 ; i < m_pOutput.GetNum() ;i++)
			{
				if (pPutNode == m_pOutput[i])
				{
					return m_pPostEffectRender->GetRenderTarget(i);
				}
			}
			return NULL;
		}
		virtual bool SetEndTarget(SKRenderTarget *pEndTarget)
		{
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					SKPostEffectFunction *pPostEffectFunc = (SKPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if (pPostEffectFunc)
					{
						if (pPostEffectFunc->m_bEnable)
						{
							pPostEffectFunc->CreateSceneRender();
							pPostEffectFunc->m_pPostEffectRender->AddRenderTarget(pEndTarget);
							pPostEffectFunc->m_bLastOne = true;
							return true;
						}
						else
						{
							return pPostEffectFunc->SetEndTarget(pEndTarget);
						}

					}
				}
			}
			return false;
		}
		//ȡӵǸڵ
		virtual SKRenderTarget * GetRenderTarget(const SKInputNode * pPutNode)
		{
			if(pPutNode->GetOutputLink())
			{
				SKPostEffectFunction *pPostEffectFunc = (SKPostEffectFunction *)pPutNode->GetOutputLink()->GetOwner();
				if (pPostEffectFunc)
				{
					if (pPostEffectFunc->m_bEnable)
					{
						SKRenderTarget * pRenderTarget = pPostEffectFunc->GetRenderTarget(pPutNode->GetOutputLink());
						return pRenderTarget;
					}
					else
					{
						return pPostEffectFunc->GetMainColorInputRenderTarget();
					}
					
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}
		virtual void GetRT(unsigned int uiWidth,unsigned int uiHeight)
		{
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					SKPostEffectFunction *pPostEffectFunc = (SKPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if(pPostEffectFunc)
					{
						pPostEffectFunc->GetRT(uiWidth,uiHeight);
					}
				}
			}
			
			CreateSceneRender();
			if (m_pPostEffectRender && !m_bLastOne && m_bEnable)
			{
				m_pPostEffectRender->GetRT(uiWidth,uiHeight);
			}
			
		}
		virtual void DisableRT()
		{
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					SKPostEffectFunction *pPostEffectFunc = (SKPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if(pPostEffectFunc)
					{
						pPostEffectFunc->DisableRT();
					}
				}
			}

			CreateSceneRender();
			if (m_pPostEffectRender && !m_bLastOne && m_bEnable)
			{
				m_pPostEffectRender->DisableRT();
			}
		}
		virtual void Draw(SKCuller & Culler,double dAppTime)
		{
			if (m_bIsVisited == 1)
			{
				return ;
			}
			m_bIsVisited = 1;
			
			for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
			{
				if(m_pInput[i]->GetOutputLink())
				{
					SKPostEffectFunction *pPostEffectFunc = (SKPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
					if(pPostEffectFunc)
					{
						pPostEffectFunc->Draw(Culler,dAppTime);
					}
				}
			}
			if (m_bEnable)
			{
				CreateSceneRender();
				OnDraw(Culler,dAppTime);
			}
			
		}
	public:
		bool m_bEnable;
	};
	DECLARE_Ptr(SKPostEffectFunction);
	SKTYPE_MARCO(SKPostEffectFunction);

}
#endif