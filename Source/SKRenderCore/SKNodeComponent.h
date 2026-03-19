#ifndef SKNODECOMPONENT_H
#define SKNODECOMPONENT_H
#include "SKNode.h"

namespace SKEngine2
{
	class SKSocketNode;
	class SKGRAPHIC_API SKNodeComponent : public SKNode
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKNodeComponent();
		virtual ~SKNodeComponent();
		
		friend class SKSocketNode;
		virtual bool IsNeedDraw()
		{
			return false;
		}
		template<typename T>
		static T * CreateComponet();
		virtual void PostCreate(){};
		virtual void OnDestroy();
		void AttachParentSocket(const SKUsedName & AttackSocketName);
		virtual bool PostLoad(void * pDate = NULL);
		virtual bool PostClone(SKObject * pObjectSrc);
		virtual void UpdateTransform(double dAppTime);
		virtual void UpdateLightState(double dAppTime);
		virtual void UpdateCameraState(double dAppTime);
	protected:
		virtual void UpdateNodeAll(double dAppTime);
		SKUsedName m_AttachSocketName;
		SKSocketNode *  m_pAttachSocket;
	};
 	DECLARE_Ptr(SKNodeComponent);
 	SKTYPE_MARCO(SKNodeComponent);

	template<typename T>
	T * SKNodeComponent::CreateComponet()
	{
		
		if (T::ms_Type.IsDerived(SKNodeComponent::ms_Type))
		{
			T * Component = (T *)SKObject::GetInstance<T>();
			Component->PostCreate();
			return Component;
		}
		else
		{
			return NULL;
		}
	}
}
#endif
