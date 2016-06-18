// FishBot.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <Windows.h>
#include "atlstr.h"
#include <TlHelp32.h>


int Fish_address0 = 0xA7DB84;	
int Fish_address1 = 0xA7DB84;
int battle_offset = 0x324;
int Fish_offset1 = 0x1BC;
int Fish_offset2 = 0x22C;
int Fish_magma_offset = +0x4BC;
int Fish_candy_offset = +0x25C;
int Fish_Water;
int Fish_magma;
int Fish_candy;

PROCESSENTRY32 __gameProcess;
DWORD __dwordEngine;
PROCESSENTRY32 __gameProcess1;
DWORD __dwordEngine1;
void rest()
{
	Fish_address1 = Fish_address0;
	__gameProcess = __gameProcess1;
	__dwordEngine = __dwordEngine1;
}


bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}
DWORD FindPattern(DWORD dValor, DWORD dLer, BYTE *bMaskara, char * szMaskara)
{
	for (DWORD i = 0; i < dLer; i++)
		if (bCompare((PBYTE)(dValor + i), bMaskara, szMaskara))
			return (DWORD)(dValor + i);
	return false;
}


DWORD GetModuleNamePointer(LPSTR LPSTRModuleName, DWORD __DwordProcessId)
{
	MODULEENTRY32 lpModuleEntry = { 0 };
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, __DwordProcessId);
	int error1 = GetLastError();

	if (!hSnapShot)
		return NULL;
	lpModuleEntry.dwSize = sizeof(lpModuleEntry);
	BOOL __RunModule = Module32First(hSnapShot, &lpModuleEntry);
	while (__RunModule)
	{

		//*TEXT  WCHAR* TO CHAR*
		size_t origsize = wcslen(lpModuleEntry.szModule) + 1;
		size_t convertedChars = 0;
		char strConcat[] = " (char *)";
		size_t strConcatsize = (strlen(strConcat) + 1) * 2;
		const size_t newsize = origsize * 2;
		char *nstring = new char[newsize + strConcatsize];
		wcstombs_s(&convertedChars, nstring, newsize, lpModuleEntry.szModule, _TRUNCATE);

		//*TEXT  WCHAR* TO CHAR*
		//printf("模組：%s\n", nstring);
		//if (!strcmp(lpModuleEntry.szModule, LPSTRModuleName))
		if (!strcmp(nstring, LPSTRModuleName))
		{
			CloseHandle(hSnapShot);
			return (DWORD)lpModuleEntry.modBaseAddr;
		}
		__RunModule = Module32Next(hSnapShot, &lpModuleEntry);
	}
	CloseHandle(hSnapShot);
	return NULL;
}
DWORD FindProcessName(const char *__ProcessName, PROCESSENTRY32 *pEntry)
{
	PROCESSENTRY32 __ProcessEntry;
	__ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
	if (!Process32First(hSnapshot, &__ProcessEntry))
	{
		CloseHandle(hSnapshot);
		return 0;
	}
	do {


		//*TEXT  WCHAR* TO CHAR*
		size_t origsize = wcslen(__ProcessEntry.szExeFile) + 1;
		size_t convertedChars = 0;
		char strConcat[] = " (char *)";
		size_t strConcatsize = (strlen(strConcat) + 1) * 2;
		const size_t newsize = origsize * 2;
		char *nstring = new char[newsize + strConcatsize];
		wcstombs_s(&convertedChars, nstring, newsize, __ProcessEntry.szExeFile, _TRUNCATE);

		//*TEXT  WCHAR* TO CHAR*

		//char *aaa = (__ProcessEntry.szExeFile);

		//printf("偵測到：%s\n", nstring);

		if (!_strcmpi(nstring, __ProcessName))
			//if (!_strcmpi(__ProcessEntry.szExeFile, __ProcessName))
		{
			memcpy((void *)pEntry, (void *)&__ProcessEntry, sizeof(PROCESSENTRY32));
			CloseHandle(hSnapshot);
			return __ProcessEntry.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &__ProcessEntry));
	CloseHandle(hSnapshot);
	return 0;
}
void runSetDebugPrivs()
{
	HANDLE __HandleProcess = GetCurrentProcess(), __HandleToken;
	TOKEN_PRIVILEGES priv;
	LUID __LUID;
	OpenProcessToken(__HandleProcess, TOKEN_ADJUST_PRIVILEGES, &__HandleToken);
	LookupPrivilegeValue(0, L"seDebugPrivilege", &__LUID);
	priv.PrivilegeCount = 1;
	priv.Privileges[0].Luid = __LUID;
	priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(__HandleToken, false, &priv, 0, 0, 0);
	CloseHandle(__HandleToken);
	CloseHandle(__HandleProcess);
}

int GetVirtualKey(CHAR mKey, INT caps);//mKye = 按鍵，caps = 0按下1彈起

int main()
{
	system("CLS");	//清除
	printf("尋找遊戲中...\n");
	rest();
	//---找到模塊起始點
	runSetDebugPrivs();
	while (!FindProcessName("Trove.exe", &__gameProcess)) 
	{
		Sleep(12);	
		if (FindWindow(NULL, L"Trove") == 0) main();
	}
	printf("exe位置 = %0X\n", __gameProcess);
	while (__dwordEngine == 0x0) 
	{
		__dwordEngine = GetModuleNamePointer("trove.exe", __gameProcess.th32ProcessID);
		if (FindWindow(NULL, L"Trove") == 0) main();
	}
	printf("dll位置 = %0X\n", __dwordEngine);
	//---計算位置
	Fish_address1 += __dwordEngine; //計算基址

	
	HWND hWnd = FindWindow(NULL,L"Trove");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE,(DWORD)__gameProcess.th32ProcessID);	
	DWORD tmp , dwNumberOfBytesRead;
	ReadProcessMemory(hProcess, (LPCVOID)Fish_address1, &tmp, 4, &dwNumberOfBytesRead);
	DWORD battle = tmp + 324;
	ReadProcessMemory(hProcess, (LPCVOID)(tmp + Fish_offset1), &tmp, 4, &dwNumberOfBytesRead);
	//ReadProcessMemory(hProcess, (LPCVOID)(tmp + Fish_offset2), &tmp, 4, &dwNumberOfBytesRead);;
	tmp += Fish_offset2;

	//---按鍵
	//Sleep(5000);
	int VK_F_DOWN = GetVirtualKey('F',0);
	int VK_F_UP = GetVirtualKey('F', 1);

	int Fish_Count = 0 , Fish_water_OK=0 , Fish_magma_OK = 0, Fish_candy_OK = 0;

	Fish_Water = tmp;
	Fish_magma = Fish_magma_offset + Fish_Water;
	Fish_candy = Fish_candy_offset + Fish_Water;
	//system("CLS");	//清除
	printf("已找到遊戲%X\n", Fish_address1);
	printf("=============Trove自動釣魚程式============= \n");
	printf("使用方法：直接開始釣魚即會自動上鉤、拋竿。\n");
	printf("By ak1254664\n" );
	printf("Water :%X\n" , Fish_Water);
	printf("Magma :%X\n", Fish_magma);
	printf("Candy :%X\n", Fish_candy);
	printf("===========正確讀取，可開始釣魚=========== \n\n");

	/*
	int rfffhg = 0xffff;
	BYTE CY[0xFFFF];
	BYTE ST[20];
	DWORD oldProtect;
	DWORD dwDXDevice = 0;
	DWORD coun=0;
	while (dwDXDevice == 0 )
	{
		//if (VirtualProtect((LPVOID)(__dwordEngine+ 0x140000), 0xFFFF, PAGE_EXECUTE_READWRITE, &oldProtect))
		ReadProcessMemory(hProcess, (LPCVOID)(__dwordEngine + coun), CY, 0XFFFF, NULL);

		dwDXDevice = FindPattern((DWORD)(CY), 0XFFFF, (PBYTE)"\x00\x8B\x46\x10\x3B\x73\x14\x75\x05\x89\x43\x14\xEB\x03\x89\x47\x10\xFF\x75\xFC\x8B\xF0\xFF\x35", "xxxxxxxxxxxxxxxxxxxxxxxx");
		coun += 0xffff;
	}

	printf("%X", dwDXDevice);

	*/

	while (1)
	{
		WORD text;
		ReadProcessMemory(hProcess, (LPCVOID)(Fish_Water+0xE), &text, 4, &dwNumberOfBytesRead);

		if (text != 0x447A || FindWindow(NULL, L"Trove") == 0) main();	//檢查遊戲



		ReadProcessMemory(hProcess, (LPCVOID)Fish_Water , &Fish_water_OK, 4, &dwNumberOfBytesRead);
		ReadProcessMemory(hProcess, (LPCVOID)Fish_magma, &Fish_magma_OK, 4, &dwNumberOfBytesRead);
		ReadProcessMemory(hProcess, (LPCVOID)Fish_candy, &Fish_candy_OK, 4, &dwNumberOfBytesRead);
		if (Fish_water_OK == 1 || Fish_magma_OK==1 || Fish_candy_OK == 1 )
		{
			//按鍵，0按下1彈起
			SendMessage(hWnd, WM_KEYDOWN, toascii('F'), VK_F_DOWN);
			SendMessage(hWnd, WM_KEYUP, toascii('F'), VK_F_UP);

			Fish_Count += 1;
			printf("累積：%d\n",Fish_Count);
			while (Fish_water_OK == 1 || Fish_magma_OK == 1 || Fish_candy_OK ==1)
			{
				ReadProcessMemory(hProcess, (LPCVOID)Fish_Water, &Fish_water_OK, 4, &dwNumberOfBytesRead);
				ReadProcessMemory(hProcess, (LPCVOID)(Fish_magma), &Fish_magma_OK, 4, &dwNumberOfBytesRead);
				ReadProcessMemory(hProcess, (LPCVOID)Fish_candy, &Fish_candy_OK, 4, &dwNumberOfBytesRead);
			}
			;
			Sleep(1000);
			SendMessage(hWnd, WM_KEYDOWN, toascii('F'), VK_F_DOWN);
			SendMessage(hWnd, WM_KEYUP, toascii('F'), VK_F_UP);
		}

	}

	getchar();
    return 0;
}

int GetVirtualKey(CHAR mKey,INT caps)
{		
	int key_lParam ;
	if ( caps == 0 )
	{
		key_lParam = 0x00000000;
	}
	else if ( caps == 1 )
	{
		key_lParam = 0xC0000000;
	}
	key_lParam += (MapVirtualKey(mKey, 0) * 0x10000);
	key_lParam += 1;	
	return key_lParam;
}
