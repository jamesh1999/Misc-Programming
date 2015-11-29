//
// Melter - By Napalm @ NetCore2K.net
// Inspired by MetalHead (http://www.rohitab.com/discuss/index.php?showtopic=23191)
//
#include "stdafx.h"
#include <windows.h>
#include <cmath>

int nRandWidth = 150, nRandY = 15, nRandX = 15, nSpeed = 1;
int nScreenWidth, nScreenHeight;

int maxXShift = 100;
int maxYShift = 100;
int freezeChance = 10 / nSpeed;
int glitchChance = 3;
int glitchFreezeChance = 4;
int clearChance = 10;
bool glitching = false;
bool glitchingPause = false;
bool freezing = false;
bool createArtifact = false;

inline int GetFilePointer(HANDLE FileHandle){
	return SetFilePointer(FileHandle, 0, 0, FILE_CURRENT);
}

bool SaveBMPFile(LPCWSTR filename, HBITMAP bitmap, HDC bitmapDC, int width, int height){
	bool Success = false;
	HDC SurfDC = NULL;        // GDI-compatible device context for the surface
	HBITMAP OffscrBmp = NULL; // bitmap that is converted to a DIB
	HDC OffscrDC = NULL;      // offscreen DC that we can select OffscrBmp into
	LPBITMAPINFO lpbi = NULL; // bitmap format info; used by GetDIBits
	LPVOID lpvBits = NULL;    // pointer to bitmap bits array
	HANDLE BmpFile = INVALID_HANDLE_VALUE;    // destination .bmp file
	BITMAPFILEHEADER bmfh;  // .bmp file header
	// We need an HBITMAP to convert it to a DIB:
	if ((OffscrBmp = CreateCompatibleBitmap(bitmapDC, width, height)) == NULL)
		return false;
	// The bitmap is empty, so let's copy the contents of the surface to it.
	// For that we need to select it into a device context. We create one.
	if ((OffscrDC = CreateCompatibleDC(bitmapDC)) == NULL)
		return false;
	// Select OffscrBmp into OffscrDC:
	HBITMAP OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);
	// Now we can copy the contents of the surface to the offscreen bitmap:
	BitBlt(OffscrDC, 0, 0, width, height, bitmapDC, 0, 0, SRCCOPY);
	// GetDIBits requires format info about the bitmap. We can have GetDIBits
	// fill a structure with that info if we pass a NULL pointer for lpvBits:
	// Reserve memory for bitmap info (BITMAPINFOHEADER + largest possible
	// palette):
	if ((lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)])) == NULL)
		return false;
	ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Get info but first de-select OffscrBmp because GetDIBits requires it:
	SelectObject(OffscrDC, OldBmp);
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, NULL, lpbi, DIB_RGB_COLORS))
		return false;
	// Reserve memory for bitmap bits:
	if ((lpvBits = new char[lpbi->bmiHeader.biSizeImage]) == NULL)
		return false;
	// Have GetDIBits convert OffscrBmp to a DIB (device-independent bitmap):
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, lpvBits, lpbi, DIB_RGB_COLORS))
		return false;
	// Create a file to save the DIB to:
	if ((BmpFile = CreateFile(filename,
		GENERIC_WRITE,
		0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL)) == INVALID_HANDLE_VALUE)

		return false;
	DWORD Written;    // number of bytes written by WriteFile

	// Write a file header to the file:
	bmfh.bfType = 19778;        // 'BM'
	// bmfh.bfSize = ???        // we'll write that later
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	// bmfh.bfOffBits = ???     // we'll write that later
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
		return false;
	if (Written < sizeof(bmfh))
		return false;
	// Write BITMAPINFOHEADER to the file:
	if (!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER), &Written, NULL))
		return false;

	if (Written < sizeof(BITMAPINFOHEADER))
		return false;
	// Calculate size of palette:
	int PalEntries;
	// 16-bit or 32-bit bitmaps require bit masks:
	if (lpbi->bmiHeader.biCompression == BI_BITFIELDS)
		PalEntries = 3;
	else
		// bitmap is palettized?
		PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
		// 2^biBitCount palette entries max.:
		(int)(1 << lpbi->bmiHeader.biBitCount)
		// bitmap is TrueColor -> no palette:
		: 0;
	// If biClrUsed use only biClrUsed palette entries:
	if (lpbi->bmiHeader.biClrUsed)
		PalEntries = lpbi->bmiHeader.biClrUsed;
	// Write palette to the file:
	if (PalEntries){
		if (!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD), &Written, NULL))
			return false;
		if (Written < PalEntries * sizeof(RGBQUAD))
			return false;
	}
	// The current position in the file (at the beginning of the bitmap bits)
	// will be saved to the BITMAPFILEHEADER:
	bmfh.bfOffBits = GetFilePointer(BmpFile);
	// Write bitmap bits to the file:
	if (!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage, &Written, NULL))
		return false;

	if (Written < lpbi->bmiHeader.biSizeImage)
		return false;
	// The current pos. in the file is the final file size and will be saved:
	bmfh.bfSize = GetFilePointer(BmpFile);
	// We have all the info for the file header. Save the updated version:
	SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
		return false;
	if (Written < sizeof(bmfh))
		return false;
	return true;
}

bool ScreenCapture(int x, int y, int width, int height, LPCWSTR filename){
	// get a DC compat. w/ the screen
	HDC hDc = CreateCompatibleDC(0);

	// make a bmp in memory to store the capture in
	HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);

	// join em up
	SelectObject(hDc, hBmp);

	// copy from the screen to my bitmap
	BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);

	// save my bitmap
	bool ret = SaveBMPFile(filename, hBmp, hDc, width, height);

	// free the bitmap memory
	DeleteObject(hBmp);

	return ret;
}

LRESULT WINAPI MelterProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg){
	case WM_CREATE:
	{
		HDC hdcDesktop = GetDC(HWND_DESKTOP);
		HDC hdcWindow = GetDC(hWnd);
		BitBlt(hdcWindow, 0, 0, nScreenWidth, nScreenHeight, hdcDesktop, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdcWindow);

		ScreenCapture(0, 0, nScreenWidth, nScreenHeight, L"buffer.bmp");

		ReleaseDC(HWND_DESKTOP, hdcDesktop);
		SetTimer(hWnd, 0, nSpeed, NULL);
		ShowWindow(hWnd, SW_SHOW);

		ShowCursor(FALSE);
	}
		return 0;
	case WM_ERASEBKGND:
		return 0;
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	case WM_TIMER:
	{
		HDC hdcWindow = GetDC(hWnd);

		if (freezing)
		{
			if (rand() % freezeChance == 0)
			{
				freezing = false;
			}
		}

		else if (glitching)
		{
			if (glitchingPause)
			{
				if (rand() % glitchFreezeChance == 0)
				{
					glitchingPause = false;
				}
			}
			else
			{
				int movX = (rand() % nRandX),
					movY = (rand() % nRandY);

				if (rand() % 2)
					movX *= -1;

				if (rand() % 2)
					movY *= -1;

				BitBlt(hdcWindow, movX > 0 ? movX : 0, movY > 0 ? movY : 0, nScreenWidth, nScreenHeight, hdcWindow, movY < 0 ? -movY : 0, movY < 0 ? -movY : 0, SRCCOPY);

				if (rand() % glitchChance == 0)
				{
					glitching = false;
				}
				else
					glitchingPause = true;
			}
		}
		//If not performing a prolonged effect
		else
		{
			//Decide whether to refresh
			if (rand() % clearChance == 0)
			{
				HBITMAP image = (HBITMAP)LoadImage(0, L"buffer.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				HDC hdc = CreateCompatibleDC(hdcWindow);
				SelectObject(hdc, image);
				BitBlt(hdcWindow, 0, 0, nScreenWidth, nScreenHeight, hdc, 0, 0, SRCCOPY);
				DeleteDC(hdc);
				DeleteObject(image);
			}

			int choice = rand() % 2;
			switch (choice)
			{
				//Artifact
			case 0:
			{
				createArtifact = true;
			}
				//Freeze
			case 1:
			{
				freezing = true;
				break;
			}
			}
		}

		//Select artifact type
		if (createArtifact)
		{
			switch (rand() % 9)
			{
			//Disalign band
			case 0:
			{
				int nXPos = (rand() % nScreenWidth) - (nRandWidth / 2),
					nYPos = (rand() % nRandY),
					nWidth = (rand() % nRandWidth);
				BitBlt(hdcWindow, nXPos, nYPos, nWidth, nScreenHeight, hdcWindow, nXPos, 0, SRCCOPY);
			}
			//Shift screen
			case 1:
			{
				glitching = true;
			}
			//Faulty pixel
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			{
				int xPos = rand() % nScreenWidth, yPos = rand() % nScreenHeight;
				SetPixel(hdcWindow, xPos, yPos, RGB(rand() % 255, rand() % 255, rand() % 255));
			}
			//Chromatic shift
			case 8:
			{
				int xMin = rand() % nScreenWidth - 1, yMin = rand() % nScreenHeight - 1;
				int xDist = 1 + rand() % nScreenWidth - xMin - 1, yDist = 1 + rand() % nScreenHeight - yMin - 1;
				xDist %= maxXShift;
				yDist %= maxYShift;
				int selection = rand() % 3;
				for (int i = 0; i < xDist; ++i)
				{
					for (int j = 0; j < yDist; ++j)
					{
						COLORREF col = GetPixel(hdcWindow, i + xMin, j + yMin);
						switch (selection)
						{
						case 0:
							col = RGB(GetRValue(col), GetGValue(col), 0);
						case 1:
							col = RGB(GetRValue(col), 0, GetBValue(col));
						case 2:
							col = RGB(0, GetGValue(col), GetBValue(col));
						}
						SetPixel(hdcWindow, i + xMin, j + yMin, col);
					}
				}
			}
			}

			createArtifact = false;
		}

		ReleaseDC(hWnd, hdcWindow);
	}
		return 0;
	case WM_CLOSE:
	case WM_DESTROY:
	{
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
	}
		return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)
{
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	WNDCLASS wndClass = { 0, MelterProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), 0, NULL, L"Melter" };
	if (!RegisterClass(&wndClass)) return MessageBox(HWND_DESKTOP, L"Cannot register class!", NULL, MB_ICONERROR | MB_OK);

	HWND hWnd = CreateWindow(L"Melter", NULL, WS_POPUP, 0, 0, nScreenWidth, nScreenHeight, HWND_DESKTOP, NULL, hInstance, NULL);
	if (!hWnd) return MessageBox(HWND_DESKTOP, L"Cannot create window!", NULL, MB_ICONERROR | MB_OK);

	srand(GetTickCount());
	MSG Msg = { 0 };
	while (Msg.message != WM_QUIT){
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		if (GetAsyncKeyState(VK_ESCAPE) && GetAsyncKeyState(VK_LCONTROL))
			DestroyWindow(hWnd);
	}
	return 0;
}