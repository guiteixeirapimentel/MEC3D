#pragma once
#include <cmath>
#include <cassert>
#include "Ponto.h"

class Vetor
{
public:
	Vetor(double x1, double x2, double x3) : cXXX{ x1, x2, x3 }, cX1(cXXX[0]), cX2(cXXX[1]), cX3(cXXX[2])
	{};
	// vetor de p1 para p2 => p2 - p1
	Vetor(const Ponto& p1, const Ponto& p2) : cXXX{ (p2.cX1 - p1.cX1),  (p2.cX2 - p1.cX2), (p2.cX3 - p1.cX3) },
		cX1(cXXX[0]), cX2(cXXX[1]), cX3(cXXX[2])
	{};
	Vetor() : cXXX{ 0.0, 0.0, 0.0 }, cX1(cXXX[0]), cX2(cXXX[1]), cX3(cXXX[2])
	{

	};
	Vetor(const Vetor& v) : cXXX{ v.cX1, v.cX2, v.cX3 }, cX1(cXXX[0]), cX2(cXXX[1]), cX3(cXXX[2])
	{

	};
	~Vetor() {};

	inline double& operator()(int i)
	{
		return cXXX[i - 1];
	}

	inline double operator()(int i) const
	{
		return cXXX[i - 1];		
	}

	inline Vetor operator-(const Ponto& rhs) const
	{
		return { cX1 - rhs.cX1, cX2 - rhs.cX2, cX3 - rhs.cX3 };
	}

	inline Vetor& operator=(const Vetor& rhs)
	{
		cX1 = rhs.cX1;
		cX2 = rhs.cX2;
		cX3 = rhs.cX3;

		return *this;
	}


	inline Vetor operator+(const Ponto& rhs) const
	{
		return { cX1 + rhs.cX1, cX2 + rhs.cX2, cX3 + rhs.cX3 };
	}

	inline Vetor operator+(const Vetor& rhs) const
	{
		return { rhs.cX1 + cX1, rhs.cX2 + cX2, rhs.cX3 + cX3 };
	}
	inline Vetor operator-(const Vetor& rhs) const
	{
		return { rhs.cX1 - cX1, rhs.cX2 - cX2, rhs.cX3 - cX3 };
	}

	inline Vetor& operator+=(const Vetor& rhs)
	{
		cX1 += rhs.cX1;
		cX2 += rhs.cX2;
		cX3 += rhs.cX3;

		return *this;
	}
	inline Vetor& operator-=(const Vetor& rhs)
	{
		cX1 -= rhs.cX1;
		cX2 -= rhs.cX2;
		cX3 -= rhs.cX3;

		return *this;
	}

	inline double operator*(const Vetor& rhs) const
	{		
		return rhs.cX1*cX1 + rhs.cX2*cX2 + rhs.cX3*cX3;
	}

	inline Vetor operator*(double rhs) const
	{
		return {cX1 * rhs, cX2 * rhs, cX3 * rhs };
	}
	inline Vetor operator/(double rhs) const
	{
		return { cX1/ rhs,  cX2 / rhs, cX3 / rhs };
	}
	inline Vetor& operator*=(double rhs)
	{
		cX1 *= rhs;
		cX2 *= rhs;
		cX3 *= rhs;
		return *this;
	}
	inline Vetor& operator/=(double rhs)
	{
		cX1 /= rhs;
		cX2 /= rhs;
		cX3 /= rhs;
		return *this;
	}

	double& operator[](int i)
	{
		return cXXX[i - 1];
	}

	double operator[](int i) const
	{
		return cXXX[i - 1];
	}

	Vetor Cross(const Vetor& rhs) const 
	{
		return { cX2*rhs.cX3 - cX3*rhs.cX2, 
				cX3*rhs.cX1 - cX1*rhs.cX3, 
				cX1*rhs.cX2 - cX2*rhs.cX1 };
	}

	Vetor& CrossEqual(const Vetor& rhs)
	{
		const double nX1 = cX2 * rhs.cX3 - cX3 * rhs.cX2;
		const double nX2 = cX3 * rhs.cX1 - cX1 * rhs.cX3;
		const double nX3 = cX1 * rhs.cX2 - cX2 * rhs.cX1;

		cX1 = nX1;
		cX2 = nX2;
		cX3 = nX3;

		return *this;
	}

	inline double ModSquared() const
	{
		return cX1 * cX1 + cX2 * cX2 + cX3*cX3;
	}

	inline double Mod() const
	{
		return sqrt(ModSquared());
	}

	double AngleBetween(const Vetor& v2) const
	{
		const double dotProd = (*this) * v2;

		const double cosalfa = dotProd / (v2.Mod() * Mod());

		return acos(cosalfa);
	}

	operator Ponto()
	{
		return { cX1, cX2, cX3 };
	}

public:
	double &cX1;
	double &cX2;
	double &cX3;

	double cXXX[3];

};