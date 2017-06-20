#include "Windows.h"
#pragma once


struct PointStruct { int x; int y; };

static INPUT inputKeybd; // keyboard

static void initKeyboard() {
  // Set up a generic keyboard event.
  inputKeybd.type = INPUT_KEYBOARD;
  inputKeybd.ki.wScan = 0; // hardware scan code for key
  inputKeybd.ki.time = 0;
  inputKeybd.ki.dwExtraInfo = 0;
}

static void PressUpKey() {
  inputKeybd.ki.wVk = 0x26; // virtual-key code for the "up" key
  inputKeybd.ki.dwFlags = 0; // 0 for key press
  SendInput(1, &inputKeybd, sizeof(INPUT));

  inputKeybd.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &inputKeybd, sizeof(INPUT));
}


static void HoldDownKey() {
  inputKeybd.ki.wVk = 0x28; // virtual-key code for the "up" key
  inputKeybd.ki.dwFlags = 0; // 0 for key press
  SendInput(1, &inputKeybd, sizeof(INPUT));
}


static void ReleaseDownKey() {
  inputKeybd.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &inputKeybd, sizeof(INPUT));
}
