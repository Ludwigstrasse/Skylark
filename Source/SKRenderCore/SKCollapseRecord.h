#ifndef SKCOLLAPSERECORD_H
#define SKCOLLAPSERECORD_H
#include "SKObject.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKCollapseRecord : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	protected:
		
		unsigned int m_uiDesireRecordID;
	public:
		unsigned int m_uiCurRecordID;
		SKCollapseRecord();
		virtual ~SKCollapseRecord() = 0;
		FORCEINLINE unsigned int DesireRecordID()const
		{
			return m_uiDesireRecordID;
		}
		FORCEINLINE void SetDesireRecordID(unsigned int DesireRecordID)
		{
			if (DesireRecordID <GetRecordNum())
			{
				m_uiDesireRecordID = DesireRecordID;
			}
		}
		virtual unsigned int GetRecordNum() = 0;
	public:

	};
	DECLARE_Ptr(SKCollapseRecord);
	SKTYPE_MARCO(SKCollapseRecord);

	class RecordTriInfo : public SKObject
	{
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		RecordTriInfo()
		{
			m_uiKeep = 0 ;
			m_uiThrow = 0;
			m_uiLeftTraingleNum = 0;
			m_MapIndex.Clear();
		}
		~RecordTriInfo()
		{

		}
		unsigned int m_uiKeep;
		unsigned int m_uiThrow;
		unsigned int m_uiLeftTraingleNum;
		SKArray<unsigned int>	m_MapIndex;
	};
	DECLARE_Ptr(RecordTriInfo);
	SKTYPE_MARCO(RecordTriInfo);
	class SKCollapseRecordTri : public SKCollapseRecord
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKCollapseRecordTri();
		~SKCollapseRecordTri();
		
		friend class SKCreateClodTriFactory;
		
		virtual unsigned int GetRecordNum()
		{
			return m_RecordInfo.GetNum();
		}
		friend class SKTriangleSet;
	protected:
		SKArray<RecordTriInfo> m_RecordInfo;

	};
	DECLARE_Ptr(SKCollapseRecordTri);
	SKTYPE_MARCO(SKCollapseRecordTri);


	class RecordLineInfo
	{
	public:
		RecordLineInfo()
		{
			m_uiKeep = SKMAX_INTEGER ;
			m_uiThrow = SKMAX_INTEGER;
			m_uiLeftEdgeNum = 0;
			m_MapIndex = SKMAX_INTEGER;
		}
		~RecordLineInfo()
		{

		}
		unsigned int m_uiKeep;
		unsigned int m_uiThrow;
		unsigned int m_uiLeftEdgeNum;
		unsigned int	m_MapIndex;

	};
	class SKCollapseRecordLine : public SKCollapseRecord
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		SKCollapseRecordLine();
		~SKCollapseRecordLine();
		
		friend class SKCreateClodLineFactory;

		friend class SKLineSet;
		virtual unsigned int GetRecordNum()
		{
			return m_RecordInfo.GetNum();
		}
		friend class SKLineSet;

	protected:
		SKArray<RecordLineInfo> m_RecordInfo;

	};
	DECLARE_Ptr(SKCollapseRecordLine);
	SKTYPE_MARCO(SKCollapseRecordLine);

}
#endif