#include "MathUtils.h"
#include <assert.h>
#include <fstream>

// Defini��es das fun��es definidas em "MathUtilsM.h"

Matriz CriaMatrizAleatoria(size_t numlinhas, size_t numColunas)
{
	std::vector<double> buf;
	buf.resize(numlinhas * numColunas);

	for (size_t i = 0; i < buf.size(); i++)
	{
		buf[i] = ((double(rand()) / RAND_MAX) - 0.5);
	}
	Matriz m(numlinhas, numColunas, std::move(buf));

	return m;
}

Matriz SubstSucessivas(const Matriz& L, const Matriz& b)
{
	std::vector<double> res;
	res.resize(b.cVals.size());
	res[0] = b.cVals[0] / L.cVals[0 + (0 * L.cNColunas)];

	for (size_t index = 1; index < res.size(); index++)
	{
		double soma = 0.0;
		for (size_t j = 0; j < index; j++)
		{
			soma += L.cVals[j + (index * L.cNColunas)] * res[j];
		}
		res[index] = (b.cVals[index] - soma) / L.cVals[index + (index * L.cNColunas)];
	}

	Matriz m(b.cNLinhas, b.cNColunas, std::move(res));

	return m;
}

Matriz SubstRetroativas(const Matriz& U, const Matriz& b)
{
	std::vector<double> res;
	res.resize(b.cVals.size());

	res[b.cVals.size() - 1] = b.cVals.back() / U.cVals[(U.cNColunas - 1) + ((U.cNLinhas - 1) * U.cNColunas)];

	for (size_t index = res.size() - 2; index > 0; index--)
	{
		double soma = 0.0;
		for (size_t j = U.cNColunas - 1; j > index; j--)
		{
			soma += U.cVals[j + (index * U.cNColunas)] * res[j];
		}
		res[index] = (b.cVals[index] - soma) / U.cVals[index + (index * U.cNColunas)];
	}

	double soma = 0.0;
	for (size_t j = U.cNColunas - 1; j > 0; j--)
	{
		soma += U.cVals[j + (0 * U.cNColunas)] * res[j];
	}
	res[0] = (b.cVals[0] - soma) / U.cVals[0 + (0 * U.cNColunas)];

	Matriz m(b.cNLinhas, b.cNColunas, std::move(res));

	return m;
}

void MostrarMatriz(const Matriz& M)
{
	std::cout << "= {\n";
	for (size_t i = 0; i < M.cNLinhas; i++)
	{
		std::cout << "[";

		for (size_t j = 0; j < M.cNColunas; j++)
		{
			std::cout << M.cVals[j + (i * M.cNColunas)] << ",\t ";
		}

		std::cout << "]," << std::endl;
	}

	std::cout << "};" << std::endl;
}

void LogMatrixToFile(const std::string& filename, const Matriz& M)
{
	std::ofstream arq(filename);

	arq << filename.c_str() << "= {\n";
	for (size_t i = 0; i < M.cNLinhas; i++)
	{
		arq << "\t[";

		for (size_t j = 0; j < M.cNColunas; j++)
		{
			arq << M.cVals[j + (i * M.cNColunas)] << " ,\t";
		}

		arq << "]," << std::endl;
	}

	arq << "};" << std::endl;
}

void DecompPALU(const Matriz& A, Matriz& POut, Matriz& LOut, Matriz& UOut)
{
	/*
	L = {[1 0 0 .. 0],
		 [M 1 0 .. 0],
		 [M M 1 .. 0],
		 [M M M .. 1]};

	U = {[ Linha Piv� 1],
		 [ Linha Piv� 2],
		 [ Linha Piv� n],};

	P = {[ 1 na coluna = linha piv� 1],
		 [ 1 na coluna = linha piv� 2],
		 [ 1 na coluna = linha piv� n]};
	*/

	const size_t m = A.cNLinhas;
	const size_t n = A.cNColunas;

	Matriz ML = MatrizZeros(n, n);
	Matriz MP = MatrizI(A.cNColunas);
	Matriz MU = A;

	std::vector<double>& L = ML.cVals;
	std::vector<double>& U = MU.cVals;
	std::vector<double>& P = MP.cVals;


	for (size_t k = 0; k < n; k++)
	{
		const size_t ipiv = AchaIndicePivo(MU, k, k);
		TrocaLinha(MU, k, ipiv);
		TrocaLinha(MP, k, ipiv);
		TrocaLinha(ML, k, ipiv);

		for (size_t j = k + 1; j < n; j++)
		{
			L[k + (j * ML.cNColunas)] = U[k + (j * MU.cNColunas)] / U[k + (k * MU.cNColunas)];
			for (size_t index = k; index < n; index++)
			{
				U[index + (j * MU.cNColunas)] -= L[k + (j * MU.cNColunas)] * U[index + (k * MU.cNColunas)];
			}
		}

	}

	ML += MatrizI(n);

	POut = std::move(MP);
	LOut = std::move(ML);
	UOut = std::move(MU);
}

void TrocaLinha(Matriz& matriz, size_t l1, size_t l2)
{
	for (size_t j = 0; j < matriz.cNColunas; j++)
	{
		double tmp = matriz.cVals[j + (l2*matriz.cNColunas)];
		matriz.cVals[j + (l2*matriz.cNColunas)] = matriz.cVals[j + (l1*matriz.cNColunas)];
		matriz.cVals[j + (l1*matriz.cNColunas)] = tmp;
	}
}

Matriz MatrizI(size_t n)
{
	std::vector<double> res;
	res.resize(n * n);

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			if (i != j)
			{
				res[j + (i * n)] = 0.0;
			}
			else
			{
				res[j + (i * n)] = 1.0;
			}
		}
	}

	Matriz mres(n, n, std::move(res));

	return mres;
}

size_t AchaIndicePivo(const Matriz& m, size_t nColuna, size_t nLinInicial)
{
	size_t res = 0;
	double maior = 0.0;
	for (size_t i = nLinInicial; i < m.cNLinhas; ++i)
	{
		if (nLinInicial == m.cNColunas)
		{
			int x = 0;
		}
		const double val = fabs(m.cVals[nColuna + (i * m.cNColunas)]);

		if (val > maior)
		{
			res = i;
			maior = fabs(val);
		}
	}

	return res;
}

Matriz MatrizZeros(size_t n, size_t m)
{
	std::vector<double> res;
	res.resize(n * m);

	for (size_t i = 0; i < res.size(); i++)
	{
		res[i] = 0.0;
	}

	return Matriz(n, m, std::move(res));
}

Matriz ResSistLinearPALU(const Matriz& A, const Matriz& b)
{
	Matriz P = MatrizZeros(1, 1);
	Matriz U = MatrizZeros(1, 1);
	Matriz L = MatrizZeros(1, 1);

	DecompPALU(A, P, L, U);

	Matriz Pb = P * b;

	Matriz K = SubstSucessivas(L, Pb);

	// K = UX

	Matriz x = SubstRetroativas(U, K);

	return x;
}

Matriz CalcInvMatriz(const Matriz& A)
{
	// 49 -> frederico
	Matriz P = MatrizZeros(1, 1);
	Matriz L = MatrizZeros(1, 1);
	Matriz U = MatrizZeros(1, 1);

	DecompPALU(A, P, L, U);

	std::vector<double> res;
	res.resize(A.cNLinhas*A.cNColunas);

	for(size_t j = 0; j < A.cNColunas; j++)
	{
		Matriz v = MatrizZeros(A.cNLinhas, 1);

		(*v.GetPtrMatriz())[j] = 1.0;

		v = P*v;
		Matriz K = SubstSucessivas(L, v);

		// K = UX

		Matriz x = SubstRetroativas(U, K);

		for(size_t i = 0; i < A.cNLinhas; i++)
		{
			res[j + (i * A.cNColunas)] = (*x.GetPtrMatriz())[i];
		}
	}

	return Matriz(A.cNLinhas, A.cNColunas, res);
}