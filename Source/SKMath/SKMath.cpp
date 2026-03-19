#include "SKMath.h"
using namespace SKEngine2;
namespace SKEngine2
{
#define CPU_NAME_LEN	48
/******************************CPUṹ*************************************/
typedef struct CPUINFO_TYP 
{
	bool bSSE;					// SIMD 
	bool bSSE2;					// SIMD2
	bool b3DNOW;				// 3DNow!
	bool b3DNOWEX;				// 3DNow! AMDչ
	bool bMMX;					// MMX
	bool bMMXEX;				// MMX AMDչ
	bool bEXT;					// չ
	char vendor[13];			// 
	char name[CPU_NAME_LEN];    // cpu
} CPUINFO;
// G L O B A L S ///////////////////////////////////////////////////

bool g_bSSE = false;
SKREAL FastSin[361];
SKREAL FastCos[361];


bool GetCPUInfo();
bool    OSSupportsSSE();
void    GetCPUName(char*, int, const char*);

// F U N C T I O N S ///////////////////////////////////////////////
unsigned int Rounding(SKREAL fNum)
{
	return unsigned int(fNum + 0.5f);
}
SKREAL GetFastSin(unsigned int i)
{
	return FastSin[i];
}
SKREAL GetFastCos(unsigned int i)
{
	return FastCos[i];
}
/**
* ʼ
*/
bool SKInitCPU(void) 
{
	bool bSSE = GetCPUInfo();
	bool    bOS  = OSSupportsSSE();

	if (bSSE && bOS) g_bSSE= true;
	else g_bSSE = false;

	return g_bSSE;
}
/*----------------------------------------------------------------*/



/**
* õCPUϢ
*/
bool GetCPUInfo() 
{
	CPUINFO info;
	char *pStr = info.vendor;
	int n=1;
	int *pn = &n;


	SKMemset(&info, 0, sizeof(CPUINFO));

	//CPUIDָ鿴ǰCPUǷ֧MMX,SSE2,SSE1

	_asm 
	{
		mov  eax, 0          // س
			CPUID                

			mov  esi,     pStr
			mov  [esi],   ebx    // 14ֽ
			mov  [esi+4], edx    // 24ֽ
			mov  [esi+8], ecx    // һ4ֽ

			mov  eax, 1          // CPU
			CPUID                // 

			test edx, 04000000h  // 鿴SSE2
			jz   _NOSSE2         
			mov  [info.bSSE2], 1 

_NOSSE2: test edx, 02000000h  // 鿴SSE1
		 jz   _NOSSE          
		 mov  [info.bSSE], 1  

_NOSSE:  test edx, 00800000h  // 鿴MMX
		 jz   _EXIT1          
		 mov  [info.bMMX], 1  
_EXIT1:  
	}


	// 2:鿴Ƿ֧EXT 3DNOW
	_asm 
	{
		mov  eax, 80000000h     //Ƿ֧EXT
			CPUID
			cmp  eax, 80000000h     
			jbe  _EXIT2             
			mov [info.bEXT], 1      

			mov  eax, 80000001h     //Ƿ֧3DNOW
			CPUID
			test edx, 80000000h     
			jz   _EXIT2             
			mov  [info.b3DNOW], 1   
_EXIT2:
	}

	// 3: õCPU չ

	if ( (strncmp(info.vendor, "GenuineIntel", 12)==0) && info.bEXT) 
	{   // INTEL
		_asm 
		{

			mov  eax, 1             
				CPUID                   
				mov  esi,   pn          
				mov  [esi], ebx         
		}
		int m=0;
		SKMemcpy(&m, pn, sizeof(char)); 
		n = m;
	}
	else if ( (strncmp(info.vendor, "AuthenticAMD", 12)==0) && info.bEXT) 
	{  // AMD

		_asm 
		{
			mov  eax, 1             
				CPUID                  
				mov  esi,   pn          
				mov  [esi], eax

				mov  eax, 0x80000001    
				CPUID                   

				test edx, 0x40000000    // AMD 3DNow!
				jz   _AMD1              
				mov  [info.b3DNOWEX], 1 
_AMD1:  test edx, 0x00400000    // AMD MMX
		jz   _AMD2              
		mov  [info.bMMXEX], 1   
_AMD2:
		}
	}

	else 
	{
		if (info.bEXT)
			;
		else
			;
	}

	info.vendor[13] = '\0';                // ַ
	GetCPUName(info.name, n, info.vendor); // ȡCPU

	return info.bSSE;
}
/*----------------------------------------------------------------*/

/**
* 鿴ǰϵͳǷ֧SSE
*/
bool OSSupportsSSE() 
{

	__try
	{
		_asm xorps xmm0, xmm0
	}
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{
		if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION)
			return false;  
		return false;     
	}

	return true;
}
/*----------------------------------------------------------------*/


/**
* õCPU
* 
*/
void GetCPUName(char *chName, int n, const char *vendor) 
{
	// Intel 
	if (strncmp(vendor, "GenuineIntel", 12)==0) 
	{
		switch (n) 
		{
		case 0: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"< Pentium III/Celeron");
			} break;
		case 1: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium Celeron (1)");
			} break;
		case 2: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium III (2)");
			} break;
		case 3: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium III Xeon/Celeron");
			} break;
		case 4: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium III (4)");
			} break;
		case 6: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium III-M");
			} break;
		case 7: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium Celeron (7)");
			} break;
		case 8: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium IV (Genuine)");
			} break;
		case 9: 
			{
				sprintf_s(chName,CPU_NAME_LEN, "Pentium IV");
			} break;
		case 10: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium Celeron (10)");
			} break;
		case 11: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium Xeon / Xeon-MP");
			} break;
		case 12: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium Xeon-MP");
			} break;
		case 14: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium IV-M / Xeon");
			} break;
		case 15: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Pentium Celeron (15)");
			} break;

		default: break;
		}
	}
	// AMD 
	else if (strncmp(vendor, "AuthenticAMD", 12)==0) 
	{
		switch (n) 
		{
		case 1660: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Athlon / Duron (Model-7)");
			} break;
		case 1644: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Athlon / Duron (Model-6)");
			} break;
		case 1596: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Athlon / Duron (Model-3)");
			} break;
		case 1612: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Athlon (Model-4)");
			} break;
		case 1580: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Athlon (Model-2)");
			} break;
		case 1564: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"Athlon (Model-1)");
			} break;
		case 1463: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K6-III (Model-9)");
			} break;
		case 1420: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K6-2 (Model-8)");
			} break;
		case 1404: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K6 (Model-7)");
			} break;
		case 1388: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K6 (Model-6)");
			} break;
		case 1340: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K5 (Model-3)");
			} break;
		case 1324: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K5 (Model-2)");
			} break;
		case 1308: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K5 (Model-1)");
			} break;
		case 1292: 
			{
				sprintf_s(chName, CPU_NAME_LEN,"K5 (Model-0)");
			} break;

		default: { sprintf_s(chName, CPU_NAME_LEN,"older than K5"); break; }
		}
	}
	return;
}
#define POLYNOMIAL 0x04c11db7L
unsigned int CRCTable[256];
bool SKInitMath()
{

	SKInitCRCTable();
	SKInitCPU();
	for (unsigned int i = 0 ; i <= 360 ; i++)
	{
		SKREAL iRadian = AngleToRadian(SKREAL(i));
		FastSin[i] = SIN(iRadian);
		FastCos[i] = COS(iRadian);
	}
	return 1;
}
void SKInitCRCTable()
{
	register int i, j;  
	register unsigned long crc_accum;
	for ( i = 0;  i < 256;  i++ )
	{ 
		crc_accum = ( (unsigned long) i << 24 );
		for ( j = 0;  j < 8;  j++ )
		{
			if( crc_accum & 0x80000000L ) crc_accum = ( crc_accum << 1 ) ^ POLYNOMIAL;
			else crc_accum = ( crc_accum << 1 ); 
		}
		CRCTable[i] = crc_accum; 
	}
}
unsigned int CRC32Compute( const void *pData, unsigned int uiDateSize )
{
	unsigned int uiResult;

	if( uiDateSize == 0 ) return 0;

	const unsigned char *pDateTemp = (const unsigned char *)pData;
	uiResult  = *pDateTemp++ << 24;
	if( uiDateSize > 1 )
	{
		uiResult |= *pDateTemp++ << 16;
		if( uiDateSize > 2 )
		{
			uiResult |= *pDateTemp++ << 8;
			if( uiDateSize > 3 ) uiResult |= *pDateTemp++;
		}
	}
	uiResult = ~ uiResult;

	for( size_t i=4; i<uiDateSize; i++ )
	{
		uiResult = (uiResult << 8 | *pDateTemp++) ^ CRCTable[uiResult >> 24];
	}

	return ~uiResult;
}
}
/*----------------------------------------------------------------*/
