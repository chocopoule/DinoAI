#include "stdafx.h"
#include "World.h"
#include "Utils.h"


#define ARGB_TO_COLORREF(a)     (COLORREF)(((a) & 0xFF00FF00) | (((a) & 0xFF0000) >> 16) | (((a) & 0xFF) << 16)) // ARGB to ABGR
#define BitmapPixel(b, x, y)    ((b)->pixels[(y) * (b)->width + (x)]) // pixel is ARGB
#define GetAValue(cr)           (int)((cr) >> 24)
#define ColorNoAlpha(p)         ((p) & 0x00FFFFFF)


World::World(const Dino& dino) : _dino(dino)
{
  initKeyboard();
}


World::~World()
{
}





BOOL World::CaptureScreen(BITMAPCAPTURE *bmpCapture)
{
  BOOL bResult = FALSE;
  if (!bmpCapture)
    return bResult;

  ZeroMemory(bmpCapture, sizeof(BITMAPCAPTURE));

  HDC hdcScreen = GetDC(NULL);
  HDC hdcCapture = CreateCompatibleDC(NULL);
  int nWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN),
    nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

  // Bitmap is stored top down as BGRA,BGRA,BGRA when used as
  // DWORDs endianess would change it to ARGB.. windows COLORREF is ABGR
  LPBYTE lpCapture;
  BITMAPINFO bmiCapture = { {
      sizeof(BITMAPINFOHEADER), nWidth, -nHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0,
    } };

  bmpCapture->hbm = CreateDIBSection(hdcScreen, &bmiCapture,
    DIB_RGB_COLORS, (LPVOID *)&lpCapture, NULL, 0);
  if (bmpCapture->hbm) {
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcCapture, bmpCapture->hbm);
    BitBlt(hdcCapture, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);
    SelectObject(hdcCapture, hbmOld);
    bmpCapture->pixels = (LPDWORD)lpCapture;
    bmpCapture->width = nWidth;
    bmpCapture->height = nHeight;
    bResult = TRUE;
  }

  DeleteDC(hdcCapture);
  DeleteDC(hdcScreen);
  return bResult;
}


void World::Scan()
{

}

Obstacle& World::GetNearestObstacle()
{
	return _obstacleVec[0];
}