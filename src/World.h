#pragma once

#include "Dino.h"
class BITMAPCAPTURE;

class World
{
public:
  World(const Dino& dino);
  ~World();

  void Scan();

private:
  BOOL CaptureScreen(BITMAPCAPTURE *bmpCapture);

private:
  Dino _dino;
};


