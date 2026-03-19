#ifndef SKVECTOR3W_H
#define SKVECTOR3W_H
#include "SKMath.h"
#include "SKVector3.h"
namespace SKEngine2
{
	class SKMatrix3X3W;
	//4 ɫ
	class  SKMATH_API SKVector3W 
	{
	public:
		union
		{
			SKREAL m[4];
			struct  
			{
				SKREAL x, y, z, w;
			};
			struct  
			{
				SKREAL r,g,b,a;
			};
		};


		SKVector3W(void);
		SKVector3W(SKREAL _x, SKREAL _y, SKREAL _z,SKREAL _w = 1.0f);
		SKVector3W(const SKVector3 & V);

		SKVector3W(unsigned char R,unsigned char G,unsigned char B,unsigned char A = 255);
		FORCEINLINE void GetV3(SKVector3 & V)const;
		FORCEINLINE SKVector3 GetV3()const;
		FORCEINLINE void Set(const SKVector3 & V);
		FORCEINLINE void Set(SKREAL _x, SKREAL _y, SKREAL _z, SKREAL _w = 1.0f);    
		SKVector3W operator * (const SKMatrix3X3W &m)const; 
		
		void operator += (const SKVector3W &v);          
		void operator -= (const SKVector3W &v);          
		void operator *= (const SKVector3W &v);          
		void operator /= (const SKVector3W &v); 

		void operator *= (SKREAL f);                     
		void operator /= (SKREAL f);                     
		void operator += (SKREAL f);                     
		void operator -= (SKREAL f);                     

		

		SKVector3W operator * (SKREAL f)const;            
		SKVector3W operator / (SKREAL f)const;            
		SKVector3W operator + (SKREAL f)const;            
		SKVector3W operator - (SKREAL f)const;            


		SKVector3W operator + (const SKVector3W &v)const; 
		SKVector3W operator - (const SKVector3W &v)const;
		SKVector3W operator * (const SKVector3W &v)const;
		SKVector3W operator / (const SKVector3W &v)const;

		bool operator ==(const SKVector3W &v)const;

		DWORD GetDWARGB()const;
		DWORD GetDWRGBA()const;

		DWORD GetDWBGRA()const;
		DWORD GetDWABGR()const;
		void GetUCColor(unsigned char &R,unsigned char &G,unsigned char &B,unsigned char &A)const;

		void CreateFromARGB(DWORD ARGB);
		void CreateFromBGRA(DWORD BGRA);
		void CreateFromRGBA(DWORD RGBA);
		void CreateFormABGR(DWORD ABGR);
	};  // class

	typedef class SKVector3W SKColorRGBA;
	FORCEINLINE  DWORD SKDWCOLORARGB(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)));
	}
	FORCEINLINE  DWORD SKDWCOLORBGRA(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((b)&0xff)<<24)|(((g)&0xff)<<16)|(((r)&0xff)<<8)|((a)&0xff)));
	}
	FORCEINLINE  DWORD SKDWCOLORRGBA(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)));
	}
	FORCEINLINE  DWORD SKDWCOLORABGR(unsigned char a, unsigned char r, unsigned char g,unsigned char b) 
	{
		return ((DWORD)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)));
	}

	FORCEINLINE  void SKDWCOLORGetARGB(DWORD ARGB,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		a = (ARGB>>24) & 0xff;
		r = (ARGB>>16) & 0xff;
		g = (ARGB>>8) & 0xff;
		b = (ARGB) & 0xff;
	}
	FORCEINLINE  void SKDWCOLORGetBGRA(DWORD BGRA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		b = (BGRA>>24) & 0xff;
		g = (BGRA>>16) & 0xff;
		r = (BGRA>>8) & 0xff;
		a = (BGRA) & 0xff;
	}
	FORCEINLINE  void SKDWCOLORGetRGBA(DWORD RGBA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		r = (RGBA>>24) & 0xff;
		g = (RGBA>>16) & 0xff;
		b = (RGBA>>8) & 0xff;
		a = (RGBA) & 0xff;
	}
	FORCEINLINE  void SKDWCOLORGetABGR(DWORD ABGR,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b) 
	{
		a = (ABGR>>24) & 0xff;
		b = (ABGR>>16) & 0xff;
		g = (ABGR>>8) & 0xff;
		r = (ABGR) & 0xff;
	}
	#include "SKVector3W.inl"
}
#endif