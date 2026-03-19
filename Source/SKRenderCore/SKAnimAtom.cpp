#include "SKAnimAtom.h"
#include "SKOtherMath.h"
#include "SKTransform.h"
using namespace SKEngine2;
void SKAnimAtom::Identity()
{
	m_fScale.Set(1.0f,1.0f,1.0f);
	m_Pos.Set(0.0f,0.0f,0.0f);
	m_Rotator.Set(0.0f,0.0f,0.0f,1.0f);

}
void SKAnimAtom::GetMatrix(SKMatrix3X3W & OutMatrix)const
{
	SKMatrix3X3 mRotate;
	m_Rotator.GetMatrix(mRotate);

	SKMatrix3X3 Mat;
	Mat = SKMatrix3X3(	mRotate._00 * m_fScale.x,mRotate._01 * m_fScale.x,mRotate._02 * m_fScale.x,
						mRotate._10 * m_fScale.y,mRotate._11 * m_fScale.y,mRotate._12 * m_fScale.y,
						mRotate._20 * m_fScale.z,mRotate._21 * m_fScale.z,mRotate._22 * m_fScale.z);
	OutMatrix.AddTranslate(m_Pos);
	OutMatrix.Add3X3(Mat);
}
void SKAnimAtom::FromTransfrom(const SKTransform& T)
{
	m_fScale = T.GetScale();
	m_Pos = T.GetTranslate();
	m_Rotator =T.GetRotate().GetQuat();
}
void SKAnimAtom::FromMatrix(const SKMatrix3X3W & m)
{
	SKTransform T;
	T.SetMatrix(m);
	FromTransfrom(T);
}
void SKAnimAtom::Interpolation(const SKAnimAtom & Atom1 , const SKAnimAtom Atom2,SKREAL t)
{
	while (t < 0.0f)
	{
		t += 1.0f;
	}
	while(t > 1.0f)
	{
		t -= 1.0f;
	}

	m_fScale = LineInterpolation(Atom1.m_fScale,Atom2.m_fScale,t);
	m_Pos = LineInterpolation(Atom1.m_Pos,Atom2.m_Pos,t);
	m_Rotator.Slerp(t,Atom1.m_Rotator,Atom2.m_Rotator);

	


}
void SKAnimAtom::BlendWith(const SKAnimAtom &Atom)
{
	m_fScale += Atom.m_fScale;
	m_Pos += Atom.m_Pos;

	m_Rotator *= Atom.m_Rotator;
}
SKAnimAtom SKAnimAtom::Blend(const SKAnimAtom &Atom)
{
	SKAnimAtom Temp;

	Temp.m_fScale = m_fScale + Atom.m_fScale;
	Temp.m_Pos = m_Pos + Atom.m_Pos;

	Temp.m_Rotator = m_Rotator * Atom.m_Rotator;
	return Temp;
}
void    SKAnimAtom::operator *= (SKREAL f)
{
	m_fScale *= f;
	m_Pos *= f;

	m_Rotator = m_Rotator.Pow(f);

	//m_Rotator *= f;

}
SKAnimAtom SKAnimAtom::operator *  (SKREAL f)
{
	SKAnimAtom Temp ;
	Temp.m_fScale = m_fScale * f;
	Temp.m_Pos = m_Pos * f;

	Temp.m_Rotator = m_Rotator.Pow(f);

	//Temp.m_Rotator = m_Rotator * f;

	return Temp;

}
void SKAnimAtom::AddTwo(const SKAnimAtom & Atom1)
{
	m_Pos = m_Pos + Atom1.m_Pos;
	m_fScale = m_fScale * Atom1.m_fScale;
	m_Rotator = m_Rotator * Atom1.m_Rotator;
}
void SKAnimAtom::AddTwo(const SKAnimAtom & Atom1, const SKAnimAtom Atom2)
{
	m_Pos = Atom1.m_Pos + Atom2.m_Pos;
	m_fScale = Atom1.m_fScale * Atom2.m_fScale;
	m_Rotator = Atom1.m_Rotator * Atom2.m_Rotator;
}
void SKAnimAtom::operator += (const SKAnimAtom &Atom)
{
	SKMatrix3X3W T1;
	GetMatrix(T1);

	SKMatrix3X3W T2;

	Atom.GetMatrix(T2);

	SKMatrix3X3W T = T1 * T2;

	FromMatrix(T);
}
SKAnimAtom SKAnimAtom::operator +  (const SKAnimAtom &Atom) const
{
	SKAnimAtom Temp ;
	SKMatrix3X3W T1;
	GetMatrix(T1);

	SKMatrix3X3W T2;

	Atom.GetMatrix(T2);

	SKMatrix3X3W T = T1 * T2;

	Temp.FromMatrix(T);
	return Temp;
}
void SKAnimAtom::operator -= (const SKAnimAtom &Atom)
{
	SKMatrix3X3W T1;
	GetMatrix(T1);

	SKMatrix3X3W T2;

	Atom.GetMatrix(T2);

	SKMatrix3X3W T = T2.GetInverse() * T1;

	FromMatrix(T);
}
SKAnimAtom SKAnimAtom::operator -  (const SKAnimAtom &Atom) const
{
	SKAnimAtom Temp;
	SKMatrix3X3W T1;
	GetMatrix(T1);

	SKMatrix3X3W T2;

	Atom.GetMatrix(T2);

	SKMatrix3X3W T = T2.GetInverse() * T1;

	Temp.FromMatrix(T);
	return Temp;
}