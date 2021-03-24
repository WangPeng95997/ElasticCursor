#include <Windows.h>
#define MAXSPEED 32

int g_speed = 4;
BOOLEAN g_bFall = TRUE;

// 随机数函数
HCRYPTPROV hProv;
int Random()
{
	if (hProv == 0)
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			return 0;

	int out = 0;
	CryptGenRandom(hProv, sizeof(out), (BYTE*)(&out));

	return out & 0x7FFFFFFF;
}

// 鼠标移动速度会随着时间增加越来越快
DWORD WINAPI Speedup(LPVOID lpParameter)
{
	while (g_bFall)
	{
		g_speed++;
		Sleep(750);
	}

	while (g_speed < MAXSPEED)
	{
		g_speed++;
		Sleep(3000);
	}

	return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	POINT cursor;
	ZeroMemory(&cursor, sizeof(POINT));
	HANDLE hThread = CreateThread(NULL, 0, Speedup, NULL, 0, NULL);

	while (g_bFall)
	{
		GetCursorPos(&cursor);
		SetCursorPos(cursor.x, cursor.y + g_speed);
		GetCursorPos(&cursor);

		if (GetSystemMetrics(SM_CYSCREEN) - 1 <= cursor.y)
			g_bFall = FALSE;
		Sleep(20);
	}
	g_speed = g_speed + g_speed / 6;

	int x = (Random() % 2) ? -g_speed : g_speed;
	int y = -g_speed;
	for (;;)
	{
		GetCursorPos(&cursor);
		SetCursorPos(cursor.x + x, cursor.y + y);
		GetCursorPos(&cursor);

		if (cursor.x == 0)
			x = g_speed;
		else if (cursor.y == 0)
			y = (int)(g_speed * 0.5625);
		else if (GetSystemMetrics(SM_CXSCREEN) - 1 <= cursor.x)
			x = -g_speed;
		else if (GetSystemMetrics(SM_CYSCREEN) - 1 <= cursor.y)
			y = (int)(-g_speed * 0.5625);
		Sleep(20);
	}
	CloseHandle(hThread);

	return 0;
}