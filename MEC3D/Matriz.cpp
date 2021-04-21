#include "Matriz.h"

#include "Quad.h"

Quad Matriz::operator*(const Quad& rhs) const
{
	Quad res;
	res.p1 = *this * rhs.p1;
	res.p2 = *this * rhs.p2;
	res.p3 = *this * rhs.p3;
	res.p4 = *this * rhs.p4;

	return res;
}