#ifndef SKTRANSFORM_H
#define SKTRANSFORM_H
#include "SKVector3.h"
#include "SKMatrix3X3W.h"
#include "SKOtherMath.h"
#include "SKGraphic.h"
//#pragma comment(lib, "SKMath.lib")
/*
	任ı任˳S,R,T


*/

namespace SKEngine2
{
class SKStream;
class SKGRAPHIC_API SKTransform
{

private:
	SKMatrix3X3W m_mCombine;
	SKMatrix3X3W m_mCombineInverse;
	void Combine();
	SKVector3 m_fScale;
	SKMatrix3X3 m_mRotate;
	SKVector3 m_vTranslate;
	bool m_bIsCombine;
	bool m_bIsHaveInverse;
public:
	
	SKTransform();
	~SKTransform();

	FORCEINLINE const SKMatrix3X3W & GetCombine();
	FORCEINLINE const SKMatrix3X3W & GetCombineInverse();
	FORCEINLINE const SKVector3& GetScale()const;
	FORCEINLINE const SKVector3 & GetTranslate()const;
	FORCEINLINE const SKMatrix3X3 & GetRotate()const;
	FORCEINLINE void GetDir(SKVector3 &Dir,SKVector3 &Up,SKVector3 & Right)const;
	FORCEINLINE void SetScale(const SKVector3& fScale);
	FORCEINLINE void SetTranslate(const SKVector3& Translate);
	FORCEINLINE void SetRotate(const SKMatrix3X3 & Rotate);
	
	enum //Transform type
	{
		TT_POS,
		TT_DIR,
		TT_MAX
	};
	void ApplyForward(const SKVector3 & In, SKVector3 & Out,unsigned int uiTransformType);
	enum //Transform Flag
	{
		TF_SCALE = 1,
		TF_ROTATE = 2,
		TF_TRANSFORM = 4,
		TF_ALL = TF_SCALE | TF_TRANSFORM | TF_ROTATE
	};
	void Product(const SKTransform & t1,const SKTransform &t2,unsigned int TransformFlag = TF_ALL);
	void Inverse(SKTransform & Out)const;
	void ApplyInverse(const SKVector3 & In, SKVector3 & Out,unsigned int uiTransformType);
	
	void SetMatrix(const SKMatrix3X3W & SKMat);

	void Interpolation(const SKTransform &t1 , const SKTransform &t2,SKREAL t);

	void Indetity();
	const static SKTransform ms_Indetity;
};

#include "SKTransform.inl"
};
#endif