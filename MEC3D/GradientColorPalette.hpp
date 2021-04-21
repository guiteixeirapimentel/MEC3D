#pragma once
#include <cassert>
#include <vector>

class GradientColorPalette
{
public:
	struct Color
	{
		int r;
		int g;
		int b;
	};

public:
	GradientColorPalette(Color c0, Color cend, int palletSize)
		:
		cPalletSize(palletSize),
		cPPallet(nullptr)
	{
		cPPallet = new int[palletSize];

		for (int i = 1; i <= palletSize; i++) 
		{
			const int itDiff = palletSize - i;
			 
			const int r = static_cast<int>(float((cend.r * i) + (c0.r * itDiff)) / palletSize);
			const int g = static_cast<int>(float((cend.g * i) + (c0.g * itDiff)) / palletSize);
			const int b = static_cast<int>(float((cend.b * i) + (c0.b * itDiff)) / palletSize);
			
			cPPallet[i - 1] = 0xFF000000 | (r << 16) | (g << 8) | b;
		}


	}

	GradientColorPalette(Color c0, Color c1, Color c2, int palletSize)
		:
		cPalletSize(palletSize),
		cPPallet(nullptr)
	{
		cPPallet = new int[palletSize];

		const int half = palletSize / 2;

		for (int i = 1; i <= half; i++)
		{
			const int itDiff = half - i;

			const int r = static_cast<int>(float((c1.r * i) + (c0.r * itDiff)) / palletSize);
			const int g = static_cast<int>(float((c1.g * i) + (c0.g * itDiff)) / palletSize);
			const int b = static_cast<int>(float((c1.b * i) + (c0.b * itDiff)) / palletSize);

			cPPallet[i - 1] = 0xFF000000 | (r << 16) | (g << 8) | b;
		}

		for (int i = 1; i <= half; i++)
		{
			const int itDiff = half - i;

			const int r = static_cast<int>(float((c2.r * i) + (c1.r * itDiff)) / palletSize);
			const int g = static_cast<int>(float((c2.g * i) + (c1.g * itDiff)) / palletSize);
			const int b = static_cast<int>(float((c2.b * i) + (c1.b * itDiff)) / palletSize);

			cPPallet[i + half - 1] = 0xFF000000 | (r << 16) | (g << 8) | b;
		}
	}

	GradientColorPalette(const std::vector<Color>& cores, int palletSize)
		:
		cPalletSize(palletSize),
		cPPallet(nullptr)
	{
		assert(cores.size() > 1);

		cPPallet = new int[palletSize];

		const int lim = palletSize / (cores.size() - 1);

		for (size_t index = 0; index < cores.size() - 1; index++)
		{
			for (int i = 1; i <= lim; i++)
			{
				const int itDiff = lim - i;

				const int r = static_cast<int>(float((cores[index + 1].r * i) + (cores[index].r * itDiff)) / palletSize);
				const int g = static_cast<int>(float((cores[index + 1].g * i) + (cores[index].g * itDiff)) / palletSize);
				const int b = static_cast<int>(float((cores[index + 1].b * i) + (cores[index].b * itDiff)) / palletSize);

				cPPallet[i + (lim * index) - 1] = 0xFF000000 | (r << 16) | (g << 8) | b;
			}
		}
	}
	
	GradientColorPalette()
		:
		cPalletSize(256),
		cPPallet(nullptr)
	{
		cPPallet = new int[cPalletSize];

		for (int i = 1; i <= cPalletSize; i++)
		{
			const int itDiff = cPalletSize - i;

			const int r = float((100 * i) + (0 * itDiff)) / cPalletSize;
			const int g = float((0 * i) + (255 * itDiff)) / cPalletSize;
			const int b = float((0 * i) + (255 * itDiff)) / cPalletSize;

			cPPallet[i - 1] = 0xFF000000 | (r << 16) | (g << 8) | b;
		}

	}
	~GradientColorPalette()
	{
		if (cPPallet)
		{
			delete[] cPPallet;
			cPPallet = nullptr;
		}
	}

	int GetColor(int index) const
	{
		if (index < cPalletSize)
		{
			return cPPallet[index];
		}
		else
		{
			assert(0);

			return 0;
		}
	}

private:
	const int cPalletSize;
	
	int* cPPallet;
};