#include "MessageBox.h"

/// <summary>
/// Centers MessageBoxes at the center of the desired window.
/// </summary>
/// <param name="hWnd: ">The handle to the desired window.</param>
/// <param name="lpText: ">The main text to be shown on the messageBox.</param>
/// <param name="lpCaption: ">The caption text to be shown on the messageBox.</param>
/// <param name="uType: ">The button(s) of the MessageBox.</param>
/// <param name="Window_Width: ">The width of the desired window.</param>
/// <param name="Window_Height: ">The height of the desired window.</param>
/// <returns>Whichever button the user pressed.</returns>
int MessageBoxCentered(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, int Window_Width, int Window_Height)
{
	// Center message box at its parent window
	static int Window_W = Window_Width;
	static int Window_H = Window_Height;
	static HHOOK hHookCBT{};
	hHookCBT = SetWindowsHookEx(WH_CBT,
		[](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT
		{
			if (nCode == HCBT_CREATEWND)
			{
				if (((LPCBT_CREATEWND)lParam)->lpcs->lpszClass == (LPWSTR)(ATOM)32770)  // #32770 = dialog box class
				{
					RECT rect{};
					bool test{};
					test = GetWindowRect(((LPCBT_CREATEWND)lParam)->lpcs->hwndParent, &rect);
					if (!test) exit(0x1); //exit (code 1) if there is a problem somewhere, probably in the code.
					((LPCBT_CREATEWND)lParam)->lpcs->x = rect.left + (Window_W - ((LPCBT_CREATEWND)lParam)->lpcs->cx) / 2;
					((LPCBT_CREATEWND)lParam)->lpcs->y = rect.bottom - (Window_H + ((LPCBT_CREATEWND)lParam)->lpcs->cy) / 2;
				}
			}

			return CallNextHookEx(hHookCBT, nCode, wParam, lParam);
		},
		0, GetCurrentThreadId());

	int iRet{ MessageBox(hWnd, lpText, lpCaption, uType) };

	UnhookWindowsHookEx(hHookCBT);

	return iRet;
}
