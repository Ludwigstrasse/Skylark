#ifndef SKGRAPHIC_H
#define SKGRAPHIC_H
#ifdef SKGRAPHIC_EXPORTS
#define  SKGRAPHIC_API __declspec(dllexport) 
#else
#define  SKGRAPHIC_API __declspec(dllimport)
#endif
#define PROFILER
namespace SKEngine2
{
#define TEXLEVEL 16 
	class SKGRAPHIC_API SKEngineFlag
	{
	public:
		static bool IsRunEditor;
		static bool EnableCLODMesh;
	};
}
#endif