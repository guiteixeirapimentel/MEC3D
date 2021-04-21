#pragma once
#include "Vetor.h"

class Triangulo
{
public:	
	Triangulo(const Ponto& p1, const Ponto& p2, const Ponto& p3)
		:
		cP1(p1),
		cP2(p2),
		cP3(p3),
		cV1(cP1, cP2),
		cV2(cP1, cP3)
	{
		Vetor norm = cV1.Cross(cV2);
		const double modn = norm.Mod();
		cArea = modn / 2.0;

		cNormalU = norm / modn;

		cCM = Vetor(cP1.cX1 + cP2.cX1 + cP3.cX1, cP1.cX2 + cP2.cX2 + cP3.cX2, cP1.cX3 + cP2.cX3 + cP3.cX3) / 3.0;

		const Vetor vcm1({ cCM.cX1, cCM.cX2, cCM.cX3 }, cP1);
		const Vetor vcm2({ cCM.cX1, cCM.cX2, cCM.cX3 }, cP2);
		const Vetor vcm3({ cCM.cX1, cCM.cX2, cCM.cX3 }, cP3);

		cTheta1 = vcm1.AngleBetween(vcm2);
		cTheta2 = vcm2.AngleBetween(vcm3) + cTheta1;
		cTheta3 = vcm3.AngleBetween(vcm1) + cTheta2;

		const double L1 = (cP1 - cP2).Mod();
		const double L2 = (cP2 - cP3).Mod();
		const double L3 = (cP3 - cP1).Mod();

		cLadoMedio = (L1 + L2 + L3) / 3.0;

		assert(abs(cTheta3 - 2.0* 3.14159265359) < 1e-6);
	}
	Triangulo() : cArea(0.0), cTheta1(0.0), cTheta2(0.0), cTheta3(0.0), cLadoMedio(0.0) {};
	~Triangulo() {};

	Vetor GetGlobalFromHomogeneous(const Vetor& homogeneous) const
	{
		return {cP1.cX1* homogeneous.cX1 + cP2.cX1 * homogeneous.cX2 + cP3.cX1 * homogeneous.cX3,
				cP1.cX2* homogeneous.cX1 + cP2.cX2 * homogeneous.cX2 + cP3.cX2 * homogeneous.cX3,
				cP1.cX3* homogeneous.cX1 + cP2.cX3 * homogeneous.cX2 + cP3.cX3 * homogeneous.cX3,
				};
	}

	static Vetor GetGlobalFromHomogeneous(const Vetor& homogeneous, const Vetor& P1, const Vetor& P2, const Vetor& P3)
	{
		return { P1.cX1 * homogeneous.cX1 + P2.cX1 * homogeneous.cX2 + P3.cX1 * homogeneous.cX3,
				P1.cX2 * homogeneous.cX1 +  P2.cX2 * homogeneous.cX2 + P3.cX2 * homogeneous.cX3,
				P1.cX3 * homogeneous.cX1 +  P2.cX3 * homogeneous.cX2 + P3.cX3 * homogeneous.cX3,
		};
	}

public:
	Ponto cP1;
	Ponto cP2;
	Ponto cP3;

	Vetor cV1;
	Vetor cV2;

	Vetor cCM;
	
	double cArea;	

	double cLadoMedio;

	// angulo entre CM-1 e CM-2
	double cTheta1;
	// angulo entre CM-2 e CM-3
	double cTheta2;
	// angulo entre CM-3 e CM-1
	double cTheta3;

	Vetor cNormalU;
};