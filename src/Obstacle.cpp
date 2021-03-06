#include "stdafx.h"
#include "Obstacle.h"
#include "Dino.h"

Obstacle::Obstacle(const PointStruct& pos):_pos(pos)
{
	_time  = std::chrono::high_resolution_clock::now();
}

Obstacle::~Obstacle()
{
}

int Obstacle::GetDistanceFromDino(const Dino& dino)
{
	PointStruct dinoPos = dino.GetDinoPos();
	return (_pos.x - dinoPos.x);
}