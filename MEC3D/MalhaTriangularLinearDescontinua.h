#pragma once
#include "MalhaTriangular.h"

class MalhaTriangularLinearDescontinua : public MalhaTriangular
{
public:
	struct PontosInternos
	{
		Ponto p123[3];
	};

public:
	MalhaTriangularLinearDescontinua(const std::vector<Ponto>& pontos, const std::vector<InstElemento> idsNohsElementos,
		const std::vector<CondContorno>& pressPrescritas, const std::vector<CondContorno>& deslPrescritos, 
		const double cValue = 0.1625)
		:
		MalhaTriangular(pontos, idsNohsElementos, pressPrescritas, deslPrescritos)
	{
		const double c1 = 0.1625;
		const double c2 = 0.1625;
		const double c3 = 0.1625;
		const double c4 = 1.0 - c1 - c2 - c3;
		
		const int nElementos = idsNohsElementos.size();

		cPontosInternos.resize(nElementos);
		
		for (int i = 0; i < nElementos; i++)
		{
			const int id1 = idsNohsElementos[i].idPonto1 - 1;
			const int id2 = idsNohsElementos[i].idPonto2 - 1;
			const int id3 = idsNohsElementos[i].idPonto3 - 1;

			Matriz X(9, 1, { pontos[id1][0],pontos[id2][0],pontos[id3][0],
				pontos[id1][1],pontos[id2][1],pontos[id3][1],
				pontos[id1][2],pontos[id2][2],pontos[id3][2], });

			Ponto p1;
			Ponto p2;
			Ponto p3;
			
			{
				const double ksi1b = 1.0;
				const double ksi2b = 0.0;
				const double ksi3b = 0.0;

				const double ksi1 = ksi1b * c4 + c1;
				const double ksi2 = ksi2b * c4 + c2;
				const double ksi3 = ksi3b * c4 + c3;

				Matriz phiT(3, 9, { ksi1, ksi2, ksi3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
									 0.0, 0.0, 0.0, ksi1, ksi2, ksi3, 0.0, 0.0, 0.0,
									 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, ksi1, ksi2, ksi3 });

				Matriz PM = phiT * X;

				p1 = { PM[0], PM[1], PM[2] };
			}

			{
				const double ksi1b = 0.0;
				const double ksi2b = 1.0;
				const double ksi3b = 0.0;

				const double ksi1 = ksi1b * c4 + c1;
				const double ksi2 = ksi2b * c4 + c2;
				const double ksi3 = ksi3b * c4 + c3;

				Matriz phiT(3, 9, { ksi1, ksi2, ksi3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
									 0.0, 0.0, 0.0, ksi1, ksi2, ksi3, 0.0, 0.0, 0.0,
									 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, ksi1, ksi2, ksi3 });

				Matriz PM = phiT * X;

				p2 = { PM[0], PM[1], PM[2] };
			}

			{
				const double ksi1b = 0.0;
				const double ksi2b = 0.0;
				const double ksi3b = 1.0;

				const double ksi1 = ksi1b * c4 + c1;
				const double ksi2 = ksi2b * c4 + c2;
				const double ksi3 = ksi3b * c4 + c3;

				Matriz phiT(3, 9, { ksi1, ksi2, ksi3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
									 0.0, 0.0, 0.0, ksi1, ksi2, ksi3, 0.0, 0.0, 0.0,
									 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, ksi1, ksi2, ksi3 });

				Matriz PM = phiT * X;

				p3 = { PM[0], PM[1], PM[2] };
			}
			
			
			cPontosInternos[i] = { p1, p2, p3 };
		}
	}

	~MalhaTriangularLinearDescontinua() {};

public:
	std::vector<PontosInternos> cPontosInternos;
};