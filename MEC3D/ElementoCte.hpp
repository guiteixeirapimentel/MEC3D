#pragma once
#include <iostream>
#include "Triangulo.h"
#include "Matriz.h"

#include "MalhaTriangular.h"

#include "HammerConstantTriangleIntegration.h"
#include "SingularIntegrationConstantTriangle.h"

#include "UStarFunNoSing.h"
#include "PStarFunNoSing.h"

#include "MathUtils.h"

#include "Timer.h"


Resultado ElementoCte(const MalhaTriangular& malha, const double E, const double v)
{
    const double G = E / (2.0 * (1.0 + v));

    Timer timer;

    const int idtTotal = timer.SetTimer("Tempo total programa");
    const int idtMatrizHG = timer.SetTimer("Criacao das Matrizes H e G");
    const int idtRearrSist = timer.SetTimer("Rearranjo de matrizes");
    const int idtSolSistLin = timer.SetTimer("Solucao sistema linear");

    timer.Tick(idtTotal);


    //constexpr double kk = 0.1;
    //std::vector<Ponto> pontos = {
    //    {0.0000*kk, 0.0000*kk, 4.0000},
    //    {2.0000*kk, 0.0000*kk, 4.0000},
    //    {0.0000*kk, 2.0000*kk, 4.0000},
    //    {0.0000*kk, 2.0000*kk, 4.0000},
    //    {2.0000*kk, 0.0000*kk, 4.0000},
    //    {2.0000*kk, 2.0000*kk, 4.0000},
    //    {0.0000*kk, 0.0000*kk, 0.0000},
    //    {0.0000*kk, 2.0000*kk, 0.0000},
    //    {2.0000*kk, 0.0000*kk, 0.0000},
    //    {0.0000*kk, 2.0000*kk, 0.0000},
    //    {2.0000*kk, 2.0000*kk, 0.0000},
    //    {2.0000*kk, 0.0000*kk, 0.0000},
    //    {2.0000*kk, 0.0000*kk, 4.0000},
    //    {2.0000*kk, 0.0000*kk, 0.0000},
    //    {2.0000*kk, 2.0000*kk, 0.0000},
    //    {2.0000*kk, 0.0000*kk, 4.0000},
    //    {2.0000*kk, 2.0000*kk, 0.0000},
    //    {2.0000*kk, 2.0000*kk, 4.0000},
    //    {0.0000*kk, 2.0000*kk, 4.0000},
    //    {0.0000*kk, 2.0000*kk, 0.0000},
    //    {0.0000*kk, 0.0000*kk, 0.0000},
    //    {0.0000*kk, 2.0000*kk, 4.0000},
    //    {0.0000*kk, 0.0000*kk, 0.0000},
    //    {0.0000*kk, 0.0000*kk, 4.0000},
    //    {2.0000*kk, 2.0000*kk, 4.0000},
    //    {2.0000*kk, 2.0000*kk, 0.0000},
    //    {0.0000*kk, 2.0000*kk, 0.0000},
    //    {2.0000*kk, 2.0000*kk, 4.0000},
    //    {0.0000*kk, 2.0000*kk, 0.0000},
    //    {0.0000*kk, 2.0000*kk, 4.0000},
    //    {0.0000*kk, 0.0000*kk, 0.0000},
    //    {2.0000*kk, 0.0000*kk, 0.0000},
    //    {0.0000*kk, 0.0000*kk, 4.0000},
    //    {0.0000*kk, 0.0000*kk, 4.0000},
    //    {2.0000*kk, 0.0000*kk, 0.0000},
    //    {2.0000*kk, 0.0000*kk, 4.0000}
    //};
    //std::vector<InstElemento> elementos =
    //{
    // {1,  2,  3},
    // {4,  5,  6},
    // {7,  8,  9},
    // {10, 11, 12},
    // {13, 14, 15},
    // {16, 17, 18},
    // {19, 20, 21},
    // {22, 23, 24},
    // {25, 26, 27},
    // {28, 29, 30},
    // {31, 32, 33},
    // {34, 35, 36}
    //};
    //std::vector<CondContorno> deslPrescs =
    //{
    //    { 3,  3,  0 },
    //    { 4,  3,  0 },
    //    { 3,  2,  0 },
    //    { 4,  2,  0 },
    //    { 3,  1,  0 },
    //    { 4,  1,  0 }
    //};
    //constexpr double P = 4.0e-6 / (kk * kk * 2.0 * 2.0);
    //constexpr double F = P * (kk * 2.0 * kk * 2.0);
    //std::vector<CondContorno> forcsPrescs =
    //{
    //    { 1,  2,  P },
    //    { 2,  2,  P }/*,
    //    { 3,  3,  -P },
    //    { 4,  3,  -P }*/
    //};

//    constexpr double k = 1.0;
//    constexpr double k2 = 1.0;
//    std::vector<Ponto> pontos = 
//    {
//
//    {0.0000*k, 2.0000*k, 4.0000*k2},
//    {0.0000*k, 0.0000*k, 4.0000*k2},
//    {1.0000*k, 1.0000*k, 4.0000*k2},
//    {2.0000*k, 2.0000*k, 4.0000*k2},
//    {0.0000*k, 2.0000*k, 4.0000*k2},
//    {1.0000*k, 1.0000*k, 4.0000*k2},
//    {2.0000*k, 0.0000*k, 4.0000*k2},
//    {2.0000*k, 2.0000*k, 4.0000*k2},
//    {1.0000*k, 1.0000*k, 4.0000*k2},
//    {0.0000*k, 0.0000*k, 4.0000*k2},
//    {2.0000*k, 0.0000*k, 4.0000*k2},
//    {1.0000*k, 1.0000*k, 4.0000*k2},
//    {2.0000*k, 2.0000*k, 0.0000*k2},
//    {2.0000*k, 0.0000*k, 0.0000*k2},
//    {1.0000*k, 1.0000*k, 0.0000*k2},
//    {0.0000*k, 2.0000*k, 0.0000*k2},
//    {2.0000*k, 2.0000*k, 0.0000*k2},
//    {1.0000*k, 1.0000*k, 0.0000*k2},
//    {0.0000*k, 0.0000*k, 0.0000*k2},
//    {0.0000*k, 2.0000*k, 0.0000*k2},
//    {1.0000*k, 1.0000*k, 0.0000*k2},
//    {0.0000*k, 0.0000*k, 0.0000*k2},
//    {1.0000*k, 1.0000*k, 0.0000*k2},
//    {2.0000*k, 0.0000*k, 0.0000*k2},
//    {2*k, 2*k, 2*k2},
//    {2*k, 1*k, 1*k2},
//    {2*k, 2*k, 0*k2},
//    {2*k, 0*k, 2*k2},
//    {2*k, 1*k, 1*k2},
//    {2*k, 2*k, 2*k2},
//    {2*k, 0*k, 2*k2},
//    {2*k, 0*k, 0*k2},
//    {2*k, 1*k, 1*k2},
//    {2*k, 1*k, 1*k2},
//    {2*k, 0*k, 0*k2},
//    {2*k, 2*k, 0*k2},
//    {2*k, 2*k, 4*k2},
//    {2*k, 1*k, 3*k2},
//    {2*k, 2*k, 2*k2},
//    {2*k, 0*k, 4*k2},
//    {2*k, 1*k, 3*k2},
//    {2*k, 2*k, 4*k2},
//    {2*k, 0*k, 4*k2},
//    {2*k, 0*k, 2*k2},
//    {2*k, 1*k, 3*k2},
//    {2*k, 1*k, 3*k2},
//    {2*k, 0*k, 2*k2},
//    {2*k, 2*k, 2*k2},
//    {0*k, 0*k, 2*k2},
//    {0*k, 1*k, 1*k2},
//    {0*k, 0*k, 0*k2},
//    {0*k, 1*k, 1*k2},
//    {0*k, 0*k, 2*k2},
//    {0*k, 2*k, 2*k2},
//    {0*k, 2*k, 2*k2},
//    {0*k, 2*k, 0*k2},
//    {0*k, 1*k, 1*k2},
//    {0*k, 1*k, 1*k2},
//    {0*k, 2*k, 0*k2},
//    {0*k, 0*k, 0*k2},
//    {0*k, 0*k, 4*k2},
//    {0*k, 1*k, 3*k2},
//    {0*k, 0*k, 2*k2},
//    {0*k, 1*k, 3*k2},
//    {0*k, 0*k, 4*k2},
//    {0*k, 2*k, 4*k2},
//    {0*k, 2*k, 4*k2},
//    {0*k, 2*k, 2*k2},
//    {0*k, 1*k, 3*k2},
//    {0*k, 1*k, 3*k2},
//    {0*k, 2*k, 2*k2},
//    {0*k, 0*k, 2*k2},
//    {0*k, 2*k, 0*k2},
//    {0*k, 2*k, 2*k2},
//    {1*k, 2*k, 1*k2},
//    {1*k, 2*k, 1*k2},
//    {2*k, 2*k, 0*k2},
//    {0*k, 2*k, 0*k2},
//    {2*k, 2*k, 2*k2},
//    {2*k, 2*k, 0*k2},
//    {1*k, 2*k, 1*k2},
//    {1*k, 2*k, 1*k2},
//    {0*k, 2*k, 2*k2},
//    {2*k, 2*k, 2*k2},
//    {0*k, 2*k, 4*k2},
//    {1*k, 2*k, 3*k2},
//    {0*k, 2*k, 2*k2},
//    {2*k, 2*k, 4*k2},
//    {1*k, 2*k, 3*k2},
//    {0*k, 2*k, 4*k2},
//    {2*k, 2*k, 4*k2},
//    {2*k, 2*k, 2*k2},
//    {1*k, 2*k, 3*k2},
//    {1*k, 2*k, 3*k2},
//    {2*k, 2*k, 2*k2},
//    {0*k, 2*k, 2*k2},
//    {2*k, 0*k, 2*k2},
//    {1*k, 0*k, 1*k2},
//    {2*k, 0*k, 0*k2},
//    {0*k, 0*k, 2*k2},
//    {1*k, 0*k, 1*k2},
//    {2*k, 0*k, 2*k2},
//    {0*k, 0*k, 2*k2},
//    {0*k, 0*k, 0*k2},
//    {1*k, 0*k, 1*k2},
//    {1*k, 0*k, 1*k2},
//    {0*k, 0*k, 0*k2},
//    {2*k, 0*k, 0*k2},
//    {2*k, 0*k, 4*k2},
//    {1*k, 0*k, 3*k2},
//    {2*k, 0*k, 2*k2},
//    {0*k, 0*k, 4*k2},
//    {1*k, 0*k, 3*k2},
//    {2*k, 0*k, 4*k2},
//    {0*k, 0*k, 4*k2},
//    {0*k, 0*k, 2*k2},
//    {1*k, 0*k, 3*k2},
//    {1*k, 0*k, 3*k2},
//    {0*k, 0*k, 2*k2},
//    {2*k, 0*k, 2*k2 } 
//};
//    std::vector<InstElemento> elementos = 
//    {
//    {1, 2, 3},
//    {4, 5, 6},
//    {7, 8, 9},
//    {10, 11, 12},
//    {13, 14, 15},
//    {16, 17, 18},
//    {19, 20, 21},
//    {22, 23, 24},
//    {25, 26, 27},
//    { 28, 29, 30},
//    { 31, 32, 33},
//    { 34, 35, 36},
//    { 37, 38, 39},
//    { 40, 41, 42},
//    { 43, 44, 45},
//    { 46, 47, 48},
//    { 49, 50, 51},
//    { 52, 53, 54},
//    { 55, 56, 57},
//    { 58, 59, 60},
//    { 61, 62, 63},
//    { 64, 65, 66},
//    { 67, 68, 69},
//    { 70, 71, 72},
//    { 73, 74, 75},
//    { 76, 77, 78},
//    { 79, 80, 81},
//    { 82, 83, 84},
//    { 85, 86, 87},
//    { 88, 89, 90},
//    { 91, 92, 93},
//    { 94, 95, 96},
//    { 97, 98, 99},
//    { 100, 101, 102},
//    { 103, 104, 105},
//    { 106, 107, 108},
//    { 109, 110, 111},
//    { 112, 113, 114},
//    { 115, 116, 117},
//    { 118, 119, 120} };
//    std::vector<CondContorno> deslPrescs =
//    {
//		{ 5,  3,  0 },
//		{ 6,  3,  0 },
//		{ 7,  3,  0 },
//		{ 8,  3,  0 },
//        { 5,  2,  0 },
//        { 6,  2,  0 },
//        { 7,  2,  0 },
//        { 8,  2,  0 },
//        { 5,  1,  0 },
//        { 6,  1,  0 },
//        { 7,  1,  0 },
//        { 8,  1,  0 }
//    };
//    std::vector<CondContorno> forcsPrescs =
//    {
//        { 1,  2,   1.0 },
//        { 2,  2,   1.0 },
//        { 3,  2,   1.0 },
//        { 4,  2,   1.0 }/*,
//        { 5,  3,  -1.0 },
//        { 6,  3,  -1.0 },
//        { 7,  3,  -1.0 },
//        { 8,  3,  -1.0 }*/
//    };
    

    timer.Tick(idtMatrizHG);

    FundSolPStar solFundPStar(v);
    FundSolUStar solFundUStar(v, G);

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
                            HPstar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsPStarSubElement(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundPStar, I, J, 4, 1e-6);

                            GUStar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsUStarSubElement(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundUStar, I, J, 4, 1e-6);
                        }
                        else
                        {
                            /*HPstar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsPStar(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundPStar, I, J);

                            GUStar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsUStar(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundUStar, I, J);*/

                            HPstar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsPStarSubElement(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundPStar, I, J, 0, 1e-6);

                            GUStar(ii + I - 1, jj + J - 1) = hammerIntegration.Integrate7PtsUStarSubElement(
                                malha.cElementos[j].triangulo,
                                malha.cElementos[i].triangulo.cCM,
                                solFundUStar, I, J, 0, 1e-6);
                        }

                    }
            }
            else
            {
                for (int I = 1; I <= 3; I++)
                    for (int J = 1; J <= 3; J++)
                    {
                        HPstar(ii + I - 1, jj + J - 1) = singIntegration.IntegratePStar(
                            malha.cElementos[j].triangulo,
                            malha.cElementos[i].triangulo.cCM,
                            solFundPNoSingPStar, I, J, 1e-3);

                        GUStar(ii + I - 1, jj + J - 1) = singIntegration.IntegrateUStar(
                            malha.cElementos[j].triangulo,
                            malha.cElementos[i].triangulo.cCM,
                            solFundUNoSingUStar, I, J, 1e-3);

                        /*HPstar(ii + I - 1, jj + J - 1) = 0.0;
                        GUStar(ii + I - 1, jj + J - 1) = 0.0;*/
                    }
            }

        }
    }

    //MostrarMatriz(HPstar);

    for (int i = 0; i < 3 * nelementos; i++)
    {
        HPstar(i, i) += 0.5;
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


        maxDelta = std::fmax(maxDelta, std::fabs(HPstar(i, i) - (-sum)));

        const double delta = std::fabs(HPstar(i, i) - (-sum));

		//HPstar(i, i) = -sum;

        //HPstar(i, i) = (-sum + HPstar(i, i)) / 2.0;

        //std::cout << -sum << " - " << HPstar(i, i) << " - delta " << delta << std::endl;
    }

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


    // MostrarMatriz(HPstar, "HPStar");

    /*MostrarMatriz(GUStar, "GUStar");*/

    Matriz b = B * y;

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
        std::cout << "i: " << i << " " << X((i + 1) * 3 - 3, 0) << std::endl;
		res.compX1[i] = X((i + 1) * 3 - 3, 0);
    }

    std::cout << "### GRAU 2 ### \n";
    for (size_t i = 0; i < nelementos; i++)
    {
        std::cout << "i: " << i <<" " << X((i + 1) * 3 - 2, 0) << std::endl;
		res.compX2[i] = X((i + 1) * 3 - 2, 0);
    }

    std::cout << "### GRAU 3 ### \n";
    for (size_t i = 0; i < nelementos; i++)
    {
        std::cout << "i: " << i << " " << X((i + 1) * 3 - 1, 0) << std::endl;
		res.compX3[i] = X((i + 1) * 3 - 1, 0);
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