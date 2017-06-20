#include "Windows.h"
#pragma once


struct PointStruct { int x; int y; };

//static INPUT inputKeybd; // keyboard

const static COLORREF BLACK_COLOR = 4283650899; // 535353
const static COLORREF BLACK_COLOR2 = 4294111986; // 535353
const static COLORREF WHITE_COLOR = 4294440951; // F7F7F7
const static COLORREF WHITE_COLOR2 = 4283716692; // F7F7F7

const static int X_RANGE_MAX = 400;
const static int Y_DELTA_FOR_RAY = 20;
const static int Y_RANGE_JUMP_MAX = 200;
const static int Y_CRAWL_DELTA = 20;
//
//static void initKeyboard() {
//  // Set up a generic keyboard event.
//  inputKeybd.type = INPUT_KEYBOARD;
//  inputKeybd.ki.wScan = 0; // hardware scan code for key
//  inputKeybd.ki.time = 0;
//  inputKeybd.ki.dwExtraInfo = 0;
//}

static void PressUpKey() {
	INPUT inputKeybd;

	inputKeybd.type = INPUT_KEYBOARD;
	inputKeybd.ki.wScan = 0; // hardware scan code for key
	inputKeybd.ki.time = 0;
	inputKeybd.ki.dwExtraInfo = 0;

  inputKeybd.ki.wVk = 0x26; // virtual-key code for the "up" key
  inputKeybd.ki.dwFlags = 0; // 0 for key press
  SendInput(1, &inputKeybd, sizeof(INPUT));

  inputKeybd.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &inputKeybd, sizeof(INPUT));
}


static void HoldDownKey() {
	INPUT inputKeybd;

	inputKeybd.type = INPUT_KEYBOARD;
	inputKeybd.ki.wScan = 0; // hardware scan code for key
	inputKeybd.ki.time = 0;
	inputKeybd.ki.dwExtraInfo = 0;
  inputKeybd.ki.wVk = 0x28; // virtual-key code for the "down" key
  inputKeybd.ki.dwFlags = 0; // 0 for key press
  SendInput(1, &inputKeybd, sizeof(INPUT));
}


static void ReleaseDownKey() {
	INPUT inputKeybd;

	inputKeybd.type = INPUT_KEYBOARD;
	inputKeybd.ki.wScan = 0; // hardware scan code for key
	inputKeybd.ki.time = 0;
	inputKeybd.ki.dwExtraInfo = 0;
  inputKeybd.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &inputKeybd, sizeof(INPUT));
}
