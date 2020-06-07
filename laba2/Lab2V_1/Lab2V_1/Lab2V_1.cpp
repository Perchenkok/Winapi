#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName2[] = _T("MainFrame2");

int WINAPI wWinMain(HINSTANCE This, // Дескриптор текущего приложения
	HINSTANCE Prev, // В современных системах всегда 0
	LPTSTR cmd, // Командная строка
	int mode) // Режим отображения окна
{
	HWND hWnd; // Дескриптор главного окна программы
	MSG msg; // Структура для хранения сообщения
	WNDCLASS wc; // Класс окна
	// Определение класса окна
	wc.hInstance = This;
	wc.lpszClassName = WinName2; // Имя класса окна
	wc.lpfnWndProc = WndProc; // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль окна
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Стандартная иконка
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
	wc.lpszMenuName = NULL; // Нет меню
	wc.cbClsExtra = 0; // Нет дополнительных данных класса
	wc.cbWndExtra = 0; // Нет дополнительных данных окна
	wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 200))); // Заполнение окна белым цветом


	// Регистрация класса окна
	if (!RegisterClass(&wc)) return 0;

	// Создание окна
	hWnd = CreateWindow(WinName2, // Имя класса окна
		_T("Окно 1"), // Заголовок окна
		WS_OVERLAPPEDWINDOW, // Стиль окна
		77, // x
		100, // y Размеры окна
		400, // width
		500, // Height
		HWND_DESKTOP, // Дескриптор родительского окна
		NULL, // Нет меню
		This, // Дескриптор приложения
		NULL); // Дополнительной информации нет

	ShowWindow(hWnd, mode); // Показать окно

	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}

// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения

int i = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND des;
	switch (message) // Обработчик сообщений
	{
	case WM_CREATE:
	{
		HMENU hMenubar = CreateMenu();
		HMENU hMenu = CreateMenu();
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"Опции");
		AppendMenu(hMenu, MF_STRING, 1000, L"Открыть");
		AppendMenu(hMenu, MF_STRING, 1001, L"Закрыть"); //создание меню
		SetMenu(hWnd, hMenubar);
	}
	return 0;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1000)
		{
			WinExec("C:\\Users\\User\\Desktop\\Winapi\\laba2\\Lab2V_2\\Debug\\Lab2V_2.exe", SW_SHOW);
		}
		else if (LOWORD(wParam) == 1001) {

			HWND secondWindow = FindWindow(NULL, L"Окно 2");
			SendMessage(secondWindow, WM_DESTROY, NULL, NULL);
		}
	}
	return 0;
	case WM_USER + 1:
	{
		i++;
		MessageBox(hWnd, _T("Сообщение об изменении размеров окна 2 получено."), _T("Изменение размеров окна"), MB_OK);
		if ((i % 2) == 1)
			SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 400, 250, SWP_SHOWWINDOW);
		else SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 400, 500, SWP_SHOWWINDOW);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	case WM_USER + 2:
	{
		MessageBox(hWnd, _T("Сообщение о выключении компьютера получено."), _T("Выключение компьютера"), MB_OK);
		//ExitWindows(EWX_SHUTDOWN, 0);

	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break; // Завершение программы
	default: // Обработка сообщения по умолчанию
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}