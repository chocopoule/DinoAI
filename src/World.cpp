#include "stdafx.h"
#include "World.h"
#include "Utils.h"
#include <algorithm>




World::World(Dino& dino) : _dino(dino), _isInverted(false), _lastFrameObstacle(nullptr)
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

	for (int i = dinoPos.x; i <= dinoPos.x + X_RANGE_MAX; i = i +2 )
	{
		COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, i, dinoPos.y + Y_DELTA_FOR_RAY));
		BYTE pixRGBLumi = GetGrayScale(pixel);
		if (IsBlack(pixRGBLumi, _isInverted))
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
			if (IsWhite(pixRGBLumi, _isInverted))
			{
				isFirstPixelDino = false;
			}
		}
	}
}

void World::UpdateDino()
{
	bool isDinoFound = false;

	PointStruct dinoPos = _dino.GetDinoPos();
	COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, dinoPos.x, dinoPos.y));
	BYTE pixRGBLumi = GetGrayScale(pixel);
	if (IsBlack(pixRGBLumi, _isInverted))
	{
		_dino.SetState(GROUND);
		isDinoFound = true;
	}
	else
	{
		pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, dinoPos.x, dinoPos.y + Y_CRAWL_DELTA));
		pixRGBLumi = GetGrayScale(pixel);
		if (IsBlack(pixRGBLumi, _isInverted))
		{
			_dino.SetState(CRAWLING);
			isDinoFound = true;
		}
		else
		{
			for (int i = dinoPos.y; i >= dinoPos.y - Y_RANGE_JUMP_MAX; i = i - 2)
			{
				pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, dinoPos.x, i));
				pixRGBLumi = GetGrayScale(pixel);
				if (IsBlack(pixRGBLumi, _isInverted))
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

	// Dino's speed

	auto obstacle = GetNearestObstacle();
	if (obstacle && _lastFrameObstacle)
	{
		int distanceNew = obstacle->GetDistanceFromDino(_dino);
		int distanceOld = _lastFrameObstacle->GetDistanceFromDino(_dino);
		int distancebetween = distanceOld - distanceNew;
		if (distancebetween > 0 )
		{
			auto timeNew = obstacle->GetCapturedTime();
			auto timeOld = _lastFrameObstacle->GetCapturedTime();
			auto timeBetween = std::chrono::duration_cast<std::chrono::milliseconds>(timeNew - timeOld);

			double speed = (double)distancebetween / (double)timeBetween.count();
			_dino.SetDinoSpeed(speed);
			std::cout << "DINO speed: " << speed << "  distance:" << distancebetween << " time:" << timeBetween.count() << std::endl;
		}
	}


}


void World::UpdateInversion()
{
	COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&_grab, constantPos.x, constantPos.y));
	BYTE pixRGBLumi = GetGrayScale(pixel);
	if (IsBlack(pixRGBLumi, false))
	{
		if (!_isInverted) {
			_isInverted = true;
			std::cout << "INVERT !!!! " << std::endl;
		}
	}
	else
	{
		if (_isInverted) {
			_isInverted = false;
			std::cout << "INVERT !!!! " << std::endl;
		}
	}
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

	UpdateInversion();
	UpdateObstacle();
	UpdateDino();

	_lastFrameObstacle = GetNearestObstacle();
	

	return true;
}

std::shared_ptr<Obstacle> World::GetNearestObstacle()
{
	if (_obstacleVec.size())
		return _obstacleVec[0];
	else
		return nullptr;
}
