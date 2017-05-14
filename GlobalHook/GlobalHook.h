#pragma once
#include "stdafx.h"

extern "C" __declspec(dllexport) LRESULT CALLBACK MouseHookProc(int, WPARAM, LPARAM);
extern "C" __declspec(dllexport) int SetHook(HWND hWnd);
extern "C" __declspec(dllexport) int ResetHook();
