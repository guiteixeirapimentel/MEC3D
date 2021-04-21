#pragma once
#define _USE_MATH_DEFINES 
#include <math.h>
#include "Vetor.h"

// Solução complementar de Mindlin
class FundSolUComp
{
public:
	FundSolUComp(double poisson, double G) : cCoefElastCis(G),
		cCoefPoisson(poisson),
		cKd(1.0 / (16.0 * M_PI * (1.0 - poisson) * cCoefElastCis)),
		cKBoussCerrut(1 / (2 * M_PI * G))
	{};

	double operator()(int i, int j, const Vetor& ksi, const Vetor& X, double c) const
	{
		if (c == 0.0)
			return 0.0;

		const Vetor ksiprime = ksi + Vetor(0.0, 0.0, 2.0 * c);
		const Vetor r = X - ksi;
		const Vetor R = X - ksiprime;
		const double xbarra = c + abs(ksi.cX3);

		const double modr = r.Mod();
		const double modR = R.Mod();

		double res = 0.0;

		if (i == 1)
		{
			if (j == 1)
			{
				res = cKd * (
					(8*(1-cCoefPoisson)* (1 - cCoefPoisson) - (3.0-4.0*cCoefPoisson))/modR
					+
					((3-4*cCoefPoisson)*R.cX1*R.cX1 - 2*c*xbarra)/(modR*modR*modR)
					+
					(6*c*xbarra*R.cX1*R.cX1)/(modR* modR* modR* modR* modR)
					);
			}
			else if (j == 2)
			{
				res = cKd * r.cX2*(
					(r.cX1 * (3 - 4*cCoefPoisson) ) / (modR* modR* modR)
					-
					(4*(1 - cCoefPoisson) *(1-2*cCoefPoisson)) / (modR *(modR + R.cX1))
					+
					(6 * c * xbarra * R.cX1) / (modR * modR * modR * modR * modR)
					);
			}
			else
			{
				res = cKd * r.cX3 * (
					(r.cX1 * (3 - 4 * cCoefPoisson)) / (modR * modR * modR)
					-
					(4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson)) / (modR * (modR + R.cX1))
					+
					(6 * c * xbarra * R.cX1) / (modR * modR * modR * modR * modR)
					);
			}
		}
		else if (i == 2)
		{
			if (j == 1)
			{
				res = cKd * r.cX2 *(
					(r.cX1 * (3 - 4 * cCoefPoisson)) / (modR * modR * modR)
					+
					(4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson)) / (modR * (modR + R.cX1))
					-
					(6 * c * xbarra * R.cX1) / (modR * modR * modR * modR * modR)
					);
			}
			else if (j == 2)
			{
				res = cKd * (
					1/modR
					+
					((3-4*cCoefPoisson)*r.cX2*r.cX2)/(modR*modR*modR)
					+
					2*c*xbarra/(modR*modR*modR) * (1 - 3*r.cX2*r.cX2 /(modR*modR))
					+
					(4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR + R.cX1)) * 
						(1 - (r.cX2 * r.cX2) / (modR * (modR + R.cX1)))
					);
			}
			else
			{
				res = cKd * r.cX2 * r.cX3 * (
					(3 - 4 * cCoefPoisson) / (modR * modR * modR)
					-
					(4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson)) / (modR * (modR + R.cX1)* (modR + R.cX1))
					-
					(6 * c * xbarra) / (modR * modR * modR * modR * modR)
					);
			}
		}
		else if (i == 3)
		{
			if (j == 1)
			{
				res = cKd * r.cX3 * (
					(r.cX1 * (3 - 4 * cCoefPoisson)) / (modR * modR * modR)
					+
					(4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson)) / (modR * (modR + R.cX1))
					-
					(6 * c * xbarra * R.cX1) / (modR * modR * modR * modR * modR)
					);
			}
			else if (j == 2)
			{				
				res = cKd * r.cX2 * r.cX3 * (
					(3 - 4 * cCoefPoisson) / (modR * modR * modR)
					-
					(4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson)) / (modR * (modR + R.cX1) * (modR + R.cX1))
					-
					(6 * c * xbarra) / (modR * modR * modR * modR * modR)
					);
			}
			else
			{
				res = cKd * (
					1 / modR
					+
					((3 - 4 * cCoefPoisson) * r.cX3 * r.cX3) / (modR * modR * modR)
					+
					2 * c * xbarra / (modR * modR * modR) * (1 - 3 * r.cX3 * r.cX3 / (modR * modR))
					+
					(4 * (1 - cCoefPoisson) * (1 - 2 * cCoefPoisson) / (modR + R.cX1)) *
					(1 - (r.cX3 * r.cX3) / (modR * (modR + R.cX1)))
					);
			}
		}

		return res;
	}

	double GetBoussinesqCerrutiSolNonSingPart(int i, int j, const Vetor& ksi, const Vetor& X, double c) const
	{
		assert(c == 0.0);
		const Vetor r = X - ksi;
		const double modr = r.Mod();
		const Vetor drdi = r / modr;

		if (i == 1)
		{
			if (j == 1)
			{
				const double u11 = cKBoussCerrut * ((1 - cCoefPoisson) + cCoefPoisson * drdi.cX1 * drdi.cX1);

				return u11;
			}
			else if (j == 2)
			{
				const double u12 = cKBoussCerrut * cCoefPoisson * drdi.cX1 * drdi.cX2;
				return u12;
			}
			else
			{
				const double u13 = cKBoussCerrut * (0.5 - cCoefPoisson) * drdi.cX1;
				return u13;
			}
		}
		else if (i == 2)
		{
			if (j == 1)
			{
				const double u21 = cKBoussCerrut * cCoefPoisson * drdi.cX1 * drdi.cX2;
				return u21;
			}
			else if (j == 2)
			{

				const double u22 = cKBoussCerrut * ((1 - cCoefPoisson) + cCoefPoisson * drdi.cX2 * drdi.cX2);
				return u22;
			}
			else
			{
				const double u23 = cKBoussCerrut * (0.5 - cCoefPoisson) * drdi.cX2;
				return u23;
			}
		}
		else if (i == 3)
		{
			if (j == 1)
			{
				const double u31 = -cKBoussCerrut * (0.5 - cCoefPoisson) * drdi.cX1;
				return u31;
			}
			else if (j == 2)
			{
				const double u32 = -cKBoussCerrut * (0.5 - cCoefPoisson) * drdi.cX2;
				return u32;
			}
			else
			{
				const double u33 = cKBoussCerrut * (1 - cCoefPoisson);
				return u33;
			}
		}

		return NAN;
	}

	double GetBoussinesqCerrutiSolSingPart(int i, int j, const Vetor& ksi, const Vetor& X, double c) const
	{
		assert(c == 0.0);
		const Vetor r = X - ksi;
		const double modr = r.Mod();
		const Vetor drdi = r / modr;

		if (i == 1)
		{
			if (j == 1)
			{
				const double u11 = cKBoussCerrut/modr * ((1 - cCoefPoisson) + cCoefPoisson * drdi.cX1 * drdi.cX1);

				return u11;
			}
			else if (j == 2)
			{
				const double u12 = cKBoussCerrut / modr * cCoefPoisson * drdi.cX1 * drdi.cX2;
				return u12;
			}
			else
			{
				const double u13 = cKBoussCerrut / modr * (0.5 - cCoefPoisson) * drdi.cX1;
				return u13;
			}
		}
		else if (i == 2)
		{
			if (j == 1)
			{
				const double u21 = cKBoussCerrut / modr * cCoefPoisson * drdi.cX1 * drdi.cX2;
				return u21;
			}
			else if (j == 2)
			{

				const double u22 = cKBoussCerrut / modr * ((1 - cCoefPoisson) + cCoefPoisson * drdi.cX2 * drdi.cX2);
				return u22;
			}
			else
			{
				const double u23 = cKBoussCerrut / modr * (0.5 - cCoefPoisson) * drdi.cX2;
				return u23;
			}
		}
		else if (i == 3)
		{
			if (j == 1)
			{
				const double u31 = -cKBoussCerrut / modr * (0.5 - cCoefPoisson) * drdi.cX1;
				return u31;
			}
			else if (j == 2)
			{
				const double u32 = -cKBoussCerrut / modr * (0.5 - cCoefPoisson) * drdi.cX2;
				return u32;
			}
			else
			{
				const double u33 = cKBoussCerrut / modr * (1 - cCoefPoisson);
				return u33;
			}
		}

		return NAN;
	}
private:
	const double cCoefPoisson;
	const double cCoefElastCis;

	const double cKBoussCerrut;

	const double cKd;
};