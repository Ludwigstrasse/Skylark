#ifndef SKRENDERSTATE_H
#define SKRENDERSTATE_H
#include "SKDepthStencilState.h"
#include "SKBlendState.h"
#include "SKRasterizerState.h"
#include "SKPlane3.h"
#include "SKArray.h"
#include "SKRect2.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKRenderState : public SKObject
	{
		friend class SKSpatial;
		friend class SKRenderer;
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Inherit Flag
		{
			IF_WIRE_ENABLE = 0X0000001,
			IF_CULL_TYPE = 0X0000002,

			IF_ALL = 0XFFFFFFFF
		};
		SKRenderState()
		{
			m_pBlendState = (SKBlendState *)SKBlendState::GetDefault();
			m_pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
			m_pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
			m_RectArray.Clear();
			m_Plane.Clear();
		}
		~SKRenderState()
		{
			m_pBlendState = NULL;
			m_pDepthStencilState = NULL;
			m_pRasterizerState = NULL;
		}

		const SKRenderState & operator =(const SKRenderState &RenderState)
		{
			GetAll(&RenderState);
			return *this;
		}
		const SKRenderState & operator =(const SKRenderState *RenderState)
		{
			GetAll(RenderState);
			return *this;
		}
		void GetAll(const SKRenderState * pRenderState)
		{

			SKMAC_ASSERT(pRenderState);
			if (pRenderState)
			{
				SetRasterizerState(pRenderState->m_pRasterizerState);
				SetDepthStencilState(pRenderState->m_pDepthStencilState);
				SetBlendState(pRenderState->m_pBlendState);
				m_RectArray = pRenderState->m_RectArray;
				m_Plane = pRenderState->m_Plane;
			}
			else
			{
				ClearState();
			}
		}
		void Inherit(const SKRenderState * pRenderState,unsigned int uiInheritFlag = 0);
		void SwapCull();
		
		void AddRect(const SKRect2 & Rect)
		{
			m_RectArray.AddElement(Rect);
		}
		void ClearRect()
		{
			m_RectArray.Clear();
		}

		void AddPlane(const SKPlane3 & Plane)
		{
			m_Plane.AddElement(Plane);
		}
		void ClearPlane()
		{
			m_Plane.Clear();
		}

		void ClearState()
		{	
			m_pDepthStencilState = NULL;
			m_pBlendState = NULL;
			m_pRasterizerState = NULL;
			m_RectArray.Clear();
			m_Plane.Clear();
		}
		void SetBlendState(SKBlendState * pBlendState)
		{
			if (!pBlendState)
			{
				pBlendState = (SKBlendState *)SKBlendState::GetDefault();
			}
			if (m_pBlendState != pBlendState)
			{
				m_pBlendState = pBlendState;
				m_BlendDesc = pBlendState->GetBlendDesc();
			}

		}
		void SetDepthStencilState(SKDepthStencilState *pDepthStencilState)
		{
			if (!pDepthStencilState)
			{
				pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
			}
			if (m_pDepthStencilState != pDepthStencilState)
			{
				m_pDepthStencilState = pDepthStencilState;
				m_DepthStencilDesc = pDepthStencilState->GetDepthStencilDesc();
			}
		}
		void SetRasterizerState(SKRasterizerState *pRasterizerState)
		{
			if (!pRasterizerState)
			{
				pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
			}
			if (m_pRasterizerState != pRasterizerState)
			{
				m_pRasterizerState = pRasterizerState;
				m_RasterizerDesc = pRasterizerState->GetRasterizerDesc();
			}
		}
		const SKDepthStencilState * GetDepthStencilState()const
		{
			return m_pDepthStencilState;
		}
		const SKBlendState * GetBlendState()const
		{
			return m_pBlendState;
		}
		const SKRasterizerState * GetRasterizerState()const
		{
			return m_pRasterizerState;
		}
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
	protected:
		SKDepthStencilStatePtr	m_pDepthStencilState;
		SKBlendStatePtr			m_pBlendState;
		SKRasterizerStatePtr	m_pRasterizerState;

		SKDepthStencilDesc m_DepthStencilDesc;
		SKRasterizerDesc m_RasterizerDesc;
		SKBlendDesc m_BlendDesc;

		SKArray<SKRect2> m_RectArray;
		SKArray<SKPlane3> m_Plane;
	};
	SKTYPE_MARCO(SKRenderState);
	DECLARE_Ptr(SKRenderState);
}
#endif