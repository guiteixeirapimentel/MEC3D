#pragma once
#include <vector>
#include "Vetor.h"
#include "Triangulo.h"

#include "DataPoint.h"

#include "Quad.h"

#include "ColorShader.h"

#include <iostream>


struct ElementoTriangular
{
	Triangulo triangulo;
};

// dado um elemento, indices que representam o elemento.
struct InstElemento
{
	int idPonto1;
	int idPonto2;
	int idPonto3;

};

// estrutura que guarda o id do elemento, o grau de liberdade e o valor imposto.
struct CondContorno
{
	int idEle; // 1
	int grauLib; // 2
	double val; // 3
};

class MalhaTriangular
{
public:
	// lembrar que utiliza sistema de numeracao do matlab ( 1 -> n; o construtor subtrai 1 de todos => 0 -> n - 1)
	MalhaTriangular(const std::vector<Ponto>& pontos, const std::vector<InstElemento> idsNohsElementos,
		const std::vector<CondContorno>& pressPrescritas, const std::vector<CondContorno>& deslPrescritos)
	{
		const int nElementos = idsNohsElementos.size();

		cElementos.resize(nElementos);

		for (int i = 0; i < nElementos; i++)
		{
			const int id1 = idsNohsElementos[i].idPonto1 - 1;
			const int id2 = idsNohsElementos[i].idPonto2 - 1;
			const int id3 = idsNohsElementos[i].idPonto3 - 1;

			cElementos[i] = { Triangulo(pontos[id1], pontos[id2], pontos[id3]) };
		}

		cPressoesPrescritas = pressPrescritas;
		cDeslsPrescritos = deslPrescritos;

		/*for (size_t i = 0; i < cPressoesPrescritas.size(); i++)
			cPressoesPrescritas[i].idEle -= 1;

		for (size_t i = 0; i < cPressoesPrescritas.size(); i++)
			cPressoesPrescritas[i].idEle -= 1;*/

	}

	MalhaTriangular(const std::vector<Quad>& quads,
		const std::vector<CondContorno>& pressPrescritasQuads, const std::vector<CondContorno>& deslPrescritosQuads)
	{
		const size_t nElementos = quads.size() * 4; // 4 triangulos por quad
		
		cElementos.resize(nElementos);

		for (int i = 0; i < nElementos; i+=4)
		{
			const auto CM = (quads[i / 4].p1 + quads[i / 4].p2 + quads[i / 4].p3 + quads[i / 4].p4) / 4.0;
			const Ponto CMp = { CM.cX1, CM.cX2, CM.cX3 };

			cElementos[i] = { Triangulo(quads[i / 4].p1, quads[i / 4].p2,  CMp/*quads[i / 2].p3*/) };

			cElementos[i + 1UL] = { Triangulo(quads[i / 4].p2, quads[i / 4].p4, CMp/*quads[i / 2].p3*/) };

			cElementos[i + 2UL] = { Triangulo(quads[i / 4].p4, quads[i / 4].p3, CMp/*quads[i / 2].p3*/) };

			cElementos[i + 3UL] = { Triangulo(quads[i / 4].p3, quads[i / 4].p1, CMp/*quads[i / 2].p3*/) };
		}

		const size_t nPressPrescritas = pressPrescritasQuads.size() * 4;

		cPressoesPrescritas.resize(nPressPrescritas);
		
		for (size_t i = 0; i < nPressPrescritas; i+=4)
		{
			cPressoesPrescritas[i] = { (pressPrescritasQuads[i/4].idEle) * 4 - 3, pressPrescritasQuads[i / 4].grauLib,
				pressPrescritasQuads[i / 4].val };

			cPressoesPrescritas[i + 1] = { (pressPrescritasQuads[i / 4].idEle) * 4 - 2, pressPrescritasQuads[i / 4].grauLib,
				pressPrescritasQuads[i / 4].val };

			cPressoesPrescritas[i + 2] = { (pressPrescritasQuads[i / 4].idEle) * 4 - 1, pressPrescritasQuads[i / 4].grauLib,
				pressPrescritasQuads[i / 4].val };

			cPressoesPrescritas[i + 3] = { (pressPrescritasQuads[i / 4].idEle) * 4, pressPrescritasQuads[i / 4].grauLib,
				pressPrescritasQuads[i / 4].val };
		}

		const size_t nDeslPrescritos = deslPrescritosQuads.size() * 4;

		cDeslsPrescritos.resize(nDeslPrescritos);

		for (size_t i = 0; i < nDeslPrescritos; i+=4)
		{
			cDeslsPrescritos[i] = { (deslPrescritosQuads[i / 4].idEle) * 4 - 3, deslPrescritosQuads[i / 4].grauLib,
				deslPrescritosQuads[i / 4].val };

			cDeslsPrescritos[i + 1] = { (deslPrescritosQuads[i / 4].idEle) * 4 - 2, deslPrescritosQuads[i / 4].grauLib,
				deslPrescritosQuads[i / 4].val };

			cDeslsPrescritos[i + 2] = { (deslPrescritosQuads[i / 4].idEle) * 4 - 1, deslPrescritosQuads[i / 4].grauLib,
				deslPrescritosQuads[i / 4].val };

			cDeslsPrescritos[i + 3] = { (deslPrescritosQuads[i / 4].idEle) * 4, deslPrescritosQuads[i / 4].grauLib,
				deslPrescritosQuads[i / 4].val };

		}

		/*for (size_t i = 0; i < cPressoesPrescritas.size(); i++)
			cPressoesPrescritas[i].idEle -= 1;

		for (size_t i = 0; i < cPressoesPrescritas.size(); i++)
			cPressoesPrescritas[i].idEle -= 1;*/

	}
	
	~MalhaTriangular() {};

	std::vector<VertexType> GetMeshAsDataPoints() const
	{
		std::vector<VertexType> res;
		res.resize(cElementos.size() * 3);

		VertexType dp;

		for (size_t i = 0; i < cElementos.size(); i++)
		{
			dp.color = { (float)(cElementos[i].triangulo.cNormalU.cX1 + 1.0f) / 2.0f, 
				(float)(cElementos[i].triangulo.cNormalU.cX2 + 1.0f) / 2.0f,
				(float)(cElementos[i].triangulo.cNormalU.cX3 + 1.0f) / 2.0f, 1.0f };

			dp.position = { (float)cElementos[i].triangulo.cP1.cX1, (float)cElementos[i].triangulo.cP1.cX2,
			(float)cElementos[i].triangulo.cP1.cX3, 1.0f };

			res[3 * i + 0] = dp;

			dp.position = { (float)cElementos[i].triangulo.cP2.cX1, (float)cElementos[i].triangulo.cP2.cX2,
			(float)cElementos[i].triangulo.cP2.cX3, 1.0f };

			res[3 * i + 1] = dp;

			dp.position = { (float)cElementos[i].triangulo.cP3.cX1, (float)cElementos[i].triangulo.cP3.cX2,
			(float)cElementos[i].triangulo.cP3.cX3, 1.0f };

			res[3 * i + 2] = dp;
		}

		return res;
	}
	
	std::vector<VertexType> GetMeshAsDataPointsPainted(const std::vector<double>& resultado) const
	{
		float maxRes = -1e30;
		float minRes = +1e30;

		for (size_t i = 0; i < resultado.size(); i++)
		{
			maxRes = std::fmax(static_cast<float>(resultado[i]), maxRes);
			minRes = std::fmin(static_cast<float>(resultado[i]), minRes);
		}

		std::cout << "min "<< minRes << std::endl;
		std::cout << "max" << maxRes << std::endl;

		std::vector<VertexType> res;
		res.resize(cElementos.size() * 3);

		VertexType dp;

		for (size_t i = 0; i < cElementos.size(); i++)
		{
			dp.color = { (float(resultado[i]) - minRes) / (maxRes - minRes),
				0.0f,
				0.0f, 1.0f };

			std::cout << (float(resultado[i]) - minRes) / (maxRes - minRes) << std::endl;

			dp.position = { (float)cElementos[i].triangulo.cP1.cX1, (float)cElementos[i].triangulo.cP1.cX2,
			(float)cElementos[i].triangulo.cP1.cX3, 1.0f };

			res[3 * i + 0] = dp;

			dp.position = { (float)cElementos[i].triangulo.cP2.cX1, (float)cElementos[i].triangulo.cP2.cX2,
			(float)cElementos[i].triangulo.cP2.cX3, 1.0f };

			res[3 * i + 1] = dp;

			dp.position = { (float)cElementos[i].triangulo.cP3.cX1, (float)cElementos[i].triangulo.cP3.cX2,
			(float)cElementos[i].triangulo.cP3.cX3, 1.0f };

			res[3 * i + 2] = dp;
		}

		return res;
	}

	void Tesselate(int nivel)
	{
		if (nivel == 0)
		{
			return;
		}
		
		const int nne = cElementos.size();

		std::vector<ElementoTriangular> eleTesselated;
		eleTesselated.resize(4 * nne, {});

		int newEles = 0;
		int newNodes = 0;

		for (int ie = 0; ie < nne; ie++)
		{
			const Ponto X1 = cElementos[ie].triangulo.cP1;

			const Ponto X2 = cElementos[ie].triangulo.cP2;

			const Ponto X3 = cElementos[ie].triangulo.cP3;

			const Ponto X4 = ((Vetor(X1) * 1.0) + X2) / 2;
			const Ponto X5 = ((Vetor(X2) * 1.0) + X3) / 2;
			const Ponto X6 = ((Vetor(X3) * 1.0) + X1) / 2;

			//nodesTesselated[newNodes] = X1;
			//nodesTesselated[newNodes + 1] = X4;
			//nodesTesselated[newNodes + 2] = X6;

			eleTesselated[newEles].triangulo = Triangulo(X1, X4, X6);


			//eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
			newEles = newEles + 1;

			newNodes = newNodes + 3;

			//nodesTesselated[newNodes] = X4;
			//nodesTesselated[newNodes + 1] = X2;
			//nodesTesselated[newNodes + 2] = X5;

			//eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
			eleTesselated[newEles].triangulo = Triangulo(X4, X2, X5);
			newEles = newEles + 1;

			newNodes = newNodes + 3;

			//nodesTesselated[newNodes] = X4;
			//nodesTesselated[newNodes + 1] = X5;
			//nodesTesselated[newNodes + 2] = X6;
			//
			//eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
			eleTesselated[newEles].triangulo = Triangulo(X4, X5, X6);
			newEles = newEles + 1;

			newNodes = newNodes + 3;

			//nodesTesselated[newNodes] = X5;
			//nodesTesselated[newNodes + 1] = X3;
			//nodesTesselated[newNodes + 2] = X6;
			//
			//eleTesselated[newEles] = { newNodes + 1, newNodes + 2, newNodes + 3 };
			eleTesselated[newEles].triangulo = Triangulo(X5, X3, X6);
			newEles = newEles + 1;

			newNodes = newNodes + 3;
		}

		const int ndp = cDeslsPrescritos.size();
		const int nfp = cPressoesPrescritas.size();

		std::vector<CondContorno> ldptess;
		ldptess.resize(4 * ndp, {});
		std::vector<CondContorno> lfptess;
		lfptess.resize(4 * nfp, {});

		int indp = 0;

		for (int idp = 0; idp < ndp; idp++)
		{
			int newildp = (cDeslsPrescritos[idp].idEle * 4) - 3;

			ldptess[indp] = { newildp, cDeslsPrescritos[idp].grauLib, cDeslsPrescritos[idp].val };
			ldptess[indp + 1] = { newildp + 1, cDeslsPrescritos[idp].grauLib, cDeslsPrescritos[idp].val };
			ldptess[indp + 2] = { newildp + 2, cDeslsPrescritos[idp].grauLib, cDeslsPrescritos[idp].val };
			ldptess[indp + 3] = { newildp + 3, cDeslsPrescritos[idp].grauLib, cDeslsPrescritos[idp].val };

			indp = indp + 4;
		}


		int infp = 0;

		for (int ifp = 0; ifp < nfp; ifp++)
		{
			int newilfp = (cPressoesPrescritas[ifp].idEle * 4) - 3;

			lfptess[infp] = { newilfp, cPressoesPrescritas[ifp].grauLib, cPressoesPrescritas[ifp].val };
			lfptess[infp + 1] = { newilfp + 1, cPressoesPrescritas[ifp].grauLib, cPressoesPrescritas[ifp].val };
			lfptess[infp + 2] = { newilfp + 2, cPressoesPrescritas[ifp].grauLib, cPressoesPrescritas[ifp].val };
			lfptess[infp + 3] = { newilfp + 3, cPressoesPrescritas[ifp].grauLib, cPressoesPrescritas[ifp].val };

			infp = infp + 4;
		}

		cPressoesPrescritas = std::move(lfptess);
		cDeslsPrescritos = std::move(ldptess);
		cElementos = std::move(eleTesselated);

		return Tesselate(nivel - 1);
	}

	Vetor GetCMElement(size_t id) const
	{
		if (id < cElementos.size())
		{
			return cElementos[id].triangulo.cCM;
		}

		return { NAN, NAN, NAN };
	}
	
public:
	std::vector<ElementoTriangular> cElementos;
	std::vector<CondContorno> cPressoesPrescritas;
	std::vector<CondContorno> cDeslsPrescritos;

};