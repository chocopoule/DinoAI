#pragma once
#include <iostream>
#include "Utils.h"

class Dino
{
public:
  Dino(const PointStruct& dinoPos);
  ~Dino();

  enum DinoState { NONE, GROUND, JUMPING, CRAWLING};
  inline DinoState GetState() { return _state; };
  inline void SetState(const DinoState& state) { 
	  switch (state)
	  {
	  case NONE:
		  if(_state != state)
			std::cout << "Dino[State]: NONE" << std::endl;
		  break;
	  case GROUND:
		  if (_state != state)
			std::cout << "Dino[State]: GROUND" << std::endl;
		  break;
	  case JUMPING:
		  if (_state != state)
			std::cout << "Dino[State]: JUMPING" << std::endl;
		  break;
	  case CRAWLING:
		  if (_state != state)
			std::cout << "Dino[State]: CRAWLING" << std::endl;
		  break;
	}
	  _state = state;
  };

  void Jump();
  void Crawl();
  
  inline PointStruct GetDinoPos() const { return _dinoPos; };

private:
  DinoState _state;
  PointStruct _dinoPos;
};

