#ifndef SKNODE_H
#define SKNODE_H
#include "SKSpatial.h"
namespace SKEngine2
{
	class SKSort;
	class SKGeometry;
	class SKStream;
	class SKNodeComponent;
	class SKGRAPHIC_API SKNode : public SKSpatial
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class SKMeshComponent;
		SKNode();
		virtual ~SKNode() = 0;
		FORCEINLINE unsigned int GetNodeNum()const;
		virtual unsigned int AddChild(SKSpatial * pChild);
		virtual unsigned int DeleteChild(SKSpatial *pChild);
		virtual bool DeleteChild(unsigned int i);
		SKSpatial * GetChild(unsigned int i)const;
		virtual void		DeleteAllChild();
		FORCEINLINE SKArray<SKSpatialPtr> *GetChildList();

	protected:
		
		virtual void UpdateWorldBound(double dAppTime);//߽ 

		virtual void UpdateNodeAll(double dAppTime);
		virtual void ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime);
	protected:
		SKArray<SKSpatialPtr>m_pChild;
		
		
	public:
		virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);
		virtual void CreateLocalAABB();
	};
	#include "SKNode.inl"
	DECLARE_Ptr(SKNode);
	SKTYPE_MARCO(SKNode);
}
#endif