#include "SKRect2.h"
using namespace SKEngine2;
SKRect2::SKRect2(void)
{

}
SKRect2::SKRect2(const SKVector2& Min, const SKVector2 &Max)
{
	m_Min = Min;
	m_Max = Max;
}