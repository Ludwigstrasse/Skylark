#ifndef SKSTATICMESHNODE_H
#define SKSTATICMESHNODE_H
#include "SKModelMeshNode.h"
namespace SKEngine2
{
	DECLARE_Proxy(SKStaticMeshNode);
	class SKGRAPHIC_API SKStaticMeshNode : public SKModelMeshNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKStaticMeshNode();
		virtual ~SKStaticMeshNode();
		
		virtual unsigned int GetResourceType()const
		{
			return RT_STATIC_MODEL;
		}
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		void AddLodMesh(SKStaticMeshNodeR * pStaticMeshResource);
		void SetLodMesh(unsigned int i, SKStaticMeshNodeR * pStaticMeshResource);
		void DeleteLodMesh(unsigned int i);
	protected:
		static SKPointer<SKStaticMeshNode> Default;
	public:
		static const SKStaticMeshNode *GetDefalut()
		{
			return Default;
		}
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;

	};
	DECLARE_Ptr(SKStaticMeshNode);
	SKTYPE_MARCO(SKStaticMeshNode);
	DECLARE_Proxy(SKStaticMeshNode);
}
#endif