#pragma once
#include "ColorShader.h"

class ColorByValue
{
public:
	ColorByValue() {};
	~ColorByValue() {}

	DirectX::XMFLOAT4 operator()(float x, float maxValue, float minValue) const
	{
		DirectX::XMFLOAT4 cor = {};
		cor.w = 1.0;//std::fabsf(x - (maxValue + minValue) / 2.0f)*100;

		if (x < 0.0)
		{
			cor.x = (x / minValue) * 25.0f;
			cor.y = (x / minValue) * 1.5;
		}
		else
		{
			cor.z = (x / maxValue) * 1.5;
			cor.y = (x / maxValue) * 25.0f;
		}

		return cor;
	}

};