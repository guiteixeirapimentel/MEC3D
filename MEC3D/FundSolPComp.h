#pragma once
#define _USE_MATH_DEFINES 
#include <math.h>
#include "Vetor.h"

class FundSolPComp
{
public:
	FundSolPComp(double poisson) : cCoefPoisson(poisson), cKs(1.0 / (8.0 * M_PI * (1.0 - poisson))) {};

	double operator()(int i, int j, const Vetor& ksi, const Vetor& X, const Vetor& n, double c) const
	{
		if (c == 0.0)
			return 0.0;

		const Vetor ksiprime = ksi + Vetor(0.0, 0.0, 2.0 * c);
		const Vetor r = X - ksi;
		const Vetor R = X - ksiprime;
		const double xbarra = c + abs(ksi.cX3);

		const double modr = r.Mod();
		const double modR = R.Mod();

		const double modrquad = r.ModSquared();

		double res = 0.0;

		const double sig111 = cKs * (
			(1-2*cCoefPoisson)*r.cX1 / (modR*modR*modR)
			-
			(3*(3-4*cCoefPoisson)*xbarra*R.cX1* R.cX1 - 3*c*R.cX1*(5*xbarra-c))/(modR * modR * modR * modR* modR)
			-
			(30*c*xbarra*R.cX1* R.cX1* R.cX1)/ (modR * modR * modR * modR * modR * modR * modR)
			);
		
		const double sig121 = cKs * r.cX2 * (
			(1-2*cCoefPoisson)/(modR*modR*modR)
			-
			(3 * (3 - 4 * cCoefPoisson) * xbarra * R.cX1 - 3 * c * (3 * xbarra + c)) / (modR * modR * modR * modR * modR)
			-
			(30 * c * xbarra * R.cX1 * R.cX1) / (modR * modR * modR * modR * modR * modR * modR)
			);

		const double sig131 = cKs * r.cX3*(
			(1-2*cCoefPoisson)/(modR*modR*modR)
			-
			(3*(3-4*cCoefPoisson)*xbarra*R.cX1 - 3*c*(3*xbarra+c))/(modR * modR * modR * modR * modR)
			-
			(30 * c * xbarra * R.cX1 * R.cX1) / (modR * modR * modR * modR * modR * modR * modR)
		);

		const double sig221 = cKs * (
			(1 - 2 * cCoefPoisson)*(3*r.cX1-4*cCoefPoisson*R.cX1) / (modR * modR * modR)
			-
			(3 * (3 - 4 * cCoefPoisson) * r.cX2* r.cX2* r.cX1 - 6 * c * R.cX1*((1-2*cCoefPoisson)*xbarra - 2*cCoefPoisson*c)) / (modR * modR * modR * modR * modR)
			-
			(30 * c * xbarra * r.cX2 * r.cX2*R.cX1) / (modR * modR * modR * modR * modR * modR * modR)
			-
			4*(1-cCoefPoisson)*(1-2*cCoefPoisson)/(modR*(modR + R.cX1))*(1- r.cX2*r.cX2/(modR*(modR + R.cX1)) - r.cX2*r.cX2)/(modR*modR)
		);

		const double sig231 = cKs * r.cX2 * r.cX3 * (

			-(3 * (3 - 4 * cCoefPoisson) * r.cX1) / (modR * modR * modR* modR* modR)
			+
			4*(1-cCoefPoisson)*(1-2*cCoefPoisson)/(modR*modR*(modR + R.cX1)) * (1/(modR + R.cX1) + 1/modR)
			-
			30*c*xbarra*R.cX1/(modR* modR* modR* modR* modR* modR* modR)
			);
		
		const double sig331 = cKs * (
			(1 - 2 * cCoefPoisson) * (3 * r.cX1 - 4 * cCoefPoisson * R.cX1) / (modR * modR * modR)
			-
			(3 * (3 - 4 * cCoefPoisson) * r.cX3 * r.cX3 * r.cX1 - 6 * c * R.cX1 * ((1 - 2 * cCoefPoisson) * xbarra - 2 * cCoefPoisson * c)) / (modR * modR * modR * modR * modR)
			-
			(30 * c * xbarra * r.cX3 * r.cX3 * R.cX1) / (modR * modR * modR * modR * modR * modR * modR)
			-
			4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR * (modR + R.cX1)) * (1 - r.cX3 * r.cX3 / (modR * (modR + R.cX1)) - r.cX3 * r.cX3) / (modR * modR)
			);

		const double sig112 = cKs * r.cX2 * (
			- (1-2*cCoefPoisson)/(modR*modR*modR)
			-
			(3*(3-4*cCoefPoisson)*R.cX1 *R.cX1)/(modR* modR* modR* modR* modR)
			+
			6*c*(c + (1-2*cCoefPoisson)*R.cX1 + 5*xbarra*R.cX1*R.cX1/(modR*modR))/(modR * modR * modR * modR * modR)
			);

		const double sig122 = cKs * (
			(1 - 2 * cCoefPoisson)*r.cX1 / (modR * modR * modR)
			-
			(3 * (3 - 4 * cCoefPoisson) * r.cX2 * r.cX2 * R.cX1) / (modR * modR * modR * modR * modR)
			-
			6 * c * (xbarra*R.cX1 - (1 - 2 * cCoefPoisson) * r.cX2 * r.cX2 - 5 * xbarra * R.cX1 * r.cX2 * r.cX2 / (modR * modR))/(modR*modR*modR*modR*modR)
			);

		const double sig132 = cKs * r.cX2* r.cX3*(
			-
			(3 * (3 - 4 * cCoefPoisson) * R.cX1) / (modR * modR * modR * modR * modR)
			+
			6 * c * ((1 - 2 * cCoefPoisson) + 5 * xbarra * R.cX1  / (modR * modR))/(modR * modR * modR * modR * modR)
			);

		const double sig222 = cKs * r.cX2 * (
			(1-2*cCoefPoisson)*(5-4*cCoefPoisson)/(modR*modR*modR)
			-
			3*(3-4*cCoefPoisson)*r.cX2*r.cX2 / (modR* modR* modR* modR* modR)
			-
			4*(1-cCoefPoisson)*(1-2*cCoefPoisson)/(modR*(modR+R.cX1)* (modR + R.cX1)) * (3 - r.cX2*r.cX2*(3*modR + R.cX1)/(modR*modR*(modR + R.cX1)))
			+
			6*c/(modR*modR*modR*modR*modR)*(3*c - (3-2*cCoefPoisson)*R.cX1 + 5*r.cX2*r.cX2*xbarra/(modR*modR))
			);

		const double sig232 = cKs * r.cX3 * (
			(1 - 2 * cCoefPoisson) / (modR * modR * modR)
			-
			3 * (3 - 4 * cCoefPoisson) * r.cX2 * r.cX2 / (modR * modR * modR * modR * modR)
			-
			4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR * (modR + R.cX1) * (modR + R.cX1)) * (1 - r.cX2 * r.cX2 * (3 * modR + R.cX1) / (modR * modR * (modR + R.cX1)))
			-
			6 * c*xbarra / (modR * modR * modR * modR * modR) * (1 - 5*r.cX2*r.cX2/(modR*modR))
			);

		const double sig332 = cKs * r.cX2 * (
			(1 - 2 * cCoefPoisson) * (3 - 4 * cCoefPoisson) / (modR * modR * modR)
			-
			3 * (3 - 4 * cCoefPoisson) * r.cX3 * r.cX3 / (modR * modR * modR * modR * modR)
			-
			4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR * (modR + R.cX1) * (modR + R.cX1)) * (1 - r.cX3 * r.cX3 * (3 * modR + R.cX1) / (modR * modR * (modR + R.cX1)))
			+
			6 * c / (modR * modR * modR * modR * modR) * (c - (1 - 2 * cCoefPoisson) * R.cX1 + 5 * r.cX3 * r.cX3 * xbarra / (modR * modR))
			);

		const double sig113 = cKs * r.cX3*(
			-(1 - 2 * cCoefPoisson) / (modR * modR * modR)
			-
			(3 * (3 - 4 * cCoefPoisson) * R.cX1 * R.cX1) / (modR * modR * modR * modR * modR)
			+
			6 * c * (c + (1 - 2 * cCoefPoisson) * R.cX1 + 5 * xbarra * R.cX1 * R.cX1 / (modR * modR)) / (modR * modR * modR * modR * modR)
			);

		const double sig123 = sig132;

		const double sig133 = cKs* (
			(1 - 2 * cCoefPoisson) * r.cX1 / (modR * modR * modR)
			-
			3 * (3 - 4 * cCoefPoisson) * r.cX3 * r.cX3*R.cX1 / (modR * modR * modR * modR * modR)			
			-
			6 * c / (modR * modR * modR * modR * modR) * (xbarra*R.cX1 - (1 - 2 * cCoefPoisson) * r.cX3*r.cX3 - 5 * r.cX3 * r.cX3 * xbarra*R.cX1 / (modR * modR))
			);

		const double sig223 = cKs * r.cX3 * (
			(1 - 2 * cCoefPoisson) * (3 - 4 * cCoefPoisson) / (modR * modR * modR)
			-
			3 * (3 - 4 * cCoefPoisson) * r.cX2 * r.cX2 / (modR * modR * modR * modR * modR)
			-
			4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR * (modR + R.cX1) * (modR + R.cX1)) * (1 - r.cX2 * r.cX2 * (3 * modR + R.cX1) / (modR * modR * (modR + R.cX1)))
			+
			6 * c / (modR * modR * modR * modR * modR) * (c - (1 - 2 * cCoefPoisson) * R.cX1 + 5 * r.cX2 * r.cX2 * xbarra / (modR * modR))
			);

		const double sig233 = cKs * r.cX2 * (
			(1 - 2 * cCoefPoisson) / (modR * modR * modR)
			-
			3 * (3 - 4 * cCoefPoisson) * r.cX3 * r.cX3 / (modR * modR * modR * modR * modR)
			-
			4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR * (modR + R.cX1) * (modR + R.cX1)) * (1 - r.cX3 * r.cX3 * (3 * modR + R.cX1) / (modR * modR * (modR + R.cX1)))
			-
			6 * c*xbarra / (modR * modR * modR * modR * modR) * (1 - 5*r.cX3*r.cX3/(modR*modR))
			);

		const double sig333 = cKs * r.cX3 * (
			(1 - 2 * cCoefPoisson) * (5 - 4 * cCoefPoisson) / (modR * modR * modR)
			-
			3 * (3 - 4 * cCoefPoisson) * r.cX3 * r.cX3 / (modR * modR * modR * modR * modR)
			-
			4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR * (modR + R.cX1) * (modR + R.cX1)) * (3 - r.cX3 * r.cX3 * (3 * modR + R.cX1) / (modR * modR * (modR + R.cX1)))
			+
			6 * c / (modR * modR * modR * modR * modR) * (3*c - (3 - 2 * cCoefPoisson) * R.cX1 + 5 * r.cX3 * r.cX3 * xbarra / (modR * modR))
			);

		// teste abaixo

		const double sig211 = sig121;
		const double sig311 = sig131;
		const double sig321 = sig231;

		const double sig212 = sig122;
		const double sig312 = sig132;
		const double sig322 = sig232;

		const double sig213 = sig123;

		const double sig313 = sig133;

		const double sig323 = sig233;

		if (i == 1)
		{
			if (j == 1)
			{
				res = sig111 * n.cX1 + sig121 * n.cX2 + sig131 * n.cX3;
			}
			else if (j == 2)
			{
				res = sig211 * n.cX1 + sig221 * n.cX2 + sig231 * n.cX3;
			}
			else
			{
				res = sig311 * n.cX1 + sig321 * n.cX2 + sig331 * n.cX3;
			}
		}
		else if (i == 2)
		{
			if (j == 1)
			{
				res = sig112 * n.cX1 + sig122 * n.cX2 + sig132 * n.cX3;
			}
			else if (j == 2)
			{
				res = sig212 * n.cX1 + sig222 * n.cX2 + sig232 * n.cX3;
			}
			else
			{
				res = sig312 * n.cX1 + sig322 * n.cX2 + sig332 * n.cX3;
			}
		}
		else if (i == 3)
		{
			if (j == 1)
			{
				res = sig113 * n.cX1 + sig123 * n.cX2 + sig133 * n.cX3;
			}
			else if (j == 2)
			{
				res = sig213 * n.cX1 + sig223 * n.cX2 + sig233 * n.cX3;
			}
			else
			{
				res = sig313 * n.cX1 + sig323 * n.cX2 + sig333 * n.cX3;
			}
		}

		return res;
	}

	
private:
	const double cCoefPoisson;
	const double cKs;

};