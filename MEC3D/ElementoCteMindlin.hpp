#pragma once
#include <iostream>
#include "Triangulo.h"
#include "Matriz.h"

#include "MalhaTriangular.h"

#include "HammerConstantTriangleIntegration.h"
#include "SingularIntegrationConstantTriangle.h"

#include "FundSolUMindlin.h"
#include "FundSolPMindlin.h"

#include "MathUtils.h"

#include "Timer.h"


Resultado ElementoCteMindlin(const MalhaTriangular& malha, const double E, const double v, double C)
{
    const double G = E / (2.0 * (1.0 + v));

    Timer timer;

    const int idtTotal = timer.SetTimer("Tempo total programa");
    const int idtMatrizHG = timer.SetTimer("Criacao das Matrizes H e G");
    const int idtRearrSist = timer.SetTimer("Rearranjo de matrizes");
    const int idtSolSistLin = timer.SetTimer("Solucao sistema linear");

    timer.Tick(idtTotal);	

    timer.Tick(idtMatrizHG);

    FundSolPMindlin solFundPMindlin(v);
    FundSolUMindlin solFundUMindlin(v, G);

    FundSolPComp solFundPComp(v);
    FundSolUComp solFundUComp(v, G);

    PStarFunNoSing solFundPNoSingPStar(v);
    UStarFunNoSing solFundUNoSingUStar(v, G);

    HammerConstantTriangleIntegration hammerIntegration;
    SingularIntegrationConstantTriangle singIntegration(hammerIntegration);

    // elementos constantes...

    Matriz HPstar(3 * malha.cElementos.size(), 3 * malha.cElementos.size());
    Matriz GUStar(3 * malha.cElementos.size(), 3 * malha.cElementos.size());

    const int nelementos = malha.cElementos.size();

#ifndef _DEBUG
#pragma omp parallel for
#endif
    for (int i = 0; i < nelementos; i++)
    {
        for (int j = 0; j < nelementos; j++)
        {
            // pulando três de uma vez para trabalhar com as submatrizes.
            const int ii = i * 3;
            const int jj = j * 3;

            if (i != j)
            {
                for (int I = 1; I <= 3; I++)
                    for (int J = 1; J <= 3; J++)
                    {
                        const double raio = (malha.cElementos[j].triangulo.cCM - malha.cElementos[i].triangulo.cCM).Mod();

                        if (raio / malha.cElementos[j].triangulo.cLadoMedio < 4.0)
                        {                            
                            HPstar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsPStarSubElementMindlin(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundPMindlin, I, J, C, 4, 1e-8);

                            GUStar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsUStarSubElementMindlin(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundUMindlin, I, J, C, 4, 1e-8);
                        }
                        else
                        {
                            HPstar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsPStarSubElementMindlin(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundPMindlin, I, J, C, 0, 1e-8);

                            GUStar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsUStarSubElementMindlin(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundUMindlin, I, J, C, 0, 1e-8);
                        }

                    }
            }
            else
            {
                for (int I = 1; I <= 3; I++)
                    for (int J = 1; J <= 3; J++)
                    {
                        HPstar(ii + I - 1, jj + J - 1) = singIntegration.IntegratePMindlin(
                            malha.cElementos[j].triangulo,
                            malha.cElementos[i].triangulo.cCM,
                            solFundPNoSingPStar, solFundPComp, I, J, C, 1e-8);

                        GUStar(ii + I - 1, jj + J - 1) = singIntegration.IntegrateUMindlin(
                            malha.cElementos[j].triangulo,
                            malha.cElementos[i].triangulo.cCM,
                            solFundUNoSingUStar, solFundUComp, I, J, C, 1e-8);
                    }
            }
            
        }
    }
    
    for (int i = 0; i < 3 * nelementos; i++)
    {
        HPstar(i, i) += 1.0;
    }
	
    double maxDelta = 0.0;

    for (int i = 0; i < 3 * nelementos; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < 3 * nelementos; j++)
        {
			if (j != i)
				sum += HPstar(i, j);
        }

		const double delta = std::fabs(HPstar(i, i) - (1.0 - sum));

        maxDelta = std::fmax(maxDelta, delta);

		//std::cout << (1.0 - sum) << std::endl;

        //HPstar(i, i) = 1.0 - sum;
        //HPstar(i, i) = (-sum + HPstar(i, i)) / 2.0;

        //std::cout << -sum << " - " << HPstar(i, i) << " - delta " << delta << std::endl;
    }
		

    std::cout << "HPstar ";
    //MostrarMatriz(HPstar);

    std::cout << "GUstar ";
    //MostrarMatriz(GUStar);

    std::cout << maxDelta << std::endl;

    timer.Tock(idtMatrizHG);


    // consideração de: se não há condição de contorno, a força é zero.

    // A e B com colunas trocadas, para satisfazer B.C.
    // HU = GP -> Ax = By -> Ax = b

    timer.Tick(idtRearrSist);

    Matriz A = HPstar;
    Matriz B = GUStar;

    Matriz y(3 * nelementos, 1);

    for (size_t n = 0; n < malha.cPressoesPrescritas.size(); n++)
    {
        y((3 * (malha.cPressoesPrescritas[n].idEle - 1)) + (malha.cPressoesPrescritas[n].grauLib - 1), 0) =
            malha.cPressoesPrescritas[n].val;
    }

    for (size_t n = 0; n < malha.cDeslsPrescritos.size(); n++)
    {
        y((3 * (malha.cDeslsPrescritos[n].idEle - 1)) + (malha.cDeslsPrescritos[n].grauLib - 1), 0) =
            malha.cDeslsPrescritos[n].val;

        const int col = (3 * (malha.cDeslsPrescritos[n].idEle - 1)) + (malha.cDeslsPrescritos[n].grauLib - 1);

        A.SwapColNegative(B, col);
    }

    std::cout << "y ";
    //MostrarMatriz(y);

    int x = 0;

    //MostrarMatriz(HPstar);

    //MostrarMatriz(GUStar);

    Matriz b = B * y;

    std::cout << "b ";
    //MostrarMatriz(b);

    std::cout << "B ";
    //MostrarMatriz(B);


    timer.Tock(idtRearrSist);

    timer.Tick(idtSolSistLin);

    Matriz X = ResSistLinearPALU(A, b);

	Resultado res;

	res.compX1.resize(nelementos, 0.0);
	res.compX2 = res.compX1;
	res.compX3 = res.compX1;


    std::cout << "### GRAU 1 ### \n";
    for (size_t i = 0; i < nelementos; i++)
    {
		res.compX1[i] = X((i + 1) * 3 - 3, 0);
        std::cout << "i: " << i << " " << X((i + 1) * 3 - 3, 0) << std::endl;
    }

    std::cout << "### GRAU 2 ### \n";
    for (size_t i = 0; i < nelementos; i++)
    {
		res.compX2[i] = X((i + 1) * 3 - 2, 0);
        std::cout << "i: " << i << " " << X((i + 1) * 3 - 2, 0) << std::endl;
    }

    std::cout << "### GRAU 3 ### \n";
    for (size_t i = 0; i < nelementos; i++)
    {
		res.compX3[i] = X((i + 1) * 3 - 1, 0);
        std::cout << "i: " << i << " " << X((i + 1) * 3 - 1, 0) << std::endl;
    }

    for (size_t i = 0; i < nelementos; i++)
    {
        const auto posCM = malha.GetCMElement(i);
        std::cout << posCM.cX1 << " " << posCM.cX2 << " " << posCM.cX3 << std::endl;
    }

    timer.Tock(idtSolSistLin);

    LogMatrixToFile("Resultado.txt", X);


    //LogMatrixToFile("H.txt", HPstar);
    //LogMatrixToFile("G.txt", GUStar);

    timer.Tock(idtTotal);

    timer.WriteToCoutAllTickTock();

    //system("PAUSE");
    
	return res;
}