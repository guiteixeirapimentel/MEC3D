#pragma once

class Ponto
{
public:
	Ponto(double x1, double x2, double x3) : cX1(x1), cX2(x2), cX3(x3) {};
	Ponto(): cX1(0.0), cX2(0.0), cX3(0.0) {};
	~Ponto() {};

	class Vetor operator-(const Vetor& rhs) const;
	class Vetor operator+(const Vetor& rhs) const;

	operator Vetor() const;

	double operator [](int i) const
	{
		switch (i) {
		case 0:
			return cX1;
			break;
		case 1:
			return cX2;
			break;
		default:
			return cX3;
			break;
		}
	}
	   
public:
	double cX1;
	double cX2;
	double cX3;
};