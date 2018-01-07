#include <cstdio>
#include <tchar.h>
#include "GlobalHook.h"

// ���ׂẴX���b�h�ɃZ�b�g�����t�b�N�ɂȂ�̂�
// �O���[�o���ϐ������L����K�v������
//���L�Z�O�����g
#pragma data_seg(".shareddata")
HHOOK mouseHook = 0;
HWND g_hWnd = 0;        //�L�[�R�[�h�̑����̃E�C���h�E�n���h��
#pragma data_seg()

HINSTANCE hInst;
//bool bSetHook;    //SetHook�֐����Ă񂾃v���Z�X�ł��邩

bool isScrollMode = false;
bool isFirst = true;
bool isPaused = false;
int ctrlX = -1, ctrlY = -1;
int lastX, lastY;
int debugData = -1;
int virtualDX = 0, virtualDY = 0;

__declspec(dllexport) int SetHook(HWND hWnd)
{
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInst, NULL);
	if (mouseHook == NULL) {
		//�t�b�N���s
	}
	else {
		//bSetHook = TRUE;
		//�t�b�N����
		return 114514;
		g_hWnd = hWnd;
	}
	return 0;
}

__declspec(dllexport) int ResetHook()
{
	if (UnhookWindowsHookEx(mouseHook) != 0) {
		//�t�b�N��������
	}
	else {
		//�t�b�N�������s
	}
	return 0;
}

__declspec(dllexport) LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode < 0 || isPaused)    //���܂莖
		return CallNextHookEx(mouseHook, nCode, wp, lp);

	MSLLHOOKSTRUCT *ms = (MSLLHOOKSTRUCT*)lp;
	int x = ms->pt.x, y = ms->pt.y;
	bool cancelFlag = false;
	bool ctrl = GetAsyncKeyState(VK_CONTROL);
	if (!ctrl) { ctrlX = -1; ctrlY = -1; }
	if (ctrl) {
		if (ctrlX == -1 && ctrlY == -1) {
			ctrlX = x; ctrlY = y;
			virtualDX = 0; virtualDY = 0;
		}
		virtualDX += x - lastX;
		virtualDY += y - lastY;
		int actualDX = virtualDX/5.0;
		int actualDY = virtualDY/5.0;
		x = ctrlX + actualDX;
		y = ctrlY + actualDY;
		INPUT input;
		memset(&input, 0, sizeof(input));
		input.type = INPUT_MOUSE;
		input.mi.dx = (x - lastX);
		input.mi.dy = (y - lastY);
		input.mi.dwFlags = MOUSEEVENTF_MOVE;

		isPaused = true;
		SendInput(1, &input, sizeof(INPUT));
		cancelFlag = true;
	}
	if (GET_XBUTTON_WPARAM(ms->mouseData) == 1)
	{
		if (wp == WM_XBUTTONDOWN)
		{
			isScrollMode = !isScrollMode;
		}
		cancelFlag = true;
	}
	else if (wp == WM_MOUSEMOVE)
	{
		if (isScrollMode && !isFirst)
		{
			int delta = lastY - y;
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
	}
	else if (wp == WM_LBUTTONDOWN || wp == WM_MBUTTONDOWN || wp == WM_RBUTTONDOWN)
	{
		if (isScrollMode)
		{
			isScrollMode = false;
			cancelFlag = true;
		}
	}
	
	lastX = x; lastY = y;
	isFirst = false;
	isPaused = false;

	if (cancelFlag) return 1;
	return CallNextHookEx(mouseHook, nCode, wp, lp);
}

// �G���g���|�C���g
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// �A�^�b�`
		hInst = hModule;
		//bSetHook = FALSE;
		break;
	case DLL_PROCESS_DETACH:
		// �f�^�b�`
		break;
	}
	return TRUE;
}