
#pragma once
 //FishBot.cpp : �w�q�D���x���ε{�����i�J�I�C


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
HANDLE hProcess = 0; //���}�i�{���o�v��
HWND hWnd = 0;
DWORD PID = 0;
HWND hw = 0;
DWORD battle;

RECT AB ={ 0,0,800, 600 };

PROCESSENTRY32 __gameProcess;
DWORD __dwordEngine;


int GetVirtualKey(CHAR mKey, INT caps);//mKye = ����Acaps = 0���U1�u�_

int getmu()
{

	POINT p, p2;
	GetCursorPos(&p);  // �ѪR�׮y��
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
		hw = FindWindow(NULL, L"Trove BOT"); //�������WND
		//SetParent(hWnd, hw); //�n�r �i�H���{�����ڪ����f��
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

		//SendMessage(hWnd,/*WM_NCACTIVATE | */WM_ACTIVATE  ,0 , 0 ); //�L��
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
		//--�T�|
		hWnd = FindWindow(NULL, L"Trove");
		GetWindowThreadProcessId(hWnd, &PID);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)PID);


		runSetDebugPrivs();
		while (!FindProcessName("Trove.exe", &__gameProcess))
		{
			Sleep(12);
			if (FindWindow(NULL, L"Trove") == 0) FindGame();
		}
		//printf("exe��m = %0X\n", __gameProcess);
		while (__dwordEngine == 0x0)
		{
			__dwordEngine = GetModuleNamePointer("trove.exe", __gameProcess.th32ProcessID);
			if (FindWindow(NULL, L"Trove") == 0) FindGame();
		}
		//printf("dll��m = %0X\n", __dwordEngine);
		//---�p���m
		DWORD Fish_address_val = Fish_address1 + __dwordEngine; //�p���}

		//--�T�|����

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
	hWnd = FindWindowEx(NULL, hWnd,NULL, L"Trove");  //�h���f
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
		int VK_F_UP = GetVirtualKey('F', 1);//����A0���U1�u�_

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
