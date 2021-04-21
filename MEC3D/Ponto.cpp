#include "Ponto.h"
#include "Vetor.h"

Vetor Ponto::operator-(const Vetor& rhs) const
{
	return { cX1 - rhs.cX1, cX2 - rhs.cX2, cX3 - rhs.cX3 };
}

Vetor Ponto::operator+(const Vetor& rhs) const
{
	return { cX1 + rhs.cX1, cX2 + rhs.cX2, cX3 + rhs.cX3 };
}

Ponto::operator Vetor() const
{
	return { cX1, cX2, cX3 };
}