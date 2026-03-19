#include "SKVector2.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKVector2::SKVector2(void) 
{
	x=0;y=0;
}
/*----------------------------------------------------------------*/
SKVector2::SKVector2(SKREAL _x, SKREAL _y)
{
	x=_x; y=_y; 
}
/*----------------------------------------------------------------*/
void SKVector2::operator += (const SKVector2 &v)
{
	x += v.x;   y += v.y; 
}
/*----------------------------------------------------------------*/


SKVector2 SKVector2::operator + (const SKVector2 &v) const 
{
	return SKVector2(x+v.x, y+v.y);
}
/*----------------------------------------------------------------*/
void SKVector2::operator -= (const SKVector2 &v) 
{
	x -= v.x;   y -= v.y; 

}
/*----------------------------------------------------------------*/

SKVector2 SKVector2::operator - (const SKVector2 &v) const 
{
	return SKVector2(x-v.x, y-v.y);
}
/*----------------------------------------------------------------*/

void SKVector2::operator *= (SKREAL f) 
{
	x *= f;   y *= f;  
}
/*----------------------------------------------------------------*/

void SKVector2::operator /= (SKREAL f) 
{
	x /= f;   y /= f; 
}
/*----------------------------------------------------------------*/

SKVector2 SKVector2::operator * (SKREAL f) const 
{
	return SKVector2(x*f, y*f);
}
/*----------------------------------------------------------------*/

SKVector2 SKVector2::operator / (SKREAL f) const 
{
	return SKVector2(x/f, y/f);
}
/*----------------------------------------------------------------*/

void SKVector2::operator += (SKREAL f) 
{
	x += f;   y += f; 
}
/*----------------------------------------------------------------*/

void SKVector2::operator -= (SKREAL f) 
{
	x -= f;   y -= f; 
}
/*----------------------------------------------------------------*/

SKVector2 SKVector2::operator + (SKREAL f) const 
{
	return SKVector2(x+f, y+f);
}
/*----------------------------------------------------------------*/

SKVector2 SKVector2::operator - (SKREAL f) const 
{
	return SKVector2(x-f, y-f);
}
/*----------------------------------------------------------------*/

SKREAL SKVector2::operator * (const SKVector2 &v) const 
{
	return (v.x*x + v.y*y);
}
/*----------------------------------------------------------------*/
bool SKVector2::operator ==(const SKVector2 &v)const
{
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
// 	return (ABS(x-v.x) < EPSILON_E4 && 
// 		ABS(y-v.y) < EPSILON_E4);

}