#pragma once

#include "Dino.h"
#include "Windows.h" 
#include <vector>
#include "Obstacle.h"

class World
{
public:
  World(const Dino& dino);
  ~World();

  void Scan();

  Obstacle& GetNearestObstacle();

  typedef struct _BITMAPCAPTURE {
	  HBITMAP hbm;
	  LPDWORD pixels;
	  INT     width;
	  INT     height;
  } BITMAPCAPTURE;

private:
  BOOL CaptureScreen(BITMAPCAPTURE *bmpCapture);

private:
  Dino _dino;
  std::vector<Obstacle> _obstacleVec;
};


