#pragma once

#include <Windows.h>
//#include "atlstr.h"
#include <TlHelp32.h>

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
		//printf("¼Ò²Õ¡G%s\n", nstring);
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

		//printf("°»´ú¨ì¡G%s\n", nstring);

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