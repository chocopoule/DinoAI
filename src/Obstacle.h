#pragma once
#include "Utils.h"
#include <chrono>

class Dino;

class Obstacle
{
public:
  Obstacle(const PointStruct& pos);
  ~Obstacle();

  int GetDistanceFromDino(const Dino& dino);
  inline std::chrono::time_point<std::chrono::high_resolution_clock> GetCapturedTime() {return _time;}

private:
	PointStruct _pos;
	std::chrono::time_point<std::chrono::high_resolution_clock> _time;
};

