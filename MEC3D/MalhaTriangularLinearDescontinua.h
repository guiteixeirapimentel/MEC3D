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
		const double porcDistCMVert = 0.475)
		:
		MalhaTriangular(pontos, idsNohsElementos, pressPrescritas, deslPrescritos)
	{
		const int nElementos = idsNohsElementos.size();

		cPontosInternos.resize(nElementos);

		for (int i = 0; i < nElementos; i++)
		{
			const int id1 = idsNohsElementos[i].idPonto1 - 1;
			const int id2 = idsNohsElementos[i].idPonto2 - 1;
			const int id3 = idsNohsElementos[i].idPonto3 - 1;
			
			const Ponto p1 = pontos[id1] + ((cElementos[i].triangulo.cCM - pontos[id1]) * porcDistCMVert);
			const Ponto p2 = pontos[id2] + ((cElementos[i].triangulo.cCM - pontos[id2]) * porcDistCMVert);
			const Ponto p3 = pontos[id3] + ((cElementos[i].triangulo.cCM - pontos[id3]) * porcDistCMVert);

			cPontosInternos[i] = { p1, p2, p3 };
		}
	}

	~MalhaTriangularLinearDescontinua() {};

public:
	std::vector<PontosInternos> cPontosInternos;
};