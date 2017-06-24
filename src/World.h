#pragma once

#include "Dino.h"
#include "Windows.h" 
#include <vector>
#include "Obstacle.h"
#include <memory>

#define ARGB_TO_COLORREF(a)     (COLORREF)(((a) & 0xFF00FF00) | (((a) & 0xFF0000) >> 16) | (((a) & 0xFF) << 16)) // ARGB to ABGR
#define BitmapPixel(b, x, y)    ((b)->pixels[(y) * (b)->width + (x)]) // pixel is ARGB
#define GetAValue(cr)           (int)((cr) >> 24)
#define ColorNoAlpha(p)         ((p) & 0x00FFFFFF)


class World
{
public:
  World(Dino& dino);
  ~World();

  bool Scan();

  std::shared_ptr<Obstacle> GetNearestObstacle();

  typedef struct _BITMAPCAPTURE {
	  HBITMAP hbm;
	  LPDWORD pixels;
	  INT     width;
	  INT     height;
  } BITMAPCAPTURE;

  

private:
  BOOL CaptureScreen(BITMAPCAPTURE *bmpCapture);
  void UpdateDino();
  void UpdateObstacle();
  void UpdateInversion();
  

private:
  Dino& _dino;
  BITMAPCAPTURE _grab;
  std::vector<std::shared_ptr<Obstacle>> _obstacleVec;
  bool _isInverted;

friend class Debug;

};


