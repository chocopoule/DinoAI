#pragma once
#include <iostream>
#include "Utils.h"

enum DinoState { NONE = 0, GROUND = 1, JUMPING = 2, CRAWLING = 3 };

class Dino
{
public:
	Dino(const PointStruct& dinoPos);
	~Dino();
	

	DinoState GetState();
	void SetState(DinoState state);

	void Jump();
	void Crawl();

	inline PointStruct GetDinoPos() const  { return _dinoPos; };

	inline void SetDinoSpeed(double speed) { _speed = speed; };
	inline double GetDinoSpeed() const { return _speed; };

private:
	DinoState _state;
	PointStruct _dinoPos;
	double _speed;
};

