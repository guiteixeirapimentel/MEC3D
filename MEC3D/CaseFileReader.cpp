#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "CaseFileReader.h"
#include <string>
#include <codecvt>
#include <locale>

#include "json.hpp"

CaseFileReader::CaseFileReader()
{}

CaseFileReader::~CaseFileReader()
{}

void CaseFileReader::GetCase(const std::string& filename,
	double& outE, double& out_v, std::vector<Ponto>& outPontos, std::vector<InstElemento>& outIndListElements,
	std::vector<CondContorno>& outPressDescritas, std::vector<CondContorno>& outDeslPrescritos)
{
	outPontos.clear();

	std::ifstream arq(filename);

	if (!arq.is_open())
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		const std::wstring msg = L"Erro ao abrir arquivo " + converter.from_bytes(filename);

		MessageBox(NULL, msg.c_str(), L"ERRO AO ABRIR CASE FILE", 0);
	}

	nlohmann::json caseJson;

	arq >> caseJson;

	arq.close();
	
	out_v = caseJson["v"];
	outE = caseJson["E"];

	std::vector<Ponto> buf;

	for (size_t i = 0; i < caseJson["partes"].size(); i++)
	{
		const std::string condContX1 = caseJson["partes"][i]["condContorno"]["X1"]["tipo"];
		const double condContValX1 = caseJson["partes"][i]["condContorno"]["X1"]["valor"];

		const std::string condContX2 = caseJson["partes"][i]["condContorno"]["X2"]["tipo"];
		const double condContValX2 = caseJson["partes"][i]["condContorno"]["X2"]["valor"];

		const std::string condContX3 = caseJson["partes"][i]["condContorno"]["X3"]["tipo"];
		const double condContValX3 = caseJson["partes"][i]["condContorno"]["X3"]["valor"];

		if (ReadStlFile(caseJson["partes"][i]["arqStl"], buf))
		{
			outPontos.reserve(outPontos.size() + buf.size());
			
			outIndListElements.reserve(outIndListElements.size() + (buf.size() / 3));

			const size_t iniI = outIndListElements.size();
			const size_t initP = outPontos.size();

			outPontos.insert(outPontos.end(), buf.begin(), buf.end());
			
			for (size_t i = 0; i < buf.size() / 3; i++)
			{
				CondContorno ccX1;
				ccX1.idEle = iniI + i + 1;
				ccX1.grauLib = 1;
				ccX1.val = condContValX1;

				if (condContX1 == "forca")
				{
					outPressDescritas.push_back(ccX1);
				}
				else if(condContX1 == "deslocamento")
				{
					outDeslPrescritos.push_back(ccX1);
				}
				else
				{
					MessageBox(NULL, L"Condições de contorno incorretas", L"Erro em .json", 0);
					return;
				}

				CondContorno ccX2;
				ccX2.idEle = iniI + i + 1;
				ccX2.grauLib = 2;
				ccX2.val = condContValX2;

				if (condContX1 == "forca")
				{
					outPressDescritas.push_back(ccX2);
				}
				else if (condContX1 == "deslocamento")
				{
					outDeslPrescritos.push_back(ccX2);
				}
				else
				{
					MessageBox(NULL, L"Condições de contorno incorretas", L"Erro em .json", 0);
					return;
				}

				CondContorno ccX3;
				ccX3.idEle = iniI + i + 1;
				ccX3.grauLib = 3;
				ccX3.val = condContValX3;

				if (condContX1 == "forca")
				{
					outPressDescritas.push_back(ccX3);
				}
				else if (condContX1 == "deslocamento")
				{
					outDeslPrescritos.push_back(ccX3);
				}
				else
				{
					MessageBox(NULL, L"Condições de contorno incorretas", L"Erro em .json", 0);
					return;
				}
			}

			for (size_t i = 0; i < buf.size(); i+=3)
			{
				outIndListElements.push_back({ int(i + initP + 1), int(i + initP + 1 + 1), int(i + initP + 2 + 1) });
			}
		}
		else
		{
			MessageBox(NULL, L"Erro ao carregar arquivo .stl", L"Erro ao carregar arquivo", 0);
		}

	}

}

bool CaseFileReader::ReadStlFile(const std::string& filename, std::vector<Ponto>& outPoints)
{
	outPoints.clear();

	std::ifstream arq(filename);

	if (!arq.is_open())
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		const std::wstring msg = L"Erro ao abrir arquivo " + converter.from_bytes(filename);

		MessageBox(NULL, msg.c_str(), L"ERRO AO ABRIR CASE FILE", 0);

		return false;
	}
	
	std::string buffer;
	buffer.resize(512);
	
	std::getline(arq, buffer);

	bool close = false;

	while (arq.good() && !close)
	{
		Ponto p1, p2, p3;
		//facet normal or endsolid
		std::getline(arq, buffer);

		if(buffer.find("endsolid") != std::string::npos)
		{
			close = true;
			continue;
		}

		//outer loop
		std::getline(arq, buffer);

		//vertex 1
		std::getline(arq, buffer, ' ');
		arq >> p1.cX1;
		arq >> p1.cX2;
		arq >> p1.cX3;

		//vertex 2
		std::getline(arq, buffer, ' ');
		arq >> p2.cX1;
		arq >> p2.cX2;
		arq >> p2.cX3;

		//vertex 3
		std::getline(arq, buffer, ' ');
		arq >> p3.cX1;
		arq >> p3.cX2;
		arq >> p3.cX3;

		outPoints.push_back(p1);
		outPoints.push_back(p2);
		outPoints.push_back(p3);

		// empt space
		std::getline(arq, buffer);
		// end loop
		std::getline(arq, buffer);

		// end facet
		std::getline(arq, buffer);
	}

	return true;
}