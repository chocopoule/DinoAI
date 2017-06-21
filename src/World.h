#pragma once

#include "Dino.h"
#include "Windows.h" 
#include <vector>
#include "Obstacle.h"
#include <memory>

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

private:
  Dino& _dino;
  BITMAPCAPTURE _grab;
  std::vector<std::shared_ptr<Obstacle>> _obstacleVec;
};


