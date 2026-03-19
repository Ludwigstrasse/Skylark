#include "SKCylinder3.h"
using namespace SKEngine2;
SKCylinder3::SKCylinder3(SKVector3 Center,SKVector3 Dir,SKREAL fRadius, SKREAL fHalfHeight)
{
	m_Center = Center;
	m_Dir = Dir;
	m_fRadius = fRadius;
	m_fHalfHeight = fHalfHeight;
}
SKCylinder3::~SKCylinder3()
{

}