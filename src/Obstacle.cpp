#include "stdafx.h"
#include "Obstacle.h"
#include "Dino.h"

Obstacle::Obstacle(const PointStruct& pos):_pos(pos)
{
}

Obstacle::~Obstacle()
{
}

int Obstacle::GetDistanceFromDino(const Dino& dino)
{
	PointStruct dinoPos = dino.GetDinoPos();
	return (dinoPos.x - _pos.x);
}