#ifndef SKGEOMETRYNODE_H
#define SKGEOMETRYNODE_H
#include "SKNode.h"
#include "SKMorphSet.h"
#include "SKGeometry.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKGeometryNode : public SKNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKGeometryNode();
		virtual ~SKGeometryNode();
		void SetMorphSet(SKMorphSet * pMorphSet);
		FORCEINLINE const SKMorphSet *GetMorphSet()const
		{
			return m_pMorphSet;
		}
		SKGeometry * GetGeometry(unsigned int i);
		SKGeometry * GetNormalGeometry(unsigned int index);
		unsigned int GetNormalGeometryNum();
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
	protected:
		SKMorphSetPtr m_pMorphSet;
		void UpdateLocalAABB();
	};
	DECLARE_Ptr(SKGeometryNode);
	SKTYPE_MARCO(SKGeometryNode);
}
#endif