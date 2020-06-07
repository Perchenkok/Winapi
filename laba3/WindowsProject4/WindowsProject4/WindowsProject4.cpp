#include <Windows.h>
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <commctrl.h>
#include "resource.h"
#include "resource.h"
#include <stdio.h>
#include <string>
#pragma once 

#pragma comment( lib, "comctl32.lib")




HWND TextBox(LPCWSTR Text, LPCWSTR Font, int size, HWND hWnd, int posX, int posY, int length) {
	HWND New = CreateWindow(L"STATIC", Text, WS_VISIBLE | WS_CHILD, posX, posY, length, size, hWnd, NULL, NULL, NULL);
	HFONT hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET,
		0, 0, 0, 0,
		Font
	);
	SendMessage(New, WM_SETFONT, (WPARAM)hFont, NULL);
	return New;
}

int chosenFilm;
int chosenSession;

HWND Places[10];


BOOL jopa;


struct Session
{
	BOOL Places[10];
	wchar_t time[10];
	wchar_t price[10];
	HMENU Check;
};

struct Film {
	LPCWSTR name;
	Session sessions[5];
};

void LoadSession() {

}


Film Films[3];

bool win2opened = false;




void LoadFilms() {
	Films[0].name = L"Король лимонада 1";
	Films[1].name = L"Король лимонада 2";
	Films[2].name = L"Король лимонада 3";
	int counter = 0;
	for (int i = 0; i < 3; i++) {
		for (int time = 9; time < 21; time += 3) {
			swprintf_s(Films[i].sessions[(time / 3) - 3].time, L"%i:00", time);
			swprintf_s(Films[i].sessions[(time / 3) - 3].price, L"%i", ((time * 100) / 2) - 200);
			counter++;
			Films[i].sessions[(time / 3) - 3].Check = (HMENU)(counter);
			for (int j = 0; j < 10; j++) {
				Films[i].sessions[(time / 3) - 3].Places[j] = rand() % 2;
			}
		}
	}
}

wchar_t buff[20];

int deshifr() {
	std::string parsed = "";
	int counter = 0;
	for (int i = 0; i < 10; i++) {
		if (buff[i] != '\0') {
			parsed += buff[i];
			counter++;
		}
	}
	try {
		stoi(parsed);
	}
	catch (...) {
		MessageBox(NULL, L"Wrong Value", L"Wrong Value", MB_OK);
		return 0;
	} 
	return stoi(parsed);
}


HWND PPlace;
HWND PPPlace;

int chosenPlace;


BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG: {
		RECT ActualSize;
		GetClientRect(hWnd, &ActualSize);
		int line = 0;
		for (int i = 0; i < 10; i++) {
			if (i > 4) { line = 50; }	
			Places[i] = CreateWindow(L"STATIC", L"■", WS_VISIBLE | WS_CHILD | SS_CENTER, 30*(i%5), line, 50, 25, hWnd, (HMENU)(3220 + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
		CreateWindow(L"STATIC", L"Ряд", WS_VISIBLE | WS_CHILD, 0, 100, 50, 25, hWnd, (HMENU)(3321), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		PPlace =CreateWindow(L"EDIT", L"1", WS_VISIBLE | WS_CHILD, 50, 100, 50, 25, hWnd, (HMENU)(3327), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		CreateWindow(L"STATIC", L"Место", WS_VISIBLE | WS_CHILD, 0, 125,50, 25, hWnd, (HMENU)(3320), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		PPPlace = CreateWindow(L"EDIT", L"1", WS_VISIBLE | WS_CHILD, 50, 125, 50, 25, hWnd, (HMENU)(3326), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		CreateWindow(L"STATIC", L"Цена", WS_VISIBLE | WS_CHILD, 0, 150, 50, 25, hWnd, (HMENU)(3323), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		CreateWindow(L"STATIC", Films[chosenFilm].sessions[chosenSession].price, WS_VISIBLE | WS_CHILD, 50, 150, 50, 25, hWnd, (HMENU)(3323), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		CreateWindow(L"BUTTON", L"Продолжить", WS_VISIBLE | WS_CHILD, 0, 175, 100, 50, hWnd, (HMENU)3523, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
	}
	return 0;
	case WM_CTLCOLORSTATIC:
	{
		for (int i = 0; i < 10; i++) {
			if ((HWND)lParam == GetDlgItem(hWnd, 3220 + i)) {
				if (Films[chosenFilm].sessions[chosenSession].Places[i] == true) {
					SetBkMode((HDC)wParam, RGB(0, 255, 0));
					SetTextColor((HDC)wParam, RGB(0, 255, 0));
					return (BOOL)GetSysColorBrush(COLOR_MENU);
				}
				else if (Films[chosenFilm].sessions[chosenSession].Places[i] == false) {
					SetBkMode((HDC)wParam, RGB(255, 0, 0));
					SetTextColor((HDC)wParam, RGB(255, 0, 0));
					return (BOOL)GetSysColorBrush(COLOR_MENU);
				}

			}
		}

	}
	return 0;

	case WM_COMMAND: {

		if (LOWORD(wParam) == LOWORD(3523)) {
			GetWindowText(PPPlace, (LPWSTR)buff, 10);
			chosenPlace = deshifr();
			GetWindowText(PPlace, (LPWSTR)buff, 10);
			if (deshifr() > 2) {
				MessageBox(hWnd, L"Ошибка", L"Выбран неверный ряд", MB_OK);
			}
			chosenPlace += (5*deshifr())-5;
			if (chosenPlace > 10) {
				MessageBox(hWnd, L"Ошибка", L"Выбрано неверное значение", MB_OK);
				return 0;
			}
			if (deshifr() > 2) {
				MessageBox(hWnd, L"Ошибка", L"Такого ряда не существует", MB_OK);
				return 0;
			}
			if (Films[chosenFilm].sessions[chosenSession].Places[chosenPlace-1] == false) {
				MessageBox(hWnd, L"Ошибка", L"Место занято!", MB_OK);
				return 0;
			}
			MessageBox(hWnd, L"Успешно", L"Билет куплен", MB_OK);
			Films[chosenFilm].sessions[chosenSession].Places[chosenPlace-1] = false;
			UpdateWindow(hWnd);
			EndDialog(hWnd, 0);
			
		}
	}
	return 0;
	case WM_CLOSE:
	{
		win2opened = false;
		EndDialog(hWnd, 0);
		return 0;
	}
	}
	return 0;
}





LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HBRUSH brown = CreateSolidBrush(RGB(165, 42, 42));
	HBRUSH yellow = CreateSolidBrush(RGB(255, 255, 200));
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rectPlace;
	GetClientRect(hWnd, &rectPlace);

	switch (uMsg) {
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		EndPaint(hWnd, &ps);
	}
	
	case WM_LBUTTONDOWN:
	{
	}
	return 0;
	case WM_RBUTTONDOWN:
	{

	}
	return 0;
	case WM_CREATE:
	{
		LoadFilms();
		HWND Title = TextBox(L"Сеансы", L"Courier New", 60, hWnd, 0, 0, 500);
		hdc = GetDC(hWnd);
		for (int i = 0; i < 3; i++) {
			TextBox(Films[i].name, L"Courier New", 30, hWnd, 0, 100 + i * 100, 500);
			for (int j = 0; j < 500; j += 100)
			{
				TextBox(Films[i].sessions[j / 100].time, L"Courier New", 25, hWnd, j, 125 + i * 100, 100);
				TextBox(Films[i].sessions[j / 100].price, L"Courier New", 25, hWnd, j, 150 + i * 100, 100);
				HWND BuyButton = CreateWindow(L"BUTTON", L"Купить", WS_CHILD | WS_VISIBLE, j, 175 + i * 100, 100, 25, hWnd, Films[i].sessions[j / 100].Check, NULL, NULL);
			}
		}
		
		

	}
	return 0;
	case WM_COMMAND:
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 5; j++) {
				if (LOWORD(wParam) == LOWORD(Films[i].sessions[j].Check)) {
					chosenFilm = i;
					chosenSession = j;
				
					DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DlgProc), 0);
				}
			}
		}
		if (LOWORD(wParam) == LOWORD(100002)) {
			DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DlgProc), 0);
		}

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
	HBRUSH turquoise = CreateSolidBrush(RGB(255, 255, 200));
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
	hwnd = CreateWindow(wc.lpszClassName, L"Lab_3", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 415, 440, NULL, NULL, wc.hInstance, NULL);
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return EXIT_FAILURE;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

