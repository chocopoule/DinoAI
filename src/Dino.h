#pragma once

struct PointStruct;

class Dino
{
public:
  Dino(const PointStruct& dinoPos);
  ~Dino();

  enum DinoState { NONE, GROUND, JUMPING, CRAWLING};
  inline DinoState GetState() { return _state; };

private:
  DinoState _state;
};

