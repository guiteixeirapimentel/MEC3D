#pragma once
#define _USE_MATH_DEFINES 
#include <math.h>
#include "Vetor.h"

class FundSolUStar
{
public:
	FundSolUStar(double poisson, double G) : cCoefElastCis(G), cCoefPoisson(poisson), cCte(1.0/(16.0*M_PI*(1.0-poisson)*cCoefElastCis)) {};

	double operator()(int i, int j, const Vetor& ksi, const Vetor& X) const
	{
		const Vetor r = X - ksi;

		const double modr = r.Mod();

		const double deltakr = (i == j) ? 1.0 : 0.0;

		const double drdi = r(i) / modr;
		const double drdj = r(j) / modr;

		if (modr == 0)
			return NAN;

		return cCte / modr * (deltakr * (3.0 - 4.0 * cCoefPoisson) + drdi * drdj);
	}
private:
	const double cCoefPoisson;
	const double cCoefElastCis;

	const double cCte;
};