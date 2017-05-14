#include "stdafx.h"
#include <cstdio>
#include <tchar.h>
#include "GlobalHook.h"

// すべてのスレッドにセットされるフックになるので
// グローバル変数を共有する必要がある
//共有セグメント
#pragma data_seg(".shareddata")
HHOOK mouseHook = 0;
HWND g_hWnd = 0;        //キーコードの送り先のウインドウハンドル
#pragma data_seg()

HINSTANCE hInst;
//bool bSetHook;    //SetHook関数を呼んだプロセスであるか

bool isScrollMode = false;
bool isFirst = true;
bool spaceKeyState = false;
bool isPaused = false;
int lastX, lastY;
int debugData = -1;

__declspec(dllexport) int SetHook(HWND hWnd)
{
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInst, NULL);
	if (mouseHook == NULL) {
		//フック失敗
	}
	else {
		//bSetHook = TRUE;
		//フック成功
		return 114514;
		g_hWnd = hWnd;
	}
	return 0;
}

__declspec(dllexport) int ResetHook()
{
	if (UnhookWindowsHookEx(mouseHook) != 0) {
		//フック解除成功
	}
	else {
		//フック解除失敗
	}
	return debugData;
	return 0;
}

__declspec(dllexport) LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode < 0 || isPaused)    //決まり事
		return CallNextHookEx(mouseHook, nCode, wp, lp);

	MSLLHOOKSTRUCT *ms = (MSLLHOOKSTRUCT*)lp;
	int x = ms->pt.x, y = ms->pt.y;
	bool cancelFlag = false;
	if (wp == WM_XBUTTONDOWN && GET_XBUTTON_WPARAM(ms->mouseData) == 1)
	{
		isScrollMode = !isScrollMode;
		cancelFlag = true;
	}
	else if ((wp == WM_XBUTTONUP || wp == WM_XBUTTONDBLCLK) && GET_XBUTTON_WPARAM(ms->mouseData) == 1)
	{
		cancelFlag = true;
	}
	else if (!isFirst && wp == WM_MOUSEMOVE && isScrollMode)
	{
		int delta = lastY - y;
		debugData = max(debugData, delta);
		INPUT input;
		memset(&input, 0, sizeof(input));
		input.type = INPUT_MOUSE;
		input.mi.mouseData = delta*10;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		x = lastX; y = lastY;
		
		isPaused = true;
		SendInput(1, &input, sizeof(INPUT));
		isPaused = false;
		cancelFlag = true;
	}
	
	lastX = x; lastY = y;
	isFirst = false;

	if (cancelFlag) return 1;
	return CallNextHookEx(mouseHook, nCode, wp, lp);
}

// エントリポイント
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// アタッチ
		hInst = hModule;
		//bSetHook = FALSE;
		break;
	case DLL_PROCESS_DETACH:
		// デタッチ
		break;
	}
	return TRUE;
}