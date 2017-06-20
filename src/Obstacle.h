#pragma once
#include "Utils.h"

class Dino;

class Obstacle
{
public:
  Obstacle(const PointStruct& pos);
  ~Obstacle();

  int GetDistanceFromDino(const Dino& dino);

private:
	PointStruct _pos;
};

