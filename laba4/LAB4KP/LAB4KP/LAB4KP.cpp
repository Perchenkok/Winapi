
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
#include <fstream>
#include <iostream>


#define M_PI 3.1415926535897932384 //сокращение константы пи


HANDLE hThread1; //управление потоками 
HANDLE hThread2;

HANDLE hThread3;

std::ifstream fin("Contacts.txt");
char *bufContact = new char[1024];
char ch;
std::string bufStr;

int i = 0;
int j = 0;
int k = 0;
wchar_t *outContact = new wchar_t[64];

char *bufPhone = new char[12];
char* bufName = new char[64];



int  R, G, B;

static wchar_t* text = new wchar_t[50]; //массив для хранения текста с клавиатуры
static int counter = 0; //текущий введенный символ

static wchar_t* color = new wchar_t[13];

std::string colorStr;

DWORD WINAPI Thread3(LPVOID t) {
	std::ifstream f("Text.txt");
	f.seekg(0, std::ios::beg);
	if (f.is_open()) {
		std::getline(f, colorStr);
	}
	char actualChar='a';
	std::string temp="";
	int i = 0;
	while (actualChar != ':') {
	
		actualChar = colorStr[i];
		temp += actualChar;
		i++;
	}
	R = std::stoi(temp);
	temp = "";
	actualChar = 'a';
	while (actualChar != ':') {

		actualChar = colorStr[i];
		temp += actualChar;
		i++;
	}
	G = std::stoi(temp);
	actualChar = 'a';
	temp = "";
	while (actualChar != ':') {

		actualChar = colorStr[i];
		temp += actualChar;
		i++;
	}
	B = std::stoi(temp);
	return 0;
}



DWORD WINAPI Thread2(LPVOID t) {			//функция для первого потока
	HDC hDeviceContext;
	PAINTSTRUCT paintStruct;
	RECT rectPlace; //прямоугольник куда пишется текст 
	HFONT hFont; //шрифт текста 
	GetWindowRect((HWND)t, &rectPlace);
	hDeviceContext = GetDC((HWND)t);
	SetTextColor(hDeviceContext, RGB(rand() % 255, rand() % 255, rand() % 255));
	hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, //параметры шрифта 
		DEFAULT_CHARSET,
		0, 0, 0, 0,
		L"Times New Roman"
	);
	RECT black;
	black.top = 0;
	black.bottom = 400;			//координаты прямоугольника, который закрашивает пред. текст 
	black.left = 0;
	black.right = 2000;
	HBRUSH Black = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hDeviceContext, &black, Black);
	SelectObject(hDeviceContext, hFont);
	SetBkMode(hDeviceContext, RGB(125, 125, 125));
	DrawTextW(hDeviceContext, text, counter, &rectPlace, NULL); //вывод текста
	ReleaseDC((HWND)t, hDeviceContext);
	EndPaint((HWND)t, &paintStruct);
	return 0;
}

bool DoFill;

DWORD WINAPI Thread1(LPVOID t) {  //функция ,выполняемая первым потоком 

	HDC hdc = GetDC((HWND)t);
	RECT black;
	black.top = 400;
	black.bottom = 1000;			//координаты прямоугольника,который закрашивает предыдущее положение фигуры
	black.left = 0;
	black.right = 1000;
	int speed = 1;
	while (true) {
		for (double x = M_PI / 2; x < (5 * M_PI) / 2; x += (0.002*speed)) {
			//закрашивается прямоугольник 
			if (DoFill == true) {
				FillRect(hdc, &black, CreateSolidBrush(RGB(0, 0, 0)));	//закрашивание следа при выборе опции 
			}
			HPEN pen = CreatePen(PS_DASH, 2, RGB(120 * (sin(2 * x) + 1), 120 * (cos(4 * x) + 1), 120 * (cos(3 * x) + 1)));						//цвет обводки фигруы меняется в зависимости от x
			HBRUSH brush = CreateSolidBrush(RGB(R,G,B));		//цвет самой фигуры меняется в зависимости от x
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			POINT vertices[] = { {500 * (sin(x) + 1) - 100,650} ,
								 {500 * (sin(x) + 1), 400},											//параметры шестиугольника (координата x меняется в зависимости от x
								 {500 * (sin(x) + 1) + 100,400},
								 {500 * (sin(x) + 1) + 200,650},
								 {500 * (sin(x) + 1) + 100, 900},
								 {500 * (sin(x) + 1), 900} };
			Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0])); //отрисовка фигуры
			Sleep(11);		//задержка 
			if ((500 * (sin(x) + 1)) < 0.5) {
				speed = 5;

				//ускорение в случае если треугольник дошел до левого края
			}
			if ((500 * (sin(x) + 1)) > 998) {
				speed = 1;
				//замедление если треугольник дошел до правого края
			}
			DeleteObject(pen);						//удаление кисти и ручки ,чтобы избежать ошибок (иначе программа вылетает через какое-то время)
			DeleteObject(brush);
		}
	}
	return 0;
}






LRESULT CALLBACK WindowProcess(HWND hWindow,
	UINT uMessage,
	WPARAM wParameter,
	LPARAM lParameter)
{
	HDC hDeviceContext;
	PAINTSTRUCT paintStruct;
	RECT rectPlace;
	HFONT hFont;
	switch (uMessage) {
	case WM_CREATE:
	{
		
		hThread1 = CreateThread(NULL, 0, Thread1, hWindow, 0, NULL); // создание процесса для отрисовки треугольника
		HMENU hMenubar = CreateMenu();
		HMENU hMenu = CreateMenu();														//создание меню и пунктов меню
		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"Опции");

		AppendMenu(hMenu, MF_STRING, 1200, L"Включить след");
		AppendMenu(hMenu, MF_STRING, 1201, L"Выключить след");
		AppendMenu(hMenu, MF_STRING, 1202, L"Цвет из файла");
		SetMenu(hWindow, hMenubar);

	}
	return 0;
	case WM_LBUTTONDOWN: {

	}
						 return 0;
	case WM_RBUTTONDOWN:
	{

	}
	return 0;
	case WM_PAINT:
	{

	}
	return 0;
	case WM_COMMAND:
	{													//обработка нажатий на пункты меню 
		if (LOWORD(wParameter) == LOWORD(1200)) {
			DoFill = false;
			UpdateWindow(hWindow);
			return 0;
		}
		if (LOWORD(wParameter) == LOWORD(1201)) {
			DoFill = true;
			UpdateWindow(hWindow);
			return 0;
		}
		if (LOWORD(wParameter) == LOWORD(1202)) {
			DoFill = true;
			hThread3 = CreateThread(NULL, 0, Thread3, hWindow, 0, NULL);
			return 0;
		}
	}
	return 0;
	case WM_KEYDOWN:
	{
		switch (wParameter)
		{
		case VK_BACK:
		{
			counter--; //уменьшается число отрисовываемых символов (стертый символ остается в массиве ,но не будет отрисован,а новый символ заменит его)
			hThread2 = CreateThread(NULL, 0, Thread2, hWindow, 0, NULL); // поток который отрисовывает текст 
		}
		case VK_RETURN:
			return 0;
		default:
			text[counter] = (char)wParameter;
			counter++;   //увеличивается число отрисовываемых символов
			hThread2 = CreateThread(NULL, 0, Thread2, hWindow, 0, NULL);

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
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); //изначальный цвет окна 
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
	hwnd = CreateWindow(wc.lpszClassName, L"Lab_4",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX /*запрет на изменение размера окна*/
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

