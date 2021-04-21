#pragma once
#define _USE_MATH_DEFINES 
#include <math.h>
#include "Vetor.h"
#include "FundSolUComp.h"
#include "FundSolUStar.h"

// Solução completa de mindlin
class FundSolUMindlin
{
public:
	FundSolUMindlin(double poisson, double G) : fundSolComp(poisson, G), fundSolStar(poisson, G) {};

	double operator()(int i, int j, const Vetor& ksi, const Vetor& X, double c) const
	{		
		const double res = fundSolComp(i, j, ksi, X, c) + fundSolStar(i, j, ksi, X);

		return res;
	}

	double GetBoussinesqCerrutiSolNonSingPart(int i, int j, const Vetor& ksi, const Vetor& X, double c) const
	{
		return fundSolComp.GetBoussinesqCerrutiSolNonSingPart(i, j, ksi, X, c);
	}

	double GetBoussinesqCerrutiSolSingPart(int i, int j, const Vetor& ksi, const Vetor& X, double c) const
	{
		return fundSolComp.GetBoussinesqCerrutiSolSingPart(i, j, ksi, X, c);
	}

	const FundSolUComp& GetCompPart() const
	{
		return fundSolComp;
	}
private:

	const FundSolUComp fundSolComp;
	const FundSolUStar fundSolStar;
};