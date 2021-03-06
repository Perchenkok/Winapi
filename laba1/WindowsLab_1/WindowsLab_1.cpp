#include "stdafx.h";
#include <Windows.h>



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HBRUSH pW;
	HBRUSH turquoise = CreateSolidBrush(RGB(0, 0, 255));
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = GetDC(hWnd);
	SetDCBrushColor(
		hdc,
		RGB(0, 0, 255)
	);
	RECT rectPlace;
	GetClientRect(hWnd, &rectPlace);
	switch (uMsg) {
	case WM_PAINT:
	{

		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		EndPaint(hWnd, &ps);
	}
	return 0;
	case WM_LBUTTONDOWN:
	{
		//hdc = BeginPaint(hWnd, &ps);
		hdc = GetDC(hWnd);
		pW = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, pW);
		Pie(hdc, 100, 100, 200, 200, 150, 100, 150, 200);
		ReleaseDC(hWnd, hdc);
	}
	return 0;
	case WM_RBUTTONDOWN:
	{
		hdc = GetDC(hWnd);
		pW = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, pW);
		Pie(hdc, 100, 100, 200, 200, 150, 200, 150, 100);
		ReleaseDC(hWnd, hdc);
	}
	return 0;
	case WM_CREATE:
	{
	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};




int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow) {
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	HBRUSH turquoise = CreateSolidBrush(RGB(128, 128, 128));

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = turquoise;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;

	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"Class";
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}
	hwnd = CreateWindow(wc.lpszClassName, L"Lab_1", WS_OVERLAPPEDWINDOW, 0, 0, 600, 600, NULL, NULL, wc.hInstance, NULL);
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}