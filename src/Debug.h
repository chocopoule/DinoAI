#pragma once

#include <chrono>
#include <vector>
#include "Utils.h"

class Debug
{
public:
  static std::chrono::time_point<std::chrono::high_resolution_clock> StartChrono();
  static void StopChrono(const std::chrono::time_point<std::chrono::high_resolution_clock>& t1);
  static void PrintCursorPos();
  static void DisplayRectangles(const std::vector<PointStruct>& vecList, int rColor, int gColor, int bColor);
  static void DisplayLine(std::vector<std::pair<PointStruct, PointStruct>>& lineList, int rColor, int gColor, int bColor);
};

