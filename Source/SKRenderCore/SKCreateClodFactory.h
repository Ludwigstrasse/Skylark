#ifndef SKCREATECLODFACTORY_H
#define SKCREATECLODFACTORY_H
#include "SKCollapseRecord.h"
#include "SKVector3.h"
#include "SKArray.h"
#include "SKTriangleSet.h"
#include "SKLineSet.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKCreateClodLineFactory
	{
	protected:
		class Vertex3DAttr
		{
		public:
			Vertex3DAttr()
			{

			}
			~Vertex3DAttr()
			{

			}
			bool m_bIsDelete;
			SKVector3 m_Position;
			SKREAL m_fWeight;
			unsigned int m_uiIndexInVertexArray;
			SKArray<unsigned int> m_InEdgeIndexArray;
			bool FORCEINLINE operator == (const Vertex3DAttr &Vertex3D)const
			{
				return (m_uiIndexInVertexArray == Vertex3D.m_uiIndexInVertexArray);
			}
		};
		class Edge3DAttr
		{
		public:
			Edge3DAttr()
			{

			}
			~Edge3DAttr()
			{

			}
			bool m_bIsDelete;
			unsigned int m_ContainVertexIndex[2];
			

		};
		static SKArray<Vertex3DAttr> ms_V3Attr;
		static SKArray<Edge3DAttr> ms_E3Attr;
		static SKCollapseRecordLine * ms_pCRL;

		static SKArray<unsigned int> ms_RecordLine;
		static unsigned int ms_uiLeftLineNum;
		static SKLineSet * ms_pLineSet;
		static void InitalDate();
		static SKREAL GetCollapseWeight(const Vertex3DAttr & Vertex3D);
		static bool GetCollapseV(unsigned int &uiThrowV3DIndex);
		static bool Collapse(unsigned int uiThrowV3DIndex);
	public:
		static SKCollapseRecordLine * CreateClodLine( SKLineSet * pLineSet);
	};
	class SKGRAPHIC_API SKCreateClodTriFactory
	{
	protected:
		class Vertex3DAttr
		{
		public:
			Vertex3DAttr()
			{

			}
			~Vertex3DAttr()
			{

			}
			bool m_bIsDelete;
			SKVector3 m_Position;
			SKArray<unsigned int> m_InEdgeIndexArray;
			SKArray<unsigned int> m_InTriangleIndexArray;
		};

		class Edge3DAttr
		{
		public:
			Edge3DAttr()
			{
				
			}
			~Edge3DAttr()
			{

			}
			bool m_bIsDelete;
			unsigned int m_ContainVertexIndex[2];
			SKArray<unsigned int> m_InTriangleIndexArray;
			SKREAL m_fWeight;
			bool FORCEINLINE operator == (const Edge3DAttr &Edge3D)const
			{
				return ((m_ContainVertexIndex[0] == Edge3D.m_ContainVertexIndex[0] &&
					m_ContainVertexIndex[1] == Edge3D.m_ContainVertexIndex[1]) || 
					(m_ContainVertexIndex[1] == Edge3D.m_ContainVertexIndex[0] &&
					m_ContainVertexIndex[0] == Edge3D.m_ContainVertexIndex[1]));
			}

		};
		class Triangle3DAttr
		{
		public:
			Triangle3DAttr()
			{

			}
			~Triangle3DAttr()
			{
			
			}
			bool m_bIsDelete;
			unsigned int m_ContainVertexIndex[3];
			unsigned int m_ContainEdgeIndex[3];
		};

		static SKArray<Vertex3DAttr> ms_V3Attr;
		static SKArray<Edge3DAttr> ms_E3Attr;
		static SKArray<Triangle3DAttr> ms_T3Attr;

		static SKCollapseRecordTri * ms_pCRT;

		static SKArray<unsigned int> ms_RecordTriangle;

		static unsigned int ms_uiLeftTrangleNum;
		static SKREAL GetCollapseWeight(const Edge3DAttr & Edge3D);
		static void InitalDate();

		static SKTriangleSet * ms_pTriangleSet;

		static bool GetCollapseEV(unsigned int &uiE3DIndex,unsigned int &uiKeepV3DIndex,unsigned int &uiThrowV3DIndex);

		static bool Collapse(unsigned int uiE3DIndex,unsigned int uiKeepV3DIndex,unsigned int uiThrowV3DIndex);

	public:
		static SKCollapseRecordTri * CreateClodTriangle( SKTriangleSet * pTriangleSet);


	};
}
#endif