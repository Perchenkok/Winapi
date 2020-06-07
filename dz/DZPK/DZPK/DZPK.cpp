
#include <stdafx.h>
#include <Windows.h>
#include <string>
#include <algorithm>
#include <float.h>
#include <vector> 
#include <commctrl.h>
#include <math.h>
#include <windowsx.h>
#include <string>
#include <stdio.h>
#include <vector>



#define QUALITY	10000 //число вычисляемых точек


HANDLE hThread1; //управление потоками 
HANDLE hThread2;
HANDLE hThread3;
HANDLE hThread4;
HANDLE hThread5;



float size = 2;


COLORREF Color = RGB(0, 0, 255); //изначальный цвет графика

void DrawCoords(HWND hWindow) {																			//отрисовка координат
	PAINTSTRUCT PS;
	HDC hdc = BeginPaint(hWindow, &PS);
	HPEN pen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));											//pen который рисует линии коориднат (черный)
	SelectObject(hdc, pen);
	MoveToEx(hdc, 0, 500, NULL);																		//линия от {0,500} до {1000,500} (вертикальная черта)
	LineTo(hdc, 1000, 500);
	MoveToEx(hdc, 500, 0, NULL);																//горизонтальная
	LineTo(hdc, 500, 1000);
	for (int i = -20; i < 20; i++) {
		MoveToEx(hdc, 495, 500 + (i * 20 * size), NULL);
		LineTo(hdc, 505, 500 + (i * 20 * size));							//чёрточки 
	}
	for (int i = -20; i < 20; i++) {
		MoveToEx(hdc, 500 + (i * 20 * size), 495, NULL);
		LineTo(hdc, 500 + (i * 20 * size), 505);
	}
	RECT ok;
	ok.left = 530;																	//надпись п/4 (мера деления)
	ok.right = 600;
	ok.top = 470;
	DrawText(hdc, L"П/4", 4, &ok, NULL);
	EndPaint(hWindow, &PS);
	ReleaseDC(hWindow, hdc);
}

POINT Data[QUALITY];														//массив , который и хранит все точки 

float maxY;
float maxX;

float T = -1;

BOOL finished = false;

int multi = 1;																	//коэф перед f
int multi2 = 1;																		//коэф перед x

DWORD WINAPI Thread2(LPVOID t) {
	int pos = 0;
	Data[0].x = -10;									//начальный x
	Data[0].y = 0;
	float x = -10, y = 0;
	for (pos = 0; pos < QUALITY - 1; pos++)
	{
		x += (20.0 / QUALITY);							//точное значение x (float)
		Data[pos].x = x * 50 + 500;														//значение точки по x на экране			
		//abs(lg(sin(cos(x))))/x
		Data[pos].y = multi * -((abs(log(10) / log(sin(cos(multi2*x)))) / (multi2*x))) * 30 + 500;//значение точки по y на экране	

	}
	finished = true;
	return 0;
}




DWORD WINAPI Thread1(LPVOID t) {
	HDC hdc = GetDC((HWND)t);
	for (int i = (QUALITY * 3) / 4; i < (QUALITY); i++) {							//поток,который рисует последние 25% точек 
		SetPixelV(hdc, Data[i].x, Data[i].y, Color);
		SetPixelV(hdc, Data[i].x, Data[i].y + 1, Color);
		SetPixelV(hdc, Data[i].x + 1, Data[i].y, Color);
		SetPixelV(hdc, Data[i].x + 1, Data[i].y + 1, Color);
	}
	return 0;
}
DWORD WINAPI Thread3(LPVOID t) {
	HDC hdc = GetDC((HWND)t);
	for (int i = 0; i < (QUALITY / 4); i++) {																			//поток,который рисует первые 25% точек 
		SetPixelV(hdc, Data[i].x, Data[i].y, Color);
		SetPixelV(hdc, Data[i].x, Data[i].y + 1, Color);
		SetPixelV(hdc, Data[i].x + 1, Data[i].y, Color);
		SetPixelV(hdc, Data[i].x + 1, Data[i].y + 1, Color);
	}
	return 0;
}
DWORD WINAPI Thread4(LPVOID t) {
	HDC hdc = GetDC((HWND)t);

	for (int i = QUALITY / 4; i < (QUALITY * 2) / 4; i++) {
		SetPixelV(hdc, Data[i].x, Data[i].y, Color);									//поток,который рисует 25% точек (с 25% до 50%) 
		SetPixelV(hdc, Data[i].x, Data[i].y + 1, Color);
		SetPixelV(hdc, Data[i].x + 1, Data[i].y, Color);
		SetPixelV(hdc, Data[i].x + 1, Data[i].y + 1, Color);
	}
	return 0;
}

DWORD WINAPI Thread5(LPVOID t)
{
	HDC hdc = GetDC((HWND)t);
	for (int i = ((QUALITY * 2) / 4); i < (QUALITY * 3) / 4; i++) {
		SetPixelV(hdc, Data[i].x, Data[i].y, Color);
		SetPixelV(hdc, Data[i].x, Data[i].y + 1, Color);													//поток,который рисует 25% точек (с 50% до 75%) 
		SetPixelV(hdc, Data[i].x + 1, Data[i].y, Color);
		SetPixelV(hdc, Data[i].x + 1, Data[i].y + 1, Color);
	}
	return 0;
}


void StartDrawing(HWND hWnd) {

	hThread3 = CreateThread(NULL, 0, Thread3, hWnd, 0, NULL);
	hThread4 = CreateThread(NULL, 0, Thread4, hWnd, 0, NULL);									//функция создающая все потоки для отрисовки графика 
	hThread5 = CreateThread(NULL, 0, Thread5, hWnd, 0, NULL);
	hThread1 = CreateThread(NULL, 0, Thread1, hWnd, 0, NULL);

}

void StopDrawing(HWND hWnd) {
	TerminateThread(hThread1, 0);
	TerminateThread(hThread3, 0);												//функция останвливающая все потоки для отрисовки графика 
	TerminateThread(hThread4, 0);

}





LRESULT CALLBACK WindowProcess(HWND hWindow,
	UINT uMessage,
	WPARAM wParameter,
	LPARAM lParameter)
{

	switch (uMessage) {
	case WM_PAINT:
	{
		DrawCoords(hWindow);
	}
	return 0;
	case WM_CREATE:
	{
		hThread2 = CreateThread(NULL, 0, Thread2, hWindow, 0, NULL);
		HMENU hMenubar = CreateMenu();
		HMENU hMenu = CreateMenu();
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"Опции");

		AppendMenu(hMenu, MF_STRING, 1200, L"Изменить цвет на случайный");
		AppendMenu(hMenu, MF_STRING, 1201, L"Перевернуть график");							//создание меню
		AppendMenu(hMenu, MF_STRING, 1202, L"Отрисовка ориг. графика");
		AppendMenu(hMenu, MF_STRING, 1203, L"Коэф. 2 при X");
		AppendMenu(hMenu, MF_STRING, 1204, L"Коэф. 4 при X");
		SetMenu(hWindow, hMenubar);

	}
	return 0;
	case WM_LBUTTONDOWN: {
		StartDrawing(hWindow);
	}

						 return 0;

	case WM_KEYDOWN:
	{
	}
	return 0;
	case WM_COMMAND:
	{
		if (LOWORD(wParameter) == LOWORD(1200)) {											//обработка нажатия на пункты меню 
			RECT rect;
			HDC hdc = GetDC(hWindow);
			GetWindowRect(hWindow, &rect);
			Color = (rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100);
			InvalidateRect(hWindow, &rect, true);
			StartDrawing(hWindow);
			return 0;
		}
		if (LOWORD(wParameter) == LOWORD(1201)) {
			RECT rect;

			HDC hdc = GetDC(hWindow);
			GetWindowRect(hWindow, &rect);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
			multi *= -1;
			InvalidateRect(hWindow, &rect, true);
			hThread2 = CreateThread(NULL, 0, Thread2, hWindow, 0, NULL);
			StartDrawing(hWindow);
			return 0;
		}
		if (LOWORD(wParameter) == LOWORD(1202)) {
			RECT rect;
			HDC hdc = GetDC(hWindow);
			GetWindowRect(hWindow, &rect);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
			multi2 = 1;
			hThread2 = CreateThread(NULL, 0, Thread2, hWindow, 0, NULL);
			InvalidateRect(hWindow, &rect, true);
			multi = 1;
			StartDrawing(hWindow);
			return 0;
		}
		if (LOWORD(wParameter) == LOWORD(1203)) {
			RECT rect;
			HDC hdc = GetDC(hWindow);
			GetWindowRect(hWindow, &rect);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
			multi2 = 2;
			InvalidateRect(hWindow, &rect, true);
			hThread2 = CreateThread(NULL, 0, Thread2, hWindow, 0, NULL);
			StartDrawing(hWindow);
			return 0;
		}
		if (LOWORD(wParameter) == LOWORD(1204)) {
			RECT rect;
			HDC hdc = GetDC(hWindow);
			GetWindowRect(hWindow, &rect);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
			multi2 = 4;
			InvalidateRect(hWindow, &rect, true);
			hThread2 = CreateThread(NULL, 0, Thread2, hWindow, 0, NULL);
			StartDrawing(hWindow);
			return 0;
		}
	}
	return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
	}
	return NULL;
}




int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow) {
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;

	wc.lpfnWndProc = WindowProcess;
	wc.lpszClassName = L"Class";
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClassEx(&wc)) {
		return EXIT_FAILURE;
	}
	hwnd = CreateWindow(wc.lpszClassName, L"DZ",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
		, 0, 0, 1000, 1000, NULL, NULL, wc.hInstance, NULL);
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

