#pragma once
#include <math.h>
#include "Matriz.h"
#include <iostream>

// retornar resultado;
// na função GetMeshPoints adicionar questão da cor 
// dependendo dos deslocamentos/carregamentos
struct Resultado
{
	std::vector<double> compX1;
	std::vector<double> compX2;
	std::vector<double> compX3;
};

// Declara��es das fun��es a serem definidas em "MathUtilsM.cpp"

// Fun��o que retorna Matriz com valores aleat�rios (0,0 - 1,0)
Matriz CriaMatrizAleatoria(size_t numlinhas, size_t numColunas);

// Fun��o que realiza substitui��es sucessivas (resolu��o de sistemas triangulares inferior)
Matriz SubstSucessivas(const Matriz& L, const Matriz& b);

// Fun��o que realiza substitui��es sucessivas (resolu��o de sistemas triangulares superior)
Matriz SubstRetroativas(const Matriz& U, const Matriz& b);

// Fun��o que realiza a decomposi��o LU da Matriz (com pivota��o)
void DecompPALU(const Matriz& A, Matriz& POut, Matriz& LOut, Matriz& UOut);

// Fun��o auxiliar que escreve Matriz na tela
void MostrarMatriz(const Matriz& M);

// guarda matriz em arquivo
void LogMatrixToFile(const std::string& filename, const Matriz& M);

// Fun��o que resolve sistemas lineares atrav�s da decomposi��o LU
Matriz ResSistLinearPALU(const Matriz& A, const Matriz& b);

Matriz CalcInvMatriz(const Matriz& A);

// Fun��o auxiliar que troca linhas em uma Matriz
void TrocaLinha(Matriz& m, size_t l1, size_t l2);

// Fun��o que retorna uma Matriz identidade nxn
Matriz MatrizI(size_t n);
// Fun��o que retorna uma Matriz nula nxm
Matriz MatrizZeros(size_t n, size_t m);

// Fun��o que retorna o indice do pivo da linha
size_t AchaIndicePivo(const Matriz& m, size_t nColuna, size_t nLinInicial);