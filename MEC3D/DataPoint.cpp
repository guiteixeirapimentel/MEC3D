#include "DataPoint.h"
#include "ColorShader.h"

const GridDirectX& GridDirectX::operator=(const Grid& rhs)
{
	m = rhs.m;
	n = rhs.n;

	points.resize(rhs.points.size());

	for (size_t i = 0; i < rhs.points.size(); i++)
	{
		points[i].position = { (float)rhs.points[i].x, float(rhs.points[i].y), float(rhs.points[i].z), 1.0f };
		points[i].color = { (float)rhs.points[i].cor[0], (float)rhs.points[i].cor[1], 
			(float)rhs.points[i].cor[2], (float)rhs.points[i].cor[3] };
	}

	return *this;
}