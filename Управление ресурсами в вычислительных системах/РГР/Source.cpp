#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#include <windows.h>
#include <string>
#include <sstream>
#include <tchar.h>


HWND hwnd, button, button_exit, screenheightLabel;
static HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));

void thread()
{

	HINSTANCE hinstLib = LoadLibrary(TEXT("info.dll"));//загрузка динамической библиотеки
	if (hinstLib != NULL)
	{
		typedef int (*getSystemMetrics_t)(); 
		getSystemMetrics_t getSystemMetrics = (getSystemMetrics_t)GetProcAddress(hinstLib, "getSystemMetrics");
		int hight = getSystemMetrics();
		FreeLibrary(hinstLib);//Закрытие библиотеки
		char screenhight[1024];
		sprintf_s(screenhight, "Main task:\n\"Determine screen height\"\
\n Screen heights %d pixels", hight);
		WCHAR screenhightWString[1024];
		MultiByteToWideChar(CP_ACP, 0, screenhight, strlen(screenhight) + 1, (LPWSTR)screenhightWString, sizeof(screenhightWString) / sizeof(screenhightWString[0]));
		// И выведем в окно
		SetWindowText(screenheightLabel, LPWSTR(screenhightWString));
	}
	else
	{
		MessageBox(hwnd, L"Error loading DLL", L"Error", 0);//  вывод ошибки
	}
}

// Обработчик сообщений
LONG WINAPI WndProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
	switch (Message)
	{
	case WM_CTLCOLORSTATIC:
	{
		DWORD CtrlID = GetDlgCtrlID((HWND)lparam);
		if (CtrlID == 3001)
		{
			
			HDC hdcStatic = (HDC)wparam;
			SetTextColor(hdcStatic, RGB(0, 255, 0));// установка цвета текста
			SetBkColor(hdcStatic, RGB(0, 0, 255));// установка  цвета обводки текста
			return (INT_PTR)hBrush;
		}
	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(wparam) == 3000)
		{
	
			HANDLE hThread;//Дескриптор потока
			DWORD IDThread;//Идентификатор потока
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL, 0, &IDThread); // Запуск  потока
			CloseHandle(hThread);// Удаление дескриптора потока
		}
		if (LOWORD(wparam) == 3002)
		{

			PostQuitMessage(0);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, Message, wparam, lparam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,	LPSTR	 lpCmdLine,	int	   nCmdShow)
{
	WNDCLASS w; //Элемент структуры класса
	memset(&w, 0, sizeof(WNDCLASS));
	w.style = CS_HREDRAW | CS_VREDRAW| CS_BYTEALIGNCLIENT;//Стиль  окна
	w.lpfnWndProc = WndProc;			// Обработчик сообщений
	w.hInstance = hInstance; //Дескриптор  приложения
	w.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
	w.lpszClassName = L"window";//Имя  окна
	RegisterClass(&w); //Регистрация класса окна
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	HFONT hf = CreateFontIndirect(&ncm.lfMessageFont);
	HDC hDCScreen = GetDC(NULL);
	int Horres = GetDeviceCaps(hDCScreen, HORZRES);
	int Vertres = GetDeviceCaps(hDCScreen, VERTRES);
	int Height = 220;
	int width = 460;
	int ButtonHeight = 36;
	int Buttonwidth = 75;

	hwnd = CreateWindow(L"window", L"Writer screen height", \
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, \
		(Horres - width) / 2, (Vertres - Height) / 2, width, Height, NULL, NULL, hInstance, NULL);

	 CreateWindow(L"button", L"GOGO", \
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_VCENTER | BS_CENTER, width - 2 * Buttonwidth - 50, Height - 2 * ButtonHeight - 7, \
		Buttonwidth, ButtonHeight, hwnd, (HMENU)3000, hInstance, NULL);

	button_exit = CreateWindow(L"button", L"EXIT", \
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_VCENTER | BS_CENTER, width - Buttonwidth - 40, Height - 2 * ButtonHeight - 7, \
		Buttonwidth, ButtonHeight, hwnd, (HMENU)3002, hInstance, NULL);

	screenheightLabel = CreateWindow(L"static", L"", WS_CHILD | WS_VISIBLE  | BS_PUSHBUTTON , \
		0, 0, width, Height / 2 - ButtonHeight , hwnd, (HMENU)3001, hInstance, NULL);

	for (int i = 0; i < 4; i++)
		SendDlgItemMessage(hwnd, 3000 + i, WM_SETFONT, (WPARAM)hf, TRUE);

	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
