#pragma once
#include "MalhaTriangular.h"

class GeometryCreator
{
public:
	GeometryCreator() {}
	~GeometryCreator() {}

	static void GetCylinderInternalPressure(double R, double L, double h, double press_int,
		double delta_theta, double delta_y, double delta_r,
		std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs, std::vector<CondContorno>& outPress);

	static void GetCrucifix(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
		std::vector<CondContorno>& outPress);

	static void GetSwimmingPool(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
		std::vector<CondContorno>& outPress);
	
	static void GetAxialStress(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
		std::vector<CondContorno>& outPress);

	static void GetBendedRectBar(std::vector<Quad>& outQuads, std::vector<CondContorno>& outDelocs,
		std::vector<CondContorno>& outPress);

private:

};