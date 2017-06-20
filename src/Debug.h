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
  static void DisplayTargets(const std::vector<PointStruct>& vecList);
};

