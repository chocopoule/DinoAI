#pragma once
#include <iostream>
#include "Utils.h"

class Dino
{
public:
	Dino(const PointStruct& dinoPos);
	~Dino();

	enum DinoState { NONE = 0, GROUND = 1, JUMPING = 2, CRAWLING = 3 };
	inline DinoState GetState() { return _state; };
	void SetState(DinoState state) {
		_state = state;
		switch (state)
		{
		case NONE:
			//if (_state != state)
				std::cout << "Dino[State]: NONE" << std::endl;
			break;
		case GROUND:
			//if (_state != state)
				std::cout << "Dino[State]: GROUND" << std::endl;
			break;
		case JUMPING:
			//if (_state != state)
				std::cout << "Dino[State]: JUMPING" << std::endl;
			break;
		case CRAWLING:
			//if (_state != state)
				std::cout << "Dino[State]: CRAWLING" << std::endl;
			break;
		}
		std::cout << "_state " << _state << std::endl;
		std::cout << "state " << state << std::endl;

	};

	void Jump();
	void Crawl();

	inline PointStruct GetDinoPos() const { return _dinoPos; };

private:
	DinoState _state;
	PointStruct _dinoPos;
};

