#ifndef SKDEBUGDRAW_H
#define SKDEBUGDRAW_H
#include "SKReference.h"
#include "SKDynamicBufferGeometry.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKDebugDraw : public SKReference,public SKMemObject
	{
		//Graph Debug
	public:
		SKDebugDraw();
		~SKDebugDraw();
		void AddDebugLine(const SKVector3 & P1,const SKVector3 & P2,const DWORD &Color,bool bDepth);
		void AddDebugTriangle(const SKVector3 & P1,const SKVector3 & P2,const SKVector3 &P3,
			const DWORD &Color,bool bDepth);
		void AddDebugLineAABB(const SKAABB3 & AABB,const DWORD &Color,bool bDepth);
		void AddDebugLineSphere(const SKSphere3 & Sphere,const DWORD &Color,bool bDepth);
		void AddDebugLineOBB(const SKOBB3 & OBB,const DWORD &Color,bool bDepth);

		void AddDebugTriangleAABB(const SKAABB3 & AABB,const DWORD &Color,bool bDepth);
		void AddDebugTriangleSphere(const SKSphere3 & Sphere,const DWORD &Color,bool bDepth);
		void AddDebugTriangleOBB(const SKOBB3 & OBB,const DWORD &Color,bool bDepth);
		bool m_bEnable;
	protected:
		struct DebugVertexType
		{
			SKVector3 Pos;
			DWORD Color;

		};
		SKArray<DebugVertexType> DepthDebugLineArray;
		SKArray<DebugVertexType> NoDepthDebugLineArray;

		SKMaterialRPtr m_pOnlyVertexColor;
		SKMaterialRPtr m_pOnlyVertexColorDisableDepth;
		SKVertexFormatPtr m_pDrawVertexFormat;
	public:
		void DrawDebugInfo(SKCamera * pCamera);
	};
	DECLARE_Ptr(SKDebugDraw);
}	
#endif