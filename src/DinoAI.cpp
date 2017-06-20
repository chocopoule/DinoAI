//// DinoAI.cpp : Defines the entry point for the console application.
////
//
#include "stdafx.h"
#include <iostream>
#include "windows.h" 
#include <chrono>
#include <thread>
#include <stdio.h>

#define ARGB_TO_COLORREF(a)     (COLORREF)(((a) & 0xFF00FF00) | (((a) & 0xFF0000) >> 16) | (((a) & 0xFF) << 16)) // ARGB to ABGR
#define BitmapPixel(b, x, y)    ((b)->pixels[(y) * (b)->width + (x)]) // pixel is ARGB
#define GetAValue(cr)           (int)((cr) >> 24)
#define ColorNoAlpha(p)         ((p) & 0x00FFFFFF)

typedef struct _BITMAPCAPTURE {
  HBITMAP hbm;
  LPDWORD pixels;
  INT     width;
  INT     height;
} BITMAPCAPTURE;

BOOL CaptureScreen(BITMAPCAPTURE *bmpCapture) // by Napalm
{
  BOOL bResult = FALSE;
  if (!bmpCapture)
    return bResult;

  ZeroMemory(bmpCapture, sizeof(BITMAPCAPTURE));

  HDC hdcScreen = GetDC(NULL);
  HDC hdcCapture = CreateCompatibleDC(NULL);
  int nWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN),
    nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

  // Bitmap is stored top down as BGRA,BGRA,BGRA when used as
  // DWORDs endianess would change it to ARGB.. windows COLORREF is ABGR
  LPBYTE lpCapture;
  BITMAPINFO bmiCapture = { {
      sizeof(BITMAPINFOHEADER), nWidth, -nHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0,
    } };

  bmpCapture->hbm = CreateDIBSection(hdcScreen, &bmiCapture,
    DIB_RGB_COLORS, (LPVOID *)&lpCapture, NULL, 0);
  if (bmpCapture->hbm) {
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcCapture, bmpCapture->hbm);
    BitBlt(hdcCapture, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);
    SelectObject(hdcCapture, hbmOld);
    bmpCapture->pixels = (LPDWORD)lpCapture;
    bmpCapture->width = nWidth;
    bmpCapture->height = nHeight;
    bResult = TRUE;
  }

  DeleteDC(hdcCapture);
  DeleteDC(hdcScreen);
  return bResult;
}


//
//
static INPUT inputKeybd; // keyboard

void initKeyboard() {
  // Set up a generic keyboard event.
  inputKeybd.type = INPUT_KEYBOARD;
  inputKeybd.ki.wScan = 0; // hardware scan code for key
  inputKeybd.ki.time = 0;
  inputKeybd.ki.dwExtraInfo = 0;
}

void Jump() {
  std::cout << "JUMP" << std::endl;
  inputKeybd.ki.wVk = 0x26; // virtual-key code for the "up" key
  inputKeybd.ki.dwFlags = 0; // 0 for key press
  SendInput(1, &inputKeybd, sizeof(INPUT));

  inputKeybd.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &inputKeybd, sizeof(INPUT));
}


void Crawl() {
  std::cout << "CRAWL" << std::endl;
  inputKeybd.ki.wVk = 0x28; // virtual-key code for the "up" key
  inputKeybd.ki.dwFlags = 0; // 0 for key press
  SendInput(1, &inputKeybd, sizeof(INPUT));

  inputKeybd.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &inputKeybd, sizeof(INPUT));
}


void PrintCursorPos()
{
  POINT _cursor;
  if (GetCursorPos(&_cursor)) {
    std::cout << "x: " << _cursor.x << "  y: " << _cursor.y << std::endl;
  }
}

enum DinoState { GROUND, JUMPING, CRAWLING, NONE };
struct PointStruct { int x; int y; };


const static COLORREF BLACK_COLOR = 4283650899; // 535353
const static COLORREF BLACK_COLOR2 = 4294111986; // 535353
const static COLORREF WHITE_COLOR = 4294440951; // F7F7F7
const static COLORREF WHITE_COLOR2 = 4283716692; // F7F7F7

const static PointStruct piafTarget = { 330,218 };
const static PointStruct cactusTarget = { 330,239 };
const static PointStruct dinoTarget = { 220,245 };
const static int cactusXRange = 20;


void DisplayTargets(const PointStruct& pointCenter1, const PointStruct& pointCenter2)
{
  auto t1 = std::thread([pointCenter1, pointCenter2]() {
    static const int REC_SIZE = 8;
    while (true) {
      HDC hDC2 = GetDC(NULL);
      SelectObject(hDC2, CreatePen(PS_SOLID, 5, RGB(255, 0, 0)));
      Rectangle(hDC2, pointCenter1.x + REC_SIZE, pointCenter1.y + REC_SIZE, pointCenter1.x - REC_SIZE, pointCenter1.y - REC_SIZE);
      Rectangle(hDC2, pointCenter2.x + REC_SIZE, pointCenter2.y + REC_SIZE, pointCenter2.x - REC_SIZE, pointCenter2.y - REC_SIZE);
      DeleteDC(hDC2);
    }

  });
  t1.detach();
}


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
  DinoState dinoState = NONE;

  int crawlTimer;

  // Create threads to Display targets
  //DisplayTargets(cactusTarget, dinoTarget);

  while (true) {
    auto t1 = std::chrono::high_resolution_clock::now();
    CaptureScreen(&grab);

    //PrintCursorPos();

    if (dinoState == CRAWLING)
    {
      crawlTimer--;
      if (crawlTimer < 0)
      {
        dinoState = GROUND;
      }
    }
    else
    {
      dinoState = GetDinoState(&grab, dinoTarget);

      if (dinoState == GROUND)
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
      if (dinoState == GROUND)
      {
        for (int i = piafTarget.x - cactusXRange; i <= piafTarget.x + cactusXRange; i++)
        {
          COLORREF pixel = ARGB_TO_COLORREF(BitmapPixel(&grab, i, piafTarget.y));
          if (pixel == BLACK_COLOR || pixel == BLACK_COLOR2) {
            Crawl();
            crawlTimer = 15;
            dinoState = CRAWLING;
            break;
          }
        }
      }
    }



    DeleteObject(grab.hbm);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    //std::cout << "-- perfo -- " << elapsed.count() << " ms" << '\n';
  }
}


//
//
//void CaptureScreen(BYTE*& screenCapture, int& MAX_WIDTH, int& MAX_HEIGHT)
//{
//	HDC hdc = GetDC(HWND_DESKTOP);
//	RECT rect;
//	GetWindowRect(GetDesktopWindow(), &rect);
//	int MAX_WIDTH = rect.right;
//	int MAX_HEIGHT = rect.bottom;
//
//	HDC hdcTemp = CreateCompatibleDC(hdc);
//	BITMAPINFO bitmap;
//	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
//	bitmap.bmiHeader.biWidth = MAX_WIDTH;
//	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
//	bitmap.bmiHeader.biPlanes = 1;
//	bitmap.bmiHeader.biBitCount = 24;
//	bitmap.bmiHeader.biCompression = BI_RGB;
//	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 3 * MAX_HEIGHT;
//	bitmap.bmiHeader.biClrUsed = 0;
//	bitmap.bmiHeader.biClrImportant = 0;
//	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&screenCapture), NULL, NULL);
//	SelectObject(hdcTemp, hBitmap2);
//	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, 0, 0, SRCCOPY);
//}
//
//
//int main()
//{
//	initKeyboard();
//	DinoState dinoState = NONE;
//
//	//HDC hDC = GetDC(NULL);
//
//	// set default brush from displaying rectangle
//	//SelectObject(hDC, CreatePen(PS_SOLID, 5, RGB(255, 0, 0)));
//	// Create threads to Display targets
//	DisplayTargets(cactusTarget, dinoTarget);
//
//
//	while (true) {
//		auto t1 = std::chrono::high_resolution_clock::now();
//
//		BYTE* screenCapture = nullptr;
//		int MAX_WIDTH;
//		int MAX_HEIGHT;
//		CaptureScreen(screenCapture, MAX_WIDTH, MAX_HEIGHT);
//
//		//PrintCursorPos();
//		dinoState = GetDinoState(screenCapture, dinoTarget);
//
//		if (dinoState == GROUND)
//		{
//			for (int i = cactusTarget.x - cactusXRange; i <= cactusTarget.x + cactusXRange; i++)
//			{
//				COLORREF  pixel = GetPixel(hDC, i, cactusTarget.y);
//				if (pixel == BLACK_COLOR) {
//					Jump();
//					break;
//				}
//			}
//		}
//
//		auto t2 = std::chrono::high_resolution_clock::now();
//		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
//		//std::cout << "-- perfo -- " << elapsed.count() << " ms"<< '\n';
//	}
//
//	return 0;
//}
//
//
////
////int main() {
////
////}
////
////
////int main() {
////	HDC hdc, hdcTemp;
////	RECT rect;
////	BYTE* bitPointer;
////	int x, y;
////	int red, green, blue, alpha;
////
////	while (true)
////	{
////		auto t1 = std::chrono::high_resolution_clock::now();
////		hdc = GetDC(HWND_DESKTOP);
////		GetWindowRect(GetDesktopWindow(), &rect);
////		int MAX_WIDTH = rect.right;
////		int MAX_HEIGHT = rect.bottom;
////
////		hdcTemp = CreateCompatibleDC(hdc);
////		BITMAPINFO bitmap;
////		bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
////		bitmap.bmiHeader.biWidth = MAX_WIDTH;
////		bitmap.bmiHeader.biHeight = MAX_HEIGHT;
////		bitmap.bmiHeader.binputKeybdlanes = 1;
////		bitmap.bmiHeader.biBitCount = 32;
////		bitmap.bmiHeader.biCompression = BI_RGB;
////		bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
////		bitmap.bmiHeader.biClrUsed = 0;
////		bitmap.bmiHeader.biClrImportant = 0;
////		HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
////		SelectObject(hdcTemp, hBitmap2);
////		BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, 0, 0, SRCCOPY);
////
////		for (int i = 0; i<(MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
////		{
////			/*red = (int)bitPointer[i];
////			green = (int)bitPointer[i + 1];
////			blue = (int)bitPointer[i + 2];
////			alpha = (int)bitPointer[i + 3];
////
////			x = i / (4 * MAX_HEIGHT);
////			y = i / (4 * MAX_WIDTH);
////
////			if (red == 255 && green == 0 && blue == 0)
////			{
////				SetCursorPos(x, y);
////				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
////				Sleep(50);
////				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
////				Sleep(25);
////			}*/
////		}
////
////
////		auto t2 = std::chrono::high_resolution_clock::now();
////		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
////		std::cout << "-- perfo -- " << elapsed.count() << " ms"<< '\n';
////	}
////}



