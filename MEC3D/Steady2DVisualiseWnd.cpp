#include "Steady2DVisualiseWnd.h"
#include <fstream>
#include <iostream>

Steady2DVisualiseWnd::Steady2DVisualiseWnd(const Simple2DWindow::SteadyFieldInfo2D& field,
	int viewportWidth, int viewportHeight, float dxViewport, float dyViewport, const wchar_t* titulo)
	:
	Simple2DWindow(viewportWidth, viewportHeight, titulo),
	cField(field),
	cNXViewport(viewportWidth),
	cNYViewport(viewportHeight),
	cdxViewport(dxViewport),
	cdyViewport(dyViewport),
	cPViewportBuffer(nullptr),
	cPalette({ 100, 0, 0 }, { 0, 255, 255 }, 256)
{
	cPViewportBuffer = new char[cNXViewport * cNYViewport * 4];

	memset(cPViewportBuffer, 0, cNXViewport * cNYViewport * 4);
}
Steady2DVisualiseWnd::Steady2DVisualiseWnd(Simple2DWindow::SteadyFieldInfo2D&& field, 
	int viewportWidth, int viewportHeight, float dxViewport, float dyViewport, const wchar_t* titulo)
	:
	Simple2DWindow(viewportWidth, viewportHeight, titulo),
	cNXViewport(viewportWidth),
	cNYViewport(viewportHeight),
	cdxViewport(dxViewport),
	cdyViewport(dyViewport),
	cPViewportBuffer(nullptr),
	cPalette({ 100, 0, 0 }, { 0, 255, 255 }, 256)
{
	cPViewportBuffer = new char[cNXViewport * cNYViewport * 4];

	memset(cPViewportBuffer, 0, cNXViewport * cNYViewport * 4);

	cField.cUVelField = std::move(field.cUVelField);
	cField.cVVelField = std::move(field.cVVelField);
	cField.cWVelField = std::move(field.cWVelField);
	cField.cPressField = std::move(field.cPressField);

	cField.cdx = field.cdx;
	cField.cdz = field.cdz;
	cField.cNX = field.cNX;
	cField.cNZ = field.cNZ;
}

Steady2DVisualiseWnd::Steady2DVisualiseWnd(const std::string& datBaseFilename, int64_t jValue, float dx, float dz,
	int viewportWidth, int viewportHeight, float dxViewport, float dyViewport, const wchar_t* titulo)
	:
	Simple2DWindow(viewportWidth, viewportHeight, titulo),
	cNXViewport(viewportWidth),
	cNYViewport(viewportHeight),
	cdxViewport(dxViewport),
	cdyViewport(dyViewport),
	cPViewportBuffer(nullptr),
	cPalette({ {255, 0, 0}, {255, 255, 255}, { 0, 0, 250 } }, 513)
{
	cField.cdx = dx;
	cField.cdz = dz;

	cPViewportBuffer = new char[cNXViewport * cNYViewport * 4];

	memset(cPViewportBuffer, 0, cNXViewport * cNYViewport * 4);

	std::vector<double> ff;

	{ // open U file

		std::ifstream uFile(datBaseFilename + "U.dat", std::ios::binary);
		
		int64_t NX, NY, NZ;

		uFile.read(reinterpret_cast<char*>(&NX), sizeof(int64_t));
		uFile.read(reinterpret_cast<char*>(&NY), sizeof(int64_t));
		uFile.read(reinterpret_cast<char*>(&NZ), sizeof(int64_t));

		cField.cNX = NX;
		cField.cNZ = NZ;

		cField.cUVelField.resize(NX * NZ);

		ff.resize(NX * NZ);
		
		uFile.ignore(((NX - 1) + ((NZ - 1) * NX) + ((jValue-1) * NX * NZ)) * sizeof(double));

		uFile.read(reinterpret_cast<char*>(ff.data()), ff.size() * sizeof(double));

		for (size_t i = 0; i < ff.size(); i++)
		{
			cField.cUVelField[i] = static_cast<float>(ff[i]);
		}

		uFile.close();
	}	

	{ // open V file

		std::ifstream vFile(datBaseFilename + "V.dat", std::ios::binary);

		int64_t NX, NY, NZ;

		vFile.read(reinterpret_cast<char*>(&NX), sizeof(int64_t));
		vFile.read(reinterpret_cast<char*>(&NY), sizeof(int64_t));
		vFile.read(reinterpret_cast<char*>(&NZ), sizeof(int64_t));

		cField.cNX = NX;
		cField.cNZ = NZ;

		cField.cVVelField.resize(NX * NZ);

		vFile.ignore(((NX - 1) + ((NZ - 1) * NX) + ((jValue - 1) * NX * NZ)) * sizeof(double));

		vFile.read(reinterpret_cast<char*>(ff.data()), ff.size() * sizeof(double));

		for (size_t i = 0; i < ff.size(); i++)
		{
			cField.cVVelField[i] = static_cast<float>(ff[i]);
		}

		vFile.close();
	}

	{ // open W file

		std::ifstream wFile(datBaseFilename + "W.dat", std::ios::binary);

		int64_t NX, NY, NZ;

		wFile.read(reinterpret_cast<char*>(&NX), sizeof(int64_t));
		wFile.read(reinterpret_cast<char*>(&NY), sizeof(int64_t));
		wFile.read(reinterpret_cast<char*>(&NZ), sizeof(int64_t));

		cField.cNX = NX;
		cField.cNZ = NZ;

		cField.cWVelField.resize(NX * NZ);

		wFile.ignore(((NX - 1) + ((NZ - 1) * NX) + ((jValue - 1) * NX * NZ)) * sizeof(double));

		wFile.read(reinterpret_cast<char*>(ff.data()), ff.size() * sizeof(double));

		for (size_t i = 0; i < ff.size(); i++)
		{
			cField.cWVelField[i] = static_cast<float>(ff[i]);
		}

		wFile.close();
	}

	{ // open P file

		std::ifstream pFile(datBaseFilename + "P.dat", std::ios::binary);

		int64_t NX, NY, NZ;

		pFile.read(reinterpret_cast<char*>(&NX), sizeof(int64_t));
		pFile.read(reinterpret_cast<char*>(&NY), sizeof(int64_t));
		pFile.read(reinterpret_cast<char*>(&NZ), sizeof(int64_t));

		cField.cNX = NX;
		cField.cNZ = NZ;

		cField.cPressField.resize(NX * NZ);

		pFile.ignore(((NX - 1) + ((NZ - 1) * NX) + ((jValue - 1) * NX * NZ)) * sizeof(double));

		pFile.read(reinterpret_cast<char*>(ff.data()), ff.size() * sizeof(double));

		for (size_t i = 0; i < ff.size(); i++)
		{
			cField.cPressField[i] = static_cast<float>(ff[i]);
		}

		pFile.close();
	}

}
Steady2DVisualiseWnd::~Steady2DVisualiseWnd()
{
	if (cPViewportBuffer)
	{
		delete[] cPViewportBuffer;
		cPViewportBuffer = nullptr;
	}
}

void Steady2DVisualiseWnd::CutField(const int xOffset, const int yOffset, const int newWidth, const int newHeight)
{
	Simple2DWindow::SteadyFieldInfo2D newField;

	newField.cdx = cField.cdx;
	newField.cdz = cField.cdz;
	newField.cNX = newWidth;
	newField.cNZ = newHeight;

	assert(xOffset + newWidth < cField.cNX);
	assert(yOffset + newHeight < cField.cNZ);

	newField.cPressField.resize(newWidth * newHeight, 0.0f);
	newField.cUVelField.resize(newWidth * newHeight, 0.0f);
	newField.cVVelField.resize(newWidth * newHeight, 0.0f);
	newField.cWVelField.resize(newWidth * newHeight, 0.0f);

	for (int k = 0; k < newHeight; k++)
	{
		for (int i = 0; i < newWidth; i++)
		{
			newField.cPressField[i + (k * newWidth)] = cField.cPressField[(i+xOffset) + ((k+yOffset) * cField.cNX)];
			newField.cUVelField[i + (k * newWidth)]	 = cField.cUVelField[(i+xOffset) + ((k+yOffset) * cField.cNX)];	
			newField.cVVelField[i + (k * newWidth)]	 = cField.cVVelField[(i+xOffset) + ((k+yOffset) * cField.cNX)];	
			newField.cWVelField[i + (k * newWidth)]	 = cField.cWVelField[(i+xOffset) + ((k+yOffset) * cField.cNX)];	
		}
	}

	cField = std::move(newField);
}

void Steady2DVisualiseWnd::Draw()
{
	RECT r = {};

	GetClientRect(cHWnd, &r);

	const int width = r.right - r.left;
	const int height = r.bottom - r.top;

	BITMAPINFO bi = {};
	bi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bi.bmiHeader.biWidth = cNXViewport;
	bi.bmiHeader.biHeight = -cNYViewport;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = cNXViewport * cNYViewport * 4;

	ValidateRect(cHWnd, nullptr);
	HDC hdc = GetDC(cHWnd);

	SetStretchBltMode(hdc, HALFTONE);
	// Exibe na janela o buffer onde foi armazenado os pixels.
	int res = StretchDIBits(hdc, 0, 0, width, height, 0, 0, cNXViewport, cNYViewport, cPViewportBuffer, &bi, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(cHWnd, hdc);

	if (res == 0)
	{
		MessageBox(NULL, L"Erro ao desenhar na tela.", L"Erro", 0);
	}
}
void Steady2DVisualiseWnd::Update()
{
	if (cNeedsUpdate)
	{
		cNeedsUpdate = false;
		switch (cShowing)
		{
		case 0:
			SetBufferToU();
			//SetWindowTextA(cHWnd, "U");
			break;
		case 1:
			SetBufferToV();
			//SetWindowTextA(cHWnd, "V");
			break;
		case 2:
			SetBufferToW();
			//SetWindowTextA(cHWnd, "W");
			break;
		case 3:
			SetBufferToP();
			//SetWindowTextA(cHWnd, "P");
			break;
		case 4: 
			SetBufferToCurl();
			//SetWindowTextA(cHWnd, "Curl");
			break;
		case 5:
			SetBufferToVel();
			//SetWindowTextA(cHWnd, "Vel");
			break;
		}
	}

}

void Steady2DVisualiseWnd::Input()
{
	char key = 0;
	while (key = cKeyboard.ReadKey())
	{
		if (key == VK_RIGHT)
		{
			cShowing++;
			if (cShowing > 5)
			{
				cShowing = 0;
			}

			cNeedsUpdate = true;
		}

		if (key == VK_LEFT)
		{
			cShowing--;
			if (cShowing < 0)
			{
				cShowing = 5;
			}

			cNeedsUpdate = true;
		}
	}

	while (!cMouse.MouseEmpty())
	{
		MouseEvent me = cMouse.ReadMouse();

		switch (me.GetType())
		{
		case MouseEvent::WheelUp:
		{
		}
		break;
		case MouseEvent::WheelDown:
		{
		}
		break;
		}

	}

	if (cMouse.LeftIsPressed())
	{

	}
	else
	{
	}
}

void Steady2DVisualiseWnd::PutPixel(int x, int y, int cor)
{
	reinterpret_cast<int*>(cPViewportBuffer)[x + (y * cNXViewport)] = cor;
}

void Steady2DVisualiseWnd::SetBufferToU()
{
	std::vector<float> uVels;
	uVels.resize(cNXViewport * cNYViewport);

	float biggestU = 0.0f;
	float smallestU = 0.0f;

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float xvp = i * cdxViewport;
			const float yvp = j * cdyViewport;

			const int iField = (xvp / cField.cdx) + 1;
			const int iFieldm = (xvp / cField.cdx);

			const int jField = (yvp / cField.cdz)+1;
			const int jFieldm = (yvp / cField.cdz);
			
			float uVel = 0.0f;

			if (iField < cField.cNX && jField < cField.cNZ)
			{
				uVel = cField.cUVelField[iField + (jField * cField.cNX)];

				//if (iField > 1)
				//{
				//	uVel += (cField.cUVelField[iField + (jField * cField.cNX)] - cField.cUVelField[iFieldm + (jField * cField.cNX)])
				//		* (xvp - (iFieldm * cField.cdx)) / cField.cdx;
				//}
				//if (jField > 0)
				//{
				//	uVel += (cField.cUVelField[iField + (jField * cField.cNX)] - cField.cUVelField[iField + (jFieldm * cField.cNX)])
				//		* (yvp - (jFieldm * cField.cdz)) / cField.cdz;
				//}							
			}			

			uVels[i + (j * cNXViewport)] = uVel;

			if (uVel > biggestU)
				biggestU = uVel;
			if (uVel < smallestU)
				smallestU = uVel;
		}
	}

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float uvel = uVels[i + (j * cNXViewport)];

			const float A = (biggestU- smallestU);
			const float r = (uvel - smallestU) / A;

			int cor = cPalette.GetColor(r * 512);

			if (uvel == 0.0f)
				cor = 0xFFFFFFFF;

			PutPixel(i, j, cor);

		}
	}

	std::cout << "Maior U: " << biggestU << std::endl;
	std::cout << "Menor U: " << smallestU << std::endl;
	std::cout << "\n";

}

void Steady2DVisualiseWnd::SetBufferToV()
{
	std::vector<float> vVels;
	vVels.resize(cNXViewport * cNYViewport);

	float biggestV = 0.0f;
	float smallestV = 0.0f;

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float xvp = i * cdxViewport;
			const float yvp = j * cdyViewport;

			const int iField = (xvp / cField.cdx)+1;
			const int iFieldm = (xvp / cField.cdx);

			const int jField = (yvp / cField.cdz)+1;
			const int jFieldm = (yvp / cField.cdz);

			float vVel = 0.0f;

			if (iField < cField.cNX && jField < cField.cNZ)
			{
				vVel = cField.cVVelField[iField + (jField * cField.cNX)];

				//if (iField > 0)
				//{
				//	vVel += (cField.cVVelField[iField + (jField * cField.cNX)] - cField.cVVelField[iFieldm + (jField * cField.cNX)])
				//		* (xvp - (iFieldm * cField.cdx)) / cField.cdx;
				//}
				//if (jField > 0)
				//{
				//	vVel += (cField.cVVelField[iField + (jField * cField.cNX)] - cField.cVVelField[iField + (jFieldm * cField.cNX)])
				//		* (yvp - (jFieldm * cField.cdz)) / cField.cdz;
				//}		

				
			}

			vVels[i + (j * cNXViewport)] = vVel;

			if (vVel > biggestV)
				biggestV = vVel;
			if (vVel < smallestV)
				smallestV = vVel;
			
		}
	}

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float vvel = vVels[i + (j * cNXViewport)];

			const float A = (biggestV - smallestV);
			const float r = (vvel - smallestV) / A;

			int cor = cPalette.GetColor(r * 512);

			if (vvel == 0.0f)
				cor = 0xFFFFFFFF;

			PutPixel(i, j, cor);
		}
	}

	std::cout << "Maior V: " << biggestV << std::endl;
	std::cout << "Menor V: " << smallestV << std::endl;
	std::cout << "\n";
}
void Steady2DVisualiseWnd::SetBufferToW()
{
	std::vector<float> wVels;
	wVels.resize(cNXViewport * cNYViewport);

	float biggestW = 0.0f;
	float smallestW = 0.0f;

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float xvp = i * cdxViewport;
			const float yvp = j * cdyViewport;

			const int iField = (xvp / cField.cdx)+1;
			const int iFieldm = (xvp / cField.cdx);

			const int jField = (yvp / cField.cdz)+1;
			const int jFieldm = (yvp / cField.cdz);

			float wVel = 0.0f;

			if (iField < cField.cNX && jField < cField.cNZ)
			{
				wVel = cField.cWVelField[iField + (jField * cField.cNX)];

				//if (iField > 0)
				//{
				//	wVel += (cField.cWVelField[iField + (jField * cField.cNX)] - cField.cWVelField[iFieldm + (jField * cField.cNX)])
				//		* (xvp - (iFieldm * cField.cdx)) / cField.cdx;
				//}
				//if (jField > 1)
				//{
				//	wVel += (cField.cWVelField[iField + (jField * cField.cNX)] - cField.cWVelField[iField + (jFieldm * cField.cNX)])
				//		* (yvp - (jFieldm * cField.cdz)) / cField.cdz;
				//}
			}

			wVels[i + (j * cNXViewport)] = wVel;

			if (wVel > biggestW)
				biggestW = wVel;
			if (wVel < smallestW)
				smallestW = wVel;
		}
	}

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float wvel = wVels[i + (j * cNXViewport)];

			const float A = (biggestW - smallestW);
			const float r = (wvel - smallestW) / A;

			int cor = cPalette.GetColor(r * 512);

			if (wvel == 0.0f)
				cor = 0xFFFFFFFF;

			PutPixel(i, j, cor);
		}
	}

	std::cout << "Maior W: " << biggestW << std::endl;
	std::cout << "Menor W: " << smallestW << std::endl;
	std::cout << "\n";
}
void Steady2DVisualiseWnd::SetBufferToP()
{
	std::vector<float> pressures;
	pressures.resize(cNXViewport * cNYViewport);

	float biggestP = 0.0f;
	float smallestP = 0.0f;

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float xvp = i * cdxViewport;
			const float yvp = j * cdyViewport;

			const int iField = (xvp / cField.cdx)+1;
			const int iFieldm = (xvp / cField.cdx);

			const int jField = (yvp / cField.cdz)+1;
			const int jFieldm = (yvp / cField.cdz);

			float pressure = 0.0f;

			if (iField < cField.cNX && jField < cField.cNZ)
			{
				pressure = cField.cPressField[iField + (jField * cField.cNX)];

				//if (iField > 0)
				//{
				//	pressure += (cField.cPressField[iField + (jField * cField.cNX)] - cField.cPressField[iFieldm + (jField * cField.cNX)])
				//		* (xvp - (iFieldm * cField.cdx)) / cField.cdx;
				//}
				//if (jField > 0)
				//{
				//	pressure += (cField.cPressField[iField + (jField * cField.cNX)] - cField.cPressField[iField + (jFieldm * cField.cNX)])
				//		* (yvp - (jFieldm * cField.cdz)) / cField.cdz;
				//}
			}

			pressures[i + (j * cNXViewport)] = pressure;

			if (pressure > biggestP)
				biggestP = pressure;
			if (pressure < smallestP)
				smallestP = pressure;
		}
	}

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float pressure = pressures[i + (j * cNXViewport)];

			const float A = (biggestP - smallestP);
			const float r = (pressure - smallestP) / A;

			int cor = cPalette.GetColor(r * 512);

			if (pressure == 0.0f)
				cor = 0xFFFFFFFF;

			PutPixel(i, j, cor);
		}
	}

	std::cout << "Maior P: " << biggestP << std::endl;
	std::cout << "Menor P: " << smallestP << std::endl;
	std::cout << "\n";
}

void Steady2DVisualiseWnd::SetBufferToCurl()
{
	std::vector<float> curls;
	curls.resize(cNXViewport * cNYViewport, 0.0f);

	float biggestCurl = 0.0f;
	float smallestCurl = 0.0f;

	int iBC = 0;
	int jBC = 0;

	int iSC = 0;
	int jSC = 0;

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float xvp = i * cdxViewport;
			const float yvp = j * cdyViewport;

			const int iFieldU = ((xvp-(cdxViewport/2)) / cField.cdx) + 1;
			const int iFieldmU = ((xvp - (cdxViewport / 2)) / cField.cdx);

			const int jFieldU = ((yvp-(cdyViewport/2)) / cField.cdz) + 1;
			const int jFieldmU = ((yvp - (cdyViewport / 2)) / cField.cdz);

			const int iFieldW = ((xvp - (cdxViewport / 2)) / cField.cdx) + 1;
			const int iFieldmW = ((xvp - (cdxViewport / 2)) / cField.cdx);

			const int jFieldW = ((yvp - (cdyViewport / 2)) / cField.cdz) + 1;
			const int jFieldmW = ((yvp - (cdyViewport / 2)) / cField.cdz);

			float uComp = 0.0f;
			float wComp = 0.0f;

			if (iFieldU < cField.cNX && jFieldU < cField.cNZ)
			{
				if (iFieldW > 0)
				{
					const double wvel = cField.cWVelField[iFieldW + (jFieldW * cField.cNX)];
					const double wvelm = cField.cWVelField[iFieldmW + (jFieldW * cField.cNX)];

					if(wvel != 0.0 && wvelm != 0.0)
						wComp = (wvel - wvelm) / cField.cdx;
					else if (wvel != 0.0)
					{
						wComp = (cField.cWVelField[iFieldW + 1 + (jFieldW * cField.cNX)] - wvel) / cField.cdx;
					}

				}
				if (jFieldU > 0)
				{
					const double uvel = cField.cUVelField[iFieldU + (jFieldU * cField.cNX)];
					const double uvelm = cField.cUVelField[iFieldU + (jFieldmU * cField.cNX)];

					if(uvel != 0.0 && uvelm != 0.0)
						uComp = (uvel - uvelm) / cField.cdz;
					else if (uvel != 0.0)
					{
						uComp = (cField.cUVelField[iFieldU + (jFieldU + 1 * cField.cNX)] - uvel) / cField.cdz;
					}
				}

				curls[i + (j * cNXViewport)] = (wComp - uComp);

				if (curls[i + (j * cNXViewport)] > biggestCurl)
				{
					biggestCurl = curls[i + (j * cNXViewport)];
					iBC = iFieldU;
					jBC = jFieldU;
				}
					
				if (curls[i + (j * cNXViewport)] < smallestCurl)
				{
					smallestCurl = curls[i + (j * cNXViewport)];
					iSC = iFieldU;
					jSC = jFieldU;
				}
			}
		}
	}

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			float curl = curls[i + (j * cNXViewport)];

			float A = (biggestCurl - smallestCurl);
			float r = (curl - smallestCurl) / A;

			int palIndex = 0;
			if (curl > 0.0)
			{
				palIndex = (512 / 2) + ((512 / 2) * (curl / biggestCurl));
			}
			else
			{
				palIndex = (512 / 2) - ((512 / 2) * (curl / smallestCurl));
			}

			int cor = cPalette.GetColor(palIndex);

			if (curl == 0.0f)
				cor = 0xFFFFFFFF;

			PutPixel(i, j, cor);

		}
	}

	std::cout << "Maior CURL: " << biggestCurl << std::endl;
	std::cout << "Menor CURL: " << smallestCurl << std::endl;
	std::cout << "ibc " << iBC << " jbc " << jBC << std::endl;
	std::cout << "isc " << iSC << " jsc " << iSC << std::endl;
	std::cout << "\n";
}

void Steady2DVisualiseWnd::SetBufferToVel()
{
	std::vector<float> vels;
	vels.resize(cNXViewport * cNYViewport);

	float biggestVel = 0.0f;
	float smallestVel = 0.0f;

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float xvp = i * cdxViewport;
			const float yvp = j * cdyViewport;

			const int iField = (xvp / cField.cdx) + 1;
			const int iFieldm = (xvp / cField.cdx);

			const int jField = (yvp / cField.cdz) + 1;
			const int jFieldm = (yvp / cField.cdz);

			float uVel = 0.0f;
			float vVel = 0.0f;
			float wVel = 0.0f;

			if (iField < cField.cNX && jField < cField.cNZ)
			{
				uVel = cField.cUVelField[iField + (jField * cField.cNX)];
				vVel = cField.cVVelField[iField + (jField * cField.cNX)];
				wVel = cField.cWVelField[iField + (jField * cField.cNX)];

				//if (iField > 1)
				//{
				//	uVel += (cField.cUVelField[iField + (jField * cField.cNX)] - cField.cUVelField[iFieldm + (jField * cField.cNX)])
				//		* (xvp - (iFieldm * cField.cdx)) / cField.cdx;
				//
				//	vVel += (cField.cVVelField[iField + (jField * cField.cNX)] - cField.cVVelField[iFieldm + (jField * cField.cNX)])
				//		* (xvp - (iFieldm * cField.cdx)) / cField.cdx;
				//
				//	wVel += (cField.cWVelField[iField + (jField * cField.cNX)] - cField.cWVelField[iFieldm + (jField * cField.cNX)])
				//		* (xvp - (iFieldm * cField.cdx)) / cField.cdx;
				//}				
				//if (jField > 0)
				//{
				//	uVel += (cField.cUVelField[iField + (jField * cField.cNX)] - cField.cUVelField[iField + (jFieldm * cField.cNX)])
				//		* (yvp - (jFieldm * cField.cdz)) / cField.cdz;
				//
				//	vVel += (cField.cVVelField[iField + (jField * cField.cNX)] - cField.cVVelField[iField + (jFieldm * cField.cNX)])
				//		* (yvp - (jFieldm * cField.cdz)) / cField.cdz;
				//
				//	wVel += (cField.cWVelField[iField + (jField * cField.cNX)] - cField.cWVelField[iField + (jFieldm * cField.cNX)])
				//		* (yvp - (jFieldm * cField.cdz)) / cField.cdz;
				//}

				
			}
			vels[i + (j * cNXViewport)] = sqrtf((uVel * uVel) + (vVel * vVel) + (wVel * wVel));

			if (vels[i + (j * cNXViewport)] > biggestVel)
				biggestVel = vels[i + (j * cNXViewport)];
			if (vels[i + (j * cNXViewport)] < smallestVel)
				smallestVel = vels[i + (j * cNXViewport)];
		}
	}

	for (int j = 0; j < cNYViewport; j++)
	{
		for (int i = 0; i < cNXViewport; i++)
		{
			const float vel = vels[i + (j * cNXViewport)];

			const float A = (biggestVel - smallestVel);
			const float r = (vel - smallestVel) / A;

			int cor = cPalette.GetColor(r * 512);

			if (vel == 0.0f)
				cor = 0xFFFFFFFF;

			PutPixel(i, j, cor);

		}
	}

	std::cout << "Maior Vel: " << biggestVel << std::endl;
	std::cout << "Menor Vel: " << smallestVel << std::endl;
	std::cout << "\n";
}