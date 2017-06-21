#include "stdafx.h"
#include "World.h"
#include "Utils.h"


#define ARGB_TO_COLORREF(a)     (COLORREF)(((a) & 0xFF00FF00) | (((a) & 0xFF0000) >> 16) | (((a) & 0xFF) << 16)) // ARGB to ABGR
#define BitmapPixel(b, x, y)    ((b)->pixels[(y) * (b)->width + (x)]) // pixel is ARGB
#define GetAValue(cr)           (int)((cr) >> 24)
#define ColorNoAlpha(p)         ((p) & 0x00FFFFFF)


World::World(Dino& dino) : _dino(dino)
{
}


World::~World()
{
	if (_grab.hbm)
		DeleteObject(_grab.hbm);
}


BOOL World::CaptureScreen(BITMAPCAPTURE *bmpCapture) // from napalm
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


void World::UpdateObstacle() 
{
	PointStruct dinoPos = _dino.GetDinoPos();
	
	bool isFirstPixelDino = true;

	for (int i = dinoPos.x; i <= dinoPos.x + X_RANGE_MAX; i = i + 2)
	{
		COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, i, dinoPos.y + Y_DELTA_FOR_RAY));

		if (pixel == BLACK_COLOR || pixel == BLACK_COLOR2)
		{
			if (!isFirstPixelDino)
			{
				//std::cout << "dino " << dinoPos.x << " " << dinoPos.y << std::endl;
				//std::cout << "obstacle " << i << " " << dinoPos.y << std::endl;
				PointStruct pt = { i, dinoPos.y };
				std::shared_ptr<Obstacle> obstacle(new Obstacle(pt));
				_obstacleVec.push_back(obstacle);
				break;
			}
		}
		else
		{
			if (pixel == WHITE_COLOR || pixel == WHITE_COLOR2)
			{
				isFirstPixelDino = false;
			}
			else
			{
				std::cout << pixel << std::endl;
				
			}
			
			
		}
	}
}

void World::UpdateDino()
{
	bool isDinoFound = false;

	PointStruct dinoPos = _dino.GetDinoPos();
	COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, dinoPos.x, dinoPos.y));
	if (pixel == BLACK_COLOR || pixel == BLACK_COLOR2)
	{
		_dino.SetState(GROUND);
		isDinoFound = true;
	}
	else
	{
		COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, dinoPos.x, dinoPos.y + Y_CRAWL_DELTA));
		if (pixel == BLACK_COLOR || pixel == BLACK_COLOR2)
		{
			_dino.SetState(CRAWLING);
			isDinoFound = true;
		}
		else
		{
			for (int i = dinoPos.y; i >= dinoPos.y - Y_RANGE_JUMP_MAX; i = i - 2)
			{
				COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, dinoPos.x, i));
				if (pixel == BLACK_COLOR || pixel == BLACK_COLOR2)
				{
					_dino.SetState(JUMPING);
					isDinoFound = true;
					break;
				}
			}
		}
	}

	if (!isDinoFound)
		std::cout << "DINO IS LOST ! " << std::endl;
}


bool World::Scan()
{
	if(_grab.hbm)
		DeleteObject(_grab.hbm);

	_obstacleVec.clear();

	BOOL bCapture = CaptureScreen(&_grab);

	if (bCapture == FALSE || !_grab.hbm)
	{
		std::cout << "Capture error" << std::endl;
		return false;
	}

	UpdateDino();
	UpdateObstacle();

	return true;
}

std::shared_ptr<Obstacle> World::GetNearestObstacle()
{
	if (_obstacleVec.size())
		return _obstacleVec[0];
	else
		return nullptr;
}
