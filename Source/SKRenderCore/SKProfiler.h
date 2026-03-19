#ifndef SKPROFILER_H
#define SKPROFILER_H
#include "SKString.h"
#include "SKArray.h"
#include "SKObject.h"
namespace SKEngine2
{
	class SKRenderer;
	class SKProfilerNode;
	struct ProfilerString
	{
		int iX;
		int iY;
		SKColorRGBA rColor;
		SKString sText;
	};
	class SKGRAPHIC_API SKProfiler
	{
	public:
		DECLARE_INITIAL_ONLY
		static void AddProfileNode(SKProfilerNode * pProfilerNode);
		static void Draw();
		static void Draw(SKRenderer * pRenderer);
		static void GetProfilerNameString(const SKString & Name, unsigned int uiLayer,SKString & OutString);
		static void ClearAll();
		static unsigned int ms_uiCurCount;
	protected:
		static SKArray<SKProfilerNode *> & GetProfilerNode()
		{
			static SKArray<SKProfilerNode *> m_pProfilerNodes;
			return m_pProfilerNodes;
		}
		
		static bool InitialDefaultState();
		static SKArray<SKProfilerNode *> m_pRoot;
		static TCHAR * ms_LayerFlag[10];
	};

	class SKGRAPHIC_API SKProfilerNode
	{
	public:
		SKProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		
		virtual void Clear() = 0;
		void AddChild(SKProfilerNode * pChild);
		void Draw( unsigned int uiLayer);
	protected:
		virtual void OnDraw( unsigned int uiLayer) = 0;
		friend class SKProfilerNodeTImer;
		friend class SKProfiler;
		
		SKProfilerNode * m_pParentNode;
		SKArray<SKProfilerNode*>m_pChildNode;
		SKString m_Name;
		SKString m_ParentName;
	};
	class SKGRAPHIC_API SKTimeProfilerNode : public SKProfilerNode
	{
	public:
		class SKGRAPHIC_API SKProfilerNodeTImer
		{
		public:
			SKProfilerNodeTImer(SKTimeProfilerNode * pProfilerNode);
			~SKProfilerNodeTImer();
		protected:
			SKTimeProfilerNode * m_pOwner;
			SKREAL m_fBeginTime;
		};
		SKTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
			m_fProfilerTime = 0.0f;
		}
		virtual void OnDraw(unsigned int uiLayer);
	protected:
		SKREAL m_fProfilerTime;
	};
	class SKGRAPHIC_API SKCountProfilerNode : public SKProfilerNode
	{
	public:
		class SKGRAPHIC_API SKProfilerNodeCounter
		{
		public:
			SKProfilerNodeCounter(SKCountProfilerNode * pProfilerNode, int uiCount);
		};
		SKCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
			m_uiCounter = 0;
		}
		virtual void OnDraw( unsigned int uiLayer);
	protected:
		int m_uiCounter;
	};
	class SKGRAPHIC_API SKNoClearCountProfilerNode : public SKCountProfilerNode
	{
	public:
		SKNoClearCountProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
		}
	};
	class SKGRAPHIC_API SKOnlyTimeProfilerNode : public SKProfilerNode
	{
	public:
		class SKGRAPHIC_API SKProfilerNodeOnlyTImer
		{
		public:
			SKProfilerNodeOnlyTImer(SKOnlyTimeProfilerNode * pProfilerNode, float fProfilerTime);
		};
		SKOnlyTimeProfilerNode(const TCHAR *Name, const TCHAR * ParentName = NULL);
		virtual void Clear()
		{
			m_fProfilerTime = 0.0f;
		}
		virtual void OnDraw( unsigned int uiLayer);
	protected:
		SKREAL m_fProfilerTime;
	};
#ifdef PROFILER
#define DECLEAR_TIME_PROFILENODE(Name,ParentName)  SKTimeProfilerNode TimeProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_TIME_PROFILE(Name) SKTimeProfilerNode::SKProfilerNodeTImer ProfilerNodeTimer_##Name(&TimeProfilerNode_##Name);

#define DECLEAR_COUNT_PROFILENODE(Name,ParentName)  SKCountProfilerNode CountProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define DECLEAR_NOCLEAR_COUNT_PROFILENODE(Name,ParentName)  SKNoClearCountProfilerNode CountProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_COUNT_PROFILE(Name,Count) SKCountProfilerNode::SKProfilerNodeCounter ProfilerNodeCounter_##Name(&CountProfilerNode_##Name,Count);

#define DECLEAR_ONLYTIME_PROFILENODE(Name,ParentName)  SKOnlyTimeProfilerNode OnlyTimeProfilerNode_##Name(_T(#Name),_T(#ParentName));
#define ADD_ONLYTIME_PROFILE(Name,fProfilerTime) SKOnlyTimeProfilerNode::SKProfilerNodeOnlyTImer ProfilerNodeOnlyTimer_##Name(&OnlyTimeProfilerNode_##Name,fProfilerTime);
#else
#define DECLEAR_TIME_PROFILENODE(Name,ParentName)  
#define ADD_TIME_PROFILE(Name) 

#define DECLEAR_COUNT_PROFILENODE(Name,ParentName)  
#define DECLEAR_NOCLEAR_COUNT_PROFILENODE(Name,ParentName)  
#define ADD_COUNT_PROFILE(Name,Count) 

#define DECLEAR_ONLYTIME_PROFILENODE(Name,ParentName)  
#define ADD_ONLYTIME_PROFILE(Name,fProfilerTime) 
#endif
}
#endif