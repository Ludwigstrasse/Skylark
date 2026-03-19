#include "SKMatrix3X3W.h"
#include "SKMath.h"
using namespace SKEngine2;
SKMatrix3X3W SKMatrix3X3W::ms_Materix3X3WIdentity;
/*----------------------------------------------------------------*/
SKMatrix3X3W::SKMatrix3X3W(void)
{
	Identity();

}
/*----------------------------------------------------------------*/
SKMatrix3X3W::SKMatrix3X3W(SKREAL m00,SKREAL m01,SKREAL m02,SKREAL m03,
							 SKREAL m10,SKREAL m11,SKREAL m12,SKREAL m13,
							 SKREAL m20,SKREAL m21,SKREAL m22,SKREAL m23,
							 SKREAL m30,SKREAL m31,SKREAL m32,SKREAL m33)
{

	_00 = m00; _01 = m01; _02 = m02; _03 = m03;
	_10 = m10; _11 = m11; _12 = m12; _13 = m13;
	_20 = m20; _21 = m21; _22 = m22; _23 = m23;
	_30 = m30; _31 = m31; _32 = m32; _33 = m33;
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::CreateTranslate(SKREAL dx, SKREAL dy, SKREAL dz) 
{
	Identity();
	_30 = dx;
	_31 = dy;
	_32 = dz;
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::CreateTranslate(const SKVector3 & V)
{
	Identity();
	_30 = V.x;
	_31 = V.y;
	_32 = V.z;

}
/*----------------------------------------------------------------*/

void SKMatrix3X3W::CreateFormBillboard(const SKVector3 &vcPos, 
										const SKMatrix3X3 &CameraRotMatrix,
										bool bAxisY) 
{
	SKMatrix3X3 Mat;
	Mat.InverseOf(CameraRotMatrix);
	CreateFrom3X3(Mat);
	_30 = vcPos.x;
	_31 = vcPos.y;
	_32 = vcPos.z;
	_33 = 1.0f;
	/*
	ֻаYתôֻҪAY(0,1,0)
	A * A = E
		x1,y1,z1
	A=( x2,y2,z2	)
		x3,y3,z3

			a1,a2,a3
	A=(	b1,b2,b3	)
			c1,c2,c3

	ΪAΪ󣬴пҲΪλҲǵΪ0

	AҲΪ󣬴п

	ʵдпνֻΪ˺ϣⷽ

	ΪֻùֻYתǼYʼΪ(0,1,0),Ҳ˵

	YϣҲֻYתˡ

	ǲֻҪ޸Ϳԡ

	

		x1,0,z1
	A=( x2,1,z2	)
		x3,0,z3

	ΪXZǰϸ,
	ΪY(0,1,0),Ȼӵ͹۲ER
	ȻREϷ

	޸ĵAûڼϷԲܱ֤AΪ

			a1,a2,a3
	A=(	0,1,0	)
			c1,c2,c3

	*/
	if(bAxisY)
	{
		_10 = 0.0f;
		_11 = 1.0f;
		_12 = 0.0f;

	}
} // Billboard
/*----------------------------------------------------------------*/
bool SKMatrix3X3W::CreateFromLookDir(const SKVector3 &vcPos,					
									const SKVector3 &vcDir,					
									const SKVector3 &vcWorldUp)
{
	SKVector3 vcTemp, vcUp;
	SKVector3 Dir = vcDir;
	Dir.Normalize();

	//㳯Ϸ,ΪDirΪλ,fDotΪvcWorld
	//DirϵͶӰ
	SKREAL fDot = vcWorldUp.Dot(Dir);
	//ͶӰ,Dirͬ
	vcTemp = Dir * fDot;
	//ƽ4ηup
	/*
			  /|
worldup-->	 / |<---up
			/__|__ 
	vcTemp
	*/				
	vcUp = vcWorldUp - vcTemp;

	//
	SKREAL fL = vcUp.GetLength();

	// Ϊ0,˵ vcDirworldUpƽ,ǼΪ0.
	if (fL < EPSILON_E4) 
	{
		SKVector3 vcY;
		//ѡ,up
		vcY.Set(0.0f, 0.0f, 1.0f);

		vcTemp = Dir * Dir.z;
		vcUp = vcY - vcTemp;


		fL = vcUp.GetLength();
		// Ϊ0,˵ vcDirworldUpƽ,ǼΪ0.
		if (fL < EPSILON_E4) 
		{
			
			//ѡ,up
			vcY.Set(1.0f, 0.0f, 0.0f);

			vcTemp = Dir * Dir.x;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			
				if(fL < EPSILON_E4) return 0;
			
		}
	}
	//λ
	vcUp /= fL;


	SKVector3 vcRight;
	vcRight.Cross(vcUp, Dir);

	CreateInWorldObject(vcRight, vcUp, Dir, vcPos);
	return 1;

}//LookDir
/*----------------------------------------------------------------*/
bool SKMatrix3X3W::CreateFromLookAt(const SKVector3 &vcPos, const SKVector3 &vcLookAt,
									const SKVector3 &vcWorldUp) 
{
	SKVector3 vcDir;
	//㳯
	vcDir = vcLookAt - vcPos;
	return CreateFromLookDir(vcPos,vcDir,vcWorldUp);
	
} // LookAt
/*----------------------------------------------------------------*/
void SKMatrix3X3W::CreateInWorldObject(const SKVector3 &U,const SKVector3 &V,const SKVector3 & N,const SKVector3 &Point)
{
	_00 = U.x;
	_10 = U.y;
	_20 = U.z;

	_01 = V.x;
	_11 = V.y;
	_21 = V.z;

	_02 = N.x;
	_12 = N.y;
	_22 = N.z;

	_03=0.0f; _13=0.0f; _23=0.0f; _33=1.0f;

	_30 = -(U.Dot(Point));
	_31 = -(V.Dot(Point));
	_32 = -(N.Dot(Point));

}
/*----------------------------------------------------------------*/
SKMatrix3X3W SKMatrix3X3W::operator +(SKREAL f)const
{

	SKMatrix3X3W Temp;
#ifdef SK_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_add_ps(_v1,_f);
		__m128 _r2 = _mm_add_ps(_v2,_f);
		__m128 _r3 = _mm_add_ps(_v3,_f);
		__m128 _r4 = _mm_add_ps(_v4,_f);

		Temp.M[0][0] = _r1.m128_f32[3]; Temp.M[0][1] = _r1.m128_f32[2]; Temp.M[0][2] = _r1.m128_f32[1]; Temp.M[0][3] = _r1.m128_f32[0];
		Temp.M[1][0] = _r2.m128_f32[3]; Temp.M[1][1] = _r2.m128_f32[2]; Temp.M[1][2] = _r2.m128_f32[1]; Temp.M[1][3] = _r2.m128_f32[0];
		Temp.M[2][0] = _r3.m128_f32[3]; Temp.M[2][1] = _r3.m128_f32[2]; Temp.M[2][2] = _r3.m128_f32[1]; Temp.M[2][3] = _r3.m128_f32[0];
		Temp.M[3][0] = _r4.m128_f32[3]; Temp.M[3][1] = _r4.m128_f32[2]; Temp.M[3][2] = _r4.m128_f32[1]; Temp.M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		
		Temp.m[0] = m[0] + f;
		Temp.m[1] = m[1] + f;
		Temp.m[2] = m[2] + f;
		Temp.m[3] = m[3] + f;
		Temp.m[4] = m[4] + f;
		Temp.m[5] = m[5] + f;
		Temp.m[6] = m[6] + f;
		Temp.m[7] = m[7] + f;
		Temp.m[8] = m[8] + f;
		Temp.m[9] = m[9] + f;
		Temp.m[10] = m[10] + f;
		Temp.m[11] = m[11] + f;
		Temp.m[12] = m[12] + f;
		Temp.m[13] = m[13] + f;
		Temp.m[14] = m[14] + f;
		Temp.m[15] = m[15] + f;
		
	}
#endif
	return Temp;

}
/*----------------------------------------------------------------*/
SKMatrix3X3W SKMatrix3X3W::operator -(SKREAL f)const
{
	SKMatrix3X3W Temp;
#ifdef SK_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_sub_ps(_v1,_f);
		__m128 _r2 = _mm_sub_ps(_v2,_f);
		__m128 _r3 = _mm_sub_ps(_v3,_f);
		__m128 _r4 = _mm_sub_ps(_v4,_f);

		Temp.M[0][0] = _r1.m128_f32[3]; Temp.M[0][1] = _r1.m128_f32[2]; Temp.M[0][2] = _r1.m128_f32[1]; Temp.M[0][3] = _r1.m128_f32[0];
		Temp.M[1][0] = _r2.m128_f32[3]; Temp.M[1][1] = _r2.m128_f32[2]; Temp.M[1][2] = _r2.m128_f32[1]; Temp.M[1][3] = _r2.m128_f32[0];
		Temp.M[2][0] = _r3.m128_f32[3]; Temp.M[2][1] = _r3.m128_f32[2]; Temp.M[2][2] = _r3.m128_f32[1]; Temp.M[2][3] = _r3.m128_f32[0];
		Temp.M[3][0] = _r4.m128_f32[3]; Temp.M[3][1] = _r4.m128_f32[2]; Temp.M[3][2] = _r4.m128_f32[1]; Temp.M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		Temp.m[0] = m[0] - f;
		Temp.m[1] = m[1] - f;
		Temp.m[2] = m[2] - f;
		Temp.m[3] = m[3] - f;
		Temp.m[4] = m[4] - f;
		Temp.m[5] = m[5] - f;
		Temp.m[6] = m[6] - f;
		Temp.m[7] = m[7] - f;
		Temp.m[8] = m[8] - f;
		Temp.m[9] = m[9] - f;
		Temp.m[10] = m[10] - f;
		Temp.m[11] = m[11] - f;
		Temp.m[12] = m[12] - f;
		Temp.m[13] = m[13] - f;
		Temp.m[14] = m[14] - f;
		Temp.m[15] = m[15] - f;
	}
#endif
	return Temp;

}
/*----------------------------------------------------------------*/
SKMatrix3X3W SKMatrix3X3W::operator *(SKREAL f)const
{
	SKMatrix3X3W Temp;
	#ifdef SK_SSE
	{
		SKFastMul(*this,f,Temp);
	}
	#else
	{
		Temp.m[0] = m[0] * f;
		Temp.m[1] = m[1] * f;
		Temp.m[2] = m[2] * f;
		Temp.m[3] = m[3] * f;
		Temp.m[4] = m[4] * f;
		Temp.m[5] = m[5] * f;
		Temp.m[6] = m[6] * f;
		Temp.m[7] = m[7] * f;
		Temp.m[8] = m[8] * f;
		Temp.m[9] = m[9] * f;
		Temp.m[10] = m[10] * f;
		Temp.m[11] = m[11] * f;
		Temp.m[12] = m[12] * f;
		Temp.m[13] = m[13] * f;
		Temp.m[14] = m[14] * f;
		Temp.m[15] = m[15] * f;
	}
	#endif
	return Temp;
}
/*----------------------------------------------------------------*/
SKMatrix3X3W SKMatrix3X3W::operator +(const SKMatrix3X3W &Matirx)const
{

	SKMatrix3X3W Temp;
	#ifdef SK_SSE
	{
		SKFastAdd(*this,Matirx,Temp);
	}
	#else
	{
		Temp.m[0] = m[0] + Matirx.m[0];
		Temp.m[1] = m[1] + Matirx.m[1];
		Temp.m[2] = m[2] + Matirx.m[2];
		Temp.m[3] = m[3] + Matirx.m[3];
		Temp.m[4] = m[4] + Matirx.m[4];
		Temp.m[5] = m[5] + Matirx.m[5];
		Temp.m[6] = m[6] + Matirx.m[6];
		Temp.m[7] = m[7] + Matirx.m[7];
		Temp.m[8] = m[8] + Matirx.m[8];
		Temp.m[9] = m[9] + Matirx.m[9];
		Temp.m[10] = m[10] + Matirx.m[10];
		Temp.m[11] = m[11] + Matirx.m[11];
		Temp.m[12] = m[12] + Matirx.m[12];
		Temp.m[13] = m[13] + Matirx.m[13];
		Temp.m[14] = m[14] + Matirx.m[14];
		Temp.m[15] = m[15] + Matirx.m[15];
	}
	#endif
	return Temp;	

}
/*----------------------------------------------------------------*/
SKMatrix3X3W SKMatrix3X3W::operator -(const SKMatrix3X3W &Matirx)const
{
	SKMatrix3X3W Temp;
	#ifdef SK_SSE
	{
		SKFastSub(*this,Matirx,Temp);
	}
	#else
	{
		Temp.m[0] = m[0] - Matirx.m[0];
		Temp.m[1] = m[1] - Matirx.m[1];
		Temp.m[2] = m[2] - Matirx.m[2];
		Temp.m[3] = m[3] - Matirx.m[3];
		Temp.m[4] = m[4] - Matirx.m[4];
		Temp.m[5] = m[5] - Matirx.m[5];
		Temp.m[6] = m[6] - Matirx.m[6];
		Temp.m[7] = m[7] - Matirx.m[7];
		Temp.m[8] = m[8] - Matirx.m[8];
		Temp.m[9] = m[9] - Matirx.m[9];
		Temp.m[10] = m[10] - Matirx.m[10];
		Temp.m[11] = m[11] - Matirx.m[11];
		Temp.m[12] = m[12] - Matirx.m[12];
		Temp.m[13] = m[13] - Matirx.m[13];
		Temp.m[14] = m[14] - Matirx.m[14];
		Temp.m[15] = m[15] - Matirx.m[15];
	}
	#endif
	return Temp;	

}


/*----------------------------------------------------------------*/
void SKMatrix3X3W::operator +=(SKREAL f)
{
#ifdef SK_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_add_ps(_v1,_f);
		__m128 _r2 = _mm_add_ps(_v2,_f);
		__m128 _r3 = _mm_add_ps(_v3,_f);
		__m128 _r4 = _mm_add_ps(_v4,_f);

		M[0][0] = _r1.m128_f32[3]; M[0][1] = _r1.m128_f32[2]; M[0][2] = _r1.m128_f32[1]; M[0][3] = _r1.m128_f32[0];
		M[1][0] = _r2.m128_f32[3]; M[1][1] = _r2.m128_f32[2]; M[1][2] = _r2.m128_f32[1]; M[1][3] = _r2.m128_f32[0];
		M[2][0] = _r3.m128_f32[3]; M[2][1] = _r3.m128_f32[2]; M[2][2] = _r3.m128_f32[1]; M[2][3] = _r3.m128_f32[0];
		M[3][0] = _r4.m128_f32[3]; M[3][1] = _r4.m128_f32[2]; M[3][2] = _r4.m128_f32[1]; M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		m[0] = m[0] + f;
		m[1] = m[1] + f;
		m[2] = m[2] + f;
		m[3] = m[3] + f;
		m[4] = m[4] + f;
		m[5] = m[5] + f;
		m[6] = m[6] + f;
		m[7] = m[7] + f;
		m[8] = m[8] + f;
		m[9] = m[9] + f;
		m[10] = m[10] + f;
		m[11] = m[11] + f;
		m[12] = m[12] + f;
		m[13] = m[13] + f;
		m[14] = m[14] + f;
		m[15] = m[15] + f;
	}
#endif


}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::operator -=(SKREAL f)
{
#ifdef SK_SSE
	{
		__m128 _v1 = _mm_set_ps(m[0],m[1],m[2],m[3]);
		__m128 _v2 = _mm_set_ps(m[4],m[5],m[6],m[7]);
		__m128 _v3 = _mm_set_ps(m[8],m[9],m[10],m[11]);
		__m128 _v4 = _mm_set_ps(m[12],m[13],m[14],m[15]);
		__m128 _f = _mm_set_ps(f,f,f,f);

		__m128 _r1 = _mm_sub_ps(_v1,_f);
		__m128 _r2 = _mm_sub_ps(_v2,_f);
		__m128 _r3 = _mm_sub_ps(_v3,_f);
		__m128 _r4 = _mm_sub_ps(_v4,_f);

		M[0][0] = _r1.m128_f32[3]; M[0][1] = _r1.m128_f32[2]; M[0][2] = _r1.m128_f32[1]; M[0][3] = _r1.m128_f32[0];
		M[1][0] = _r2.m128_f32[3]; M[1][1] = _r2.m128_f32[2]; M[1][2] = _r2.m128_f32[1]; M[1][3] = _r2.m128_f32[0];
		M[2][0] = _r3.m128_f32[3]; M[2][1] = _r3.m128_f32[2]; M[2][2] = _r3.m128_f32[1]; M[2][3] = _r3.m128_f32[0];
		M[3][0] = _r4.m128_f32[3]; M[3][1] = _r4.m128_f32[2]; M[3][2] = _r4.m128_f32[1]; M[3][3] = _r4.m128_f32[0];
	}
#else
	{
		m[0] = m[0] - f;
		m[1] = m[1] - f;
		m[2] = m[2] - f;
		m[3] = m[3] - f;
		m[4] = m[4] - f;
		m[5] = m[5] - f;
		m[6] = m[6] - f;
		m[7] = m[7] - f;
		m[8] = m[8] - f;
		m[9] = m[9] - f;
		m[10] = m[10] - f;
		m[11] = m[11] - f;
		m[12] = m[12] - f;
		m[13] = m[13] - f;
		m[14] = m[14] - f;
		m[15] = m[15] - f;
	}
#endif

}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::operator *=(SKREAL f)
{
	#ifdef SK_SSE
	{
		SKFastMul(*this,f,*this);
	}
	#else
	{
		m[0] = m[0] * f;
		m[1] = m[1] * f;
		m[2] = m[2] * f;
		m[3] = m[3] * f;
		m[4] = m[4] * f;
		m[5] = m[5] * f;
		m[6] = m[6] * f;
		m[7] = m[7] * f;
		m[8] = m[8] * f;
		m[9] = m[9] * f;
		m[10] = m[10] * f;
		m[11] = m[11] * f;
		m[12] = m[12] * f;
		m[13] = m[13] * f;
		m[14] = m[14] * f;
		m[15] = m[15] * f;
	}
	#endif
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::operator +=(const SKMatrix3X3W &Matirx)
{

	SKMatrix3X3W Temp;
	#ifdef SK_SSE
	{
		SKFastAdd(*this,Matirx,*this);
	}
	#else
	{
		m[0] = m[0] + Matirx.m[0];
		m[1] = m[1] + Matirx.m[1];
		m[2] = m[2] + Matirx.m[2];
		m[3] = m[3] + Matirx.m[3];
		m[4] = m[4] + Matirx.m[4];
		m[5] = m[5] + Matirx.m[5];
		m[6] = m[6] + Matirx.m[6];
		m[7] = m[7] + Matirx.m[7];
		m[8] = m[8] + Matirx.m[8];
		m[9] = m[9] + Matirx.m[9];
		m[10] = m[10] + Matirx.m[10];
		m[11] = m[11] + Matirx.m[11];
		m[12] = m[12] + Matirx.m[12];
		m[13] = m[13] + Matirx.m[13];
		m[14] = m[14] + Matirx.m[14];
		m[15] = m[15] + Matirx.m[15];
	}
	#endif

}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::operator -=(const SKMatrix3X3W &Matirx)
{

	SKMatrix3X3W Temp;
	#ifdef SK_SSE
	{
		SKFastSub(*this,Matirx,*this);
	}
	#else
	{
		m[0] = m[0] - Matirx.m[0];
		m[1] = m[1] - Matirx.m[1];
		m[2] = m[2] - Matirx.m[2];
		m[3] = m[3] - Matirx.m[3];
		m[4] = m[4] - Matirx.m[4];
		m[5] = m[5] - Matirx.m[5];
		m[6] = m[6] - Matirx.m[6];
		m[7] = m[7] - Matirx.m[7];
		m[8] = m[8] - Matirx.m[8];
		m[9] = m[9] - Matirx.m[9];
		m[10] = m[10] - Matirx.m[10];
		m[11] = m[11] - Matirx.m[11];
		m[12] = m[12] - Matirx.m[12];
		m[13] = m[13] - Matirx.m[13];
		m[14] = m[14] - Matirx.m[14];
		m[15] = m[15] - Matirx.m[15];
	}
	#endif
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::CreateFrom3X3(const SKMatrix3X3 & Mat)
{
	Identity();
	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			M[i][j] = Mat.M[i][j];
		}
	
	}
	
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::Add3X3(const SKMatrix3X3 & Mat)
{
	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			M[i][j] = Mat.M[i][j];
		}

	}
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::AddTranslate(const SKVector3 & V)
{
	_30 = V.x;
	_31 = V.y;
	_32 = V.z;
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::AddTranslate(SKREAL dx, SKREAL dy, SKREAL dz)
{
	_30 = dx;
	_31 = dy;
	_32 = dz;
}
/*----------------------------------------------------------------*/
bool SKMatrix3X3W::CreatePerspective(SKREAL fFov , SKREAL fAspect, SKREAL fZN ,SKREAL fZF)
{
	
	if(ABS(fZF - fZN) < 0.01f)
		return 0;

	SKREAL sinFOV2 = SIN(fFov/2);

	if(ABS(sinFOV2) < EPSILON_E5)
		return 0;

	SKREAL cosFOV2 = COS(fFov/2);

	SKREAL w =  1.0f  *(cosFOV2 / sinFOV2);
	SKREAL h =   fAspect * (cosFOV2 / sinFOV2);
	SKREAL Q = fZF / (fZF - fZN);
	Identity();
	_00 = w;
	_11 = h;
	_22 = Q;
	_23 = 1.0f;
	_32 = -Q * fZN;
	_33 = 0; 
	return 1;

}
/*----------------------------------------------------------------*/
bool SKMatrix3X3W::CreateOrthogonal(SKREAL fW ,SKREAL fH,	SKREAL fZN ,SKREAL fZF)
{
	if(ABS(fZF - fZN) < 0.01f)
		return 0;
	Identity();
	_00 = 2.0f / fW;
	_11 = 2.0f / fH;
	_22 = 1.0f / (fZF - fZN);
	_32 = -fZN / (fZF - fZN);
	return 1;
}
/*----------------------------------------------------------------*/
bool SKMatrix3X3W::CreateViewPort(SKREAL fX,SKREAL fY,SKREAL fWidth,SKREAL fHeight,SKREAL fMinz,SKREAL fMaxz)
{
	if(fMinz > fMaxz)
		return 0;
	Identity();
	_00 = fWidth * 0.5f ;
	_11 = - fHeight * 0.5f ;
	_22 = fMaxz - fMinz;
	_30 = fX + _00;
	_31 = fY - _11;
	_32 = fMinz;

	return 1;
	 
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::GetRowVector(SKVector3W Row[4])const
{
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 4 ; j++)
			Row[i].m[j] = M[i][j];

	}

}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::GetColumnVector(SKVector3W Column[4])const
{
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 4 ; j++)
			Column[i].m[j] = M[j][i];

	}

}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::GetRowVector(SKVector3W &Row0,SKVector3W &Row1,SKVector3W &Row2,SKVector3W &Row3)const
{
	Row0.x = M[0][0] ; Row0.y = M[0][1] ; Row0.z = M[0][2] ; Row0.w = M[0][3] ;
	Row1.x = M[1][0] ; Row1.y = M[1][1] ; Row1.z = M[1][2] ; Row1.w = M[1][3] ;
	Row2.x = M[2][0] ; Row2.y = M[2][1] ; Row2.z = M[2][2] ; Row2.w = M[2][3] ;
	Row3.x = M[3][0] ; Row3.y = M[3][1] ; Row3.z = M[3][2] ; Row3.w = M[3][3] ;
}
/*----------------------------------------------------------------*/
void SKMatrix3X3W::GetColumnVector(SKVector3W &Column0,SKVector3W &Column1,SKVector3W &Column2,SKVector3W &Column3)const
{
	Column0.x = M[0][0] ; Column0.y = M[1][0] ; Column0.z = M[2][0] ; Column0.w = M[3][0] ;
	Column1.x = M[0][1] ; Column1.y = M[1][1] ; Column1.z = M[2][1] ; Column1.w = M[3][1] ;
	Column2.x = M[0][2] ; Column2.y = M[1][2] ; Column2.z = M[2][2] ; Column2.w = M[3][2] ;
	Column3.x = M[0][3] ; Column3.y = M[1][3] ; Column3.z = M[2][3] ; Column3.w = M[3][3] ; 

}
/*----------------------------------------------------------------*/
bool SKMatrix3X3W::operator ==(const SKMatrix3X3W &v)const
{
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		for (unsigned int j = 0 ; j < 4 ; j++)
		{
			if (ABS(M[i][j] - v.M[i][j]) > EPSILON_E4)
			{
				return false;
			}
		}
		
	}
	return true;
}
/*void SKMatrix3X3W::CreatePointLightPlaneShadow(const SKVector3 & LightPoint,const SKPlane3 &Plane)
{
	SKVector3 PlaneN = Plane.GetN();
	SKREAL fD = Plane.GetfD();
	SKREAL temp = PlaneN * LightPoint + fD;

	_00 = temp - PlaneN.x * LightPoint.x;
	_01 = -PlaneN.x * LightPoint.y;
	_02 = -PlaneN.x * LightPoint.z;
	_03 = -PlaneN.x;

	_10 = -PlaneN.y * LightPoint.x;
	_11 = temp - PlaneN.y * LightPoint.y;
	_12 = -PlaneN.y * LightPoint.z;
	_13 = -PlaneN.y;

	_20 = -PlaneN.z * LightPoint.x;
	_21 = -PlaneN.z * LightPoint.y;
	_22 = temp - PlaneN.z * LightPoint.z;
	_23 = -PlaneN.z;

	_30 = -fD * LightPoint.x ;
	_31 = -fD * LightPoint.y ;
	_32 = -fD * LightPoint.z ;
	_33 = PlaneN * LightPoint;
}
/*----------------------------------------------------------------*/
/*void SKMatrix3X3W::CreateInfiniteLightPlaneShadow(const SKVector3 & LightDir,const SKPlane3 &Plane)
{

	SKVector3 PlaneN = Plane.GetN();
	SKREAL fD = Plane.GetfD();
	SKREAL temp = PlaneN * LightDir;

	_00 = temp - PlaneN.x * LightDir.x;
	_01 = -PlaneN.x * LightDir.y;
	_02 = -PlaneN.x * LightDir.z;
	_03 = -PlaneN.x;

	_10 = -PlaneN.y * LightDir.x;
	_11 = temp - PlaneN.y * LightDir.y;
	_12 = -PlaneN.y * LightDir.z;
	_13 = -PlaneN.y;

	_20 = -PlaneN.z * LightDir.x;
	_21 = -PlaneN.z * LightDir.y;
	_22 = temp - PlaneN.z * LightDir.z;
	_23 = -PlaneN.z;

	_30 = -fD * LightDir.x ;
	_31 = -fD * LightDir.y ;
	_32 = -fD * LightDir.z ;
	_33 = PlaneN * LightDir;
}*/
