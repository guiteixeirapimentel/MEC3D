#pragma once
#include <cstdint>
#include <vector>

struct VertexType;

struct DATAPOINT
{
	double x;
	double y;
	double z;
	// r g b a
	double cor[4];
};

struct FlowFieldVar
{
	double u;
	double v;
	double p;
};


struct Grid
{
	std::vector<DATAPOINT> points; //pontos
	uint32_t n; //número de linhas
	uint32_t m; //número de colunas
};

struct GridMath
{
	std::vector<FlowFieldVar> points; //pontos

	double dx; // delta
	double dy; // delta
	double dz; // delta
	uint32_t n; //número de linhas
	uint32_t m; //número de colunas
	uint32_t o = 1; //profundidade
};

struct GridDirectX
{
	std::vector<VertexType> points; //pontos
	uint32_t n; //número de linhas
	uint32_t m; //número de colunas
	uint32_t o = 1; // profundidade

	const GridDirectX& operator=(const Grid& rhs);
};