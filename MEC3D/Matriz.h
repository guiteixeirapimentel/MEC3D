#pragma once
#include <vector>
#include <cassert>
#include "Vetor.h"

class Matriz
{
public:
	Matriz(int ni, int nj, const std::vector<double>& vals) : cNLinhas(ni), cNColunas(nj), cVals(vals) 
	{ assert(cVals.size() == cNLinhas * cNColunas); };
	Matriz(int ni, int nj, std::vector<double>&& vals) : cNLinhas(ni), cNColunas(nj), cVals(std::move(vals)) 
	{ assert(cVals.size() == cNLinhas * cNColunas); };
	Matriz(int ni, int nj) : cNLinhas(ni), cNColunas(nj)
	{
		cVals.resize(cNLinhas * cNColunas, 0.0);
	}
	Matriz() : cNLinhas(0), cNColunas(0), cVals({}) {};
	
	Matriz operator+(const Matriz& rhs) const
	{
		assert(rhs.cNColunas == cNColunas && rhs.cNLinhas == cNLinhas);

		const int indexMax = cNLinhas * cNColunas;

		std::vector<double> nvals;
		nvals.resize(indexMax);

		for (int index = 0; index < indexMax; index++)
		{
			nvals[index] = rhs[index] + cVals[index];
		}

		return Matriz(cNLinhas, cNColunas, std::move(nvals));
	}

	Matriz& operator+=(const Matriz& rhs)
	{
		const int n = cVals.size();

		for (int index = 0; index < n; index++)
		{
			cVals[index] += rhs[index];
		}

		return *this;
	}

	Matriz& operator-=(const Matriz& rhs)
	{
		const int n = cVals.size();

		for (int index = 0; index < n; index++)
		{
			cVals[index] -= rhs[index];
		}

		return *this;
	}

	Matriz operator-(const Matriz& rhs) const
	{
		assert(rhs.cNColunas == cNColunas && rhs.cNLinhas == cNLinhas);

		const int indexMax = cNLinhas * cNColunas;

		std::vector<double> nvals;
		nvals.resize(indexMax);

		for (int index = 0; index < indexMax; index++)
		{
			nvals[index] = cVals[index] - rhs[index];
		}

		return Matriz(cNLinhas, cNColunas, std::move(nvals));
	}

	Matriz operator*(const Matriz& rhs) const
	{
		assert(cNColunas == rhs.cNLinhas);

		const int nNLinhas = cNLinhas;
		const int nNColunas = rhs.cNColunas;

		std::vector<double> nVals;
		nVals.resize(nNLinhas * nNColunas);

		for (int i = 0; i < nNLinhas; i++)
		{
			for (int j = 0; j < nNColunas; j++)
			{
				double val = 0.0;

				for (int index = 0; index < cNColunas; index++)
				{
					val += cVals[i*cNColunas + index] * rhs(index, j);
				}

				nVals[i * nNColunas + j] = val;
			}
		}

		return Matriz(nNLinhas, nNColunas, std::move(nVals));
	}

	Matriz operator*(double rhs) const
	{
		Matriz res(cNLinhas, cNColunas, cVals);

		for (size_t i = 0; i < res.cVals.size(); i++)
			res[i] *= rhs;

		return res;
	}

	Matriz& operator*(double rhs)
	{
		for (size_t i = 0; i < cVals.size(); i++)
			cVals[i] *= rhs;

		return *this;
	}

	~Matriz() {};

	int GetNLinhas() const { return cNLinhas; }
	int GetNColunas() const { return cNColunas; }

	std::vector<double>& GetRefToVals() { return cVals; }

	double operator[](int index) const
	{
		return cVals[index];
	}

	double& operator[](int index)
	{
		return cVals[index];
	}

	double operator()(int i, int j) const
	{
		return cVals[j + (i * cNColunas)];
	}

	double& operator()(int i, int j)
	{
		return cVals[j + (i * cNColunas)];
	}

	Vetor operator*(const Vetor& rhs) const
	{
		const int nNLinhas = 3;
		const int nNColunas = 1;

		std::vector<double> nVals;
		nVals.resize(nNLinhas * nNColunas);

		for (int i = 0; i < nNLinhas; i++)
		{
			for (int j = 0; j < nNColunas; j++)
			{
				double val = 0.0;

				for (int index = 0; index < cNColunas; index++)
				{
					val += cVals[i * cNColunas + index] * rhs[index + 1];
				}

				nVals[i * nNColunas + j] = val;
			}
		}
		const Vetor res(nVals[0], nVals[1], nVals[2]);

		return res;
	}

	int size() const
	{
		return cVals.size();
	}

	std::vector<double>* GetPtrMatriz()
	{
		return &cVals;
	}

	void SwapCol(Matriz& m2, int col)
	{
		assert(m2.cNLinhas == cNLinhas && m2.cNColunas == cNColunas);

		double tmp;

		for (int i = 0; i < m2.cNLinhas; i++)
		{
			tmp = m2(i, col);

			m2(i, col) = this->operator()(i, col);

			this->operator()(i, col) = tmp;
		}
	}

	void SwapColNegative(Matriz& m2, int col)
	{
		assert(m2.cNLinhas == cNLinhas && m2.cNColunas == cNColunas);

		double tmp;

		for (int i = 0; i < m2.cNLinhas; i++)
		{
			tmp = -m2(i, col);

			m2(i, col) = -this->operator()(i, col);

			this->operator()(i, col) = tmp;
		}
	}

	struct Quad operator*(const Quad& rhs) const;

public:
	int cNLinhas;
	int cNColunas;

	std::vector<double> cVals;
};