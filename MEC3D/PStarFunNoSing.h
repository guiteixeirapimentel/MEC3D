#pragma once
#define _USE_MATH_DEFINES 
#include <math.h>
#include "Vetor.h"

class PStarFunNoSing
{
public:
	PStarFunNoSing(double poisson) : cCoefPoisson(poisson), cCte(1.0 / (8.0 * M_PI * (1.0 - poisson))) {};

	double operator()(int i, int j, const Vetor& ksi, const Vetor& X, const Vetor& normal) const
	{
		const Vetor r = X - ksi;

		const double modrquad = r.ModSquared();
		const double modr = sqrt(modrquad);

		const double deltakr = (i == j) ? 1.0 : 0.0;

		const Vetor drdxi = r / modr;

		const double drdn = drdxi * normal;

 		const double drdi = r(i) / modr;
		const double drdj = r(j) / modr;
		
		return cCte * (((1.0 - 2.0 * cCoefPoisson) * deltakr + 3.0 * drdi * drdj) * drdn
			- (1.0 - 2.0 * cCoefPoisson) * (drdi * normal(j) - drdj * normal(i))
			);
	}
private:
	const double cCoefPoisson;

	const double cCte;
};