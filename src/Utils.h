#include "Windows.h"
#pragma once


struct PointStruct { int x; int y; };


const static COLORREF BLACK_COLOR = 4283650899; // 535353
const static COLORREF BLACK_COLOR2 = 4283716692; // 535353
const static COLORREF WHITE_COLOR = 4294440951; // F7F7F7
const static COLORREF WHITE_COLOR2 = 4294375158; // F7F7F7
//4294111986
//4289506476

TODO: tout coonvertir en grayscale:
R+G+B > 255*1.5  == clair color
R + G + B < 255 * 1.5 == dark color


const static int X_RANGE_MAX = 200;
const static int Y_DELTA_FOR_RAY = 20;
const static int Y_RANGE_JUMP_MAX = 150;
const static int Y_CRAWL_DELTA = 25;

const static int maxDistanceBeforeJump = 80;

const static PointStruct dinoHeadPos = { 1198, 219 }; // point on dino's body

static void PressUpKey() {
	INPUT input[3];

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0x28;
	input[0].ki.wScan = 0;
	input[0].ki.dwFlags = KEYEVENTF_KEYUP;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0x26;
	input[1].ki.wScan = 0;
	input[1].ki.dwFlags = 0;

	input[2].type = INPUT_KEYBOARD;
	input[2].ki.wVk = 0x26;
	input[2].ki.wScan = 0;
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(4, input, sizeof(INPUT));



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
