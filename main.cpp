#include <stdio.h>
#include <windows.h>

HANDLE gv_handle;
ULONG GetProcessID (HWND lp_hwnd) {
	ULONG lv_idProc;
	GetWindowThreadProcessId(lp_hwnd, &lv_idProc);
	return lv_idProc;
}

//Hookup Proc
LRESULT CALLBACK KeyProc(int lp_code, WPARAM lp_wParam, LPARAM lp_lParam) {
	KBDLLHOOKSTRUCT lv_struct = *(KBDLLHOOKSTRUCT*)lp_lParam;
	
	if (lp_wParam == WM_SYSKEYDOWN) {
		if (lv_struct.vkCode == VK_F4) {
		if (lv_struct.flags & LLKHF_ALTDOWN) {
			printf("TerminateProcess(%d)\n", GetForegroundWindow());
			gv_handle = OpenProcess(PROCESS_ALL_ACCESS, 0, GetProcessID(GetForegroundWindow()));
			TerminateProcess(gv_handle, 0);
			CloseHandle(gv_handle);
			return 1;
		}}
	}

	return CallNextHookEx(NULL, lp_code, lp_wParam, lp_lParam);
}

int WINAPI WinMain (HINSTANCE lp_hInstance, HINSTANCE lp_hPrevInstance, LPSTR lp_pCmdLine, int lp_nCmdShow) {
	SetWindowsHookEx(WH_KEYBOARD_LL, KeyProc, lp_hInstance, 0);

	//Message Loop
	MSG lv_msg = {};
	while (GetMessage(&lv_msg, NULL, 0, 0) > 0) {
		TranslateMessage(&lv_msg);
		DispatchMessage(&lv_msg);
	}
	return 0;
}