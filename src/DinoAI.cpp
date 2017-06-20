//// DinoAI.cpp : Defines the entry point for the console application.
////
//
#include "stdafx.h"
#include <iostream>
#include "windows.h" 
#include <chrono>
#include <thread>
#include <stdio.h>




const static COLORREF BLACK_COLOR = 4283650899; // 535353
const static COLORREF BLACK_COLOR2 = 4294111986; // 535353
const static COLORREF WHITE_COLOR = 4294440951; // F7F7F7
const static COLORREF WHITE_COLOR2 = 4283716692; // F7F7F7

const static PointStruct cactusTarget = { -1100,207 };
const static PointStruct dinoTarget = { -1243,227 };
const static int cactusXRange = 20;






DinoState GetDinoState(BITMAPCAPTURE* grab, const PointStruct& dinoTarget)
{
  static DinoState lastDinoState;
  DinoState dinoState = NONE;

  COLORREF pixelDino = ARGB_TO_COLORREF(BitmapPixel(grab, dinoTarget.x, dinoTarget.y));


  if (pixelDino == WHITE_COLOR || pixelDino == WHITE_COLOR2)
  {
    dinoState = JUMPING;
    if (dinoState != lastDinoState)
      std::cout << "DINO[state] = JUMP" << std::endl;
  }
  else if (pixelDino == BLACK_COLOR || pixelDino == BLACK_COLOR2)
  {
    dinoState = GROUND;
    if (dinoState != lastDinoState)
      std::cout << "DINO[state] = GROUND" << std::endl;
  }
  else
  {
    if (dinoState != lastDinoState)
      std::cout << "DINO[state] = ERROR ---  color:  " << pixelDino << std::endl;
  }

  lastDinoState = dinoState;

  return dinoState;
}

void main() {
  initKeyboard();

  BITMAPCAPTURE grab;
  DinoState dinoState = GROUND;

  while (true) {
    auto t1 = std::chrono::high_resolution_clock::now();
    CaptureScreen(&grab);

    //PrintCursorPos();

    dinoState = GetDinoState(&grab, dinoTarget);

    if (dinoState != JUMPING)
    {
      for (int i = cactusTarget.x - cactusXRange; i <= cactusTarget.x + cactusXRange; i++)
      {
        COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&grab, i, cactusTarget.y));
        if (pixel == BLACK_COLOR || pixel == BLACK_COLOR2) {
          CrawlUp();
          Jump();
          dinoState = JUMPING;
          break;
        }
      }
    }

    if (dinoState != JUMPING && dinoState != GROUND)
    {
      CrawlDown();
      dinoState = CRAWLING;
    }

    DeleteObject(grab.hbm);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    //std::cout << "-- perfo -- " << elapsed.count() << " ms" << '\n';
    DisplayTargets(cactusTarget, dinoTarget);
  }
}


void main2() {
  initKeyboard();

  BITMAPCAPTURE grab;
  DinoState dinoState = CRAWLING;

  int crawlTimer;

  // Create threads to Display targets
  //DisplayTargets(cactusTarget, dinoTarget);

  while (true) {
    auto t1 = std::chrono::high_resolution_clock::now();
    //CaptureScreen(&grab);

    PrintCursorPos();


    dinoState = GetDinoState(&grab, dinoTarget);

    if (dinoState != JUMPING)
    {
      for (int i = cactusTarget.x - cactusXRange; i <= cactusTarget.x + cactusXRange; i++)
      {
        COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&grab, i, cactusTarget.y));
        if (pixel == BLACK_COLOR || pixel == BLACK_COLOR2) {
          Jump();
          dinoState = JUMPING;
          break;
        }
      }
    }
    
    if (dinoState != JUMPING)
    {
      Crawl();
      dinoState = CRAWLING;
    }

    DeleteObject(grab.hbm);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    //std::cout << "-- perfo -- " << elapsed.count() << " ms" << '\n';
  }
}

