#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <ShellApi.h>
#include <unistd.h>

#include "utils.h"
#include "strsafe.h"
#include "structure.h"

using namespace std;

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")


void del_me()
{
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    GetModuleFileName(NULL, szModuleName, MAX_PATH);

    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}


int main(int argc, char** argv) {
	ofstream kk("D:\\kek.txt");

	setlocale(0, "");

	ifstream f(argv[0], std::ios::binary);
	// считываем путь до exe
	char exe_path[MAX_PATH_SIZE]; 
	f.seekg(-MAX_PATH_SIZE, std::ios::end);
	f.read(exe_path, MAX_PATH_SIZE);
	// читаем окружение
	f.seekg(-MAX_PATH_SIZE - STRUCT_SIZE, std::ios::end);
	ENVIRON* env = new ENVIRON();
	read_struct(env, &f);
	// закрываем файл
	f.close();

	wcout << L"ЛОГ: считанный путь: "; cout << exe_path << endl;
	sleep(2);
	
	ifstream exe(exe_path, std::ios::binary);
	// читаем сигнатуру
	char signature[SIG_LEN];
	exe.seekg(-SIG_LEN, std::ios::end);
	exe.read(signature, SIG_LEN);
	exe.close();

	string s_signature(signature);

	int retval = 0;
	if (strcmp(signature, UNCHECKED) != 0) {
		wcout << L"ОШИБКА: ПАТЧЕР: сигнатура в exe не установлена\n";
		retval = 1;
	} else {
		ofstream exe(exe_path, std::ios::binary | std::ios::app);
		// записываем окружение
		write_struct(env, &exe);
		// записываем сигнатуру
		exe.write(CHECKED, SIG_LEN);
		exe.close();
		sleep(2);

		wcout << L"ЛОГ: Перезапускаем приложение\n";
		int a = (int) ShellExecuteA(GetDesktopWindow(), "open", exe_path, NULL, NULL, SW_SHOW);
	}
	
	del_me();
	return retval;
}