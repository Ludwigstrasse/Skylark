#ifndef SKDATABUFFER_H
#define SKDATABUFFER_H
#include "SKObject.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKDataBuffer : public SKObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	//Date Type
		{
			DT_FLOAT32_1,// 1D SKREAL expanded to (value, 0., 0., 1.)
			DT_FLOAT32_2,// 2D SKREAL expanded to (value, value, 0., 1.)
			DT_FLOAT32_3,// 3D SKREAL expanded to (value, value, value, 1.)
			DT_FLOAT32_4,// 4D SKREAL

			DT_USHORT,
			DT_UINT,

			DT_UBYTE4,	// 4D unsigned byte
			DT_UBYTE4N, // Each of 4 bytes is normalized by dividing to 255.0

			DT_SHORT2,	// 2D signed short expanded to (value, value, 0., 1.)
			DT_SHORT2N,	// 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
			DT_USHORT2N,// 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)

			DT_SHORT4,	// 4D signed short
			DT_SHORT4N,	// 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
			DT_USHORT4N,// 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)

			DT_FLOAT16_2,// Two 16-bit floating point values, expanded to (value, value, 0, 1)
			DT_FLOAT16_4,// Four 16-bit floating point values

			DT_COLOR,	// 4D packed unsigned bytes mapped to 0. to 1. range
						// Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
			DT_MAXNUM
		};
		
		virtual ~SKDataBuffer();
		SKDataBuffer();
	public:
		FORCEINLINE unsigned int GetDT()const{return m_uiDT;}
		FORCEINLINE unsigned int GetStride()const{return ms_uiDateTypeByte[m_uiDT];}
		FORCEINLINE unsigned int GetChannel()const{return ms_uiDateTypeChannel[m_uiDT];}
		FORCEINLINE unsigned int GetNum()const{return m_uiNum;}
		FORCEINLINE unsigned int GetSize()const{return GetStride() * m_uiNum;}
		FORCEINLINE void *GetDate()const {return m_pDate;}
		//ͨڹ涨򷵻0
		bool SetDate(const void *pDate,unsigned int uiNum,unsigned int uiDT);
		//ͨڹ涨򷵻0
		bool AddDate(const void *pDate,unsigned int uiNum,unsigned int uiDT);
		bool CreateEmptyBuffer(unsigned int uiNum,unsigned int uiDT);
		static unsigned int ms_uiDateTypeByte[DT_MAXNUM];
		static unsigned int ms_uiDateTypeChannel[DT_MAXNUM];
	protected:
		unsigned int m_uiDT;
		unsigned int m_uiNum;
		unsigned char *m_pDate;
		unsigned int m_uiSize;
	
	};
	DECLARE_Ptr(SKDataBuffer);
	SKTYPE_MARCO(SKDataBuffer);
}
#endif