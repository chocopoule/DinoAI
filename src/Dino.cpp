#include "stdafx.h"
#include "Dino.h"
#include "Utils.h"
#include <iostream>


Dino::Dino(const PointStruct& dinoPos) :_dinoPos(dinoPos), _state(NONE)
{
}


Dino::~Dino()
{
}


void Dino::Jump()
{
  std::cout << "JUMP" << std::endl;
  ReleaseDownKey();
  PressUpKey();
}

DinoState Dino::GetState()
{
  return _state;
}

void Dino::Crawl()
{
  std::cout << "CRAWL" << std::endl;
  HoldDownKey();
}

void Dino::SetState(DinoState state) {

  switch (state)
  {
  case NONE:
    if (_state != state)
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

