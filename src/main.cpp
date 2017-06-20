#include "stdafx.h"
#include "World.h"
#include "Dino.h"
#include "Obstacle.h"
#include "Utils.h"
#include "Debug.h"

void main() 
{
  const int maxDistanceBeforeJump = 30;
  PointStruct dinoHeadPos = {235, 220 }; // point on dino's body
  Dino dino(dinoHeadPos);
  World world(dino);

  Debug::DisplayTargets({ dinoHeadPos });

  while (true) {
    //auto t1 = Debug::StartChrono();
    bool bScan = world.Scan();

    //Debug::PrintCursorPos();

	std::shared_ptr<Obstacle> obstacle = world.GetNearestObstacle();
	if (obstacle)
	{
		int distanceFromDino = obstacle->GetDistanceFromDino(dino);
		//std::cout << "Distance : " << distanceFromDino << std::endl;
	}
	

    if (dino.GetState() != Dino::JUMPING)
    {
      if (dino.GetState() != Dino::CRAWLING)
      {
        //dino.Crawl();
      }
      else
      {
        std::shared_ptr<Obstacle> obstacle = world.GetNearestObstacle();
        int distanceFromDino = obstacle->GetDistanceFromDino(dino);
        //if (distanceFromDino < maxDistanceBeforeJump)
        //  dino.Jump();
      }
    }
	
    //Debug::StopChrono(t1);
  }
}
