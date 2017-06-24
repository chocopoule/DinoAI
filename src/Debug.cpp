#include "stdafx.h"
#include "Debug.h"
#include "Windows.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include "World.h"




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

void Debug::PrintCursorPosAndColor(World& world)
{
  POINT _cursor;
  if (GetCursorPos(&_cursor)) {
	World::BITMAPCAPTURE grab;
	BOOL bCapture = world.CaptureScreen(&grab);
	COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&grab, _cursor.x, _cursor.y));
	BYTE pixRGBLumi = GetGrayScale(pixel);
	std::cout << "x: " << _cursor.x << "  y: " << _cursor.y << "  color: "<< (int)pixRGBLumi << std::endl;
  }
}

void Debug::DisplayRectangles(const std::vector<PointStruct>& vecList, int rColor, int gColor, int bColor)
{
  auto t1 = std::thread([vecList, rColor, gColor, bColor]() {
    static const int REC_SIZE = 8;
    while (true) {
      HDC hDC2 = GetDC(NULL);
      //SelectObject(hDC2, CreatePen(PS_SOLID, 5, RGB(rColor, gColor, bColor)));
      auto printRectangle = [hDC2](const PointStruct& point) {
		SelectObject(hDC2, GetStockObject(NULL_BRUSH));
        Rectangle(hDC2, point.x + REC_SIZE, point.y + REC_SIZE, point.x - REC_SIZE, point.y - REC_SIZE);
      };
      std::for_each(vecList.begin(), vecList.end(), printRectangle);

      Sleep(20);

      DeleteDC(hDC2);
    }
  });
  t1.detach();
}

void Debug::DisplayLine(std::vector<std::pair<PointStruct, PointStruct>>& lineList, int rColor, int gColor, int bColor)
{
  auto t1 = std::thread([lineList, rColor, gColor, bColor]() {
    while (true) {
      HDC hDC2 = GetDC(NULL);
     

      HPEN hPenOld;

      // Draw a red line
      HPEN hLinePen;
      COLORREF qLineColor;
      qLineColor = RGB(255, 0, 0);
      hLinePen = CreatePen(PS_SOLID, 7, qLineColor);
      hPenOld = (HPEN)SelectObject(hDC2, hLinePen);

      auto printLine = [hDC2](const std::pair<PointStruct, PointStruct>& point) {
        MoveToEx(hDC2, point.first.x, point.first.y, NULL);
        LineTo(hDC2, point.second.x , point.second.y);
      };
      std::for_each(lineList.begin(), lineList.end(), printLine);

      SelectObject(hDC2, hPenOld);
      DeleteObject(hLinePen);


      Sleep(20);

      DeleteDC(hDC2);
    }
  });
  t1.detach();
}
