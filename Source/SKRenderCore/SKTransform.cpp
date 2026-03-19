#include "SKTransform.h"
#include "SKMatrix3X3.h"
#include "SKStream.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
const SKTransform SKTransform::ms_Indetity;
SKTransform::SKTransform()
{
	m_mRotate.Identity();
	m_fScale = SKVector3(1.0f,1.0f,1.0f);
	m_vTranslate.Set(0.0f,0.0f,0.0f);
	m_mCombine.Identity();
	m_mCombineInverse.Identity();
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
void SKTransform::Indetity()
{
	m_mRotate.Identity();
	m_fScale = SKVector3(1.0f,1.0f,1.0f);
	m_vTranslate.Set(0.0f,0.0f,0.0f);
	m_mCombine.Identity();
	m_mCombineInverse.Identity();
	m_bIsCombine = 1;
	m_bIsHaveInverse = 1;
}
SKTransform::~SKTransform()
{



}
void SKTransform::Combine()
{
	if(!m_bIsCombine)
	{
		SKMatrix3X3 Mat;
		Mat = SKMatrix3X3(	m_mRotate._00 * m_fScale.x,m_mRotate._01 * m_fScale.x,m_mRotate._02 * m_fScale.x,
							m_mRotate._10 * m_fScale.y,m_mRotate._11 * m_fScale.y,m_mRotate._12 * m_fScale.y,
							m_mRotate._20 * m_fScale.z,m_mRotate._21 * m_fScale.z,m_mRotate._22 * m_fScale.z);
		m_mCombine.Add3X3(Mat);
		m_mCombine.AddTranslate(m_vTranslate);
		m_bIsCombine = 1;
	}

}

void SKTransform::ApplyForward(const SKVector3 & In, SKVector3 & Out,unsigned int uiTransformType)
{
	
	if(uiTransformType == TT_POS)
	{
		Combine();
		Out = In * m_mCombine;
	}
	else if (uiTransformType == TT_DIR)
	{
		Combine();
		SKMatrix3X3 Mat;
		m_mCombine.Get3X3(Mat);
		Out = In * Mat;
	}


}
void SKTransform::Product(const SKTransform & t1,const SKTransform &t2,unsigned int TransformFlag)
{
	
	SKMatrix3X3 Mat1;
	Mat1 = SKMatrix3X3(	t1.m_mRotate._00 * t1.m_fScale.x,t1.m_mRotate._01 * t1.m_fScale.x,t1.m_mRotate._02 * t1.m_fScale.x,
						t1.m_mRotate._10 * t1.m_fScale.y,t1.m_mRotate._11 * t1.m_fScale.y,t1.m_mRotate._12 * t1.m_fScale.y,
						t1.m_mRotate._20 * t1.m_fScale.z,t1.m_mRotate._21 * t1.m_fScale.z,t1.m_mRotate._22 * t1.m_fScale.z);

	SKMatrix3X3 TempRotate = t2.m_mRotate;
	SKVector3 TempTranslate = t2.m_vTranslate;
	SKVector3 TempScale = t2.m_fScale;
	if (!(TransformFlag & TF_SCALE))
	{
		TempScale = SKVector3(1.0f,1.0f,1.0f);
	}

	if (!(TransformFlag & TF_ROTATE))
	{
		TempRotate.Identity();	
	}
	if (!(TransformFlag & TF_TRANSFORM))
	{
		TempTranslate = SKVector3(0.0f,0.0f,0.0f);
	}

	SKMatrix3X3 Mat2;
	Mat2 = SKMatrix3X3(	TempRotate._00 * TempScale.x,TempRotate._01 * TempScale.x,TempRotate._02 * TempScale.x,
						TempRotate._10 * TempScale.y,TempRotate._11 * TempScale.y,TempRotate._12 * TempScale.y,
						TempRotate._20 * TempScale.z,TempRotate._21 * TempScale.z,TempRotate._22 * TempScale.z);

	m_mRotate = Mat1 * Mat2;
	m_mRotate.GetScaleAndRotated(m_fScale);
	m_vTranslate = t1.m_vTranslate * Mat2 + TempTranslate;

	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}

void SKTransform::Inverse(SKTransform & Out)const
{
	
	SKMatrix3X3 Mat;
	Mat = SKMatrix3X3(	m_mRotate._00 * m_fScale.x,m_mRotate._01 * m_fScale.x,m_mRotate._02 * m_fScale.x,
						m_mRotate._10 * m_fScale.y,m_mRotate._11 * m_fScale.y,m_mRotate._12 * m_fScale.y,
						m_mRotate._20 * m_fScale.z,m_mRotate._21 * m_fScale.z,m_mRotate._22 * m_fScale.z);

	Out.m_mRotate.InverseOf(Mat);
	Out.m_vTranslate = m_vTranslate * Out.m_mRotate * (-1.0f);
	Out.m_mRotate.GetScaleAndRotated(Out.m_fScale);
	Out.m_bIsCombine = 0;
	Out.m_bIsHaveInverse = 0;
}
void SKTransform::ApplyInverse(const SKVector3 & In, SKVector3 & Out,unsigned int uiTransformType)
{
	
	if(uiTransformType == TT_POS)
	{
		const SKMatrix3X3W &mat = GetCombineInverse();
		Out = In * mat;

	}
	else if (uiTransformType == TT_DIR)
	{
		const SKMatrix3X3W &mat = GetCombineInverse();
		SKMatrix3X3 Mat;
		mat.Get3X3(Mat);
		Out = In * Mat;
	}
}
void SKTransform::SetMatrix(const SKMatrix3X3W & SKMat)
{
	SKMat.Get3X3(m_mRotate);
	m_vTranslate = SKMat.GetTranslation();
	m_mRotate.GetScaleAndRotated(m_fScale);

	m_bIsCombine = 1;
	m_mCombine = SKMat;
	m_bIsHaveInverse = 0;
}
void SKTransform::Interpolation(const SKTransform &t1 , const SKTransform & t2,SKREAL t)
{

	m_mRotate.Slerp(t,t1.m_mRotate,t2.m_mRotate);	


	m_fScale = LineInterpolation(t1.m_fScale,t2.m_fScale,t);


	m_vTranslate = LineInterpolation(t1.m_vTranslate , t2.m_vTranslate , t);

	
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
