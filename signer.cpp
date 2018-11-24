#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
// #include "stdafx.h"
#include <unistd.h>
#include <windows.h>
#include <tlhelp32.h>

#include "utils.h"
#include "structure.h"

using namespace std;

bool FindProcessId(string exe_path) {
	string process_name = exe_path.substr(exe_path.find_last_of("\\") + 1);
	cout << process_name;
 	
 	PROCESSENTRY32 processInfo;
 	processInfo.dwSize = sizeof(processInfo);

  	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  	if (processesSnapshot == INVALID_HANDLE_VALUE)
    	return false;

	Process32First(processesSnapshot, &processInfo);
	if (!process_name.compare(processInfo.szExeFile)) {
    	CloseHandle(processesSnapshot);
    	return true;
  	}

  	while (Process32Next(processesSnapshot, &processInfo)) {
    	if (!process_name.compare(processInfo.szExeFile)) {
      		CloseHandle(processesSnapshot);
      		return true;
    	}
  	}

  	CloseHandle(processesSnapshot);
  	return false;
}

void wait(string exe_path) {
	while (FindProcessId(exe_path)) {
		wcout << L"ЛОГ: ждём пока закроется exe\n"; 
		sleep(3);
	}
	wcout << L"ЛОГ: exe остановлено\n";
}


int main(int argc, char** argv) {
	setlocale(0, "");

	ifstream f(argv[0], std::ios::binary);
	// считываем путь до exe
	char exe_path[MAX_PATH_SIZE]; // = "a.exe";
	f.seekg(-MAX_PATH_SIZE, std::ios::end);
	f.read(exe_path, MAX_PATH_SIZE);
	// читаем окружение
	f.seekg(-MAX_PATH_SIZE - STRUCT_SIZE, std::ios::end);
	ENVIRON* env = new ENVIRON();
	read_struct(env, &f);
	// закрываем файл
	f.close();

	// wcout << L"ЛОГ: считанный путь: "; cout << exe_path << endl;
	// wait(string(exe_path));
	sleep(5);
	
	ifstream exe(exe_path, std::ios::binary);
	// читаем сигнатуру
	char signature[SIG_LEN];
	exe.seekg(-SIG_LEN, std::ios::end);
	exe.read(signature, SIG_LEN);
	exe.close();

	string s_signature(signature);
	// wcout << L"Считанная сигнатура: " << wstring(s_signature.begin(), s_signature.end()) << L'\n';

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
	}
	
	return retval;
}