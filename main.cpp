#include <Windows.h>
#include <direct.h>
#include <fstream>
#include <string>
#include <iostream>

MSG message;
const int ID_BUTTON = 2137;
bool inloop = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == ID_BUTTON)
	{
		inloop = false;
	}
	switch (msg)
	{
	case WM_CLOSE:
		exit(0);

	default:
		DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int main()
{
	std::string defaultPath;
	std::ifstream istr;
	istr.open("default.txt");
	std::getline(istr, defaultPath);
	istr.close();


	ShowWindow(GetConsoleWindow(), SW_HIDE);
	std::cout << defaultPath;
	HMODULE instance = GetModuleHandle(0);


	LPSTR className = const_cast<char*>("regWindow"); //chuje muje

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0; //???
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = 0;
	wc.lpszClassName = className;
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, "Nie uda³o siê", "Chuj", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	int windowWidth = 800;
	int windowHeight = 200;
	int windowX = 400;
	int windowY = 300;
	HWND window = CreateWindowEx(WS_EX_WINDOWEDGE, className, "Minix Runner", WS_BORDER | WS_SYSMENU,
		windowX, windowY, windowWidth, windowHeight, 0, 0, instance, 0);

	if (window == 0)
	{
		MessageBox(0, "Znowu siê zesra³o", "dupa", MB_OK);
		return 2;
	}

	HWND okButton = CreateWindowEx(0, "BUTTON", "OK", WS_CHILD | WS_VISIBLE,
		windowWidth / 2 - 50, windowHeight - 80, 50, 30, window, (HMENU)ID_BUTTON, instance, 0); //wartoœci z dupy (eksperymentalne)


	HWND editField = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT",
		defaultPath.data(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		60, 20, windowWidth - 100, 20, window, 0, instance, 0);


	HWND etiquette = CreateWindowEx(0, "STATIC", 0, WS_CHILD | WS_VISIBLE | SS_LEFT,
		0, 20, 50, 20, window, 0, instance, 0);
	SetWindowText(etiquette, "path:");

	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);
	ShowWindow(okButton, SW_SHOW);
	ShowWindow(editField, SW_SHOW);
	ShowWindow(etiquette, SW_SHOW);

	while (GetMessage(&message, 0, 0, 0) && inloop)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	ShowWindow(window, SW_HIDE);

	DWORD pathLength = GetWindowTextLength(editField);
	LPSTR path = (LPSTR)GlobalAlloc(GPTR, pathLength);
	GetWindowText(editField, path, pathLength + 1);

	std::ofstream ostr; 
	ostr.open("default.txt", std::fstream::trunc);
	defaultPath = path;
	std::cout << defaultPath;
	ostr.write(path, pathLength);
	ostr.close();

	_chdir(path);

	const char* instruction = "qemu-system-i386 -net nic -m 256 minix203.img";
	system(instruction);

	GlobalFree(path);
}