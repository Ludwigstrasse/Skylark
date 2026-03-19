#ifndef SKTYPE_H
#define SKTYPE_H
#include "SKPointer.h"
#include "SKString.h"

//һڲʹõĴobject̳еģҪԵģrttiԣVSTYPE_MARCO
//ڲobject̳еģ֧Ϊλʵл

//ԶͣҪ֧ԶлҪCUSTOMTYPE_MARCO  ԣ
//԰汾ԼҪƶ
namespace SKEngine2
{
	//custom type
	template<typename T> struct TIsCustomType									{ enum { Value = false }; };
	template<typename T> struct TIsCustomPointerType							{ enum { Value = false }; };

#define CUSTOMTYPE_MARCO(ClassName) \
	template<>	struct TIsCustomType<ClassName>	{ enum { Value = true }; }; \
	template<>	struct TIsCustomPointerType<ClassName *>	{ enum { Value = true }; };

	// vs type
	template<typename T> struct TIsVSType										{ enum { Value = false }; };

	//native pointer
	template<typename T> struct TIsNativePointerType								{ enum { Value = false }; };
	template<typename T> struct TIsNativePointerType<T*>							{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T*>						{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T* const>				{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* volatile>					{ enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* const volatile>				{ enum { Value = true }; };


	//vs pointer
	template<typename T> struct TIsVSPointerType								{ enum { Value = false }; };
	template<typename T> struct TIsVSSmartPointerType							{ enum { Value = false }; };
#define SKTYPE_MARCO(ClassName) \
	template<>	struct TIsVSType<ClassName>										{ enum { Value = true }; }; \
	template<>	struct TIsVSPointerType<ClassName *>							{ enum { Value = true }; }; \
	template<> struct TIsVSSmartPointerType<SKPointer<ClassName>>				{ enum { Value = true }; }; \
	template<> struct TIsVSSmartPointerType<const SKPointer<ClassName>>			{ enum { Value = true }; };

	template<typename T> struct TIsVSEnumType									{ enum { Value = false }; };
#define SKENUMTYPE_MARCO(ClassName) \
	template<>	struct TIsVSEnumType<ClassName>									{ enum { Value = true }; }; 
	
	//resource
	class SKFSM;
	class SKAnimSet;
	class SKMaterial;
	class SKVShader;
	class SKPShader;
	class SKTexAllState;
	class SKSkelectonMeshNode;
	class SKStaticMeshNode;
	class SKResource;
	template<class T>
	class SKResourceProxy;
	template<typename T> struct TIsVSResourceType								{ enum { Value = false}; };
	template<typename T> struct TIsVSResourcePointerType						{ enum { Value = false}; };
	template<typename T> struct TIsVSResourceProxyType							{ enum { Value = false}; };
	template<typename T> struct TIsVSResourceProxyPointType						{ enum { Value = false}; };

	template<typename T> struct TIsVSResourceProxyType<SKResourceProxy<T>>		{ enum { Value = true }; };
	template<typename T> struct TIsVSResourceProxyPointType<SKPointer<SKResourceProxy<T>>>		{ enum { Value = true }; };

// 	template<>	struct TIsVSResourceType<SKResource *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKResource>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKFSM *>								{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKFSM>>						{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKMaterial *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKMaterial>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKAnimSet *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKAnimSet>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKVShader *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKVShader>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKPShader *>							{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKPShader>>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKTexAllState *>						{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKTexAllState>>				{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKSkelectonMeshNode *>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKSkelectonMeshNode>>		{ enum { Value = true }; };
// 	template<>	struct TIsVSResourceType<SKStaticMeshNode *>					{ enum { Value = true }; };
// 	template<>	struct TIsVSResourcePointerType<SKPointer<SKStaticMeshNode>>			{ enum { Value = true }; };

	// string
	template<typename T> struct TIsVSStringType									{ enum { Value = false}; };
	template<>	struct TIsVSStringType<SKString>								{ enum { Value = true }; };
	

}
#endif