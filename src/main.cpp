#include "stdafx.h"
#include "World.h"
#include "Dino.h"
#include "Obstacle.h"
#include "Utils.h"
#include "Debug.h"

void main() 
{
  const int maxDistanceBeforeJump = 30;
  PointStruct dinoHeadPos = { -1243, 227 }; // point on dino's body
  Dino dino(dinoHeadPos);
  World world(dino);

  while (true) {
    //auto t1 = Debug::StartChrono();
    world.Scan();

    //Debug::PrintCursorPos();

    if (dino.GetState() != Dino::JUMPING)
    {
      if (dino.GetState() != Dino::CRAWLING)
      {
        dino.Crawl();
      }
      else
      {
        Obstacle obstacle = world.GetNearestObstacle();
        int distanceFromDino = obstacle.GetDistanceFromDino();
        if (distanceFromDino < maxDistanceBeforeJump)
          dino.Jump();
      }
    }
    
    //Debug::DisplayTargets(cactusTarget, dinoTarget);
    //Debug::StopChrono(t1);

  }
}
