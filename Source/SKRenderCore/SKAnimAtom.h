#ifndef SKANIMATOM_H
#define SKANIMATOM_H
#include "SKMatrix3X3W.h"
#include "SKGraphic.h"
#include "SKTransform.h"
/*
	

*/
namespace SKEngine2
{
	class SKGRAPHIC_API SKAnimAtom
	{
	public:
		SKAnimAtom()
		{
			Identity();
		}
		~SKAnimAtom()
		{
			
		}
		static SKAnimAtom Create();
		SKVector3	m_fScale;
		SKVector3	m_Pos;
		SKQuat		m_Rotator;
		void GetMatrix(SKMatrix3X3W & OutMatrix)const;
		void Interpolation(const SKAnimAtom & Atom1 , const SKAnimAtom Atom2,SKREAL t);
		void Identity();
		void FromTransfrom(const SKTransform &T);
		void FromMatrix(const SKMatrix3X3W & m);

		void    operator *= (SKREAL f);
		SKAnimAtom operator *  (SKREAL f);

		void    operator += (const SKAnimAtom &Atom);
		SKAnimAtom operator +  (const SKAnimAtom &Atom) const;
		void    operator -= (const SKAnimAtom &Atom);
		SKAnimAtom operator -  (const SKAnimAtom &Atom) const;
		void BlendWith(const SKAnimAtom &Atom);
		SKAnimAtom Blend(const SKAnimAtom &Atom);

		void AddTwo(const SKAnimAtom & Atom1, const SKAnimAtom Atom2);
		void AddTwo(const SKAnimAtom & Atom1);
	};
	
}
#endif