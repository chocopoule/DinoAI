#include "stdafx.h"
#include "World.h"
#include "Dino.h"
#include "Obstacle.h"
#include "Utils.h"
#include "Debug.h"


static enum Order {None, Jump, Crawl};

void main()
{
  Sleep(1000);
  Dino dino(dinoHeadPos);
  World world(dino);

  //Debug::DisplayRectangles({ dinoHeadPos }, 255,0,0);
  //Debug::DisplayRectangles({ PointStruct{ dinoHeadPos.x, dinoHeadPos.y + Y_CRAWL_DELTA } }, 0, 255, 0);

  //std::pair<PointStruct, PointStruct> rayForJump{ dinoHeadPos, PointStruct{ dinoHeadPos.x, dinoHeadPos.y - Y_RANGE_JUMP_MAX } };
  //std::vector< std::pair<PointStruct, PointStruct> > vec{ rayForJump };
  //Debug::DisplayLine(vec, 0, 0, 255);
  //
  //std::pair<PointStruct, PointStruct> rayForObstacle{ PointStruct{ dinoHeadPos.x, dinoHeadPos.y + Y_DELTA_FOR_RAY }, PointStruct{ dinoHeadPos.x + X_RANGE_MAX, dinoHeadPos.y + Y_DELTA_FOR_RAY } };
  //std::vector< std::pair<PointStruct, PointStruct> > vec2{ rayForObstacle };
  //Debug::DisplayLine(vec2, 0, 255, 255);

  Order order = None;

  while (true) {
    //auto t1 = Debug::StartChrono();
    bool bScan = world.Scan();

    //Debug::PrintCursorPosAndColor(world);


    DinoState dinoState = dino.GetState();

    if (order == Jump && dinoState != JUMPING) {
      dino.Jump();
    }
    else if (order == Crawl && dinoState != CRAWLING) {
      dino.Crawl();
    }
    else
    {
      order = None;
      if (dinoState != JUMPING)
      {
        if (dinoState != CRAWLING)
        {
          dino.Crawl();
          order = Crawl;
        }
        else
        {
          auto obstacle = world.GetNearestObstacle();
          if (obstacle)
          {
            int distanceFromDino = obstacle->GetDistanceFromDino(dino);
            //std::cout << "Prepare for impact in " << distanceFromDino << std::endl;
            if (distanceFromDino < maxDistanceBeforeJump)
            {
              dino.Jump();
              order = Jump;
            }
          }
        }
      }
    }


   
    //Debug::StopChrono(t1);
  }
}
