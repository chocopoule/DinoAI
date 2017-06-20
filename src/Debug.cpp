#include "stdafx.h"
#include "Debug.h"
#include "Windows.h"
#include <iostream>
#include <thread>
#include <algorithm>



std::chrono::time_point<std::chrono::high_resolution_clock> Debug::StartChrono() {
  auto t1 = std::chrono::high_resolution_clock::now();
  return t1;
}

void Debug::StopChrono(const std::chrono::time_point<std::chrono::high_resolution_clock>& t1)
{
  auto t2 = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
  std::cout << "-- perfo -- " << elapsed.count() << " ms" << '\n';
}

void Debug::PrintCursorPos()
{
  POINT _cursor;
  if (GetCursorPos(&_cursor)) {
    std::cout << "x: " << _cursor.x << "  y: " << _cursor.y << std::endl;
  }
}

void DisplayTargets(const std::vector<PointStruct>& vecList)
{
  auto t1 = std::thread([vecList]() {
    static const int REC_SIZE = 8;
    while (true) {
      HDC hDC2 = GetDC(NULL);
      SelectObject(hDC2, CreatePen(PS_SOLID, 5, RGB(255, 0, 0)));
      auto printRectangle = [hDC2](const PointStruct& point) {
        Rectangle(hDC2, point.x + REC_SIZE, point.y + REC_SIZE, point.x - REC_SIZE, point.y - REC_SIZE);
      };
      std::for_each(vecList.begin(), vecList.end(), printRectangle);

      DeleteDC(hDC2);
    }
  });
  t1.detach();
}
