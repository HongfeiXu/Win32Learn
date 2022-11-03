#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateButtonLayoutForDpi(HWND hWnd);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	{
	AllocConsole();
	FILE* file = nullptr;
	freopen_s(&file, "CONIN$", "r", stdin);
	freopen_s(&file, "CONOUT$", "w", stdout);
	}

	// Register the window class
	const wchar_t CLASS_NAME[] = L"Sample Window Class";
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	// you must call the corresponding API before any HWNDs have been created.
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Create the window
	HWND hwnd = CreateWindowEx(
		0,								// Optional window styles
		CLASS_NAME,						// Window class
		L"Learn to Program Windows",	// Window text
		WS_OVERLAPPEDWINDOW,			// Window style

		// Position and size
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,

		NULL,			// Parent window
		NULL,			// Menu
		hInstance,		// Instance handle
		NULL			// Additional application data
	);
	if (hwnd == NULL)
	{
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);

	// Run the message loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_NCCREATE:
		{
			// DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 不需要手动开启了，见DpiAwarenessContext示例
			EnableNonClientDpiScaling(hWnd);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			// All painting occurs here, between BeginPaint and EndPaint.

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hWnd, &ps);
		}
		return 0;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			std::cout << "x = " << pt.x << ", y = " << pt.y << std::endl;

			int iDpi = GetDpiForWindow(hWnd);
			std::cout << "iDpi = " << iDpi << std::endl;
		}
		return 0;
	case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			std::cout << "width = " << width << ", height = " << height << std::endl;
		}
		return 0;
	case WM_CREATE:
		{
			HWND hWndChild = CreateWindow(L"BUTTON", L"Click Me",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 0, 0,
				hWnd, (HMENU)NULL, NULL, NULL);
			if (hWndChild != NULL)
			{
				UpdateButtonLayoutForDpi(hWndChild);
			}
		}
		return 0;
	case WM_DPICHANGED:
		{
			UINT uDpi = HIWORD(wParam);
			UINT vDpi = LOWORD(wParam);
			std::cout << "uDpi = " << uDpi << ", vDpi = " << vDpi << std::endl;
			int iDpi = GetDpiForWindow(hWnd);
			std::cout << "iDpi = " << iDpi << std::endl;

			HWND hWndButton = FindWindowEx(hWnd, NULL, NULL, NULL);
			if (hWndButton != NULL)
			{
				UpdateButtonLayoutForDpi(hWndButton);
			}

			// Resize the window
			auto lprcNewScale = reinterpret_cast<RECT*>(lParam);
			SetWindowPos(hWnd, nullptr, lprcNewScale->left, lprcNewScale->top,
				lprcNewScale->right - lprcNewScale->left, lprcNewScale->bottom - lprcNewScale->top,
				SWP_NOZORDER | SWP_NOACTIVATE);

		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// Default message handling
}

#define INITIALX_96DPI 50 
#define INITIALY_96DPI 50 
#define INITIALWIDTH_96DPI 100 
#define INITIALHEIGHT_96DPI 50 


// DPI scale the position and size of the button control 
void UpdateButtonLayoutForDpi(HWND hWnd)
{
	int iDpi = GetDpiForWindow(hWnd);
	int dpiScaledX = MulDiv(INITIALX_96DPI, iDpi, 96);
	int dpiScaledY = MulDiv(INITIALY_96DPI, iDpi, 96);
	int dpiScaledWidth = MulDiv(INITIALWIDTH_96DPI, iDpi, 96);
	int dpiScaledHeight = MulDiv(INITIALHEIGHT_96DPI, iDpi, 96);
	SetWindowPos(hWnd, hWnd, dpiScaledX, dpiScaledY, dpiScaledWidth, dpiScaledHeight, SWP_NOZORDER | SWP_NOACTIVATE);
}



// 打开控制台
//{
//	AllocConsole();
//	FILE* file = nullptr;
//	freopen_s(&file, "CONIN$", "r", stdin);
//	freopen_s(&file, "CONOUT$", "w", stdout);
//}
//std::cout << "hello console!" << std::endl;
// 关闭控制台
//{
//	FreeConsole();
//}