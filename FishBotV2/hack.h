
#pragma once
 //FishBot.cpp : 定義主控台應用程式的進入點。


#include <stdarg.h>
#include <Windows.h>
#include "find.h"

int Fish_address1 = 0x00A732CC; // 0xD8FBE0; // ;
int battle_offset = 0x324;
int Fish_offset1 = 0x1BC;
int Fish_offset2 = 0x22C;
int Fish_magma_offset = +0x4BC;
int Fish_candy_offset = +0x25C;
int Fish_Water = 0;
int Fish_magma = 0;
int Fish_candy = 0;
HANDLE hProcess = 0; //打開進程取得權限
HWND hWnd = 0;
DWORD PID = 0;
HWND hw = 0;
DWORD battle;

RECT AB ={ 0,0,800, 600 };

PROCESSENTRY32 __gameProcess;
DWORD __dwordEngine;


int GetVirtualKey(CHAR mKey, INT caps);//mKye = 按鍵，caps = 0按下1彈起

int getmu()
{

	POINT p, p2;
	GetCursorPos(&p);  // 解析度座標
	ScreenToClient(hWnd, &p2); //

	CURSORINFO cursorInfo = { 0 };
	cursorInfo.cbSize = sizeof(cursorInfo);
	 GetCursorInfo(&cursorInfo);

	return p2.x;

}

void battle_go()
{
	
	RECT lpRect;
	GetWindowRect(hWnd, &lpRect);
	if (  ( lpRect.right -  lpRect.left) != AB.right || (lpRect.bottom -lpRect.top) != AB.bottom)
	{
		hw = FindWindow(NULL, L"Trove BOT"); //獲取本身WND
		//SetParent(hWnd, hw); //好猛 可以讓程式塞到我的窗口裡
		SetWindowPos(hWnd, 0, lpRect.left, lpRect.top, 800, 600, 4);
	}

	DWORD battle_ok;
	ReadProcessMemory(hProcess, (LPCVOID)(battle), &battle_ok, 4, NULL);

	if (battle_ok == 3349503358)
	{
		/*
		SwitchToThisWindow(hWnd, true);
		SetActiveWindow(hWnd);
		SetFocus(hWnd);
		UpdateWindow(hWnd);

		Sleep(500);
		*/
	//	mouse_event(MOUSEEVENTF_LEFTDOWN,0, 0, 0, 0);
	//	Sleep(64);
	//	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		//SendMessage(hWnd,/*WM_NCACTIVATE | */WM_ACTIVATE  ,0 , 0 ); //無效
		LPARAM LParam = MAKELPARAM(0x22d , 0x1a0);
		SendMessage(hWnd, WM_MOUSEMOVE, 0, LParam);

		SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, LParam);

		Sleep(64);
		SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, LParam);

		Sleep(100);

		LPARAM LParam2 = MAKELPARAM(0x22d, 0x19B);
		SendMessage(hWnd, WM_MOUSEMOVE, 0, LParam2);

		SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, LParam2);

		Sleep(64);
		SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, LParam2);



		 LParam2 = MAKELPARAM(0x22d, 0x1a5);
		SendMessage(hWnd, WM_MOUSEMOVE, 0, LParam2);

		SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, LParam2);

		Sleep(64);
		SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, LParam2);

	
	}
	//mouse_event(0x8000 | 0x0001, 65535,65535,0 ,0);

}


int FindGame()
{

	WORD text = 0;
	ReadProcessMemory(hProcess, (LPCVOID)(Fish_Water + 0xE), &text, 4, NULL);

	if (text != 0x447A )
	{
		//--枚舉
		hWnd = FindWindow(NULL, L"Trove");
		GetWindowThreadProcessId(hWnd, &PID);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)PID);


		runSetDebugPrivs();
		while (!FindProcessName("Trove.exe", &__gameProcess))
		{
			Sleep(12);
			if (FindWindow(NULL, L"Trove") == 0) FindGame();
		}
		//printf("exe位置 = %0X\n", __gameProcess);
		while (__dwordEngine == 0x0)
		{
			__dwordEngine = GetModuleNamePointer("trove.exe", __gameProcess.th32ProcessID);
			if (FindWindow(NULL, L"Trove") == 0) FindGame();
		}
		//printf("dll位置 = %0X\n", __dwordEngine);
		//---計算位置
		DWORD Fish_address_val = Fish_address1 + __dwordEngine; //計算基址

		//--枚舉結束

		DWORD tmp, dwNumberOfBytesRead;
		ReadProcessMemory(hProcess, (LPCVOID)Fish_address_val, &tmp, 4, &dwNumberOfBytesRead);
		battle = tmp + battle_offset;
		ReadProcessMemory(hProcess, (LPCVOID)(tmp+Fish_offset1), &tmp, 4, &dwNumberOfBytesRead);

		//ReadProcessMemory(hProcess, (LPCVOID)(tmp + Fish_offset2), &tmp, 4, &dwNumberOfBytesRead);
		//ReadProcessMemory(hProcess, (LPCVOID)(tmp + Fish_offset2), &tmp, 4, &dwNumberOfBytesRead);;
		tmp += Fish_offset2; 
		Fish_Water = tmp;
		Fish_magma = Fish_magma_offset + Fish_Water;
		Fish_candy = Fish_candy_offset + Fish_Water;

		return PID;
	}

	/*
	while (hWnd)
	{
	hWnd = FindWindowEx(NULL, hWnd,NULL, L"Trove");  //多窗口
	}
	*/
	return PID;
}


void fish_go()
{

	int Fish_Count = 0, Fish_water_OK = 0, Fish_magma_OK = 0, Fish_candy_OK = 0;

	ReadProcessMemory(hProcess, (LPCVOID)Fish_Water, &Fish_water_OK, 4, NULL);
	ReadProcessMemory(hProcess, (LPCVOID)Fish_magma, &Fish_magma_OK, 4, NULL);
	ReadProcessMemory(hProcess, (LPCVOID)Fish_candy, &Fish_candy_OK, 4, NULL);
		
	if (Fish_water_OK == 1 || Fish_magma_OK == 1 || Fish_candy_OK == 1)
	{
		
		int VK_F_DOWN = GetVirtualKey('F', 0);
		int VK_F_UP = GetVirtualKey('F', 1);//按鍵，0按下1彈起

		PostMessage(hWnd, WM_KEYDOWN, toascii('F'), VK_F_DOWN);
		Sleep(64);
		PostMessage(hWnd, WM_KEYUP, toascii('F'), VK_F_UP);

		Fish_Count += 1;
		
		while (Fish_water_OK == 1 || Fish_magma_OK == 1 || Fish_candy_OK == 1)
		
		{
		
			ReadProcessMemory(hProcess, (LPCVOID)Fish_Water, &Fish_water_OK, 4, NULL);
			ReadProcessMemory(hProcess, (LPCVOID)(Fish_magma), &Fish_magma_OK, 4, NULL);
			ReadProcessMemory(hProcess, (LPCVOID)Fish_candy, &Fish_candy_OK, 4, NULL);
			
		}
		Sleep(1064);
		PostMessage(hWnd, WM_KEYDOWN, toascii('F'), VK_F_DOWN);
		Sleep(64);
		PostMessage(hWnd, WM_KEYUP, toascii('F'), VK_F_UP);

	}
}

int GetVirtualKey(CHAR mKey, INT caps)
{
	int key_lParam;
	if (caps == 0)
	{
		key_lParam = 0x00000000;
	}
	else if (caps == 1)
	{
		key_lParam = 0xC0000000;
	}
	key_lParam += (MapVirtualKey(mKey, 0) * 0x10000);
	key_lParam += 1;
	return key_lParam;
}
