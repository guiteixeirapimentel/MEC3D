#pragma once
#include "MalhaTriangular.h"


class CaseFileReader
{
public:
	CaseFileReader();
	~CaseFileReader();

	static void GetCase(const std::string& filename,
		double& outE, double& out_v, std::vector<Ponto>& outPontos, std::vector<InstElemento>& outIndListElements,
		std::vector<CondContorno>& outPressDescritas, std::vector<CondContorno>& outDeslPrescritos);

private:
	static bool ReadStlFile(const std::string& filename, std::vector<Ponto>& outPoints);
	
};