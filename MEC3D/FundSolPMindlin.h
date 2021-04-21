#pragma once
#define _USE_MATH_DEFINES 
#include <math.h>
#include "Vetor.h"

#include "FundSolPComp.h"
#include "FundSolPStar.h"

class FundSolPMindlin
{
public:
	FundSolPMindlin(double poisson) : fundSolComp(poisson), fundSolStar(poisson) {};

	double operator()(int i, int j, const Vetor& ksi, const Vetor& X, const Vetor& n, double c) const
	{
		double res = fundSolComp(i, j, ksi, X, n, c) + fundSolStar(i, j, ksi, X, n);
		
		return res;
	}
private:
	FundSolPComp fundSolComp;
	FundSolPStar fundSolStar;
};