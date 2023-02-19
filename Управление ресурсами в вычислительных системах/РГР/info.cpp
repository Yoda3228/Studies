#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#include <stdio.h>
#include <windows.h>
#include <time.h>

// Определение высоты экрана в пикселях
extern "C" __declspec(dllexport)
int getSystemMetrics()
{

	return GetSystemMetrics(SM_CYSCREEN);
}

